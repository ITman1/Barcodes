///////////////////////////////////////////////////////////////////////////////
// Project:    QR Reader for Android
// Package:    com.android.camera
// File:       AdapterRegistry.java
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Contains the class of the surface view where is rendered
//             the camera preview.
///////////////////////////////////////////////////////////////////////////////

package com.android.camera;

import java.io.IOException;

import android.content.Context;
import android.hardware.Camera;
import android.util.AttributeSet;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

/**
 * Class of the surface view for viewing the camera preview.
 * 
 * @version 1.0
 */
public class CameraPreview extends SurfaceView {
    
    /** The Constant Debug TAG. */
    private static final String TAG = "com.android.camera::CameraPreview";
    
	/** The holder of the surface for previewing. */
	private SurfaceHolder      camHolder;
	
	/** The camera. */
	private Camera             camera;
	
    /** The preview request. True when previewing is demanded 
     *  by calling {@link #startPreviewing()} */
	private boolean            previewRequest      = false;
    
    /** The surface created. True when surface has been created 
     *  {@link #surfaceCreated(SurfaceHolder)}. And false when surface has 
     *  been destroyed {@link #surfaceDestroyed(SurfaceHolder)}*/
	private boolean            surfaceCreated      = false;
    
    /** The previewing is running. This can happen only when 
     *  {@link #surfaceCreated} is true an {@link #previewRequest} is true.*/
	private boolean            previewing          = false;
	
	/** The custom camera. True when has been passed for previewing already 
	 *  opened camera which will be used for previewing. Otherwise would 
	 *  be created (opened) camera for previewing locally. */
	private boolean            customCamera        = false;
	
	/** The surface callback. Catches events of the surface life-cycle. */
	private SurfaceHolder.Callback surfaceCallback = new SurfaceHolder.Callback() {
	    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
            Log.i(TAG, "SurfaceHolder.Callback::surfaceChanged");
            updatePreviewing();
        }
    
        public void surfaceCreated(SurfaceHolder holder) {
            Log.i(TAG, "SurfaceHolder.Callback::surfaceCreated");
            surfaceCreated = true;
        }
    
        public void surfaceDestroyed(SurfaceHolder holder) {
            Log.i(TAG, "SurfaceHolder.Callback::surfaceDestroyed");
            surfaceCreated = false;
            removeCamera();
        }
	};
	
    /**
     * Instantiates a new camera preview.
     *
     * @param context The context.
     */
    public CameraPreview(Context context) {
        super(context);
        Log.i(TAG, "CameraPreview");
        camHolder = getHolder();
        camHolder.addCallback(surfaceCallback);    // adds surface callbacks
    }
	
	/**
	 * Instantiates a new camera preview.
	 *
	 * @param context The context.
	 * @param attrs The attributes read from the XML document.
	 */
	public CameraPreview(Context context, AttributeSet attrs) {
        super(context, attrs);
        Log.i(TAG, "CameraPreview");
        camHolder = getHolder();
        camHolder.addCallback(surfaceCallback);    // adds surface callbacks
    }
	
    /**
     * Instantiates a new camera preview.
     *
     * @param context The context.
     * @param camera The opened camera.
     */
    public CameraPreview(Context context, Camera camera) {
        super(context);
        Log.i(TAG, "CameraPreview");
        customCamera = true;
        this.camera = camera;
        camHolder = getHolder();
        camHolder.addCallback(surfaceCallback);    // adds surface callbacks
    }
	   

	/**
	 * Starts previewing when is demanded and updates the previewing surface.
	 */
    // TODO: Change surface sizes in the dependence of the supported sizes by the camera.
	private void updatePreviewing() {
        Log.i(TAG, "updatePreviewing");
        
	    if (surfaceCreated && previewRequest) {         // There is the previewing request
	        
	        // Open camera if is not yet
	        if (camera == null) {
	            if ((camera = Camera.open()) != null) {
	                customCamera = false;
	            } else {
	                Log.e(TAG, "Unable to open camera!");
	                previewing = false;
	                return;
	            }
	        }
	    
	        // Before updating preview surface stop previewing
            if (previewing) {
                camera.stopPreview();
                previewing = false;
            }    

            // Try to set a new camera settings (TODO: supported camera sizes parameters.getSupported...)
            try {
                camera.setPreviewDisplay(camHolder);
                camera.startPreview();
                previewing = true;
            } catch (IOException e) {
                Log.e(TAG, "Setting preview display failed: " + e);
            }
	    }

	}
	
	/**
	 * Releases the camera if has been opened locally.
	 */
	public void removeCamera() {
	    if (!customCamera && camera != null) {
	        camera.release();  
	    }
        previewing = false;
	}
	
	/**
	 * Gets the camera instance.
	 *
	 * @return the camera
	 */
	public Camera getCamera() {
        Log.i(TAG, "getCamera");
        return camera;
    }
	
    /**
     * Sets the new opened camera.
     *
     * @param camera the new camera
     */
    public void setCamera(Camera camera) {
        Log.i(TAG, "setCamera");
        
        removeCamera();
        customCamera = true;
        this.camera = camera;
    }
	
	/**
	 * Request to start previewing.
	 */
	public void startPreviewing() {
        Log.i(TAG, "startPreviewing");
        previewRequest = true;
        
	    // deprecated, but required on Android 3.X.X
        camHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
        updatePreviewing();
	}
	
	/**
     * Request to stop previewing.
	 */
	public void stopPreviewing() {
	    Log.i(TAG, "stopPreviewing");
	    previewRequest = false;
	    
	    if (previewing && camera != null) {
	        camera.stopPreview();
	        removeCamera();
	    }
	}
	
	/**
     * Returns whether preview is active.
     * 
     * @return True if camera preview is active, otherwise false.
     */
    public boolean isPreviewing() {
        return previewing;
    }
}

// End of the file