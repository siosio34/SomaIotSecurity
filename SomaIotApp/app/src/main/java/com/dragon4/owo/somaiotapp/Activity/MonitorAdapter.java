package com.dragon4.owo.somaiotapp.Activity;

import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import com.dragon4.owo.somaiotapp.Model.Monitor;
import com.dragon4.owo.somaiotapp.R;

import java.util.ArrayList;

/**
 * Created by joyeongje on 2016. 11. 18..
 */

public class MonitorAdapter extends RecyclerView.Adapter<MonitorAdapter.ViewHolder>{

    // Provide a reference to the views for each data item
    // Complex data items may need more than one view per item, and
    // you provide access to all the views for a data item in a view holder

    private ArrayList<Monitor> monitors;

    public static class ViewHolder extends RecyclerView.ViewHolder {
        // each data item is just a string in this case

        TextView macText;
        TextView ipText;
        TextView hostText;
        TextView rsByte;
        TextView csByte;
        TextView connectTime;
        TextView connectOutTime;

        public ViewHolder(View view) {
            super(view);
            macText = (TextView)view.findViewById(R.id.monitor_mac);
            hostText = (TextView)view.findViewById(R.id.monitor_host);

        }
    }

    public MonitorAdapter(ArrayList<Monitor> monitorsArray) {
        monitors = monitorsArray;
    }



    @Override
    public MonitorAdapter.ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {

        // create a new view
        View v = LayoutInflater.from(parent.getContext())
                .inflate(R.layout.monitor_recycle, parent, false);
        // set the view's size, margins, paddings and layout parameters

        ViewHolder vh = new ViewHolder(v);
        return vh;


    }

    @Override
    public void onBindViewHolder(MonitorAdapter.ViewHolder holder, int position) {



    }

    @Override
    public int getItemCount() {
        return monitors.size();
    }
}
