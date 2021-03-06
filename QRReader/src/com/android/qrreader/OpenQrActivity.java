///////////////////////////////////////////////////////////////////////////////
// Project:    QR Reader for Android
// Package:    com.android.qrreader
// File:       OpenQrActivity.java
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Contains the activity for opening and decoding the QR codes.
///////////////////////////////////////////////////////////////////////////////

package com.android.qrreader;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.StreamCorruptedException;
import java.util.ArrayList;
import java.util.Formatter;
import java.util.List;

import android.app.Activity;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.content.res.Resources;
import android.net.Uri;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.view.ViewParent;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.ImageView.ScaleType;
import android.widget.LinearLayout;
import android.widget.ScrollView;
import android.widget.TextView;
import android.widget.Toast;

import com.android.qrreader.R;
import com.android.qrreader.qrcode.InstallableQrCodeViewManager;
import com.android.qrreader.qrcode.InstallableQrDecoderManager;
import com.qrcode.QrCodes.DataSegments;
import com.qrcode.QrCodes.DataSegmentsFlags;
import com.qrcode.qrcodes.QrCode;

/**
 * The Class OpenQrActivity implements the activity which opens the QR code
 * saved in the file and tries to decode them via 
 * {@link com.android.qrreader.qrcode.InstallableQrDecoderManager InstallableQrDecoderManager} 
 * and then display their content where the correct view is found by 
 * {@link com.android.qrreader.qrcode.InstallableQrCodeViewManager InstallableQrCodeViewManager}.
 * <p>
 * This activity also serves for a decision whether to save the image or QR code.
 */
public class OpenQrActivity extends Activity {
    
    /**
     * The interface for a callback that is called every time when this activity is paused.
     */
    public interface OnPauseCallback {
        
        /**
         * This method is called every time when is this activity paused.
         */
        void onPause();
    }
    
    /** The extra data for including the path to decoded image 
     * and opening the preview inside this activity. */
    public static final String EXTRA_IMAGE                   = "EXTRA_IMAGE";
    
    /** The extra for letting know this activity that has been called by another 
     * activity from the application and not from eg file browser as an view intent. */
    public static final String EXTRA_INTERNAL_INTENT         = "EXTRA_INTERNAL_INTENT";
    
    /** The extra which adds the result button for saving the image. */
    public static final String EXTRA_ADD_SAVE_IMAGE_BUTTON   = "EXTRA_ADD_SAVE_IMAGE_BUTTON";
    
    /** The extra which adds the result button for saving the QR code. */
    public static final String EXTRA_ADD_SAVE_QRCODE_BUTTON  = "EXTRA_ADD_SAVE_QRCODE_BUTTON";
    
    /** The extra which adds the result button for saving the image and QR code. */
    public static final String EXTRA_ADD_SAVE_BOTH_BUTTON    = "EXTRA_ADD_SAVE_BOTH_BUTTON";
    
    /** The extra which adds the custom result button. */
    public static final String EXTRA_ADD_CUSTOM_BUTTONS      = "EXTRA_ADD_CUSTOM_BUTTON";
    
    /** The activity result flag that signals that close button has been clicked. */
    public static final int RESULT_CLOSE_BUTTON_CLICKED      = 0x01;
    
    /** The activity result flag that signals that save image button has been clicked.
     * {@link #EXTRA_ADD_SAVE_IMAGE_BUTTON}. */
    public static final int RESULT_SAVE_IMAGE_BUTTON_CLICKED = 0x02;
    
    /** The activity result flag that signals that save QR code button has been clicked.
     * {@link #EXTRA_ADD_SAVE_QRCODE_BUTTON}. */
    public static final int RESULT_SAVE_QRCODE_CLICKED       = 0x04;
    
    /** The activity result flag that signals that save both button has been clicked.
     * {@link #EXTRA_ADD_SAVE_BOTH_BUTTON}. */
    public static final int RESULT_SAVE_BOTH_BUTTON_CLICKED  = 0x08;
    
    /** The activity result flag that signals that custom button has been clicked.
     * {@link #EXTRA_ADD_CUSTOM_BUTTONS}. */
    public static final int RESULT_CUSTOM_BUTTON_CLICKED     = 0x0F;

    /** The maximal boundary after which are not added any other flags. */
    public static final int MAX_RESULT_CODES                 = 0xFF;
    
    /** The reference to the layout which contains the result buttons. */
    private LinearLayout buttonsLayout;
    
    /** The reference to the text view with the title of the result. */
    private TextView resultTitle;
    
    /** The reference to the icon which signals error control error. */
    private ImageView decodeWarning;
    
    /** The reference to the text view with the subtitle of the result. */
    private TextView resultSubTitle;
    
    /** The view where result of decoding is displayed. */
    private LinearLayout resultView;
    
    /** The image view for displaying the image preview. */
    private ImageView qrCodeImage;
    
    /** The registered on pause callbacks - {@link OnPauseCallback}. */
    private List<OnPauseCallback> OnPauseCallbacks = new ArrayList<OnPauseCallback>();
    
    /** Layout where subtitle is placed. */
    private LinearLayout resultSutitleLayout;
    
    /** The listener of the click on the some result button. */
    private OnClickListener resultButtonOnClick = new OnClickListener() {
        public void onClick(View v) {
            setResult(v.getId(), null);
            finish();
        }
    };
    
    /** The listener of the click on the some result button. */
    private OnClickListener decodeWarningClick = new OnClickListener() {
        public void onClick(View v) {
            Toast warningToast = Toast.makeText(OpenQrActivity.this, R.string.OpenQrActivity_Decode_Warning, Toast.LENGTH_SHORT);
            warningToast.show();
        }
    };
    
    /**
     * Called when the activity is firstly created.
     * Initializes the window and gets the references to the GUI elements.
     * Proceeds also the whole decoding of the QR image and attaching the 
     * result view.
     *
     * @see android.app.Activity#onCreate(android.os.Bundle) onCreate(Bundle) 
     */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        // Setting window features - landscape only when internal intent
        if (getIntent().getStringExtra(EXTRA_INTERNAL_INTENT) != null) {
            setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
        }
        
        // Inflating layout
        setContentView(R.layout.openqr_layout);  
        
        // Getting references for later use
        resultTitle = (TextView) findViewById(R.id.QRCodeResultTitle);
        resultSubTitle = (TextView) findViewById(R.id.QRCodeResultSubTitle);
        resultView = (LinearLayout) findViewById(R.id.QRCodeResult);
        qrCodeImage = (ImageView) findViewById(R.id.QRCodeImage);
        decodeWarning = (ImageView) findViewById(R.id.decodeWarning);
        resultSutitleLayout = (LinearLayout) findViewById(R.id.QRCodeResultSubTitleLinearLayout);
        decodeWarning.setOnClickListener(decodeWarningClick);
        
        // Inflating result buttons and inserting image if exists
        createResultButtons();
        insertQrCodeImage();
                        
        // Getting QR code decoder and QR view manager
        InstallableQrDecoderManager decoderManager =  InstallableQrDecoderManager.getDecoderManager(this);
        InstallableQrCodeViewManager viewManager = InstallableQrCodeViewManager.getViewManager(this);
        
        // Getting QR data from the intent
        DataSegments dataSegments = getQrCodeSegments();

        if (dataSegments == null || dataSegments.segments == null || dataSegments.segments.length == 0) {
            resultTitle.setText(R.string.OpenQrActivity_Title_QrNoFound);
            resultSubTitle.setVisibility(TextView.INVISIBLE);
            removeButton(RESULT_SAVE_QRCODE_CLICKED);
            removeButton(RESULT_SAVE_BOTH_BUTTON_CLICKED);
            return;
        } else {
            resultTitle.setText(R.string.OpenQrActivity_Title_QrFound);
        }
        resultTitle.requestFocusFromTouch();
        
        // Decoding QR code
        QrCode qrCode = decoderManager.decodeQrCode(dataSegments);
        if (dataSegments != null && dataSegments.flags == DataSegmentsFlags.DATA_SEGMENTS_CORRUPTED) {
            decodeWarning.setVisibility(ImageView.VISIBLE);
        } else {
            resultSutitleLayout.removeView(decodeWarning);
        }
        if (qrCode == null) {
            resultSubTitle.setText(R.string.OpenQrActivity_SubTitle_NoDecoder);
            displayRAW(dataSegments.toByteArray());
            return;
        }
        
        // Getting the view for the QR code
        View view = viewManager.getViewForQrCode(qrCode);
        if (view == null) {
            resultSubTitle.setText(R.string.OpenQrActivity_SubTitle_NoView);
            displayRAW(dataSegments.toByteArray());
            return;
        } else {
            resultSubTitle.setText(viewManager.getTitleForQrCode(qrCode));
            resultView.addView(view);
            ((ScrollView)findViewById(R.id.scrollView)).fullScroll(ScrollView.FOCUS_UP);
        }
    }
    
    //http://stackoverflow.com/questions/1949066/java-lang-outofmemoryerror-bitmap-size-exceeds-vm-budget-android
    private void unbindDrawables(View view) {
        if (view != null && view.getBackground() != null) {
        view.getBackground().setCallback(null);
        }
        if (view instanceof ViewGroup) {
            for (int i = 0; i < ((ViewGroup) view).getChildCount(); i++) {
            unbindDrawables(((ViewGroup) view).getChildAt(i));
            }
        ((ViewGroup) view).removeAllViews();
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    
        ViewParent parent = qrCodeImage.getParent();
        if (parent instanceof View) unbindDrawables((View)parent);
        System.gc();
    }

    
    /**
     * Called when activity is stopping. Fires the on pause callbacks
     * {@see #OnPauseCallbacks}.
     * 
     * @see android.app.Activity#onPause() onPause()
     */
    @Override
    protected void onPause() {
        super.onPause();
        for (OnPauseCallback callback : OnPauseCallbacks) {
            callback.onPause();
        }
    }
    
    /**
     * Registers the on pause callback - {@link OnPauseCallback}..
     *
     * @param callback the callback
     */
    public void registerOnPauseCallback(OnPauseCallback callback) {
        OnPauseCallbacks.add(callback);
    }
    
    /**
     * Releases the on pause callback - {@link OnPauseCallback}..
     *
     * @param callback the callback
     */
    public void releaseOnPauseCallback(OnPauseCallback callback) {
        OnPauseCallbacks.remove(callback);
    }
    
    /**
     * Inserts the preview of the image only if {@link #EXTRA_IMAGE} is attached.
     */
    private void insertQrCodeImage() {
        Intent intent = getIntent();
        String imagePath = intent.getStringExtra(EXTRA_IMAGE);
        
        if (imagePath != null) {    // There is probably some image, getting the
            File imageFile = new File(imagePath);
            if (imageFile.exists()) { // If exists open it from the URI
                qrCodeImage.setImageURI(Uri.fromFile(imageFile));
                qrCodeImage.setScaleType(ScaleType.FIT_XY);
                return;
            }
        }
        
        // There is no image preview, remove this view from the layout
        LinearLayout imageParent = (LinearLayout)(qrCodeImage.getParent());
        imageParent.removeView(qrCodeImage);       
    }
    
    /**
     * Converts the byte array to a hexadecimal string.
     *
     * @param bytes The array of bytes to be converted.
     * @return The converted array as a hexadecimal string.
     */
    private static String toHexString(byte[] bytes) {  
        StringBuilder stringBuilder = new StringBuilder(2 * bytes.length);  
      
        Formatter formatter = new Formatter(stringBuilder);  
        for (byte currByte : bytes) {  
            formatter.format(" %02x", currByte);  
        }  
      
        return stringBuilder.toString();  
    }  
    
    /**
     * Displays the decode result in the RAW.
     *
     * @param data the data
     */
    private void displayRAW(byte[] data) {
        LayoutInflater resultInflater = LayoutInflater.from(getBaseContext());
        LinearLayout rawResult = (LinearLayout) resultInflater.inflate(R.layout.openqr_rawresult, null);
        
        String rawData = toHexString(data);
        String rawText = new String(data);
        
        if (rawText.length() > 0) {
            ((TextView)rawResult.findViewById(R.id.rawText)).setText(rawText);
        }
        
        if (rawData.length() > 0) {
            ((TextView)rawResult.findViewById(R.id.rawData)).setText(rawData);
        }
        
        // Adding the on click listener of the Try luck! button
        // The click on this button sends and blindness intent where data
        // of the intent are the decoded text
        Button tryLuckButton = (Button)rawResult.findViewById(R.id.tryLuck);
        tryLuckButton.setTag(rawText);
        tryLuckButton.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent();
                
                // Sending intent and trying luck
                intent.setData(Uri.parse((String)v.getTag()));
                Resources res = OpenQrActivity.this.getResources();
                OpenQrActivity.this.startActivity(Intent.createChooser(intent,  res.getString(R.string.OpenQrActivity_RawResult_TryLuckWith)));   
            }
        });
 
                
        
        resultView.addView(rawResult);
    }
    
    /**
     * Reads the path to the QR code from the intent data and then reads this QR file.
     *
     * @return The read bytes of the QR code file.
     */
    private DataSegments getQrCodeSegments() {
        Uri data = getIntent().getData();
        File qrFile = new File(data.getPath());
        
        try {
            FileInputStream fis = new FileInputStream(qrFile);
            ObjectInputStream ois = new ObjectInputStream(fis);

            DataSegments dataSegments = (DataSegments) ois.readObject();
            ois.close();
            
            return dataSegments;
        } catch (FileNotFoundException e) {
        } catch (StreamCorruptedException e) {
        } catch (IOException e) {
        } catch (ClassNotFoundException e) {
        } catch (ClassCastException e) {
        }
        
        return null;
    }
    
    /**
     * Inserts one result button with the specified ID and text.
     *
     * @param id The ID of the button.
     * @param textResID The text which will be set on the button.
     */
    private void addButton(int id, int textResID) {
        LayoutInflater buttonInflater = LayoutInflater.from(getBaseContext());

        Button button = (Button) buttonInflater.inflate(R.layout.openqr_button_layout, null);
        button.setId(id);
        button.setText(textResID);
        button.setOnClickListener(resultButtonOnClick);
        buttonsLayout.addView(button);
    }
    
    /**
     * Removes one result button by specified ID.
     *
     * @param id The ID of the button.
     */
    private void removeButton(int id) {
        for (int i = 0; i < buttonsLayout.getChildCount(); i++) {
            View v = buttonsLayout.getChildAt(i);
            
            if (v instanceof Button && v.getId() == id) {
                buttonsLayout.removeView(v);
            }
        }
    }
        
    /**
     * Inserts the all result buttons into this activity.
     */
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
        
        // Inserting the custom buttons
        int i = 0;
        int[] customExtraButtons = intent.getIntArrayExtra(EXTRA_ADD_CUSTOM_BUTTONS);
        if (customExtraButtons != null) {
            for (int textResID : customExtraButtons) {
                addButton(RESULT_CUSTOM_BUTTON_CLICKED + MAX_RESULT_CODES + i, textResID);
                i++;
            }
        }
    }
    
    /**
     * Retrieves the clicked button from the result code retrieved by this activity.
     *
     * @param resultCode The result code to decode.
     * @return The clicked result button.
     */
    final public static int getClickedButtonID(int resultCode) {
        return resultCode - MAX_RESULT_CODES;
    }
}

//End of the file