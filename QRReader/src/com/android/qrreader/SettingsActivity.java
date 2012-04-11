///////////////////////////////////////////////////////////////////////////////
// Project:    QR Reader for Android
// Package:    com.android.qrreader
// File:       SettingsActivity.java
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Contains the activity of the preference manager.
///////////////////////////////////////////////////////////////////////////////

package com.android.qrreader;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

import com.android.camera.DroidCamera;
import com.android.preferences.PromptDialogPreference;
import com.android.preferences.PromptDialogPreference.PromptDialogPreferenceCallback;
import com.android.preferences.ValidateEditTextPreference;
import com.android.preferences.ValidateEditTextPreference.ValidateCallback;
import com.filesystem.Filename;
import com.filesystem.Operations;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.content.res.Resources;
import android.hardware.Camera;
import android.hardware.Camera.Parameters;
import android.os.Bundle;
import android.os.Environment;
import android.preference.CheckBoxPreference;
import android.preference.ListPreference;
import android.preference.Preference;
import android.preference.Preference.OnPreferenceClickListener;
import android.preference.PreferenceActivity;
import android.preference.PreferenceManager;
import android.preference.PreferenceScreen;
import android.text.Editable;
import android.util.Log;

/**
 * The Class SettingsActivity implements the activity of the preference manager.
 */
public class SettingsActivity extends PreferenceActivity {
    
    /** The Constant Debug TAG. */
    private static final String TAG = "com.android.qrreader::QrReaderSettingsActivity";
    
    /** The reference to the path edit text for inputing the storage path of the images. */
    private ValidateEditTextPreference pathImagesEditTextPref;
    
    /** The reference to the path edit text for inputing the storage path of the QR codes. */
    private ValidateEditTextPreference pathQRCodesEditTextPref;

    /**
     * Resets all shared preferences to default values.
     *
     * @param context the context
     */
    public static void resetSettings(Context context) {
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(context);
        Editor editor = prefs.edit();
        Resources res = context.getResources();
        
        editor.clear(); // Reseting the shared preferences
        
        // This will add the default storage paths for images and QR codes
        editor.putString("Preferences_Images_FilePath", 
                Environment.getExternalStorageDirectory().getPath() + 
                File.separator + res.getString(R.string.app_name) +
                File.separator + res.getString(R.string.dir_images));
        editor.putString("Preferences_QRCodes_FilePath", 
                Environment.getExternalStorageDirectory().getPath() + 
                File.separator + res.getString(R.string.app_name) +
                File.separator + res.getString(R.string.dir_qrcodes));
        
        // Sets default image resolution
        DroidCamera droidCamera = DroidCamera.open();
        if (droidCamera != null) {
            List<Camera.Size> sizes = droidCamera.getCamera().getParameters().getSupportedPictureSizes();
            
            if (sizes.size() > 0) {
                editor.putString("Preferences_Images_Resolution", String.format(
                        "%d_%d", sizes.get(sizes.size() / 2).width, sizes.get(sizes.size() / 2).height)
                );
            }
            droidCamera.release();
        }
        
        editor.commit();
    }
    
    /** The listener of the click on the result button of reset preferences dialog. */
    private PromptDialogPreferenceCallback onResultResetDialogPreference = new PromptDialogPreferenceCallback() {
        public void onResult(int which) {
            Log.i(TAG, "onResultResetDialogPreference::onResult");
            if (which == DialogInterface.BUTTON_POSITIVE) {
                resetSettings(getBaseContext());
                restart();
            }
        }    
    };
    
    /** The listener of the click on the result button of clear images preferences dialog. */
    private PromptDialogPreferenceCallback onResultClearImagesDialogPreference = new PromptDialogPreferenceCallback() {
        public void onResult(int which) {
            Log.i(TAG, "onResultClearImagesDialogPreference::onResult");
            if (which == DialogInterface.BUTTON_POSITIVE) {
                SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(getBaseContext());
                String path = prefs.getString("Preferences_Images_FilePath", null);
                Operations.removeFiles(new File(path));
            }
        }    
    };
    
    /** The listener of the click on the result button of clear QR codes preferences dialog. */
    private PromptDialogPreferenceCallback onResultClearQRCodesDialogPreference = new PromptDialogPreferenceCallback() {
        public void onResult(int which) {
            Log.i(TAG, "onResultClearImagesDialogPreference::onResult");
            if (which == DialogInterface.BUTTON_POSITIVE) {
                SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(getBaseContext());
                String path = prefs.getString("Preferences_QRCodes_FilePath", null);
                Operations.removeFiles(new File(path));
            }
        }    
    };
    
    /** The listener of the click on the preference which opens the about activity. */
    private OnPreferenceClickListener onAboutPreferenceClick = new OnPreferenceClickListener() {
        public boolean onPreferenceClick(Preference preference) {
            startActivity(new Intent(getBaseContext(), AboutActivity.class));
            return true;
        }
    };
    
    /** The listener of the click on the preference which opens the installation manager activity. */
    private OnPreferenceClickListener onInstallationManagerClick = new OnPreferenceClickListener() {
        public boolean onPreferenceClick(Preference preference) {
            startActivity(new Intent(getBaseContext(), InstallationManagerActivity.class));
            return true;
        }
    };
    
    /** The validate callback which is invoked when path edit is changed. 
     * When the path is invalid it disables the confirm button. */
    private ValidateCallback pathValidateCallback = new ValidateCallback() {
        public void validate(AlertDialog dialog, Editable str) {
            Log.i(TAG, "ValidateCallback::validate");
            dialog.getButton(AlertDialog.BUTTON_POSITIVE).setEnabled(
                    Filename.isFilenameValid(str.toString()) && str.length() != 0);
        }    
    };
   
    /**
     * Called when the activity is firstly created.
     * Sets the listeners for opening the other activities 
     * or capturing the result buttons on the dialogs.
     *
     * @see android.app.Activity#onCreate(android.os.Bundle) onCreate(Bundle) 
     */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.i(TAG, "onCreate");
        super.onCreate(savedInstanceState);
        addPreferencesFromResource(R.xml.preferences);
        
        PromptDialogPreference promptDialogPreference;
        
        // Registering the listener of the click on the result button of the reset preferences dialog
        promptDialogPreference = (PromptDialogPreference) findPreference("Prefereces_ResetPreferences");
        promptDialogPreference.setResultCallback(onResultResetDialogPreference);
        
        // Registering the listener of the click on the result button of the clear images dialog
        promptDialogPreference = (PromptDialogPreference) findPreference("Prefereces_Images_Clear");
        promptDialogPreference.setResultCallback(onResultClearImagesDialogPreference);
        
        // Registering the listener of the click on the result button of the clear QR codes dialog
        promptDialogPreference = (PromptDialogPreference) findPreference("Prefereces_QRCodes_Clear");
        promptDialogPreference.setResultCallback(onResultClearQRCodesDialogPreference);
        
        // Registering the callback for validating the path of the storage for images
        pathImagesEditTextPref = (ValidateEditTextPreference)findPreference("Preferences_Images_FilePath");
        pathImagesEditTextPref.setValidateCallback(pathValidateCallback);
        
        // Registering the callback for validating the path of the storage for QR codes
        pathQRCodesEditTextPref = (ValidateEditTextPreference)findPreference("Preferences_QRCodes_FilePath");
        pathQRCodesEditTextPref.setValidateCallback(pathValidateCallback);
        
        // Getting the camera features
        CheckBoxPreference autoFocusPreference = (CheckBoxPreference)findPreference("Preferences_Camera_AutoFocus");
        ListPreference flashPreference = (ListPreference)findPreference("Preferences_Camera_Flash");
        
        // Disables the setting of sum preferences when the camera does not support this features
        DroidCamera droidCamera = DroidCamera.open();
        if (droidCamera != null) {
            // Enable/Disable autofocus preference
            autoFocusPreference.setEnabled(droidCamera.autoFocusSupport());

            // Enable/Disable flash preference whether at least on flash mode exists
            Parameters camParams = droidCamera.getCamera().getParameters();
            List<String> flashModes = camParams.getSupportedFlashModes();

            flashPreference.setEnabled(flashModes != null);
            
            // Sets resolution entries
            ListPreference resolutionsPreference = (ListPreference)findPreference("Preferences_Images_Resolution");
            
            List<CharSequence> entries = new ArrayList<CharSequence>();
            List<CharSequence> entryValues = new ArrayList<CharSequence>();
            
            List<Camera.Size> sizes = droidCamera.getCamera().getParameters().getSupportedPictureSizes();
            
            for (Camera.Size size : sizes) {
                entries.add(String.format("%d x %d", size.width, size.height));
                entryValues.add(String.format("%d_%d", size.width, size.height));
            }
            
            resolutionsPreference.setEntries((CharSequence[])entries.toArray(new CharSequence[entries.size()]));
            resolutionsPreference.setEntryValues((CharSequence[])entryValues.toArray(new CharSequence[entryValues.size()]));
        }
        
        // Registering the listener of the click for opening the about activity
        PreferenceScreen aboutApplication = (PreferenceScreen)findPreference("Prefereces_About");
        
        aboutApplication.setOnPreferenceClickListener(onAboutPreferenceClick);
        // Registering the listener of the click for opening the installation manager
        PreferenceScreen installationManager = (PreferenceScreen)findPreference("Prefereces_InstallationManager");
        installationManager.setOnPreferenceClickListener(onInstallationManagerClick);
        
        droidCamera.release();
    }

    /**
     * Restarts this activity.
     */
    public void restart() {
        Log.i(TAG, "restart");
        Intent intent = getIntent();
        finish();
        startActivity(intent);
        
    }

}

//End of the file
