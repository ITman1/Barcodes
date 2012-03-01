package com.android.qrreader.qrcode.views;

import android.content.Context;
import android.view.View;

public interface QrCodeViewProvider {
    public View getView(Context context);
    
    public String getTitleName(Context context);
}
