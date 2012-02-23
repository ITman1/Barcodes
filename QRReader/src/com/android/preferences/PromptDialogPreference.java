package com.android.preferences;

import android.content.Context;
import android.content.DialogInterface;
import android.preference.DialogPreference;
import android.util.AttributeSet;
import android.util.Log;


public class PromptDialogPreference extends DialogPreference {
    private static final String TAG = "com.android.preferences::PromptDialogPreference";
    
    public interface PromptDialogPreferenceCallback {
        public void onResult(int which);
    }
    
    private PromptDialogPreferenceCallback callback;

    public PromptDialogPreference(Context context, AttributeSet attrs) {
        super(context, attrs);
        Log.i(TAG, "PromptDialogPreference");
    }

    @Override
    protected void onDialogClosed(boolean positiveResult) {
        Log.i(TAG, "onDialogClosed");
        super.onDialogClosed(positiveResult);
        persistBoolean(positiveResult);
    }
    
    @Override
    public void onClick (DialogInterface dialog, int which) {
        Log.i(TAG, "onClick");
        if (callback != null) callback.onResult(which);       
    }
    
    public void setResultCallback(PromptDialogPreferenceCallback callback) {
        Log.i(TAG, "setResultCallback");
        this.callback = callback;
    }
}
