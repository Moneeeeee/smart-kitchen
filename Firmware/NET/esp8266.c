
//��Ƭ��ͷ�ļ�
#include "main.h"

//�����豸����
#include "esp8266.h"

//Ӳ������
#include "usart.h"

//C��
#include <string.h>
#include <stdio.h>
#include <string.h>


#define ESP01S_WIFI_INFO		"AT+CWJAP=\"iPhone\",\"20050623\"\r\n"
#define ESP01S_ONENET_INFO		"AT+CIPSTART=\"TCP\",\"broker.emqx.io\",1883\r\n"

unsigned char ESP01S_buf[64];
unsigned short ESP01S_cnt = 0, ESP01S_cntPre = 0;

unsigned char aRxBuffer;			//�����жϻ���
//==========================================================
//	�������ƣ�	ESP01S_Clear
//
//	�������ܣ�	��ջ���
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void ESP01S_Clear(void)
{

    memset(ESP01S_buf, 0, sizeof(ESP01S_buf));
    ESP01S_cnt = 0;

}

//==========================================================
//	�������ƣ�	ESP01S_WaitRecive
//
//	�������ܣ�	�ȴ��������
//
//	��ڲ�����	��
//
//	���ز�����	REV_OK-�������		REV_WAIT-���ճ�ʱδ���
//
//	˵����		ѭ�����ü���Ƿ�������
//==========================================================
_Bool ESP01S_WaitRecive(void)
{

    if(ESP01S_cnt == 0) 							//������ռ���Ϊ0 ��˵��û�д��ڽ��������У�����ֱ����������������
        return REV_WAIT;

    if(ESP01S_cnt == ESP01S_cntPre)				//�����һ�ε�ֵ�������ͬ����˵���������
    {
        ESP01S_cnt = 0;							//��0���ռ���

        return REV_OK;								//���ؽ�����ɱ�־
    }

    ESP01S_cntPre = ESP01S_cnt;					//��Ϊ��ͬ

    return REV_WAIT;								//���ؽ���δ��ɱ�־

}

//==========================================================
//	�������ƣ�	ESP01S_SendCmd
//
//	�������ܣ�	��������
//
//	��ڲ�����	cmd������
//				res����Ҫ���ķ���ָ��
//
//	���ز�����	0-�ɹ�	1-ʧ��
//
//	˵����		
//==========================================================
_Bool ESP01S_SendCmd(char *cmd, char *res)
{

    unsigned char timeOut = 200;

    Usart_SendString(huart2, (unsigned char *)cmd, strlen((const char *)cmd));
    while(timeOut--)
    {
        if(ESP01S_WaitRecive() == REV_OK)							//����յ�����
        {
            if(strstr((const char *)ESP01S_buf, res) != NULL)		//����������ؼ���
            {
                ESP01S_Clear();									//��ջ���
                return 0;
            }
        }

        HAL_Delay(10);
    }

    return 1;

}

//==========================================================
//	�������ƣ�	ESP01S_SendData
//
//	�������ܣ�	��������
//
//	��ڲ�����	data������
//				len������
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void ESP01S_SendData(unsigned char *data, unsigned short len)
{

    char cmdBuf[32];

    ESP01S_Clear();								//��ս��ջ���
    sprintf(cmdBuf, "AT+CIPSEND=%d\r\n", len);		//��������
    if(!ESP01S_SendCmd(cmdBuf, ">"))				//�յ���>��ʱ���Է�������
    {
        Usart_SendString(huart2, data, len);		//�����豸������������
    }

}

//==========================================================
//	�������ƣ�	ESP01S_GetIPD
//
//	�������ܣ�	��ȡƽ̨���ص�����
//
//	��ڲ�����	�ȴ���ʱ��(����10ms)
//
//	���ز�����	ƽ̨���ص�ԭʼ����
//
//	˵����		��ͬ�����豸���صĸ�ʽ��ͬ����Ҫȥ����
//				��ESP01S�ķ��ظ�ʽΪ	"+IPD,x:yyy"	x�������ݳ��ȣ�yyy����������
//==========================================================
unsigned char *ESP01S_GetIPD(unsigned short timeOut)
{

    char *ptrIPD = NULL;

    do
    {
        if(ESP01S_WaitRecive() == REV_OK)								//����������
        {
            ptrIPD = strstr((char *)ESP01S_buf, "IPD,");				//������IPD��ͷ
            if(ptrIPD == NULL)											//���û�ҵ���������IPDͷ���ӳ٣�������Ҫ�ȴ�һ�ᣬ�����ᳬ���趨��ʱ��
            {
                //printf("\"IPD\" not found\r\n");
            }
            else
            {
                ptrIPD = strchr(ptrIPD, ':');							//�ҵ�':'
                if(ptrIPD != NULL)
                {
                    ptrIPD++;
                    return (unsigned char *)(ptrIPD);
                }
                else
                    return NULL;

            }
        }

        HAL_Delay(5);	        //��ʱ�ȴ�
    } while(timeOut--);

    return NULL;														//��ʱ��δ�ҵ������ؿ�ָ��

}

//==========================================================
//	�������ƣ�	ESP01S_Init
//
//	�������ܣ�	��ʼ��ESP01S
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void    ESP01S_Init(void)
{


    ESP01S_Clear();

    printf("0. AT\r\n");
    while(ESP01S_SendCmd("AT\r\n", "OK"));
        HAL_Delay(500);

    printf("1. RST\r\n");
    ESP01S_SendCmd("AT+RST\r\n", "");
    HAL_Delay(500);

    ESP01S_SendCmd("AT+CIPCLOSE\r\n", "");              //�ر�TCP����
    HAL_Delay(500);

    printf("2. CWMODE\r\n");
    while(ESP01S_SendCmd("AT+CWMODE=1\r\n", "OK"))     //ģʽ1(Station),Ĭ�ϱ���Flash
        HAL_Delay(500);

    printf("3. AT+CWDHCP\r\n");       //����DHCP(��ȡ����IP),Ĭ�ϱ���Flash
    while(ESP01S_SendCmd("AT+CWDHCP=1,1\r\n", "OK"))
        HAL_Delay(500);

    printf("4. CWJAP\r\n");           //����WIFI
    while(ESP01S_SendCmd(ESP01S_WIFI_INFO, "GOT IP"))
        HAL_Delay(500);

    printf("5. CIPSTART\r\n");        //����TCP����
    while(ESP01S_SendCmd(ESP01S_ONENET_INFO, "CONNECT"))
        HAL_Delay(500);

    printf("6. ESP01S Init OK\r\n");

}

/* USER CODE BEGIN 4 */



/* USER CODE END 4 */

