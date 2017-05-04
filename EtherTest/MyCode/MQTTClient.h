
#ifndef __MYMQTTCLIENT_H
#define __MYMQTTCLIENT_H

#include "UART.h"
#include "MQTTPacket.h"

#define MQTT_STRUCT_ID				"MQTC"
#define MQTT_STRUCT_VERSION			3
#define MQTT_CLIENT_ID				"me"
#define MQTT_KEEPALIVE_INTERVAL		60
#define MQTT_CLEANSESSION			1
#define MQTT_USERNAME				"thaonm"
#define MQTT_PASSWORD				"thaonm"


#define ucServerADDR				"m13.cloudmqtt.com"
#define	ulServerPort				11346
#define MQTT_TOPIC_PUBLISH			"HomeKit_Publish"
#define MQTT_TOPIC_SUBCRIBE			"HomeKit_Subcribe"

#define MQTT_MAX_LENGHT_DATA		1024


unsigned char ucMQTTClient_Connect(void);

void vMQTTClient_Disconnect(void);

unsigned char ucMQTTClient_Publish(char *cTopic, unsigned char *ucPayload);

unsigned char ucMQTTClient_Subcribe(char *cTopic);

#endif
