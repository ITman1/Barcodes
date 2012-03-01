package com.android.qrreader;

import java.io.File;
import java.io.IOException;
import java.util.Formatter;

import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.Window;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ScrollView;
import android.widget.TextView;

import com.android.qrreader.R;
import com.android.qrreader.qrcode.InstallableQrCodeViewManager;
import com.android.qrreader.qrcode.InstallableQrDecoderManager;
import com.filesystem.Operations;
import com.qrcode.qrcodes.QrCode;

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
    private TextView resultTitle;
    private TextView resultSubTitle;
    private LinearLayout resultView;
    private ImageView qrCodeImage;
    
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
        
        // Inflating layout
        setContentView(R.layout.openqr_layout);  
        
        // Getting references for later use
        resultTitle = (TextView) findViewById(R.id.QRCodeResultTitle);
        resultSubTitle = (TextView) findViewById(R.id.QRCodeResultSubTitle);
        resultView = (LinearLayout) findViewById(R.id.QRCodeResult);
        qrCodeImage = (ImageView) findViewById(R.id.QRCodeImage);
        
        // Inflating result buttons and inserting image if exists
        createResultButtons();
        insertQrCodeImage();
                        
        // Getting QR code decoder and QR view manager
        InstallableQrDecoderManager decoderManager =  InstallableQrDecoderManager.getDecoderManager(this);
        InstallableQrCodeViewManager viewManager = InstallableQrCodeViewManager.getViewManager(this);
        
        // Getting QR data from the intent
        byte[] qrCodeData = getQrCodeData();
        if (qrCodeData == null) {
            resultTitle.setText(R.string.OpenQrActivity_Title_QrNoFound);
            return;
        } else {
            resultTitle.setText(R.string.OpenQrActivity_Title_QrFound);
        }
        resultTitle.requestFocusFromTouch();
        
        // Decoding QR code
        QrCode qrCode = decoderManager.decodeQrCode(qrCodeData);
        if (qrCode == null) {
            resultSubTitle.setText(R.string.OpenQrActivity_SubTitle_NoDecoder);
            displayRAW(qrCodeData);
            return;
        }
        
        // Getting the view for the QR code
        View view = viewManager.getViewForQrCode(qrCode);
        if (view == null) {
            resultSubTitle.setText(R.string.OpenQrActivity_SubTitle_NoView);
            displayRAW(qrCodeData);
            return;
        } else {
            resultSubTitle.setText(viewManager.getTitleForQrCode(qrCode));
            resultView.addView(view);
            ((ScrollView)findViewById(R.id.scrollView)).fullScroll(ScrollView.FOCUS_UP);
        }
    }
    
    private void insertQrCodeImage() {
        Intent intent = getIntent();
        String imagePath = intent.getStringExtra(EXTRA_IMAGE);
        
        if (imagePath != null) {
            File imageFile = new File(imagePath);
            if (imageFile.exists()) {
                qrCodeImage.setImageURI(Uri.fromFile(imageFile));
                return;
            }
        }
        LinearLayout imageParent = (LinearLayout)(qrCodeImage.getParent());
        imageParent.removeView(qrCodeImage);       
    }
    
    private static String toHexString(byte[] bytes) {  
        StringBuilder stringBuilder = new StringBuilder(2 * bytes.length);  
      
        Formatter formatter = new Formatter(stringBuilder);  
        for (byte currByte : bytes) {  
            formatter.format(" %02x", currByte);  
        }  
      
        return stringBuilder.toString();  
    }  
    
    private void displayRAW(byte[] data) {
        LayoutInflater resultInflater = LayoutInflater.from(getBaseContext());
        LinearLayout rawResult = (LinearLayout) resultInflater.inflate(R.layout.openqr_rawresult, null);
        
        String rawData = toHexString(data);
        String rawText = new String(data);
        
        if (!rawText.isEmpty()) {
            ((TextView)rawResult.findViewById(R.id.rawText)).setText(rawText);
        }
        
        if (!rawData.isEmpty()) {
            ((TextView)rawResult.findViewById(R.id.rawData)).setText(rawData);
        }
        
        resultView.addView(rawResult);
    }
    
    private byte[] getQrCodeData() {
        Uri data = getIntent().getData();
        File qrFile = new File(data.getPath());
        
        try {
            return Operations.readFile(qrFile);
        } catch (IOException e) {
            e.printStackTrace();
            return null;
        }
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
