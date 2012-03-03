package com.qrcode.qrcodes;

public class SmsQrCode extends QrCode {
    private String receiver;
    private String body;
    private String subject;

    public boolean setReceiver(String receiver) {
        try {
            char leadingChar = receiver.charAt(0);
            
            if (leadingChar == '+') {
                Integer.parseInt(receiver.substring(1));
            } else {
                Integer.parseInt(receiver);
            }
            
            this.receiver = receiver;
            return true;
        } catch (IndexOutOfBoundsException e) {} 
          catch (NumberFormatException e) {}
        return false;
    }
    
    public String getReceiver() {
        return receiver;
    }
        
    @Override
    public String toString() {
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
