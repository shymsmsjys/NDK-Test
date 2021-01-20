package com.shcha.test;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {
    NativeApi nativeApi;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        TextView tv = findViewById(R.id.text_result);
        nativeApi = new NativeApi();
        tv.setText("result = " + nativeApi.getAbi());
        int ret = nativeApi.startThread();
        Log.i("jni_test", "startThread: " + ret);
    }
}