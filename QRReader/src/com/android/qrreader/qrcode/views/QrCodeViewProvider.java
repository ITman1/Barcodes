///////////////////////////////////////////////////////////////////////////////
// Project:    QR Reader for Android
// Package:    com.android.qrreader.qrcode.views
// File:       QrCodeViewProvider.java
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Contains the interface for view provider which is returned by
//             the adapter which adapts QR codes to the views.
///////////////////////////////////////////////////////////////////////////////

package com.android.qrreader.qrcode.views;

import android.content.Context;
import android.view.View;

/**
 * The interface QrCodeViewProvider serves for providing the UI views and
 * title names for specific QR code objects. This interface implements 
 * the adapters which does the adapting of the QR codes to the views. 
 * 
 * @see com.adapter.Adapter Adapter
 * @see android.view.View View
 * 
 * @version 1.0
 */
public interface QrCodeViewProvider {
    
    /**
     * Gets the view for this QR code.
     *
     * @param context The context.
     * @return The view for QR code.
     */
    public View getView(Context context);
    
    /**
     * Gets the title name for adapted QR code.
     *
     * @param context The context.
     * @return The title name for QR code.
     */
    public String getTitleName(Context context);
}

//End of the file
