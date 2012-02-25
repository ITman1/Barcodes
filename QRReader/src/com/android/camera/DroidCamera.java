package com.android.camera;

import java.util.List;

import android.graphics.ImageFormat;
import android.hardware.Camera;
import android.hardware.Camera.Parameters;
import android.hardware.Camera.PreviewCallback;
import android.hardware.Camera.Size;
import android.util.Log;

public class DroidCamera {
    private static final String TAG = "com.android.camera::DroidCamera";
    
    private static final Object       sLock               = new Object(); 
    private static DroidCamera        instance;
    private static byte[]             callbackBuffer;
    private static byte[]             callbackBuffer_2;
    
    private Camera                    camera;
	private CameraPreview             camPreviewSurface;

    private DroidCamera(){}
    
    public Camera getCamera() {
        Log.i(TAG, "getCamera");
    	return camera;
    }
   
    public final CameraPreview getPreviewSurface() {
        Log.i(TAG, "getPreviewSurface");
        return camPreviewSurface;
    }
    
    public static DroidCamera open() {
        synchronized (sLock) { 
            Log.i(TAG, "open");
            Camera cam;
            
            if (instance == null) {
                try {
                    instance = ((cam = Camera.open()) != null)? new DroidCamera() : null;
                    if (instance != null) instance.camera = cam;
                } catch (RuntimeException e) {
                    e.printStackTrace();
                }
            }
            return instance;
        }
    }
    
    public final void release() {
        synchronized (sLock) { 
            Log.i(TAG, "release");
            if (camPreviewSurface != null) {
                camPreviewSurface.stopPreviewing();
            }
            camera.release();
            camera = null;
            instance = null;
        }
    }
    
    public final void startPreviewing(CameraPreview surface) {
        camPreviewSurface = surface;
        if (camPreviewSurface != null) {
            camPreviewSurface.setCamera(camera);
            camPreviewSurface.startPreviewing();
        }
        
    }
    
    public final void stopPreviewing() {
        if (camPreviewSurface != null) {
            camPreviewSurface.stopPreviewing();
        }
        camPreviewSurface = null;
    }

    public final void setPreviewCallback(PreviewCallback cb) {
        int bitsPrePixel = ImageFormat.getBitsPerPixel(camera.getParameters().getPreviewFormat());
        Size size = camera.getParameters().getPreviewSize();
        int bufferSize = (size.width * size.height * bitsPrePixel) / 8 + 1;
        callbackBuffer = new byte[bufferSize];
        callbackBuffer_2 = new byte[bufferSize];
        
        camera.setPreviewCallbackWithBuffer(null);
        camera.addCallbackBuffer(callbackBuffer);
        camera.addCallbackBuffer(callbackBuffer_2);
        camera.setPreviewCallbackWithBuffer(cb);
    }
    
    public final boolean autoFocusSupport() {
        Parameters camParams = camera.getParameters();
        List<String> suppFocuses = camParams.getSupportedFocusModes();
        
        if (suppFocuses != null && suppFocuses.contains(Parameters.FOCUS_MODE_AUTO)) {
            return true;
        }
        
        return false;
    }
} 