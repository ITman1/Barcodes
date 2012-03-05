///////////////////////////////////////////////////////////////////////////////
// Project:    QR Reader for Android
// Package:    com.android.qrreader.qrcode.views
// File:       HttpLinkQrCodeView.java
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Contains the view adapter for adapting the HTTP link QR code
//             to the proper view.
///////////////////////////////////////////////////////////////////////////////

package com.android.qrreader.qrcode.views;

import java.util.regex.Pattern;

import android.content.Context;
import android.content.res.Resources;
import android.text.util.Linkify;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.adapter.Adapter;
import com.android.qrreader.R;
import com.qrcode.qrcodes.HttpLinkQrCode;

/**
 * The Class HttpLinkQrCodeView adapts the HTTP link QR code to the proper view.
 * 
 * @see com.qrcode.qrcodes.HttpLinkQrCode HttpLinkQrCode
 * @see com.adapter.Adapter Adapter
 * @see QrCodeViewProvider
 * 
 * @version 1.0
 */
public class HttpLinkQrCodeView implements Adapter {
    
    /**
     * This class implements the provider of view of the adapted HTTP link QR code. 
     * 
     * @see QrCodeViewProvider
     * 
     * @version 1.0
     */
    public class HttpLinkQrCodeViewProvider implements QrCodeViewProvider {
        
        /** The HTTP_SCHEME. */
        final private static String HTTP_SCHEME = "http";
        
        /** The QR code which is being adapted. */
        private HttpLinkQrCode qrCode;
        
        /** The result view. */
        private RelativeLayout resultView;
        
        /**
         * Instantiates a new HTTP link QR code view provider.
         *
         * @param qrCode The QR code to be adapted.
         */
        private HttpLinkQrCodeViewProvider(HttpLinkQrCode qrCode) {
            this.qrCode = qrCode;
        }
        
        /** 
         * Gets the view for HTTP link QR code.
         * 
         * @see com.android.qrreader.qrcode.views.QrCodeViewProvider#getView(android.content.Context) getView(Context)
         */
        public View getView(Context context) {
            if (resultView == null) { // Instantiate just once per this instance of the provider
                LayoutInflater resultInflater = LayoutInflater.from(context);
                resultView = (RelativeLayout) resultInflater.inflate(R.layout.openqr_view_url, null);
                
                // Displaying the HTTP link
                TextView urlLink = (TextView)resultView.findViewById(R.id.linkURL);
                urlLink.setText(qrCode.getLink().toExternalForm());
                Pattern pattern = Pattern.compile("^.*$");
                Linkify.addLinks(urlLink, pattern, HTTP_SCHEME);
            }

            return resultView;
        }
        
        /**
         * Gets the title name for HTTP link QR code.
         * 
         * @see com.android.qrreader.qrcode.views.QrCodeViewProvider#getTitleName(android.content.Context) getTitleName(Context)
         */
        public String getTitleName(Context context) {
            Resources res = context.getResources();
            return res.getString(R.string.QrCode_Title_Url);
        }
    };
    
    /**
     * Gets the view provider for HTTP link QR code.
     * 
     * @see com.adapter.Adapter#getProvider(java.lang.Object) getProvider(Object)
     */
    @Override
    public Object getProvider(Object o) {
        return new HttpLinkQrCodeViewProvider((HttpLinkQrCode) o);
    }
    
    /**
     * Gets the HttpLinkQrCode class.
     * 
     * @see com.adapter.Adapter#getAdapteeClass() getAdapteeClass()
     */
    @Override
    public Class<?> getAdapteeClass() {
        return HttpLinkQrCode.class;
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
