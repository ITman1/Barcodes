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

public class SeekBarPreference extends DialogPreference {
    private static final int DEFAULT_PROGRESS = 25;
    private static final int MAX_PROGRESS     = 100;
    
    private int currProgress                  = DEFAULT_PROGRESS;

    private SeekBar seekBar;
    private TextView progressText;
    private TextView dialogMessage;
    
    private int attr_maxProgress = MAX_PROGRESS;
    private int attr_defaultProgress = DEFAULT_PROGRESS;
    private String attr_progressText;
    private String attr_dialogMessage;
    private String attr_positiveButtonText;
    private String attr_negativeButtonText;
       
    private OnSeekBarChangeListener onSeekBarChange = new OnSeekBarChangeListener() {
        @Override
        public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
            if(progressText != null) {
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

    public SeekBarPreference(Context context, AttributeSet attrs) {
        super(context, attrs);
        TypedArray styledAttrs = context.obtainStyledAttributes(attrs, R.styleable.SeekBarPreference);
        
        setDialogLayoutResource(R.layout.seekbar_layout);
        
        attr_maxProgress = styledAttrs.getInt(R.styleable.SeekBarPreference_maxProgress, MAX_PROGRESS);
        attr_defaultProgress = styledAttrs.getInt(R.styleable.SeekBarPreference_defaultProgress, DEFAULT_PROGRESS);
        attr_progressText = styledAttrs.getString(R.styleable.SeekBarPreference_progressText);
        attr_dialogMessage = styledAttrs.getString(R.styleable.SeekBarPreference_android_dialogMessage);
        attr_positiveButtonText = styledAttrs.getString(R.styleable.SeekBarPreference_android_positiveButtonText);
        attr_negativeButtonText = styledAttrs.getString(R.styleable.SeekBarPreference_android_negativeButtonText);
        
        setDefaultValue((Integer)attr_defaultProgress);
    }
    
    @Override
    protected void onSetInitialValue(boolean restorePersistedValue, Object defaultValue) {
        super.onSetInitialValue(restorePersistedValue, defaultValue);
        
        if (restorePersistedValue) {
            currProgress = shouldPersist() ? getPersistedInt(attr_defaultProgress) : attr_defaultProgress;
        } else {
            currProgress = (Integer)defaultValue;
            if (shouldPersist()) persistInt(currProgress);
        }
    }
    
    @Override
    protected View onCreateDialogView () {
        View view = super.onCreateDialogView();
        seekBar = (SeekBar) view.findViewById(R.id.seekBar);
        progressText = (TextView) view.findViewById(R.id.progressText);
        dialogMessage = (TextView) view.findViewById(R.id.dialogMessage);
               
        return view;
    }
    
    @Override
    protected void onBindDialogView (View view) {
        super.onBindView(view);

        currProgress = (shouldPersist())? getPersistedInt(currProgress) : currProgress;
        
        seekBar.setOnSeekBarChangeListener(onSeekBarChange);
        seekBar.setMax(attr_maxProgress);
        setProgress(currProgress);
        
        dialogMessage.setText(attr_dialogMessage);
        
        if (attr_positiveButtonText != null) {
            setPositiveButtonText(attr_positiveButtonText);
        } else {
            setPositiveButtonText(android.R.string.ok);
        }
        
        if (attr_negativeButtonText != null) {
            setNegativeButtonText(attr_negativeButtonText);
        } else {
            setNegativeButtonText(android.R.string.cancel);
        }
    }
       
    @Override
    protected void onDialogClosed(boolean positiveResult) {
        super.onDialogClosed(positiveResult);
        
        if (positiveResult && shouldPersist()) {
            persistInt(seekBar.getProgress());
        }
    }
    
    public void setProgress(int progress) {
        currProgress = progress;
        if (seekBar != null) {
            seekBar.setProgress(progress);
        }
    }
    
    public int getProgress() {
        return currProgress;
    }

}
