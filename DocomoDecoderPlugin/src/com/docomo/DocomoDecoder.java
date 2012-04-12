///////////////////////////////////////////////////////////////////////////////
// Project:    Docomo Decoder Plugin
// Package:    com.docomo
// File:       DocomoDecoder.java
// Date:       May 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Implements Docomo Decoder.
///////////////////////////////////////////////////////////////////////////////

package com.docomo;

import java.util.Arrays;
import java.util.List;

import com.qrcode.QrDecoderManager;
import com.qrcode.QrCodes.DataSegments;
import com.qrcode.decoders.QrDecoder;
import com.qrcode.qrcodes.MailQrCode;
import com.qrcode.qrcodes.QrCode;

/**
 * Implements Decoder developed with NTT DOCOMO, INC.
 * 
 * @see http://www.nttdocomo.co.jp/english/service/developer/make/content/barcode/function/application/ 
 * @author Radim Loskot
 *
 */
public class DocomoDecoder extends QrDecoder {
    
    /** The name of this decoder. */
    final private static String DECODER_NAME = "Docomo decoder";
    
    /** The MATMSG: schema for the {@link com.qrcode.qrcodes.MailQrCode MailQrCode}. */
    final private static String MATMSG_SCHEMA       = "MATMSG";
    
    /** The list contains all supported schemas. */
    final private static List<String> SUPPORTED_SCHEMAS = Arrays.asList(
            MATMSG_SCHEMA
    );
    
    /** The regular expression which matches the SUBJECT query. */
    final private static String MAILTO_TO_REGEX = "[:;]TO:(([^;]*))";
    
    /** The regular expression which matches the SUBJECT part. */
    final private static String MAILTO_SUBJECT_REGEX = "[:;]SUB:(([^;]*))";
    
    /** The regular expression which matches the BODY part. */
    final private static String MAILTO_BODY_REGEX = "[:;]BODY:(([^;]*))";
    
    /**
     * Instantiates this decoder.
     */
    public DocomoDecoder() {
        super(SUPPORTED_SCHEMAS, DECODER_NAME);
    }
    
    /**
     * Decodes the data to the one of these specific QR codes:
     * {@link com.qrcode.qrcodes.MailQrCode MailQrCode},
     * 
     * @see com.qrcode.decoders.QrDecoder#decode(DataSegments) decode(DataSegments)
     */
    @Override
    public QrCode decode(DataSegments dataSegments) {
        byte[] data = dataSegments.toByteArray();
        String schema = QrDecoderManager.getUriScheme(data);

        if (schema.equalsIgnoreCase(MATMSG_SCHEMA)) {             // MATMSG:
            return mailQrCode(data);  
        }
        
        return null;
    }
    
    /**
     * Decodes the Mail link QR code. (MATMSG: schema)
     *
     * @param data The serialized QR code.
     * @return The Mail link QR code.
     */
    private MailQrCode mailQrCode(byte[] data) {
        String mailtoStr = new String(data);
        
        if (mailtoStr.length() == 0) return null;
        
        MailQrCode mailQrCode = new MailQrCode();

        mailQrCode.setBody(QrDecoderManager.decodeUri(QrDecoderManager.regexMatch(MAILTO_BODY_REGEX, mailtoStr, 1)));
        mailQrCode.setSubject(QrDecoderManager.decodeUri(QrDecoderManager.regexMatch(MAILTO_SUBJECT_REGEX, mailtoStr, 1)));

        return (mailQrCode.setReceiver(
                    QrDecoderManager.decodeUri(QrDecoderManager.regexMatch(MAILTO_TO_REGEX, mailtoStr, 1)))
                )? mailQrCode : null;
    }
}
