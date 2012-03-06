///////////////////////////////////////////////////////////////////////////////
// Project:    QR Reader for Android
// Package:    com.android.qrreader
// File:       AboutActivity.java
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Contains the activity of the about window.
///////////////////////////////////////////////////////////////////////////////

package com.android.qrreader;

import java.util.List;

import com.android.camera.DroidCamera;

import android.app.Activity;
import android.content.pm.ApplicationInfo;
import android.hardware.Camera.Parameters;
import android.os.Bundle;
import android.widget.TextView;

/**
 * The Class AboutActivity implements the about window of this application.
 * This window shows the information about the application and supported
 * features by the devices on which runs.
 */
public class AboutActivity extends Activity {

    /**
     * Called when the activity is firstly created.
     *
     * @see android.app.Activity#onCreate(android.os.Bundle) onCreate(Bundle) 
     */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        setContentView(R.layout.about_layout);
        
        TextView textView;
        DroidCamera droidCamera = DroidCamera.open();
        if (droidCamera != null) {
            Parameters camParams = droidCamera.getCamera().getParameters();
            List<String> flashModes = camParams.getSupportedFlashModes();
            
            // Printing the information about flash support by this device
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
            
            // Printing whether this device supports the auto-focusing
            textView = (TextView) findViewById(R.id.CameraAutoFocus);
            if (droidCamera.autoFocusSupport()) textView.setText(R.string.yes);
            
            droidCamera.release();
        }

    
        // Printing whether is the OI File manager installed
        textView = (TextView) findViewById(R.id.OIFileManager);
        if (packageInstalled("org.openintents.filemanager")) textView.setText(R.string.yes);
    }
    
    /**
     * Method for retrieving whether demanded package is installed on the android.
     *
     * @param packageName The name of the package.
     * @return True, if the package is installed, otherwise false.
     */
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

//End of the file
