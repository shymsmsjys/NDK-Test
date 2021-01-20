package com.shcha.test;

public class NativeApi {
    static {
        System.loadLibrary("csh_test-lib");
    }

    private native int addNative (int a, int b);
    private native String stringFromJni();

    public int add(int a, int b) {
        return addNative(4, 7);
    }

    public String getAbi() {
        return stringFromJni();
    }
}
