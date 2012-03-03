package com.android.qrreader.qrcode.views;

import java.util.List;

import android.app.Activity;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.res.Resources;
import android.telephony.SmsManager;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;

import com.adapter.Adapter;
import com.android.qrreader.OpenQrActivity;
import com.android.qrreader.OpenQrActivity.OnPauseCallback;
import com.android.qrreader.R;
import com.qrcode.qrcodes.SmsQrCode;

public class SmsQrCodeView  implements Adapter {
    public class SmsQrCodeViewProvider implements QrCodeViewProvider {
        final public static String ACTION_SMS_SENT = "com.android.qrreader.qrcode.views.SmsQrCodeView.SmsQrCodeViewProvider.SMS_SENT_ACTION";

        private SmsQrCode qrCode;
        private Context context;
        
        private RelativeLayout resultView;
        private EditText receiverEdit;
        private EditText bodyEdit;
        
        private OnClickListener onClickSendSms = new OnClickListener() {
                        
            private RelativeLayout sendingView;
            
            private OnPauseCallback onPauseCallback = new OnPauseCallback() {
                public void onPause() {
                    context.unregisterReceiver(broadcastReceiver);
                    ((OpenQrActivity)context).releaseOnPauseCallback(this);
                }
            };
            
            private BroadcastReceiver broadcastReceiver = new BroadcastReceiver() {
                @Override
                public void onReceive(Context context, Intent intent) {
                    String message = null;
                    boolean error = true;
                    
                    context.unregisterReceiver(broadcastReceiver);
                    ((OpenQrActivity)context).releaseOnPauseCallback(onPauseCallback);
                    LinearLayout resultParent = (LinearLayout)sendingView.getParent();
                    resultParent.removeView(sendingView);
                    resultParent.addView(resultView);
                    
                    // some errors taken exactly from the
                    // http://developer.android.com/resources/samples/ApiDemos/src/com/example/android/apis/os/SmsMessagingDemo.html
                    switch (getResultCode()) {
                    case Activity.RESULT_OK:
                        message = "Message sent!";
                        error = false;
                        break;
                    case SmsManager.RESULT_ERROR_GENERIC_FAILURE:
                        message = "Error: Probably bad number.";
                        break;
                    case SmsManager.RESULT_ERROR_NO_SERVICE:
                        message = "Error: No service.";
                        break;
                    case SmsManager.RESULT_ERROR_NULL_PDU:
                        message = "Error: Null PDU.";
                        break;
                    case SmsManager.RESULT_ERROR_RADIO_OFF:
                        message = "Error: Radio off.";
                        break;
                    }
                                       
                    int icon = (error)? android.R.drawable.stat_notify_error : android.R.drawable.sym_action_email;
                    int statusMsg = (error)? R.string.QrCode_Title_Sms_Sending_Error : R.string.QrCode_Title_Sms_Sending_Success;
                    CharSequence title = context.getResources().getString(statusMsg);

                    NotificationManager manager = (NotificationManager)context.getSystemService(Context.NOTIFICATION_SERVICE);
                    PendingIntent contentIntent = PendingIntent.getActivity(context, 0, intent, 0);
                    Notification notification = new Notification(icon, title, System.currentTimeMillis());
                    notification.setLatestEventInfo(context, title, message, contentIntent);
                    manager.notify(1, notification);
                    
                    if (!error) manager.cancel(1);
                }
            };
            
            @Override
            public void onClick(View v) {
                SmsManager smsManager = SmsManager.getDefault();
                List<String> messages = smsManager.divideMessage(bodyEdit.getText().toString());

                String receiver = receiverEdit.getText().toString();
                context.registerReceiver(broadcastReceiver, new IntentFilter(ACTION_SMS_SENT));
                ((OpenQrActivity)context).registerOnPauseCallback(onPauseCallback);
                
                if (receiver.length() > 0 && messages.size() > 0) {
                    LayoutInflater resultInflater = LayoutInflater.from(context);
                    sendingView = (RelativeLayout) resultInflater.inflate(R.layout.openqr_view_sms_sending, null);
                    
                    LinearLayout resultParent = (LinearLayout)resultView.getParent();
                    resultParent.removeView(resultView);
                    resultParent.addView(sendingView);
                    for (String message : messages) {
                        smsManager.sendTextMessage(receiver, null, message, PendingIntent.getBroadcast(
                                context, 0, new Intent(ACTION_SMS_SENT), 0), null);
                    }
                } else if (receiver.length() == 0) { // No receiver
                    ;
                } else { // No body
                    ;
                }
            }

        };
                
        private SmsQrCodeViewProvider(SmsQrCode qrCode) {
            this.qrCode = qrCode;
        }
        
        public View getView(Context context) {
            if (resultView == null) { // Instantiate just once per this instance of the provider
                this.context = context;
                LayoutInflater resultInflater = LayoutInflater.from(context);
                resultView = (RelativeLayout) resultInflater.inflate(R.layout.openqr_view_sms, null);
                
                receiverEdit = (EditText)resultView.findViewById(R.id.receiverEdit);
                bodyEdit = (EditText)resultView.findViewById(R.id.bodyEdit);
                Button sendSms = (Button) resultView.findViewById(R.id.sendSms);
                
                sendSms.setOnClickListener(onClickSendSms);
                
                receiverEdit.setText(qrCode.getReceiver());                
                bodyEdit.setText(qrCode.getBody());
            }
            
            return resultView;
        }
        
        public String getTitleName(Context context) {
            Resources res = context.getResources();
            return res.getString(R.string.QrCode_Title_Sms);
        }
    };
    
    @Override
    public Object getProvider(Object o) {
        return new SmsQrCodeViewProvider((SmsQrCode) o);
    }
    
    @Override
    public Class<?> getAdapteeClass() {
        return SmsQrCode.class;
    }
    
    @Override
    public Class<?> getResultClass() {
        return View.class;
    }
}
