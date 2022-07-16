<template>
  <div class="wrapper">
    <div class="header-wrapper">
      <img
        class="main-background"
        mode="heightFix"
        src="/static/images/background_1.jpg"
      />
      <div class="UserInfo">
        <div class="avatar-wrapper">
          <open-data
            type="userAvatarUrl"
            default-avatar="/static/images/avatar.png"
          ></open-data>
        </div>
        <div class="UserName-wrapper">
          <open-data type="userNickName" default-text="读取中"></open-data>
        </div>
      </div>
    </div>
    <div class="body-wrapper">
      <div class="List-wrapper">
        <div class="List">
          <div>手机型号</div>
          <div>{{ Model }}</div>
        </div>
      </div>
      <div class="List-wrapper">
        <div class="List">
          <div>使用语言</div>
          <div>{{ Language }}</div>
        </div>
      </div>
      <div class="List-wrapper">
        <div class="List">
          <div>微信版本号</div>
          <div>{{ Version }}</div>
        </div>
      </div>
      <div class="List-wrapper">
        <div class="List">
          <div>操作系统</div>
          <div>{{ System }}</div>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
export default {
  data() {
    return {
      Model: "请求中",
      Language: "请求中",
      Version: "请求中",
      System: "请求中",
    };
  },

  onLoad: function () {
    var that = this;
    wx.getSystemInfo({
      success: function (res) {
        that.Model = res.model;
        that.Language = res.language;
        that.Version = res.version;
        that.System = res.system;
      },
      fail: function (res) {
        console.log("获取失败");
      },
      complete: function (res) {
        console.log("加载完成");
      },
    });
  },
};
</script>
<style scoped>
.wrapper {
  min-height: 100vh;
  overflow: hidden;
}
.header-wrapper {
  background: rgba(0, 0, 0, 0.6) no-repeat center center;
  background-blend-mode: multiply;
  height: 40vh;
  color: #fff;
}
.main-background {
  height: 40vh;
  z-index: -999;
  position: absolute;
}
.UserInfo {
  padding: 80px 0 20px 0;
  display: flex;
  justify-content: space-around;
  align-items: center;
}
.avatar-wrapper {
  height: 100px;
  width: 100px;
  border-radius: 50%;
  overflow: hidden;
  margin-left: 10px;
}
.UserName-wrapper {
  font-size: 36px;
  margin-right: 100px;
}
.body-wrapper {
  box-sizing: border-box;
  padding: 20px 15px 0 15px;
}
.List-wrapper {
  margin: 10px 0;
}
.List {
  border-radius: 25px;
  background-color: #2f2f2f;
  display: flex;
  justify-content: space-around;
  align-items: center;
  color: #fff;
  padding: 10px 20px;
}
</style>
