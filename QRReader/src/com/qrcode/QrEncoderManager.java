package com.qrcode;

import com.qrcode.qrcodes.QrCode;

final public class QrEncoderManager {
    private static QrEncoderManager qrEncoderManager;
    
    private QrEncoderManager() {}
    
    public static QrEncoderManager getEncoderManager() {
        return qrEncoderManager = (qrEncoderManager == null)? new QrEncoderManager() : qrEncoderManager;
    }
    
    public byte[] encodeQrCode(QrCode qrCode, Class<?> encoder) {
        return null;
    }
}
