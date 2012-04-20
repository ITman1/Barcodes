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
import java.util.List;

import android.graphics.Rect;

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
     * Enumeration of all supported image formats to be passed into Image.
     *
     */
    final public class ImageFormats {
        final public static int JPEG = 0x01;
    };
    
    final public static class DecoderModes {
        public static final int NUMERIC_MODE          = 0x01;
        public static final int ALPHANUMERIC_MODE     = 0x02;
        public static final int STRUCTUREDAPPEND_MODE = 0x03;
        public static final int BYTE_MODE             = 0x04;
        public static final int FNC1_MODE             = 0x05;
        public static final int ECI_MODE              = 0x07;
        public static final int KANJI_MODE            = 0x08;
        public static final int FNC1_2_MODE           = 0x09;
    };
    
    final public static class DataSegmentsFlags {
        public static final int DATA_SEGMENTS_CORRUPTED = 0x01;
    };
   
    /**
     * Represents JNI Size class similar to C++ cv::Size.
     */
    public static class Size {
        public Size() {}
        public Size(int width, int height) {
            this.width = width;
            this.height = height;
        }
        public int width        = -1;
        public int height       = -1;
    }
    
    /**
     * Represents JNI Point class similar to C++ cv::Point.
     */
    public static class Point {
        public int x;
        public int y;
    }
    
    /**
     * Represents JNI DetectedMark class similar to C++ barcodes::DetectedMark.
     */
    public static class DetectedMark {
        public Point[] points;
        public double match;
        public int flags;
        
        /**
         * Transforms the points of the detected marks to the specified rectangle.
         * 
         * @param detectedMarks Detected marks of which points should be transformed.
         * @param srcSize Rectangle size in which are currently represented points.
         * @param destRect Rectangle into which should be transformed points.
         */
        public static void scaleOffsetMarks(DetectedMark[] detectedMarks, Size srcSize, Rect destRect) {
            if (detectedMarks != null) {
                for (DetectedMark detectedMark : detectedMarks) {
                    if (detectedMark.points != null) {
                        for (Point point : detectedMark.points) {
                            // Scale
                            point.x = (int)((double)point.x * (double)destRect.width() / (double)srcSize.width);
                            point.y = (int)((double)point.y * (double)destRect.height() / (double)srcSize.height);
                            // Offset
                            point.x += destRect.left;
                            point.y += destRect.top;
                        }
                    }
                }
            }
        }
    };
    
    /**
     * Represents JNI DataSegment class similar to C++ barcodes::DataSegment.
     */
    public static class DataSegment implements Serializable  {
        private static final long serialVersionUID = 1L;
        public byte[] data;
        public int mode;
        public int flags;
        public int remainderBits;
        
        /**
         * Converts the data (byte array) into Byte List. 
         * @return
         */
        public List<Byte> dataToByteList() {
            List<Byte> byteList = new ArrayList<Byte>();
            for (byte b : data) {
                byteList.add(b);
            }

            return byteList;
        }
    };
    
    /**
     * Represents JNI DataSegments class similar to C++ barcodes::DataSegments.
     */
    public static class DataSegments implements Serializable {
        private static final long serialVersionUID = 1L;
        public DataSegment[] segments;
        public int flags;
        
        /**
         * Converts all data segments into one byte array.
         * Information about the type of segments is lost.
         * 
         * @return Byte array where are concatenated all data segments.
         */
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
    
    /**
     * Represents JNI Image class similar to C++ barcodes::Image.
     */
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
    
    /**
     * Tries to locate the QR code in the image and then decode it.
     * 
     * @param image Image with the QR code.
     * @param request Additional request. (not used feature)
     * @param flags Additional flags. (not used feature)
     * @return Decoded data segments from the QR code.
     */
    public static native DataSegments readQrCode(Image image, int request, int flags);
    
    /**
     * Locates the QR code finder patterns in the image.
     * 
     * @param image Image where should be located the QR finder patterns.
     * @param request Additional request. (not used feature)
     * @param flags Additional flags. (not used feature)
     * @return Detected finder patterns.
     */
    public static native DetectedMark[] detectQrCode(Image image, int request, int flags);
    
    static {
        System.loadLibrary("JNI_QRBarcodesLibrary");
    }
}

//End of the file