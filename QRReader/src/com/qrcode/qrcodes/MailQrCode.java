package com.qrcode.qrcodes;

public class MailQrCode extends QrCode {
    final public static String MAILTO_SCHEME = "mailto:";
    
    private String receiver;
    private String subject;
    private String body;

    public byte[] encode() {
        return null;
    }
    
    public String getType() {
        return null;
    }
    
    public void setReceiver(String receiver) {
        this.receiver = receiver;
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
    
    public String getUri() {
        return MAILTO_SCHEME + receiver;
    }
}
