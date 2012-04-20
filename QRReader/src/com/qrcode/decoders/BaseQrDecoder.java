///////////////////////////////////////////////////////////////////////////////
// Project:    QR Reader for Android
// Package:    com.qrcode.decoders
// File:       BaseQrDecoder.java
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Contains the QR code decoder with the most common URI schemas.
///////////////////////////////////////////////////////////////////////////////

package com.qrcode.decoders;

import java.net.MalformedURLException;
import java.net.URI;
import java.net.URISyntaxException;
import java.net.URL;
import java.util.Arrays;
import java.util.List;

import com.qrcode.QrDecoderManager;
import com.qrcode.QrCodes.DataSegments;
import com.qrcode.decoders.QrDecoder;
import com.qrcode.qrcodes.MailQrCode;
import com.qrcode.qrcodes.QrCode;
import com.qrcode.qrcodes.SmsQrCode;
import com.qrcode.qrcodes.TelephoneQrCode;
import com.qrcode.qrcodes.HttpLinkQrCode;;

/**
 * The Class BaseQrDecoder implements the QR code decoder which decodes the
 * most common QR codes stored in the URI format.
 */
// http://code.google.com/p/zxing/wiki/BarcodeContents
// http://en.wikipedia.org/wiki/URI_scheme
// http://sixrevisions.com/web-development/qr-codes-uri-schemes/
final public class BaseQrDecoder extends QrDecoder {
    
    /** The HTTP: schema for the {@link com.qrcode.qrcodes.HttpLinkQrCode HttpLinkQrCode}. */
    final private static String HTTP_SCHEMA         = "http";
    
    /** The MAILTO: schema for the {@link com.qrcode.qrcodes.MailQrCode MailQrCode}. */
    final private static String MAILTO_SCHEMA       = "mailto";
    
    /** The TEL: schema for the {@link com.qrcode.qrcodes.TelephoneQrCode TelephoneQrCode}. */
    final private static String TELEPHONE_SCHEMA    = "tel";
    
    /** The URLTO: schema for the {@link com.qrcode.qrcodes.HttpLinkQrCode HttpLinkQrCode}. */
    final private static String URLTO_SCHEMA        = "URLTO";
    
    /** The SMS: schema for the {@link com.qrcode.qrcodes.SmsQrCode SmsQrCode}. */
    final private static String SMS_SCHEMA          = "sms";
    
    /** The SMSTO: schema for the {@link com.qrcode.qrcodes.SmsQrCode SmsQrCode}. */
    final private static String SMSTO_SCHEMA        = "SMSTO";
    
    /** The list contains all supported schemas. */
    final private static List<String> SUPPORTED_SCHEMAS = Arrays.asList(
            HTTP_SCHEMA,
            MAILTO_SCHEMA,
            TELEPHONE_SCHEMA,
            URLTO_SCHEMA,
            SMS_SCHEMA,
            SMSTO_SCHEMA
    );
    
    /** The name of this decoder. */
    final private static String DECODER_NAME = "Base decoder";
    
    /** The regular expression which matches the SUBJECT query. */
    final private static String MAILTO_QUERY_SUBJECT_REGEX = "[&?]Subject=(([^&]*))";
    
    /** The regular expression which matches the BODY query. */
    final private static String MAILTO_QUERY_BODY_REGEX = "[&?]Body=(([^&]*))";
    
    /** The regular expression which matches the whole SMS/SMSTO encoded QR code. */
    final private static String SMS_URI_NUMBERS_REXEG = "^(([+]?[0-9]+),)*([+]?[0-9]+)([?]Body=(.*))?$";
    
    /** The regular expression which matches the whole SMS encoded QR code in the format "sms:number:text". */
    final private static String SMS_URI_BROKEN_REGEX = "^([+]?[0-9]+):(.*)$";;
    
    /**
     * Instantiates this decoder.
     */
    public BaseQrDecoder() {
        super(SUPPORTED_SCHEMAS, DECODER_NAME);
    }

    /**
     * Decodes the data to the one of these specific QR codes:
     * {@link com.qrcode.qrcodes.HttpLinkQrCode HttpLinkQrCode},
     * {@link com.qrcode.qrcodes.MailQrCode MailQrCode},
     * {@link com.qrcode.qrcodes.TelephoneQrCode TelephoneQrCode},
     * {@link com.qrcode.qrcodes.SmsQrCode SmsQrCode}.
     * 
     * @see com.qrcode.decoders.QrDecoder#decode(DataSegments) decode(DataSegments)
     */
    @Override
    public QrCode decode(DataSegments dataSegments) {
        byte[] data = dataSegments.toByteArray();
        String schema = QrDecoderManager.getUriScheme(data);

        if (schema.equalsIgnoreCase(HTTP_SCHEMA)) {             // HTTP:
            return urlQrCode(data);  
        } else if (schema.equalsIgnoreCase(URLTO_SCHEMA)) {     // URLTO:
            return urltoQrCode(data);
        } else if (schema.equalsIgnoreCase(MAILTO_SCHEMA)) {    // MAILTO:
            return mailQrCode(data);
        } else if (schema.equalsIgnoreCase(TELEPHONE_SCHEMA)) { // TEL:
            return telephoneQrCode(data);
        } else if (schema.equalsIgnoreCase(SMS_SCHEMA)) {       // SMS:
            return smsQrCode(data);
        } else if (schema.equalsIgnoreCase(SMSTO_SCHEMA)) {     // SMSTO:
            return smstoQrCode(data);
        }
        return null;
    }
    
    /**
     * Decodes the HTTP link QR code. (HTTP: schema)
     *
     * @param data The serialized QR code.
     * @return The HTTP link QR code.
     */
    private HttpLinkQrCode urlQrCode(byte[] data) {
        HttpLinkQrCode urlQrCode = new HttpLinkQrCode();
        String urlString = new String(data);
        
        try {
            return (urlQrCode.setLink(new URL(urlString)))? urlQrCode : null;
        } catch (MalformedURLException e) {
            return null;
        }
    }
    
    /**
     * Decodes the HTTP link QR code. (URLTO: schema)
     *
     * @param data The serialized QR code.
     * @return The HTTP link QR code.
     */
    private HttpLinkQrCode urltoQrCode(byte[] data) {
        HttpLinkQrCode urlQrCode = new HttpLinkQrCode();
        String urlString = new String(data).substring(URLTO_SCHEMA.length() + 1);
        
        try {
            return (urlQrCode.setLink(URI.create(urlString).toURL()))? urlQrCode : null;
        } catch (MalformedURLException e) {
        } catch (IllegalArgumentException e) {}
        
        return null;
    }
    
    /**
     * Decodes the SMS link QR code. (SMS: schema)
     *
     * @param data The serialized QR code.
     * @return The SMS link QR code.
     */
    private SmsQrCode smsQrCode(byte[] data) {
        SmsQrCode smsQrCode = new SmsQrCode();
        String strData = new String(data).substring(SMS_SCHEMA.length() + 1);
        
        if (QrDecoderManager.regexMatches(SMS_URI_NUMBERS_REXEG, strData)) {
            smsQrCode.setBody(QrDecoderManager.decodeUri(QrDecoderManager.regexMatch(SMS_URI_NUMBERS_REXEG, strData, 5)));
            return (smsQrCode.setReceiver(QrDecoderManager.regexMatch(SMS_URI_NUMBERS_REXEG, strData, 3)))? smsQrCode : null;
        } else if (QrDecoderManager.regexMatches(SMS_URI_BROKEN_REGEX, strData)) {
            smsQrCode.setBody(QrDecoderManager.decodeUri(QrDecoderManager.regexMatch(SMS_URI_BROKEN_REGEX, strData, 2)));
            return (smsQrCode.setReceiver(QrDecoderManager.regexMatch(SMS_URI_BROKEN_REGEX, strData, 1)))? smsQrCode : null;
        }
        
        return null;
    }
    
    /**
     * Decodes the SMS link QR code. (SMSTO: schema)
     *
     * @param data The serialized QR code.
     * @return The SMS link QR code.
     */
    private SmsQrCode smstoQrCode(byte[] data) {
        SmsQrCode smsQrCode = new SmsQrCode();
        String strData = new String(data).substring(SMSTO_SCHEMA.length() + 1);
        
        if (QrDecoderManager.regexMatches(SMS_URI_NUMBERS_REXEG, strData)) {
            smsQrCode.setBody(QrDecoderManager.decodeUri(QrDecoderManager.regexMatch(SMS_URI_NUMBERS_REXEG, strData, 5)));
            return (smsQrCode.setReceiver(QrDecoderManager.regexMatch(SMS_URI_NUMBERS_REXEG, strData, 3)))? smsQrCode : null;
        } else if (QrDecoderManager.regexMatches(SMS_URI_BROKEN_REGEX, strData)) {
            smsQrCode.setBody(QrDecoderManager.decodeUri(QrDecoderManager.regexMatch(SMS_URI_BROKEN_REGEX, strData, 2)));
            return (smsQrCode.setReceiver(QrDecoderManager.regexMatch(SMS_URI_BROKEN_REGEX, strData, 1)))? smsQrCode : null;
        }
        
        return null;
    }
        
    /**
     * Decodes the Mail link QR code. (MAILTO: schema)
     *
     * @param data The serialized QR code.
     * @return The Mail link QR code.
     */
    private MailQrCode mailQrCode(byte[] data) {
        String mailtoStr = new String(data);

        try {
            new URI(mailtoStr);
        } catch (URISyntaxException e) {
            return null;
        }
        
        MailQrCode mailQrCode = new MailQrCode();
        int queryIndex = mailtoStr.indexOf('?');
        String query = (queryIndex != -1)? mailtoStr.substring(queryIndex) : null;

        mailQrCode.setBody(QrDecoderManager.decodeUri(QrDecoderManager.regexMatch(MAILTO_QUERY_BODY_REGEX, query, 1)));
        mailQrCode.setSubject(QrDecoderManager.decodeUri(QrDecoderManager.regexMatch(MAILTO_QUERY_SUBJECT_REGEX, query, 1)));

        // Getting receiver e-mail address
        String receiver = mailtoStr.substring(
                // Start behind the "mailto:"
                MAILTO_SCHEMA.length() + 1,                           
                // If the query is present, stop here, otherwise at the end of the string
                (queryIndex != -1)? queryIndex : mailtoStr.length()   
        );
        return (mailQrCode.setReceiver(receiver))? mailQrCode : null;
    }
    
    /**
     * Decodes the Telephone link QR code. (TEL: schema)
     *
     * @param data The serialized QR code.
     * @return The Telephone link QR code.
     */
    private TelephoneQrCode telephoneQrCode(byte[] data) {
        TelephoneQrCode telephoneQrCode = new TelephoneQrCode();
        String telephone = new String(data).substring(TELEPHONE_SCHEMA.length() + 1);
        return (telephoneQrCode.setTelephone(telephone))? telephoneQrCode : null;
    }
    

    
}

//End of the file
