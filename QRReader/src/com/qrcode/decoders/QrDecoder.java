package com.qrcode.decoders;

import java.util.Iterator;
import java.util.List;

import com.qrcode.qrcodes.QrCode;

public abstract class QrDecoder {
       
    public class LexicalError extends Exception {
        private static final long serialVersionUID = 1L;

        public LexicalError(String message) {
            super(message);
        }
    };
    
    public class SyntaxError extends Exception {
        private static final long serialVersionUID = 1L;

        public SyntaxError(String message) {
            super(message);
        }
    };
    
    public class SemanticError extends Exception {
        private static final long serialVersionUID = 1L;

        public SemanticError(String message) {
            super(message);
        }
    };
    
    final protected String decoderName; 
    final protected List<String> supportedSchemas;
    
    protected static QrDecoder decoderInstance;
    
    protected QrDecoder(List<String> supportedSchemas, String decoderName) {
        this.supportedSchemas = supportedSchemas;
        this.decoderName = decoderName;       
    }
          
    public List<String> getSupportedSchemas() {
        return supportedSchemas;
    }
    
    public boolean isSupported(String scheme) {
        Iterator <String> it = supportedSchemas.iterator();

        while(it.hasNext()) {
            if(it.next().equalsIgnoreCase(scheme)) return true;
        }
        
        return false;
    }
    
    public String getDecoderName() {
        return decoderName;
    }
        
    public abstract QrCode decode(byte[] data);
}
