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
import java.util.List;

import android.content.Context;
import android.hardware.Camera;
import android.hardware.Camera.Size;
import android.util.AttributeSet;
import android.util.Log;
import android.view.Display;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;

/**
 * Class of the surface view for viewing the camera preview.
 * Some parts are taken from the API SDK demo:
 * http://developer.android.com/resources/samples/ApiDemos/src/com/example/android/apis/graphics/CameraPreview.html
 * 
 * @version 1.0
 */
public class CameraPreview extends ViewGroup {
    
    public interface CameraPreviewCallback {
        public void previewStarted(Size previewSize);
    };
    
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
	
	/** Camera preview size */
	Size previewSize;
	
	/** Surface where preview is rendered */
	SurfaceView cameraSurfaceView;
	
	/** Preview callback. */
	private CameraPreviewCallback previewCallback;
	
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
        cameraSurfaceView = new SurfaceView(context);
        addView(cameraSurfaceView);

        camHolder = cameraSurfaceView.getHolder();
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
        cameraSurfaceView = new SurfaceView(context);
        addView(cameraSurfaceView);

        camHolder = cameraSurfaceView.getHolder();
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
        cameraSurfaceView = new SurfaceView(context);
        addView(cameraSurfaceView);

        camHolder = cameraSurfaceView.getHolder();
        camHolder.addCallback(surfaceCallback);    // adds surface callbacks
    }
    
    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        final int width = resolveSize(getSuggestedMinimumWidth(), widthMeasureSpec);
        final int height = resolveSize(getSuggestedMinimumHeight(), heightMeasureSpec);
        setMeasuredDimension(width, height);

        if (camera != null && previewSize == null) {
            previewSize = getOptimalPreviewSize(camera.getParameters().getSupportedPreviewSizes(), width, height);
        }
    }   
    
    @Override
    protected void onLayout(boolean changed, int l, int t, int r, int b) {
        if (getChildCount() > 0) {
            final View child = getChildAt(0);

            final int width = r - l;
            final int height = b - t;

            int previewWidth = width;
            int previewHeight = height;
            if (previewSize != null) {
                previewWidth = previewSize.width;
                previewHeight = previewSize.height;
            }

            if (width * previewHeight > height * previewWidth) {
                final int scaledChildWidth = previewWidth * height / previewHeight;
                child.layout((width - scaledChildWidth) / 2, 0,
                        (width + scaledChildWidth) / 2, height);
            } else {
                final int scaledChildHeight = previewHeight * width / previewWidth;
                child.layout(0, (height - scaledChildHeight) / 2,
                        width, (height + scaledChildHeight) / 2);
            }
        }
    }
    
    /**
     * Sets demanded preview size.
     * 
     * @param previewSize New preview size of camera.
     */
    public void setPreviewSize(Size previewSize) {
        this.previewSize = previewSize;
    }
    
    public Size getPreviewSize() {
        return previewSize;
    }

    /**
     * Returns optimal preview size calculated from the display size.
     * 
     * @param context Application context.
     * @param camera Opened camera.
     * @return Calculated optimal size.
     */
    public static Size getOptimalPreviewSize(Context context, Camera camera) {
        if (context == null) return null;
        
        List<Size> cameraSizes = null;
        if (camera != null) {
            cameraSizes = camera.getParameters().getSupportedPreviewSizes();
        }
        
        Display display = ((WindowManager) context.getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay();

        int width = display.getWidth();
        int height = display.getHeight();
        int rotation = display.getRotation();
        
        if (rotation == Surface.ROTATION_90 || rotation == Surface.ROTATION_270) {
            return getOptimalPreviewSize(cameraSizes, width, height);
        } else {
            return getOptimalPreviewSize(cameraSizes, height, width);
        }
    }

    /**
     * Calculates optimal size from the specified dimensions.
     * 
     * @param sizes Sizes from which should be taken optimal size.
     * @param w Suggested optimal width.
     * @param h Suggested optimal height.
     * @return Optimal size.
     */
    private static Size getOptimalPreviewSize(List<Size> sizes, int w, int h) {
        final double ASPECT_TOLERANCE = 0.1;
        double targetRatio = (double) w / h;
        if (sizes == null) return null;

        Size optimalSize = null;
        double minDiff = Double.MAX_VALUE;

        int targetHeight = h;

        // Try to find an size match aspect ratio and size
        for (Size size : sizes) {
            double ratio = (double) size.width / size.height;
            if (Math.abs(ratio - targetRatio) > ASPECT_TOLERANCE) continue;
            if (Math.abs(size.height - targetHeight) < minDiff) {
                optimalSize = size;
                minDiff = Math.abs(size.height - targetHeight);
            }
        }

        // Cannot find the one match the aspect ratio, ignore the requirement
        if (optimalSize == null) {
            minDiff = Double.MAX_VALUE;
            for (Size size : sizes) {
                if (Math.abs(size.height - targetHeight) < minDiff) {
                    optimalSize = size;
                    minDiff = Math.abs(size.height - targetHeight);
                }
            }
        }
        return optimalSize;
    }


	/**
	 * Starts previewing when is demanded and updates the previewing surface.
	 */
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

            // Try to set a new camera settings
            try {
                camera.setPreviewDisplay(camHolder);
                Camera.Parameters params = camera.getParameters();
                params.setPreviewSize(previewSize.width, previewSize.height);
                camera.setParameters(params);
                camera.startPreview();
                previewing = true;
                firePreviewStarted(params.getPreviewSize());
                requestLayout();
            } catch (IOException e) {
                Log.e(TAG, "Setting preview display failed: " + e);
            }
	    }
	    
	}
	
	/**
	 * Returns view where camera preview is actually rendered.
	 * 
	 * @return View where camera preview is actually rendered.
	 */
	public View getPreviewView() {
	    return getChildAt(0);
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
        requestLayout();
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
     * Fires preview started callback.
     */
    private void firePreviewStarted(Size previewSize) {
        if (previewCallback != null) {
            previewCallback.previewStarted(previewSize);
        }
    }
    
    /**
     * Sets preview callback.
     */
    public void setPreviewCallback(CameraPreviewCallback previewCallback) {
        this.previewCallback = previewCallback;
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