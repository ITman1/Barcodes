///////////////////////////////////////////////////////////////////////////////
// Project:    QR Reader for Android
// Package:    com.android.qrreader
// File:       BrowseActivity.java
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Contains activity which extends the file list activity.
///////////////////////////////////////////////////////////////////////////////

package com.android.qrreader;

import android.content.ActivityNotFoundException;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import com.android.listviews.FileListActivity;
import com.android.qrreader.R;

/**
 * The Class BrowseActivity extends the FileListActivity.
 * For now only adds the menu item for opening the OI Manager for further
 * management of the files.
 * 
 * @see com.android.listviews.FileListActivity FileListActivity
 */
public class BrowseActivity extends FileListActivity {

    /*
     * @see com.android.listviews.FileListActivity#onCreate(android.os.Bundle)
     */
    public void onCreate(Bundle bundle) {
        super.onCreate(bundle);      
    }
    
    /**
     * Callback which is called for the creating of the activity menu.
     * Inflating the menu from XML resource is done here.
     * 
     * @see android.app.Activity#onCreateOptionsMenu(android.view.Menu) onCreateOptionsMenu(Menu)
     */
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.browse_options_menu, menu);
         
        return true;
    }

    /**
     * Listener of the click on the activity menu item.
     * Performs on click actions, here starts only OI File Manager
     * in which user can manage the base directory.
     * 
     * @see android.app.Activity#onOptionsItemSelected(android.view.MenuItem) onOptionsItemSelected(MenuItem)
     */
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        
        switch (item.getItemId()) {
            case R.id.manageStorage:    // Sending the intent for opening the file
                try {
                    Intent intent = new Intent(Intent.ACTION_GET_CONTENT);  
                    Uri fileUri = Uri.parse("file://" + baseDir);
                    intent.setDataAndType(fileUri, "file/*");  
                    startActivity(intent);  
                } catch (ActivityNotFoundException e) {
                    e.printStackTrace();
                    MainActivity.showErrorAlert(this, R.string.Errors_OI_Manager_Intent);
                } 
                return true;
            default:
                return super.onOptionsItemSelected(item);
        }
    }
}

//End of the file
