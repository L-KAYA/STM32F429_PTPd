
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_eth_ex.h"

/**
  * @brief  Enables or disables the Enhanced descriptor structure.
  * @param  NewState: new state of the Enhanced descriptor structure.
  *   This parameter can be: ENABLE or DISABLE. 
  * @retval None
  */
void ETH_EnhancedDescriptorCmd(FunctionalState NewState)
{ 
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable enhanced descriptor structure */
    ETH->DMABMR |= ETH_DMABMR_EDE;  
  }
  else
  {
    /* Disable enhanced descriptor structure */
    ETH->DMABMR &= ~ETH_DMABMR_EDE;
  }
}

/**
  * @brief  Enables or disables the specified ETHERNET MAC interrupts.
  * @param  ETH_MAC_IT: specifies the ETHERNET MAC interrupt sources to be
  *   enabled or disabled.
  *   This parameter can be any combination of the following values:
  *     @arg ETH_MAC_IT_TST : Time stamp trigger interrupt 
  *     @arg ETH_MAC_IT_PMT : PMT interrupt 
  * @param  NewState: new state of the specified ETHERNET MAC interrupts.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ETH_MACITConfig(uint32_t ETH_MAC_IT, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_ETH_MAC_IT(ETH_MAC_IT));
  assert_param(IS_FUNCTIONAL_STATE(NewState));  
  
  if (NewState != DISABLE)
  {
    /* Enable the selected ETHERNET MAC interrupts */
    ETH->MACIMR &= (~(uint32_t)ETH_MAC_IT);
  }
  else
  {
    /* Disable the selected ETHERNET MAC interrupts */
    ETH->MACIMR |= ETH_MAC_IT;
  }
}


/**
  * @brief  Sends an Ethernet frame with time stamp. 
  * @param  heth pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @param  FrameLength Amount of data to be sent
  * @TimeStamp TimeStamp
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ETH_TransmitFrame_TimeStamp(ETH_HandleTypeDef *heth, uint32_t FrameLength, ETH_TimeStamp *TimeStamp)
{
  uint32_t bufcount = 0U, size = 0U, i = 0U;
  uint32_t timeout = 0;
  
  /* Process Locked */
  __HAL_LOCK(heth);
  
  /* Set the ETH peripheral state to BUSY */
  heth->State = HAL_ETH_STATE_BUSY;

	/* Check timestamp.  */
  if (TimeStamp == NULL)
  {
    /* Return ERROR: Bad timestamp structure */
		return HAL_ERROR;
  }
    /* Initialize the timestamp. */
	TimeStamp->TimeStampHigh = 0;
	TimeStamp->TimeStampLow = 0;
	
  if (FrameLength == 0U) 
  {
    /* Set ETH HAL state to READY */
    heth->State = HAL_ETH_STATE_READY;
    
    /* Process Unlocked */
    __HAL_UNLOCK(heth);
    
    return  HAL_ERROR;                                    
  }  
  
  /* Check if the descriptor is owned by the ETHERNET DMA (when set) or CPU (when reset) */
  if(((heth->TxDesc)->Status & ETH_DMATXDESC_OWN) != (uint32_t)RESET)
  {  
    /* OWN bit set */
    heth->State = HAL_ETH_STATE_BUSY_TX;
    
    /* Process Unlocked */
    __HAL_UNLOCK(heth);
    
    return HAL_ERROR;
  }
  
  /* Get the number of needed Tx buffers for the current frame */
  if (FrameLength > ETH_TX_BUF_SIZE)
  {
    bufcount = FrameLength/ETH_TX_BUF_SIZE;
    if (FrameLength % ETH_TX_BUF_SIZE) 
    {
      bufcount++;
    }
  }
  else 
  {  
    bufcount = 1U;
  }
  if (bufcount == 1U)
  {
    /* Set timestamp, LAST and FIRST segment */
    heth->TxDesc->Status |= ETH_DMATXDESC_TTSE|ETH_DMATXDESC_FS|ETH_DMATXDESC_LS;
    /* Set frame size */
    heth->TxDesc->ControlBufferSize = (FrameLength & ETH_DMATXDESC_TBS1);
    /* Set Own bit of the Tx descriptor Status: gives the buffer back to ETHERNET DMA */
    heth->TxDesc->Status |= ETH_DMATXDESC_OWN;	//该位置位后开始发送
    /* Point to next descriptor */
    //heth->TxDesc= (ETH_DMADescTypeDef *)(heth->TxDesc->Buffer2NextDescAddr);
  }
  else
  {
	/* Transmit over multiple descriptors. */
    for (i=0U; i< bufcount; i++)
    {
      /* Clear FIRST and LAST segment bits */
      heth->TxDesc->Status &= ~(ETH_DMATXDESC_FS | ETH_DMATXDESC_LS);
      /* First segment handling. */
      if (i == 0U) 
      {
        /* Setting the first segment bit */
        heth->TxDesc->Status |= ETH_DMATXDESC_FS;  
      }
      
      /* Program size */
      heth->TxDesc->ControlBufferSize = (ETH_TX_BUF_SIZE & ETH_DMATXDESC_TBS1);
      
      if (i == (bufcount-1U))
      {
        /* Setting the timestamp enable on the last segment bit */
        heth->TxDesc->Status |= ETH_DMATXDESC_TTSE|ETH_DMATXDESC_LS;
        size = FrameLength - (bufcount-1U)*ETH_TX_BUF_SIZE;
        heth->TxDesc->ControlBufferSize = (size & ETH_DMATXDESC_TBS1);
      }
      
      /* Set Own bit of the Tx descriptor Status: gives the buffer back to ETHERNET DMA */
      heth->TxDesc->Status |= ETH_DMATXDESC_OWN;
	  if (i < (bufcount-1U)){
	  	/* point to next descriptor */
      	heth->TxDesc = (ETH_DMADescTypeDef *)(heth->TxDesc->Buffer2NextDescAddr);
	  }
      
    }
  }
  
  /* When Tx Buffer unavailable flag is set: clear it and resume transmission */
  if (((heth->Instance)->DMASR & ETH_DMASR_TBUS) != (uint32_t)RESET)
  {
    /* Clear TBUS ETHERNET DMA flag */
    (heth->Instance)->DMASR = ETH_DMASR_TBUS;
    /* Resume DMA transmission*/
    (heth->Instance)->DMATPDR = 0U;
  }
  
    /* Wait for ETH_DMATXDESC_TTSS flag to be set */
  do
  {
    timeout++;
  } while (!(heth->TxDesc->Status & ETH_DMATXDESC_TTSS) && (timeout < PHY_READ_TO));

  /* Fill in time stamp of no timeout */
  if (timeout < PHY_READ_TO)
  {
	  /* Fill in the time stamp */
	  TimeStamp->TimeStampHigh = heth->TxDesc->TimeStampHigh;
	  TimeStamp->TimeStampLow = heth->TxDesc->TimeStampLow;
  }

  /* Clear the DMATxDescToSet status register TTSS flag */
  heth->TxDesc->Status &= ~ETH_DMATXDESC_TTSS;
  
  heth->TxDesc= (ETH_DMADescTypeDef *)(heth->TxDesc->Buffer2NextDescAddr);
  
  /* Set ETH HAL State to Ready */
  heth->State = HAL_ETH_STATE_READY;
  
  /* Process Unlocked */
  __HAL_UNLOCK(heth);
  
  /* Return function status */
  return HAL_OK;
}

/*---------------------------------  PTP  ------------------------------------*/

/**
  * @brief  Updated the PTP block for fine correction with the Time Stamp Addend register value.
  * @param  None
  * @retval None
  */
void ETH_EnablePTPTimeStampAddend(void)
{
  /* Enable the PTP block update with the Time Stamp Addend register value */
  ETH->PTPTSCR |= ETH_PTPTSCR_TSARU;    
}

/**
  * @brief  Enable the PTP Time Stamp interrupt trigger
  * @param  None
  * @retval None
  */
void ETH_EnablePTPTimeStampInterruptTrigger(void)
{
  /* Enable the PTP target time interrupt */
  ETH->PTPTSCR |= ETH_PTPTSCR_TSITE;    
}

/**
  * @brief  Updated the PTP system time with the Time Stamp Update register value.
  * @param  None
  * @retval None
  */
void ETH_EnablePTPTimeStampUpdate(void)
{
  /* Enable the PTP system time update with the Time Stamp Update register value */
  ETH->PTPTSCR |= ETH_PTPTSCR_TSSTU;    
}

/**
  * @brief  Initialize the PTP Time Stamp
  * @param  None
  * @retval None
  */
void ETH_InitializePTPTimeStamp(void)
{
  /* Initialize the PTP Time Stamp */
  ETH->PTPTSCR |= ETH_PTPTSCR_TSSTI;    
}

/**
  * @brief  Selects the PTP Update method
  * @param  UpdateMethod: the PTP Update method
  *   This parameter can be one of the following values:
  *     @arg ETH_PTP_FineUpdate   : Fine Update method 
  *     @arg ETH_PTP_CoarseUpdate : Coarse Update method 
  * @retval None
  */
void ETH_PTPUpdateMethodConfig(uint32_t UpdateMethod)
{
  /* Check the parameters */
  assert_param(IS_ETH_PTP_UPDATE(UpdateMethod));
  
  if (UpdateMethod != ETH_PTP_CoarseUpdate)
  {
    /* Enable the PTP Fine Update method */
    ETH->PTPTSCR |= ETH_PTPTSCR_TSFCU;
  }
  else
  {
    /* Disable the PTP Coarse Update method */
    ETH->PTPTSCR &= (~(uint32_t)ETH_PTPTSCR_TSFCU);
  } 
}

/**
  * @brief  Enables or disables the PTP time stamp for transmit and receive frames.
  * @param  NewState: new state of the PTP time stamp for transmit and receive frames
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ETH_PTPTimeStampCmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the PTP time stamp for transmit and receive frames */
    ETH->PTPTSCR |= ETH_PTPTSCR_TSE | ETH_PTPTSSR_TSSIPV4FE | ETH_PTPTSSR_TSSIPV6FE | ETH_PTPTSSR_TSSARFE;
  }
  else
  {
    /* Disable the PTP time stamp for transmit and receive frames */
    ETH->PTPTSCR &= (~(uint32_t)ETH_PTPTSCR_TSE);
  }
}

/**
  * @brief  Checks whether the specified ETHERNET PTP flag is set or not.
  * @param  ETH_PTP_FLAG: specifies the flag to check.
  *   This parameter can be one of the following values:
  *     @arg ETH_PTP_FLAG_TSARU : Addend Register Update 
  *     @arg ETH_PTP_FLAG_TSITE : Time Stamp Interrupt Trigger Enable 
  *     @arg ETH_PTP_FLAG_TSSTU : Time Stamp Update 
  *     @arg ETH_PTP_FLAG_TSSTI  : Time Stamp Initialize                       
  * @retval The new state of ETHERNET PTP Flag (SET or RESET).
  */
FlagStatus ETH_GetPTPFlagStatus(uint32_t ETH_PTP_FLAG)
{
  FlagStatus bitstatus = RESET;
  /* Check the parameters */
  assert_param(IS_ETH_PTP_GET_FLAG(ETH_PTP_FLAG));
  
  if ((ETH->PTPTSCR & ETH_PTP_FLAG) != (uint32_t)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  return bitstatus;
}

/**
  * @brief  Sets the system time Sub-Second Increment value.
  * @param  SubSecondValue: specifies the PTP Sub-Second Increment Register value.
  * @retval None
  */
void ETH_SetPTPSubSecondIncrement(uint32_t SubSecondValue)
{
  /* Check the parameters */
  assert_param(IS_ETH_PTP_SUBSECOND_INCREMENT(SubSecondValue));
  /* Set the PTP Sub-Second Increment Register */
  ETH->PTPSSIR = SubSecondValue;    
}

/**
  * @brief  Sets the Time Stamp update sign and values.
  * @param  Sign: specifies the PTP Time update value sign.
  *   This parameter can be one of the following values:
  *     @arg ETH_PTP_PositiveTime : positive time value. 
  *     @arg ETH_PTP_NegativeTime : negative time value.  
  * @param  SecondValue: specifies the PTP Time update second value. 
  * @param  SubSecondValue: specifies the PTP Time update sub-second value.
  *   This parameter is a 31 bit value, bit32 correspond to the sign.
  * @retval None
  */
void ETH_SetPTPTimeStampUpdate(uint32_t Sign, uint32_t SecondValue, uint32_t SubSecondValue)
{
  /* Check the parameters */
  assert_param(IS_ETH_PTP_TIME_SIGN(Sign));  
  assert_param(IS_ETH_PTP_TIME_STAMP_UPDATE_SUBSECOND(SubSecondValue)); 
  /* Set the PTP Time Update High Register */
  ETH->PTPTSHUR = SecondValue;
  
  /* Set the PTP Time Update Low Register with sign */
  ETH->PTPTSLUR = Sign | SubSecondValue;   
}

/**
  * @brief  Sets the Time Stamp Addend value.
  * @param  Value: specifies the PTP Time Stamp Addend Register value.
  * @retval None
  */
void ETH_SetPTPTimeStampAddend(uint32_t Value)
{
  /* Set the PTP Time Stamp Addend Register */
  ETH->PTPTSAR = Value;    
}

/**
  * @brief  Sets the Target Time registers values.
  * @param  HighValue: specifies the PTP Target Time High Register value.
  * @param  LowValue: specifies the PTP Target Time Low Register value.
  * @retval None
  */
void ETH_SetPTPTargetTime(uint32_t HighValue, uint32_t LowValue)
{
  /* Set the PTP Target Time High Register */
  ETH->PTPTTHR = HighValue;
  /* Set the PTP Target Time Low Register */
  ETH->PTPTTLR = LowValue;    
}

/**
  * @brief  Get the specified ETHERNET PTP register value.
  * @param  ETH_PTPReg: specifies the ETHERNET PTP register.
  *   This parameter can be one of the following values:
  *     @arg ETH_PTPTSCR  : Sub-Second Increment Register 
  *     @arg ETH_PTPSSIR  : Sub-Second Increment Register 
  *     @arg ETH_PTPTSHR  : Time Stamp High Register
  *     @arg ETH_PTPTSLR  : Time Stamp Low Register 
  *     @arg ETH_PTPTSHUR : Time Stamp High Update Register  
  *     @arg ETH_PTPTSLUR : Time Stamp Low Update Register
  *     @arg ETH_PTPTSAR  : Time Stamp Addend Register
  *     @arg ETH_PTPTTHR  : Target Time High Register 
  *     @arg ETH_PTPTTLR  : Target Time Low Register 
  * @retval The value of ETHERNET PTP Register value.
  */
uint32_t ETH_GetPTPRegister(uint32_t ETH_PTPReg)
{
  /* Check the parameters */
  assert_param(IS_ETH_PTP_REGISTER(ETH_PTPReg));
  
  /* Return the selected register value */
  return (*(__IO uint32_t *)(ETH_MAC_BASE + ETH_PTPReg));
}