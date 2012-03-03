package com.qrcode.qrcodes;

import java.net.MalformedURLException;
import java.net.URI;
import java.net.URISyntaxException;

public class MailQrCode extends QrCode {
    final private static String MAILTO_SCHEMA = "mailto";
    final private static String MAILTO_RECEIVER_REGEX = "^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z][A-Za-z]+$";
    
    private String receiver;
    private String subject;
    private String body;

    public boolean setReceiver(String receiver) {
        if (receiver.matches(MAILTO_RECEIVER_REGEX)) {
            this.receiver = receiver;
            return true;
        }
        return false;
    }
    
    public String getReceiver() {
        return receiver;
    }
    
    public void setSubject(String subject) {
        this.subject = subject;
    }
    
    public String getSubject() {
        return subject;
    }
    
    public void setBody(String body) {
        this.body = body;
    }
    
    public String getBody() {
        return body;
    }
    
}
