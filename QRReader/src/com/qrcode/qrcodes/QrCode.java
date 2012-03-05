///////////////////////////////////////////////////////////////////////////////
// Project:    QR Reader for Android
// Package:    com.qrcode.qrcodes
// File:       QrCode.java
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Contains abstract class of the QR code.
///////////////////////////////////////////////////////////////////////////////

package com.qrcode.qrcodes;

import com.qrcode.QrCodes;

/**
 * The abstract class of the QR code which extend all QR codes.
 */
public abstract class QrCode {
    
    /**
     * Encodes this QR code.
     *
     * @param encoder The encoder by which should be the QR code encoded.
     * @return The serialized QR code as the byte array.
     * 
     * @see com.qrcode.QrEncoderManager#encodeQrCode(QrCode, Class) encodeQrCode(QrCode, Class)
     */
    public byte[] encode(Class<?> encoder) {
        return QrCodes.encodeQrCode(this, encoder);
    }
}

//End of the file