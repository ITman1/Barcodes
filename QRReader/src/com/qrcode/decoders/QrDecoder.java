///////////////////////////////////////////////////////////////////////////////
// Project:    QR Reader for Android
// Package:    com.qrcode.decoders
// File:       QrDecoder.java
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Contains the abstract class of the QR decoder.
///////////////////////////////////////////////////////////////////////////////

package com.qrcode.decoders;

import java.util.Iterator;
import java.util.List;

import com.qrcode.QrCodes.DataSegments;
import com.qrcode.qrcodes.QrCode;

/**
 * The Class QrDecoder is abstract class for creating the decoders of the
 * {@link com.qrcode.qrcodes.QrCode QR codes}. All decoders must extends this 
 * class.
 * 
 * @version 1.0
 */
public abstract class QrDecoder {
       
    /**
     * The exception which signals the lexical error.
     */
    public class LexicalError extends Exception {
        
        /** The Constant serialVersionUID. */
        private static final long serialVersionUID = 1L;

        /**
         * Instantiates a new lexical error exception.
         *
         * @param message the message
         */
        public LexicalError(String message) {
            super(message);
        }
    };
    
    /**
     * The exception which signals the syntax error.
     */
    public class SyntaxError extends Exception {
        
        /** The Constant serialVersionUID. */
        private static final long serialVersionUID = 1L;

        /**
         * Instantiates a new syntax error exception.
         *
         * @param message the message
         */
        public SyntaxError(String message) {
            super(message);
        }
    };
    
    /**
     * The exception which signals the semantic error.
     */
    public class SemanticError extends Exception {
        
        /** The Constant serialVersionUID. */
        private static final long serialVersionUID = 1L;

        /**
         * Instantiates a new semantic error exception.
         *
         * @param message the message
         */
        public SemanticError(String message) {
            super(message);
        }
    };
    
    /** The decoder name. */
    final protected String decoderName; 
    
    /** The supported URI schemas by this decoder. Can be null when 
     * does not support the URI representation. */
    final protected List<String> supportedSchemas;
        
    /**
     * Instantiates a new QR decoder.
     *
     * @param supportedSchemas The supported URI schemas by this decoder. Can be 
     * null when this decoder does not support the URI schemas or is binary based.
     * @param decoderName The name of this decoder.
     */
    protected QrDecoder(List<String> supportedSchemas, String decoderName) {
        this.supportedSchemas = supportedSchemas;
        this.decoderName = decoderName;       
    }
          
    /**
     * Gets the supported URI schemas.
     *
     * @return The supported schemas. Returns null whether this decoder does
     * not support URI based decoding.
     */
    public List<String> getSupportedSchemas() {
        return supportedSchemas;
    }
    
    /**
     * Checks whether is the specified URI scheme supported by this decoder.
     *
     * @param scheme The scheme to be tested.
     * @return True, if this scheme is supported, otherwise false.
     */
    public boolean isSupported(String scheme) {
        
        // Whether there is null as supported schemas, it means that 
        // decoder is binary based or does not uses URI schemas
        if (supportedSchemas == null) return true;
        
        Iterator <String> it = supportedSchemas.iterator();

        while(it.hasNext()) {
            if(it.next().equalsIgnoreCase(scheme)) return true;
        }
        
        return false;
    }
    
    /**
     * Gets the decoder name.
     *
     * @return the decoder name
     */
    public String getDecoderName() {
        return decoderName;
    }
        
    /**
     * The decode method to be implemented by the specific decoder.
     *
     * @param data The serialized QR code.
     * @return The specific QR code.
     */
    public abstract QrCode decode(DataSegments dataSegments);
}

//End of the file
