package com.qrcode.decoders;

import java.net.MalformedURLException;
import java.net.URI;
import java.net.URISyntaxException;
import java.net.URL;
import java.util.Arrays;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import com.qrcode.QrDecoderManager;
import com.qrcode.decoders.QrDecoder;
import com.qrcode.qrcodes.MailQrCode;
import com.qrcode.qrcodes.QrCode;
import com.qrcode.qrcodes.SmsQrCode;
import com.qrcode.qrcodes.TelephoneQrCode;
import com.qrcode.qrcodes.HttpLinkQrCode;;

//http://code.google.com/p/zxing/wiki/BarcodeContents
public class BaseQrDecoder extends QrDecoder {
    final private static String HTTP_SCHEMA         = "http";
    final private static String MAILTO_SCHEMA       = "mailto";
    final private static String TELEPHONE_SCHEMA    = "tel";
    final private static String URLTO_SCHEMA        = "URLTO";
    final private static String SMS_SCHEMA          = "sms";
    final private static String SMSTO_SCHEMA        = "SMSTO";
    
    final private static List<String> SUPPORTED_SCHEMAS = Arrays.asList(
            HTTP_SCHEMA,
            MAILTO_SCHEMA,
            TELEPHONE_SCHEMA,
            URLTO_SCHEMA,
            SMS_SCHEMA,
            SMSTO_SCHEMA
    );
    final private static String DECODER_NAME = "Base decoder";
    
    final private static String MAILTO_QUERY_SUBJECT_REGEX = "[&?]Subject=(([^&]*))";
    final private static String MAILTO_QUERY_BODY_REGEX = "[&?]Body=(([^&]*))";
    final private static String SMS_URI_NUMBERS_REXEG = "^(([+]?[0-9]+),)*([+]?[0-9]+)([?]Body=(.*))?$";
    final private static String SMS_URI_BROKEN_REGEX = "^([+]?[0-9]+):(.*)$";;
    
    public BaseQrDecoder() {
        super(SUPPORTED_SCHEMAS, DECODER_NAME);
    }

    public QrCode decode(byte[] data) {
        String schema = QrDecoderManager.getUriScheme(data);
        if (schema.equalsIgnoreCase(HTTP_SCHEMA)) {
            return urlQrCode(data);  
        } else if (schema.equalsIgnoreCase(URLTO_SCHEMA)) {
            return urltoQrCode(data);
        } else if (schema.equalsIgnoreCase(MAILTO_SCHEMA)) {
            return mailQrCode(data);
        } else if (schema.equalsIgnoreCase(TELEPHONE_SCHEMA)) {
            return telephoneQrCode(data);
        } else if (schema.equalsIgnoreCase(SMS_SCHEMA)) {
            return smsQrCode(data);
        } else if (schema.equalsIgnoreCase(SMSTO_SCHEMA)) {
            return smstoQrCode(data);
        }
        return null;
    }
    
    private HttpLinkQrCode urlQrCode(byte[] data) {
        HttpLinkQrCode urlQrCode = new HttpLinkQrCode();
        String urlString = new String(data);
        
        try {
            return (urlQrCode.setLink(new URL(urlString)))? urlQrCode : null;
        } catch (MalformedURLException e) {
            return null;
        }
    }
    
    private HttpLinkQrCode urltoQrCode(byte[] data) {
        HttpLinkQrCode urlQrCode = new HttpLinkQrCode();
        String urlString = new String(data).substring(URLTO_SCHEMA.length() + 1);
        
        try {
            return (urlQrCode.setLink(URI.create(urlString).toURL()))? urlQrCode : null;
        } catch (MalformedURLException e) {
            return null;
        }
    }
    
    private SmsQrCode smsQrCode(byte[] data) {
        SmsQrCode smsQrCode = new SmsQrCode();
        String strData = new String(data).substring(SMS_SCHEMA.length() + 1);
        
        if (regexMatches(SMS_URI_NUMBERS_REXEG, strData)) {
            smsQrCode.setBody(QrDecoderManager.decodeUri(regexMatch(SMS_URI_NUMBERS_REXEG, strData, 5)));
            return (smsQrCode.setReceiver(regexMatch(SMS_URI_NUMBERS_REXEG, strData, 3)))? smsQrCode : null;
        } else if (regexMatches(SMS_URI_BROKEN_REGEX, strData)) {
            smsQrCode.setBody(QrDecoderManager.decodeUri(regexMatch(SMS_URI_BROKEN_REGEX, strData, 2)));
            return (smsQrCode.setReceiver(regexMatch(SMS_URI_BROKEN_REGEX, strData, 1)))? smsQrCode : null;
        }
        
        return null;
    }
    
    private SmsQrCode smstoQrCode(byte[] data) {
        SmsQrCode urlQrCode = new SmsQrCode();
        return (urlQrCode.setReceiver(new String(data).substring(SMSTO_SCHEMA.length() + 1)))? urlQrCode : null;
    }
    
    private String regexMatch(String pattern, String against, int group) {
        if (pattern != null && against != null) {
            Pattern bodyPattern = Pattern.compile(pattern, Pattern.CASE_INSENSITIVE);
            Matcher matcher = bodyPattern.matcher(against);
            if (group < matcher.groupCount() + 1 && matcher.find()) {
                return matcher.group(group);
            }
        }
            
        return null;
    }
    
    private boolean regexMatches(String pattern, String against) {
        if (pattern != null && against != null) {
            Pattern bodyPattern = Pattern.compile(pattern, Pattern.CASE_INSENSITIVE);
            Matcher matcher = bodyPattern.matcher(against);
            return matcher.matches();
        }
            
        return false;
    }
    
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

        mailQrCode.setBody(QrDecoderManager.decodeUri(regexMatch(MAILTO_QUERY_BODY_REGEX, query, 1)));
        mailQrCode.setSubject(QrDecoderManager.decodeUri(regexMatch(MAILTO_QUERY_SUBJECT_REGEX, query, 1)));

        // Getting receiver e-mail address
        String receiver = mailtoStr.substring(
                              // Start behind the "mailto:"
                              MAILTO_SCHEMA.length() + 1,                           
                              // If the query is present, stop here, otherwise at the end of the string
                              (queryIndex != -1)? queryIndex : mailtoStr.length()   
                          );
        return (mailQrCode.setReceiver(receiver))? mailQrCode : null;
    }
    
    private TelephoneQrCode telephoneQrCode(byte[] data) {
        TelephoneQrCode telephoneQrCode = new TelephoneQrCode();
        String telephone = new String(data).substring(TELEPHONE_SCHEMA.length() + 1);
        return (telephoneQrCode.setTelephone(telephone))? telephoneQrCode : null;
    }
    
}
