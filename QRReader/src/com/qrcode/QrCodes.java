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

import java.io.Serializable;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

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
    
    public static class DataSegment implements Serializable  {
        private static final long serialVersionUID = 1L;
        public byte[] data;
        public int mode;
        public int flags;
        public int remainderBits;
        
        public List<Byte> dataToByteList() {
            List<Byte> byteList = new ArrayList<Byte>();
            for (byte b : data) {
                byteList.add(b);
            }

            return byteList;
        }
    };
    
    public static class DataSegments implements Serializable {
        private static final long serialVersionUID = 1L;
        public DataSegment[] segments;
        public int flags;
        
        public byte[] toByteArray() {
            List<Byte> byteList = new ArrayList<Byte>();
            
            for (DataSegment segment : segments) {
                byteList.addAll(segment.dataToByteList());
            }
            
            byte[] bytes = new byte[byteList.size()];
            int i = 0;
            for (Byte b : byteList) {
                bytes[i] = b;
                i++;
            }
            
            return bytes;

        }
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
     * @see com.qrcode.QrDecoderManager#decodeQrCode(DataSegments) decodeQrCode(DataSegments)
     */
    public static QrCode decodeQrCode(DataSegments dataSegments) {
        return QrDecoderManager.getDecoderManager().decodeQrCode(dataSegments);
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
    
    public static native DataSegments readQrCode(Image image, int request, int flags);
    public static native DetectedMark[] detectQrCode(Image image, int request, int flags);
    
    static {
        System.loadLibrary("JNI_QRBarcodesLibrary");
    }
}

//End of the file