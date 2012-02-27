package com.android.qrreader;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;

public class QrReaderOpenQrActivity extends Activity {
    public static final String EXTRA_IMAGE                   = "EXTRA_IMAGE";
    public static final String EXTRA_ADD_BUTTON              = "EXTRA_ADD_BUTTON";
    
    public static final int RESULT_CLOSE_BUTTON_CLICKED      = 0x01;
    public static final int RESULT_EXTRA_BUTTON_CLICKED      = 0x02;

    public static final int MAX_RESULT_CODES                 = 0xFF;
    
    private Button closeButton;
    
    private OnClickListener closeButtonOnClick = new OnClickListener() {
        public void onClick(View v) {
            setResult(RESULT_CLOSE_BUTTON_CLICKED, null);
            finish();
        }
    };
    
    @Override
    public void onCreate(Bundle savedInstanceState) {
        setContentView(R.layout.openqr_layout);
        
        closeButton = (Button) findViewById(R.id.closeButton);
        closeButton.setOnClickListener(closeButtonOnClick);
    }
    
    final public static int getClickedButtonID(int resultCode) {
        return resultCode - MAX_RESULT_CODES;
    }
}
