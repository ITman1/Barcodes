package com.android.qrreader;

import java.io.File;
import java.io.IOException;

import com.android.preferences.PromptDialogPreference;
import com.android.preferences.PromptDialogPreference.PromptDialogPreferenceCallback;
import com.android.preferences.ValidateEditTextPreference;
import com.android.preferences.ValidateEditTextPreference.ValidateCallback;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.content.res.Resources;
import android.os.Bundle;
import android.os.Environment;
import android.preference.PreferenceActivity;
import android.preference.PreferenceManager;
import android.text.Editable;
import android.util.Log;

public class QrReaderSettingsActivity extends PreferenceActivity {
    private static final String TAG = "com.android.qrreader::QrReaderSettingsActivity";
    
    private ValidateEditTextPreference pathImagesEditTextPref;
    private ValidateEditTextPreference pathQRCodesEditTextPref;

    public static void resetSettings(Context context) {
        PreferenceManager.setDefaultValues(context, R.xml.preferences, false);
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
    
    private PromptDialogPreferenceCallback onResultPromptDialogPreference = new PromptDialogPreferenceCallback() {
        public void onResult(int which) {
            Log.i(TAG, "PromptDialogPreferenceCallback::onResult");
            if (which == DialogInterface.BUTTON_POSITIVE) {
                resetSettings(getBaseContext());
                restart();
            }
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
        super.onCreate(savedInstanceState);
        addPreferencesFromResource(R.xml.preferences);
        
        PromptDialogPreference promptDialog = (PromptDialogPreference) findPreference("Prefereces_ResetPreferences");
        promptDialog.setResultCallback(onResultPromptDialogPreference);
        
        pathImagesEditTextPref = (ValidateEditTextPreference)findPreference("Preferences_Images_FilePath");
        pathImagesEditTextPref.setValidateCallback(pathValidateCallback);
        
        pathQRCodesEditTextPref = (ValidateEditTextPreference)findPreference("Preferences_QRCodes_FilePath");
        pathQRCodesEditTextPref.setValidateCallback(pathValidateCallback);
    }
    
    public void restart() {
        Log.i(TAG, "restart");
        Intent intent = getIntent();
        finish();
        startActivity(intent);
        
    }

}

