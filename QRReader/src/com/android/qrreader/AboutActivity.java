package com.android.qrreader;

import java.util.List;

import com.android.camera.DroidCamera;

import android.app.Activity;
import android.content.pm.ApplicationInfo;
import android.hardware.Camera.Parameters;
import android.os.Bundle;
import android.view.Window;
import android.widget.TextView;

public class AboutActivity extends Activity {

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        setContentView(R.layout.about_layout);
        
        TextView textView;
        DroidCamera droidCamera = DroidCamera.open();
        if (droidCamera != null) {
            Parameters camParams = droidCamera.getCamera().getParameters();
            List<String> flashModes = camParams.getSupportedFlashModes();
            
            textView = (TextView) findViewById(R.id.CameraFlash);
            if (flashModes != null) {
                textView.setText(R.string.yes);
                
                textView = (TextView) findViewById(R.id.CameraFlashAuto);
                if (flashModes.contains(Parameters.FLASH_MODE_AUTO)) textView.setText(R.string.yes);
                
                textView = (TextView) findViewById(R.id.CameraFlashOn);
                if (flashModes.contains(Parameters.FLASH_MODE_ON)) textView.setText(R.string.yes);
                
                textView = (TextView) findViewById(R.id.CameraFlashRedEyes);
                if (flashModes.contains(Parameters.FLASH_MODE_RED_EYE)) textView.setText(R.string.yes);
                
                textView = (TextView) findViewById(R.id.CameraFlashTorch);
                if (flashModes.contains(Parameters.FLASH_MODE_TORCH)) textView.setText(R.string.yes);
            }
            
            textView = (TextView) findViewById(R.id.CameraAutoFocus);
            if (droidCamera.autoFocusSupport()) textView.setText(R.string.yes);
            
            droidCamera.release();
        }

    
        textView = (TextView) findViewById(R.id.OIFileManager);
        if (packageInstalled("org.openintents.filemanager")) textView.setText(R.string.yes);
    }
    
    public boolean packageInstalled(String packageName) {
        List<ApplicationInfo> packages;
        packages = getPackageManager().getInstalledApplications(0);
         
        for (ApplicationInfo packageInfo : packages) {
            if (packageInfo.packageName.equals(packageName)) {
                return true;
            }
        }        
        
        return false;

    }

}

