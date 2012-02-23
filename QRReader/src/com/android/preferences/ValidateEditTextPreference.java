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

public class ValidateEditTextPreference extends EditTextPreference {
    private static final String TAG = "com.android.preferences::ValidateEditTextPreference";
    
    public interface ValidateCallback {
        public void validate(AlertDialog dialog, Editable str);
    }
    
    private ValidateCallback validateCallback;
    
    public ValidateEditTextPreference(Context ctx, AttributeSet attrs, int defStyle) {
        super(ctx, attrs, defStyle);
        Log.i(TAG, "ValidateEditTextPreference");
    }

    public ValidateEditTextPreference(Context ctx, AttributeSet attrs) {
        super(ctx, attrs);        
        Log.i(TAG, "ValidateEditTextPreference");
    }
    
    public void setValidateCallback(ValidateCallback callback) {
        Log.i(TAG, "setValidateCallback");
        validateCallback = callback;
    }
    
    private TextWatcher textWatcher = new TextWatcher() {
        public void afterTextChanged(Editable s) {
            callValidateCallback(s);
        }
     
        public void beforeTextChanged(CharSequence s, int start, int count, int after) {}
     
        public void onTextChanged(CharSequence s, int start, int before, int count) {}
    };
    
    private void callValidateCallback(Editable s) {
        Log.i(TAG, "callValidateCallback");
        if (validateCallback != null) {
            Dialog dialog = getDialog();
            if (dialog instanceof AlertDialog) {
                validateCallback.validate((AlertDialog)dialog, s);          
            }
        }
    }
    
    @Override
    protected void showDialog(Bundle state) {
        Log.i(TAG, "showDialog");
        super.showDialog(state);

        getEditText().removeTextChangedListener(textWatcher);
        getEditText().addTextChangedListener(textWatcher);
        callValidateCallback(getEditText().getText());
    }    
}

