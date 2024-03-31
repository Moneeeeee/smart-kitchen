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
// 在Page({})函数外定义 threshold 变量
let tem_threshold = 40; // 默认阈值为50
let mq2_threshold = 300; // 默认阈值为50
Page({
  data: {
    client: null,
    Temp:0,
    Hum:0,
    MQ2:0,
    MQ4:0,
    LED:false,
    Steer:false,
    FUN:false,
    WATER:false,
    tem_threshold:0,
    MQ2_threshold:0,
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

  onFUNChange(event){
    const that  = this;
    const sw = event.detail.value;
    that.setData({ FUN: sw });
    // console.log(event.detail,value);
    // that.setData({LED:sw})
      that.data.client.publish(mpPubTopic, JSON.stringify({
        target: "FUN",
        value: sw ? 1 : 0
      }), function (err) {
        if (!err) {
          console.log('成功下发指令：' + (sw ? '打开风扇' : '关闭风扇'));
        }
    });
  },

  onWATERChange(event){
    const that  = this;
    const sw = event.detail.value;
    that.setData({ WATER: sw });
    // console.log(event.detail,value);
    // that.setData({LED:sw})
      that.data.client.publish(mpPubTopic, JSON.stringify({
        target: "WATER",
        value: sw ? 1 : 0
      }), function (err) {
        if (!err) {
          console.log('成功下发指令：' + (sw ? '打开喷淋' : '关闭喷淋'));
        }
    });
  },

  onTEMThresholdChange(event) {
    const that = this;
    const tem_threshold_read = event.detail.value;
    // 将 number 类型的数据转换为 uint8 类型
    const tem_threshold_uint8 = Math.round(tem_threshold_read); // 使用 Math.round() 进行四舍五入取整
    that.setData({ tem_threshold: tem_threshold_read });
    // 发送 uint8 类型的数据到 STM32 端
    that.data.client.publish(mpPubTopic, JSON.stringify({
      target: "tem_threshold",
      value: tem_threshold_uint8 // 发送 uint8 类型的数据
    }), function (err) {
      if (!err) {
        console.log('成功下发温度阈值:', tem_threshold_uint8);
      }
    });
  },
  

  onMQ2ThresholdChange(event) {
    const that = this;
    const MQ2_threshold_read = event.detail.value;
    that.setData({ MQ2_threshold: MQ2_threshold_read });
    // console.log(event.detail,value);
    // that.setData({LED:sw})
      that.data.client.publish(mpPubTopic, JSON.stringify({
        target: "MQ2_threshold",
        value: MQ2_threshold_read
      }), function (err) {
        if (!err) {
          console.log('成功下发阈值:', MQ2_threshold_read);
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
          MQ4:dataFromDev.MQ4,
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
