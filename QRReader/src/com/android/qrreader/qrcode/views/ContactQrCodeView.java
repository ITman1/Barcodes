package com.android.qrreader.qrcode.views;

import android.content.Context;
import android.content.res.Resources;
import android.view.View;

import com.adapter.Adapter;
import com.android.qrreader.R;
import com.android.qrreader.qrcode.views.ContactQrCodeView.ContactQrCodeViewProvider;
import com.qrcode.qrcodes.ContactQrCode;

public class ContactQrCodeView implements Adapter {
    public class ContactQrCodeViewProvider implements QrCodeViewProvider {
        private ContactQrCode qrCode;
        
        private ContactQrCodeViewProvider(ContactQrCode qrCode) {
            this.qrCode = qrCode;
        }
        
        public View getView(Context context) {
            View view = new View(context);
            return view;
        }
        
        public String getTitleName(Context context) {
            Resources res = context.getResources();
            return res.getString(R.string.QrCode_Title_Contact);
        }
    };
    
    @Override
    public Object getProvider(Object o) {
        return new ContactQrCodeViewProvider((ContactQrCode) o);
    }
    
    @Override
    public Class<?> getAdapteeClass() {
        return ContactQrCode.class;
    }
    
    @Override
    public Class<?> getResultClass() {
        return View.class;
    }
}
