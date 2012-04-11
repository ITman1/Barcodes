///////////////////////////////////////////////////////////////////////////////
// Project:    QR Reader for Android
// Package:    com.android.qrreader.qrcode
// File:       InstallableQrDecoderManager.java
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Contains the class which provides interface for decoding the 
//             QR codes.
///////////////////////////////////////////////////////////////////////////////

package com.android.qrreader.qrcode;

import java.util.ArrayList;
import java.util.List;

import android.content.Context;

import com.android.qrreader.installation.InstallationManager;
import com.android.qrreader.installation.InstallationManager.InstallationCallback;
import com.android.qrreader.installation.InstallationManager.PackageClass;
import com.qrcode.decoders.QrDecoder;
import com.qrcode.QrCodes.DataSegments;
import com.qrcode.QrDecoderManager;
import com.qrcode.qrcodes.QrCode;

/**
 * The Class InstallableQrDecoderManager provides the interface for decoding 
 * the {@link com.qrcode.qrcodes.QrCode QR codes}. It uses the 
 * {@link com.qrcode.QrDecoderManager QrDecoderManager}
 * for actual decoding. This class only extends the manager about installing 
 * the custom decoders via plug-ins. 
 * <p>
 * For providing the installable decoder classes inside installable .jar packages 
 * should be used the destination {@value #CLASS_DESTINATION}. The decoder should
 * implement abstract class {@link com.qrcode.decoders.QrDecoder QrDecoder}.
 * 
 * @see com.qrcode.QrDecoderManager QrDecoderManager
 * @see com.android.qrreader.installation.InstallationManager InstallationManager
 *  
 * @version 1.0
 */
final public class InstallableQrDecoderManager {
    
    /** The destination of installable classes that handles this manager. */
    final static private String CLASS_DESTINATION = "QrDecoder";
        
    /** The instance of this manager. */
    private static InstallableQrDecoderManager qrDecoderManager;
    
    /** The instance of decoder manager which handles statically loaded decoders. */
    final private QrDecoderManager qrStaticDecoderManager;
    
    /** The context. */
    final private Context context;
    
    /** The list containing all decoders. */
    private ArrayList<QrDecoder> decoders = new ArrayList<QrDecoder>();
    
    /** The callback which is called by {@link com.android.qrreader.installation.InstallationManager installation manager}
     * when a list of plug-ins has changed. */
    private InstallationCallback packagesChanged = new InstallationCallback() {
        public void packageListChanged() {
            reloadDecoders();
        }
    };
    
    /**
     * Instantiates a new installable QR decoder manager.
     *
     * @param context the context
     */
    private InstallableQrDecoderManager (Context context) {
        this.context = context;
        qrStaticDecoderManager = QrDecoderManager.getDecoderManager();

        // Registering the plug-in change callback and loading the decoders
        InstallationManager installationManager = InstallationManager.getInstallationManager(this.context);
        installationManager.registerInstallationCallback(packagesChanged);
        reloadDecoders();
    }
    
    /**
     * Reloads the list with the installable decoders.
     */
    private void reloadDecoders() {
        InstallationManager installationManager = InstallationManager.getInstallationManager(this.context);
        List<PackageClass> classes = installationManager.getInstalledClasses(CLASS_DESTINATION);

        decoders.clear();
        if (classes != null) {
            
            // All decoders are iteratively instantiated
            for (PackageClass packageClass : classes) {
                Object decoder;
                try {
                    decoder = installationManager.dexConstruct(packageClass.packagePath, packageClass.className);
                    if (decoder instanceof QrDecoder) decoders.add((QrDecoder)decoder);
                } catch (InstantiationException e) {
                    e.printStackTrace();
                } catch (IllegalAccessException e) {
                    e.printStackTrace();
                } catch (ClassNotFoundException e) {
                    e.printStackTrace();
                } catch (ClassCastException e) {
                    e.printStackTrace();
                }
            }
        }
    }
       
    /**
     * Decodes the QR code.
     *
     * @param The byte array that should contain the serialized QR code object.
     * @return The specific QR code object on success or null when no proprietary
     * decoder has been found.
     * 
     */
    public QrCode decodeQrCode(DataSegments dataSegments) {
        QrCode qrCode = qrStaticDecoderManager.decodeQrCode(dataSegments);
        return (qrCode == null)? QrDecoderManager.decodeQrCode(decoders, dataSegments) : qrCode;
    }
    
    /**
     * Gets the list of all used decoders, includes also static decoders.
     *
     * @return the decoders
     */
    public List<QrDecoder> getDecoders() {
        ArrayList<QrDecoder> decList = null;
        
        // Gets static decoders
        if (qrStaticDecoderManager != null) {
            decList = qrStaticDecoderManager.getDecoders();
        }
        
        // Gets installable decoders
        if (decList == null) {
            decList = decoders;
        } else {
            decList.addAll(decoders);
        }
        
        return decList;
        
    }
    
    /**
     * Gets the decoder manager instance.
     *
     * @param context The context.
     * @return The instance of the decoder manager.
     */
    public static InstallableQrDecoderManager getDecoderManager(Context context) {
        return qrDecoderManager = (qrDecoderManager == null)? 
                new InstallableQrDecoderManager(context) : qrDecoderManager;
    }
}

//End of the file
