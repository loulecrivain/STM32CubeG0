/**
  ******************************************************************************
  * @file    interfaces_conf.h
  * @author  MCD Application Team
  * @brief   Contains Interfaces configuration
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INTERFACES_CONF_H
#define INTERFACES_CONF_H

#define MEMORIES_SUPPORTED                7U

/* ------------------------- Definitions for USART -------------------------- */
#define USARTx                            USART1
#define USARTx_GPIO_CLK_ENABLE __HAL_RCC_GPIOA_CLK_ENABLE
#define USARTx_CLK_ENABLE __HAL_RCC_USART1_CLK_ENABLE

#define USARTx_TX_PIN                     GPIO_PIN_9
#define USARTx_TX_GPIO_PORT               GPIOA
#define USARTx_TX_PIN_REMAP(void) LL_SYSCFG_EnablePinRemap(LL_SYSCFG_PIN_RMP_PA11)

#define USARTx_RX_PIN                     GPIO_PIN_10
#define USARTx_RX_GPIO_PORT               GPIOA
#define USARTx_RX_PIN_REMAP(void) LL_SYSCFG_EnablePinRemap(LL_SYSCFG_PIN_RMP_PA12)

#define USARTx_ALTERNATE                  GPIO_AF1_USART1


#endif /* INTERFACES_CONF_H */
