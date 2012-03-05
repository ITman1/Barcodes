///////////////////////////////////////////////////////////////////////////////
// Project:    QR Reader for Android
// Package:    com.qrcode
// File:       QrCodes.java
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Contains the class which groups major the methods for 
//             decoding/encoding QR codes.
///////////////////////////////////////////////////////////////////////////////

package com.qrcode;

import com.qrcode.qrcodes.QrCode;

/**
 * The Class QrCodes groups together the major methods for decoding 
 * and encoding the QR codes.
 * 
 * @see com.qrcode.QrDecoderManager QrDecoderManager
 * @see com.qrcode.QrEncoderManager QrEncoderManager
 * 
 * @version 1.0
 */
final public class QrCodes {

    /**
     * Decodes the QR code.
     *
     * @param The byte array that should contain the serialized QR code object.
     * @return The specific QR code object on success or null when no proprietary
     * decoder has been found.
     * 
     * @see com.qrcode.QrDecoderManager#decodeQrCode(byte[]) decodeQrCode(byte[])
     */
    public static QrCode decodeQrCode(byte[] data) {
        return QrDecoderManager.getDecoderManager().decodeQrCode(data);
    }
    
    /**
     * Encodes the QR code.
     *
     * @param qrCode The QR code.
     * @param encoder The encoder by which should be the QR code encoded.
     * @return The serialized QR code.
     * 
     * @see com.qrcode.QrEncoderManager#encodeQrCode(QrCode, Class) encodeQrCode(QrCode, Class)
     */
    public static byte[] encodeQrCode(QrCode qrCode, Class<?> encoder) {
        return QrEncoderManager.getEncoderManager().encodeQrCode(qrCode, encoder);
    }
}

//End of the file