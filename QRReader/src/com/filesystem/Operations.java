package com.filesystem;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.channels.FileChannel;

public class Operations {
    
    private static String EXCEPTION_FILE_LARGE = "File is too large for the integer!";
    private static String EXCEPTION_FILE_NOT_READ = "File has not been read whole!";
    
    public static boolean removeFiles(File directory) {
        if (directory.isDirectory()) {
            File[] content = directory.listFiles();
            if (content != null) {
                for (File file : content) {
                    if (file.isDirectory()) {
                        if (!removeDirectory(file)) return false;
                    }
                    if (!file.delete()) return false;
                }
            }
            return true;
        }
        return false;
    }
    
    public static boolean removeDirectory(File directory) {
        if (directory.isDirectory()) {
            File[] content = directory.listFiles();
            if (content != null) {
                for (File file : content) {
                    if (file.isDirectory()) {
                        if (!removeDirectory(file)) return false;
                    }
                    if (!file.delete()) return false;
                }
            }
            
            return directory.delete();
        }
        return false;
    }
    
    public static void copyFile(File which, File where) throws IOException {
    
        FileChannel fromChannel = null;
        FileChannel toChannel = null;
        try {
            fromChannel = new FileInputStream(which).getChannel();
            toChannel = new FileOutputStream(where).getChannel();
    
            long sourceSize = fromChannel.size();
            long alreadyCopied = 0;
            while((alreadyCopied += toChannel.transferFrom(fromChannel, 0, sourceSize - alreadyCopied)) < sourceSize);
        } finally {
            if(fromChannel != null) fromChannel.close();
            if(toChannel != null) toChannel.close();
        }
    }
    
    public static byte[] readFile(File file) throws IOException {   
        long fileLength = file.length();
    
        if (fileLength > Integer.MAX_VALUE) {
            throw new IOException(EXCEPTION_FILE_LARGE);
        }
    
        byte[] readBytes = new byte[(int)fileLength];
    
        int alreadyRead = 0;
        int currRead = 0;
        
        InputStream inputStream = null;
        try {
            inputStream = new FileInputStream(file);
            while (alreadyRead < readBytes.length && (currRead = inputStream.read(readBytes, 
                    alreadyRead, readBytes.length - alreadyRead)) >= 0) {
                alreadyRead += currRead;
            }
        
            if (alreadyRead < readBytes.length) {
                throw new IOException(EXCEPTION_FILE_NOT_READ);
            }
        } finally {
            if(inputStream != null) inputStream.close();
        }
        return readBytes;
    }
}
