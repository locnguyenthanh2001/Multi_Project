#include "config.h"
#include <OneWire.h>
#include <DallasTemperature.h>

//Define Input pin for each sensor
#define TdsSensorPin 36
#define HumiSensorPin 34
#define TempSensorPin 17

//Define variables for the task scheduler

#define SCH_MAX_TASKS 40 //max size of the scheduler
#define NO_TASK_ID 0

int size = 0; //size of the scheduler

//Define variales for calculating and buffering of the TDs sensor
#define VREF 5.0
#define SCOUNT  30

//Declare buffer fo calculating TDs value
int analogBuffer[SCOUNT];
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0, copyIndex = 0;
float averageVoltage = 0;


int tdsValue = 0 , temperature = 25, humiValue = 0;

//Declare variable to connect with AdafruitIO feeds
AdafruitIO_Feed *tds = io.feed("tds");
AdafruitIO_Feed *humid = io.feed("humid");
AdafruitIO_Feed *temp = io.feed("temp");

//Declare varibale for using temperature sensor
OneWire oneWire(TempSensorPin);
DallasTemperature sensors(&oneWire);


//Define struct scheduler
typedef struct {
  // Pointer to the task (must be a ’ void ( void ) ’ function )
  void ( * pTask) ( void ) ;
  // Delay ( ti ck s ) until the function will ( next ) be run
  uint32_t Delay ;
  // Interval ( ti ck s ) between subsequent runs .
  uint32_t Period ;
  // Incremented ( by scheduler ) when task is due to execute
  uint8_t RunMe;
  // This i s a hint to solve the question below .
  uint32_t TaskID ;
} sTask ;
sTask SCH_tasks_G[SCH_MAX_TASKS] ;



//Declare all functions we need for this project

//Scheduler's funtions
void SCH_Init(void);
int SCH_Add_Task( void (* pFunction)() , unsigned int DELAY, unsigned int PERIOD);
void SCH_Dispatch_Tasks(void);
void SCH_Delete_Task(const uint32_t TASK_INDEX);
void SCH_Update( void );
//void printSchedule();

//TDs's functions
void readTDS();
int getMedianNum(int bArray[], int iFilterLen) ;
void sendTDS();

//Humidity's function
void read_sendHumid();

//Temperature's function
void read_sentTemp();




//Declare the Timer Interrupt
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;



void IRAM_ATTR onTimer() {
  SCH_Update();
}




void setup() {
  pinMode(TdsSensorPin, INPUT);
  pinMode(HumiSensorPin, INPUT);
  pinMode(TempSensorPin, INPUT);
  Serial.begin(115200);
  sensors.begin();
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 10000, true);
  timerAlarmEnable(timer);
  SCH_Init();
  SCH_Add_Task(read_sendHumid , 0, 700);
  SCH_Add_Task(read_sentTemp, 100, 1200);
  SCH_Add_Task(sendTDS, 200, 2000);
  io.connect();
  Serial.print("Connecting to Adafruit IO");
  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  tds->get();
  humid->get();
  temp->get();
}



void loop() {
  readTDS();
  io.run();
  SCH_Dispatch_Tasks();
}




void sendTDS() {
//  Serial.print("voltage:");
//  Serial.print(averageVoltage, 2);
//  Serial.print("V   ");
  Serial.print("TDS Value:");
  Serial.print(tdsValue, 0);
  Serial.println("ppm");
  Serial.print("sending TDs -> "); Serial.println();
  tds->save(tdsValue, 0);
}

void read_sentTemp() {
  sensors.requestTemperatures();
  temperature = sensors.getTempCByIndex(0);
  Serial.print("Temperature: ");
  Serial.println(temperature, 0);
  Serial.print("sending temperature -> "); Serial.println();
  temp->save(temperature, 0);
}

void readTDS()
{
  static unsigned long analogSampleTimepoint = millis();
  if (millis() - analogSampleTimepoint > 40U)  //every 40 milliseconds,read the analog value from the ADC
  {
    analogSampleTimepoint = millis();
    analogBuffer[analogBufferIndex] = analogRead(TdsSensorPin);  //read the analog value and store into the buffer
    analogBufferIndex++;
    if (analogBufferIndex == SCOUNT)
      analogBufferIndex = 0;
  }
  static unsigned long printTimepoint = millis();
  if (millis() - printTimepoint > 800U)
  {
    printTimepoint = millis();
    for (copyIndex = 0; copyIndex < SCOUNT; copyIndex++)
      analogBufferTemp[copyIndex] = analogBuffer[copyIndex];
    averageVoltage = getMedianNum(analogBufferTemp, SCOUNT) * (float)VREF / 1024.0; // read the analog value more stable by the median filtering algorithm, and convert to voltage value
    float compensationCoefficient = 1.0 + 0.02 * (temperature - 25.0); //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
    float compensationVolatge = averageVoltage / compensationCoefficient; //temperature compensation
    tdsValue = (133.42 * compensationVolatge * compensationVolatge * compensationVolatge - 255.86 * compensationVolatge * compensationVolatge + 857.39 * compensationVolatge) * 0.5; //convert voltage value to tds value
  }
}
void read_sendHumid() {
  humiValue = analogRead(HumiSensorPin);
  Serial.print("Humidity Value:");
  Serial.println(humiValue, 0);
  Serial.print("sending humi -> "); Serial.println();
  humid->save(humiValue, 0);
}
int getMedianNum(int bArray[], int iFilterLen)
{
  int bTab[iFilterLen];
  for (byte i = 0; i < iFilterLen; i++)
    bTab[i] = bArray[i];
  int i, j, bTemp;
  for (j = 0; j < iFilterLen - 1; j++)
  {
    for (i = 0; i < iFilterLen - j - 1; i++)
    {
      if (bTab[i] > bTab[i + 1])
      {
        bTemp = bTab[i];
        bTab[i] = bTab[i + 1];
        bTab[i + 1] = bTemp;
      }
    }
  }
  if ((iFilterLen & 1) > 0)
    bTemp = bTab[(iFilterLen - 1) / 2];
  else
    bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
  return bTemp;
}




void SCH_Delete_Task( const uint32_t TASK_INDEX) {
  SCH_tasks_G[TASK_INDEX].pTask = 0x0000 ;
  SCH_tasks_G[TASK_INDEX].Delay = 0;
  SCH_tasks_G[TASK_INDEX].Period = 0;
  SCH_tasks_G[TASK_INDEX].RunMe = 0;
}

void SCH_Init() {
  unsigned char i ;
  for ( i = 0; i < SCH_MAX_TASKS; i++) {
    SCH_Delete_Task(i) ;
  }
}

int SCH_Add_Task( void (* pFunction) () , unsigned int DELAY, unsigned int PERIOD) {

  int Index = 0;
  if (size == 0)
  {
    // If we ’ re here , there i s a space in the task array
    SCH_tasks_G[Index].pTask = pFunction ;
    SCH_tasks_G[Index].Delay = DELAY;
    SCH_tasks_G[Index].Period = PERIOD;
    SCH_tasks_G[Index].RunMe = 0;
    // return position of task ( to allow l at e r deletion )
    size++;
    return Index;
  }
  while (DELAY > SCH_tasks_G[Index].Delay && Index < size)
  {
    DELAY -= SCH_tasks_G[Index].Delay;
    Index++;
  }
  for (int i = size; i > Index; i--)
  {
    SCH_tasks_G[i].pTask = SCH_tasks_G[i - 1].pTask;
    SCH_tasks_G[i].Delay = SCH_tasks_G[i - 1].Delay;
    SCH_tasks_G[i].Period = SCH_tasks_G[i - 1].Period;
    SCH_tasks_G[i].RunMe = SCH_tasks_G[i - 1].RunMe;
  }
  // If we ’ re here , there i s a space in the task array
  SCH_tasks_G[Index].pTask = pFunction ;
  SCH_tasks_G[Index].Delay = DELAY;
  SCH_tasks_G[Index].Period = PERIOD;
  SCH_tasks_G[Index].RunMe = 0;
  size++;
  if (Index != size - 1) SCH_tasks_G[Index + 1].Delay -= DELAY;
  // return position of task ( to allow l at e r deletion )
  return Index ;
}
void SCH_Dispatch_Tasks()
{
  if (SCH_tasks_G[0].RunMe > 0)
  {
    (*SCH_tasks_G[0].pTask)() ;
    SCH_tasks_G[0].RunMe -= 1;
    sTask temp;
    temp.pTask = SCH_tasks_G[0].pTask;
    temp.Period = SCH_tasks_G[0].Period;
    temp.Delay = SCH_tasks_G[0].Period;
    temp.RunMe = SCH_tasks_G[0].RunMe;
    for (int i = 0; i < size - 1; i++)
    {
      SCH_tasks_G[i].pTask = SCH_tasks_G[i + 1].pTask;
      SCH_tasks_G[i].Delay = SCH_tasks_G[i + 1].Delay;
      SCH_tasks_G[i].Period = SCH_tasks_G[i + 1].Period;
      SCH_tasks_G[i].RunMe = SCH_tasks_G[i + 1].RunMe;
    }
    size--;
    //      Serial.println("Before");
    //      printSchedule();
    SCH_Add_Task(temp.pTask, temp.Delay, temp.Period);
    //      Serial.println("After");
    //      printSchedule();
  }

}
void SCH_Update( void )
{
  // Update with O1
  if (SCH_tasks_G[0].pTask)
  {
    if ( SCH_tasks_G[0].Delay == 0)
    {
      SCH_tasks_G[0].RunMe += 1;
    }
    else
    {
      SCH_tasks_G[0].Delay -= 1;
    }
  }
}

//Extra function to debug
//void printSchedule() {
//  Serial.print("Size: ");
//  Serial.println(size);
//  for (int i = 0; i < size; i++)
//  {
//    Serial.print("Task: ");
//    Serial.println(i);
//
//    if (SCH_tasks_G[i].pTask == &sendTDS) Serial.println("Task TDS");
//    else if (SCH_tasks_G[i].pTask == &read_sendHumid) Serial.println("Task Humid");
//    Serial.print("Delay: ");
//    Serial.println(SCH_tasks_G[i].Delay);
//  }
//}
