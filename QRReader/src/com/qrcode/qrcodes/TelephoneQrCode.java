///////////////////////////////////////////////////////////////////////////////
// Project:    QR Reader for Android
// Package:    com.qrcode.qrcodes
// File:       TelephoneQrCode.java
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Contains class which implements the Telephone QR code.
///////////////////////////////////////////////////////////////////////////////

package com.qrcode.qrcodes;

/**
 * The Class TelephoneQrCode implements the Telephone QR code.
 */
public class TelephoneQrCode extends QrCode {
    
    /** The telephone number. */
    private String telephone;
    
    /**
     * Sets the telephone number.
     *
     * @param telephone The new telephone number.
     * @return True, if successful, otherwise false.
     */
    public boolean setTelephone(String telephone) {
        if (telephone.length() > 0) {
            char leadingChar = telephone.charAt(0);
            
            if (leadingChar == '+') {
                if (!telephone.substring(1).matches("\\d+")) return false;
            } else {
                if (!telephone.matches("\\d+")) return false;
            }
            
            this.telephone = telephone;
            return true;
        }
        return false;
    }
    
    /**
     * Gets the telephone number.
     *
     * @return The telephone number.
     */
    public String getTelephone() {
        return telephone;
    }

}

//End of the file