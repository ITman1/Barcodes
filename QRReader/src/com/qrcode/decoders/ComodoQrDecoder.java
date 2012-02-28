package com.qrcode.decoders;

import java.util.Arrays;
import java.util.List;

import com.qrcode.decoders.QrDecoder;
import com.qrcode.qrcodes.QrCode;

//http://www.nttdocomo.co.jp/english/service/developer/make/content/barcode/function/application/common/index.html
final public class ComodoQrDecoder extends QrDecoder {
    final private static List<String> SUPPORTED_SCHEMAS = 
            Arrays.asList("MECARD", "MATMSG", "MEBKM");
    final private static String DECODER_NAME = "NTT DOCOMO";
    
    private enum Token {
        TOKEN_ID_MECARD,
        TOKEN_ID_MATMSG,
        TOKEN_ID_MEBKM,
        TOKEN_ID_LAPL,
        
        TOKEN_TYPE_N,
        TOKEN_TYPE_SOUND,
        TOKEN_TYPE_TEL,
        TOKEN_TYPE_TEL_AV,
        TOKEN_TYPE_EMAIL,
        TOKEN_TYPE_NOTE,
        TOKEN_TYPE_BDAY,
        TOKEN_TYPE_ADR,
        TOKEN_TYPE_URL,
        TOKEN_TYPE_NICKNAME,
        TOKEN_TYPE_TO,
        TOKEN_TYPE_SUB,
        TOKEN_TYPE_BODY,
        TOKEN_TYPE_TITLE,
        TOKEN_TYPE_CMD,
        TOKEN_TYPE_ADFURL,
        TOKEN_TYPE_PARAM,
        
        TOKEN_END
    };
    
    public ComodoQrDecoder() {
        super(SUPPORTED_SCHEMAS, DECODER_NAME);
    }

    public QrCode decode(byte[] data) {
        return null;
    }
    
    private Token getNextToken(String data) {
        return Token.TOKEN_END;
    }    
}
