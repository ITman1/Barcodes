///////////////////////////////////////////////////////////////////////////////
// Project:    QR Reader for Android
// Package:    com.filesystem
// File:       Operations.java
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Contains the class which implements some additional operations 
//             above files which are not in the standard java.
///////////////////////////////////////////////////////////////////////////////

package com.filesystem;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.channels.FileChannel;

/**
 * The Class Operations implements some additional feature operations above files
 * which are not available in the standard java.
 * 
 * @version 1.0
 */
public class Operations {
    
    /** The exception text which is thrown when file is too large for reading. */
    private static String EXCEPTION_FILE_LARGE = "File is too large for the integer!";
    
    /** The exception text which is thrown when file has not been read whole. */
    private static String EXCEPTION_FILE_NOT_READ = "File has not been read whole!";
    
    /**
     * Removes the files from the directory.
     *
     * @param directory The directory file.
     * @return True, if all files has been removed from the directory, otherwise false.
     */
    public static boolean removeFiles(File directory) {
        if (directory.isDirectory()) {
            File[] content = directory.listFiles(); // Getting list of all files to remove
            
            if (content != null) {
                for (File file : content) {
                    if (file.isDirectory()) {       // Is directory
                        if (!removeDirectory(file)) return false;
                    } else {                        // Is regular file
                        if (!file.delete()) return false;
                    }
                }
            }
            
            return true;
        }
        return false;
    }
    
    /**
     * Removes the directory from the file system.
     *
     * @param directory The directory file.
     * @return True, if directory has been removed, otherwise false.
     */
    public static boolean removeDirectory(File directory) {
        if (directory.isDirectory()) {
            if (!removeFiles(directory)) return false;
            
            return directory.delete();  // Finally remove also this directory
        }
        return false;
    }
    
    /**
     * Copies file from the location to the another one.
     *
     * @param which The file which should be copied.
     * @param where The file where should be copied.
     * @throws IOException Signals that an I/O exception has occurred.
     */
    public static void copyFile(File which, File where) throws IOException {
    
        FileChannel fromChannel = null;
        FileChannel toChannel = null;
        try {
            fromChannel = new FileInputStream(which).getChannel();
            toChannel = new FileOutputStream(where).getChannel();
    
            long sourceSize = fromChannel.size();
            // Too large file
            if (sourceSize > Integer.MAX_VALUE) {   
                throw new IOException(EXCEPTION_FILE_LARGE);
            }
            
            // Just copy
            long alreadyCopied = 0;
            while((alreadyCopied += toChannel.transferFrom(fromChannel, 0, sourceSize - alreadyCopied)) < sourceSize);
        } finally {
            if(fromChannel != null) fromChannel.close();
            if(toChannel != null) toChannel.close();
        }
    }
    
    /**
     * Reads file and returns the file as the byte array.
     *
     * @param file The file to be read.
     * @return The array of the read bytes.
     * @throws IOException Signals that an I/O exception has occurred.
     */
    public static byte[] readFile(File file) throws IOException {   
        long fileLength = file.length();
    
        // Too large file
        if (fileLength > Integer.MAX_VALUE) {   
            throw new IOException(EXCEPTION_FILE_LARGE);
        }
    
        byte[] readBytes = new byte[(int)fileLength];
    
        int alreadyRead = 0;
        int currRead = 0;
        
        InputStream inputStream = null;
        try {
            // Just read
            inputStream = new FileInputStream(file);
            while (alreadyRead < readBytes.length && (currRead = inputStream.read(readBytes, 
                    alreadyRead, readBytes.length - alreadyRead)) >= 0) {
                alreadyRead += currRead;
            }
        
            // Not read the whole file
            if (alreadyRead < readBytes.length) { 
                throw new IOException(EXCEPTION_FILE_NOT_READ);
            }
        } finally {
            if(inputStream != null) inputStream.close();
        }
        return readBytes;
    }
}

//End of the file