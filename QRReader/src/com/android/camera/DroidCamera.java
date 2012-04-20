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

import com.android.camera.CameraPreview.CameraPreviewCallback;

import android.content.Context;
import android.graphics.ImageFormat;
import android.graphics.Rect;
import android.hardware.Camera;
import android.hardware.Camera.Parameters;
import android.hardware.Camera.PreviewCallback;
import android.hardware.Camera.Size;
import android.util.Log;
import android.view.View;

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
    
    /** The third buffer for the preview callback. For better FPS. */
    private static byte[]             callbackBuffer_3;
    
    /** The instance of the opened camera. */
    private Camera                    camera;
	
	/** The preview surface of the camera. */
	private CameraPreview             camPreviewSurface;
	
	/** Preview callback */
	private PreviewCallback cb;
	
	private Context context;
	
	/** Callback of the CameraPreview class */
	private CameraPreviewCallback previewCameraSurfaceCallback = new CameraPreviewCallback() {

        @Override
        public void previewStarted(Size previewSize) {
            setPreviewCallback(previewSize);
        }
	    
	};

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
    public static DroidCamera open(Context context) {
        synchronized (sLock) { 
            Log.i(TAG, "open");
            Camera cam;
            
            if (instance == null) {
                try {
                    instance = ((cam = Camera.open()) != null)? new DroidCamera() : null;
                    if (instance != null) instance.camera = cam;
                    instance.context = context;
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
        startPreviewing(surface, null);
    }
    
    public void startPreviewing(CameraPreview surface, Size previewSize) {
        camPreviewSurface = surface;
        if (camPreviewSurface != null) {
            camPreviewSurface.setCamera(camera);
            camPreviewSurface.setPreviewCallback(previewCameraSurfaceCallback);
            camPreviewSurface.setPreviewSize(previewSize);
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
        this.cb = cb;
        if (camera != null && camPreviewSurface != null && camPreviewSurface.isPreviewing()) {
            setPreviewCallback(camera.getParameters().getPreviewSize());
        }
    }
    
    /**
     * Sets the preview callback that will be called when a new frame is 
     * available to be rendered. 
     *
     * @param previewSize The preview sizeThe new preview callback.
     */
    private void setPreviewCallback(Size previewSize) {
        // Counting the size for the store buffers
        int bitsPrePixel = ImageFormat.getBitsPerPixel(camera.getParameters().getPreviewFormat());
        int bufferSize = (previewSize.width * previewSize.height * bitsPrePixel) / 8 + 1;
        
        // Allocating the store buffers
        callbackBuffer = new byte[bufferSize];
        callbackBuffer_2 = new byte[bufferSize];
        callbackBuffer_3 = new byte[bufferSize];
        
        // Clearing the queue with the preview buffers
        camera.setPreviewCallbackWithBuffer(null);     
        
        // Adding a new buffers to the queue and setting a new preview callback
        camera.addCallbackBuffer(callbackBuffer);
        camera.addCallbackBuffer(callbackBuffer_2);
        camera.addCallbackBuffer(callbackBuffer_3);
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
     * Retrieves the rectangle of the camera preview where camera preview is rendered.
     *
     * @return Size of the camera preview surface whether is active.
     */
    public Rect previewRect() {
        if (camPreviewSurface != null && camPreviewSurface.isPreviewing()) {
            View v = camPreviewSurface.getPreviewView();
            return new Rect(v.getLeft(), v.getTop(), v.getRight(), v.getBottom());
        }
        
        return null;
    }
    
    /**
     * Returns suggestion of the optimal size which will be used if there is not any specified size.
     *
     * @return Suggestion of the camera preview size which is calculated from the size of the display.
     */
    public Size getOptimalPreviewSize() {
        return CameraPreview.getOptimalPreviewSize(context, camera);
    }
    
    
    /**
     * Retrieves the sizes of the camera preview.
     *
     * @return Size of the camera preview whether is active.
     */
    public Size previewSize() {
        if (camera != null) {
            return camera.getParameters().getPreviewSize();
        }
        
        return null;
    }
} 

// End of the file