// index.js
const defaultAvatarUrl = 'https://mmbiz.qpic.cn/mmbiz/icTdbqWNOwNRna42FI242Lcia07jQodd2FJGIYQfG0LAJGFxM4FbnQP6yfMxBgJ0F3YRqJCJ1aPAK2dQagdusBZg/0'

const app = getApp()

const { connect } = require('../../utils/mqtt')

const mqttHost = 'broker.emqx.io' // mqtt服务器域名
const mqttPort = 8084 // mqtt服务器端口

const deviceSubTopic = '/mysmartkitchen/sub' // 设备订阅topic
const devicePubTopic = '/mysmartkitchen/pub' // 设备发布topic

const mpSubTopic = devicePubTopic
const mpPubTopic = deviceSubTopic

Page({
  data: {
    client: null,
    Temp:0,
    Hum:0,
    MQ2:0,
    LED:false,
    Steer:false,
    Relay:false
  },
  onLEDChange(event){
    const that  = this;
    const sw = event.detail.value;
    that.setData({ LED: sw });
    // console.log(event.detail,value);
    // that.setData({LED:sw})
      that.data.client.publish(mpPubTopic, JSON.stringify({
        target: "LED",
        value: sw ? 1 : 0
      }), function (err) {
        if (!err) {
          console.log('成功下发指令：' + (sw ? '开灯' : '关灯'));
        }
    });
  },

  onSTEERChange(event){
    const that  = this;
    const sw = event.detail.value;
    that.setData({ Steer: sw });
    // console.log(event.detail,value);
    // that.setData({LED:sw})
      that.data.client.publish(mpPubTopic, JSON.stringify({
        target: "Steer",
        value: sw ? 1 : 0
      }), function (err) {
        if (!err) {
          console.log('成功下发指令：' + (sw ? '打开管道' : '关闭管道'));
        }
    });
  },

  onRELAYChange(event){
    const that  = this;
    const sw = event.detail.value;
    that.setData({ Relay: sw });
    // console.log(event.detail,value);
    // that.setData({LED:sw})
      that.data.client.publish(mpPubTopic, JSON.stringify({
        target: "Relay",
        value: sw ? 1 : 0
      }), function (err) {
        if (!err) {
          console.log('成功下发指令：' + (sw ? '打开风扇' : '关闭风扇'));
        }
    });
  },
  //事件处理函数
  onShow() {
    const that = this
    that.setData({
      client: connect(`wxs://${mqttHost}:${mqttPort}/mqtt`)
    });
    that.data.client.on('connect', function () {
      console.log('成功连接到MQTT服务器');
      wx.showToast({
        title: '连接成功',
        icon: 'success',
        mask: true
      });

      that.data.client.subscribe(mpSubTopic, function (err) {
        if (err) {
          console.error('订阅设备上行数据Topic失败:', err);
        } else {
          console.log('成功订阅设备上行数据Topic');
        }
  
      });
    });
  
    that.data.client.on('message',function(topic,message){
      console.log(topic);
      // console.log(message);
      //message是十六进制的buffer流

      let dataFromDev = {};
      try{
        dataFromDev = JSON.parse(message)
        console.log(dataFromDev);

        that.setData({
          Temp:dataFromDev.Temp,
          Hum:dataFromDev.Hum,
          MQ2:dataFromDev.MQ2,
          Steer:dataFromDev.Steer,
          Relay:dataFromDev.Relay,
          LED:dataFromDev.LED,
        });

      }catch(error){
        console.log(error);
        console.log('JSON解析失败',error);
      }

    });
  }
});
