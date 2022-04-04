package dashboard.iot.bku;

import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentActivity;
import android.support.v4.app.FragmentManager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallbackExtended;
import org.eclipse.paho.client.mqttv3.MqttMessage;

import static dashboard.iot.bku.Constants.BEDROOM_FRAGMENT_INDEX;
import static dashboard.iot.bku.Constants.KITCHEN_FRAGMENT_INDEX;
import static dashboard.iot.bku.Constants.LIVINGROOM_FRAGMENT_INDEX;

public class MainActivity extends FragmentActivity {

    Fragment fragment, fragment_setup;
    FragmentLivingroom fragmentLivingRoom;
    FragmentBedroom fragmentBedRoom;
    FragmentKitchen fragmentkitchen;
    Button btnLiv, btnBed, btnKit;

    MQTTHelper mqttHelper;
//    String Temp = "25";
//    String Humi = "50";
    int now = 1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        startMQTT();

        btnLiv = findViewById(R.id.btnLivingroom);
        btnLiv.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                selectFragment(LIVINGROOM_FRAGMENT_INDEX);
                now = LIVINGROOM_FRAGMENT_INDEX;
            }
        });

        btnBed = findViewById(R.id.btnBedroom);
        btnBed.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                selectFragment(BEDROOM_FRAGMENT_INDEX);
                now = BEDROOM_FRAGMENT_INDEX;
            }
        });

        btnKit = findViewById(R.id.btnKitchen);
        btnKit.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                selectFragment(KITCHEN_FRAGMENT_INDEX);
                now = KITCHEN_FRAGMENT_INDEX;
            }
        });
        fragmentLivingRoom = new FragmentLivingroom();
        fragmentBedRoom = new FragmentBedroom();
        fragmentkitchen = new FragmentKitchen();

    }
    public void selectFragment(int pos){
        Class fragmentClass = null;
        switch (pos){
            case LIVINGROOM_FRAGMENT_INDEX:
                fragmentClass = FragmentLivingroom.class;
                fragment = fragmentLivingRoom;
                break;
            case BEDROOM_FRAGMENT_INDEX:
                fragmentClass = FragmentBedroom.class;
                fragment = fragmentBedRoom;
                break;
            case KITCHEN_FRAGMENT_INDEX:
                fragmentClass = FragmentKitchen.class;
                fragment = fragmentkitchen;
                break;
            default:
                break;
        }

        try {


            ;
            //fragment = (Fragment) fragmentClass.newInstance();


            //Transfer main activity to fragment(main giao tiep vs fragment)
//            Bundle bundle = new Bundle();
//            bundle.putString("fragmentTagTemp", Temp);
//            bundle.putString("fragmentTagHumi", Humi);
//
//            fragment.setArguments(bundle);

            // Insert the fragment by replacing any existing fragment
            FragmentManager fragmentManager = getSupportFragmentManager();
            fragmentManager.beginTransaction().replace(R.id.fragment_content, fragment).commitAllowingStateLoss();
        } catch (Exception e) {

        }
    }

    //Dung de tu fragment giao tiep vs
    public void proccessDataFromFragment(int fragment_index, String fragment_data){
        Log.d("Fragment", "Data is from: " + fragment_index);
        Log.d("Fragment", "Value: " + fragment_data);
    }

    //1:34
    public String decode(String message){
        String temp = "";
        for(int i = 2; i < message.length(); i++){
            temp += message.charAt(i);
        }
        return temp;
    }

    private void startMQTT() {
        mqttHelper = new MQTTHelper(getApplicationContext(), "123456");

        mqttHelper.setCallback(new MqttCallbackExtended() {
            @Override
            public void connectComplete(boolean reconnect, String serverURI) {
                Log.d("mqtt", "Connection is successful");
            }

            @Override
            public void connectionLost(Throwable cause) {

            }

            @Override
            public void messageArrived(String topic, MqttMessage message) throws Exception {
                Log.d("mqtt", "Received: " + message.toString());
//                String id = "";
//                String val;
//                val = decode(message.toString());
//                id += message.toString().charAt(0);
                if(topic.equals("locthanh2001/feeds/temp") ){
                    fragmentLivingRoom.temperature = Integer.parseInt(message.toString());
                }
                if(topic.equals("locthanh2001/feeds/humid")){
                    fragmentLivingRoom.humidity = Integer.parseInt(message.toString());
                }
                if(topic.equals("locthanh2001/feeds/tds")){
                    fragmentLivingRoom.pH = Integer.parseInt(message.toString());
                }
            }

            @Override
            public void deliveryComplete(IMqttDeliveryToken token) {

            }
        });
    }
}
