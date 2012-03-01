package com.android.qrreader.qrcode;

import java.util.ArrayList;
import java.util.List;

import android.content.Context;

import com.android.qrreader.installation.InstallationManager;
import com.android.qrreader.installation.InstallationManager.InstallationCallback;
import com.android.qrreader.installation.InstallationManager.PackageClass;
import com.qrcode.decoders.QrDecoder;
import com.qrcode.QrDecoderManager;
import com.qrcode.qrcodes.QrCode;

final public class InstallableQrDecoderManager {
    final private String CLASS_DESTINATION = "QrDecoder";
        
    private static InstallableQrDecoderManager qrDecoderManager;
    final private QrDecoderManager qrStaticDecoderManager;
    final private Context context;
    
    private ArrayList<QrDecoder> decoders = new ArrayList<QrDecoder>();
    
    private InstallationCallback packagesChanged = new InstallationCallback() {
        public void packageListChanged() {
            reloadDecoders();
        }
    };
    
    private InstallableQrDecoderManager (Context context) {
        this.context = context;
        qrStaticDecoderManager = QrDecoderManager.getDecoderManager();

        InstallationManager installationManager = InstallationManager.getInstallationManager(this.context);
        installationManager.registerInstallationCallback(packagesChanged);
        
        reloadDecoders();
    }
    
    private void reloadDecoders() {
        InstallationManager installationManager = InstallationManager.getInstallationManager(this.context);
        List<PackageClass> classes = installationManager.getInstalledClasses(CLASS_DESTINATION);

        decoders.clear();
        if (classes != null) {
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
        return qrDecoderManager = (qrDecoderManager == null)? new InstallableQrDecoderManager(context) : qrDecoderManager;
    }
}
