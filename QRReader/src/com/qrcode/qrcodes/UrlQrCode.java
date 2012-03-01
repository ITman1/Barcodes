package com.qrcode.qrcodes;

public class UrlQrCode extends QrCode {
    final public static String HTTP_SCHEME = "http://";
    
    private String address;

    public byte[] encode() {
        return null;
    }
    
    public String getType() {
        return null;
    }
    
    public void setAddress(String address) {
        this.address = address;
    }
    
    public String getAddress() {
        return address;
    }
    
    public String getUri() {
        return HTTP_SCHEME + address;
    }
}
