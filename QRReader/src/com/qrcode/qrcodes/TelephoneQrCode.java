package com.qrcode.qrcodes;

public class TelephoneQrCode extends QrCode {
    final public static String TEL_SCHEME = "tel:";
    
    private String telephone;

    public byte[] encode() {
        return null;
    }
    
    public String getType() {
        return null;
    }
    
    public void setTelephone(String telephone) {
        this.telephone = telephone;
    }
    
    public String getTelephone() {
        return telephone;
    }
    
    public String getUri() {
        return TEL_SCHEME + telephone;
    }

}
