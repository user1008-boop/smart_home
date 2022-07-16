<template>
  <div>
    <div class="header">
      <div v-if="isLogin">
        <div class="header-title">请登录</div>
        <div class="header-info">Please Login Your Account</div>
      </div>
      <div v-else>
        <div class="header-title">请注册</div>
        <div class="header-info">Please Register Your Account</div>
      </div>
    </div>
    <div class="body">
      <div class="login-form">
        <van-field
          placeholder="请输入用户名"
          :value="inputUserName"
          @change="onUserNameChange"
        />
        <van-field
          type="password"
          placeholder="请输入密码"
          :value="inputPassWord"
          @change="onPassWordChange"
        />
        <div v-if="!isLogin">
          <van-field
            placeholder="请输入账号所需绑定的手机号"
            :value="inputContact"
            @change="onContactChange"
          />
        </div>
        <van-button
          slot="button"
          round
          block
          color="#3d7ef9"
          @click="onClick"
          >{{ isLogin ? "登录" : "注册" }}</van-button
        >
        <div class="other-options">
          <div @click="onOptionsClick">
            <span>{{ isLogin ? "注册账户" : "登录账户" }}</span>
          </div>
          <span style="margin: 0 30px">|</span>
          <div @click="onForgetClick">
            <span>找回密码</span>
          </div>
        </div>
        <div class="copyright-wrapper">
          <span class="copyright">Powerd by Deng</span>
        </div>
        <van-dialog
          use-slot
          title="找回密码校验"
          :show="showFindPassWord"
          show-cancel-button
          transition="fade"
          @confirm="onFindPassWordConfirm"
          @cancel="onFindPassWordCancel"
        >
          <van-field
            label="手机号"
            title-width="60px"
            placeholder="请输入账号所绑定的手机号"
            :value="inputContact"
            @change="onContactChange"
          />
        </van-dialog>
         <van-dialog
          use-slot
          title="重置密码"
          :show="showResetPassWord"
          show-cancel-button
          transition="fade"
          @confirm="onResetPassWordConfirm"
          @cancel="onResetPassWordCancel"
        >
          <van-field
            label="用户名"
            title-width="60px"
            :value="inputUserName"
            readonly
          />
           <van-field
            label="新密码"
            type="password"
            title-width="60px"
            placeholder="请输入新密码"
            :value="inputPassWord"
            @change="onPassWordChange"
          />
        </van-dialog>
        <van-toast id="van-toast" />
      </div>
    </div>
  </div>
</template>

<script>
import Toast from "vant-weapp/dist/toast/toast";

export default {
  data() {
    return {
      isLogin: true,
      inputUserName: "",
      inputPassWord: "",
      inputContact: "",
      showFindPassWord: false,
      showResetPassWord: false,
    };
  },
  methods: {
    onUserNameChange(event) {
      var that = this;
      that.$set(that, "inputUserName", event.mp.detail);
      console.log("当前输入用户名为:", event.mp.detail);
    },
    onPassWordChange(event) {
      var that = this;
      that.$set(that, "inputPassWord", event.mp.detail);
      console.log("当前输入密码为:", event.mp.detail);
    },
    onContactChange(event) {
      var that = this;
      that.$set(that, "inputContact", event.mp.detail);
      console.log("您当前输入手机号为:", event.mp.detail);
    },
    onClick(event) {
      var that = this;
      Toast.loading({
        duration: 0,
        forbidClick: true,
        message: that.isLogin ? "登陆中..." : "注册中...",
      });
      setTimeout(() => {
        if (!that.isLogin) {
          wx.setStorage({
            key: "user",
            data: {
              username: that.inputUserName,
              password: that.inputPassWord,
              contect: that.inputContact,
            },
            success(res) {
              console.log(res);
              console.log("注册成功!");
              Toast.success("注册成功!");
            },
            fail(res) {
              console.log(res);
              console.log("注册失败!");
              Toast.success("注册失败!");
            },
          });
        } else {
          wx.getStorage({
            key: "user",
            success(res) {
              console.log(res.data);
              if (
                that.inputUserName === res.data.username &&
                that.inputPassWord === res.data.password
              ) {
                Toast.success("登录成功");
                setTimeout(() => {
                  wx.switchTab({
                    url: "/pages/index/main",
                  });
                }, 500);
              } else {
                Toast.fail("用户名或密码错误");
              }
            },
            fail(res) {
              console.log(res);
              Toast.fail("数据库暂无该注册用户");
            },
          });
        }
      }, 1000);
    },
    onOptionsClick(event) {
      var that = this;
      that.isLogin = !that.isLogin;
      console.log(`当前处于${that.isLogin ? "登录" : "注册"}状态`);
    },
    onForgetClick(event) {
      var that = this;
      that.showFindPassWord = true;
    },
    onFindPassWordConfirm(event) {
      var that = this;
      wx.getStorage({
        key: "user",
        success(res) {
          console.log(res.data);
          if (that.inputContact === res.data.contect) {
            that.inputUserName = res.data.username
            that.showResetPassWord = true
            console.log("找到用户:",res.data.username);
          } else {
            Toast.fail("无该用户信息");
            that.showFindPassWord=false
            that.inputContact="";
          }
        },
        fail(res) {
          console.log(res);
          Toast.fail("数据库暂无该注册用户");
           that.showFindPassWord=false
          that.inputContact="";
        },
      });
    },
    onFindPassWordCancel(event){
      var that = this
      that.showFindPassWord=false
      that.inputContact=""
    },
    onResetPassWordConfirm(event){
      var that = this
       wx.setStorage({
            key: "user",
            data: {
              username: that.inputUserName,
              password: that.inputPassWord,
              contect: that.inputContact,
            },
            success(res) {
              console.log(res);
              console.log("密码重置成功!");
              Toast.success("密码重置成功!");
            },
            fail(res) {
              console.log(res);
              console.log("密码重置失败!");
              Toast.success("密码重置失败!");
            },
          });
    },
    onResetPassWordCancel(event){
      var that= this
      that.showResetPassWord=false
      that.inputUserName=""
      that.inputPassWord=""
      that.inputContact=""
    }
  },
};
</script>

<style lang="scss" scoped>
.header {
  height: 100px;
  padding: 25px 0;
  background-color: #3d7ef9;
  .header-title {
    font-size: 28px;
    font-weight: 500;
    color: #fff;
    margin-left: 20px;
  }
  .header-info {
    font-size: 14px;
    color: #fff;
    margin-left: 20px;
  }
}
.body {
  padding: 20px;
  margin-top: -20px;
  border-radius: 15px 15px 0 0;
  background-color: #fff;
  .login-form {
    margin-bottom: 30px;
  }
  .other-options {
    display: flex;
    justify-content: center;
    margin-top: 20px;
    color: #9f9f9f;
  }
  .copyright-wrapper {
    display: flex;
    justify-content: center;
    .copyright {
      color: #d6d6d6;
      position: fixed;
      bottom: 50px;
    }
  }
}
</style>
