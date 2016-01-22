package ece_protect.e_protect;

import android.content.Context;
import android.content.Intent;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.text.InputType;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.webkit.WebView;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.EditText;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;


public class SettingsPage extends ActionBarActivity {

    static public String EMAIL = "";
    static public String PASSWORD = "";

    public static void loadData(Context context){
        File file = new File(context.getFilesDir(),"password.dt");

        if(file.exists()) {
            //Read text from file
            StringBuilder text = new StringBuilder();

            try {
                BufferedReader br = new BufferedReader(new FileReader(file));
                String line;

                while ((line = br.readLine()) != null) {
                    text.append(line);
                    text.append('\n');
                }
            } catch (IOException e) {
                //You'll need to add proper error handling here
            }

            String dt = text.toString();

            String[] splitDT = dt.split(":");

            if(splitDT.length == 2){
                EMAIL = splitDT[0];
                PASSWORD = splitDT[1];
            }
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_settings_page);

        Button connectB = (Button)findViewById(R.id.button_login);
        EditText email = (EditText)findViewById(R.id.email_login);
        EditText pass = (EditText)findViewById(R.id.password_login);

        loadData(this);

        email.setText(EMAIL);
        if(PASSWORD != "")
            pass.setText("******");


        CheckBox passCh = (CheckBox)findViewById(R.id.password_check);

        passCh.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                EditText pass = (EditText)findViewById(R.id.password_login);
                if(isChecked){
                    pass.setInputType(InputType.TYPE_CLASS_TEXT | InputType.TYPE_TEXT_VARIATION_VISIBLE_PASSWORD);
                }else{
                    pass.setInputType(InputType.TYPE_CLASS_TEXT | InputType.TYPE_TEXT_VARIATION_PASSWORD);
                }
            }
        });

        connectB.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                File file = new File(getFilesDir(), "password.dt");
                file.delete();

                String email = ((EditText) findViewById(R.id.email_login)).getText().toString();
                String pass = ((EditText)findViewById(R.id.password_login)).getText().toString();

                try {
                    MessageDigest digest = MessageDigest.getInstance("SHA-256");
                    MessageDigest salt = MessageDigest.getInstance("MD5");

                    String toHash = ToHex(pass.getBytes("UTF-8"))+ToHex(salt.digest(pass.getBytes("UTF-8")));

                    String passHashed = ToHex(digest.digest(toHash.getBytes("UTF-8")));

                    FileOutputStream outputStream;

                    outputStream = openFileOutput("password.dt", MODE_PRIVATE);
                    outputStream.write((email+":"+passHashed).getBytes());
                    outputStream.close();


                    EMAIL = email;
                    PASSWORD = passHashed;

                }catch (Exception e){
                    e.printStackTrace();
                }
            }
        });

    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_settings_page, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        int id = item.getItemId();
        Intent intent;
        switch(id){
            case R.id.action_home:
                intent = new Intent(SettingsPage.this, HomePage.class);
                startActivity(intent);
                finish();
                break;
            case R.id.action_settings:
                break;
            default:
                break;
        }
        return super.onOptionsItemSelected(item);
    }

    static public String ToHex(byte[] byteData){
        StringBuffer sb = new StringBuffer();
        for (int i = 0; i < byteData.length; i++) {
            sb.append(Integer.toString((byteData[i] & 0xff) + 0x100, 16).substring(1));
        }
        return sb.toString();
    }
}
