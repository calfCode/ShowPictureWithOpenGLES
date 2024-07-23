package com.smartdevice.opengles;

import android.view.Surface;

public class PngPreviewController {
    static {
        System.loadLibrary("opengles");
    }

    public native void init(String filePath);
    public native void setSurface(Surface surface);
    public native void resetSize(int width,int height);
    public native void stop();
}
