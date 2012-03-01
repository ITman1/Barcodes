package com.qrcode.decoders;

import java.util.Arrays;
import java.util.List;

import com.qrcode.QrDecoderManager;
import com.qrcode.decoders.QrDecoder;
import com.qrcode.qrcodes.MailQrCode;
import com.qrcode.qrcodes.QrCode;
import com.qrcode.qrcodes.TelephoneQrCode;
import com.qrcode.qrcodes.UrlQrCode;

//http://code.google.com/p/zxing/wiki/BarcodeContents
public class ZxingQrDecoder extends QrDecoder {
    final private static List<String> SUPPORTED_SCHEMAS = 
            Arrays.asList("http", "mailto", "tel");
    final private static String DECODER_NAME = "ZXING";
    
    final private static String HTTP_URI = "http";
    final private static String MAILTO_URI = "mailto";
    final private static String TELEPHONE_URI = "tel";

    public ZxingQrDecoder() {
        super(SUPPORTED_SCHEMAS, DECODER_NAME);
    }

    public QrCode decode(byte[] data) {
        String scheme = QrDecoderManager.getUriScheme(data);
        if (scheme.equalsIgnoreCase(HTTP_URI)) {
            return urlQrCode(data);
        } else if (scheme.equalsIgnoreCase(MAILTO_URI)) {
            return mailQrCode(data);
        } else if (scheme.equalsIgnoreCase(TELEPHONE_URI)) {
            return telephoneQrCode(data);
        }
        return null;
    }
    
    private UrlQrCode urlQrCode(byte[] data) {
        UrlQrCode urlQrCode = new UrlQrCode();
        urlQrCode.setAddress(new String(data).substring(HTTP_URI.length() + 1));
        return urlQrCode;
    }
    
    private MailQrCode mailQrCode(byte[] data) {
        MailQrCode mailQrCode = new MailQrCode();
        mailQrCode.setReceiver(new String(data).substring(MAILTO_URI.length() + 1));
        return mailQrCode;
    }
    
    private TelephoneQrCode telephoneQrCode(byte[] data) {
        TelephoneQrCode telephoneQrCode = new TelephoneQrCode();
        telephoneQrCode.setTelephone(new String(data).substring(TELEPHONE_URI.length() + 1));
        return telephoneQrCode;
    }
    
}
