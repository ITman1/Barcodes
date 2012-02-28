package com.qrcode.qrcodes;

import com.qrcode.QrCodes;

public abstract class QrCode {
    public abstract String getType();
    public abstract byte[] encode();
    
}
