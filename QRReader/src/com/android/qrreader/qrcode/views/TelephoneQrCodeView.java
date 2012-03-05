///////////////////////////////////////////////////////////////////////////////
// Project:    QR Reader for Android
// Package:    com.android.qrreader.qrcode.views
// File:       TelephoneQrCodeView.java
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Contains the view adapter for adapting the Telephone QR code
//             to the proper view.
///////////////////////////////////////////////////////////////////////////////

package com.android.qrreader.qrcode.views;

import com.adapter.Adapter;
import com.android.qrreader.R;
import com.qrcode.qrcodes.TelephoneQrCode;

import android.content.Context;
import android.content.Intent;
import android.content.res.Resources;
import android.net.Uri;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.RelativeLayout;
import android.widget.TextView;

/**
 * The Class SmsQrCodeView adapts the Telephone QR code to the proper view.
 * 
 * @see com.qrcode.qrcodes.TelephoneQrCode TelephoneQrCode
 * @see com.adapter.Adapter Adapter
 * @see QrCodeViewProvider
 * 
 * @version 1.0
 */
public class TelephoneQrCodeView implements Adapter {
    
    /**
     * This class implements the provider of view of the adapted Telephone QR code. 
     * 
     * @see QrCodeViewProvider
     * 
     * @version 1.0
     */
    public class TelephoneQrCodeViewProvider implements QrCodeViewProvider {
        
        /** The URI schema for calling. */
        final private static String TEL_SCHEMA = "tel"; 
        
        /** The QR code which is being adapted. */
        private TelephoneQrCode qrCode;
        
        /** The context. */
        private Context context;
        
        /** The result view. */
        private RelativeLayout resultView;

        /** The listener of the click on the call button. */
        private OnClickListener onClickCallButton = new OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(Intent.ACTION_CALL);
                
                // Sending call intent and hoping that we will have lucky
                intent.setData(Uri.parse(TEL_SCHEMA + ":" + qrCode.getTelephone()));
                Resources res = context.getResources();
                context.startActivity(Intent.createChooser(intent,  res.getString(R.string.QrCode_Title_Telephone_Call)));    
            }
        };
                
        /**
         * Instantiates a new Telephone QR code view provider.
         *
         * @param qrCode The QR code to be adapted.
         */
        private TelephoneQrCodeViewProvider(TelephoneQrCode qrCode) {
            this.qrCode = qrCode;
        }
        
        /** 
         * Gets the view for Telephone QR code.
         * 
         * @see com.android.qrreader.qrcode.views.QrCodeViewProvider#getView(android.content.Context) getView(Context)
         */
        public View getView(Context context) {
            if (resultView == null) { // Instantiate just once per this instance of the provider
                this.context = context;
                LayoutInflater resultInflater = LayoutInflater.from(context);
                resultView = (RelativeLayout) resultInflater.inflate(R.layout.openqr_view_telephone, null);
                
                // Getting references and setting on call click listener
                Button callButton = (Button) resultView.findViewById(R.id.callButton);
                TextView telephoneNumber = (TextView)resultView.findViewById(R.id.telephoneNumber);
                telephoneNumber.setText(qrCode.getTelephone());
                callButton.setOnClickListener(onClickCallButton);
                
            }
            
            return resultView;
        }
        
        /**
         * Gets the title name for Telephone QR code.
         * 
         * @see com.android.qrreader.qrcode.views.QrCodeViewProvider#getTitleName(Context)
         */
        public String getTitleName(Context context) {
            Resources res = context.getResources();
            return res.getString(R.string.QrCode_Title_Telephone);
        }
    };
    
    /**
     * Gets the view provider for Telephone QR code.
     * 
     * @see com.adapter.Adapter#getProvider(java.lang.Object) getProvider(Object)
     */
    @Override
    public Object getProvider(Object o) {
        return new TelephoneQrCodeViewProvider((TelephoneQrCode) o);
    }
    
    /**
     * Gets the TelephoneQrCode class.
     * 
     * @see com.adapter.Adapter#getAdapteeClass() getAdapteeClass()
     */
    @Override
    public Class<?> getAdapteeClass() {
        return TelephoneQrCode.class;
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