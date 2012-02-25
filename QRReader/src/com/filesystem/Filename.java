package com.filesystem;

import java.io.File;
import java.io.IOException;

public class Filename {
    private final String fullPath;
    private final char pathSeparator;
    private final char extensionSeparator;
    private static final char DEFAULT_EXTENSION_SEPARATOR = '.';
    private static final char DEFAULT_PATH_SEPARATOR = '/';

    public Filename(String fullPath, char pathSeparator, char extensionSeparator) {
        this.fullPath = fullPath;
        this.pathSeparator = pathSeparator;
        this.extensionSeparator = extensionSeparator;
    }
    
    public static String getExtension(String fullPath, char extensionSeparator) {
        // get dot + 1
        int extPos = fullPath.lastIndexOf(extensionSeparator);
        return (extPos != -1)? fullPath.substring(extPos + 1) : null;
    }
    
    public static String getExtension(String fullPath) {
        // get dot + 1
        return getExtension(fullPath, DEFAULT_EXTENSION_SEPARATOR);
    }

    public String getExtension() {
        // get dot + 1
        return getExtension(fullPath, extensionSeparator);
    }

    public static String getFilename(String fullPath, char pathSeparator, char extensionSeparator) {
        int extSepPos = fullPath.lastIndexOf(extensionSeparator);
        int pathSepPos = fullPath.lastIndexOf(pathSeparator);
        
        extSepPos = (extSepPos != -1 && pathSepPos + 1 <= extSepPos)? extSepPos : fullPath.length();
        
        return (pathSepPos != -1)? fullPath.substring(pathSepPos + 1, extSepPos) : null;
    }
    
    public static String getFilename(String fullPath) {
        return getFilename(fullPath, DEFAULT_PATH_SEPARATOR, DEFAULT_EXTENSION_SEPARATOR);
    }
    
    public String getFilename() {
        return getFilename(fullPath, pathSeparator, extensionSeparator);
    }
    
    public static String getFullFilename(String fullPath, char pathSeparator) {
        int pathSepPos = fullPath.lastIndexOf(pathSeparator);
        return (pathSepPos != -1)? fullPath.substring(pathSepPos + 1, fullPath.length()) : null;
    }
    
    public static String getFullFilename(String fullPath) {
        return getFullFilename(fullPath, DEFAULT_PATH_SEPARATOR);
    }
    
    public String getFullFilename() {
        return getFullFilename(fullPath, pathSeparator);
    }
    
    public static String getPath(String fullPath, char pathSeparator) {
        int pathSepPos = fullPath.lastIndexOf(pathSeparator);
        return (pathSepPos != -1)? fullPath.substring(0, pathSepPos) : null;
    }
    
    public static String getPath(String fullPath) {
        return getPath(fullPath, DEFAULT_PATH_SEPARATOR);
    }

    public String getPath() {
        return getPath(fullPath, pathSeparator);
    }
    
    public static boolean isSubDirectory(String dirPath, String subDirPath) {
        try {
            File dir = new File(dirPath).getCanonicalFile();
            File subDir = new File(subDirPath).getCanonicalFile();
    
            File parentFile = subDir;
            while (parentFile != null) {
                if (dir.equals(parentFile)) {
                    return true;
                }
                parentFile = parentFile.getParentFile();
            }
            return false;
        } catch (IOException e) {
            e.printStackTrace();
            return false;
        }
    }
    
    public boolean isSubDirectory(String subDirPath) {
        return isSubDirectory(fullPath, subDirPath);
    }
}