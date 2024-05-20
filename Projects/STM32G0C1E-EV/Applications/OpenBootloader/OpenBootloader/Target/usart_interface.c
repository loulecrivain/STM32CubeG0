/**
  ******************************************************************************
  * @file    usart_interface.c
  * @author  MCD Application Team
  * @brief   Contains USART HW configuration
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "platform.h"
#include "openbl_core.h"
#include "openbl_usart_cmd.h"
#include "usart_interface.h"
#include "iwdg_interface.h"
#include "interfaces_conf.h"

//#define HFR_USE_HAL
#define HFR_NO_AUTOBAUDRATE
//#define HFR_LOOPBACK_TEST_MINICOM

#ifdef HFR_USE_HAL

UART_HandleTypeDef huart;

/* Buffer used for transmission */
uint8_t aTxBuffer[] = "CAFEFADE\n";
#define TXBUFFERSIZE                      (sizeof(aTxBuffer))

/* Buffer used for reception */
#define RXBUFFERSIZE                      TXBUFFERSIZE
uint8_t aRxBuffer[RXBUFFERSIZE];

#endif


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void OPENBL_USART_Init(void);

/* Private functions ---------------------------------------------------------*/

static void OPENBL_USART_Init(void)
{
#ifdef HFR_USE_HAL
  huart1.Instance = USART1;
  huart.Init.BaudRate = 115200;
  huart.Init.WordLength = UART_WORDLENGTH_9B;
  huart.Init.StopBits = UART_STOPBITS_1;
  huart.Init.Parity = UART_PARITY_EVEN;
  huart.Init.Mode = UART_MODE_TX_RX;
  huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart.Init.OverSampling = UART_OVERSAMPLING_16;
  huart.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
#ifdef HFR_LOOPBACK_TEST_MINICOM
  huart.Init.WordLength = UART_WORDLENGTH_8B;
  huart.Init.Parity = UART_PARITY_NONE;
#endif
  HAL_UART_Init(&huart);
#else
  LL_USART_InitTypeDef USART_InitStruct;

  USART_InitStruct.PrescalerValue      = LL_USART_PRESCALER_DIV1;
  USART_InitStruct.BaudRate            = 115200U;
  USART_InitStruct.DataWidth           = LL_USART_DATAWIDTH_9B;
  USART_InitStruct.StopBits            = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity              = LL_USART_PARITY_EVEN;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.TransferDirection   = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.OverSampling        = LL_USART_OVERSAMPLING_16;
#ifdef HFR_LOOPBACK_TEST_MINICOM
  USART_InitStruct.DataWidth           = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.Parity              = LL_USART_PARITY_NONE;
#endif


#ifndef HFR_NO_AUTOBAUDRATE
  if (IS_USART_AUTOBAUDRATE_DETECTION_INSTANCE(USARTx))
  {
    LL_USART_EnableAutoBaudRate(USARTx);
    LL_USART_SetAutoBaudRateMode(USARTx, LL_USART_AUTOBAUD_DETECT_ON_7F_FRAME);
  }
  else
#endif
  {
    LL_USART_DisableAutoBaudRate(USARTx);
    USART_InitStruct.BaudRate = 115200;
  }

  LL_USART_Init(USARTx, &USART_InitStruct);
  LL_USART_Enable(USARTx);
#endif
}

/* Exported functions --------------------------------------------------------*/

/**
 * @brief  This function is used to configure USART pins and then initialize the used USART instance.
 * @retval None.
 */
void OPENBL_USART_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* Enable all resources clocks --------------------------------------------*/
  /* Enable used GPIOx clocks */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* Enable USART clock */
  __HAL_RCC_USART1_CLK_ENABLE();

  /* USARTx pins configuration -----------------------------------------------*/

  GPIO_InitStruct.Pin       = USARTx_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = USARTx_ALTERNATE;
  HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = USARTx_RX_PIN;
  HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);

  OPENBL_USART_Init();
}

/**
 * @brief  This function is used to detect if there is any activity on USART protocol.
 * @retval None.
 */
uint8_t OPENBL_USART_ProtocolDetection(void)
{
  uint8_t detected;
  uint8_t rx;

#ifdef HFR_LOOPBACK_TEST_MINICOM
  rx = OPENBL_USART_ReadByte();
  OPENBL_USART_SendByte(rx);
#endif

  /* Check if the USARTx is addressed */
//HFR KO...  if (((USARTx->ISR & LL_USART_ISR_ABRF) != 0) && ((USARTx->ISR & LL_USART_ISR_ABRE) == 0))
  rx = OPENBL_USART_ReadByte();
  if (rx)
  {
 //   test_point(5);

    /* Read byte in order to flush the 0x7F synchronization byte */
//    OPENBL_USART_ReadByte();

    /* Acknowledge the host */
    OPENBL_USART_SendByte(ACK_BYTE);

    detected = 1;
  }
  else
  {
    detected = 0;
  }

  return detected;
}

/**
 * @brief  This function is used to get the command opcode from the host.
 * @retval Returns the command.
 */
uint8_t OPENBL_USART_GetCommandOpcode(void)
{
  uint8_t command_opc = 0x0;

  /* Get the command opcode */
  command_opc = OPENBL_USART_ReadByte();

  /* Check the data integrity */
  if ((command_opc ^ OPENBL_USART_ReadByte()) != 0xFF)
  {
    command_opc = ERROR_COMMAND;
  }
  else
  {
    /* nothing to do */
  }

  return command_opc;
}

/**
  * @brief  This function is used to read one byte from USART pipe.
  * @retval Returns the read byte.
  */
uint8_t OPENBL_USART_ReadByte(void)
{
#if HFR_USE_HAL
  return 0;
#else
  while (!LL_USART_IsActiveFlag_RXNE_RXFNE(USARTx))
  {
    OPENBL_IWDG_Refresh();
  }

  return LL_USART_ReceiveData8(USARTx);
#endif
}

/**
  * @brief  This function is used to send one byte through USART pipe.
  * @param  Byte The byte to be sent.
  * @retval None.
  */
void OPENBL_USART_SendByte(uint8_t Byte)
{
#if HFR_USE_HAL
 HAL_UART_Transmit(&huart, (uint8_t *)aTxBuffer, TXBUFFERSIZE);
#else
  LL_USART_TransmitData8(USARTx, (Byte & 0xFF));

  while (!LL_USART_IsActiveFlag_TC(USARTx))
  {
  }
#endif
}
