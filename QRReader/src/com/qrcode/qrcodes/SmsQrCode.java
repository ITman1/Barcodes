///////////////////////////////////////////////////////////////////////////////
// Project:    QR Reader for Android
// Package:    com.qrcode.qrcodes
// File:       SmsQrCode.java
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Contains class which implements the SMS QR code.
///////////////////////////////////////////////////////////////////////////////

package com.qrcode.qrcodes;

/**
 * The Class SmsQrCode implements the SMS QR code.
 */
public class SmsQrCode extends QrCode {
    
    /** The SMS receiver number. */
    private String receiver;
    
    /** The SMS body. */
    private String body;
    
    /**
     * Sets the receiver's number.
     *
     * @param receiver The number of the receiver.
     * @return True, if successful, otherwise false.
     */
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
    
    /**
     * Gets the receiver's number.
     *
     * @return The receiver's number.
     */
    public String getReceiver() {
        return receiver;
    }
    
    /**
     * Sets the SMS body.
     *
     * @param body The new body of this SMS.
     */
    public void setBody(String body) {
        this.body = body;
    }
    
    /**
     * Gets the SMS body.
     *
     * @return The body of this SMS.
     */
    public String getBody() {
        return body;
    }
    
}

//End of the file