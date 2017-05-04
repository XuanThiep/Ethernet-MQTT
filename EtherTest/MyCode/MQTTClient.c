
#include "api.h"
#include "string.h"
#include "MQTTClient.h"

extern struct netconn *conn;


unsigned char ucMQTTClient_Connect(void)
{
  unsigned char uMQTTConnect_Buff[255] = {0};
  unsigned int uMQTTConnect_BuffLen = sizeof(uMQTTConnect_Buff);
  unsigned int uMQTTConnect_Lenght = 0, uTCPSend_Lenght = 0;
  MQTTPacket_connectData MQTTPacket_DataConnect = MQTTPacket_connectData_initializer;

  MQTTPacket_DataConnect.clientID.cstring = MQTT_CLIENT_ID;
  MQTTPacket_DataConnect.username.cstring = MQTT_USERNAME;
  MQTTPacket_DataConnect.password.cstring = MQTT_PASSWORD;
  MQTTPacket_DataConnect.keepAliveInterval = 350;

  uMQTTConnect_Lenght = MQTTSerialize_connect(uMQTTConnect_Buff, uMQTTConnect_BuffLen, &MQTTPacket_DataConnect);

  err_t err = netconn_write(conn, uMQTTConnect_Buff, uMQTTConnect_Lenght, NETCONN_NOFLAG);
  if(err == ERR_OK)
  {
    APP_DEBUG("--- MQTTClient: MQTTClient Connect OK\r\n");
    return 1;
  }
  else
  {
    APP_DEBUG("--- MQTTClient: MQTTClient Connect FAIL\r\n");
    return 0;
  }
}

void vMQTTClient_Disconnect(void)
{

}

unsigned char ucMQTTClient_Publish(char *cTopic, unsigned char *ucPayload)
{
  unsigned char MQTTPublish_Buff[255] = {0};
  signed int MQTTPublish_BuffLen = 0;

  signed int MQTTPayload_Lenght = 0;
  signed long MQTTPublish_Lenght = 0;
  signed long MQTT_TCPlenght = 0;
  MQTTString TopicString = MQTTString_initializer;

  MQTTPublish_BuffLen = sizeof(MQTTPublish_Buff);
  MQTTPayload_Lenght = strlen((char *)ucPayload);
  TopicString.cstring = cTopic;

  APP_DEBUG("--- MQTTClient: Create packet for Topic_Publish: %s\r\n", TopicString.cstring);
  MQTTPublish_Lenght = MQTTSerialize_publish(MQTTPublish_Buff + MQTTPublish_Lenght, MQTTPublish_BuffLen 	\
                  - MQTTPublish_Lenght, 0, 0, 0, 0, TopicString, ucPayload, MQTTPayload_Lenght); /* 2 */			\

  err_t err = netconn_write(conn, MQTTPublish_Buff, MQTTPublish_Lenght, NETCONN_NOFLAG);

  if(err == ERR_OK)
  {
    APP_DEBUG("--- MQTTClient: Send data Done, Number of bytes Sent = %d\r\n", MQTT_TCPlenght);

    APP_DEBUG("--- MQTTClient: Data Publish = ");
    for(unsigned char uci = 2; uci < MQTTPublish_BuffLen; uci++)
            APP_DEBUG("%c", MQTTPublish_Buff[uci]);
    APP_DEBUG("\r\n");
    return 1;
  }
  else
  {
    APP_DEBUG("--- MQTTClient: Send data ERROR, MQTT_TCPlenght = %d\r\n", MQTT_TCPlenght);
    return 0;
  }
}

unsigned char ucMQTTClient_Subcribe(char *cTopic)
{
  signed long slMQTTSubcribe_IsOK = 0;
  unsigned char ucMQTTSubcribe_Buff[255] = {0};
  unsigned int uiMQTTSubcribe_BuffLenght = 0;
  signed long slMQTTSubcribe_Lenght = 0;
  MQTTString TopicString = MQTTString_initializer;
  unsigned int msgid = 1;
  int uiReq_qos = 0;

  uiMQTTSubcribe_BuffLenght = sizeof(ucMQTTSubcribe_Buff);
  TopicString.cstring = cTopic;

  slMQTTSubcribe_Lenght = MQTTSerialize_subscribe(ucMQTTSubcribe_Buff, uiMQTTSubcribe_BuffLenght, 0, msgid, 1, &TopicString, &uiReq_qos);
  err_t err = netconn_write(conn, ucMQTTSubcribe_Buff, slMQTTSubcribe_Lenght, NETCONN_NOFLAG);
  if(err == ERR_OK)
  {
    APP_DEBUG("------------ Subcribe OK ------------\r\n");
    return 1;
  }
  else
  {
    APP_DEBUG("------------ Subcribe FAIL ------------\r\n");
    return 0;
  }
}





