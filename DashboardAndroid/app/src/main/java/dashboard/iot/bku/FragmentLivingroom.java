package dashboard.iot.bku;

import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;

import static dashboard.iot.bku.Constants.BEDROOM_FRAGMENT_INDEX;
import static dashboard.iot.bku.Constants.LIVINGROOM_FRAGMENT_INDEX;

/**
 * Created by ADMIN on 2/21/2022.
 */

public class FragmentLivingroom extends Fragment {

    //Button btnBack;
    TextView txtTemp, txtHumi, txtpH;
    int temperature, humidity, pH;
    @Override
    public void onCreate(@Nullable Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
    }
    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState)
    {
        View view = inflater.inflate(R.layout.layout_fragment_livingroom, container, false);

        /*btnBack = view.findViewById(R.id.btnBack);
        btnBack.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ((MainActivity)getActivity()).selectFragment(BEDROOM_FRAGMENT_INDEX);
                ((MainActivity)getActivity()).proccessDataFromFragment(LIVINGROOM_FRAGMENT_INDEX, "ABC");
            }
        });*/

        //get data from main activity
//        txtTemp = view.findViewById(R.id.txtLivTemp);
//        txtHumi = view.findViewById(R.id.txtLivHumi);
//        txtpH   = view.findViewById(R.id.txtLivpH);

//        String strData1 = getArguments().getString("fragmentTagTemp");
//        String strData2 = getArguments().getString("fragmentTagHumi");

//        txtTemp.setText(temperature + "°C");
//        txtHumi.setText(humidity + "%");
//        txtpH.setText(pH);

        return view;
    }
}
