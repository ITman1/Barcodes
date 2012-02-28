package com.qrcode.decoders;

import java.util.Arrays;
import java.util.List;

import com.qrcode.decoders.QrDecoder;
import com.qrcode.qrcodes.QrCode;

//http://code.google.com/p/zxing/wiki/BarcodeContents
public class ZxingQrDecoder extends QrDecoder {
    final private static List<String> SUPPORTED_SCHEMAS = 
            Arrays.asList("http", "mailto");
    final private static String DECODER_NAME = "ZXING";
    
    public ZxingQrDecoder() {
        super(SUPPORTED_SCHEMAS, DECODER_NAME);
    }

    public QrCode decode(byte[] data) {
        return null;
    }
    
}
