/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @brief   This file provides main program functions
  ******************************************************************************
   * @attention
   *
   * Copyright (c) 2019-2020 STMicroelectronics.
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
#include "stm32g0316_discovery.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void           SystemClockConfig(void);
static void           ErrorHandler(void);

/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/**
* @brief  Main program
* @param  None
* @retval int
*/
int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* LED init */
  BSP_LED_Init(LED2);

  /* Configure the system clock */
  SystemClockConfig();

  /* HFR */
  while(1)
  {
    static unsigned char cnt;

    BSP_LED_On(LED2);
    HAL_Delay(cnt);

    BSP_LED_Off(LED2);
    HAL_Delay(cnt);

    cnt+=20;
  }
}

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow:
  *         in case og NRST Mode:
  *           System Clock source            = HSI
  *           SYSCLK(Hz)                     = 16000000
  *           HCLK(Hz)                       = 16000000
  *           AHB Prescaler                  = 1
  *           APB1 Prescaler                 = 1
  *           Flash Latency(WS)              = 1
  *         in case GPIO Mode:
  *            System Clock source            = PLL (HSI)
  *            SYSCLK(Hz)                     = 64000000
  *            HCLK(Hz)                       = 64000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            HSI Frequency(Hz)              = 16000000
  *            PLLM                           = 4
  *            PLLN                           = 64
  *            PLLP                           = 16
  *            PLLQ                           = 5
  *            PLLR                           = 2
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
static void SystemClockConfig()
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  uint32_t flashlatency;

  /* Set common parameters depending on subdemo mode */
  RCC_OscInitStruct.OscillatorType      = (RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSI);
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.HSIState            = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv              = RCC_HSI_DIV1;
  RCC_OscInitStruct.LSIState            = RCC_LSI_ON;

  RCC_ClkInitStruct.ClockType      = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1);
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  /* Set HSI as PLL source to use it for max 64MHz frequency */
  RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM            = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN            = 64;
  RCC_OscInitStruct.PLL.PLLP            = RCC_PLLP_DIV16;
  RCC_OscInitStruct.PLL.PLLQ            = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR            = RCC_PLLR_DIV4;

  /* Set PLL as SYSCLK source and Flash latency */
  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
  flashlatency = FLASH_LATENCY_2;

  /* Increase voltage scaling : set range 1 (default value) */
  if(HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    ErrorHandler();
  }

  /* Configure oscillator */
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK)
  {
    ErrorHandler();
  }

  /* Configure clocks */
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, flashlatency)!= HAL_OK)
  {
    ErrorHandler();
  }
}

/**
  * @brief  functional error report to user: LED stay on forever
  * @param  None
  * @retval None
  */
static void ErrorHandler(void)
{
  /* Turn LED On */
  BSP_LED_On(LED2);

  /* Infinite loop */
  while(1)
  {
  }
}
