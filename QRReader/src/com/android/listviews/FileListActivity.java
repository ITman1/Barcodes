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
import android.widget.ListView;
import android.widget.TextView;

import com.android.qrreader.R;
import com.filesystem.Filename;

public class FileListActivity extends ListActivity {
    

    public static final String CWD          = "CWD";
    public static final String BASE_DIR     = "BASE_DIR";
    
    protected String cwd                      = "/sdcard";
    protected String baseDir                  = "/";
    private ArrayList<String> currPathList;
    
    private TextView path_TextView;
    
    private int pathToDelete;

    public void onCreate(Bundle bundle) {
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        super.onCreate(bundle);      
        setContentView(R.layout.filelist_main);
        
        path_TextView = (TextView) findViewById(R.id.path);
        
        ListView listView = getListView();

        listView.setOnItemLongClickListener( new AdapterView.OnItemLongClickListener(){
            @Override
            public boolean onItemLongClick(AdapterView<?> av, View v, int pos, long id) {
                onLongListItemClick(v,pos,id);
                return false;
            }
    
        });
        
        String cwdExtra = getIntent().getStringExtra(CWD);
        cwd = (cwdExtra != null)? cwdExtra : cwd;
        
        String baseDirExtr = getIntent().getStringExtra(BASE_DIR);
        baseDir = (baseDirExtr != null)? baseDirExtr : baseDir;

        changeDir(cwd);
 
    }
       
    protected void onLongListItemClick(View v, int position, long id) {
        boolean hasGotoParent = !(new File(cwd)).equals(new File(baseDir));
        if (hasGotoParent && position == 0) return;
        
        String path = currPathList.get(position);
        int message;
        
        if ((new File(path)).isDirectory()) {
            message = R.string.FileListActivity_delete_folder;
        } else {
            message = R.string.FileListActivity_delete_regular_file;
        }

        Resources res = getResources();
        pathToDelete = position;
        new AlertDialog.Builder(this)
        .setTitle(R.string.confirm_alert)
        .setMessage(String.format(res.getString(message), Filename.getFullFilename(path, File.separatorChar)))
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
    } 

    @Override
    protected void onListItemClick(ListView l, View v, int position, long id) {
        String path = currPathList.get(position);
        
        if ((new File(path)).isDirectory()) {
            changeDir(path);
        } else {
            File file = new File(path);
            MimeTypeMap myMime = MimeTypeMap.getSingleton();
            Intent intent = new Intent(android.content.Intent.ACTION_VIEW);

            String mimeType = myMime.getMimeTypeFromExtension(Filename.getExtension(path));
            intent.setDataAndType(Uri.fromFile(file),mimeType);
            try {
                startActivityForResult(intent, 0);
            } catch (android.content.ActivityNotFoundException e) {
                new AlertDialog.Builder(this)
                .setTitle(R.string.err_alert)
                .setMessage(R.string.Errors_No_Open_Intent)
                .setNeutralButton(android.R.string.ok, null)
                .setIcon(android.R.drawable.ic_dialog_alert)
                .show();
            }
            
        }
    }
    
    private void changeDir(String dirPath) {
        File dir =  (Filename.isSubDirectory(baseDir, dirPath))? new File(dirPath) : new File(baseDir);
        boolean hasGotoParent = !dir.equals(new File(baseDir));
        
        currPathList = new ArrayList<String>();
        File[] listOfFiles = dir.listFiles(); 
        
        if (hasGotoParent) {
            String parent = dir.getParent();
            currPathList.add((parent != null)? parent : cwd);
        }
        
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
        
        Collections.sort(directories);
        Collections.sort(files);
        currPathList.addAll(directories);
        currPathList.addAll(files);
        
        cwd = dir.getAbsolutePath();
        path_TextView.setText(cwd);
        
        FileListAdapter adapter = new FileListAdapter(this, currPathList);
        adapter.hasGotoParentRow(hasGotoParent);
        setListAdapter(adapter);
    }

    
} 