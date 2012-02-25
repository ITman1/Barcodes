package com.android.qrreader;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.ActivityInfo;
import android.content.res.Resources;
import android.graphics.PixelFormat;
import android.hardware.Camera;
import android.hardware.Camera.AutoFocusCallback;
import android.hardware.Camera.Parameters;
import android.hardware.Camera.PictureCallback;
import android.hardware.Camera.PreviewCallback;
import android.os.Bundle;
import android.os.Environment;
import android.preference.PreferenceManager;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup.LayoutParams;
import android.view.Window;
import android.view.WindowManager;
import android.view.View.OnClickListener;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.android.camera.CameraPreview;
import com.android.camera.DroidCamera;
import com.android.listviews.FileListActivity;
import com.android.qrreader.R;

public class QrReaderActivity extends Activity {
    private static final String TAG = "com.android.qrreader::QrReaderActivity";
    
    public static final String SD_CARD_DIR                  = "/sdcard";
    private static final String JPEG_FILE_SUFFIX            = ".jpg";
    private static final String RAW_FILE_SUFFIX             = ".raw";
    private static final String DEFAULT_IMAGE_PREFIX        = "IMG_";
    private static final String DEFAULT_IMAGE_FORMAT        = "JPEG";
    private static final String FPS_STR                     = "FPS: ";

    private RelativeLayout     cameraPreviewLayout;
	private CameraPreview      cameraPreview;
	private LayoutInflater     controlInflater;
	
    private TextView           fps_TextView;
    private ImageView          centerMark;
    private ImageButton        read_btn;
    private ProgressBar        statusProgress_ProgressBar; 
    private LinearLayout       status_LinearLayout;
    private TextView           statusText_TextView;
    
    private DroidCamera        droidCamera;
    private boolean            takingPicture;
    
    private long               lastTimeFPS                  = System.currentTimeMillis();
    private long               lastTimeSnapshot             = System.currentTimeMillis();
       
    private OnClickListener read_btn_OnClick = new OnClickListener() {
        public void onClick(View v) {
            Log.i(QrReaderActivity.TAG, "OnClickListener::read_btn_OnClick");

        	if (droidCamera != null && !takingPicture) {
        	    read_btn.setEnabled(false);
        	    takingPicture = true;
        	    status_LinearLayout.setVisibility(LinearLayout.VISIBLE);
                statusText_TextView.setText(R.string.QRReaderActivity_StatusText_Capturing);
                droidCamera.getCamera().setPreviewCallback(null);
                
                SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(getBaseContext());
                Boolean autoFocus = prefs.getBoolean("Preferences_Camera_AutoFocus", false);
                
                if (autoFocus && droidCamera.autoFocusSupport()) {
                    droidCamera.getCamera().autoFocus(autoFocusCallback);
                } else {
                    takePicture();
                }
        	}
        }
    };
    
    private AutoFocusCallback autoFocusCallback = new AutoFocusCallback() {
        @Override
        public void onAutoFocus(boolean success, Camera camera) {
            takePicture();
        }
    };
    
    private PreviewCallback droidCamera_PreviewCallback = new PreviewCallback() {
        public void onPreviewFrame(byte[] data, Camera camera) {
            Log.i(QrReaderActivity.TAG, "PreviewCallback::onPreviewFrame");
            fps_TextView.setText(FPS_STR + (long)(1 / (double)((System.currentTimeMillis()
                    - lastTimeFPS) / (double)1000)));
            lastTimeFPS = System.currentTimeMillis();
            camera.addCallbackBuffer(data);
        }
    };
    
    private PictureCallback takePictureCallback = new PictureCallback () {
        public void onPictureTaken(byte[] data, Camera camera) {
            Log.i(TAG, "onPictureTaken");
            statusText_TextView.setText(R.string.QRReaderActivity_StatusText_Saving);
            long currTimeSnapshot = System.currentTimeMillis();
            if (currTimeSnapshot - lastTimeSnapshot < 1000 && currTimeSnapshot > lastTimeSnapshot) {
                android.os.SystemClock.sleep(currTimeSnapshot - lastTimeSnapshot);
            }
            
            lastTimeSnapshot = System.currentTimeMillis();
            
            Resources res = getResources();
            SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(getBaseContext());
            String saveMethod = prefs.getString("Preferences_Images_SaveMethod", "autoSave");
            if (data != null && !saveMethod.equals("notSave")) {
                File pictureFile;
                
                if (saveMethod.equals("autoSave")) {
                    pictureFile = getImageFile();
                } else {
                    pictureFile = null; //TODO TODO TODO
                }
                if (pictureFile != null) { 
                    try {
                        FileOutputStream fos = new FileOutputStream(pictureFile);
                        fos.write(data);
                        fos.close();
                    } catch (FileNotFoundException e) {
                        Log.e(TAG, "File not found: " + e.getMessage());
                        showErrorAlert(String.format(res.getString(R.string.
                                Errors_Storage_OpenWrite), res.getString(R.string.str_image)));
                    } catch (IOException e) {
                        Log.e(TAG, "Error accessing file: " + e.getMessage());
                        showErrorAlert(String.format(res.getString(R.string.
                                Errors_Storage_Save), res.getString(R.string.str_image)));
                    }
                }
            } else {
                showErrorAlert(R.string.Errors_Camera_PictureCallback);
            }

            if (droidCamera != null) {
                cameraPreviewLayout.setVisibility(CameraPreview.VISIBLE);
                
                statusText_TextView.setText(R.string.QRReaderActivity_StatusText_Processing);
                //readQRCode();
                
                droidCamera.startPreviewing(cameraPreview);
                droidCamera.setPreviewCallback(droidCamera_PreviewCallback);
                status_LinearLayout.setVisibility(LinearLayout.INVISIBLE);
                statusText_TextView.setText("");
                read_btn.setEnabled(true);
                takingPicture = false;
            }
            
        }
    };
    
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
    	Log.i(TAG, "onCreate");
        super.onCreate(savedInstanceState);
        
        // Setting window features and flags for the making available the whole display
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
        getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN);
        
        PreferenceManager.setDefaultValues(this, R.xml.preferences, false);
               
        // Setting main window view as camera preview from the layouts
        setContentView(R.layout.camera_preview);
        getWindow().setFormat(PixelFormat.UNKNOWN);

        // Getting view surface for the camera
        cameraPreview = (CameraPreview)findViewById(R.id.cameraPreview);
        cameraPreviewLayout = (RelativeLayout)findViewById(R.id.cameraPreviewLayout);
        
        // Adding camera controls layout over the camera preview view
        controlInflater = LayoutInflater.from(getBaseContext());
        View cameraControls = controlInflater.inflate(R.layout.camera_controls, null);
        this.addContentView(cameraControls, new LayoutParams(LayoutParams.FILL_PARENT, 
                LayoutParams.FILL_PARENT));
       
        // Getting handlers for other controls on the layout
        fps_TextView = (TextView) findViewById(R.id.fps_TextView);
        centerMark = (ImageView) findViewById(R.id.centerMark);
        read_btn = (ImageButton) findViewById(R.id.read_btn);
        statusProgress_ProgressBar = (ProgressBar) findViewById(R.id.currSnapshot_ProgressBar);
        status_LinearLayout = (LinearLayout) findViewById(R.id.status_LinearLayout);
        statusText_TextView = (TextView) findViewById(R.id.statusText_TextView);
        
        read_btn.setOnClickListener(read_btn_OnClick);
    }
  
    @Override
    protected void onPause() {
        super.onPause();
        Log.i(TAG, "onPause");
        stopDroidCamera();
    }

    @Override
    protected void onResume() {
        super.onResume();
        Log.i(TAG, "onResume");
        startDroidCamera();
        applySettings();
    }
    
    @Override
    protected void onDestroy() {
        super.onDestroy();
        Log.i(TAG, "onDestroy");
        stopDroidCamera();
    }
    
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        Log.i(TAG, "onCreateOptionsMenu");
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.camera_options_menu, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        Log.i(TAG, "onOptionsItemSelected");
        
        Intent intent;
        String cwd;
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(getBaseContext()); 
        
        switch (item.getItemId()) {
            case R.id.settings:
                startActivity(new Intent(this, QrReaderSettingsActivity.class));
                return true;
            case R.id.recent_qrcodes:
                intent = new Intent(this, QrReaderBrowseImagesActivity.class);
                cwd = prefs.getString("Preferences_QRCodes_FilePath", SD_CARD_DIR);
                intent.putExtra(FileListActivity.BASE_DIR, cwd);
                startActivity(intent);
                return true;
            case R.id.recent_images:
                intent = new Intent(this, QrReaderBrowseImagesActivity.class);
                cwd = prefs.getString("Preferences_Images_FilePath", SD_CARD_DIR);
                intent.putExtra(FileListActivity.BASE_DIR, cwd);
                startActivity(intent);
                return true;
            default:
                return super.onOptionsItemSelected(item);
        }
    }
    
    private void takePicture() {
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(getBaseContext());
        Resources res = getResources();
        String prefFormat = prefs.getString("Preferences_Images_Format", DEFAULT_IMAGE_FORMAT);
        
        cameraPreviewLayout.setVisibility(CameraPreview.INVISIBLE);
        if (prefFormat.equals(res.getString(R.string.fileformat_jpeg))) {
            droidCamera.getCamera().takePicture(null, null, takePictureCallback);
        } else if (prefFormat.equals(res.getString(R.string.fileformat_raw))) {
            droidCamera.getCamera().takePicture(null, takePictureCallback, null);
        }
    }
    
    private void startDroidCamera() {
        Log.i(TAG, "startDroidCamera");
        if ((droidCamera = DroidCamera.open()) == null) {
            read_btn.setEnabled(false);
            Log.e(TAG, "Failed to open DroidCamera!");
            showErrorAlert(R.string.Errors_Camera_Failure);
            return;
        } 
        
        droidCamera.startPreviewing(cameraPreview);
        droidCamera.setPreviewCallback(droidCamera_PreviewCallback);
    }
    
    private void stopDroidCamera() {
        Log.i(TAG, "stopDroidCamera");
        if (droidCamera != null) {
            droidCamera.release();
            droidCamera = null;
        }
        takingPicture = false;
        status_LinearLayout.setVisibility(LinearLayout.INVISIBLE);
        statusText_TextView.setText("");
    }
       
    private String getDemandedFlashMode() {
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(getBaseContext());
        String demanded = prefs.getString("Preferences_Camera_Flash", Parameters.FLASH_MODE_OFF);
        
        if (demanded.equals("off")) {
            return Parameters.FLASH_MODE_OFF;
        } else if (demanded.equals("auto")) {
            return Parameters.FLASH_MODE_AUTO;
        } else if (demanded.equals("on")) {
            return Parameters.FLASH_MODE_ON;
        } else if (demanded.equals("redEye")) {
            return Parameters.FLASH_MODE_RED_EYE;
        } else if (demanded.equals("torch")) {
            return Parameters.FLASH_MODE_TORCH;
        }
        
        return Parameters.FLASH_MODE_OFF;
    }
    
    private void applySettings() {
        
        // Visibility of the FPS text view
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(getBaseContext());
        fps_TextView.setVisibility(
            (prefs.getBoolean("Preferences_View_ShowFPS", true))? TextView.VISIBLE : TextView.INVISIBLE
        );      
        
        // Visibility of the center mark
        centerMark.setVisibility(
            (prefs.getBoolean("Preferences_View_ShowTarget", true))? ImageView.VISIBLE : ImageView.INVISIBLE
        );
        
        if (droidCamera != null) {
            Camera cam = droidCamera.getCamera();
            Parameters camParams = cam.getParameters();
            
            // Flash mode
            List<String> flashModes = camParams.getSupportedFlashModes();
            String demandedFlashMode = getDemandedFlashMode();
            
            if (flashModes != null && flashModes.contains(demandedFlashMode)) {
                camParams.setFlashMode(demandedFlashMode);
                cam.setParameters(camParams);
            }
        }
               
    }
       
    /** Create a File for saving an image or video */
    private File getImageFile(){
        Log.i(TAG, "getOutputMediaFile");

        if (Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED)) {

            SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(getBaseContext());
            Resources res = getResources();
            String defaultDir =  Environment.getExternalStorageDirectory().getAbsolutePath() + 
                    File.separator + res.getString(R.string.app_name);
            File imagesDir = new File(prefs.getString("Preferences_Images_FilePath", defaultDir));
    
            // Create the storage directory if it does not exist
            if (!imagesDir.exists()){
                if (!imagesDir.mkdirs()){
                    Log.e(TAG, "Failed to create directory structure!");
                    showErrorAlert(R.string.Errors_Storage_MKDIRS);
                    return null;
                }
            }
    
            // Create a media file name
            File imageFile;
            String timeStamp = new SimpleDateFormat("yyyyMMdd_HHmmss").format(new Date());
            String filePrefix = prefs.getString("Preferences_Images_FilePrefix", DEFAULT_IMAGE_PREFIX);
            String prefFormat = prefs.getString("Preferences_Images_Format", DEFAULT_IMAGE_FORMAT);
            String fileSuffix = JPEG_FILE_SUFFIX;
            
            if (prefFormat.equals(res.getString(R.string.fileformat_jpeg))) {
                fileSuffix = JPEG_FILE_SUFFIX;
            } else if (prefFormat.equals(res.getString(R.string.fileformat_raw))) {
                fileSuffix = RAW_FILE_SUFFIX;
            }
            
            imageFile = new File(imagesDir.getPath() + File.separator + filePrefix + timeStamp + 
                    fileSuffix);
    
            return imageFile;
        }
        Log.e(TAG, "Not mounted sd card!");
        showErrorAlert(R.string.Errors_Storage_Mount);
        return null;
    }
    
    private void showErrorAlert(int resID) {
        showErrorAlert(this, resID);
    }
    
    private void showErrorAlert(String message) {
        showErrorAlert(this, message);
    }
       
	public static void showErrorAlert(Context context, int resID) {
	    showErrorAlert(context, context.getResources().getString(resID));
	}
	
	
	public static void showErrorAlert(Context context, String message) {
        new AlertDialog.Builder(context)
        .setTitle(R.string.err_alert)
        .setMessage(message)
        .setNeutralButton(android.R.string.ok, null)
        .setIcon(android.R.drawable.ic_dialog_alert)
        .show();
    }
}