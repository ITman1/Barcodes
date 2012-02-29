package com.qrcode.qrcodes;

public abstract class QrCode {
    public abstract String getType();
    public abstract byte[] encode();
    
}
