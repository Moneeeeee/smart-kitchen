
//单片机头文件
#include "main.h"

//网络设备驱动
#include "esp8266.h"

//硬件驱动
#include "usart.h"

//C库
#include <string.h>
#include <stdio.h>
#include <string.h>


#define ESP01S_WIFI_INFO		"AT+CWJAP=\"iPhone\",\"20050623\"\r\n"
#define ESP01S_ONENET_INFO		"AT+CIPSTART=\"TCP\",\"broker.emqx.io\",1883\r\n"

unsigned char ESP01S_buf[64];
unsigned short ESP01S_cnt = 0, ESP01S_cntPre = 0;

unsigned char aRxBuffer;			//接收中断缓冲
//==========================================================
//	函数名称：	ESP01S_Clear
//
//	函数功能：	清空缓存
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void ESP01S_Clear(void)
{

    memset(ESP01S_buf, 0, sizeof(ESP01S_buf));
    ESP01S_cnt = 0;

}

//==========================================================
//	函数名称：	ESP01S_WaitRecive
//
//	函数功能：	等待接收完成
//
//	入口参数：	无
//
//	返回参数：	REV_OK-接收完成		REV_WAIT-接收超时未完成
//
//	说明：		循环调用检测是否接收完成
//==========================================================
_Bool ESP01S_WaitRecive(void)
{

    if(ESP01S_cnt == 0) 							//如果接收计数为0 则说明没有处于接收数据中，所以直接跳出，结束函数
        return REV_WAIT;

    if(ESP01S_cnt == ESP01S_cntPre)				//如果上一次的值和这次相同，则说明接收完毕
    {
        ESP01S_cnt = 0;							//清0接收计数

        return REV_OK;								//返回接收完成标志
    }

    ESP01S_cntPre = ESP01S_cnt;					//置为相同

    return REV_WAIT;								//返回接收未完成标志

}

//==========================================================
//	函数名称：	ESP01S_SendCmd
//
//	函数功能：	发送命令
//
//	入口参数：	cmd：命令
//				res：需要检查的返回指令
//
//	返回参数：	0-成功	1-失败
//
//	说明：		
//==========================================================
_Bool ESP01S_SendCmd(char *cmd, char *res)
{

    unsigned char timeOut = 200;

    Usart_SendString(huart2, (unsigned char *)cmd, strlen((const char *)cmd));
    while(timeOut--)
    {
        if(ESP01S_WaitRecive() == REV_OK)							//如果收到数据
        {
            if(strstr((const char *)ESP01S_buf, res) != NULL)		//如果检索到关键词
            {
                ESP01S_Clear();									//清空缓存
                return 0;
            }
        }

        HAL_Delay(10);
    }

    return 1;

}

//==========================================================
//	函数名称：	ESP01S_SendData
//
//	函数功能：	发送数据
//
//	入口参数：	data：数据
//				len：长度
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void ESP01S_SendData(unsigned char *data, unsigned short len)
{

    char cmdBuf[32];

    ESP01S_Clear();								//清空接收缓存
    sprintf(cmdBuf, "AT+CIPSEND=%d\r\n", len);		//发送命令
    if(!ESP01S_SendCmd(cmdBuf, ">"))				//收到‘>’时可以发送数据
    {
        Usart_SendString(huart2, data, len);		//发送设备连接请求数据
    }

}

//==========================================================
//	函数名称：	ESP01S_GetIPD
//
//	函数功能：	获取平台返回的数据
//
//	入口参数：	等待的时间(乘以10ms)
//
//	返回参数：	平台返回的原始数据
//
//	说明：		不同网络设备返回的格式不同，需要去调试
//				如ESP01S的返回格式为	"+IPD,x:yyy"	x代表数据长度，yyy是数据内容
//==========================================================
unsigned char *ESP01S_GetIPD(unsigned short timeOut)
{

    char *ptrIPD = NULL;

    do
    {
        if(ESP01S_WaitRecive() == REV_OK)								//如果接收完成
        {
            ptrIPD = strstr((char *)ESP01S_buf, "IPD,");				//搜索“IPD”头
            if(ptrIPD == NULL)											//如果没找到，可能是IPD头的延迟，还是需要等待一会，但不会超过设定的时间
            {
                //printf("\"IPD\" not found\r\n");
            }
            else
            {
                ptrIPD = strchr(ptrIPD, ':');							//找到':'
                if(ptrIPD != NULL)
                {
                    ptrIPD++;
                    return (unsigned char *)(ptrIPD);
                }
                else
                    return NULL;

            }
        }

        HAL_Delay(5);	        //延时等待
    } while(timeOut--);

    return NULL;														//超时还未找到，返回空指针

}

//==========================================================
//	函数名称：	ESP01S_Init
//
//	函数功能：	初始化ESP01S
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void ESP01S_Init(void)
{


    ESP01S_Clear();

    printf("0. AT\r\n");
    while(ESP01S_SendCmd("AT\r\n", "OK"));
        HAL_Delay(500);

    printf("1. RST\r\n");
    ESP01S_SendCmd("AT+RST\r\n", "");
    HAL_Delay(500);

    ESP01S_SendCmd("AT+CIPCLOSE\r\n", "");              //关闭TCP连接
    HAL_Delay(500);

    printf("2. CWMODE\r\n");
    while(ESP01S_SendCmd("AT+CWMODE=1\r\n", "OK"))     //模式1(Station),默认保存Flash
        HAL_Delay(500);

    printf("3. AT+CWDHCP\r\n");       //开启DHCP(获取分配IP),默认保存Flash
    while(ESP01S_SendCmd("AT+CWDHCP=1,1\r\n", "OK"))
        HAL_Delay(500);

    printf("4. CWJAP\r\n");           //链接WIFI
    while(ESP01S_SendCmd(ESP01S_WIFI_INFO, "GOT IP"))
        HAL_Delay(500);

    printf("5. CIPSTART\r\n");        //开启TCP连接
    while(ESP01S_SendCmd(ESP01S_ONENET_INFO, "CONNECT"))
        HAL_Delay(500);

    printf("6. ESP01S Init OK\r\n");

}

/* USER CODE BEGIN 4 */



/* USER CODE END 4 */

