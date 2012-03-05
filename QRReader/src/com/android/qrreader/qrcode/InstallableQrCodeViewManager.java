///////////////////////////////////////////////////////////////////////////////
// Project:    QR Reader for Android
// Package:    com.android.qrreader.qrcode
// File:       InstallableQrCodeViewManager.java
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Contains the class which provides interface for retrieving 
//             the views for specific QR codes.
///////////////////////////////////////////////////////////////////////////////

package com.android.qrreader.qrcode;

import java.util.List;

import android.content.Context;
import android.view.View;

import com.adapter.Adapter;
import com.adapter.AdapterRegistry;
import com.android.qrreader.installation.InstallationManager;
import com.android.qrreader.installation.InstallationManager.InstallationCallback;
import com.android.qrreader.installation.InstallationManager.PackageClass;
import com.android.qrreader.qrcode.views.*;
import com.qrcode.qrcodes.QrCode;

/**
 * The Class InstallableQrCodeViewManager provides the interface for retrieving 
 * the {@link android.view.View views} for specific QR code objects. For providing 
 * the views is used of the {@link com.adapter.AdapterRegistry adapter registry}. The
 * view {@link com.adapter.Adapter adapters} are registered locally and are loaded
 * from the internal package {@link com.android.qrreader.qrcode.views} and from the
 * plug-ins through the {@link com.android.qrreader.installation.InstallationManager
 * installation manager}. 
 * <p>
 * For providing the installable view classes inside installable .jar packages 
 * should be used the destination {@value #CLASS_DESTINATION}. The adapter provider
 * of the QR code views should implement the {@link com.android.qrreader.qrcode.
 * views.QrCodeViewProvider QrCodeViewProvider}.
 * 
 * @see com.adapter.AdapterRegistry AdapterRegistry
 * @see com.android.qrreader.qrcode.views.QrCodeViewProvider QrCodeViewProvider
 * @see com.android.qrreader.installation.InstallationManager InstallationManager
 * 
 * @version 1.0
 */
public class InstallableQrCodeViewManager {
    
    /** The destination of installable classes that handles this manager. */
    final private static String CLASS_DESTINATION = "QrCodeView";
    
    /** The unique instance of this manager. */
    private static InstallableQrCodeViewManager qrCodeViewManager;
    
    /** The context. */
    final private Context context;
    
    /** The callback which is called by {@link com.android.qrreader.installation.
     * InstallationManager installation manager} when a list of plug-ins has changed. */
    private InstallationCallback packagesChanged = new InstallationCallback() {
        public void packageListChanged() {
            loadInstalledAdapters();
        }
    };
    
    /**
     * Instantiates a new installable QR code view manager.
     *
     * @param context the context
     */
    private InstallableQrCodeViewManager (Context context) {
        this.context = context;

        InstallationManager installationManager = InstallationManager.getInstallationManager(this.context);
        installationManager.registerInstallationCallback(packagesChanged);
        
        // Views loaded internally from com.android.qrreader.qrcode.views and
        // from installation manager
        loadInternalAdapters();
        loadInstalledAdapters();
    }
    
    /**
     * Loads the internal adapters inside com.android.qrreader.qrcode.views
     * and registers them inside the {@link com.adapter.AdapterRegistry adapter registry}.
     */
    private void loadInternalAdapters() {
        AdapterRegistry adapterRegistry = AdapterRegistry.getRegistry();
        adapterRegistry.registerAdapter(HttpLinkQrCodeView.class);
        adapterRegistry.registerAdapter(MailQrCodeView.class);
        adapterRegistry.registerAdapter(ContactQrCodeView.class);
        adapterRegistry.registerAdapter(TelephoneQrCodeView.class);
        adapterRegistry.registerAdapter(SmsQrCodeView.class);
    }
    
    /**
     * Loads the installed adapters via {@link com.android.qrreader.installation.
     * InstallationManager installation manager} and registers them inside 
     * the {@link com.adapter.AdapterRegistry adapter registry}.
     */
    private void loadInstalledAdapters() {
        AdapterRegistry adapterRegistry = AdapterRegistry.getRegistry();
        InstallationManager installationManager = InstallationManager.getInstallationManager(context);
        List<PackageClass> classes = installationManager.getInstalledClasses(CLASS_DESTINATION);

        if (classes != null) {        // Some installable classes really exist 
            
            // Iterative registering inside the adapter registry
            for (PackageClass packageClass : classes) {
                try {
                    Class<?> adapter = installationManager.dexLoad(packageClass.
                            packagePath, packageClass.className);
                    adapterRegistry.registerAdapter(adapter);
                } catch (ClassNotFoundException e) {
                    e.printStackTrace();
                }
            }
        }
    }
    
    /**
     * Gets the instance of the view manager.
     *
     * @param context The context.
     * @return The instance of the view manager.
     */
    public static InstallableQrCodeViewManager getViewManager(Context context) {
        return qrCodeViewManager = (qrCodeViewManager == null)? 
                new InstallableQrCodeViewManager(context) : qrCodeViewManager;
    }
    
    /**
     * Retrieves the view for specific QR code from the 
     * {@link com.adapter.AdapterRegistry adapter registry}.
     *
     * @param qrCode The specific QR code to be searched inside the adapter registry.
     * @return The view for passed QR code.
     * 
     * @see #getProviderForQrCode(QrCode)
     */
    public View getViewForQrCode(QrCode qrCode) {
        QrCodeViewProvider provider = getProviderForQrCode(qrCode);
        return (provider != null)? provider.getView(context) : null;
    }
    
    /**
     * Gets the title for specific QR code.
     *
     * @param qrCode The specific QR code.
     * @return The title for passed QR code.
     * 
     * @see com.android.qrreader.qrcode.views.QrCodeViewProvider
     * #getTitleName(Context) getTitleName(Context)
     */
    public String getTitleForQrCode(QrCode qrCode) {
        QrCodeViewProvider provider = getProviderForQrCode(qrCode);
        return (provider != null)? provider.getTitleName(context) : null;
    }
    
    /**
     * Retrieves the view provider for specific QR code from the 
     * {@link com.adapter.AdapterRegistry adapter registry}.
     *
     * @param qrCode The specific QR code to be searched inside the adapter registry.
     * @return The view provider for passed QR code.
     */
    public QrCodeViewProvider getProviderForQrCode(QrCode qrCode) {
        AdapterRegistry adapterRegistry = AdapterRegistry.getRegistry();
        Adapter adapter = adapterRegistry.getAdapter(qrCode.getClass(), QrCodeViewProvider.class);

        if (adapter != null) {
            try {
                QrCodeViewProvider provider = (QrCodeViewProvider)adapter.getProvider(qrCode);
                return provider;
            } catch (ClassCastException e) {
                e.printStackTrace();
            }

        }
        
        return null;
    }
}

//End of the file
