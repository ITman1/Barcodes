package com.android.qrreader.qrcode.views;

import android.content.Context;
import android.content.res.Resources;
import android.view.View;

import com.adapter.Adapter;
import com.android.qrreader.R;
import com.qrcode.qrcodes.UrlQrCode;

public class UrlQrCodeView implements Adapter {
    public class UrlQrCodeViewProvider implements QrCodeViewProvider {
        private UrlQrCode qrCode;
        
        private UrlQrCodeViewProvider(UrlQrCode qrCode) {
            this.qrCode = qrCode;
        }
        
        public View getView(Context context) {
            View view = new View(context);
            return view;
        }
        
        public String getTitleName(Context context) {
            Resources res = context.getResources();
            return res.getString(R.string.QrCode_Title_Url);
        }
    };
    
    @Override
    public Object getProvider(Object o) {
        return new UrlQrCodeViewProvider((UrlQrCode) o);
    }
    
    @Override
    public Class<?> getAdapteeClass() {
        return UrlQrCode.class;
    }
    
    @Override
    public Class<?> getResultClass() {
        return View.class;
    }
}
