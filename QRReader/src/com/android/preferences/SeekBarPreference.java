///////////////////////////////////////////////////////////////////////////////
// Project:    QR Reader for Android
// Package:    com.android.preferences
// File:       SeekBarPreference.java
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Contains the class which implements seekbar preference dialog.
///////////////////////////////////////////////////////////////////////////////

package com.android.preferences;

import android.content.Context;
import android.content.res.TypedArray;
import android.preference.DialogPreference;
import android.util.AttributeSet;
import android.view.View;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;
import android.widget.TextView;

import com.android.qrreader.R;

/**
 * The Class SeekBarPreference implements the dialog preference with the seek bar.
 * 
 * @see android.content.DialogInterface DialogPreference
 * 
 * @version 1.0
 */
public class SeekBarPreference extends DialogPreference {
    
    /** The default progress of the seek bar. */
    private static final int DEFAULT_PROGRESS = 25;
    
    /** The maximum progress of the seek bar. */
    private static final int MAX_PROGRESS     = 100;
    
    /** The actual progress of the seekbar. */
    private int currProgress                  = DEFAULT_PROGRESS;

    /** The reference to the seek bar. */
    private SeekBar seekBar;
    
    /** The progress text which is updated when seek bar is changed. */
    private TextView progressText;
    
    /** The message which is displayed on the dialog. */
    private TextView dialogMessage;
    
    /** The attribute of the maximum progress read from XML. */
    private int attr_maxProgress = MAX_PROGRESS;
    
    /** The attribute of the default progress read from XML. */
    private int attr_defaultProgress = DEFAULT_PROGRESS;
    
    /** The attribute of the progress text read from XML. */
    private String attr_progressText;
    
    /** The attribute of the dialog message read from XML. */
    private String attr_dialogMessage;
    
    /** The attribute of the possitive button text read from XML. */
    private String attr_positiveButtonText;
    
    /** The attribute of the negative button text read from XML. */
    private String attr_negativeButtonText;
       
    /** The listener of the change on the seek bar. */
    private OnSeekBarChangeListener onSeekBarChange = new OnSeekBarChangeListener() {
        @Override
        public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
            if(progressText != null) {
                // Updating progress text
                if (attr_progressText != null) {
                    progressText.setText(String.format("%d%s", progress, attr_progressText));
                } else {
                    progressText.setText(progress);
                }
            }
        }

        public void onStartTrackingTouch(SeekBar seekBar) {}
        public void onStopTrackingTouch(SeekBar seekBar) {}
    };

    /**
     * Instantiates a new seek bar preference.
     *
     * @param context The context.
     * @param attrs The attributes read from the XML document.
     * 
     * @see android.preference.DialogPreference#DialogPreference(Context, AttributeSet) DialogPreference
     */
    public SeekBarPreference(Context context, AttributeSet attrs) {
        super(context, attrs);
        TypedArray styledAttrs = context.obtainStyledAttributes(attrs, R.styleable.SeekBarPreference);
        
        // Inflating preference dialog layout
        setDialogLayoutResource(R.layout.seekbar_layout);
        
        // Getting XML attributes
        attr_maxProgress = styledAttrs.getInt(R.styleable.SeekBarPreference_maxProgress, MAX_PROGRESS);
        attr_defaultProgress = styledAttrs.getInt(R.styleable.SeekBarPreference_defaultProgress, DEFAULT_PROGRESS);
        attr_progressText = styledAttrs.getString(R.styleable.SeekBarPreference_progressText);
        attr_dialogMessage = styledAttrs.getString(R.styleable.SeekBarPreference_android_dialogMessage);
        attr_positiveButtonText = styledAttrs.getString(R.styleable.SeekBarPreference_android_positiveButtonText);
        attr_negativeButtonText = styledAttrs.getString(R.styleable.SeekBarPreference_android_negativeButtonText);
        
        // Sets the default value when shared preferences are being loaded
        setDefaultValue((Integer)attr_defaultProgress);
    }
    
    /** 
     * Ensures the setting of the default value after {@link android.preference.DialogPreference
     * #setDefaultValue(Object) setDefaultValue} is being called, reset of the shared 
     * preferences is required or no initial value is not set. 
     * 
     * @see android.preference.Preference#onSetInitialValue(boolean, java.lang.Object) onSetInitialValue
     */
    @Override
    protected void onSetInitialValue(boolean restorePersistedValue, Object defaultValue) {
        super.onSetInitialValue(restorePersistedValue, defaultValue);
        
        if (restorePersistedValue) {    // load the stored persisted value
            currProgress = shouldPersist() ? getPersistedInt(attr_defaultProgress) : attr_defaultProgress;
        } else {                        // reset or restore the default value
            currProgress = (Integer)defaultValue;
            if (shouldPersist()) persistInt(currProgress);
        }
    }
    
    /**
     * Create dialog callback. References to the views on the preference 
     * dialog are retrieving here.
     * 
     * @see android.preference.DialogPreference#onCreateDialogView() onCreateDialogView
     */
    @Override
    protected View onCreateDialogView () {
        View view = super.onCreateDialogView();
        seekBar = (SeekBar) view.findViewById(R.id.seekBar);
        progressText = (TextView) view.findViewById(R.id.progressText);
        dialogMessage = (TextView) view.findViewById(R.id.dialogMessage);
               
        return view;
    }
    
    /**
     * Bind dialog callback. Setting the UI elements is processed here.
     * 
     * @see android.preference.DialogPreference#onBindDialogView(android.view.View) onBindDialogView
     */
    @Override
    protected void onBindDialogView (View view) {
        super.onBindView(view);

        // Setting current progress
        currProgress = (shouldPersist())? getPersistedInt(currProgress) : currProgress;
        seekBar.setOnSeekBarChangeListener(onSeekBarChange);
        seekBar.setMax(attr_maxProgress);
        setProgress(currProgress);
        
        // Setting dialog message text
        dialogMessage.setText(attr_dialogMessage);
        
        // Setting positive button text
        if (attr_positiveButtonText != null) {
            setPositiveButtonText(attr_positiveButtonText);
        } else {
            setPositiveButtonText(android.R.string.ok);
        }
        
        // Setting negative button text
        if (attr_negativeButtonText != null) {
            setNegativeButtonText(attr_negativeButtonText);
        } else {
            setNegativeButtonText(android.R.string.cancel);
        }
    }
       
    /**
     * On close dialog callback. Persisting value is being proceeded here.
     * 
     * @see android.preference.DialogPreference#onDialogClosed(boolean) onDialogClosed
     */
    @Override
    protected void onDialogClosed(boolean positiveResult) {
        super.onDialogClosed(positiveResult);
        
        // On positive result store a new value
        if (positiveResult && shouldPersist()) {
            persistInt(seekBar.getProgress());
        }
    }
    
    /**
     * Sets the progress of the seek bar.
     *
     * @param progress The new progress to be set on the seek bar.
     */
    public void setProgress(int progress) {
        currProgress = progress;
        if (seekBar != null) {
            seekBar.setProgress(progress);
        }
    }
    
    /**
     * Gets the seek bar progress.
     *
     * @return The progress of the seek bar.
     */
    public int getProgress() {
        return currProgress;
    }
}

// End of the file
