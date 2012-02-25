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

public class QrReaderBrowseActivity extends FileListActivity {

    public void onCreate(Bundle bundle) {
        super.onCreate(bundle);      
    }
    
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.browse_options_menu, menu);
         
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        
        switch (item.getItemId()) {
            case R.id.manageStorage:
                try {
                    Intent intent = new Intent(Intent.ACTION_GET_CONTENT);  
                    Uri fileUri = Uri.parse("file://" + baseDir);
                    intent.setDataAndType(fileUri, "file/*");  
                    startActivity(intent);  
                } catch (ActivityNotFoundException e) {
                    e.printStackTrace();
                    QrReaderActivity.showErrorAlert(this, R.string.Errors_OI_Manager_Intent);
                } 
                return true;
            default:
                return super.onOptionsItemSelected(item);
        }
    }
}
