#include "DHT11.h"



uint8_t temperature = 1 ;
uint8_t humidity = 1;
uint8_t aTXbuf[32] ;

/**
  * @brief  us级延时函数
  * @param  delay 控制延时的时长
  * @retval 无
  */
void Delay_us(uint32_t udelay)
{
    uint32_t startval,tickn,delays,wait;

    startval = SysTick->VAL;
    tickn = HAL_GetTick();
    delays =udelay * 72;
    if(delays > startval)
    {
        while(HAL_GetTick() == tickn)
        {

        }
        wait = 72000 + startval - delays;
        while(wait < SysTick->VAL)
        {

        }
    }
    else
    {
        wait = startval - delays;
        while(wait < SysTick->VAL && HAL_GetTick() == tickn)
        {

        }
    }
}
/**
  * @brief  DATA引脚（PA7）设置为输出模式
  * @param  无
  * @retval 无
  */
void DHT11_OUT(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}
/**
  * @brief  DATA引脚（PA7）设置为输入模式
  * @param  无
  * @retval	无
  */
void DHT11_IN(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin  = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}
/**
  * @brief  DHT11检测起始信号
  * @param  无
  * @retval 无
  */
void DHT11_Strat(void)
{
    DHT11_OUT();   //PA7设置为输出模式
    DHT11_LOW;     //主机拉低总线
    HAL_Delay(20); //延迟必须大于18ms ；
    DHT11_HIGH;    //主机拉高总线等待DHT11响应
    Delay_us(30);
}
/**
  * @brief  DHT11发送响应信号
  * @param  无
  * @retval 返回值0/1  0：响应成功 1：响应失败
  */
uint8_t DHT11_Check(void)
{
    uint8_t retry = 0 ;
    DHT11_IN();
    //采用while循环的方式检测响应信号
    while(DHT11_IO_IN && retry <100) // DHT11会拉低 40us ~80us
    {
        retry++;
        Delay_us(1);//1us
    }
    if(retry>=100) //判断当DHT11延迟超过80us时return 1 ， 说明响应失败
    {return  1;}
    else retry =  0 ;

    while(!DHT11_IO_IN && retry<100)// // DHT11拉低之后会拉高 40us ~80us
    {
        retry++;
        Delay_us(1);//1us
    }

    if(retry>=100)
    {return 1;}
    return 0 ;
}
/**
  * @brief  DHT11读取一位数据
  * @param  无
  * @retval 返回值0/1  1：读取成功 0：读取失败
  */
uint8_t DHT11_Read_Bit(void)
{
    uint8_t retry = 0 ;
    while(DHT11_IO_IN && retry <100)//同上采用while循环的方式去采集数据
    {
        retry++;
        Delay_us(1);
    }
    retry = 0 ;
    while(!DHT11_IO_IN && retry<100)
    {
        retry++;
        Delay_us(1);
    }

    Delay_us(40);              //结束信号，延时40us
    if(DHT11_IO_IN) return 1;  //结束信号后，总线会被拉高 则返回1表示读取成功
    else
        return 0 ;
}

/**
  * @brief  DHT11读取一个字节数据
  * @param  无
  * @retval 返回值：dat 将采集到的一个字节的数据返回
  */
uint8_t DHT11_Read_Byte(void)
{
    uint8_t i , dat ;
    dat = 0 ;
    for(i=0; i<8; i++)
    {
        dat <<= 1; //通过左移存储数据
        dat |= DHT11_Read_Bit();
    }
    return dat ;
}

/**
  * @brief  DHT11读取数据
  * @param  temp：温度值 humi ：湿度值
  * @retval 返回值0/1 0：读取数据成功 1：读取数据失败
  */
uint8_t DHT11_Read_Data(uint8_t* temp , uint8_t* humi)
{
    uint8_t buf[5];        //储存五位数据
    uint8_t i;
    DHT11_Strat();         //起始信号
    if(DHT11_Check() == 0) //响应信号
    {
        for(i=0; i<5; i++)
        {
            buf[i] = DHT11_Read_Byte();
        }
        if(buf[0]+buf[1]+buf[2]+buf[3] == buf[4]) //校验数据
        {
            *humi = buf[0]; // 湿度
            *temp = buf[2]; // 温度
        }
    }else return 1;

    return 0 ;
}




//
//uint8_t Data[5]={0x00,0x00,0x00,0x00,0x00};   //Data存储读取的温湿度信息
//
//void Delay_us(uint16_t us){     //微秒延时
//    uint16_t differ = 0xffff-us-5;
//    __HAL_TIM_SET_COUNTER(&htim1,differ);	//设定TIM1计数器起始值
//    HAL_TIM_Base_Start(&htim1);		//启动定时器
//
//    while(differ < 0xffff-5){	//判断
//        differ = __HAL_TIM_GET_COUNTER(&htim1);		//查询计数器的计数值
//    }
//    HAL_TIM_Base_Stop(&htim1);
//}
//
///*------------------------------*/
//void DHT_GPIO_SET_OUTPUT(void)     //设置GPIOx为输出模式（MCU的IO口向DHT11发激活信号）
//{
//    GPIO_InitTypeDef GPIO_InitStructure;    //在GPIO_InitTypeDef结构体中修改IO口参数（结构体成员）
//    GPIO_InitStructure.Pin=GPIO_PIN_0;      //设置的格式必须严格遵循注释，比如GPIO_PIN_define
//    GPIO_InitStructure.Mode=GPIO_MODE_OUTPUT_PP;
////	GPIO_InitStructure.Pull=;
//    GPIO_InitStructure.Speed=GPIO_SPEED_FREQ_HIGH;
//    HAL_GPIO_Init(GPIOB,&GPIO_InitStructure);
//}
//
//void DHT_GPIO_SET_INPUT(void)     //设置GPIOx为输入模式（DHT11向MUC的IO发电平信号，信号里包含了温湿度信息）
//{
//    GPIO_InitTypeDef GPIO_InitStructure;
//    GPIO_InitStructure.Pin=GPIO_PIN_0;
//    GPIO_InitStructure.Mode=GPIO_MODE_INPUT;
//    GPIO_InitStructure.Speed=GPIO_SPEED_FREQ_HIGH;
//    HAL_GPIO_Init(GPIOB,&GPIO_InitStructure);
//}
//
///*------------------------------*/
//
///*
//     uint8_t DHT_Read_Byte(void)用于转译采集DHT11发送给IO口的电平信号（8位）。
//*/
//uint8_t DHT_Read_Byte(void)  //从DHT11读取一位（8字节）信号
//{
//    uint8_t ReadData=0;  //ReadData用于存放8bit数据，即8个单次读取的1bit数据的组合
//    uint8_t temp;      //临时存放信号电平（0或1）
//    uint8_t retry=0;   //retry用于防止卡死
//    uint8_t i;
//    for(i=0; i<8; i++)   //一次温湿度信号读取八位
//    {
//        while(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)==0 && retry<100)
//            //等待直到DHT11输出高电平：当PA5=1，上升沿，表示开始接受数据，可以判断0 or 1，跳出循环，执行后续判断（若PA5=0，将一直循环等待）
//        {
//            Delay_us(1);
//            retry++;             //retry防止PA5读取不到数据卡死在这一步，当经历100us后retry自增到100，跳出循环。
//        }
//        retry=0;
//
//        Delay_us(40);    //延时30us
//        //根据时序图，DHT传回高电平信号维持26us~28us表示0，	维持70us表示1
//        //延时30us后，如果IO读取到仍是高电平，说明采集到1；如果IO读取到低电平，说明采集到0
//        //读取电平信号暂存temp内，随后会压入ReadData中
//        if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)==1)   temp=1;
//        else   temp=0;
//
//        while(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)==1 && retry<100)
//            //等待直到DHT11输出低电平，表示退出。本轮1bit信号接收完毕。
//        {
//            Delay_us(1);
//            retry++;
//        }
//        retry=0;
//
//        ReadData<<=1;    //ReadData内信号先全部左移一位，空出末尾位置
//        ReadData |= temp;        //将temp写入ReadData
//    }
//
//    return ReadData;
//}
//
///*------------------------------*/
//
///*
//     uint8_t DHT_Read(void)表达完整时序
//*/
//uint8_t DHT_Read(void)
//{
//    uint8_t retry=0;
//    uint8_t i;
//
//    DHT_GPIO_SET_OUTPUT();    //IO设置为输出模式。在传输的最开始，MCU要向DHT11发送信号
//    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);   //IO->DHT11:先拉低电平18ms（应时序要求）
//    HAL_Delay(18);
//    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);   //IO->DHT11:随后拉高电平20us
//    Delay_us(20);
//
//    //MCU通过IO向DHT11发送请求完毕。接下来DHT11向IO发送响应,IO转为输入模式。在这之后就开始信号的转译读取。
//    DHT_GPIO_SET_INPUT();
//    Delay_us(20);
//    if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)==0) //DHT11发回低电平响应（读取到低电平，说明DHT11有响应）
//    {
//        //接下来，DHT11拉低电平一段时间后拉高电平一段时间
//        while(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)==0 && retry<100)
//        {
//            Delay_us(1);
//            retry++;
//        }
//        retry=0;
//        while(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)==1 && retry<100)
//        {
//            Delay_us(1);
//            retry++;
//        }
//        retry=0;
//
//        //一共传输40位，一次DHT_Read_Byte返回8位，共读取5次。存储在Data[]中。（Data[]定义为全局）
//        for(i=0; i<5; i++)
//        {
//            Data[i] = DHT_Read_Byte();  //每次读取一字节（8位）
//        }
//        Delay_us(50);
//        //说明：Data[0]湿度， Data[2]温度。Data[1]和Data[3]分别为0和2的小数位。Data[4]用于校验。
//    }
//
//    uint32_t sum=Data[0]+Data[1]+Data[2]+Data[3];  //校验
//    if((sum)==Data[4])    return 1;
//    else   return 0;
//
//}
