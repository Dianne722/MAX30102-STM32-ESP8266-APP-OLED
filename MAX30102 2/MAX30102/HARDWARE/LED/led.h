#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
#include "delay.h"
#include "ESP8266.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
#define LED0 PAout(1)	// PA8

void LED_Init(void);//��ʼ��
void beep(u8 dis_hr,u8 dis_spo2,u8 MAX_hr,u8 MIN_sp);
void Beep(void);		 				    
#endif
