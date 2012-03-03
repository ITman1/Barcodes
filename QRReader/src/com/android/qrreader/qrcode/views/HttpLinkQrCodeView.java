package com.android.qrreader.qrcode.views;

import java.util.regex.Pattern;

import android.content.Context;
import android.content.res.Resources;
import android.text.util.Linkify;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.adapter.Adapter;
import com.android.qrreader.R;
import com.qrcode.qrcodes.HttpLinkQrCode;

public class HttpLinkQrCodeView implements Adapter {
    public class HttpLinkQrCodeViewProvider implements QrCodeViewProvider {
        final private static String HTTP_SCHEME = "http";
        private HttpLinkQrCode qrCode;
        
        private RelativeLayout resultView;
        
        private HttpLinkQrCodeViewProvider(HttpLinkQrCode qrCode) {
            this.qrCode = qrCode;
        }
        
        public View getView(Context context) {
            if (resultView == null) { // Instantiate just once per this instance of the provider
                LayoutInflater resultInflater = LayoutInflater.from(context);
                resultView = (RelativeLayout) resultInflater.inflate(R.layout.openqr_view_url, null);
                
                TextView urlLink = (TextView)resultView.findViewById(R.id.linkURL);
                urlLink.setText(qrCode.getLink().toExternalForm());
                Pattern pattern = Pattern.compile("^.*$");
                Linkify.addLinks(urlLink, pattern, HTTP_SCHEME);
            }

            return resultView;
        }
        
        public String getTitleName(Context context) {
            Resources res = context.getResources();
            return res.getString(R.string.QrCode_Title_Url);
        }
    };
    
    @Override
    public Object getProvider(Object o) {
        return new HttpLinkQrCodeViewProvider((HttpLinkQrCode) o);
    }
    
    @Override
    public Class<?> getAdapteeClass() {
        return HttpLinkQrCode.class;
    }
    
    @Override
    public Class<?> getResultClass() {
        return View.class;
    }
}
