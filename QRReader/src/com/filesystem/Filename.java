///////////////////////////////////////////////////////////////////////////////
// Project:    QR Reader for Android
// Package:    com.filesystem
// File:       Filename.java
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Contains the class which implements some additional features 
//             above file paths/name which are not in the standard java.
///////////////////////////////////////////////////////////////////////////////

package com.filesystem;

import java.io.File;
import java.io.IOException;

/**
 * The Class Filename implements some additional features above file paths/names
 * which are not available in the standard java.
 * 
 * @version 1.0
 */
final public class Filename {
    
    /** The full path to the file. */
    private final String fullPath;
    
    /** The path separator by which to parse the path. */
    private final char pathSeparator;
    
    /** The extension separator by which to parse the file extension. */
    private final char extensionSeparator;
    
    /** The default extension separator {@link #extensionSeparator}. */
    public static final char DEFAULT_EXTENSION_SEPARATOR = '.';
    
    /** The default path separator {@link #pathSeparator}. */
    public static final char DEFAULT_PATH_SEPARATOR = '/';

    /**
     * Instantiates a new filename object.
     *
     * @param fullPath The full path to the file.
     * @param pathSeparator The path separator by which to parse the path.
     * @param extensionSeparator The extension separator by which to parse the file extension.
     */
    public Filename(String fullPath, char pathSeparator, char extensionSeparator) {
        this.fullPath = fullPath;
        this.pathSeparator = pathSeparator;
        this.extensionSeparator = extensionSeparator;
    }
    
    /**
     * Gets the extension of the file.
     *
     * @param fullPath The full path to the file.
     * @param extensionSeparator The extension separator by which to parse the file extension.
     * @return The extension of the file.
     * 
     * @see #getExtension(String)
     */
    public static String getExtension(String fullPath, char extensionSeparator) {
        // Get position separator + 1
        int extPos = fullPath.lastIndexOf(extensionSeparator);
        return (extPos != -1)? fullPath.substring(extPos + 1) : null;
    }
    
    /**
     * Gets the extension of the file with the default file separator
     * extension {@link #DEFAULT_EXTENSION_SEPARATOR}.
     *
     * @param fullPath The full path to the file.
     * @return The extension of the file.
     * 
     * @see #getExtension(String, char)
     */
    public static String getExtension(String fullPath) {
        // get dot + 1
        return getExtension(fullPath, DEFAULT_EXTENSION_SEPARATOR);
    }

    /**
     * Gets the extension of this file.
     *
     * @return The extension of the file.
     */
    public String getExtension() {
        // get dot + 1
        return getExtension(fullPath, extensionSeparator);
    }

    /**
     * Gets the filename without file extension.
     *
     * @param fullPath The full path to the file.
     * @param pathSeparator The path separator by which to parse the path.
     * @param extensionSeparator The extension separator by which to parse the file extension.
     * @return The filename without file extension.
     * 
     * @see #getFilename(String)
     */
    public static String getFilename(String fullPath, char pathSeparator, char extensionSeparator) {
        int extSepPos = fullPath.lastIndexOf(extensionSeparator);   // position of the extension
        int pathSepPos = fullPath.lastIndexOf(pathSeparator);       // position of the last path separator
        
        // it there is no extension separator end with length of the full path
        extSepPos = (extSepPos != -1 && pathSepPos + 1 <= extSepPos)? extSepPos : fullPath.length();
        
        // if there is no path separator start with position 0
        pathSepPos = (pathSepPos != -1)? pathSepPos + 1 : 0;
        
        return fullPath.substring(pathSepPos, extSepPos);
    }
    
    /**
     * Gets the filename without file extension using default file extension separator
     * {@link #DEFAULT_EXTENSION_SEPARATOR} and path separator {@link #DEFAULT_PATH_SEPARATOR}.
     *
     * @param fullPath The full path to the file.
     * @return The filename without file extension.
     * 
     * @see #getFilename(String, char, char)
     */
    public static String getFilename(String fullPath) {
        return getFilename(fullPath, DEFAULT_PATH_SEPARATOR, DEFAULT_EXTENSION_SEPARATOR);
    }
    
    /**
     * Gets the filename without file extension.
     *
     * @return The filename without file extension.
     */
    public String getFilename() {
        return getFilename(fullPath, pathSeparator, extensionSeparator);
    }
    
    /**
     * Gets the filename with the file extension.
     *
     * @param fullPath The full path to the file.
     * @param pathSeparator The path separator by which to parse the path.
     * @param extensionSeparator The extension separator by which to parse the file extension.
     * @return The filename with the file extension.
     * 
     * @see #getFullFilename(String)
     */
    public static String getFullFilename(String fullPath, char pathSeparator) {
        int pathSepPos = fullPath.lastIndexOf(pathSeparator);
        return (pathSepPos != -1)? fullPath.substring(pathSepPos + 1, fullPath.length()) : null;
    }
    
    /**
     * Gets the filename with the file extension using default path separator {@link #DEFAULT_PATH_SEPARATOR}.
     *
     * @param fullPath The full path to the file.
     * @return The filename with the file extension.
     * 
     * @see #getFullFilename(String, char)
     */
    public static String getFullFilename(String fullPath) {
        return getFullFilename(fullPath, DEFAULT_PATH_SEPARATOR);
    }
    
    /**
     * Gets the filename with the file extension.
     *
     * @return The filename with the file extension.
     */
    public String getFullFilename() {
        return getFullFilename(fullPath, pathSeparator);
    }
    
    /**
     * Gets the path to the file.
     *
     * @param fullPath The full path to the file.
     * @param pathSeparator The path separator by which to parse the path.
     * @return The path to the file.
     * 
     * @see #getPath(String)
     */
    public static String getPath(String fullPath, char pathSeparator) {
        int pathSepPos = fullPath.lastIndexOf(pathSeparator);
        return (pathSepPos != -1)? fullPath.substring(0, pathSepPos) : null;
    }
    
    /**
     * Gets the path to the file uses default path separator {@link #DEFAULT_PATH_SEPARATOR}.
     *
     * @param fullPath The full path to the file.
     * @return The path to the file.
     * 
     * @see #getPath(String, char)
     */
    public static String getPath(String fullPath) {
        return getPath(fullPath, DEFAULT_PATH_SEPARATOR);
    }

    /**
     * Gets the path of this file.
     *
     * @return The path to the file.
     */
    public String getPath() {
        return getPath(fullPath, pathSeparator);
    }
    
    /**
     * Checks if the file is sub-directory of another file.
     *
     * @param dirPath The full path of the directory against which to be test.
     * @param subDirPath The full path of the sub-directory to be tested.
     * @return True, if is sub-directory, otherwise false.
     */
    public static boolean isSubDirectory(String dirPath, String subDirPath) {
        try {
            File dir = new File(dirPath).getCanonicalFile();
            File subDir = new File(subDirPath).getCanonicalFile();
    
            File parentFile = subDir;
            
            // Tries to get from the nested location of the sub-directory
            // to the location of the directory against which is tested 
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
    
    /**
     * Checks if the file is sub-directory of this file.
     *
     * @param subDirPath The full path of the sub-directory to be tested
     * @return True, if is sub-directory, otherwise false.
     */
    public boolean isSubDirectory(String subDirPath) {
        return isSubDirectory(fullPath, subDirPath);
    }
    
    /**
     * Checks if is a filename valid.
     *
     * @param fullPath The full path to the file.
     * @return True, if is filename valid, otherwise false.
     */
    public static boolean isFilenameValid(String fullPath) {
        File f = new File(fullPath);
        try {
           f.getCanonicalPath();
           return true;
        } catch (IOException e) {
           return false;
        }
    }
    
    /**
     * Checks if is this filename valid.
     *
     * @return True, if is filename valid, otherwise false.
     */
    public boolean isFilenameValid() {
        File f = new File(fullPath);
        try {
           f.getCanonicalPath();
           return true;
        } catch (IOException e) {
           return false;
        }
    }
}

//End of the file
