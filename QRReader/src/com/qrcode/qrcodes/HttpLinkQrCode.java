package com.qrcode.qrcodes;

import java.net.URL;

public class HttpLinkQrCode extends QrCode {    
    final private static String HTTP_SCHEMA = "http";
    
    private URL link;

    public boolean setLink(URL link) {
        if (link.getProtocol().equalsIgnoreCase(HTTP_SCHEMA)) {
            this.link = link;
            return true;
        } else {
            return false;
        }
    }
    
    public URL getLink() {
        return link;
    }
    
    @Override
    public String toString() {
        return link.toExternalForm();
    }
}
