package com.example.smartapp;

import androidx.appcompat.app.AppCompatActivity;

import android.content.ContentValues;
import android.content.Intent;
import android.database.sqlite.SQLiteDatabase;
import android.os.Bundle;
import android.util.Log;
import android.view.ContentInfo;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import com.example.smartapp.db.MyHelper;

public class RegisterActivity extends AppCompatActivity {
    private EditText EtUserName;
    private EditText EtUserPsd;
    private Button btn_Register;
    MyHelper myHelper;
    SQLiteDatabase db;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_register);
        EtUserName = findViewById(R.id.Ed_Account_Reg);
        EtUserPsd = findViewById(R.id.Ed_PassWord_Reg);
        btn_Register = findViewById(R.id.btn_register);
        myHelper = new MyHelper(getApplicationContext());
        db = myHelper.getWritableDatabase();
        btn_Register.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String name = EtUserName.getText().toString();
                String psd = EtUserPsd.getText().toString();
                ContentValues cv = new ContentValues();
                cv.put("user_id",name);
                cv.put("user_psd",psd);
                db.insert("user",null,cv);
                Log.e("---Insert---","Insert");
                Toast.makeText(getApplicationContext(), "注册成功", Toast.LENGTH_SHORT).show();
                Intent intent = new Intent(getApplicationContext(),LoginActivity.class);
                startActivity(intent);
            }
        });
    }
}