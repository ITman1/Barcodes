///////////////////////////////////////////////////////////////////////////////
// Project:    QR Reader for Android
// Package:    com.android.qrreader.qrcode.views
// File:       HttpLinkQrCodeView.java
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Contains the view adapter for adapting the Mail QR code
//             to the proper view.
///////////////////////////////////////////////////////////////////////////////

package com.android.qrreader.qrcode.views;

import android.content.Context;
import android.content.Intent;
import android.content.res.Resources;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RelativeLayout;

import com.adapter.Adapter;
import com.android.qrreader.R;
import com.qrcode.qrcodes.MailQrCode;

/**
 * The Class MailQrCodeView adapts the Mail QR code to the proper view.
 * 
 * @see com.qrcode.qrcodes.MailQrCode MailQrCode
 * @see com.adapter.Adapter Adapter
 * @see QrCodeViewProvider
 * 
 * @version 1.0
 */
public class MailQrCodeView  implements Adapter {
    
    /**
     * This class implements the provider of view of the adapted Mail QR code. 
     * 
     * @see QrCodeViewProvider
     * 
     * @version 1.0
     */
    public class MailQrCodeViewProvider implements QrCodeViewProvider {
        
        /** The QR code which is being adapted. */
        private MailQrCode qrCode;
        
        /** The context. */
        private Context context;
        
        /** The result view. */
        private RelativeLayout resultView;
        
        /** The mail receiver edit. */
        private EditText receiverEdit;
        
        /** The mail subject edit. */
        private EditText subjectEdit;
        
        /** The mail body edit. */
        private EditText bodyEdit;
        
        /** The listener of the click on the send mail button. */
        private OnClickListener onClickSendMail = new OnClickListener() {
            @Override
            public void onClick(View v) {
                final Intent intent = new Intent(android.content.Intent.ACTION_SEND);
                
                // Sending the intent and hoping that there is some application for sending the e-mails
                intent.setType("plain/text");
                intent.putExtra(android.content.Intent.EXTRA_EMAIL, new String[]{ receiverEdit.getText().toString()});
                intent.putExtra(android.content.Intent.EXTRA_SUBJECT, subjectEdit.getText().toString());
                intent.putExtra(android.content.Intent.EXTRA_TEXT, bodyEdit.getText().toString());

                Resources res = context.getResources();
                context.startActivity(Intent.createChooser(intent,  res.getString(R.string.QrCode_Title_Mail_Send)));                
            }
        };
                
        /**
         * Instantiates a new Mail QR code view provider.
         *
         * @param qrCode The QR code to be adapted.
         */
        private MailQrCodeViewProvider(MailQrCode qrCode) {
            this.qrCode = qrCode;
        }
        
        /** 
         * Gets the view for mail QR code.
         * 
         * @see com.android.qrreader.qrcode.views.QrCodeViewProvider#getView(android.content.Context) getView(Context)
         */
        public View getView(Context context) {
            if (resultView == null) { // Instantiate just once per this instance of the provider
                this.context = context;
                LayoutInflater resultInflater = LayoutInflater.from(context);
                resultView = (RelativeLayout) resultInflater.inflate(R.layout.openqr_view_mail, null);
                
                receiverEdit = (EditText)resultView.findViewById(R.id.receiverEdit);
                subjectEdit = (EditText)resultView.findViewById(R.id.subjectEdit);
                bodyEdit = (EditText)resultView.findViewById(R.id.bodyEdit);
                Button sendMail = (Button) resultView.findViewById(R.id.sendMail);
                
                sendMail.setOnClickListener(onClickSendMail);
                
                subjectEdit.setText(qrCode.getSubject());
                receiverEdit.setText(qrCode.getReceiver());                
                bodyEdit.setText(qrCode.getBody());
            }
            
            return resultView;
        }
        
        /**
         * Gets the title name for Mail QR code.
         * 
         * @see com.android.qrreader.qrcode.views.QrCodeViewProvider#getTitleName(Context)
         */
        public String getTitleName(Context context) {
            Resources res = context.getResources();
            return res.getString(R.string.QrCode_Title_Mail);
        }
    };
    
    /**
     * Gets the view provider for Mail QR code.
     * 
     * @see com.adapter.Adapter#getProvider(java.lang.Object) getProvider(Object)
     */
    @Override
    public Object getProvider(Object o) {
        return new MailQrCodeViewProvider((MailQrCode) o);
    }
    
    /**
     * Gets the MailQrCode class.
     * 
     * @see com.adapter.Adapter#getAdapteeClass() getAdapteeClass()
     */
    @Override
    public Class<?> getAdapteeClass() {
        return MailQrCode.class;
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
