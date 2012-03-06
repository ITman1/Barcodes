///////////////////////////////////////////////////////////////////////////////
// Project:    QR Reader for Android
// Package:    com.android.qrreader
// File:       MainActivity.java
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Contains the main activity of this application.
///////////////////////////////////////////////////////////////////////////////

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
import android.net.Uri;
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
import android.view.WindowManager;
import android.view.View.OnClickListener;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.android.camera.CameraPreview;
import com.android.camera.DroidCamera;
import com.android.listviews.FileListActivity;
import com.android.qrreader.R;

/**
 * The Class of the main activity of the application QR Reader for Android.
 */
public class MainActivity extends Activity {
    
    /** The Constant Debug TAG. */
    private static final String TAG = "com.android.qrreader::QrReaderActivity";
    
    /** The default directory where the SD card is mounted. */
    public static final String SD_CARD_DIR                  = "/sdcard";
    
    /** The file extension for JPEG images. */
    private static final String JPEG_FILE_EXTENSION         = ".jpg";
    
    ///** The file extension for the RAW images. */
    //private static final String RAW_FILE_EXTENSION          = ".raw";
    
    /** The file extension for the QR codes. */
    private static final String QR_FILE_EXTENSION           = ".qr";
    
    /** The default file prefix for the images stored by this application. */
    private static final String DEFAULT_IMAGE_PREFIX        = "IMG_";
    
    /** The default file prefix for the QR codes stored by this application. */
    private static final String DEFAULT_QR_PREFIX           = "QR_";
    
    ///** The default image format in which the images will be stored. */
    //private static final String DEFAULT_IMAGE_FORMAT        = "JPEG";
    
    /** The text which displays in the text view that contains the FPS informations. */
    private static final String FPS_STR                     = "FPS: ";
    
    /** The default quality of the storing of JPEG images. */
    private static final int DEFAULT_JPEG_QUALITY           = 90;
    
    /** The filename for temporary QR code which will stored inside the
     * internal application directory and then passed its filename to 
     * another activity via Intent. */
    private static String TMP_QR_FILENAME                   = ".tmp_qr_code.qr";
    
    /** The filename for temporary decoded image which will stored inside the
     * internal application directory and then passed its filename to 
     * another activity via Intent. */
    private static String TMP_QR_IMAGE_FILENAME             = ".tmp_qr_code.jpg";

    /** The reference to the layout that contains the camera preview surface. */
    private RelativeLayout     cameraPreviewLayout;
	
	/** The reference to the surface where camera preview is displayed. */
	private CameraPreview      cameraPreview;
	
    /** The reference to the text view that contains the FPS information. */
    private TextView           fps_TextView;
    
    /** The reference to the image view that contains the center mark. */
    private ImageView          centerMark;
    
    /** The reference to the main button that snaps the image and then decodes. */
    private ImageButton        read_btn;
       
    /** The reference to the layout which contains status of the decoding the image. */
    private LinearLayout       status_LinearLayout;
    
    /** The reference to text view which displays the status of the decoding the image. */
    private TextView           statusText_TextView;
    
    /** The reference to the camera. */
    private DroidCamera        droidCamera;
    
    /** Variable informs that read button has been pushed and no snapshot 
     * has been returned yet. */
    private boolean            takingPicture;
    
    /** Informs about the time when has been initiated 
     * the preview callback for a last time. */
    private long               lastTimeFPS                  = System.currentTimeMillis();
    
    /** Informs about the time when has been snapped 
     * the image for a last time. */
    private long               lastTimeSnapshot             = System.currentTimeMillis();
    
    /** The reference to the snapped/captured image. */
    private byte[]             snapshotData;
    
    /** The reference to the decoded QR data. */
    private byte[]             QRCodeData;
       
    /** The listener of the click on the read button */
    private OnClickListener read_btn_OnClick = new OnClickListener() {
        public void onClick(View v) {
            Log.i(MainActivity.TAG, "OnClickListener::read_btn_OnClick");

        	if (droidCamera != null && !takingPicture) {
        	    
        	    // Disabling the read button and adding the status layout
        	    // which displays the progress of the image processing
        	    read_btn.setEnabled(false);
        	    takingPicture = true;
        	    status_LinearLayout.setVisibility(LinearLayout.VISIBLE);
                statusText_TextView.setText(R.string.QRReaderActivity_StatusText_Capturing);
                
                // Just due to bug in the version 2.3.X
                droidCamera.getCamera().setPreviewCallback(null); 
                
                // Getting whether a flash is requested
                SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(getBaseContext());
                Boolean autoFocus = prefs.getBoolean("Preferences_Camera_AutoFocus", false);
                
                // Taking the picture or registering the flash callback
                if (autoFocus && droidCamera.autoFocusSupport()) {
                    droidCamera.getCamera().autoFocus(autoFocusCallback);
                } else {
                    takePicture();
                }
        	}
        }
    };
    
    /** The auto-focus callback. In this callback is initiated the
     * capturing of the image */
    private AutoFocusCallback autoFocusCallback = new AutoFocusCallback() {
        @Override
        public void onAutoFocus(boolean success, Camera camera) {
            takePicture();
        }
    };
    
    /** The preview callback. In this callback is displayed FPS information.
     * TODO: Add support for real-time decoding. (in future) */
    private PreviewCallback droidCamera_PreviewCallback = new PreviewCallback() {
        public void onPreviewFrame(byte[] data, Camera camera) {
            Log.i(MainActivity.TAG, "PreviewCallback::onPreviewFrame");
            
            // Displaying the FPS
            fps_TextView.setText(FPS_STR + (long)(1 / (double)((System.currentTimeMillis()
                    - lastTimeFPS) / (double)1000)));
            lastTimeFPS = System.currentTimeMillis();
            
            // Returning the data buffer to the processing queue
            camera.addCallbackBuffer(data);
        }
    };
    
    /** The snapshot callback. This callback is called when a new 
     * snapshot is available and ready to be decoded as a QR code. */
    private PictureCallback takePictureCallback = new PictureCallback () {
        public void onPictureTaken(byte[] data, Camera camera) {
            Log.i(TAG, "onPictureTaken");
            
            // If is this method called in the interval shorter than 1 second
            // this will delay this thread till one second passes.
            // Prevents the generating of the same name for the files which uses
            // in the name the current time with accuracy of one second.
            long currTimeSnapshot = System.currentTimeMillis();
            if (currTimeSnapshot - lastTimeSnapshot < 1000 && currTimeSnapshot > lastTimeSnapshot) {
                android.os.SystemClock.sleep(currTimeSnapshot - lastTimeSnapshot);
            }
            lastTimeSnapshot = System.currentTimeMillis();

            if (data != null) {     // There are some data from the camera
                // Calling the decode method via JNI from the Barcode Library
                statusText_TextView.setText(R.string.QRReaderActivity_StatusText_Processing);
                //byte[] QRCodeData = readQRCode(data, FORMAT_JPEG);
                QRCodeData = new String("skype:225154").getBytes();
                snapshotData = data;
                startOpenQrIntent();
            } else {                // There are not any data, ignoring the fact (error)
                status_LinearLayout.setVisibility(LinearLayout.INVISIBLE);
                statusText_TextView.setText("");
                cameraPreviewLayout.setVisibility(CameraPreview.VISIBLE);
            }
            
        }
    };
    
    /**
     * Handles returning from the Open QR activity which can include the saving
     * of the QR code or captured image. 
     *  
     * @see android.app.Activity#onActivityResult(int, int, android.content.Intent) onActivityResult(int, int, Intent)
     */
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        /* Just format saving feature - TODO in future
        String prefFormat = prefs.getString("Preferences_Images_Format", DEFAULT_IMAGE_FORMAT);
        if (prefFormat.equals("JPEG")) {
            fileExtension = JPEG_FILE_EXTENSION;
        } else if (prefFormat.equals("RAW")) {
            fileExtension = RAW_FILE_EXTENSION;
        }*/
        
        // Retrieving the save method and format
        String fileExtension = JPEG_FILE_EXTENSION;
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(getBaseContext());
        String imageSaveMethod = prefs.getString("Preferences_Images_SaveMethod", "autoSave");
        String qrcodeSaveMethod = prefs.getString("Preferences_Images_SaveMethod", "autoSave");
        
        // Printing the information about saving into the status text view
        statusText_TextView.setText(R.string.QRReaderActivity_StatusText_Saving);
        
        // Saving the image if required.
        if ((imageSaveMethod.equals("autoSave")) 
                || ((resultCode & OpenQrActivity.RESULT_SAVE_IMAGE_BUTTON_CLICKED) > 0)
                || ((resultCode & OpenQrActivity.RESULT_SAVE_BOTH_BUTTON_CLICKED) > 0)) {
                
                savePicture(snapshotData, fileExtension);
        }
        
        // Saving the QR code if required.
        if ((qrcodeSaveMethod.equals("autoSave")) 
                || ((resultCode & OpenQrActivity.RESULT_SAVE_QRCODE_CLICKED) > 0)
                || ((resultCode & OpenQrActivity.RESULT_SAVE_BOTH_BUTTON_CLICKED) > 0)) {
                    
                saveQRCode(QRCodeData);
        }

        // Ends the taking of the picture
        takingPicture = false;
        cameraPreviewLayout.setVisibility(CameraPreview.VISIBLE);
    }
    
    /**
     * Sends an intent for opening the decoded QR code.
     */
    private void startOpenQrIntent() {
        FileOutputStream fos;
        
        // Getting informations about saving methods, which will be used for
        // building the result buttons of the Open QR activity
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(getBaseContext());
        boolean imagePromptToSave = prefs.getString("Preferences_Images_SaveMethod",
                "autoSave").equals("prompt");
        boolean qrcodePromptToSave = prefs.getString("Preferences_QRCodes_SaveMethod",
                "autoSave").equals("prompt");
        
        // Image visibility in the open QR activity
        boolean showImage = prefs.getBoolean("Preferences_View_ShowDecodedImage", true);
        
        try {
            // Saving the image into the internal storage for loading it from
            // here by the Open QR activity 
            if (showImage) {
                fos = openFileOutput(TMP_QR_IMAGE_FILENAME, Context.MODE_PRIVATE);
                fos.write(snapshotData);
                fos.close();
            }
            
            // Saving the QR code into the internal storage for loading it from
            // here by the Open QR activity 
            fos = openFileOutput(TMP_QR_FILENAME, Context.MODE_PRIVATE);
            fos.write(QRCodeData);
            fos.close();
            
            // Sending the location of the temporary QR code as a classical 
            //ACTION_VIEW intent for viewing the files
            Intent qrIntent = new Intent(getBaseContext(), OpenQrActivity.class);
            File qr_file = getFileStreamPath(TMP_QR_FILENAME);
            qrIntent.setAction(Intent.ACTION_VIEW);
            qrIntent.setData(Uri.fromFile(qr_file));
            
            // Adding the result buttons in the dependence on the save method
            if (imagePromptToSave) 
                qrIntent.putExtra(OpenQrActivity.EXTRA_ADD_SAVE_IMAGE_BUTTON, new String());
            if (qrcodePromptToSave) 
                qrIntent.putExtra(OpenQrActivity.EXTRA_ADD_SAVE_QRCODE_BUTTON, new String());
            if (imagePromptToSave && qrcodePromptToSave) 
                qrIntent.putExtra(OpenQrActivity.EXTRA_ADD_SAVE_BOTH_BUTTON, new String());
            if (showImage)
                qrIntent.putExtra(OpenQrActivity.EXTRA_IMAGE, getFileStreamPath(TMP_QR_IMAGE_FILENAME).getAbsolutePath());
            
            // Informing the activity that this intent sent this activity
            qrIntent.putExtra(OpenQrActivity.EXTRA_INTERNAL_INTENT, OpenQrActivity.EXTRA_INTERNAL_INTENT);
            
            // Starting the Open QR activity
            startActivityForResult(qrIntent, 0);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    
    /**
     * Saves picture data into the file with specified extension.
     *
     * @param data The picture data to be saved.
     * @param extension The extension of the file with which to save the file.
     */
    private void savePicture(byte[] data, String extension) {
        if (data != null) {
            Resources res = getResources();
            File pictureFile = getImageFile(extension);

            if (pictureFile != null) { 
                try {
                    FileOutputStream fos = new FileOutputStream(pictureFile);
                    fos.write(data);
                    fos.close();
                // Fired on the error of opening the file
                } catch (FileNotFoundException e) {
                    Log.e(TAG, "File not found: " + e.getMessage());
                    showErrorAlert(String.format(res.getString(R.string.
                            Errors_Storage_OpenWrite), res.getString(R.string.str_image)));
                // Error during saving the the fole
                } catch (IOException e) {
                    Log.e(TAG, "Error accessing file: " + e.getMessage());
                    showErrorAlert(String.format(res.getString(R.string.
                            Errors_Storage_Save), res.getString(R.string.str_image)));
                }
            }
        } else {
            showErrorAlert(R.string.Errors_Camera_PictureCallback);
        }
    }
    
    /**
     * Saves the QR code into the file.
     *
     * @param data The data of the QR code.
     */
    private void saveQRCode(byte[] data) {
        if (data != null) {
            SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(this);
            Resources res = getResources();
            
            // Get output directory and create it
            String qrDir = prefs.getString("Preferences_QRCodes_FilePath", SD_CARD_DIR);
            if (!createDirectoryStructure(new File(qrDir))) return;
            
            // Get output QR file
            String filePrefix = prefs.getString("Preferences_QRCodes_FilePrefix", DEFAULT_QR_PREFIX);
            String timeStamp = new SimpleDateFormat("yyyyMMdd_HHmmss").format(new Date());
            File qrFile = new File(qrDir + File.separator + filePrefix + timeStamp + QR_FILE_EXTENSION);

            // Write output QR file
            if (qrFile != null) { 
                try {
                    FileOutputStream fos = new FileOutputStream(qrFile);
                    fos.write(data);
                    fos.close();
                } catch (FileNotFoundException e) {
                    Log.e(TAG, "File not found: " + e.getMessage());
                    showErrorAlert(String.format(res.getString(R.string.
                            Errors_Storage_OpenWrite), res.getString(R.string.str_qrcode)));
                } catch (IOException e) {
                    Log.e(TAG, "Error accessing file: " + e.getMessage());
                    showErrorAlert(String.format(res.getString(R.string.
                            Errors_Storage_Save), res.getString(R.string.str_qrcode)));
                }
            }
        }
    }

    /**
     * Called when the activity is firstly created.
     * Initializes the window and gets the references to the GUI elements.
     *
     * @see android.app.Activity#onCreate(android.os.Bundle) onCreate(Bundle) 
     */
    @Override
    public void onCreate(Bundle savedInstanceState) {
    	Log.i(TAG, "onCreate");
        super.onCreate(savedInstanceState);
        
        // Setting window features and flags for making available the whole display
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
        getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN);
        
        // Setting the default shared preferences on first run
        SharedPreferences prefs = getBaseContext().getSharedPreferences(
                PreferenceManager.KEY_HAS_SET_DEFAULT_VALUES, Context.MODE_PRIVATE);
        if (!prefs.getBoolean(PreferenceManager.KEY_HAS_SET_DEFAULT_VALUES, false)) {
            SettingsActivity.resetSettings(getBaseContext());
        }
               
        // Setting main window view as camera preview from the layouts
        setContentView(R.layout.camera_preview);
        getWindow().setFormat(PixelFormat.UNKNOWN);

        // Getting view surface for the camera
        cameraPreview = (CameraPreview)findViewById(R.id.cameraPreview);
        cameraPreviewLayout = (RelativeLayout)findViewById(R.id.cameraPreviewLayout);
        
        // Adding camera controls layout over the camera preview view
        LayoutInflater controlInflater = LayoutInflater.from(getBaseContext());
        View cameraControls = controlInflater.inflate(R.layout.camera_controls, null);
        this.addContentView(cameraControls, new LayoutParams(LayoutParams.FILL_PARENT, 
                LayoutParams.FILL_PARENT));
       
        // Getting handlers for other controls on the layout
        fps_TextView = (TextView) findViewById(R.id.fps_TextView);
        centerMark = (ImageView) findViewById(R.id.centerMark);
        read_btn = (ImageButton) findViewById(R.id.read_btn);
        status_LinearLayout = (LinearLayout) findViewById(R.id.status_LinearLayout);
        statusText_TextView = (TextView) findViewById(R.id.statusText_TextView);
        
        // Registering the click listener on the read button
        read_btn.setOnClickListener(read_btn_OnClick);
    }
  
    /**
     * Called when activity is stopping. Handles the releasing the camera.
     * 
     * @see android.app.Activity#onPause() onPause()
     */
    @Override
    protected void onPause() {
        super.onPause();
        Log.i(TAG, "onPause");
        stopDroidCamera();
    }

    /**
     * Called when activity is ready for a run (eg from the pause). 
     * Handles the opening the camera and applying some application settings.
     * 
     * @see android.app.Activity#onResume() onResume()
     */
    @Override
    protected void onResume() {
        super.onResume();
        Log.i(TAG, "onResume");
        startDroidCamera();
        applySettings();
    }
    
    /**
     * Called when activity is going to be destroyed. Handles the releasing the camera.
     * 
     * @see android.app.Activity#onDestroy() onDestroy()
     */
    @Override
    protected void onDestroy() {
        super.onDestroy();
        Log.i(TAG, "onDestroy");
        stopDroidCamera();
    }
    
    /**
     * Callback which is called for the creating of the activity menu.
     * Inflating the menu from XML resource is done here.
     * 
     * @see android.app.Activity#onCreateOptionsMenu(android.view.Menu) onCreateOptionsMenu(Menu)
     */
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        Log.i(TAG, "onCreateOptionsMenu");
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.camera_options_menu, menu);
        return true;
    }

    /**
     * Listener of the click on the activity menu item.
     * Performs on click actions, mostly starts another activities via Intents.
     * 
     * @see android.app.Activity#onOptionsItemSelected(android.view.MenuItem) onOptionsItemSelected(MenuItem)
     */
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        Log.i(TAG, "onOptionsItemSelected");
        
        Intent intent;
        String cwd;
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(getBaseContext()); 
        
        switch (item.getItemId()) {
            case R.id.settings:         // Opening the preference activity
                startActivity(new Intent(this, SettingsActivity.class));
                return true;
            case R.id.recent_qrcodes:   // Opening the browser of the QR codes
                intent = new Intent(this, BrowseImagesActivity.class);
                cwd = prefs.getString("Preferences_QRCodes_FilePath", SD_CARD_DIR);
                intent.putExtra(FileListActivity.BASE_DIR, cwd);
                startActivity(intent);
                return true;
            case R.id.recent_images:    // Opening the browser of the images
                intent = new Intent(this, BrowseImagesActivity.class);
                cwd = prefs.getString("Preferences_Images_FilePath", SD_CARD_DIR);
                intent.putExtra(FileListActivity.BASE_DIR, cwd);
                startActivity(intent);
                return true;
            default:
                return super.onOptionsItemSelected(item);
        }
    }
    
    /**
     * The method which calls the camera method for taking the picture.
     */
    private void takePicture() {
     /* Just format saving feature - TODO in future
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(getBaseContext());
        String prefFormat = prefs.getString("Preferences_Images_Format", DEFAULT_IMAGE_FORMAT);

        if (prefFormat.equals(res.getString("JPEG")) {
            droidCamera.getCamera().takePicture(null, null, takePictureCallback);
        } else if (prefFormat.equals("RAW")) {
            droidCamera.getCamera().takePicture(null, takePictureCallback, null);
        } */
        
        cameraPreviewLayout.setVisibility(CameraPreview.INVISIBLE);
        droidCamera.getCamera().takePicture(null, null, takePictureCallback);
    }
    
    /**
     * Opens the camera and starts the previewing on the success,
     * otherwise prints the error.
     */
    private void startDroidCamera() {
        Log.i(TAG, "startDroidCamera");
        
        // Opening the camera
        if ((droidCamera = DroidCamera.open()) == null) {
            read_btn.setEnabled(false);
            Log.e(TAG, "Failed to open DroidCamera!");
            showErrorAlert(R.string.Errors_Camera_Failure);
            return;
        } 
        
        // Start camera previewing
        if (takingPicture == false) {
            droidCamera.startPreviewing(cameraPreview);
            droidCamera.setPreviewCallback(droidCamera_PreviewCallback);
            status_LinearLayout.setVisibility(LinearLayout.INVISIBLE);
            statusText_TextView.setText("");
            read_btn.setEnabled(true);
        }
    }
    
    /**
     * Releases the camera device and stops the previewing.
     */
    private void stopDroidCamera() {
        Log.i(TAG, "stopDroidCamera");
        if (droidCamera != null) {
            droidCamera.release();
            droidCamera = null;
        }
        
        // Removes the status layout from the camera preview.
        // Because snapshot callback has been probably aborted.
        takingPicture = false;       
        status_LinearLayout.setVisibility(LinearLayout.INVISIBLE);
        statusText_TextView.setText("");
    }
       
    /**
     * Retrieves the set flash mode which is selected in the preferences by user.
     *
     * @return The selected flash demanded.
     */
    private String getDemandedFlashMode() {
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(getBaseContext());
        String demanded = prefs.getString("Preferences_Camera_Flash", Parameters.FLASH_MODE_OFF);
        
        // Testing against the list keys
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
    
    /**
     * Applies the settings read from the shared preferences on this application.
     */
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
            }
                                  
            // JPEG quality
            camParams.setJpegQuality(
                prefs.getInt("Preferences_Images_JpegQuality", DEFAULT_JPEG_QUALITY)
            );
            
            // Finally set the new one parameters
            cam.setParameters(camParams);
        }
               
    }
       
    /**
     * Gets file which has generated filename and is situated in the proper storage.
     *
     * @param extension The extension of the image file.
     * @return The file with generated proper filename and situated in the output directory.
     */
    private File getImageFile(String extension){
        Log.i(TAG, "getOutputMediaFile");

        // Image can be saved only into the external storage - eg the SD card!
        if (Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED)) {

            SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(getBaseContext());
            Resources res = getResources();
            
            // Get the output directory for the image
            String defaultDir =  Environment.getExternalStorageDirectory().getAbsolutePath() + 
                    File.separator + res.getString(R.string.app_name);
            File imagesDir = new File(prefs.getString("Preferences_Images_FilePath", defaultDir));
    
            if (!createDirectoryStructure(imagesDir)) return null;
    
            // Create a media file name
            File imageFile;
            String timeStamp = new SimpleDateFormat("yyyyMMdd_HHmmss").format(new Date());
            String filePrefix = prefs.getString("Preferences_Images_FilePrefix", DEFAULT_IMAGE_PREFIX);

            // Create the file structure
            imageFile = new File(imagesDir.getPath() + File.separator + filePrefix + timeStamp + 
                    extension);
    
            return imageFile;
        }
        Log.e(TAG, "Not mounted sd card!");
        showErrorAlert(R.string.Errors_Storage_Mount);
        return null;
    }
    
    /**
     * Creates the directory structure till the destination directory.
     *
     * @param dir The directory to be created.
     * @return True, if successful, otherwise false.
     */
    private boolean createDirectoryStructure(File dir) {
        // Create the storage directory if it does not exist
        if (!dir.exists()){
            if (!dir.mkdirs()){
                Log.e(TAG, "Failed to create directory structure!");
                showErrorAlert(R.string.Errors_Storage_MKDIRS);
                return false;
            }
        }
        return true;
    }
    
    /**
     * Shows error alert.
     *
     * @param resID The resource id of the message.
     */
    private void showErrorAlert(int resID) {
        showErrorAlert(this, resID);
    }
    
    /**
     * Shows error alert.
     *
     * @param message The error message.
     */
    private void showErrorAlert(String message) {
        showErrorAlert(this, message);
    }
       
	/**
	 * Shows error alert.
	 *
	 * @param context The context.
     * @param resID The resource id of the message.
	 */
	public static void showErrorAlert(Context context, int resID) {
	    showErrorAlert(context, context.getResources().getString(resID));
	}
	
	
	/**
	 * Shows error alert.
	 *
     * @param context The context.
     * @param message The error message.
	 */
	public static void showErrorAlert(Context context, String message) {
        new AlertDialog.Builder(context)
        .setTitle(R.string.err_alert)
        .setMessage(message)
        .setNeutralButton(android.R.string.ok, null)
        .setIcon(android.R.drawable.ic_dialog_alert)
        .show();
    }
}

//End of the file