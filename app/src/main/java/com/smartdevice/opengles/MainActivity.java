package com.smartdevice.opengles;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.widget.TextView;

import com.smartdevice.opengles.databinding.ActivityMainBinding;

import java.io.File;

public class MainActivity extends AppCompatActivity {
    private static final String TAG = "opengles";
    private ActivityMainBinding binding;
    private PngPreviewController pngPreviewController;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());
        String assertDir = "picture";
        AssectsUtil.loadAssetsDirfile(getApplicationContext(),assertDir);
        String appFilePath = AssectsUtil.getAppDir(getApplicationContext(),assertDir)+ File.separator;
        Log.d(TAG,"appFilePath="+appFilePath);
        String pngFilePath = appFilePath+"prompt.png";
        pngPreviewController = new PngPreviewController();
        pngPreviewController.init(pngFilePath);
        // Example of a call to a native method
        SurfaceView surfaceView = binding.sampleDisplay;
        surfaceView.getHolder().addCallback(new SurfaceHolder.Callback() {
            @Override
            public void surfaceCreated(@NonNull SurfaceHolder holder) {
//                openglesDemo(holder.getSurface());
                pngPreviewController.setSurface(holder.getSurface());
            }

            @Override
            public void surfaceChanged(@NonNull SurfaceHolder holder, int format, int width, int height) {
                pngPreviewController.resetSize(width,height);

            }

            @Override
            public void surfaceDestroyed(@NonNull SurfaceHolder holder) {

            }
        });
    }


}