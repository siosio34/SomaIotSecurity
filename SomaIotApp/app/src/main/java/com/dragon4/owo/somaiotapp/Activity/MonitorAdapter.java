package com.dragon4.owo.somaiotapp.Activity;

import android.content.Intent;
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

    public static class ViewHolder extends RecyclerView.ViewHolder  {
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
    public MonitorAdapter.ViewHolder onCreateViewHolder(final ViewGroup parent, int viewType) {

        // create a new view
        View v = LayoutInflater.from(parent.getContext())
                .inflate(R.layout.monitor_recycle, parent, false);


        final ViewHolder vh = new ViewHolder(v);
        v.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                int position = vh.getAdapterPosition();
                Monitor.setSelectedMonitor(monitors.get(position));
                parent.getContext().startActivity(new Intent(parent.getContext(),MonitorDetailActivity.class));

            }
        });


        return vh;


    }

    @Override
    public void onBindViewHolder(MonitorAdapter.ViewHolder holder, int position) {

        holder.macText.setText(monitors.get(position).getMacAddress());
        holder.hostText.setText(monitors.get(position).getHostName());


    }

    @Override
    public int getItemCount() {
        return monitors.size();
    }
}
