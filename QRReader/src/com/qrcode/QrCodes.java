package com.qrcode;

import com.qrcode.qrcodes.*;

final public class QrCodes {

    public static QrCode decodeQrCode(byte[] data) {
        return QrDecoderManager.getDecoderManager().decodeQrCode(data);
    }
    
    public static byte[] encodeQrCode(QrCode qrCode, Class<?> encoder) {
        return QrEncoderManager.getEncoderManager().encodeQrCode(qrCode, encoder);
    }
    
}
