package com.android.qrreader.qrcode;

import java.util.List;

import android.content.Context;
import android.view.View;

import com.adapter.Adapter;
import com.adapter.AdapterFactory;
import com.android.qrreader.installation.InstallationManager;
import com.android.qrreader.installation.InstallationManager.InstallationCallback;
import com.android.qrreader.installation.InstallationManager.PackageClass;
import com.android.qrreader.qrcode.views.ContactQrCodeView;
import com.android.qrreader.qrcode.views.MailQrCodeView;
import com.android.qrreader.qrcode.views.QrCodeViewProvider;
import com.android.qrreader.qrcode.views.TelephoneQrCodeView;
import com.android.qrreader.qrcode.views.UrlQrCodeView;
import com.qrcode.qrcodes.QrCode;

public class InstallableQrCodeViewManager {
    final private String CLASS_DESTINATION = "QrCodeView";
    
    private static InstallableQrCodeViewManager qrCodeViewManager;
    final private Context context;
    
    private InstallationCallback packagesChanged = new InstallationCallback() {
        public void packageListChanged() {
            loadInstalledAdapters();
        }
    };
    
    private InstallableQrCodeViewManager (Context context) {
        this.context = context;

        InstallationManager installationManager = InstallationManager.getInstallationManager(this.context);
        installationManager.registerInstallationCallback(packagesChanged);
        
        loadInternalAdapters();
        loadInstalledAdapters();
    }
    
    private void loadInternalAdapters() {
        AdapterFactory adapterFactory = AdapterFactory.getFactory();
        adapterFactory.registerAdapter(UrlQrCodeView.class);
        adapterFactory.registerAdapter(MailQrCodeView.class);
        adapterFactory.registerAdapter(ContactQrCodeView.class);
        adapterFactory.registerAdapter(TelephoneQrCodeView.class);
    }
    
    private void loadInstalledAdapters() {
        AdapterFactory adapterFactory = AdapterFactory.getFactory();
        InstallationManager installationManager = InstallationManager.getInstallationManager(context);
        List<PackageClass> classes = installationManager.getInstalledClasses(CLASS_DESTINATION);

        if (classes != null) {
            for (PackageClass packageClass : classes) {
                try {
                    Class<?> adapter = installationManager.dexLoad(packageClass.packagePath, packageClass.className);
                    adapterFactory.registerAdapter(adapter);
                } catch (ClassNotFoundException e) {
                    e.printStackTrace();
                }
            }
        }
    }
    
    public static InstallableQrCodeViewManager getViewManager(Context context) {
        return qrCodeViewManager = (qrCodeViewManager == null)? new InstallableQrCodeViewManager(context) : qrCodeViewManager;
    }
    
    public View getViewForQrCode(QrCode qrCode) {
        AdapterFactory adapterFactory = AdapterFactory.getFactory();
        Adapter adapter = adapterFactory.getAdapter(qrCode.getClass(), View.class);
        
        if (adapter != null) {
            try {
                QrCodeViewProvider provider = (QrCodeViewProvider)adapter.getProvider(qrCode);
                return provider.getView(context);
            } catch (ClassCastException e) {
                e.printStackTrace();
            }

        }
        
        return null;
    }
    
    public String getSubTitleForQrCode(QrCode qrCode) {
        AdapterFactory adapterFactory = AdapterFactory.getFactory();
        Adapter adapter = adapterFactory.getAdapter(qrCode.getClass(), View.class);
        
        if (adapter != null) {
            try {
                QrCodeViewProvider provider = (QrCodeViewProvider)adapter.getProvider(qrCode);
                return provider.getTitleName(context);
            } catch (ClassCastException e) {
                e.printStackTrace();
            }

        }
        
        return null;
    }
}
