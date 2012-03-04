///////////////////////////////////////////////////////////////////////////////
// Project:    QR Reader for Android
// Package:    com.android.listviews
// File:       FileListActivity.java
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Contains the activity which ensures the file browsing.
///////////////////////////////////////////////////////////////////////////////

package com.android.listviews;

import java.io.File;
import java.util.ArrayList;
import java.util.Collections;

import android.app.AlertDialog;
import android.app.ListActivity;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.res.Resources;
import android.net.Uri;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.webkit.MimeTypeMap;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemLongClickListener;
import android.widget.ListView;
import android.widget.TextView;

import com.android.qrreader.R;
import com.filesystem.Filename;

/**
 * Class extends activity and implements the browsing the file on the current 
 * file system. Also has integrated some operations with the files.
 * 
 * @version 1.0
 */
// TODO: Add more functionality, not just only deleting files/folders.
public class FileListActivity extends ListActivity implements OnItemLongClickListener {

    /** The Intent Constant for specifying the current working directory. */
    public static final String CWD            = "CWD";
    
    /** The Intent Constant for specifying the directory which will be as the root. */
    public static final String BASE_DIR       = "BASE_DIR";
    
    /** The current working directory. */
    protected String cwd                      = "/sdcard";
    
    /** The current base directory. */
    protected String baseDir                  = "/";
    
    /** The path list where are all file paths for current directory. */
    private ArrayList<String> currPathList;
    
    /** The reference to text view where is displayed current directory path. */
    private TextView path_TextView;
    
    /** The path to be deleted after confirmation dialog is positive. */
    private int pathToDelete;
    
    /**
     * Gets references to the GUI elements and opens the directory
     * on the current working directory.
     *
     * @param bundle {@inheritDoc}
     * @see android.app.Activity#onCreate(android.os.Bundle)
     */
    @Override
    public void onCreate(Bundle bundle) {
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        super.onCreate(bundle);      
        setContentView(R.layout.filelist_main);
        
        // Getting reference to path text view and setting long click listener
        path_TextView = (TextView) findViewById(R.id.path);
        ListView listView = getListView();
        listView.setOnItemLongClickListener(this);
        
        // Applying CWD extra
        String cwdExtra = getIntent().getStringExtra(CWD);
        cwd = (cwdExtra != null)? cwdExtra : cwd;
        
        // Applying BASE_DIR extra
        String baseDirExtr = getIntent().getStringExtra(BASE_DIR);
        baseDir = (baseDirExtr != null)? baseDirExtr : baseDir;

        changeDir(cwd);
 
    }

    /** 
     * On list item click handler method which opens a new directory 
     * and prints the list of the files or opens the file.
     * 
     * @see android.app.ListActivity#onListItemClick(android.widget.ListView, android.view.View, int, long)
     */
    @Override
    protected void onListItemClick(ListView l, View v, int position, long id) {
        String path = currPathList.get(position);
        
        if ((new File(path)).isDirectory()) {   // It has been clicked on the directory => change current working directory
            changeDir(path);
        } else {                                // Clicked on the file => try to open file
            
            // Getting which file has been clicked and retrieving the MIME type
            // from the file extension
            File file = new File(path);
            MimeTypeMap myMime = MimeTypeMap.getSingleton();
            Intent intent = new Intent(android.content.Intent.ACTION_VIEW);
            String mimeType = myMime.getMimeTypeFromExtension(Filename.getExtension(path));
            
            // Setting the MIME type and trying luck to open this file
            intent.setDataAndType(Uri.fromFile(file),mimeType);
            try {
                startActivityForResult(intent, 0);
            } catch (android.content.ActivityNotFoundException e) {
                // We had not any luck, so print error
                new AlertDialog.Builder(this)
                .setTitle(R.string.err_alert)
                .setMessage(R.string.Errors_No_Open_Intent)
                .setNeutralButton(android.R.string.ok, null)
                .setIcon(android.R.drawable.ic_dialog_alert)
                .show();
            }
            
        }
    }
    
    /** 
     * The handler function of the long click on the item. Implements, for now, only deleting files.
     * 
     * @see android.widget.AdapterView.OnItemLongClickListener#onItemLongClick(AdapterView, View, int, long)
     */
    @Override
    public boolean onItemLongClick(AdapterView<?> av, View v, int pos, long id) {
        // When has been clicked on the goto parent item, return from this method.
        boolean hasGotoParent = !(new File(cwd)).equals(new File(baseDir));
        if (hasGotoParent && pos == 0) return false;
        
        String path = currPathList.get(pos);
        int message;
        
        // Delete message to be printed
        if ((new File(path)).isDirectory()) {
            message = R.string.FileListActivity_delete_folder;
        } else {
            message = R.string.FileListActivity_delete_regular_file;
        }

        // Creating and showing confirmation dialog to delete this file/folder.
        Resources res = getResources();
        pathToDelete = pos;
        new AlertDialog.Builder(FileListActivity.this)
            .setTitle(R.string.confirm_alert)
            
            // Replacing the %s pattern in the message for the filename
            .setMessage(String.format(res.getString(message), Filename.getFullFilename(path, File.separatorChar)))
            
            // Adding positive button and applying delete event
            .setPositiveButton(android.R.string.yes, new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface dialog, int whichButton) {
                    if ((new File(currPathList.get(pathToDelete))).delete()) {
                        FileListAdapter adapter = ((FileListAdapter)getListAdapter());
                        adapter.remove(currPathList.get(pathToDelete));
                    }
                }
            })
            .setNegativeButton(android.R.string.no, null)
            .setIcon(android.R.drawable.ic_delete)
            .show();
        return true;
    }
    
    /**
     * Changes the current working directory.
     *
     * @param dirPath The directory path to which should be switched. 
     * This path should not be an upper than {@link #baseDir}.
     */
    private void changeDir(String dirPath) {
        // Correcting the dirPath whether it is not the sub-directory of the base directory
        File dir =  (Filename.isSubDirectory(baseDir, dirPath))? new File(dirPath) : new File(baseDir);
        // Retrieving whether we are in the base directory already
        boolean hasGotoParent = !dir.equals(new File(baseDir));
        
        // Clearing file list and getting a new one
        currPathList = new ArrayList<String>();
        File[] listOfFiles = dir.listFiles(); 
        
        // When we are not on the top directory (base) we have to add goto parent item
        if (hasGotoParent) {
            String parent = dir.getParent();
            currPathList.add((parent != null)? parent : cwd);
        }
        
        // Getting list of the files and directories
        ArrayList<String> directories = new ArrayList<String>();
        ArrayList<String> files = new ArrayList<String>();
        if (listOfFiles != null) {
            for (File file : listOfFiles) {
                if (!file.isHidden()) {
                    if (file.isFile()) {
                        files.add(file.getAbsolutePath());
                    } else if (file.isDirectory()) {
                        directories.add(file.getAbsolutePath());
                    }
                }
            }
        }
        
        // Sorting files and directories and inserting the directories as first
        Collections.sort(directories);
        Collections.sort(files);
        currPathList.addAll(directories);
        currPathList.addAll(files);
        
        // Just update cwd and path text view
        cwd = dir.getAbsolutePath();
        path_TextView.setText(cwd);
        
        // Adapter to apply over the list of files and directories
        FileListAdapter adapter = new FileListAdapter(this, currPathList);
        adapter.hasGotoParentRow(hasGotoParent);
        setListAdapter(adapter);
    }   
} 

// End of the file