#ifndef __DHT11_H
#define __DHT11_H

#include "main.h"

#define DHT11_HIGH     HAL_GPIO_WritePin(GPIOB, DHT11_Pin,GPIO_PIN_SET) //输出高电平
#define DHT11_LOW      HAL_GPIO_WritePin(GPIOB, DHT11_Pin,GPIO_PIN_RESET)//输出低电平

#define DHT11_IO_IN      HAL_GPIO_ReadPin(GPIOB, DHT11_Pin)//读取IO口电平

void Delay_us(uint32_t udelay);               //us级延时函数

//DHT11_DATA ,IO口设置为输出模式（这里的IO口指的是STM32CubeMX中配置的IO口）
void DHT11_OUT(void);

void DHT11_IN(void);                          //DHT11_Data IO设置为输入模式
void DHT11_Strat(void);				          //主机发出起始信号
uint8_t DHT11_Check(void);                    //DHT11发送响应信号
uint8_t DHT11_Read_Bit(void);                 //读取DHT11一个BIT的数据
uint8_t DHT11_Read_Byte(void);                //读取DHT11一个Byte的数据
uint8_t DHT11_Read_Data(uint8_t* temp , uint8_t* humi);  //读取DHT11湿度和温度的数据

extern uint8_t temperature ;
extern uint8_t humidity ;
//extern uint8_t aTXbuf[32] ;

#endif
