package com.android.qrreader;

import java.io.File;
import java.io.IOException;

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
import android.os.Bundle;
import android.os.Environment;
import android.preference.PreferenceActivity;
import android.preference.PreferenceManager;
import android.text.Editable;
import android.util.Log;
import android.view.Window;

public class QrReaderSettingsActivity extends PreferenceActivity {
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
        
        PromptDialogPreference promptDialog;
        
        promptDialog = (PromptDialogPreference) findPreference("Prefereces_ResetPreferences");
        promptDialog.setResultCallback(onResultResetDialogPreference);
        
        promptDialog = (PromptDialogPreference) findPreference("Prefereces_Images_Clear");
        promptDialog.setResultCallback(onResultClearImagesDialogPreference);
        
        promptDialog = (PromptDialogPreference) findPreference("Prefereces_QRCodes_Clear");
        promptDialog.setResultCallback(onResultClearQRCodesDialogPreference);
        
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

