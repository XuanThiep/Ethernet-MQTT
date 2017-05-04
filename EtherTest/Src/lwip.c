/**
 ******************************************************************************
  * File Name          : LWIP.c
  * Description        : This file provides initialization code for LWIP
  *                      middleWare.
  ******************************************************************************
  *
  * Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "lwip.h"
#include "lwip/init.h"
#include "lwip/netif.h"

/* USER CODE BEGIN 0 */
#include "api.h"
#include "err.h"
#include "string.h"
#include "UART.h"
#include "MQTTClient.h"
/* USER CODE END 0 */

/* ETH Variables initialization ----------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/* Variables Initialization */
struct netif gnetif;
ip4_addr_t ipaddr;
ip4_addr_t netmask;
ip4_addr_t gw;
uint8_t IP_ADDRESS[4];
uint8_t NETMASK_ADDRESS[4];
uint8_t GATEWAY_ADDRESS[4];

/* USER CODE BEGIN 2 */
struct netconn *conn;

static void tcp_thread(void *arg)
{
  conn = netconn_new(NETCONN_TCP);
  if (conn!=NULL)
  {
    /* Bind connection to well known port number 7. */
    ip_addr_t ip;
    IP4_ADDR(&ip, 34, 200, 51, 91);
    
    err_t err = netconn_bind ( conn, &ipaddr, 11346 );
    if(err != ERR_OK) APP_DEBUG("--- Lwip: Can not bind TCP\r\n");
    else 
    {
      APP_DEBUG("--- Lwip: Bind TCP netconn OK\r\n");
      //APP_DEBUG("--- Lwip: IP = %d.%d.%d.%d\r\n", (ipaddr.addr >> 24) & 0xff, (ipaddr.addr >> 16) & 0xff, (ipaddr.addr >> 8) & 0xff, ipaddr.addr & 0xff);
      APP_DEBUG("--- Lwip: IP Local = %d.%d.%d.%d\r\n", (gnetif.ip_addr.addr >> 24) & 0xff, (gnetif.ip_addr.addr >> 16) & 0xff, (gnetif.ip_addr.addr >> 8) & 0xff, gnetif.ip_addr.addr & 0xff);
      APP_DEBUG("--- Lwip: IP Host = %d.%d.%d.%d\r\n", (ip.addr >> 24) & 0xff, (ip.addr >> 16) & 0xff, (ip.addr >> 8) & 0xff, ip.addr & 0xff);
      err = netconn_connect(conn, &ip, 11346);
      if(err == ERR_OK)
      {
        APP_DEBUG("--- Lwip: TCP connenct to Host OK\r\n");
        ucMQTTClient_Connect();
        ucMQTTClient_Publish(MQTT_TOPIC_PUBLISH, "Hello world");
      }
      else APP_DEBUG("--- Lwip: TCP Can not Connect to host\r\n");
    }
  }
  else APP_DEBUG("--- Lwip: Can not create TCP");
  while(1)
  {
  }
}

/* USER CODE END 2 */

/**
  * LwIP initialization function
  */
void MX_LWIP_Init(void)
{
  /* IP addresses initialization */
  IP_ADDRESS[0] = 10;
  IP_ADDRESS[1] = 2;
  IP_ADDRESS[2] = 6;
  IP_ADDRESS[3] = 252;
  NETMASK_ADDRESS[0] = 255;
  NETMASK_ADDRESS[1] = 255;
  NETMASK_ADDRESS[2] = 255;
  NETMASK_ADDRESS[3] = 0;
  GATEWAY_ADDRESS[0] = 10;
  GATEWAY_ADDRESS[1] = 2;
  GATEWAY_ADDRESS[2] = 6;
  GATEWAY_ADDRESS[3] = 1;
  
  /* Initilialize the LwIP stack with RTOS */
  tcpip_init( NULL, NULL );

  /* IP addresses initialization without DHCP (IPv4) */
  IP4_ADDR(&ipaddr, IP_ADDRESS[0], IP_ADDRESS[1], IP_ADDRESS[2], IP_ADDRESS[3]);
  IP4_ADDR(&netmask, NETMASK_ADDRESS[0], NETMASK_ADDRESS[1] , NETMASK_ADDRESS[2], NETMASK_ADDRESS[3]);
  IP4_ADDR(&gw, GATEWAY_ADDRESS[0], GATEWAY_ADDRESS[1], GATEWAY_ADDRESS[2], GATEWAY_ADDRESS[3]);

  /* add the network interface (IPv4/IPv6) with RTOS */
  netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &tcpip_input);

  /* Registers the default network interface */
  netif_set_default(&gnetif);

  if (netif_is_link_up(&gnetif))
  {
    /* When the netif is fully configured this function must be called */
    netif_set_up(&gnetif);
  }
  else
  {
    /* When the netif link is down this function must be called */
    netif_set_down(&gnetif);
  }

/* USER CODE BEGIN 3 */
  /*
  int mscnt =0;
  while (gnetif.ip_addr.addr == 0)
  {
    sys_msleep(DHCP_FINE_TIMER_MSECS);
    APP_DEBUG("--- Lwip: DCHP next...\r\n");
    dhcp_fine_tmr();
    mscnt += DHCP_FINE_TIMER_MSECS;
    if (mscnt >= DHCP_COARSE_TIMER_SECS*1000)
    {
      dhcp_coarse_tmr();
      mscnt = 0;
    }
  }*/
  
  APP_DEBUG("--- Lwip: %s - Started DCHP request (%s)\n", __FUNCTION__, lwip_strerr(err));
  APP_DEBUG("--- Lwip: %s - Interface is up : %s\n", __FUNCTION__, gnetif.name);// dhcp->state);
  APP_DEBUG("--- Lwip: %s - IP %s\n", __FUNCTION__, ipaddr_ntoa(&gnetif.ip_addr));
  APP_DEBUG("--- Lwip: %s - IP GW %s\n", __FUNCTION__, ipaddr_ntoa(&gnetif.gw));
  
  sys_thread_new("tcp_thread", tcp_thread, NULL, DEFAULT_THREAD_STACKSIZE, 1);  
/* USER CODE END 3 */
}
/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
