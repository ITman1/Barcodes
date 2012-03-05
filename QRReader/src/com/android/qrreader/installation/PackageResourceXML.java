///////////////////////////////////////////////////////////////////////////////
// Project:    QR Reader for Android
// Package:    com.android.qrreader.installation
// File:       PackageResourceXML.java
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Contains the class which loads the XML files from the .jar 
//             packages.
///////////////////////////////////////////////////////////////////////////////

package com.android.qrreader.installation;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserException;
import org.xmlpull.v1.XmlPullParserFactory;

import android.content.Context;

import dalvik.system.DexClassLoader;

/**
 * The Class PackageResourceXML provides the interface for loading 
 * the XML files from the .jar packages and reading its content via 
 * {@link org.xmlpull.v1.XmlPullParser XmlPullParser} through using 
 * of the parser events.
 * 
 * @see org.xmlpull.v1.XmlPullParser XmlPullParser
 * 
 * @version 1.0
 */
public class PackageResourceXML {
    
    /** The exception text which is thrown when opening the XML failed. */
    final private static String XML_NOT_OPENED = "Resource XML is not opened!";
    
    /** The context. */
    final private Context context;
    
    /** The processed package file. */
    final private File packageFile;
    
    /** The name of the resource (includes whole path structure inside the package). */
    final private String resName;
    
    /** The XML parser. */
    private XmlPullParser parser;
    
    /** The input stream of the XML during parsing. */
    private InputStream resInputStream;
    
    /**
     * Instantiates a new object for loading XML.
     *
     * @param context The context.
     * @param packageFile The path/file name of the package file.
     * @param resName The resource name of the XML inside the package.
     */
    public PackageResourceXML(Context context, File packageFile, String resName) {
        this.context = context;
        this.packageFile = packageFile;
        this.resName = resName;       
    }
    
    /**
     * Try to open the XML resource inside the package.
     *
     * @return True, if successfully opened, otherwise false.
     */
    public boolean open() {
        // Create the loader of the resources inside the package.
        DexClassLoader classLoader = new DexClassLoader(
                packageFile.getAbsolutePath(), 
                context.getFilesDir().getAbsolutePath(),
                null, 
                getClass().getClassLoader()
            );

        // Try to get an input stream from the resource
        try {
            resInputStream = classLoader.getResourceAsStream(resName);
            if (resInputStream != null) {
                parser = XmlPullParserFactory.newInstance().newPullParser();
                parser.setInput(resInputStream, null);
                return true;
            }
        } catch (XmlPullParserException e) {
            e.printStackTrace();
        }
                
        // Probably some error occurred
        parser = null;
        return false;
    }
    
    /**
     * Close the opened XML resource.
     */
    public void close() {
        if (parser != null) {
            parser = null;
            try {
                resInputStream.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
    
    /**
     * Gets the current occurred event which happened during the parsing of XML.
     *
     * @return The event type which happened during parsing the XML.
     * @throws XmlPullParserException Signals that XML resource is not opened.
     * 
     * @see org.xmlpull.v1.XmlPullParser#getEventType() getEventType()
     */
    int getEventType() throws XmlPullParserException {
        if (parser == null) {
            throw new XmlPullParserException(XML_NOT_OPENED);
        }
        
        return parser.getEventType();
    }
    
    /**
     * Gets the current processed name of the XML tag.
     *
     * @return The name of the processed tag.
     * @throws XmlPullParserException Signals that XML resource is not opened.
     * 
     * @see org.xmlpull.v1.XmlPullParser#getName() getName()
     */
    String getName() throws XmlPullParserException {
        if (parser == null) {
            throw new XmlPullParserException(XML_NOT_OPENED);
        }
        
        return parser.getName();
    }
    
    /**
     * Gets the current processed text inside the XML tags.
     *
     * @return The text inside the processed tags.
     * @throws XmlPullParserException Signals that XML resource is not opened.
     * 
     * @see org.xmlpull.v1.XmlPullParser#getText() getText()
     */
    String getText() throws XmlPullParserException {
        if (parser == null) {
            throw new XmlPullParserException(XML_NOT_OPENED);
        }
        
        return parser.getText();
    }

    /**
     * Gets the next event which will happen during the parsing of XML.
     *
     * @return The new type of the event which has occurred.
     * @throws XmlPullParserException Signals that XML resource is not opened.
     * @throws IOException Signals that an I/O exception has occurred.
     * 
     * @see org.xmlpull.v1.XmlPullParser#next() next()
     */
    int next() throws XmlPullParserException, IOException {
        if (parser == null) {
            throw new XmlPullParserException(XML_NOT_OPENED);
        }
        
        return parser.next();
    }
}

//End of the file
