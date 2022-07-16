package com.example.smartapp;

import androidx.appcompat.app.AppCompatActivity;
import androidx.viewpager.widget.ViewPager;

import android.annotation.SuppressLint;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;
import org.json.JSONException;
import org.json.JSONObject;

import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;
import java.util.concurrent.Executor;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

public class SmartHomePage extends AppCompatActivity {
    private String host = "tcp://mqtt.ddeng.xyz";
    private String userName = "android";
    private String passWord = "android";
    private String mqtt_id = "1503172433";
    private ImageView image_1;
    private ImageView image_2;
    private ImageView image_3;
    private TextView text_temp;
    private TextView text_humi;
    private TextView text_light;
    private MqttClient client;
    private ScheduledExecutorService scheduler;
    private MqttConnectOptions options;
    private Handler handler;
    private String mqtt_sub_topic = "/mysmarthome/sub";
    private String mqtt_pub_topic = "/mysmarthome/pub";
    private int led_flag=1;
    private int alarm_flag=1;
    private int fs_flag=1;

    @SuppressLint("HandlerLeak")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_smart_home_page);
        image_1=findViewById(R.id.image_1);
        image_1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Toast.makeText(SmartHomePage.this,"led",Toast.LENGTH_SHORT).show();
                if (led_flag==0) {
                    publishmessageplus(mqtt_sub_topic,"{\n"+"\t\"target\":\"LED\",\n"+"\t\"value\":1}");
                    led_flag=1;
                }
                else{
                    publishmessageplus(mqtt_sub_topic,"{\n"+"\t\"target\":\"LED\",\n"+"\t\"value\":0}");
                    led_flag=0;
                }
            }
        });
        image_2=findViewById(R.id.image_2);
        image_2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Toast.makeText(SmartHomePage.this,"beep",Toast.LENGTH_SHORT).show();
                if (alarm_flag==0) {
                    publishmessageplus(mqtt_sub_topic,"{\n"+"\t\"target\":\"BEEP\",\n"+"\t\"value\":1}");
                    alarm_flag=1;
                }
                else{
                    publishmessageplus(mqtt_sub_topic,"{\n"+"\t\"target\":\"BEEP\",\n"+"\t\"value\":0}");
                    alarm_flag=0;
                }
            }
        });
        image_3=findViewById(R.id.image_3);
        image_3.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Toast.makeText(SmartHomePage.this,"fs",Toast.LENGTH_SHORT).show();
                if (fs_flag==0) {
                    publishmessageplus(mqtt_sub_topic,"{\n"+"\t\"target\":\"FS\",\n"+"\t\"value\":1}");
                    fs_flag=1;
                }
                else{
                    publishmessageplus(mqtt_sub_topic,"{\n"+"\t\"target\":\"FS\",\n"+"\t\"value\":0}");
                    fs_flag=0;
                }
            }
        });
        text_temp = findViewById(R.id.text_temp);
        text_humi = findViewById(R.id.text_humi);
        text_light = findViewById(R.id.text_light);
        Mqtt_Init();
        startReconnect();
        handler = new Handler(){
            public void handleMessage(Message msg){
                super.handleMessage(msg);
                switch (msg.what){
                    case 1:
                        break;
                    case 2:
                        break;
                    case 3:
                        String[] str = msg.obj.toString().split("\\,");
                        for (int i = 0; i < str.length; i++)
                            System.out.println(str[i]);
                        String T_val = str[1].substring
                                (str[1].indexOf("Temp\":")+6);
                        //Toast.makeText(SmartHomePage.this,str[0],Toast.LENGTH_SHORT).show();
                        String text_temp_val = "温度："+T_val+"℃";
                        String H_val = str[0].substring
                                (str[0].indexOf("Humi\":")+6);
                        String text_humi_val = "湿度："+H_val;
                        String L_val = str[2].substring
                                (str[2].indexOf("Light\":")+7);
                        String text_light_val = "光照度："+L_val;
                        String Led_val = str[3].substring
                                (str[3].indexOf("Led\":")+5);
                        String B_val = str[4].substring
                                (str[4].indexOf("Beep\":")+6);
                        String F_val = str[5].substring
                                (str[5].indexOf("Fs\":")+4,str[5].indexOf("}"));
                        text_temp.setText(text_temp_val);
                        text_temp.setTextColor(Color.BLACK);
                        text_humi.setText(text_humi_val);
                        text_humi.setTextColor(Color.BLACK);
                        text_light.setText(text_light_val);
                        text_light.setTextColor(Color.BLACK);
                        //Toast.makeText(SmartHomePage.this,T_val,Toast.LENGTH_SHORT).show();
                        break;
                    case 30:
                        Toast.makeText(SmartHomePage.this,"连接失败",Toast.LENGTH_SHORT).show();
                        break;
                    case 31:
                            Toast.makeText(SmartHomePage.this,"连接成功",Toast.LENGTH_SHORT).show();
                        try {
                            client.subscribe(mqtt_pub_topic,0);
                        } catch (MqttException e) {
                            e.printStackTrace();
                        }
                        break;
                    default:
                        break;
                }
            }
        };
    }
    private void Mqtt_Init()
    {
        try {
            client = new MqttClient(host,mqtt_id,new MemoryPersistence());
            options = new MqttConnectOptions();
            options.setCleanSession(false);
            options.setUserName(userName);
            options.setPassword(passWord.toCharArray());
            options.setConnectionTimeout(10);
            options.setKeepAliveInterval(20);
            client.setCallback(new MqttCallback() {
                @Override
                public void connectionLost(Throwable throwable) {
                    System.out.println("connectionLost----------");
                }

                @Override
                public void messageArrived(String topicName, MqttMessage mqttMessage) throws Exception {
                    System.out.println("Message Arrived----------");
                    Message msg = new Message();
                    msg.what = 3;
                    msg.obj = topicName + "---" + mqttMessage.toString();
                    handler.sendMessage(msg);
                }

                @Override
                public void deliveryComplete(IMqttDeliveryToken iMqttDeliveryToken) {
                    System.out.println("deliveryComplete----------"+ iMqttDeliveryToken.isComplete());
                }
            });
        }catch (Exception e) {
            e.printStackTrace();
        }
    }
    private void Mqtt_connect() {
        new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    if (!(client.isConnected())) {
                        client.connect(options);
                        Message msg = new Message();
                        msg.what = 31;
                        handler.sendMessage(msg);
                    }
                }catch (Exception e){
                    e.printStackTrace();
                    Message msg = new Message();
                    msg.what = 30;
                    handler.sendMessage(msg);
                }
            }
        }).start();
    }
    private void startReconnect(){
        scheduler = Executors.newSingleThreadScheduledExecutor();
        scheduler.scheduleAtFixedRate(new Runnable() {
            @Override
            public void run() {
                if (!client.isConnected())
                {
                    Mqtt_connect();
                }
            }
        },0*1000,10*1000,TimeUnit.MILLISECONDS);
    }
    private void publishmessageplus(String topic, String message2){
        if (client==null || !client.isConnected()){
            return;
        }
        MqttMessage message = new MqttMessage();
        message.setPayload(message2.getBytes());
        try {
            client.publish(topic,message);
        }catch (MqttException e){
            e.printStackTrace();
        }
    }
    /*public void parseJsonData(String strJson){
        System.out.println(3);
        try {
            //System.out.println(4);
            JSONObject jsonObject = new JSONObject(strJson);
            System.out.println(4);
            String T_val = jsonObject.optString("Temp");
            String H_val = jsonObject.optString("Humi");
            String L_val = jsonObject.optString("Light");
            text_temp.setText(T_val);
            //text_temp.setTextColor(Color.BLACK);
            text_humi.setText(H_val);
           // text_humi.setTextColor(Color.BLACK);
            text_light.setText(L_val);
        } catch (JSONException e) {
            e.printStackTrace();
        }

    }*/
}