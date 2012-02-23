package com.android.camera;

import java.io.IOException;

import android.content.Context;
import android.hardware.Camera;
import android.util.AttributeSet;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class CameraPreview extends SurfaceView implements SurfaceHolder.Callback {
    private static final String TAG = "com.android.camera::CameraPreview";
    
	private SurfaceHolder      camHolder;
	private Camera             camera;
	
    boolean                    previewRequest      = false;
    boolean                    surfaceCreated      = false;
    boolean                    previewing          = false;
	boolean                    customCamera        = false;
	
    public CameraPreview(Context context) {
        super(context);
        Log.i(TAG, "CameraPreview");
        camHolder = getHolder();
        camHolder.addCallback(this);
    }
	
	public CameraPreview(Context context, AttributeSet attrs) {
        super(context, attrs);
        Log.i(TAG, "CameraPreview");
        camHolder = getHolder();
        camHolder.addCallback(this);
    }
	
    public CameraPreview(Context context, Camera camera) {
        super(context);
        Log.i(TAG, "CameraPreview");
        customCamera = true;
        this.camera = camera;
        camHolder = getHolder();
        camHolder.addCallback(this);
    }
	   
	public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        Log.i(TAG, "surfaceChanged");
        updatePreviewing();
	}

	public void surfaceCreated(SurfaceHolder holder) {
        Log.i(TAG, "surfaceCreated");
        surfaceCreated = true;
	}

	public void surfaceDestroyed(SurfaceHolder holder) {
        Log.i(TAG, "surfaceDestroyed");
        surfaceCreated = false;
        removeCamera();
	}
	
	private void updatePreviewing() {
        Log.i(TAG, "updatePreviewing");
	    if (surfaceCreated && previewRequest) {
	        if (camera == null) {
	            if ((camera = Camera.open()) != null) {
	                customCamera = false;
	            } else {
	                Log.e(TAG, "Unable to open camera!");
	                previewing = false;
	                return;
	            }
	        }
	    
            if (previewing) {
                camera.stopPreview();
                previewing = false;
            }    

            try {
                camera.setPreviewDisplay(camHolder);
                camera.startPreview();
                previewing = true;
            } catch (IOException e) {
                Log.e(TAG, "Setting preview display failed: " + e);
            }
	    }

	}
	
	public void removeCamera() {
	    if (!customCamera && camera != null) {
	        camera.release();  
	    }
        previewing = false;
        camera = null;
	}
	
	public Camera getCamera() {
        Log.i(TAG, "getCamera");
        return camera;
    }
	
    public void setCamera(Camera camera) {
        Log.i(TAG, "setCamera");
        
        removeCamera();
        customCamera = true;
        this.camera = camera;
    }
	
	public void startPreviewing() {
        Log.i(TAG, "startPreviewing");
        previewRequest = true;
        
	    // deprecated, but required on Android 3.X.X
        camHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
        updatePreviewing();
	}
	
	public void stopPreviewing() {
	    Log.i(TAG, "stopPreviewing");
	    previewRequest = false;
	    
	    if (previewing && camera != null) {
	        camera.stopPreview();
	        removeCamera();
	    }
	}
    
}