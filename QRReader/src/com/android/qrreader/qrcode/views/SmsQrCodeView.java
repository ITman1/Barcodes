///////////////////////////////////////////////////////////////////////////////
// Project:    QR Reader for Android
// Package:    com.android.qrreader.qrcode.views
// File:       SmsQrCodeView.java
// Date:       March 2012
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Contains the view adapter for adapting the SMS QR code
//             to the proper view.
///////////////////////////////////////////////////////////////////////////////

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
import com.android.qrreader.MainActivity;
import com.android.qrreader.OpenQrActivity;
import com.android.qrreader.OpenQrActivity.OnPauseCallback;
import com.android.qrreader.R;
import com.qrcode.qrcodes.SmsQrCode;

/**
 * The Class SmsQrCodeView adapts the SMS QR code to the proper view.
 * 
 * @see com.qrcode.qrcodes.SmsQrCode SmsQrCode
 * @see com.adapter.Adapter Adapter
 * @see QrCodeViewProvider
 * 
 * @version 1.0
 */
public class SmsQrCodeView  implements Adapter {
    
    /**
     * This class implements the provider of view of the adapted SMS QR code. 
     * 
     * @see QrCodeViewProvider
     * 
     * @version 1.0
     */
    public class SmsQrCodeViewProvider implements QrCodeViewProvider {
        
        /** The intent action of the sending SMS. */
        final public static String ACTION_SMS_SENT = "com.android.qrreader.qrcode.views.SmsQrCodeView.SmsQrCodeViewProvider.SMS_SENT_ACTION";

        /** The QR code which is being adapted. */
        private SmsQrCode qrCode;
        
        /** The context. */
        private Context context;
        
        /** The result view. */
        private RelativeLayout resultView;
        
        /** The SMS receiver edit. */
        private EditText receiverEdit;
        
        /** The SMS body edit. */
        private EditText bodyEdit;
        
        /** The view with the progress bar which is displayed during the sending SMS. */
        private RelativeLayout sendingView;
        
        /** The callback which is called when the activity is paused. */
        private OnPauseCallback onPauseCallback = new OnPauseCallback() {
            public void onPause() {
                context.unregisterReceiver(broadcastReceiver);
                ((OpenQrActivity)context).releaseOnPauseCallback(this);
            }
        };
        
        /** The broadcast receiver which captures the result of the sent SMS. */
        private BroadcastReceiver broadcastReceiver = new BroadcastReceiver() {
            @Override
            public void onReceive(Context context, Intent intent) {
                String message = null;
                boolean error = true;
                
                // SMS sent - unregistering the broadcast receiver
                context.unregisterReceiver(broadcastReceiver);
                ((OpenQrActivity)context).releaseOnPauseCallback(onPauseCallback);
                
                // Removing the progress bar and returning the normal view
                LinearLayout resultParent = (LinearLayout)sendingView.getParent();
                resultParent.removeView(sendingView);
                resultParent.addView(resultView);
                
                // handling SMS sent errors - taken exactly from the demo
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
                                   
                // Preparing the icon and result text for user notifying
                int icon = (error)? android.R.drawable.stat_notify_error : android.R.drawable.sym_action_email;
                int statusMsg = (error)? R.string.QrCode_Title_Sms_Sending_Error : R.string.QrCode_Title_Sms_Sending_Success;
                CharSequence title = context.getResources().getString(statusMsg);

                // Getting notify manager and sending notify to the user
                NotificationManager manager = (NotificationManager)context.getSystemService(Context.NOTIFICATION_SERVICE);
                PendingIntent contentIntent = PendingIntent.getActivity(context, 0, intent, 0);
                Notification notification = new Notification(icon, title, System.currentTimeMillis());
                notification.setLatestEventInfo(context, title, message, contentIntent);
                manager.notify(1, notification);
                
                // When there is no error, it is not any reason to keep this message in the manager
                if (!error) manager.cancel(1);
            }
        };
        
        /** The listener of the click on the send SMS button. */
        private OnClickListener onClickSendSms = new OnClickListener() {
            @Override
            public void onClick(View v) {
                
                // Getting references to SMS manager, body and receiver
                SmsManager smsManager = SmsManager.getDefault();
                List<String> messages = smsManager.divideMessage(bodyEdit.getText().toString());
                String receiver = receiverEdit.getText().toString();
                
                if (receiver.length() > 0 && messages.size() > 0) { // Ready to send
                    
                    // Registering the broadcast receiver for capturing sent event
                    context.registerReceiver(broadcastReceiver, new IntentFilter(ACTION_SMS_SENT));
                    ((OpenQrActivity)context).registerOnPauseCallback(onPauseCallback);
                    
                    // Inflating progress bar view and replacing the resultView
                    LayoutInflater resultInflater = LayoutInflater.from(context);
                    sendingView = (RelativeLayout) resultInflater.inflate(R.layout.openqr_view_sms_sending, null);
                    LinearLayout resultParent = (LinearLayout)resultView.getParent();
                    resultParent.removeView(resultView);
                    resultParent.addView(sendingView);
                    
                    // Sending the SMSs
                    for (String message : messages) {
                        smsManager.sendTextMessage(receiver, null, message, PendingIntent.getBroadcast(
                                context, 0, new Intent(ACTION_SMS_SENT), 0), null);
                    }
                } else if (receiver.length() == 0) {                // No receiver
                    MainActivity.showErrorAlert(context, R.string.QrCode_Title_Sms_Sending_NoReciever);
                } else {                                            // No body
                    MainActivity.showErrorAlert(context, R.string.QrCode_Title_Sms_Sending_NoBody);
                }
            }

        };
                
        /**
         * Instantiates a new SMS QR code view provider.
         *
         * @param qrCode The QR code to be adapted.
         */
        private SmsQrCodeViewProvider(SmsQrCode qrCode) {
            this.qrCode = qrCode;
        }
        
        /** 
         * Gets the view for SMS QR code.
         * 
         * @see com.android.qrreader.qrcode.views.QrCodeViewProvider#getView(android.content.Context) getView(Context)
         */
        public View getView(Context context) {
            if (resultView == null) { // Instantiate just once per this instance of the provider
                this.context = context;
                LayoutInflater resultInflater = LayoutInflater.from(context);
                resultView = (RelativeLayout) resultInflater.inflate(R.layout.openqr_view_sms, null);
                
                // Getting element references and setting sent button receiver
                receiverEdit = (EditText)resultView.findViewById(R.id.receiverEdit);
                bodyEdit = (EditText)resultView.findViewById(R.id.bodyEdit);
                Button sendSms = (Button) resultView.findViewById(R.id.sendSms);
                sendSms.setOnClickListener(onClickSendSms);

                // Setting QR code values
                receiverEdit.setText(qrCode.getReceiver());                
                bodyEdit.setText(qrCode.getBody());
            }
            
            return resultView;
        }
        
        /**
         * Gets the title name for SMS QR code.
         * 
         * @see com.android.qrreader.qrcode.views.QrCodeViewProvider#getTitleName(Context)
         */
        public String getTitleName(Context context) {
            Resources res = context.getResources();
            return res.getString(R.string.QrCode_Title_Sms);
        }
    };
    
    /**
     * Gets the view provider for SMS QR code.
     * 
     * @see com.adapter.Adapter#getProvider(java.lang.Object) getProvider(Object)
     */
    @Override
    public Object getProvider(Object o) {
        return new SmsQrCodeViewProvider((SmsQrCode) o);
    }
    
    /**
     * Gets the SmsQrCode class.
     * 
     * @see com.adapter.Adapter#getAdapteeClass() getAdapteeClass()
     */
    @Override
    public Class<?> getAdapteeClass() {
        return SmsQrCode.class;
    }
    
    /**
     * Gets the QrCodeViewProvider class.
     * 
     * @see com.adapter.Adapter#getResultClass() getResultClass()
     */
    @Override
    public Class<?> getResultClass() {
        return QrCodeViewProvider.class;
    }
}

//End of the file
