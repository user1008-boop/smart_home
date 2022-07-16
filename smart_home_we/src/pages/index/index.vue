<template>
  <div class="wrapper">
    <div class="header-wrapper">
      <div class="header-title">
        <span>空气质量-{{ airText }}</span>
        <span>{{ city }}-{{ area }}</span>
      </div>
      <div class="header-text">
        <span>{{ airValue }}</span>
        <span>{{ weather }}</span>
      </div>
      <div class="header-advice">
        {{ weatherAdivice }}
      </div>
    </div>
    <div class="body-wrapper">
      <div class="body">
        <div class="data-wrapper">
          <div class="data">
            <img class="data-logo" src="/static/images/temp.png" />
            <div class="data-text">
              <div class="data-title">温度</div>
              <div class="data-value">{{ Temp }}℃</div>
            </div>
          </div>
          <div class="data">
            <img class="data-logo" src="/static/images/humi.png" />
            <div class="data-text">
              <div class="data-title">湿度</div>
              <div class="data-value">{{ Humi }}%</div>
            </div>
          </div>
        </div>
        <div class="data-wrapper">
          <div class="data">
            <img class="data-logo" src="/static/images/sun.png" />
            <div class="data-text">
              <div class="data-title">光照</div>
              <div class="data-value">{{ Light }}Lux</div>
            </div>
          </div>
          <div class="data">
            <img class="data-logo" src="/static/images/led.png" />
            <div class="data-text">
              <div class="data-title">电灯</div>
              <div class="data-value">
                <switch @change="onLedchange" :checked="Led" color="#3d7ef9" />
              </div>
            </div>
          </div>
        </div>
        <div class="data-wrapper">
          <div class="data">
            <img class="data-logo" src="/static/images/beep.png" />
            <div class="data-text">
              <div class="data-title">报警</div>
              <div class="data-value">
                <switch
                  @change="onBeepchange"
                  :checked="Beep"
                  color="#3d7ef9"
                />
              </div>
            </div>
          </div>
          <div class="data">
            <img class="data-logo" src="/static/images/fs.png" />
            <div class="data-text">
              <div class="data-title">风扇</div>
              <div class="data-value">
                <switch @change="onFschange" :checked="Fs" color="#3d7ef9" />
              </div>
            </div>
          </div>
        </div>
      </div>
    </div>
    <div class="foot_wrapper">
      <img class="time-logo" src="/static/images/clock.png"/>
      <div class="time_wrapper">
        <span>登录时间：{{loginYear}}/{{loginHour}}/{{loginDay}}/{{loginHour}}:{{loginMin}}</span>
      </div>
    </div>
  </div>
</template>

<script>
import { connect } from "mqtt/dist/mqtt.js";
const mqtturl = "wxs://mqtt.ddeng.xyz:8084/mqtt";
const mobile ="19375152086";
const content ="您好家中疑似有危险情况发生请及时查看！";
const connectY="云平台连接成功";
const connectN="云平台连接失败";
var sendCount=0;
export default {
  data() {
    return {
      client: {},
      Temp: 0,
      Humi: 0,
      Light: 0,
      Led: false,
      Beep: false,
      Fs: false,
      area: "请求中",
      city: "请求中",
      airText: "请求中",
      airValue: 0,
      weather: "请求中",
      weatherAdivice: "请求中",
      //loginTime:"请求中",
      loginYear:"请求中",
      loginMonth:"请求中",
      loginDay:"请求中",
      loginHour:"请求中",
      loginMin:"请求中",
    };
  },

  components: {},

  methods: {
    onLedchange(event) {
      var that = this;
      console.log(event.mp.detail);
      let sw = event.mp.detail.value;
      that.Led = sw;
      if (sw) {
        that.client.publish(
          "/mysmarthome/sub",
          '{"target":"LED","value":1}',
          function (err) {
            if (!err) {
              console.log("成功下发命令_开灯");
            }
          }
        );
      } else {
        that.client.publish(
          "/mysmarthome/sub",
          '{"target":"LED","value":0}',
          function (err) {
            if (!err) {
              console.log("成功下发命令_关灯");
            }
          }
        );
      }
    },
    onBeepchange(event) {
      var that = this;
      console.log(event.mp.detail);
      let sw = event.mp.detail.value;
      that.Beep = sw;
      if (sw) {
        that.client.publish(
          "/mysmarthome/sub",
          '{"target":"BEEP","value":1}',
          function (err) {
            if (!err) {
              console.log("成功下发命令_开始报警");
            }
          }
        );
      } else {
        that.client.publish(
          "/mysmarthome/sub",
          '{"target":"BEEP","value":0}',
          function (err) {
            if (!err) {
              console.log("成功下发命令_关闭报警");
            }
          }
        );
      }
    },
    onFschange(event) {
      var that = this;
      console.log(event.mp.detail);
      let sw = event.mp.detail.value;
      that.Fs = sw;
      if (sw) {
        that.client.publish(
          "/mysmarthome/sub",
          '{"target":"FS","value":1}',
          function (err) {
            if (!err) {
              console.log("成功下发命令_打开风扇");
            }
          }
        );
      } else {
        that.client.publish(
          "/mysmarthome/sub",
          '{"target":"FS","value":0}',
          function (err) {
            if (!err) {
              console.log("成功下发命令_关闭风扇");
            }
          }
        );
      }
    },
  },
  onShow() {
    var that = this;
    that.loginYear=new Date().getFullYear();
    that.loginMonth=new Date().getMonth();
    that.loginDay=new Date().getDay();
    that.loginHour=new Date().getHours();
    that.loginMin=new Date().getMinutes();
    //that.loginTime=loginMonth+'月'+loginDay+'日'+loginHour+'时'+loginMin+'分'+loginSec+'秒';
    that.client = connect(mqtturl);
    that.client.on("connect", function () {
      console.log("成功链接mqtt服务器！");
      that.client.subscribe("/mysmarthome/pub", function (err) {
        if (!err) {
          console.log("成功订阅设备上行topic");
          that.weatherAdivice = connectY;
        }
        else{
          that.weatherAdivice = connectN;
        }
      });
    });
    that.client.on("message", function (topic, message) {
      console.log(topic);
      let DataFormDev = {};
      DataFormDev = JSON.parse(message);
      console.log(DataFormDev);
      that.Temp = DataFormDev.Temp;
      that.Humi = DataFormDev.Humi;
      that.Light = DataFormDev.Light;
      that.Led = DataFormDev.Led;
      that.Beep = DataFormDev.Beep;
      that.Fs = DataFormDev.Fs;
      if (that.Beep === 1&&sendCount===0) {
        console.log("开始报警了")
        wx.request({
            url: `https://api.smsbao.com/sms?u=dengdengdeng&p=3f473801e4db4ac3bfb596ee73593f29&m=${mobile}&c=${content}`, //获取天气数据API接口
            success (res) {
               console.log(res.data)
              if(res.data === 0){
                console.log('短信发送成功')
                sendCount++;
              }
            }
        });
        }
    });
    /*
    wx.getLocation({
      type: 'wgs84',
      success (res) {
        const latitude = res.latitude;
        const longitude = res.longitude;
        const key = '80a2fb89f6bd4fa7814b529669872a18'
        wx.request({
           url: `https://free-api.heweather.net/s6/weather/now?location=${longitude},${latitude}&key=${key}`, //获取天气数据API接口
          success (res) {
            //console.log(res.data)
            const {basic,now} = res.data.HeWeather6[0];
            //console.log(basic)
           // console.log(now)
            that.area = basic.location
            that.city = basic.parent_city
            that.weather = now.cond_txt
            wx.request({
            url: `https://free-api.heweather.net/s6/air/now?location=${that.city}&key=${key}`, //获取天气数据API接口
            success (res) {
              // console.log(res.data)
               const{air_now_city} = res.data.HeWeather6[0];
               //console.log(air_now_city)
               const{aqi,qlty} = air_now_city
              that.airText = qlty
              that.airValue = aqi
            }
           });
           }
        });
        wx.request({
            url: `https://free-api.heweather.net/s6/weather/lifestyle?location=${longitude},${latitude}&key=${key}`, //获取天气数据API接口
            success (res) {
               //console.log(res.data)
               const{lifestyle} = res.data.HeWeather6[0];
               console.log(lifestyle)
               that.weatherAdivice = lifestyle[1].txt
            }
        });
      }
    });*/
    wx.getLocation({
      type: "wgs84",
      success(res) {
        const latitude = res.latitude;
        const longitude = res.longitude;
        const key = "80a2fb89f6bd4fa7814b529669872a18";
        wx.request({
          url: `https://geoapi.qweather.com/v2/city/lookup?location=${longitude},${latitude}&key=${key}`, //获取天气数据API接口
          success(res) {
            //console.log(res.data)
            const { adm2, name } = res.data.location[0];
            //console.log(adm2)
            //console.log(name)
            that.area = name;
            that.city = adm2;
            // that.weather = now.cond_txt
          },
        });
        wx.request({
          url: `https://devapi.qweather.com/v7/weather/now?location=${longitude},${latitude}&key=${key}`, //获取天气数据API接口
          success(res) {
            //console.log(res.data)
            const { text } = res.data.now;
            // console.log(text)
            that.weather = text;
            //console.log(name)
            //that.area = name
            // that.city = adm2
            // that.weather = now.cond_txt
          },
        });
        wx.request({
          url: `https://devapi.qweather.com/v7/air/now?location=${longitude},${latitude}&key=${key}`, //获取天气数据API接口
          success(res) {
            // console.log(res.data)
            const { aqi, category } = res.data.now;
            // console.log(aqi)
            // console.log(category)
            that.airValue = aqi;
            that.airText = category;
          },
        });
        /*wx.request({
          url: `https://devapi.qweather.com/v7/indices/1d?type=1&location=${longitude},${latitude}&key=${key}`, //获取天气数据API接口
          success(res) {
            //console.log(res.data)
            const { text } = res.data.daily[0];
            //console.log(text)
            that.weatherAdivice = text;
          },
        });*/
      },
    });
  },
};
</script>

<style lang="scss" scoped>
.wrapper {
  padding: 15px;
  .header-wrapper {
    background-color: #0f1316;
    border-radius: 20px;
    color: #fff;
    box-shadow: 0px 0px 5px;
    padding: 15px 30px;
    .header-title {
      display: flex;
      justify-content: space-between;
    }
    .header-text {
      font-size: 32px;
      font-weight: 400;
      display: flex;
      justify-content: space-between;
    }
    .header-advice {
      margin-top: 20px;
      font-size: 20px;
    }
  }
  .data-wrapper {
    margin-top: 20px;
    display: flex;
    justify-content: space-between;
    .data {
      background-color: #fff;
      width: 150px;
      height: 80px;
      border-radius: 20px;
      display: flex;
      justify-content: space-around;
      padding: 0 8px;
      box-shadow: #d6d6d6 0 0 5px;
      .data-logo {
        width: 36px;
        height: 36px;
        margin-top: 15px;
      }
      .data-text {
        margin-top: 15px;
        color: #7f7f7f;
        .data-value {
          font-size: 26px;
        }
      }
    }
  }
  .foot_wrapper{
    margin-top: 150px;
    margin-left: auto;
    background-color: #fff;
    display: flex;
    justify-content: center;
    border-radius: 20px;
    box-shadow: #d6d6d6 0 0 5px;
    .time-logo{
      width: 32px;
      height: 32px;
    }
    .time_wrapper{
      font-size: 20px;
    }
  }
}
</style>
