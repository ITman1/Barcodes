package com.android.qrreader;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import com.android.qrreader.InstallationManager.CorruptedPackage;
import com.android.qrreader.InstallationManager.PackageInfo;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View.OnClickListener;
import android.view.LayoutInflater;
import android.view.View;
import android.view.Window;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.TextView;

public class InstallationManagerActivity extends Activity {
    
    final private static String PICK_FILE_INTENT        = "org.openintents.action.PICK_FILE";
    final private static String PICK_FILE_INTENT_TITLE  = "org.openintents.extra.TITLE";
    
    private EditText pathEdit;
    private LinearLayout packageList;
    
    private int itemID  = 0;
    
    private OnClickListener browseButtonClick = new OnClickListener() {
        
        @Override
        public void onClick(View v) {
            Intent intent = new Intent(PICK_FILE_INTENT);
            String title = getBaseContext().getResources().getString(R.string.InstallationManagerActivity_pickFileTitle);
            intent.putExtra(PICK_FILE_INTENT_TITLE, title);
            try {
                startActivityForResult(intent, 1);
            } catch (android.content.ActivityNotFoundException e) {
                MainActivity.showErrorAlert(InstallationManagerActivity.this, R.string.Errors_OI_Manager_Intent);
            }
            startActivity(getIntent());
            finish();
        }
    };
    
    private OnClickListener installButtonClick = new OnClickListener() {
        
        @Override
        public void onClick(View v) {
            InstallationManager installManager = InstallationManager.getInstallationManager(InstallationManagerActivity.this);
            List<File> packages = installManager.getPackages();
            
            File installedPackage;
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

            if (packages.size() != installManager.getPackages().size()) {
                addPackageItem(installedPackage);
                pathEdit.setText("");
                
            }
        }
    };

    @Override
    public void onCreate(Bundle savedInstanceState) {
        requestWindowFeature(Window.FEATURE_NO_TITLE);
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
    
    private void createPackagesView() {
        InstallationManager installManager = InstallationManager.getInstallationManager(this);
        List<File> packages = installManager.getPackages();
        
        for (File currPackage : packages) {           
            addPackageItem(currPackage);
        }

    }
    
    private void addPackageItem(File packageFile) {
        InstallationManager installManager = InstallationManager.getInstallationManager(this);
        LayoutInflater packageInflater = LayoutInflater.from(getBaseContext());
        PackageInfo info = installManager.getPackageInfo(packageFile);
        LinearLayout view = (LinearLayout) packageInflater.inflate(R.layout.installation_manager_package, null);
        
        itemID++;
        view.setId(itemID);
        ((TextView)view.findViewById(R.id.briefText)).setText(info.brief);
        ((TextView)view.findViewById(R.id.packageName)).setText(info.name);
        Button button = (Button)view.findViewById(R.id.uninstall);
        button.setTag(packageFile);
        button.setId(itemID);
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
    
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (resultCode == RESULT_OK) {
            pathEdit.setText(data.getData().toString().substring(7));
        }
    }

}
