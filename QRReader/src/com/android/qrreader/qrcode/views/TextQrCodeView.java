///////////////////////////////////////////////////////////////////////////////
// Project:    QR Reader for Android
// Package:    com.android.qrreader.qrcode.views
// File:       TextQrCodeView.java
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Contains the view adapter for adapting the Text QR code
//             to the proper view.
///////////////////////////////////////////////////////////////////////////////

package com.android.qrreader.qrcode.views;

import com.adapter.Adapter;
import com.android.qrreader.R;
import com.qrcode.qrcodes.TextQrCode;

import android.content.Context;
import android.content.Intent;
import android.content.res.Resources;
import android.net.Uri;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RelativeLayout;

/**
 * The Class TextQrCodeView adapts the Text QR code to the proper view.
 * 
 * @see com.qrcode.qrcodes.TextQrCode TextQrCode
 * @see com.adapter.Adapter Adapter
 * @see QrCodeViewProvider
 * 
 * @version 1.0
 */
public class TextQrCodeView implements Adapter {
    
    /**
     * This class implements the provider of view of the adapted Text QR code. 
     * 
     * @see QrCodeViewProvider
     * 
     * @version 1.0
     */
    public class TextQrCodeViewProvider implements QrCodeViewProvider {
        
        /** The QR code which is being adapted. */
        private TextQrCode qrCode;
        
        /** The context. */
        private Context context;
        
        /** The result view. */
        private RelativeLayout resultView;
        
        /** The text edit. */
        private EditText textEdit;

        /** The listener of the click on the send sms button. */
        private OnClickListener onClickSendSmsButton = new OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(Intent.ACTION_VIEW);
                intent.putExtra("sms_body", textEdit.getText().toString()); 
                intent.setData(Uri.parse("sms:"));
                context.startActivity(intent);
            }
        };
        
        /** The listener of the click on the send mail button. */
        private OnClickListener onClickSendMailButton = new OnClickListener() {
            @Override
            public void onClick(View v) {
                final Intent intent = new Intent(android.content.Intent.ACTION_SEND);
                
                // Sending the intent and hoping that there is some application for sending the e-mails
                intent.setType("plain/text");
                intent.putExtra(android.content.Intent.EXTRA_EMAIL, new String[]{""});
                intent.putExtra(android.content.Intent.EXTRA_SUBJECT, "");
                intent.putExtra(android.content.Intent.EXTRA_TEXT, textEdit.getText().toString());

                Resources res = context.getResources();
                context.startActivity(Intent.createChooser(intent,  res.getString(R.string.QrCode_Title_Mail_Send)));     
            }
        };
                
        /**
         * Instantiates a new Text QR code view provider.
         *
         * @param qrCode The QR code to be adapted.
         */
        private TextQrCodeViewProvider(TextQrCode qrCode) {
            this.qrCode = qrCode;
        }
        
        /** 
         * Gets the view for Text QR code.
         * 
         * @see com.android.qrreader.qrcode.views.QrCodeViewProvider#getView(android.content.Context) getView(Context)
         */
        public View getView(Context context) {
            if (resultView == null) { // Instantiate just once per this instance of the provider
                this.context = context;
                LayoutInflater resultInflater = LayoutInflater.from(context);
                resultView = (RelativeLayout) resultInflater.inflate(R.layout.openqr_view_text, null);
                
                // Getting references and setting on call click listener
                Button sendMailButton = (Button) resultView.findViewById(R.id.sendMailButton);
                Button sendSmsButton = (Button) resultView.findViewById(R.id.sendSmsButton);
                textEdit = (EditText) resultView.findViewById(R.id.textEdit);
                
                textEdit.setText(qrCode.getText());
                sendMailButton.setOnClickListener(onClickSendMailButton);
                sendSmsButton.setOnClickListener(onClickSendSmsButton);
                
            }
            
            return resultView;
        }
        
        /**
         * Gets the title name for Text QR code.
         * 
         * @see com.android.qrreader.qrcode.views.QrCodeViewProvider#getTitleName(Context)
         */
        public String getTitleName(Context context) {
            Resources res = context.getResources();
            return res.getString(R.string.QrCode_Title_Text);
        }
    };
    
    /**
     * Gets the view provider for Text QR code.
     * 
     * @see com.adapter.Adapter#getProvider(java.lang.Object) getProvider(Object)
     */
    @Override
    public Object getProvider(Object o) {
        return new TextQrCodeViewProvider((TextQrCode) o);
    }
    
    /**
     * Gets the TextQrCode class.
     * 
     * @see com.adapter.Adapter#getAdapteeClass() getAdapteeClass()
     */
    @Override
    public Class<?> getAdapteeClass() {
        return TextQrCode.class;
    }
    
    /**
     * Gets the QrCodeViewProvider class.
     * 
     * @see com.adapter.Adapter#getResultClass() getResultClass()
     */
    @Override
    public Class<?> getResultClass() {
        return QrCodeViewProvider.class;
    }

}

//End of the file