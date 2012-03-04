///////////////////////////////////////////////////////////////////////////////
// Project:    QR Reader for Android
// Package:    com.android.installation
// File:       InstallationManager.java
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Contains the major manager handling the loading of the external
//             packages with installable classes.
///////////////////////////////////////////////////////////////////////////////

package com.android.qrreader.installation;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserException;

import android.content.Context;
import android.content.ContextWrapper;
import com.filesystem.Filename;
import com.filesystem.Operations;

import dalvik.system.DexClassLoader;

/**
 * The Class InstallationManager handles the loading of external packages from the 
 * application internal directory where can be stored .jar packages (plug-ins) 
 * that can contain installable classes. This class also provides the 
 * installing/uninstalling methods for plug-in management and interface for retrieving 
 * installed classes on demand to other specific installable managers.
 */
final public class InstallationManager {
    
    /**
     * The interface of the callback for informing about installing/uninstalling plug-in.
     */
    public interface InstallationCallback {
        
        /**
         * This method is fired when a list with the loaded plug-ins has changed.
         */
        public void packageListChanged();
    };
    
    /** The default directory for plug-ins in the internal storage. */
    final private static String INSTALL_DIRECTORY       = "packages";
    
    /** The name of the XMl which contains packaged classes inside plug-in. */
    final private static String CLASSES_XML             = "res/classes.xml";
    
    /** The name of the XML which describes the plug-in (eg. name or brief). */
    final private static String PACKAGE_XML             = "res/package.xml";
    
    /** The file extension of the plug-in package. */
    final private static String PACKAGE_EXTENSION       = "jar";
    
    /** The name of the tag what specifies the name of the installable class. */
    final private static String TAG_CLASS               = "class";
    
    /** The name of the tag what specifies the type of the installable class 
     *  (destination where should be applied/used/installed). */
    final private static String TAG_DESTINATION         = "destination";
    
    /** The name of the parent tag of {@link #TAG_CLASS} and {@link #TAG_DESTINATION}. */
    final private static String TAG_DESTINATION_CLASS   = "destination-class";
    
    /** The name of tag that contains name of plug-in. */
    final private static String TAG_NAME                = "name";
    
    /** The name of the tag that contains the brief of the plug-in. */
    final private static String TAG_BRIEF               = "brief";
    
    /** The list with the callbacks to be fired on change of the plug-in list. */
    private List<InstallationCallback> installationCallbacks = new ArrayList<InstallationCallback>();
    
    /**
     * The class of the exception which is usually fired when the package is corrupted.
     */
    public class CorruptedPackage extends Exception {
        
        /** The Constant serialVersionUID. */
        private static final long serialVersionUID = 1L;

        /**
         * Instantiates a new exception of the corrupted package.
         *
         * @param message The throw message.
         */
        public CorruptedPackage(String message) {
            super(message);
        }

        /**
         * Instantiates a new exception of the corrupted package.
         */
        public CorruptedPackage() {}
    };
    
    /**
     * The class which groups the absolute package location 
     * and the name of the class.
     */
    public class PackageClass {
        
        /** The package path in which is the installable class. */
        public String packagePath;
        
        /** The class name of the installable class. */
        public String className;
    }
    
    /**
     * The class is used for storing informations about package.
     */
    public class PackageInfo {
        
        /** The name of the package. */
        public String name;
        
        /** The brief of the package. */
        public String brief;
    }
    
    /** The unique instance of the installation manager. */
    private static InstallationManager installationManager;

    /** The context. */
    final private Context context;
    
    /** The install directory for plug-ins. */
    final private File installDirectory;
    
    /**
     * Instantiates a new installation manager.
     *
     * @param context The context.
     */
    private InstallationManager (Context context) {
        this.context = context;
        
        // Getting installation directory or creating a new one
        ContextWrapper contextWrapper = new ContextWrapper(context);
        installDirectory = contextWrapper.getDir(INSTALL_DIRECTORY, Context.MODE_PRIVATE);
    }
    
    
    /**
     * Getting the instance of the installation manager.
     *
     * @param context the context
     * @return the installation manager
     */
    public static InstallationManager getInstallationManager(Context context) {
        return installationManager = (installationManager == null)? 
                new InstallationManager(context) : installationManager;
    }
    
    /**
     * Getting the list of the installable classes that passes destination criteria.
     *
     * @param classDestination The name of the destination against which should be
     * searched the loaded classes. It servers for filtering the classes and 
     * getting only demanded classes by specific manager. For retrieving actually 
     * all classes pass the null.
     * @return The list all class that are on the specified class destination.
     */
    public List<PackageClass> getInstalledClasses(String classDestination) {              
        List<File> packages = getPackages();
        List<PackageClass> packageClasses = new ArrayList<PackageClass>();
        
        // Proceed searching of the classes in all packages iteratively
        for (File installedPackage : packages) {
            packageClasses.addAll(getPackageClasses(installedPackage, classDestination));
        }
        
        return packageClasses;
    }
    
    /**
     * Getting the list of the installable classes from the specified package.
     *
     * @param packageFile The file of the package - to be discovered for installable classes.
     * @param classDestination The name of the class destination - to get only demanded classes.
     * @return The list all class that are on the specified class destination.
     */
    private List<PackageClass> getPackageClasses(File packageFile, String classDestination) {
        List<PackageClass> classes = new ArrayList<PackageClass>();
        PackageClass currClass = new PackageClass();
        String tag = new String();
        
        // Opening package and retrieving the XML which describes the installable classes
        PackageResourceXML resourceXML = new PackageResourceXML(context, packageFile, CLASSES_XML);
        if (resourceXML.open()) {
            
            // Parsing the XMl
            try {
                int eventType = resourceXML.getEventType();
                boolean destinationMatched = false;
                
                while (eventType != XmlPullParser.END_DOCUMENT) {
                    switch (eventType) {
                    
                    // START OF THE TAG
                    case XmlPullParser.START_TAG:
                        tag = resourceXML.getName();
                        break;
                        
                    // END OF THE TAG
                    case XmlPullParser.END_TAG:
                        tag = null;
                        // There is the end of the destination-class tag, so store it into 
                        // the result list whether the destination is valid 
                        if (resourceXML.getName().equalsIgnoreCase(TAG_DESTINATION_CLASS)
                                && destinationMatched) {
                            classes.add(currClass);
                        }
                        break;
                        
                    // TEXT PART PARSED    
                    case XmlPullParser.TEXT:
                        if (tag != null) {
                            
                            // destination tag, examining the validness
                            if (tag.equalsIgnoreCase(TAG_DESTINATION)) {
                                destinationMatched = classDestination == null 
                                        || resourceXML.getText().equalsIgnoreCase(classDestination);
                                
                            // class tag, storing the package path and class name
                            } else if (tag.equalsIgnoreCase(TAG_CLASS)) {
                                currClass.className = resourceXML.getText();
                                currClass.packagePath = packageFile.getAbsolutePath();
                            }
                        }
                        break;
                    }
                    eventType = resourceXML.next(); // moving to the next token
                         
                }

            } catch (XmlPullParserException e) {
                e.printStackTrace();
            } catch (IOException e) {
                e.printStackTrace();
            } 
        }
        return classes;
    }
    
    /**
     * Installs the package/plug-in from the specified file path.
     *
     * @param packagePath The file path/name of the package to be installed. 
     * @return The file of the installed package.
     * @throws CorruptedPackage Signals that the package is corrupted.
     * @throws IOException Signals that an I/O exception has occurred.
     * @throws FileNotFoundException Signals that the package not found.
     * 
     * @see #uninstallPackage(String)
     */
    public File installPackage(String packagePath) throws CorruptedPackage, IOException, FileNotFoundException {
        File packageFile = new File(packagePath);
        File installedFile = new File(installDirectory.getAbsolutePath() +
                File.separator + Filename.getFullFilename(packagePath));
        
         // File does not exist
        if (!packageFile.exists()) {    
            throw new FileNotFoundException();
            
        // Extension is not valid or no classes loaded from this package
        } else if (!Filename.getExtension(packagePath).equalsIgnoreCase(PACKAGE_EXTENSION)
                || getPackageClasses(packageFile, null).isEmpty()) {
            throw new CorruptedPackage();
        }
        
        // Trying to install the package - can throw IOException
        Operations.copyFile(packageFile, installedFile);
        
        // Fire all registered callbacks about a newly installed package
        fireInstallationCallbacks();
        return installedFile;
    }
    
    /**
     * Uninstalls the package/plug-in.
     *
     * @param packagePath the package path
     * 
     * @see #installPackage(String)
     */
    public void uninstallPackage(String packagePath) {
        new File(packagePath).delete();
        fireInstallationCallbacks(); // Fire all registered callbacks
    }
    
    /**
     * Gets the list of all installed packages.
     *
     * @return the packages
     */
    public List<File> getPackages() {
        File[] packages = installDirectory.listFiles();
        List<File> passedPackages = new ArrayList<File>();
        
        // Getting all files from the installation directory that passes only extension
        for (File installedPackage : packages) {
            if (Filename.getExtension(installedPackage.getAbsolutePath()).equalsIgnoreCase(PACKAGE_EXTENSION)) {
                passedPackages.add(installedPackage);
            }
        }
        
        return passedPackages;
    }
    
    /**
     * Constructs the new instance of the specified class that is loaded from 
     * the specified package.
     *
     * @param packagePath The file path/name to the package/plug-in.
     * @param className The class name which should be instantiated.
     * @return The new instance of the loaded class.
     * @throws InstantiationException Signals that the instance can not be created.
     * @throws IllegalAccessException Signals that the default constructor is not visible.
     * @throws ClassNotFoundException Signals that the class can not be found.
     * 
     * @see dalvik.system.DexClassLoader#loadClass(String)
     */
    public Object dexConstruct(String packagePath, String className) throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        return dexLoad(packagePath, className).newInstance();
    }
    
    /**
     * Loads the specified class from the file and does not do the instantiation.
     *
     * @param packagePath The file path/name to the package/plug-in.
     * @param className The class name which should be instantiated.
     * @return The loaded class.
     * @throws ClassNotFoundException Signals that the class can not be found.
     */
    public Class<?> dexLoad(String packagePath, String className) throws ClassNotFoundException {
        DexClassLoader classLoader = new DexClassLoader(
            packagePath, 
            context.getFilesDir().getAbsolutePath(),
            null, 
            getClass().getClassLoader()
        );
        return classLoader.loadClass(className);
    }
        
    /**
     * Retrieves the information about specified package.
     *
     * @param packageFile The file of the package to be examined.
     * @return The information about the package.
     * 
     * @see com.android.qrreader.installation.InstallationManager.PackageInfo PackageInfo
     */
    public PackageInfo getPackageInfo(File packageFile) {
        PackageInfo packageInfo = new PackageInfo();
        String tag = new String();

        // Opening the package and demanded XML containing the package informations
        PackageResourceXML resourceXML = new PackageResourceXML(context, packageFile, PACKAGE_XML);
        if (resourceXML.open()) {
            try {
                // Parsing the XML
                int eventType = resourceXML.getEventType();
                while (eventType != XmlPullParser.END_DOCUMENT) {
                    switch (eventType) {
                    case XmlPullParser.START_TAG:
                        tag = resourceXML.getName();
                        break;
                    case XmlPullParser.END_TAG:
                        tag = null;
                        break;
                    case XmlPullParser.TEXT:
                        if (tag != null) {
                            
                            // Getting the name of the package
                            if (tag.equalsIgnoreCase(TAG_NAME)) {
                                packageInfo.name = resourceXML.getText();
                                
                            // Getting the brief of the package
                            } else if (tag.equalsIgnoreCase(TAG_BRIEF)) {
                                packageInfo.brief = resourceXML.getText();
                            }
                        }
                        break;
                    }
                    eventType = resourceXML.next();
                         
                }
            } catch (XmlPullParserException e) {
                e.printStackTrace();
                return null;
            } catch (IOException e) {
                e.printStackTrace();
                return null;
            }
        }
        
        return packageInfo;
    }
    
    /**
     * Register installation callback.
     *
     * @param callback the callback
     */
    public void registerInstallationCallback(InstallationCallback callback) {
        installationCallbacks.add(callback);
    }
    
    /**
     * Removes the installation callback.
     *
     * @param callback the callback
     */
    public void removeInstallationCallback(InstallationCallback callback) {
        installationCallbacks.remove(callback);
    }
    
    /**
     * Fire installation callbacks.
     */
    private void fireInstallationCallbacks() {
        for (InstallationCallback callback : installationCallbacks) {
            callback.packageListChanged();
        }
    }
}

//End of the file
