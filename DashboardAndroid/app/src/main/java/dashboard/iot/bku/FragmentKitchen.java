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

/**
 * Created by ADMIN on 3/6/2022.
 */

public class FragmentKitchen extends Fragment {

    //Button btnBack;
    TextView txtTemp, txtHumi;

    @Override
    public void onCreate(@Nullable Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
    }
    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState){
        View view = inflater.inflate(R.layout.layout_fragment_kitchen, container, false);


        txtTemp = view.findViewById(R.id.txtKitTemp);
        txtHumi = view.findViewById(R.id.txtKitHumi);

//        String strData1 = getArguments().getString("fragmentTagTemp");
//        String strData2 = getArguments().getString("fragmentTagHumi");

//        txtTemp.setText(strData1 + "°C");
//        txtHumi.setText(strData2 + "%");


        return view;
    }
}
