///////////////////////////////////////////////////////////////////////////////
// Project:    QR Reader for Android
// Package:    com.android.qrreader
// File:       BrowseQRCodesActivity.java
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Contains the activity which provides the UI for 
//             installing/uninstalling the plug-ins.
///////////////////////////////////////////////////////////////////////////////

package com.android.qrreader;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.List;

import com.android.qrreader.installation.InstallationManager;
import com.android.qrreader.installation.InstallationManager.CorruptedPackage;
import com.android.qrreader.installation.InstallationManager.PackageInfo;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View.OnClickListener;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.TextView;

/**
 * The Class InstallationManagerActivity provides the UI for installing
 * or uninstalling the installable plug-ins. For picking plug-ins is recommended
 * to have installed the OI File Manager.
 * 
 * @see com.android.qrreader.installation.InstallationManager InstallationManager
 */
public class InstallationManagerActivity extends Activity {

    /** The extra intent for picking the file via OI File Manager. */
    final private static String PICK_FILE_INTENT        = "org.openintents.action.PICK_FILE";
    
    /** The extra title for picking the file inside the OI File Manager. */
    final private static String PICK_FILE_INTENT_TITLE  = "org.openintents.extra.TITLE";
    
    /** The edit for supplying the path to the installable package. */
    private EditText pathEdit;
    
    /** The linear layout which contains the views of the installed plug-ins. */
    private LinearLayout packageList;
    
    /** The counter for inserting the IDs into the views and uniquely 
     * identifying them inside the {@link #packageList}. */
    private int itemID  = 0;
    
    /** The click listener of the browse button. Sends the PICK_FILE intent 
     * directed to the OI File Manager for picking the plug-in. */
    private OnClickListener browseButtonClick = new OnClickListener() {
        
        @Override
        public void onClick(View v) {
            // Sending PICK_FILE intent for picking the plug-in
            Intent intent = new Intent(PICK_FILE_INTENT);
            String title = getBaseContext().getResources().getString(R.string.InstallationManagerActivity_pickFileTitle);
            intent.putExtra(PICK_FILE_INTENT_TITLE, title);
            
            try {
                startActivityForResult(intent, 1);
            } catch (android.content.ActivityNotFoundException e) {
                MainActivity.showErrorAlert(InstallationManagerActivity.this, R.string.Errors_OI_Manager_Intent);
            }
        }
    };
    
    /** The click listener of the install button. Tries to install 
     * the specified plug-in, if there is any. */
    private OnClickListener installButtonClick = new OnClickListener() {
        
        @Override
        public void onClick(View v) {
            InstallationManager installManager = InstallationManager.getInstallationManager(InstallationManagerActivity.this);
            List<File> packages = installManager.getPackages();
            File installedPackage;
            
            // Installing the plug-in
            try {
                installedPackage = installManager.installPackage(pathEdit.getText().toString());
                itemID++;
            } catch (FileNotFoundException e) {
                MainActivity.showErrorAlert(InstallationManagerActivity.this, R.string.Errors_Package_Not_Found); return;
            } catch (CorruptedPackage e) {
                MainActivity.showErrorAlert(InstallationManagerActivity.this, R.string.Errors_Package_Corrupted); return;
            } catch (IOException e) {
                MainActivity.showErrorAlert(InstallationManagerActivity.this, R.string.Errors_Package_Install); return;
            }

            // Adding/Displaying the plug-in item on the screen
            if (packages.size() != installManager.getPackages().size()) {
                addPackageItem(installedPackage);
                pathEdit.setText("");
                
            }
        }
    };

    /**
     * Called when the activity is firstly created.
     * Gets the references to the GUI elements and creates the view
     * with all installed plug-ins.
     *
     * @see android.app.Activity#onCreate(android.os.Bundle) onCreate(Bundle) 
     */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        setContentView(R.layout.installation_manager);
        
        Button button;
        button = (Button) findViewById(R.id.browseButton);
        button.setOnClickListener(browseButtonClick);
        
        button = (Button) findViewById(R.id.installButton);
        button.setOnClickListener(installButtonClick);
        
        pathEdit = (EditText) findViewById(R.id.pathEdit);
        
        packageList = (LinearLayout) findViewById(R.id.packageList);
        
        createPackagesView();
    }
    
    /**
     * Creates the view with installed packages.
     */
    private void createPackagesView() {
        InstallationManager installManager = InstallationManager.getInstallationManager(this);
        List<File> packages = installManager.getPackages();
        
        for (File currPackage : packages) {           
            addPackageItem(currPackage);
        }

    }
    
    /**
     * Adds the package item view into the {@link #packageList}.
     * Implements also the listener of the click on the uninstall button.
     *
     * @param packageFile The package file to be added into the plug-in list.
     */
    private void addPackageItem(File packageFile) {
        InstallationManager installManager = InstallationManager.getInstallationManager(this);
        LayoutInflater packageInflater = LayoutInflater.from(getBaseContext());
        
        // Getting the informations about the plug-ins
        PackageInfo info = installManager.getPackageInfo(packageFile);
        
        // Inflating the new view for the plug-in and setting the brief and name of the plug-in
        LinearLayout view = (LinearLayout) packageInflater.inflate(R.layout.installation_manager_package, null);
        itemID++;
        view.setId(itemID);
        ((TextView)view.findViewById(R.id.briefText)).setText(info.brief);
        ((TextView)view.findViewById(R.id.packageName)).setText(info.name);
        
        // Handling the click on the uninstall button
        Button button = (Button)view.findViewById(R.id.uninstall);
        button.setTag(packageFile);     // We will need to identify which package to uninstall
        button.setId(itemID);           // Required for getting the reference to plug-in view via button
        button.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                InstallationManager installManager = InstallationManager.getInstallationManager(InstallationManagerActivity.this);
                installManager.uninstallPackage(((File)v.getTag()).getAbsolutePath());
                packageList.removeView(packageList.findViewById(v.getId()));
            }
        });
        packageList.addView(view);
    }
    
    /**
     * Handles the returning from the OI File Manager and sets the picked
     * package path into the {@link #pathEdit}.
     * 
     * @see android.app.Activity#onActivityResult(int, int, android.content.Intent)
     */
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        String path;
        if (resultCode == RESULT_OK && (path = data.getData().getPath()) != null ) {
            pathEdit.setText(path);
        }
    }
}

//End of the file