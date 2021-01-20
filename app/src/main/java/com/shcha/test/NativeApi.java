package com.shcha.test;

import android.util.Log;

public class NativeApi {
    static {
        System.loadLibrary("csh_test-lib");
    }

    private native int addNative (int a, int b);
    private native String stringFromJni();
    private native int startThreadNative();

    public static void callback(int a) {
        Log.i("jni_test", "callback: " + a);
    }

    public int add(int a, int b) {
        return addNative(4, 7);
    }

    public String getAbi() {
        return stringFromJni();
    }

    public int startThread() {
        return startThreadNative();
    }
}
