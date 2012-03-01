package com.android.qrreader.qrcode.views;

import com.adapter.Adapter;
import com.android.qrreader.R;
import com.qrcode.qrcodes.TelephoneQrCode;

import android.content.Context;
import android.content.res.Resources;
import android.view.View;

public class TelephoneQrCodeView implements Adapter {
    
    public class TelephoneQrCodeViewProvider implements QrCodeViewProvider {
        private TelephoneQrCode qrCode;
        
        private TelephoneQrCodeViewProvider(TelephoneQrCode qrCode) {
            this.qrCode = qrCode;
        }
        
        public View getView(Context context) {
            View view = new View(context);
            return view;
        }
        
        public String getTitleName(Context context) {
            Resources res = context.getResources();
            return res.getString(R.string.QrCode_Title_Telephone);
        }
    };
    
    @Override
    public Object getProvider(Object o) {
        return new TelephoneQrCodeViewProvider((TelephoneQrCode) o);
    }
    
    @Override
    public Class<?> getAdapteeClass() {
        return TelephoneQrCode.class;
    }
    
    @Override
    public Class<?> getResultClass() {
        return View.class;
    }

}
