package com.qrcode;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import com.qrcode.decoders.QrDecoder.LexicalError;
import com.qrcode.decoders.QrDecoder.SemanticError;
import com.qrcode.decoders.QrDecoder.SyntaxError;
import com.qrcode.decoders.QrDecoder;
import com.qrcode.qrcodes.QrCode;

final public class QrDecoderManager {
    final private static List<String> DECODER_CLASSES = Arrays.asList(
            "com.qrcode.decoders.ComodoQrDecoder",
            "com.qrcode.decoders.ZxingQrDecoder"
    );
    final public static char URI_SCHEME_DELIMITER = ':';
    final public static char FIRST_PRINTABLE_CHAR = ' ';
    
    private static QrDecoderManager qrDecoderManager;
    
    private ArrayList<QrDecoder> decoders = new ArrayList<QrDecoder>();
    
    private QrDecoderManager () throws ClassNotFoundException, InstantiationException, IllegalAccessException {
        for (String decoderClassName : DECODER_CLASSES) {
            decoders.add((QrDecoder) Class.forName(decoderClassName).newInstance());
        }
    }
    
    public QrCode decodeQrCode(byte[] data) {
        return decodeQrCode(decoders, data);
    }
    
    public ArrayList<QrDecoder> getDecoders() {
        return decoders;
    }
    
    public static QrDecoderManager getDecoderManager() {
        try {
            return qrDecoderManager = (qrDecoderManager == null)? new QrDecoderManager() : qrDecoderManager;
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } catch (InstantiationException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        }
        
        return qrDecoderManager = null;
    }
    
    public static String getUriScheme(byte data[]) {
        if (data != null) {
            int i = 0;
            for (byte ch : data) {
                i++;
                if (ch == URI_SCHEME_DELIMITER && ch >= FIRST_PRINTABLE_CHAR) return data.toString().substring(i);
                    
            }
        }
        return null;
    }
    
    public static QrCode decodeQrCode(ArrayList<QrDecoder> decoders, byte[] data) {
        ArrayList<QrDecoder> passedDecoders = new ArrayList<QrDecoder>();
        String scheme;
        QrCode qrCode;
        
        if (data == null || (scheme = getUriScheme(data)) == null && decoders.isEmpty()) {
            return null;
        }
        
        for (QrDecoder decoder : decoders) {
            if (decoder.isSupported(scheme)) passedDecoders.add(decoder);
        }
        
        if (passedDecoders.isEmpty()) {
            return null;
        }
        
        for (QrDecoder decoder : passedDecoders) {
            //try {
                qrCode = decoder.decode(data);
            /*} catch (LexicalError e) {
                qrCode = null;
            } catch (SyntaxError e) {
                qrCode = null;
            } catch (SemanticError e) {
                qrCode = null;
            }*/
            
            if (qrCode != null) return qrCode;
        }
        
        return null;
    }
}
