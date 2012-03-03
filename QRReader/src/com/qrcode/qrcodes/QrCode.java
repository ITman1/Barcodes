package com.qrcode.qrcodes;

import com.qrcode.QrCodes;

public abstract class QrCode {
    public byte[] encode(Class<?> encoder) {
        return QrCodes.encodeQrCode(this, encoder);
    }
}
