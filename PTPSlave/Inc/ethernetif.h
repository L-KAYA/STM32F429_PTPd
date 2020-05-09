/**
 ******************************************************************************
  * File Name          : ethernetif.h
  * Description        : This file provides initialization code for LWIP
  *                      middleWare.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
  

#ifndef __ETHERNETIF_H__
#define __ETHERNETIF_H__

#include "lwip/err.h"
#include "lwip/netif.h"
#include "cmsis_os.h"

/* Exported types ------------------------------------------------------------*/
/* Structure that include link thread parameters */
struct link_str {
  struct netif *netif;
  osSemaphoreId semaphore;
};

/* Within 'USER CODE' section, code will be kept by default at each generation */
/* USER CODE BEGIN 0 */
struct ptptime_t {
  s32_t tv_sec;
  s32_t tv_nsec;
};

  /* Examples of subsecond increment and addend values using SysClk = 72 MHz
   
   Addend * Increment = 2^63 / SysClk

    ptp_tick = Increment * 10^9 / 2^31
  
   +-----------+-----------+------------+
   | ptp tick  | Increment | Addend     |
   +-----------+-----------+------------+
   |  119 ns   |   255     | 0x1DF170C7 |
   |  100 ns   |   215     | 0x238391AA |
   |   50 ns   |   107     | 0x475C1B20 |
   |   20 ns   |    43     | 0xB191D856 |
   |   14 ns   |    30     | 0xFE843E9E |
   +-----------+-----------+------------+
  */

  /* Examples of subsecond increment and addend values using SysClk = 180 MHz
   
   Addend * Increment = 2^63 / SysClk

    ptp_tick = Increment * 10^9 / 2^31
  
   +-----------+-----------+------------+
   | ptp tick  | Increment | Addend     |
   +-----------+-----------+------------+
   |   5.58 ns |    12     | 0xFE843E9E |
   +-----------+-----------+------------+
  */

#define ADJ_FREQ_BASE_ADDEND      0xFE843E9E
#define ADJ_FREQ_BASE_INCREMENT   12

/* USER CODE END 0 */

/* Exported functions ------------------------------------------------------- */
err_t ethernetif_init(struct netif *netif);

void ethernetif_input( void const * argument );
void ethernetif_set_link(void const *argument);
void ethernetif_update_config(struct netif *netif);
void ethernetif_notify_conn_changed(struct netif *netif);

u32_t sys_jiffies(void);
u32_t sys_now(void);

/* USER CODE BEGIN 1 */
err_t raw_sendto(struct pbuf * p);

void ETH_PTPTime_SetTime(struct ptptime_t * timestamp);
void ETH_PTPTime_GetTime(struct ptptime_t * timestamp);

void ETH_PTPTime_UpdateOffset(struct ptptime_t * timeoffset);
void ETH_PTPTime_AdjFreq(int32_t Adj);
/* USER CODE END 1 */
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
