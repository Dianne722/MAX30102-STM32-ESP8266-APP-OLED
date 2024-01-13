/*************************************************************************************
STM32F103-30102:
	VCC<->3.3V
	GND<->GND
	SCL<->PB7
	SDA<->PB8
	IM<->PB9
0.96inch OLED :
	VCC<->3.3V
	GND<->GND
	D0 SCL<->PA5
	D1 SDA<->PA6
	RST<->PA3
	DC<->PA4
	CS<->PA2
USB-TTL:
	5V<->5V
	GND<->GND
	RXD<->PA9
	TXD<->PA10
**************************************************************************************/
#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "max30102.h" 
#include "myiic.h"
#include "algorithm.h"
#include "oled.h"
#include "ESP8266.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
uint32_t aun_ir_buffer[500]; //IR LED sensor data
int32_t n_ir_buffer_length;    //data length
uint32_t aun_red_buffer[500];    //Red LED sensor data
int32_t n_sp02; //SPO2 value
int8_t ch_spo2_valid;   //indicator to show if the SP02 calculation is valid
int32_t n_heart_rate;   //heart rate value
int8_t  ch_hr_valid;    //indicator to show if the heart rate calculation is valid
uint8_t uch_dummy;
u8 MAX_hr=100 ,MIN_sp=50;
#define MAX_BRIGHTNESS 255
#define ten 10
void dis_DrawCurve(u32* data,u8 x);
void ESP_APP(void);
int main(void)
{ 
	//variables to calculate the on-board LED brightness that reflects the heartbeats
	uint32_t un_min, un_max, un_prev_data;  
	int i;
	int j=0;
	int32_t n_brightness;
	float f_temp;
	//u8 temp_num=0;
	u8 temp[6];
	u8 str[100];
	u8 dis_hr=0,dis_spo2=0;
    u16 u_hr=0,u_sp=0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�ж����ȼ����� ��2��
	delay_init();	    	 //��ʱ������ʼ��	  
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	LED_Init();
	
	//OLED
	OLED_Init();
	OLED_ShowString(0,0,"  initializing  ",16);
	OLED_Refresh_Gram();//������ʾ��OLED	 

	max30102_init();
	ESP_Init();
    ESP_APP();
	un_min=0x3FFFF;
	un_max=0;
	
	n_ir_buffer_length=500; //buffer length of 100 stores 5 seconds of samples running at 100sps
	//read the first 500 samples, and determine the signal range
	
    for(i=0;i<n_ir_buffer_length;i++)
    {
		ESP_APP();
        while(MAX30102_INT==1);   //wait until the interrupt pin asserts
		max30102_FIFO_ReadBytes(REG_FIFO_DATA,temp);
		aun_red_buffer[i] =  (long)((long)((long)temp[0]&0x03)<<16) | (long)temp[1]<<8 | (long)temp[2];    // Combine values to get the actual number
		aun_ir_buffer[i] = (long)((long)((long)temp[3] & 0x03)<<16) |(long)temp[4]<<8 | (long)temp[5];   // Combine values to get the actual number
            
        if(un_min>aun_red_buffer[i])
            un_min=aun_red_buffer[i];    //update signal min
        if(un_max<aun_red_buffer[i])
            un_max=aun_red_buffer[i];    //update signal max
    }
	un_prev_data=aun_red_buffer[i];
	//calculate heart rate and SpO2 after first 500 samples (first 5 seconds of samples)
    maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_sp02, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid); 
	
	while(1)
	{
		i=0;
        un_min=0x3FFFF;
        un_max=0;
		ESP_APP();
		//dumping the first 100 sets of samples in the memory and shift the last 400 sets of samples to the top
        for(i=100;i<500;i++)
        {
			ESP_APP();
            aun_red_buffer[i-100]=aun_red_buffer[i];
            aun_ir_buffer[i-100]=aun_ir_buffer[i];
            
            //update the signal min and max
            if(un_min>aun_red_buffer[i])
            un_min=aun_red_buffer[i];
            if(un_max<aun_red_buffer[i])
            un_max=aun_red_buffer[i];
        }
		//take 100 sets of samples before calculating the heart rate.
        for(i=400;i<500;i++)
        {
			ESP_APP();
            un_prev_data=aun_red_buffer[i-1];
            while(MAX30102_INT==1);
            max30102_FIFO_ReadBytes(REG_FIFO_DATA,temp);
			aun_red_buffer[i] =  (long)((long)((long)temp[0]&0x03)<<16) | (long)temp[1]<<8 | (long)temp[2];    // Combine values to get the actual number
			aun_ir_buffer[i] = (long)((long)((long)temp[3] & 0x03)<<16) |(long)temp[4]<<8 | (long)temp[5];   // Combine values to get the actual number
        
            if(aun_red_buffer[i]>un_prev_data)
            {
                f_temp=aun_red_buffer[i]-un_prev_data;
                f_temp/=(un_max-un_min);
                f_temp*=MAX_BRIGHTNESS;
                n_brightness-=(int)f_temp;
                if(n_brightness<0)
                    n_brightness=0;
            }
            else
            {
                f_temp=un_prev_data-aun_red_buffer[i];
                f_temp/=(un_max-un_min);
                f_temp*=MAX_BRIGHTNESS;
                n_brightness+=(int)f_temp;
                if(n_brightness>MAX_BRIGHTNESS)
                    n_brightness=MAX_BRIGHTNESS;
            }
			//send samples and calculation result to terminal program through UART
			if(ch_hr_valid == 1 && n_heart_rate<120)//**/ ch_hr_valid == 1 && ch_spo2_valid ==1 && n_heart_rate<120 && n_sp02<101
			{
				dis_hr = n_heart_rate;
				dis_spo2 = n_sp02;
			}
			else
			{
				dis_hr = 0;
				dis_spo2 = 0;
			}
		}
        maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_sp02, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid);
		
		//��ʾˢ��
		if(dis_hr == 0 && dis_spo2 == 0)  //**dis_hr == 0 && dis_spo2 == 0
		{
			sprintf((char *)str,"HR:--- SpO2:--- ");//**HR:--- SpO2:--- 
			ESP_APP();
		}
		else{
			sprintf((char *)str,"HR:%3d SpO2:%3d ",dis_hr,dis_spo2);//**HR:%3d SpO2:%3d
            ESP_APP();
			if(j<10)
			{
				j++;
				u_hr+=dis_hr;
				u_sp+=dis_spo2; 
			}
			else
			{
				ESP_print(u_hr/ten,u_sp/ten);//�ɼ�10�κ�ȡƽ��ֵ
				beep(u_hr/ten,u_sp/ten,MAX_hr,MIN_sp);//����Ƿ���Ҫ����
				u_hr=0,u_sp=0;
				j=0;
			}
		}
		ESP_APP();	
		OLED_ShowString(0,0,str,16);
		OLED_Fill(0,23,127,63,0);
		//������ϣ���������
		dis_DrawCurve(aun_red_buffer,20);
		dis_DrawCurve(aun_ir_buffer,0);
		OLED_Refresh_Gram();//������ʾ��OLED	
        
	}
}

void dis_DrawCurve(u32* data,u8 x)
{
	u16 i;
	u32 max=0,min=262144;
	u32 temp;
	u32 compress;
	
	for(i=0;i<128*2;i++)
	{
		if(data[i]>max)
		{
			max = data[i];
		}
		if(data[i]<min)
		{
			min = data[i];
		}
	}
	
	compress = (max-min)/20;
	
	for(i=0;i<128;i++)
	{
		temp = data[i*2] + data[i*2+1];
		temp/=2;
		temp -= min;
		temp/=compress;
		if(temp>20)temp=20;
		OLED_DrawPoint(i,63-x-temp,1);
	}
}
void ESP_APP(void){
	
	if(USART_RX_STA & 0x8000) {
        if (strstr((const char*)USART_RX_BUF, "10011")) {
			Beep();
        }
		if (strstr((const char*)USART_RX_BUF, "632107060") ){
			const char* targetString = "632107060";

			char* foundPosition = strstr((const char*)USART_RX_BUF, targetString);
			if (foundPosition) {
			// ����ҵ� "632107060" �ַ�������ȡ��λ�ú���λ�ַ�
			int startIndex = foundPosition - (const char*)USART_RX_BUF + strlen(targetString);

			// ��ȡ��λ�ַ�
			char newString[3];  // ���ڴ洢�µ��ַ��������� '\0'
			strncpy(newString, (const char*)USART_RX_BUF + startIndex, 2);
			newString[2] = '\0';  // ����ַ���������

			// ���µ��ַ���ת��Ϊ u8 ����
			MIN_sp = (u8)atoi(newString);

			printf("minSP:%3d\r\n", MIN_sp);
		}
		}if (strstr((const char*)USART_RX_BUF, "632107070") ){
			const char* targetString = "632107070";

			char* foundPosition = strstr((const char*)USART_RX_BUF, targetString);
			if (foundPosition) {
			// ����ҵ� "632107060" �ַ�������ȡ��λ�ú���λ�ַ�
			int startIndex = foundPosition - (const char*)USART_RX_BUF + strlen(targetString);

			// ��ȡ��λ�ַ�
			char newString[4];  // ���ڴ洢�µ��ַ��������� '\0'
			strncpy(newString, (const char*)USART_RX_BUF + startIndex, 3);
			newString[3] = '\0';  // ����ַ���������

			// ���µ��ַ���ת��Ϊ u8 ����
			MAX_hr = (u8)atoi(newString);

			printf("maxHR:%3d\r\n", MAX_hr);
		}
		}
        // ��ս��ջ�������״̬
        memset(USART_RX_BUF, 0, 600);
        USART_RX_STA = 0;
      }
  }

