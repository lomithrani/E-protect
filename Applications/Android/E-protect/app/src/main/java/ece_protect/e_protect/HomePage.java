package ece_protect.e_protect;

import android.content.Intent;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.webkit.WebSettings;
import android.webkit.WebView;


public class HomePage extends ActionBarActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_home_page);

        SettingsPage.loadData(this);

        WebView wv = (WebView)findViewById(R.id.MainWebView);

        wv.loadUrl("http://projet1.valentis.fr/recherche/"+SettingsPage.EMAIL+":"+SettingsPage.PASSWORD);

        wv.setWebViewClient(new MyWebViewClient());

        WebSettings webSettings = wv.getSettings();
        webSettings.setJavaScriptEnabled(true);

    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_home_page, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        int id = item.getItemId();
        Intent intent;
        switch(id){
            case R.id.action_home:
                WebView wv = (WebView)findViewById(R.id.MainWebView);
                wv.loadUrl("http://projet1.valentis.fr");
                break;
            case R.id.action_settings:
                intent = new Intent(HomePage.this, SettingsPage.class);
                startActivity(intent);
                finish();
                break;
            default:
                break;
        }
        return super.onOptionsItemSelected(item);
    }
}
