package com.filesystem;

import java.io.File;

public class Operations {
    
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
}
