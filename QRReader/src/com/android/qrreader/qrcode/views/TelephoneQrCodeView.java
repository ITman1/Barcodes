package com.android.qrreader.qrcode.views;

import com.adapter.Adapter;
import com.android.qrreader.R;
import com.qrcode.qrcodes.MailQrCode;
import com.qrcode.qrcodes.TelephoneQrCode;

import android.content.Context;
import android.content.Intent;
import android.content.res.Resources;
import android.net.Uri;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RelativeLayout;
import android.widget.TextView;

public class TelephoneQrCodeView implements Adapter {
    
    public class TelephoneQrCodeViewProvider implements QrCodeViewProvider {
        private TelephoneQrCode qrCode;
        
        private Context context;
        private RelativeLayout resultView;

        private OnClickListener onClickCallButton = new OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(Intent.ACTION_CALL);
                intent.setData(Uri.parse(qrCode.getUri()));
                Resources res = context.getResources();
                context.startActivity(Intent.createChooser(intent,  res.getString(R.string.QrCode_Title_Telephone_Call)));    
            }
        };
                
        private TelephoneQrCodeViewProvider(TelephoneQrCode qrCode) {
            this.qrCode = qrCode;
        }
        
        public View getView(Context context) {
            if (resultView == null) { // Instantiate just once per this instance of the provider
                this.context = context;
                LayoutInflater resultInflater = LayoutInflater.from(context);
                resultView = (RelativeLayout) resultInflater.inflate(R.layout.openqr_view_telephone, null);
                
                Button callButton = (Button) resultView.findViewById(R.id.callButton);
                TextView telephoneNumber = (TextView)resultView.findViewById(R.id.telephoneNumber);
                telephoneNumber.setText(qrCode.getTelephone());
                
                callButton.setOnClickListener(onClickCallButton);
                
            }
            
            return resultView;
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
