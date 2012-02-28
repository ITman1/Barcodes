package com.android.qrreader;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;

import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserException;
import org.xmlpull.v1.XmlPullParserFactory;

import android.content.Context;
import android.content.ContextWrapper;

import com.qrcode.decoders.QrDecoder;
import com.qrcode.QrDecoderManager;
import com.qrcode.qrcodes.QrCode;

import dalvik.system.DexClassLoader;

final public class InstallableQrDecoderManager {
    
    private class PackageClass {
        String packageName;
        String className;
    }
    
    final private static String INSTALL_DIRECTORY = "packages";
    final private static String INSTALL_XML = "package_class.xml";
    private static InstallableQrDecoderManager qrDecoderManager;
    final private QrDecoderManager qrStaticDecoderManager;
    final private Context context;
    
    private ArrayList<QrDecoder> decoders = new ArrayList<QrDecoder>();
    
    private InstallableQrDecoderManager (Context context) throws ClassNotFoundException, InstantiationException, IllegalAccessException {
        this.context = context;
        qrStaticDecoderManager = QrDecoderManager.getDecoderManager();
        
        ContextWrapper contextWrapper = new ContextWrapper(context);
        File directory = contextWrapper.getDir(INSTALL_DIRECTORY, Context.MODE_PRIVATE);
        String pathDescriptionXML = directory.getAbsolutePath() + File.separator + INSTALL_XML;
        List<PackageClass> classes;

        if (new File(pathDescriptionXML).exists()) {
            classes = getInstalledClasses(pathDescriptionXML);

            for (PackageClass packageClass : classes) {
                DexClassLoader classLoader = new DexClassLoader(
                    packageClass.packageName, "/", null, getClass().getClassLoader());
                Object decoder = classLoader.loadClass(packageClass.className).newInstance();
                if (decoder instanceof QrDecoder) decoders.add((QrDecoder)decoder);
            }
        } else {
            createDescriptionXML(pathDescriptionXML);
        }

    }
    
    private void createDescriptionXML(String pathDescriptionXML) {
        try {
            FileOutputStream outputStream = new FileOutputStream(new File(pathDescriptionXML));
            
            outputStream.write("<?xml version=\"1.0\" encoding=\"utf-8\"?>".getBytes());
            outputStream.write("<root>".getBytes());
            if (decoders != null) {
                for (QrDecoder decoder : decoders) {
                    outputStream.write("<package-class>".getBytes());
                    outputStream.write("<package>".getBytes());
                    outputStream.write(decoder.getClass().getPackage().getName().getBytes());
                    outputStream.write("</package>".getBytes());
                    outputStream.write("<class>".getBytes());
                    outputStream.write(decoder.getClass().getName().getBytes());
                    outputStream.write("</class>".getBytes());
                    outputStream.write("</package-class>".getBytes());
                }
            }
            outputStream.write("</root>".getBytes());
            outputStream.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e){
            e.printStackTrace();
        }
    }
    
    private List<PackageClass> getInstalledClasses(String pathDescriptionXML) {
        List<PackageClass> classes = new ArrayList<PackageClass>();
        PackageClass currClass = new PackageClass();
        String tag = new String();
        
        try {
            FileInputStream inputStream = new FileInputStream(pathDescriptionXML);
            XmlPullParser parser = XmlPullParserFactory.newInstance().newPullParser();
            parser.setInput(inputStream, null);
    
            int eventType = parser.getEventType();
            while (eventType != XmlPullParser.END_DOCUMENT) {
                switch (eventType) {
                case XmlPullParser.START_TAG:
                    tag = parser.getName();
                    break;
                case XmlPullParser.END_TAG:
                    tag = parser.getName();
                    if (tag.equalsIgnoreCase("package-class")) {
                        classes.add(currClass);
                    }
                    break;
                case XmlPullParser.TEXT:
                    if (tag.equalsIgnoreCase("class")) {
                        currClass.className = parser.getText();
                    } else if (tag.equalsIgnoreCase("package")) {
                        currClass.packageName = parser.getText();
                    }
                    break;
                }
                 
            }
            inputStream.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (XmlPullParserException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return classes;
    }
    
    public QrCode decodeQrCode(byte[] data) {
        QrCode qrCode = qrStaticDecoderManager.decodeQrCode(data);
        return (qrCode == null)? QrDecoderManager.decodeQrCode(decoders, data) : qrCode;
    }
    
    public List<QrDecoder> getDecoders() {
        ArrayList<QrDecoder> decList = null;
        
        if (qrStaticDecoderManager != null) {
            decList = qrStaticDecoderManager.getDecoders();
        }
        
        if (decList == null) {
            decList = decoders;
        } else {
            decList.addAll(decoders);
        }
        
        return decList;
        
    }
    
    public static InstallableQrDecoderManager getDecoderManager(Context context) {
        try {
            return qrDecoderManager = (qrDecoderManager == null)? new InstallableQrDecoderManager(context) : qrDecoderManager;
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } catch (InstantiationException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        }
        
        return qrDecoderManager = null;
    }
}
