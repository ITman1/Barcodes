///////////////////////////////////////////////////////////////////////////////
// Project:    QR Reader for Android
// Package:    com.android.qrreader.qrcode.views
// File:       ContactQrCodeView.java
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Contains the view adapter for adapting the contact QR code
//             to the proper view.
///////////////////////////////////////////////////////////////////////////////

package com.android.qrreader.qrcode.views;

import android.content.Context;
import android.content.res.Resources;
import android.view.View;

import com.adapter.Adapter;
import com.android.qrreader.R;
import com.qrcode.qrcodes.ContactQrCode;

// TODO: Auto-generated Javadoc (X)
// TODO: Implement Contact adapter
/**
 * The Class ContactQrCodeView.
 */
public class ContactQrCodeView implements Adapter {
    
    /**
     * The Class ContactQrCodeViewProvider.
     */
    public class ContactQrCodeViewProvider implements QrCodeViewProvider {
        
        /** The qr code. */
        private ContactQrCode qrCode;
        
        /**
         * Instantiates a new contact qr code view provider.
         *
         * @param qrCode the qr code
         */
        private ContactQrCodeViewProvider(ContactQrCode qrCode) {
            this.qrCode = qrCode;
        }
        
        /* (non-Javadoc)
         * @see com.android.qrreader.qrcode.views.QrCodeViewProvider#getView(android.content.Context)
         */
        public View getView(Context context) {
            View view = new View(context);
            qrCode.toString(); // TODO  - delete this / just preventing not used warning
            return view;
        }
        
        /* (non-Javadoc)
         * @see com.android.qrreader.qrcode.views.QrCodeViewProvider#getTitleName(android.content.Context)
         */
        public String getTitleName(Context context) {
            Resources res = context.getResources();
            return res.getString(R.string.QrCode_Title_Contact);
        }
    };
    
    /* (non-Javadoc)
     * @see com.adapter.Adapter#getProvider(java.lang.Object)
     */
    @Override
    public Object getProvider(Object o) {
        return new ContactQrCodeViewProvider((ContactQrCode) o);
    }
    
    /* (non-Javadoc)
     * @see com.adapter.Adapter#getAdapteeClass()
     */
    @Override
    public Class<?> getAdapteeClass() {
        return ContactQrCode.class;
    }
    
    /* (non-Javadoc)
     * @see com.adapter.Adapter#getResultClass()
     */
    @Override
    public Class<?> getResultClass() {
        return QrCodeViewProvider.class;
    }
}

//End of the file
