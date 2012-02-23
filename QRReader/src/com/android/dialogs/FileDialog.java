package com.android.dialogs;

import android.app.Dialog;
import android.content.Context;

import com.android.qrreader.R;

public class FileDialog extends Dialog {

    public FileDialog(final Context context) {
        super(context);
        setContentView(R.layout.file_dialog);  
    }

} 