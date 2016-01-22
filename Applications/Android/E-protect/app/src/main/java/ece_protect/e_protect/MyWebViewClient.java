package ece_protect.e_protect;

import android.net.Uri;
import android.webkit.WebViewClient;
import android.webkit.WebView;

public class MyWebViewClient extends WebViewClient {

    @Override
    public boolean shouldOverrideUrlLoading(WebView view, String url) {
        if (Uri.parse(url).getHost().equals("projet1.valentis.fr")) {
            // This is my web site, so do not override; let my WebView load the page
            return false;
        }
        return true;
    }
}