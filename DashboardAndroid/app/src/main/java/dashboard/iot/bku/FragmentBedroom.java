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
 * Created by ADMIN on 3/17/2022.
 */

public class FragmentBedroom  extends Fragment {
    TextView txtTemp, txtHumi, txtpH;
    int temperature = 25, humidity = 93, pH = 7;

    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }
    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.layout_fragment_bedroom, container, false);
        txtTemp = view.findViewById(R.id.Temperature);
        txtHumi = view.findViewById(R.id.Humidity);
        txtpH   = view.findViewById(R.id.pH);

//        String strData1 = getArguments().getString("fragmentTagTemp");
//        String strData2 = getArguments().getString("fragmentTagHumi");

        txtTemp.setText(temperature + "°C");
        txtHumi.setText(humidity + "%");
        txtpH.setText(pH);
        return view;
    }
}
