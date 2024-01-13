#ifndef __ESP8266_H
#define __ESP8266_H	 

#include "string.h"
#include "sys.h"
#include "delay.h"
#include "led.h"
#include "usart.h"


void ESP_Init(void);
void ESP_print(u8 dis_hr,u8 dis_spo2);
#endif

