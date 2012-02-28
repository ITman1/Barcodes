package com.qrcode.encoders;

public abstract class QrEncoder {
    public class EncodeException extends Exception {
        private static final long serialVersionUID = 1L;

        public EncodeException(String message) {
            super(message);
        }
    };
}
