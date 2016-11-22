package com.dragon4.owo.somaiotapp.Activity;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

import com.dragon4.owo.somaiotapp.Model.Monitor;
import com.dragon4.owo.somaiotapp.R;

public class MonitorDetailActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_monitor_detail);

        Monitor thisMonitor = Monitor.getSelectedMonitor();
        TextView mac = (TextView)findViewById(R.id.monitor_mac);
        mac.setText(thisMonitor.getMacAddress());
        TextView ip = (TextView)findViewById(R.id.monitor_ip);
        ip.setText(thisMonitor.getIpAddress());
        TextView rx = (TextView)findViewById(R.id.monitor_rx);
        rx.setText(thisMonitor.getRsByte());
        TextView tx = (TextView)findViewById(R.id.monitor_tx);
        tx.setText(thisMonitor.getCsByte());
        TextView connect = (TextView)findViewById(R.id.monitor_connect_time);
        connect.setText(thisMonitor.getConnectTime());
        TextView connectOut = (TextView)findViewById(R.id.monitor_connect_out_time);
        connectOut.setText(thisMonitor.getConnectOutTime());

    }
}
