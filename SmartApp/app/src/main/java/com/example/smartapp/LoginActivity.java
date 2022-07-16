package com.example.smartapp;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.example.smartapp.db.MyHelper;

import org.json.JSONObject;

import java.io.Console;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

public class LoginActivity extends AppCompatActivity{
    private EditText EtUserName;
    private EditText EtUserPsd;
    private Button btn_Login;
    private TextView TvRegister;
    MyHelper myHelper;
    SQLiteDatabase db;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);
        EtUserName = findViewById(R.id.Ed_Account_Log);
        EtUserPsd = findViewById(R.id.Ed_PassWord_Log);
        btn_Login = findViewById(R.id.btn_login);
        myHelper = new MyHelper(getApplicationContext());
        db = myHelper.getReadableDatabase();
        btn_Login.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Cursor c = db.query("user",null,null,null,
                        null,null,null);
                c.moveToFirst();
                while(!c.isAfterLast()){
                    int Index_Id = c.getColumnIndex("user_id");
                    int Index_Psd = c.getColumnIndex("user_psd");
                    if (Index_Id>=0&&Index_Psd>=0)
                    {
                        String name = c.getString(Index_Id);
                        String psd = c.getString(Index_Psd);
                        if (name.equals(EtUserName.getText().toString())&&psd.equals(EtUserPsd.getText().toString()))
                        {
                            Toast.makeText(getApplicationContext(), "登录成功", Toast.LENGTH_SHORT).show();
                            Intent intent = new Intent(getApplicationContext(),SmartHomePage.class);
                            startActivity(intent);
                        }
                        c.moveToNext();
                    }
                    else
                    {
                        Toast.makeText(getApplicationContext(), "查询失败", Toast.LENGTH_SHORT).show();
                    }
                }
                db.close();
            }
        });
    }
}