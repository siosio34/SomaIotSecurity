package com.dragon4.owo.somaiotapp.Activity;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.util.Log;

import com.dragon4.owo.somaiotapp.Data.HttpHandler;
import com.dragon4.owo.somaiotapp.Model.Monitor;
import com.dragon4.owo.somaiotapp.R;

import java.util.ArrayList;
import java.util.concurrent.ExecutionException;

public class MonitorActivity extends AppCompatActivity {


    private RecyclerView mRecyclerView;
    private RecyclerView.Adapter mAdapter;
    private RecyclerView.LayoutManager mLayoutManager;
    private ArrayList<Monitor> myDataset;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_monitor);

        mRecyclerView = (RecyclerView) findViewById(R.id.my_recycler_view);

        mRecyclerView.setHasFixedSize(true);

        // use a linear layout manager
        mLayoutManager = new LinearLayoutManager(this);
        mRecyclerView.setLayoutManager(mLayoutManager);

        myDataset = new ArrayList<>();
        mAdapter = new MonitorAdapter(myDataset);

        // specify an adapter (see also next example)
        mRecyclerView.setAdapter(mAdapter);

        try {
            addMonitorList();

        } catch (ExecutionException | InterruptedException e) {
            e.printStackTrace();
        }
        //// TODO: 2016. 11. 18. json값 넘겨받기
        //myDataset.add()

    }

    public void addMonitorList() throws ExecutionException, InterruptedException {

        String siteUrl = "http:192.168.0.19:3000/util/monitoring.json";
        String temp = new HttpHandler().execute(siteUrl).get();
        Log.i("monitorText",temp);
        //myDataset.add()

    }
}
