#ifndef _ESP01S_H_
#define _ESP01S_H_

#define REV_OK		0	//接收完成标志
#define REV_WAIT	1	//接收未完成标志

extern unsigned char aRxBuffer; // 定义接收缓冲区
extern unsigned char ESP01S_buf[64];
extern unsigned short ESP01S_cnt, ESP01S_cntPre;

void ESP01S_Init(void);
void ESP01S_Clear(void);
void ESP01S_SendData(unsigned char *data, unsigned short len);
unsigned char *ESP01S_GetIPD(unsigned short timeOut);

#endif

