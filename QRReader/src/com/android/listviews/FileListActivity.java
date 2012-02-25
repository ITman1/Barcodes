package com.android.listviews;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

import android.app.AlertDialog;
import android.app.ListActivity;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.widget.AdapterView;
import android.widget.ListView;
import android.widget.TextView;

import com.android.qrreader.R;
import com.filesystem.Filename;

public class FileListActivity extends ListActivity {
    

    public static final String CWD          = "CWD";
    public static final String BASE_DIR     = "BASE_DIR";
    
    private String cwd                      = "/sdcard";
    private String baseDir                  = "/";
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
    // You then create your handler method:
    protected void onLongListItemClick(View v, int position, long id) {
        String path = currPathList.get(position);
        int message;
        
        if ((new File(path)).isDirectory()) {
            message = R.string.FileListActivity_delete_folder;
        } else {
            message = R.string.FileListActivity_delete_regular_file;
        }

        pathToDelete = position;
        new AlertDialog.Builder(this)
        .setTitle(R.string.confirm_alert)
        .setMessage(message)
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
        .show();
    } 

    @Override
    protected void onListItemClick(ListView l, View v, int position, long id) {
        String path = currPathList.get(position);
        
        if ((new File(path)).isDirectory()) {
            changeDir(path);
        } else {
            Intent intent = new Intent();
            intent.setType("image/*");
            intent.setAction(Intent.ACTION_GET_CONTENT);
            startActivityForResult(Intent.createChooser(intent,
            "Select Picture"), 1);
            
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
        
        if (listOfFiles != null) {
            for (File file : listOfFiles) {
                   
                if (file.isFile() || file.isDirectory()) {
                    currPathList.add(file.getAbsolutePath());
                }
            }
        }
        
        cwd = dir.getAbsolutePath();
        path_TextView.setText(cwd);
        
        FileListAdapter adapter = new FileListAdapter(this, currPathList);
        adapter.hasGotoParentRow(hasGotoParent);
        setListAdapter(adapter);
    }

    
} 