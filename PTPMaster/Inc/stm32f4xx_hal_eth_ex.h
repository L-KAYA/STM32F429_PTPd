#ifndef __STM32F4xx_HAL_ETH_EX_H
#define __STM32F4xx_HAL_ETH_EX_H

#include "stm32f4xx_hal_def.h"
/**
 *	----------------------------------LAN8742--------------------------------
 */

/** @defgroup LAN8742_SMR_Bit_Definition LAN8742 SMR Bit Definition
  * @{
  */
#define LAN8742_SMR_MODE       ((uint16_t)0x00E0U)
#define LAN8742_SMR_PHY_ADDR   ((uint16_t)0x001FU)

/** @defgroup LAN8742_Private_Defines LAN8742 Private Defines
  * @{
  */
#define LAN8742_SW_RESET_TO    ((uint32_t)500U)
#define LAN8742_INIT_TO        ((uint32_t)2000U)
#define LAN8742_MAX_DEV_ADDR   ((uint32_t)31U)

#define LAN8742_BCR      ((uint16_t)0x0000U)
#define LAN8742_BSR      ((uint16_t)0x0001U)
#define LAN8742_PHYI1R   ((uint16_t)0x0002U)
#define LAN8742_PHYI2R   ((uint16_t)0x0003U)
#define LAN8742_ANAR     ((uint16_t)0x0004U)
#define LAN8742_ANLPAR   ((uint16_t)0x0005U)
#define LAN8742_ANER     ((uint16_t)0x0006U)
#define LAN8742_ANNPTR   ((uint16_t)0x0007U)
#define LAN8742_ANNPRR   ((uint16_t)0x0008U)
#define LAN8742_MMDACR   ((uint16_t)0x000DU)
#define LAN8742_MMDAADR  ((uint16_t)0x000EU)
#define LAN8742_ENCTR    ((uint16_t)0x0010U)
#define LAN8742_MCSR     ((uint16_t)0x0011U)
#define LAN8742_SMR      ((uint16_t)0x0012U)
#define LAN8742_TPDCR    ((uint16_t)0x0018U)
#define LAN8742_TCSR     ((uint16_t)0x0019U)
#define LAN8742_SECR     ((uint16_t)0x001AU)
#define LAN8742_SCSIR    ((uint16_t)0x001BU)
#define LAN8742_CLR      ((uint16_t)0x001CU)
#define LAN8742_ISFR     ((uint16_t)0x001DU)
#define LAN8742_IMR      ((uint16_t)0x001EU)
#define LAN8742_PHYSCSR  ((uint16_t)0x001FU)

/** @defgroup LAN8742_BCR_Bit_Definition LAN8742 BCR Bit Definition
  * @{
  */    
#define LAN8742_BCR_SOFT_RESET         ((uint16_t)0x8000U)
#define LAN8742_BCR_LOOPBACK           ((uint16_t)0x4000U)
#define LAN8742_BCR_SPEED_SELECT       ((uint16_t)0x2000U)
#define LAN8742_BCR_AUTONEGO_EN        ((uint16_t)0x1000U)
#define LAN8742_BCR_POWER_DOWN         ((uint16_t)0x0800U)
#define LAN8742_BCR_ISOLATE            ((uint16_t)0x0400U)
#define LAN8742_BCR_RESTART_AUTONEGO   ((uint16_t)0x0200U)
#define LAN8742_BCR_DUPLEX_MODE        ((uint16_t)0x0100U) 

#define  LAN8742_STATUS_READ_ERROR            ((int32_t)-5)
#define  LAN8742_STATUS_WRITE_ERROR           ((int32_t)-4)
#define  LAN8742_STATUS_ADDRESS_ERROR         ((int32_t)-3)
#define  LAN8742_STATUS_RESET_TIMEOUT         ((int32_t)-2)
#define  LAN8742_STATUS_ERROR                 ((int32_t)-1)
#define  LAN8742_STATUS_OK                    ((int32_t) 0)
#define  LAN8742_STATUS_LINK_DOWN             ((int32_t) 1)
#define  LAN8742_STATUS_100MBITS_FULLDUPLEX   ((int32_t) 2)
#define  LAN8742_STATUS_100MBITS_HALFDUPLEX   ((int32_t) 3)
#define  LAN8742_STATUS_10MBITS_FULLDUPLEX    ((int32_t) 4)
#define  LAN8742_STATUS_10MBITS_HALFDUPLEX    ((int32_t) 5)
#define  LAN8742_STATUS_AUTONEGO_NOTDONE      ((int32_t) 6)


/**--------------------------------------------------------------------------**/
/** 
  * @brief                           Ethernet PTP defines
  */ 
/**--------------------------------------------------------------------------**/
/**
  * @}
  */

/** @defgroup ETH_PTP_time_update_method 
  * @{
  */ 
#define ETH_PTP_FineUpdate        ((uint32_t)0x00000001)  /*!< Fine Update method */
#define ETH_PTP_CoarseUpdate      ((uint32_t)0x00000000)  /*!< Coarse Update method */
#define IS_ETH_PTP_UPDATE(UPDATE) (((UPDATE) == ETH_PTP_FineUpdate) || \
                                   ((UPDATE) == ETH_PTP_CoarseUpdate))

/**
  * @}
  */ 


/** @defgroup ETH_PTP_Flags 
  * @{
  */ 
#define ETH_PTP_FLAG_TSARU        ((uint32_t)0x00000020)  /*!< Addend Register Update */
#define ETH_PTP_FLAG_TSITE        ((uint32_t)0x00000010)  /*!< Time Stamp Interrupt Trigger */
#define ETH_PTP_FLAG_TSSTU        ((uint32_t)0x00000008)  /*!< Time Stamp Update */
#define ETH_PTP_FLAG_TSSTI        ((uint32_t)0x00000004)  /*!< Time Stamp Initialize */

#define ETH_PTP_FLAG_TSTTR        ((uint32_t)0x10000002)  /* Time stamp target time reached */
#define ETH_PTP_FLAG_TSSO         ((uint32_t)0x10000001)  /* Time stamp seconds overflow */

#define IS_ETH_PTP_GET_FLAG(FLAG) (((FLAG) == ETH_PTP_FLAG_TSARU) || \
                                    ((FLAG) == ETH_PTP_FLAG_TSITE) || \
                                    ((FLAG) == ETH_PTP_FLAG_TSSTU) || \
                                    ((FLAG) == ETH_PTP_FLAG_TSSTI) || \
                                    ((FLAG) == ETH_PTP_FLAG_TSTTR) || \
                                    ((FLAG) == ETH_PTP_FLAG_TSSO)) 

/** 
  * @brief  ETH PTP subsecond increment  
  */ 
#define IS_ETH_PTP_SUBSECOND_INCREMENT(SUBSECOND) ((SUBSECOND) <= 0xFF)

/**
  * @}
  */ 


/** @defgroup ETH_PTP_time_sign 
  * @{
  */ 
#define ETH_PTP_PositiveTime      ((uint32_t)0x00000000)  /*!< Positive time value */
#define ETH_PTP_NegativeTime      ((uint32_t)0x80000000)  /*!< Negative time value */
#define IS_ETH_PTP_TIME_SIGN(SIGN) (((SIGN) == ETH_PTP_PositiveTime) || \
                                    ((SIGN) == ETH_PTP_NegativeTime))

/** 
  * @brief  ETH PTP time stamp low update  
  */ 
#define IS_ETH_PTP_TIME_STAMP_UPDATE_SUBSECOND(SUBSECOND) ((SUBSECOND) <= 0x7FFFFFFF)

/** 
  * @brief  ETH PTP registers  
  */ 
#define ETH_PTPTSCR     ((uint32_t)0x00000700)  /*!< PTP TSCR register */
#define ETH_PTPSSIR     ((uint32_t)0x00000704)  /*!< PTP SSIR register */
#define ETH_PTPTSHR     ((uint32_t)0x00000708)  /*!< PTP TSHR register */
#define ETH_PTPTSLR     ((uint32_t)0x0000070C)  /*!< PTP TSLR register */
#define ETH_PTPTSHUR    ((uint32_t)0x00000710)  /*!< PTP TSHUR register */
#define ETH_PTPTSLUR    ((uint32_t)0x00000714)  /*!< PTP TSLUR register */
#define ETH_PTPTSAR     ((uint32_t)0x00000718)  /*!< PTP TSAR register */
#define ETH_PTPTTHR     ((uint32_t)0x0000071C)  /*!< PTP TTHR register */
#define ETH_PTPTTLR     ((uint32_t)0x00000720)  /* PTP TTLR register */

#define ETH_PTPTSSR     ((uint32_t)0x00000728)  /* PTP TSSR register */

#define IS_ETH_PTP_REGISTER(REG) (((REG) == ETH_PTPTSCR) || ((REG) == ETH_PTPSSIR) || \
                                   ((REG) == ETH_PTPTSHR) || ((REG) == ETH_PTPTSLR) || \
                                   ((REG) == ETH_PTPTSHUR) || ((REG) == ETH_PTPTSLUR) || \
                                   ((REG) == ETH_PTPTSAR) || ((REG) == ETH_PTPTTHR) || \
                                   ((REG) == ETH_PTPTTLR) || ((REG) == ETH_PTPTSSR)) 

/** 
  * @brief  ETHERNET PTP clock  
  */ 
#define ETH_PTP_OrdinaryClock               ((uint32_t)0x00000000)  /* Ordinary Clock */
#define ETH_PTP_BoundaryClock               ((uint32_t)0x00010000)  /* Boundary Clock */
#define ETH_PTP_EndToEndTransparentClock    ((uint32_t)0x00020000)  /* End To End Transparent Clock */
#define ETH_PTP_PeerToPeerTransparentClock  ((uint32_t)0x00030000)  /* Peer To Peer Transparent Clock */

#define IS_ETH_PTP_TYPE_CLOCK(CLOCK) (((CLOCK) == ETH_PTP_OrdinaryClock) || \
                          ((CLOCK) == ETH_PTP_BoundaryClock) || \
                          ((CLOCK) == ETH_PTP_EndToEndTransparentClock) || \
                                      ((CLOCK) == ETH_PTP_PeerToPeerTransparentClock))
/** 
  * @brief  ETHERNET snapshot
  */
#define ETH_PTP_SnapshotMasterMessage          ((uint32_t)0x00008000)  /* Time stamp snapshot for message relevant to master enable */
#define ETH_PTP_SnapshotEventMessage           ((uint32_t)0x00004000)  /* Time stamp snapshot for event message enable */
#define ETH_PTP_SnapshotIPV4Frames             ((uint32_t)0x00002000)  /* Time stamp snapshot for IPv4 frames enable */
#define ETH_PTP_SnapshotIPV6Frames             ((uint32_t)0x00001000)  /* Time stamp snapshot for IPv6 frames enable */
#define ETH_PTP_SnapshotPTPOverEthernetFrames  ((uint32_t)0x00000800)  /* Time stamp snapshot for PTP over ethernet frames enable */
#define ETH_PTP_SnapshotAllReceivedFrames      ((uint32_t)0x00000100)  /* Time stamp snapshot for all received frames enable */

#define IS_ETH_PTP_SNAPSHOT(SNAPSHOT) (((SNAPSHOT) == ETH_PTP_SnapshotMasterMessage) || \
                           ((SNAPSHOT) == ETH_PTP_SnapshotEventMessage) || \
                           ((SNAPSHOT) == ETH_PTP_SnapshotIPV4Frames) || \
                           ((SNAPSHOT) == ETH_PTP_SnapshotIPV6Frames) || \
                           ((SNAPSHOT) == ETH_PTP_SnapshotPTPOverEthernetFrames) || \
                           ((SNAPSHOT) == ETH_PTP_SnapshotAllReceivedFrames))

/** 
  * @brief  ETH time stamp definition
  */ 
typedef struct {
  uint32_t TimeStampLow;
  uint32_t TimeStampHigh;
} ETH_TimeStamp;


void ETH_EnhancedDescriptorCmd(FunctionalState NewState);
void ETH_MACITConfig(uint32_t ETH_MAC_IT, FunctionalState NewState);
HAL_StatusTypeDef HAL_ETH_TransmitFrame_TimeStamp(ETH_HandleTypeDef *heth, uint32_t FrameLength, ETH_TimeStamp *TimeStamp);

/** 
  * @brief  PTP  
  */ 
void ETH_EnablePTPTimeStampAddend(void);
void ETH_EnablePTPTimeStampInterruptTrigger(void);
void ETH_EnablePTPTimeStampUpdate(void);
void ETH_InitializePTPTimeStamp(void);
void ETH_PTPUpdateMethodConfig(uint32_t UpdateMethod);
void ETH_PTPTimeStampCmd(FunctionalState NewState);
FlagStatus ETH_GetPTPFlagStatus(uint32_t ETH_PTP_FLAG);
void ETH_SetPTPSubSecondIncrement(uint32_t SubSecondValue);
void ETH_SetPTPTimeStampUpdate(uint32_t Sign, uint32_t SecondValue, uint32_t SubSecondValue);
void ETH_SetPTPTimeStampAddend(uint32_t Value);
void ETH_SetPTPTargetTime(uint32_t HighValue, uint32_t LowValue);
uint32_t ETH_GetPTPRegister(uint32_t ETH_PTPReg);


#endif //__STM32F4xx_HAL_ETH_EX_H
