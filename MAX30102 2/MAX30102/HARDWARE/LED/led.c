#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK mini�SSTM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//��ʼ��PB5��PE5Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void LED_Init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure; 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��PA,PD�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				 //LED0-->PA.8 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA.8
 GPIO_SetBits(GPIOA,GPIO_Pin_1);						 //PA.8 �����
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
	//��¼һ�ξ���
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
