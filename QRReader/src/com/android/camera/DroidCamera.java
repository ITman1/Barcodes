///////////////////////////////////////////////////////////////////////////////
// Project:    QR Reader for Android
// Package:    com.android.camera
// File:       DroidCamera.java
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Contains the class which adds functional layer over the API 
//             final class android.hardware.Camera.
///////////////////////////////////////////////////////////////////////////////

package com.android.camera;

import java.util.List;

import android.graphics.ImageFormat;
import android.graphics.Rect;
import android.hardware.Camera;
import android.hardware.Camera.Parameters;
import android.hardware.Camera.PreviewCallback;
import android.hardware.Camera.Size;
import android.util.Log;

/**
 * Class which extends android API {@link android.hardware.Camera Camera}.
 * Adds especially previewing support on the custom surface.
 * 
 * @version 1.0
 * @see android.hardware.Camera Camera
 */
// TODO: It was not bad to add the support for opening more cameras on some devices.
final public class DroidCamera {
    
    /** The Constant Debug TAG. */
    private static final String TAG = "com.android.camera::DroidCamera";
    
    /** The Constant lock which ensures the unique opening/releasing the camera */
    private static final Object       sLock               = new Object(); 
    
    /** The instance of the opened camera. */
    private static DroidCamera        instance;
    
    /** The buffer for the preview callback. */
    private static byte[]             callbackBuffer;
    
    /** The second buffer for the preview callback. For better FPS. */
    private static byte[]             callbackBuffer_2;
    
    /** The instance of the opened camera. */
    private Camera                    camera;
	
	/** The preview surface of the camera. */
	private CameraPreview             camPreviewSurface;

    /**
     * Instantiates a new camera.
     */
    private DroidCamera(){}
    
    /**
     * Gets the instance of the android camera.
     *
     * @return Instance of the android camera.
     * @see android.hardware.Camera Camera
     */
    public Camera getCamera() {
        Log.i(TAG, "getCamera");
    	return camera;
    }
   
    /**
     * Gets the preview surface for the camera.
     *
     * @return the preview surface
     */
    public CameraPreview getPreviewSurface() {
        Log.i(TAG, "getPreviewSurface");
        return camPreviewSurface;
    }
    
    /**
     * Opens a new camera and returns its instance.
     *
     * @return The instance of the newly opened camera. Returns null whether 
     * opening failed. If already another camera is opened returns instance 
     * of the opened camera.
     */
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
                    instance = null;
                }
            }
            return instance;
        }
    }
    
    /**
     * Releases the current opened camera. After this calling the camera 
     * object is not valid for other use.
     */
    public void release() {
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
    
    /**
     * Starts previewing on the specified surface.
     *
     * @param surface The surface where will be rendered camera preview.
     */
    public void startPreviewing(CameraPreview surface) {
        camPreviewSurface = surface;
        if (camPreviewSurface != null) {
            camPreviewSurface.setCamera(camera);
            camPreviewSurface.startPreviewing();
        }
        
    }
    
    /**
     * Stops the camera previewing.
     */
    public void stopPreviewing() {
        if (camPreviewSurface != null) {
            camPreviewSurface.stopPreviewing();
        }
        camPreviewSurface = null;
    }

    /**
     * Sets the preview callback that will be called when a new frame is 
     * available to be rendered. 
     *
     * @param cb The new preview callback.
     */
    public void setPreviewCallback(PreviewCallback cb) {
        // Counting the size for the store buffers
        int bitsPrePixel = ImageFormat.getBitsPerPixel(camera.getParameters().getPreviewFormat());
        Size size = camera.getParameters().getPreviewSize();
        int bufferSize = (size.width * size.height * bitsPrePixel) / 8 + 1;
        
        // Allocating the store buffers
        callbackBuffer = new byte[bufferSize];
        callbackBuffer_2 = new byte[bufferSize];
        
        // Clearing the queue with the preview buffers
        camera.setPreviewCallbackWithBuffer(null);     
        
        // Adding a new buffers to the queue and setting a new preview callback
        camera.addCallbackBuffer(callbackBuffer);
        camera.addCallbackBuffer(callbackBuffer_2);
        camera.setPreviewCallbackWithBuffer(cb);
    }
    
    /**
     * Retrieves whether the auto-focus support is available.
     *
     * @return True, if auto-focus ia available, otherwise false.
     */
    public boolean autoFocusSupport() {
        Parameters camParams = camera.getParameters();
        List<String> suppFocuses = camParams.getSupportedFocusModes();
        
        if (suppFocuses != null && suppFocuses.contains(Parameters.FOCUS_MODE_AUTO)) {
            return true;
        }
        
        return false;
    }
    
    /**
     * Retrieves the sizes of the camera preview.
     *
     * @return Size of the camera preview whether is active.
     */
    public Rect previewRect() {
        if (camPreviewSurface != null && camPreviewSurface.isPreviewing()) {
            return new Rect(0, 0, camPreviewSurface.getWidth(), camPreviewSurface.getHeight());
        }
        
        return null;
    }
} 

// End of the file