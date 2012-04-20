///////////////////////////////////////////////////////////////////////////////
// Project:    QR Reader for Android
// Package:    com.qrcode.qrcodes
// File:       MailQrCode.java
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Contains class which implements the Mail QR code.
///////////////////////////////////////////////////////////////////////////////

package com.qrcode.qrcodes;

/**
 * The Class MailQrCode implements the Mail QR code.
 */
public class MailQrCode extends QrCode {
        
    /** The regular expression which matches almost all most common e-mail addresses. */
    final private static String MAILTO_RECEIVER_REGEX = "^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z][A-Za-z]+$";
    
    /** The mail receiver. */
    private String receiver;
    
    /** The mail subject. */
    private String subject;
    
    /** The mail body. */
    private String body;

    /**
     * Sets the mail receiver.
     *
     * @param receiver The receiver of this mail.
     * @return True, if successful, otherwise false.
     */
    public boolean setReceiver(String rec) {
        String receiver = rec.trim();
        if (receiver.matches(MAILTO_RECEIVER_REGEX)) {
            this.receiver = receiver;
            return true;
        }
        return false;
    }
    
    /**
     * Gets the mail receiver.
     *
     * @return The mail receiver.
     */
    public String getReceiver() {
        return receiver;
    }
    
    /**
     * Sets the mail subject.
     *
     * @param subject The new subject of this mail.
     */
    public void setSubject(String subject) {
        this.subject = subject;
    }
    
    /**
     * Gets the mail subject.
     *
     * @return The mail subject.
     */
    public String getSubject() {
        return subject;
    }
    
    /**
     * Sets the mail body.
     *
     * @param body The new mail body.
     */
    public void setBody(String body) {
        this.body = body;
    }
    
    /**
     * Gets the mail body.
     *
     * @return The body of this mail.
     */
    public String getBody() {
        return body;
    }
    
}

//End of the file