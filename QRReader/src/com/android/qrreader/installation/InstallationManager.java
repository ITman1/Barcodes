package com.android.qrreader.installation;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.channels.FileChannel;
import java.util.ArrayList;
import java.util.List;

import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserException;

import android.content.Context;
import android.content.ContextWrapper;
import android.net.Uri;

import com.filesystem.Filename;
import com.filesystem.Operations;

import dalvik.system.DexClassLoader;

final public class InstallationManager {
    
    public interface InstallationCallback {
        public void packageListChanged();
    };
    
    final private static String INSTALL_DIRECTORY       = "packages";
    final private static String CLASSES_XML             = "res/classes.xml";
    final private static String PACKAGE_XML             = "res/package.xml";
    final private static String PACKAGE_EXTENSION       = "jar";
    
    final private static String TAG_CLASS               = "class";
    final private static String TAG_DESTINATION         = "destination";
    final private static String TAG_DESTINATION_CLASS   = "destination-class";
    
    final private static String TAG_NAME                = "name";
    final private static String TAG_BRIEF               = "brief";
    
    private List<InstallationCallback> installationCallbacks = new ArrayList<InstallationCallback>();
    
    public class CorruptedPackage extends Exception {
        private static final long serialVersionUID = 1L;

        public CorruptedPackage(String message) {
            super(message);
        }

        public CorruptedPackage() {}
    };
    
    public class PackageClass {
        public String packagePath;
        public String className;
    }
    
    public class PackageInfo {
        public String name;
        public String brief;
    }
    
    private static InstallationManager installationManager;
    

    final private Context context;
    final private File installDirectory;
    
    private InstallationManager (Context context) {
        this.context = context;
        
        ContextWrapper contextWrapper = new ContextWrapper(context);
        installDirectory = contextWrapper.getDir(INSTALL_DIRECTORY, Context.MODE_PRIVATE);
    }
    
    
    public static InstallationManager getInstallationManager(Context context) {
        return installationManager = (installationManager == null)? new InstallationManager(context) : installationManager;
    }
    
    public List<PackageClass> getInstalledClasses(String classDestination) {              
        List<File> packages = getPackages();
        List<PackageClass> packageClasses = new ArrayList<PackageClass>();
        
        for (File installedPackage : packages) {
            packageClasses.addAll(getPackageClasses(installedPackage, classDestination));
        }
        
        return packageClasses;
    }
    
    private List<PackageClass> getPackageClasses(File packageFile, String classDestination) {
        List<PackageClass> classes = new ArrayList<PackageClass>();
        PackageClass currClass = new PackageClass();
        String tag = new String();
        
        PackageResourceXML resourceXML = new PackageResourceXML(context, packageFile, CLASSES_XML);
        
        if (resourceXML.open()) {
            try {
                int eventType = resourceXML.getEventType();
                boolean destinationMatched = false;
                while (eventType != XmlPullParser.END_DOCUMENT) {
                    switch (eventType) {
                    case XmlPullParser.START_TAG:
                        tag = resourceXML.getName();
                        break;
                    case XmlPullParser.END_TAG:
                        tag = null;
                        if (resourceXML.getName().equalsIgnoreCase(TAG_DESTINATION_CLASS) && destinationMatched) {
                            classes.add(currClass);
                        }
                        break;
                    case XmlPullParser.TEXT:
                        if (tag != null) {
                            if (tag.equalsIgnoreCase(TAG_DESTINATION)) {
                                destinationMatched = classDestination == null || resourceXML.getText().equalsIgnoreCase(classDestination);
                            } else if (tag.equalsIgnoreCase(TAG_CLASS)) {
                                currClass.className = resourceXML.getText();
                                currClass.packagePath = packageFile.getAbsolutePath();
                            }
                        }
                        break;
                    }
                    eventType = resourceXML.next();
                         
                }

            } catch (XmlPullParserException e) {
                e.printStackTrace();
            } catch (IOException e) {
                e.printStackTrace();
            } 
        }
        return classes;
    }
    
    public File installPackage(String packagePath) throws CorruptedPackage, IOException, FileNotFoundException {
        File packageFile = new File(packagePath);
        File installedFile = new File(installDirectory.getAbsolutePath() + File.separator + Filename.getFullFilename(packagePath));
        
        if (!packageFile.exists()) {
            throw new FileNotFoundException();
        } else if (!Filename.getExtension(packagePath).equalsIgnoreCase(PACKAGE_EXTENSION) || getPackageClasses(packageFile, null).isEmpty()) {
            throw new CorruptedPackage();
        }
        
        Operations.copyFile(packageFile, installedFile);
        
        fireInstallationCallbacks();
        return installedFile;
    }
    
    public void uninstallPackage(String packagePath) {
        new File(packagePath).delete();
        fireInstallationCallbacks();
    }
    
    public List<File> getPackages() {
        File[] packages = installDirectory.listFiles();
        List<File> passedPackages = new ArrayList<File>();
        
        for (File installedPackage : packages) {
            if (Filename.getExtension(installedPackage.getAbsolutePath()).equalsIgnoreCase(PACKAGE_EXTENSION)) {
                passedPackages.add(installedPackage);
            }
        }
        
        return passedPackages;
    }
    
    public Object dexConstruct(String packagePath, String className) throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        DexClassLoader classLoader = new DexClassLoader(
            packagePath, 
            context.getFilesDir().getAbsolutePath(),
            null, 
            getClass().getClassLoader()
        );
        return classLoader.loadClass(className).newInstance();
    }
    
    public Class<?> dexLoad(String packagePath, String className) throws ClassNotFoundException {
        DexClassLoader classLoader = new DexClassLoader(
            packagePath, 
            context.getFilesDir().getAbsolutePath(),
            null, 
            getClass().getClassLoader()
        );
        return classLoader.loadClass(className);
    }
        
    public PackageInfo getPackageInfo(File packageFile) {
        PackageInfo packageInfo = new PackageInfo();
        String tag = new String();

        PackageResourceXML resourceXML = new PackageResourceXML(context, packageFile, PACKAGE_XML);
        
        if (resourceXML.open()) {
            try {
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
                            if (tag.equalsIgnoreCase(TAG_NAME)) {
                                packageInfo.name = resourceXML.getText();
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
    
    public void registerInstallationCallback(InstallationCallback callback) {
        installationCallbacks.add(callback);
    }
    
    public void removeInstallationCallback(InstallationCallback callback) {
        installationCallbacks.remove(callback);
    }
    
    private void fireInstallationCallbacks() {
        for (InstallationCallback callback : installationCallbacks) {
            callback.packageListChanged();
        }
    }
    
}
