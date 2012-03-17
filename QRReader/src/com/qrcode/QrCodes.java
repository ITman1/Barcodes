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

// TODO - comment this file
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
        
    final public class ImageFormats {
        final public static int JPEG = 0x01;
    };
    
    final public class Requests {
        final public static int GET_QR_CODE = 0x01;
    };
    
    final public class Flags {
        final public static int ALL_FEATURES = 0x01;
    };
    
    public static class Size {
        public Size() {}
        public Size(int width, int height) {
            this.width = width;
            this.height = height;
        }
        public int width        = -1;
        public int height       = -1;
    }
    
    public static class Point {
        public int x;
        public int y;
    }
    
    public static class DetectedMark {
        public Point[] points;
        public double match;
        public int flags;
    };
    
    public static class Image {
        public boolean compressed;
        public Size size;
        public byte[] data;
        public int colorFormat;
    };

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
    
    public static native byte[] readQrCode(Image image, int request, int flags);
    public static native DetectedMark[] detectQrCode(Image image, int request, int flags);
    
    static {
        System.loadLibrary("JNI_QRBarcodesLibrary");
    }
}

//End of the file