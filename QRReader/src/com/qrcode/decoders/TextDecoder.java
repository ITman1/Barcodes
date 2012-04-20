///////////////////////////////////////////////////////////////////////////////
// Project:    QR Reader for Android
// Package:    com.qrcode.decoders
// File:       TextDecoder.java
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Contains the QR code decoder which retrieves text QR code.
///////////////////////////////////////////////////////////////////////////////

package com.qrcode.decoders;

import com.qrcode.QrCodes.DataSegment;
import com.qrcode.QrCodes.DataSegments;
import com.qrcode.QrCodes.DecoderModes;
import com.qrcode.qrcodes.QrCode;
import com.qrcode.qrcodes.TextQrCode;

/**
 * The Class BaseQrDecoder implements the QR code decoder which decodes the
 * most common QR codes stored in the URI format.
 */
public class TextDecoder extends QrDecoder {

    /** The name of this decoder. */
    final private static String DECODER_NAME = "Text decoder";
    
    public TextDecoder() {
        super(null, DECODER_NAME);
    }

    @Override
    public QrCode decode(DataSegments dataSegments) {
        boolean ok = true;
        for (DataSegment segment: dataSegments.segments) {
            if (segment.mode != DecoderModes.BYTE_MODE && segment.mode != DecoderModes.ALPHANUMERIC_MODE 
                    && segment.mode != DecoderModes.NUMERIC_MODE) {
                ok = false;
            }
        }
        
        if (ok) {
            TextQrCode textQrCode = new TextQrCode();
            textQrCode.setText(new String(dataSegments.toByteArray()));
            return textQrCode;
        }
        
        return null;
    }

}
