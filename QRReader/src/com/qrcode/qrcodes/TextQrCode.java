///////////////////////////////////////////////////////////////////////////////
// Project:    QR Reader for Android
// Package:    com.qrcode.qrcodes
// File:       TextQrCode.java
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Contains class which implements the Text QR code.
///////////////////////////////////////////////////////////////////////////////

package com.qrcode.qrcodes;

/**
 * The Class TextQrCode implements the text QR code.
 */
public class TextQrCode extends QrCode {
    
    /** The text. */
    private String text;
    
    /**
     * Sets the text.
     *
     * @param text The text.
     * @return True, if successful, otherwise false.
     */
    public boolean setText(String text) {
        this.text = text;
        return true;
    }
    
    /**
     * Gets the text.
     *
     * @return The text.
     */
    public String getText() {
        return text;
    }

}

//End of the file