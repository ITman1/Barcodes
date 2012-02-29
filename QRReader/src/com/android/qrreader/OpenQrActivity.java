package com.android.qrreader;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.Window;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.LinearLayout;
import com.android.qrreader.R;

public class OpenQrActivity extends Activity {
    public static final String EXTRA_IMAGE                   = "EXTRA_IMAGE";
    
    public static final String EXTRA_ADD_SAVE_IMAGE_BUTTON   = "EXTRA_ADD_SAVE_IMAGE_BUTTON";
    public static final String EXTRA_ADD_SAVE_QRCODE_BUTTON  = "EXTRA_ADD_SAVE_QRCODE_BUTTON";
    public static final String EXTRA_ADD_SAVE_BOTH_BUTTON    = "EXTRA_ADD_SAVE_BOTH_BUTTON";
    public static final String EXTRA_ADD_CUSTOM_BUTTONS      = "EXTRA_ADD_CUSTOM_BUTTON";
    
    public static final int RESULT_CLOSE_BUTTON_CLICKED      = 0x01;
    public static final int RESULT_SAVE_IMAGE_BUTTON_CLICKED = 0x02;
    public static final int RESULT_SAVE_QRCODE_CLICKED       = 0x04;
    public static final int RESULT_SAVE_BOTH_BUTTON_CLICKED  = 0x08;
    public static final int RESULT_CUSTOM_BUTTON_CLICKED     = 0x0F;

    public static final int MAX_RESULT_CODES                 = 0xFF;
    
    private LinearLayout buttonsLayout;
    
    private OnClickListener resultButtonOnClick = new OnClickListener() {
        public void onClick(View v) {
            setResult(v.getId(), null);
            finish();
        }
    };
    
    @Override
    public void onCreate(Bundle savedInstanceState) {
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        super.onCreate(savedInstanceState);
        
        setContentView(R.layout.openqr_layout);
               
        createResultButtons();
        
        InstallableQrDecoderManager manager =  InstallableQrDecoderManager.getDecoderManager(this);
        
        /*byte[] qrCodeData = getQrCodeData(getIntent().getData());
        
        QrCode qrCode = QrCodes*/
    }
    
    private void addButton(int id, int textResID) {
        LayoutInflater buttonInflater = LayoutInflater.from(getBaseContext());

        Button button = (Button) buttonInflater.inflate(R.layout.openqr_button_layout, null);
        button.setId(id);
        button.setText(textResID);
        button.setOnClickListener(resultButtonOnClick);
        buttonsLayout.addView(button);
    }
        
    private void createResultButtons() {
        Intent intent = getIntent();
        buttonsLayout = (LinearLayout)findViewById(R.id.buttonsLayout);
        
        if (intent.getStringExtra(EXTRA_ADD_SAVE_IMAGE_BUTTON) != null) 
            addButton(RESULT_SAVE_IMAGE_BUTTON_CLICKED, R.string.QRReaderOpenQrActivity_SaveImageButtonText);
        
        if (intent.getStringExtra(EXTRA_ADD_SAVE_QRCODE_BUTTON) != null) 
            addButton(RESULT_SAVE_QRCODE_CLICKED, R.string.QRReaderOpenQrActivity_SaveQRCodeButtonText);
        
        if (intent.getStringExtra(EXTRA_ADD_SAVE_BOTH_BUTTON) != null) 
            addButton(RESULT_SAVE_BOTH_BUTTON_CLICKED, R.string.QRReaderOpenQrActivity_SaveBothButtonText);
        
        addButton(RESULT_CLOSE_BUTTON_CLICKED, R.string.close);
        
        int i = 0;
        int[] customExtraButtons = intent.getIntArrayExtra(EXTRA_ADD_CUSTOM_BUTTONS);
        if (customExtraButtons != null) {
            for (int textResID : customExtraButtons) {
                addButton(RESULT_CUSTOM_BUTTON_CLICKED + MAX_RESULT_CODES + i, textResID);
                i++;
            }
        }
    }
    
    final public static int getClickedButtonID(int resultCode) {
        return resultCode - MAX_RESULT_CODES;
    }
}
