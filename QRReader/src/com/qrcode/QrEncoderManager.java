package com.qrcode;

import com.qrcode.qrcodes.QrCode;

// TODO: Auto-generated Javadoc (X)
// TODO: Implement in the future
/**
 * The Class QrEncoderManager.
 */
final public class QrEncoderManager {
    
    /** The qr encoder manager. */
    private static QrEncoderManager qrEncoderManager;
    
    /**
     * Instantiates a new qr encoder manager.
     */
    private QrEncoderManager() {}
    
    /**
     * Gets the encoder manager.
     *
     * @return the encoder manager
     */
    public static QrEncoderManager getEncoderManager() {
        return qrEncoderManager = (qrEncoderManager == null)? new QrEncoderManager() : qrEncoderManager;
    }
    
    /**
     * Encode qr code.
     *
     * @param qrCode the qr code
     * @param encoder the encoder
     * @return the byte[]
     */
    public byte[] encodeQrCode(QrCode qrCode, Class<?> encoder) {
        return null;
    }
}
