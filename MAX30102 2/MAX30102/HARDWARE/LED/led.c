#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK miniSTM32开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//初始化PB5和PE5为输出口.并使能这两个口的时钟		    
//LED IO初始化
void LED_Init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure; 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PA,PD端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				 //LED0-->PA.8 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.8
 GPIO_SetBits(GPIOA,GPIO_Pin_1);						 //PA.8 输出高
}
void beep(u8 dis_hr,u8 dis_spo2,u8 MAX_hr,u8 MIN_sp){
	
	if(dis_hr>=MAX_hr||dis_spo2<MIN_sp)
	{
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
    delay_ms(250);
	GPIO_SetBits(GPIOA,GPIO_Pin_1);
    delay_ms(250);
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	delay_ms(250);
	GPIO_SetBits(GPIOA,GPIO_Pin_1);
	delay_ms(250);
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	delay_ms(250);
	GPIO_SetBits(GPIOA,GPIO_Pin_1);
	delay_ms(250);
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	delay_ms(250);
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	delay_ms(250);
	GPIO_SetBits(GPIOA,GPIO_Pin_1);
	delay_ms(250);
    GPIO_SetBits(GPIOA,GPIO_Pin_1);
	delay_ms(500);
	//记录一次警报
	printf("AT+MQTTPUB=0,\"/sys/k0ofjeAwCiy/ESP826601S/thing/event/property/post\",\"{\\\"method\\\":\\\"thing.event.property.post\\\"\\,\\\"id\\\":\\\"1234\\\"\\,\\\"params\\\":{\\\"beep\\\":1}\\,\\\"version\\\":\\\"1.0.0\\\"}\",1,0\r\n");
	delay_ms(500);		
	}
}

void Beep(void){	
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
    delay_ms(50);
	GPIO_SetBits(GPIOA,GPIO_Pin_1);
    delay_ms(50);
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	delay_ms(50);
	GPIO_SetBits(GPIOA,GPIO_Pin_1);
	delay_ms(50);
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	delay_ms(50);
	GPIO_SetBits(GPIOA,GPIO_Pin_1);
	delay_ms(50);
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	delay_ms(250);
	GPIO_SetBits(GPIOA,GPIO_Pin_1);
	delay_ms(250);
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	delay_ms(250);
    GPIO_SetBits(GPIOA,GPIO_Pin_1);
	delay_ms(500);
}
