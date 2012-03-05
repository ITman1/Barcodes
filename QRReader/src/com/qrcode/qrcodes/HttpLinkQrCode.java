///////////////////////////////////////////////////////////////////////////////
// Project:    QR Reader for Android
// Package:    com.qrcode.qrcodes
// File:       HttpLinkQrCode.java
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Contains class which implements the HTTP link QR code.
///////////////////////////////////////////////////////////////////////////////

package com.qrcode.qrcodes;

import java.net.URL;

/**
 * The Class HttpLinkQrCode implements the HTTP link QR code.
 */
public class HttpLinkQrCode extends QrCode {    
    
    /** The URL schema of the link. */
    final private static String HTTP_SCHEMA = "http";
    
    /** The stored link. */
    private URL link;

    /**
     * Sets the URL link.
     *
     * @param link the link
     * @return True, if successful, otherwise false.
     */
    public boolean setLink(URL link) {
        if (link.getProtocol().equalsIgnoreCase(HTTP_SCHEMA)) {
            this.link = link;
            return true;
        } else {
            return false;
        }
    }
    
    /**
     * Gets the URL link.
     *
     * @return The URL link.
     */
    public URL getLink() {
        return link;
    }
    
    /**
     * Returns the link as a string.
     * 
     * @return The link as a string. 
     */
    @Override
    public String toString() {
        return link.toExternalForm();
    }
}

//End of the file