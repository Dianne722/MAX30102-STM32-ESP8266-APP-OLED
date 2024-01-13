#include "ESP8266.h"

void ESP_Init(void){
	
	printf("AT+MQTTUSERCFG=0,1,\"NULL\",\"ESP826601S&k0ofjeAwCiy\",\"1d0a9b04255b3ff65e665d442b3d9f5b8a98b357b124fd91c7ed55cca94b5e5a\",0,0,\"\"\r\n");
	delay_ms(3000);
	printf("AT+MQTTCLIENTID=0,\"k0ofjeAwCiy.ESP826601S|securemode=2\\,signmethod=hmacsha256\\,timestamp=1703560136085|\"\r\n");
	delay_ms(3000);
	printf("AT+MQTTCONN=0,\"iot-06z00ez1lkiykep.mqtt.iothub.aliyuncs.com\",1883,1\r\n");
	delay_ms(5000);
	//printf("AT+MQTTPUB=0,\"/k0ofjeAwCiy/ESP826601S/user/update\",\"50\",1,0\r\n");
	
}

void ESP_print(u8 dis_hr,u8 dis_spo2){
	//上传数据对象
	//printf("AT+MQTTPUB=0,\"/sys/k0ofjeAwCiy/ESP826601S/thing/event/property/post\",\"{\\\"method\\\":\\\"thing.event.property.post\\\",\\\"id\\\":\\\"1234\\\",\\\"params\\\":{\\\"HeartRate\":%3d},\\\"version\\\":\\\"1.0.0\\\"}\",1,0\r\n",dis_hr);
		printf("AT+MQTTPUB=0,\"/sys/k0ofjeAwCiy/ESP826601S/thing/event/property/post\",\"{\\\"method\\\":\\\"thing.event.property.post\\\"\\,\\\"id\\\":\\\"1234\\\"\\,\\\"params\\\":{\\\"HeartRate\\\":%3d}\\,\\\"version\\\":\\\"1.0.0\\\"}\",1,0\r\n",dis_hr);
		delay_ms(500);	
		printf("AT+MQTTPUB=0,\"/sys/k0ofjeAwCiy/ESP826601S/thing/event/property/post\",\"{\\\"method\\\":\\\"thing.event.property.post\\\"\\,\\\"id\\\":\\\"1234\\\"\\,\\\"params\\\":{\\\"spO2\\\":%3d}\\,\\\"version\\\":\\\"1.0.0\\\"}\",1,0\r\n",dis_spo2);
		delay_ms(500);		
}
