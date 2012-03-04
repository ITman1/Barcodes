///////////////////////////////////////////////////////////////////////////////
// Project:    QR Reader for Android
// Package:    com.android.preferences
// File:       PromptDialogPreference.java
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Contains the class which implements the prompt dialog preference.
///////////////////////////////////////////////////////////////////////////////

package com.android.preferences;

import android.content.Context;
import android.content.DialogInterface;
import android.preference.DialogPreference;
import android.util.AttributeSet;
import android.util.Log;

/**
 * The Class PromptDialogPreference implements the preference prompt dialog.
 * 
 * @see android.content.DialogInterface DialogPreference
 */
public class PromptDialogPreference extends DialogPreference {
    
    /** The Constant Debug TAG. */
    private static final String TAG = "com.android.preferences::PromptDialogPreference";
    
    /**
     * The Interface which serves as the callback interface 
     * when the result button of the dialog has been clicked.
     */
    public interface PromptDialogPreferenceCallback {
        
        /**
         * Callback method which is called when the result button is clicked.
         *
         * @param which The which button has been clicked.
         */
        public void onResult(int which);
    }
    
    /** Stores the result callback. */
    private PromptDialogPreferenceCallback callback;

    /**
     * Instantiates a new prompt dialog preference.
     *
     * @param context the context
     * @param attrs the attrs
     */
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
    
    /**
     * Sets the result callback which is initiated when result button is clicked.
     *
     * @param callback The result callback.
     */
    public void setResultCallback(PromptDialogPreferenceCallback callback) {
        Log.i(TAG, "setResultCallback");
        this.callback = callback;
    }
}

//End of the file