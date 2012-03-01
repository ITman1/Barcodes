package com.android.qrreader.qrcode.views;

import android.content.Context;
import android.content.res.Resources;
import android.view.View;

import com.adapter.Adapter;
import com.android.qrreader.R;
import com.android.qrreader.qrcode.views.MailQrCodeView.MailQrCodeViewProvider;
import com.qrcode.qrcodes.MailQrCode;

public class MailQrCodeView  implements Adapter {
    public class MailQrCodeViewProvider implements QrCodeViewProvider {
        private MailQrCode qrCode;
        
        private MailQrCodeViewProvider(MailQrCode qrCode) {
            this.qrCode = qrCode;
        }
        
        public View getView(Context context) {
            View view = new View(context);
            return view;
        }
        
        public String getTitleName(Context context) {
            Resources res = context.getResources();
            return res.getString(R.string.QrCode_Title_Mail);
        }
    };
    
    @Override
    public Object getProvider(Object o) {
        return new MailQrCodeViewProvider((MailQrCode) o);
    }
    
    @Override
    public Class<?> getAdapteeClass() {
        return MailQrCode.class;
    }
    
    @Override
    public Class<?> getResultClass() {
        return View.class;
    }
}
