///////////////////////////////////////////////////////////////////////////////
// Project:    QR Reader for Android
// Package:    com.android.preferences
// File:       SeekBarPreference.java
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Contains the class which extends edit text about real time 
//             validating of the input text.
///////////////////////////////////////////////////////////////////////////////

package com.android.preferences;

import android.app.AlertDialog;
import android.app.Dialog;
import android.content.Context;
import android.os.Bundle;
import android.preference.EditTextPreference;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.AttributeSet;
import android.util.Log;

/**
 * The Class ValidateEditTextPreference extends the {@link android.preference.
 * EditTextPreference EditTextPreference} about validating the input text in real time.
 * 
 * @see android.preference.EditTextPreference EditTextPreference
 */
public class ValidateEditTextPreference extends EditTextPreference {
    
    /** The Constant Debug TAG. */
    private static final String TAG = "com.android.preferences::ValidateEditTextPreference";
    
    /**
     * The interface for validate callback.
     */
    public interface ValidateCallback {
        
        /**
         * Validate method which is invoked every time when input text changes.
         *
         * @param dialog The reference to the preference dialog which can be then 
         * customized in the dependence of the changes of the edit text.
         * @param str Contains a new text of the edit text.
         */
        public void validate(AlertDialog dialog, Editable str);
    }
    
    /** The validate callback to be called if is not null. */
    private ValidateCallback validateCallback;
    
    /**
     * Instantiates a new validate edit text preference.
     *
     * @param ctx The context.
     * @param attrs The attributes read from the XML.
     * 
     * @see android.preference.EditTextPreference#EditTextPreference(Context, AttributeSet) EditTextPreference
     */
    public ValidateEditTextPreference(Context ctx, AttributeSet attrs) {
        super(ctx, attrs);        
        Log.i(TAG, "ValidateEditTextPreference");
    }
    
    /**
     * Sets the validate callback which is called every time when edit text
     * is changed on the dialog.
     *
     * @param callback The new validate callback.
     */
    public void setValidateCallback(ValidateCallback callback) {
        Log.i(TAG, "setValidateCallback");
        validateCallback = callback;
    }
    
    /** The text listeners set on the dialog edit text. */
    private TextWatcher textWatcher = new TextWatcher() {
        public void afterTextChanged(Editable s) {
            callValidateCallback(s);    // Call validate callback
        }
     
        public void beforeTextChanged(CharSequence s, int start, int count, int after) {}
     
        public void onTextChanged(CharSequence s, int start, int before, int count) {}
    };
    
    /**
     * Calls the validate callback.
     *
     * @param s The new text to be sent for validation.
     */
    private void callValidateCallback(Editable s) {
        Log.i(TAG, "callValidateCallback");
        
        if (validateCallback != null) {
            Dialog dialog = getDialog();
            // Well just trying luck and supposing that preference dialog 
            // will be always the instance of the alert dialog :(
            if (dialog instanceof AlertDialog) {
                validateCallback.validate((AlertDialog)dialog, s);          
            }
        }
    }
    
    /**
     * The method which is called for showing the dialog. Here will be 
     * firstly fired the on validate callback.
     * 
     * @see android.preference.DialogPreference#showDialog(android.os.Bundle) showDialog
     */
    @Override
    protected void showDialog(Bundle state) {
        Log.i(TAG, "showDialog");
        super.showDialog(state);

        getEditText().removeTextChangedListener(textWatcher);
        getEditText().addTextChangedListener(textWatcher);
        callValidateCallback(getEditText().getText());
    }    
}

//End of the file