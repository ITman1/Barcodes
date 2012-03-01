package com.qrcode.decoders;

import java.util.Arrays;
import java.util.List;

import com.qrcode.QrDecoderManager;
import com.qrcode.decoders.QrDecoder;
import com.qrcode.qrcodes.QrCode;
import com.qrcode.qrcodes.UrlQrCode;

//http://code.google.com/p/zxing/wiki/BarcodeContents
public class ZxingQrDecoder extends QrDecoder {
    final private static List<String> SUPPORTED_SCHEMAS = 
            Arrays.asList("http", "mailto");
    final private static String DECODER_NAME = "ZXING";
    
    final private static String HTTP_URI = "http";
    final private static int HTTP_URI_OFFSET = 5;

    public ZxingQrDecoder() {
        super(SUPPORTED_SCHEMAS, DECODER_NAME);
    }

    public QrCode decode(byte[] data) {
        String scheme = QrDecoderManager.getUriScheme(data);
        if (scheme.equalsIgnoreCase(HTTP_URI)) {
            return urlQrCode(data);
        }
        return null;
    }
    
    private QrCode urlQrCode(byte[] data) {
        UrlQrCode urlQrCode = new UrlQrCode();
        urlQrCode.setAddress(new String(data).substring(HTTP_URI_OFFSET));
        return urlQrCode;
    }
    
}
