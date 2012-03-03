package com.qrcode.qrcodes;

public class TelephoneQrCode extends QrCode {
    
    private String telephone;
    
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
    
    public String getTelephone() {
        return telephone;
    }
        
    @Override
    public String toString() {
        return telephone;
    }

}
