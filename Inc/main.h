/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void enter_bootloader_mode(void);
void USER_CODE_Loop(void);
void USER_CODE_Init(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED1_Pin GPIO_PIN_13
#define LED1_GPIO_Port GPIOC
#define MODE_FIX_Pin GPIO_PIN_1
#define MODE_FIX_GPIO_Port GPIOB
#define BOOTLOADER_Pin GPIO_PIN_2
#define BOOTLOADER_GPIO_Port GPIOB
#define ACTION_HOME_Pin GPIO_PIN_10
#define ACTION_HOME_GPIO_Port GPIOB
#define ACTION_AWAY_Pin GPIO_PIN_11
#define ACTION_AWAY_GPIO_Port GPIOB
#define ACTION_HOME_OLD_Pin GPIO_PIN_12
#define ACTION_HOME_OLD_GPIO_Port GPIOB
#define ACTION_AWAY_OLD_Pin GPIO_PIN_13
#define ACTION_AWAY_OLD_GPIO_Port GPIOB
#define MODE_FIX_OLD_Pin GPIO_PIN_14
#define MODE_FIX_OLD_GPIO_Port GPIOB
#define TIMMING_Pin GPIO_PIN_15
#define TIMMING_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

#if CLOCK_SUPPORTED
#define SCORE_SUPPORTED (0)
#else
#define SCORE_SUPPORTED (1)
#endif
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
