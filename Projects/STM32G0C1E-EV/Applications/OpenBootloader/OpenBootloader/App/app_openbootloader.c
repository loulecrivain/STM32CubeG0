/**
  ******************************************************************************
  * @file    app_openbootloader.c
  * @author  MCD Application Team
  * @brief   OpenBootloader application entry point
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
#include "main.h"
#include "app_openbootloader.h"
#include "usart_interface.h"
#include "fdcan_interface.h"
#include "i2c_interface.h"
#include "usb_interface.h"
#include "flash_interface.h"
#include "ram_interface.h"
#include "optionbytes_interface.h"
#include "iwdg_interface.h"
#include "otp_interface.h"
#include "engibytes_interface.h"
#include "systemmemory_interface.h"
#include "openbl_usart_cmd.h"
#include "openbl_fdcan_cmd.h"
#include "openbl_i2c_cmd.h"
#include "openbl_usb_cmd.h"
#include "openbl_core.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static OPENBL_HandleTypeDef USART_Handle;
static OPENBL_HandleTypeDef FDCAN_Handle;
static OPENBL_HandleTypeDef I2C_Handle;
static OPENBL_HandleTypeDef USB_Handle;
static OPENBL_HandleTypeDef IWDG_Handle;

static OPENBL_OpsTypeDef USART_Ops =
{
  OPENBL_USART_Configuration,
  NULL,
  OPENBL_USART_ProtocolDetection,
  OPENBL_USART_GetCommandOpcode,
  OPENBL_USART_SendByte
};

static OPENBL_OpsTypeDef FDCAN_Ops =
{
  OPENBL_FDCAN_Configuration,
  NULL,
  OPENBL_FDCAN_ProtocolDetection,
  OPENBL_FDCAN_GetCommandOpcode,
  OPENBL_FDCAN_SendByte
};

static OPENBL_OpsTypeDef I2C_Ops =
{
  OPENBL_I2C_Configuration,
  NULL,
  OPENBL_I2C_ProtocolDetection,
  OPENBL_I2C_GetCommandOpcode,
  OPENBL_I2C_SendAcknowledgeByte
};

static OPENBL_OpsTypeDef USB_Ops =
{
  OPENBL_USB_Configuration,
  NULL,
  OPENBL_USB_ProtocolDetection,
  NULL,
  NULL
};

static OPENBL_OpsTypeDef IWDG_Ops =
{
  OPENBL_IWDG_Configuration,
  NULL,
  NULL,
  NULL,
  NULL
};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initialize open Bootloader.
  * @param  None.
  * @retval None.
  */
void OpenBootloader_Init(void)
{
  /* Register USART interfaces */
  USART_Handle.p_Ops = &USART_Ops;
  USART_Handle.p_Cmd = OPENBL_USART_GetCommandsList();

  OPENBL_RegisterInterface(&USART_Handle);

  /* Register FDCAN interfaces */
  FDCAN_Handle.p_Ops = &FDCAN_Ops;
  FDCAN_Handle.p_Cmd = OPENBL_FDCAN_GetCommandsList();

  OPENBL_RegisterInterface(&FDCAN_Handle);

  /* Register I2C interfaces */
  I2C_Handle.p_Ops = &I2C_Ops;
  I2C_Handle.p_Cmd = OPENBL_I2C_GetCommandsList();

  OPENBL_RegisterInterface(&I2C_Handle);

  /* Register USB interfaces */
  USB_Handle.p_Ops = &USB_Ops;

  OPENBL_RegisterInterface(&USB_Handle);

  /* Register IWDG interfaces */
  IWDG_Handle.p_Ops = &IWDG_Ops;
  IWDG_Handle.p_Cmd = NULL;

  OPENBL_RegisterInterface(&IWDG_Handle);

  /* Initialise interfaces */
  OPENBL_Init();

  /* Initialise memories */
  OPENBL_MEM_RegisterMemory(&FLASH_Descriptor);
  OPENBL_MEM_RegisterMemory(&RAM_Descriptor);
  OPENBL_MEM_RegisterMemory(&OB_Descriptor);
  OPENBL_MEM_RegisterMemory(&OTP_Descriptor);
  OPENBL_MEM_RegisterMemory(&ICP1_Descriptor);
  OPENBL_MEM_RegisterMemory(&ICP2_Descriptor);
  OPENBL_MEM_RegisterMemory(&EB_Descriptor);
}

/**
  * @brief  DeInitialize open Bootloader.
  * @param  None.
  * @retval None.
  */
void OpenBootloader_DeInit(void)
{
  System_DeInit();
}

/**
  * @brief  This function is used to select which protocol will be used when communicating with the host.
  * @param  None.
  * @retval None.
  */
void OpenBootloader_ProtocolDetection(void)
{
  static uint32_t interface_detected = 0;

  if (interface_detected == 0)
  {
    interface_detected = OPENBL_InterfaceDetection();
  }

  if (interface_detected == 1)
  {
    OPENBL_CommandProcess();
  }
}
