package com.android.qrreader;

import java.io.File;
import java.io.IOException;
import java.util.List;

import com.android.camera.DroidCamera;
import com.android.preferences.PromptDialogPreference;
import com.android.preferences.PromptDialogPreference.PromptDialogPreferenceCallback;
import com.android.preferences.ValidateEditTextPreference;
import com.android.preferences.ValidateEditTextPreference.ValidateCallback;
import com.filesystem.Operations;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.content.res.Resources;
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
import android.view.Window;

public class SettingsActivity extends PreferenceActivity {
    private static final String TAG = "com.android.qrreader::QrReaderSettingsActivity";
    
    private ValidateEditTextPreference pathImagesEditTextPref;
    private ValidateEditTextPreference pathQRCodesEditTextPref;

    public static void resetSettings(Context context) {
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(context);
        Editor editor = prefs.edit();
        Resources res = context.getResources();
        
        editor.clear();
        editor.putString("Preferences_Images_FilePath", 
                Environment.getExternalStorageDirectory().getPath() + 
                File.separator + res.getString(R.string.app_name) +
                File.separator + res.getString(R.string.dir_images));
        editor.putString("Preferences_QRCodes_FilePath", 
                Environment.getExternalStorageDirectory().getPath() + 
                File.separator + res.getString(R.string.app_name) +
                File.separator + res.getString(R.string.dir_qrcodes));
        
        editor.commit();
    }
    
    private PromptDialogPreferenceCallback onResultResetDialogPreference = new PromptDialogPreferenceCallback() {
        public void onResult(int which) {
            Log.i(TAG, "onResultResetDialogPreference::onResult");
            if (which == DialogInterface.BUTTON_POSITIVE) {
                resetSettings(getBaseContext());
                restart();
            }
        }    
    };
    
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
    
    private OnPreferenceClickListener onAboutPreferenceClick = new OnPreferenceClickListener() {
        public boolean onPreferenceClick(Preference preference) {
            startActivity(new Intent(getBaseContext(), AboutActivity.class));
            return true;
        }
    };
    
    private static boolean isFilenameValid(String file) {
        File f = new File(file);
        try {
           f.getCanonicalPath();
           return true;
        } catch (IOException e) {
           return false;
        }
    }

    
    private ValidateCallback pathValidateCallback = new ValidateCallback() {
        public void validate(AlertDialog dialog, Editable str) {
            Log.i(TAG, "ValidateCallback::validate");
            dialog.getButton(AlertDialog.BUTTON_POSITIVE).setEnabled(isFilenameValid(str.toString()) && str.length() != 0);
        }    
    };
   
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.i(TAG, "onCreate");
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        super.onCreate(savedInstanceState);
        addPreferencesFromResource(R.xml.preferences);
        
        PromptDialogPreference promptDialogPreference;
        
        promptDialogPreference = (PromptDialogPreference) findPreference("Prefereces_ResetPreferences");
        promptDialogPreference.setResultCallback(onResultResetDialogPreference);
        
        promptDialogPreference = (PromptDialogPreference) findPreference("Prefereces_Images_Clear");
        promptDialogPreference.setResultCallback(onResultClearImagesDialogPreference);
        
        promptDialogPreference = (PromptDialogPreference) findPreference("Prefereces_QRCodes_Clear");
        promptDialogPreference.setResultCallback(onResultClearQRCodesDialogPreference);
        
        pathImagesEditTextPref = (ValidateEditTextPreference)findPreference("Preferences_Images_FilePath");
        pathImagesEditTextPref.setValidateCallback(pathValidateCallback);
        
        pathQRCodesEditTextPref = (ValidateEditTextPreference)findPreference("Preferences_QRCodes_FilePath");
        pathQRCodesEditTextPref.setValidateCallback(pathValidateCallback);
        
        CheckBoxPreference autoFocusPreference = (CheckBoxPreference)findPreference("Preferences_Camera_AutoFocus");
        ListPreference flashPreference = (ListPreference)findPreference("Preferences_Camera_Flash");
        
        DroidCamera droidCamera = DroidCamera.open();
        if (droidCamera != null) {
            // Enable/Disable autofocus preference
            autoFocusPreference.setEnabled(droidCamera.autoFocusSupport());

            // Enable/Disable flash preference whether at least on flash mode exists
            Parameters camParams = droidCamera.getCamera().getParameters();
            List<String> flashModes = camParams.getSupportedFlashModes();

            flashPreference.setEnabled(flashModes != null);
        }
        
        PreferenceScreen aboutApplication = (PreferenceScreen)findPreference("Prefereces_About");
        aboutApplication.setOnPreferenceClickListener(onAboutPreferenceClick);
        
        droidCamera.release();
    }

    public void restart() {
        Log.i(TAG, "restart");
        Intent intent = getIntent();
        finish();
        startActivity(intent);
        
    }

}

