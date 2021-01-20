package com.shcha.test;

import android.util.Log;

public class NativeApi {
    static {
        System.loadLibrary("csh_test-lib");
    }

    private native int addNative (int a, int b);

    public int add(int a, int b) {
        return addNative(4, 7);
    }
}
