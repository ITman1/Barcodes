package com.android.qrreader.qrcode.views;

import android.content.Context;
import android.content.Intent;
import android.content.res.Resources;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RelativeLayout;

import com.adapter.Adapter;
import com.android.qrreader.R;
import com.qrcode.qrcodes.MailQrCode;

public class MailQrCodeView  implements Adapter {
    public class MailQrCodeViewProvider implements QrCodeViewProvider {
        private MailQrCode qrCode;
        
        private Context context;
        private RelativeLayout resultView;
        private EditText receiverEdit;
        private EditText subjectEdit;
        private EditText bodyEdit;
        
        private OnClickListener onClickSendMail = new OnClickListener() {
            @Override
            public void onClick(View v) {
                final Intent intent = new Intent(android.content.Intent.ACTION_SEND);
                
                intent.setType("plain/text");
                intent.putExtra(android.content.Intent.EXTRA_EMAIL, new String[]{ receiverEdit.getText().toString()});
                intent.putExtra(android.content.Intent.EXTRA_SUBJECT, subjectEdit.getText());
                intent.putExtra(android.content.Intent.EXTRA_TEXT, bodyEdit.getText());

                Resources res = context.getResources();
                context.startActivity(Intent.createChooser(intent,  res.getString(R.string.QrCode_Title_Mail_Send)));                
            }
        };
                
        private MailQrCodeViewProvider(MailQrCode qrCode) {
            this.qrCode = qrCode;
        }
        
        public View getView(Context context) {
            if (resultView == null) { // Instantiate just once per this instance of the provider
                this.context = context;
                LayoutInflater resultInflater = LayoutInflater.from(context);
                resultView = (RelativeLayout) resultInflater.inflate(R.layout.openqr_view_mail, null);
                
                receiverEdit = (EditText)resultView.findViewById(R.id.receiverEdit);
                subjectEdit = (EditText)resultView.findViewById(R.id.subjectEdit);
                bodyEdit = (EditText)resultView.findViewById(R.id.bodyEdit);
                Button sendMail = (Button) resultView.findViewById(R.id.sendMail);
                
                sendMail.setOnClickListener(onClickSendMail);
                
                subjectEdit.setText(qrCode.getSubject());
                receiverEdit.setText(qrCode.getReceiver());                
                bodyEdit.setText(qrCode.getBody());
            }
            
            return resultView;
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
