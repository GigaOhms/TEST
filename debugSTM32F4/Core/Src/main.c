/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
float m = 1.0;
#define TOP0        3599.0
uint32_t i = 0;
int _write(int file, char *ptr, int len){
  /* Implement your write code here, this is used by puts and printf for example */
  int i=0;
  for(i=0 ; i<len ; i++)
    ITM_SendChar((*ptr++));
  return len;
}
uint32_t s1, s2, s3;
float va[201] = {0.5000, 0.5302, 0.5604, 0.5905, 0.6205, 0.6355, 0.6652, 0.6948, 0.7241, 0.7532, 0.7676, 0.7962, 0.8244, 0.8522, 0.8796, 0.8932, 0.9199, 0.9373, 0.9455, 0.9532, 0.9568, 0.9636, 0.9698, 0.9755, 0.9806, 0.9830, 0.9872, 0.9908, 0.9938, 0.9963, 0.9981, 0.9988, 0.9997, 1.0000, 0.9997, 0.9988, 0.9981, 0.9963, 0.9938, 0.9908, 0.9872, 0.9851, 0.9806, 0.9755, 0.9698, 0.9668, 0.9603, 0.9532, 0.9455, 0.9373, 0.9330, 0.9415, 0.9494, 0.9568, 0.9636, 0.9668, 0.9728, 0.9782, 0.9830, 0.9872, 0.9891, 0.9924, 0.9951, 0.9973, 0.9988, 0.9993, 0.9999, 0.9999, 0.9993, 0.9981, 0.9973, 0.9951, 0.9924, 0.9891, 0.9851, 0.9830, 0.9782, 0.9728, 0.9668, 0.9603, 0.9568, 0.9494, 0.9415, 0.9330, 0.9066, 0.8932, 0.8660, 0.8384, 0.8104, 0.7820, 0.7676, 0.7387, 0.7095, 0.6801, 0.6504, 0.6355, 0.6055, 0.5755, 0.5453, 0.5151, 0.5000, 0.4698, 0.4396, 0.4095, 0.3795, 0.3645, 0.3348, 0.3052, 0.2759, 0.2468, 0.2324, 0.2038, 0.1756, 0.1478, 0.1204, 0.1068, 0.0801, 0.0627, 0.0545, 0.0468, 0.0432, 0.0364, 0.0302, 0.0245, 0.0194, 0.0170, 0.0128, 0.0092, 0.0062, 0.0037, 0.0027, 0.0012, 0.0003, 0.0000, 0.0003, 0.0007, 0.0019, 0.0037, 0.0062, 0.0092, 0.0109, 0.0149, 0.0194, 0.0245, 0.0302, 0.0332, 0.0397, 0.0468, 0.0545, 0.0627, 0.0670, 0.0585, 0.0506, 0.0432, 0.0364, 0.0332, 0.0272, 0.0218, 0.0170, 0.0128, 0.0109, 0.0076, 0.0049, 0.0027, 0.0012, 0.0007, 0.0001, 0.0001, 0.0007, 0.0019, 0.0027, 0.0049, 0.0076, 0.0109, 0.0149, 0.0170, 0.0218, 0.0272, 0.0332, 0.0397, 0.0432, 0.0506, 0.0585, 0.0670, 0.0934, 0.1068, 0.1340, 0.1616, 0.1896, 0.2180, 0.2324, 0.2613, 0.2905, 0.3199, 0.3496, 0.3645, 0.3945, 0.4245, 0.4547, 0.4849};
float vb[201] = {1.0000, 0.9997, 0.9988, 0.9973, 0.9951, 0.9938, 0.9908, 0.9872, 0.9830, 0.9782, 0.9755, 0.9698, 0.9636, 0.9568, 0.9494, 0.9455, 0.9373, 0.9199, 0.8932, 0.8660, 0.8522, 0.8244, 0.7962, 0.7676, 0.7387, 0.7241, 0.6948, 0.6652, 0.6355, 0.6055, 0.5755, 0.5604, 0.5302, 0.5000, 0.4698, 0.4396, 0.4245, 0.3945, 0.3645, 0.3348, 0.3052, 0.2905, 0.2613, 0.2324, 0.2038, 0.1896, 0.1616, 0.1340, 0.1068, 0.0801, 0.0670, 0.0585, 0.0506, 0.0432, 0.0364, 0.0332, 0.0272, 0.0218, 0.0170, 0.0128, 0.0109, 0.0076, 0.0049, 0.0027, 0.0012, 0.0007, 0.0001, 0.0001, 0.0007, 0.0019, 0.0027, 0.0049, 0.0076, 0.0109, 0.0149, 0.0170, 0.0218, 0.0272, 0.0332, 0.0397, 0.0432, 0.0506, 0.0585, 0.0670, 0.0585, 0.0545, 0.0468, 0.0397, 0.0332, 0.0272, 0.0245, 0.0194, 0.0149, 0.0109, 0.0076, 0.0062, 0.0037, 0.0019, 0.0007, 0.0001, 0.0000, 0.0003, 0.0012, 0.0027, 0.0049, 0.0062, 0.0092, 0.0128, 0.0170, 0.0218, 0.0245, 0.0302, 0.0364, 0.0432, 0.0506, 0.0545, 0.0627, 0.0801, 0.1068, 0.1340, 0.1478, 0.1756, 0.2038, 0.2324, 0.2613, 0.2759, 0.3052, 0.3348, 0.3645, 0.3945, 0.4095, 0.4396, 0.4698, 0.5000, 0.5302, 0.5453, 0.5755, 0.6055, 0.6355, 0.6652, 0.6801, 0.7095, 0.7387, 0.7676, 0.7962, 0.8104, 0.8384, 0.8660, 0.8932, 0.9199, 0.9330, 0.9415, 0.9494, 0.9568, 0.9636, 0.9668, 0.9728, 0.9782, 0.9830, 0.9872, 0.9891, 0.9924, 0.9951, 0.9973, 0.9988, 0.9993, 0.9999, 0.9999, 0.9993, 0.9981, 0.9973, 0.9951, 0.9924, 0.9891, 0.9851, 0.9830, 0.9782, 0.9728, 0.9668, 0.9603, 0.9568, 0.9494, 0.9415, 0.9330, 0.9415, 0.9455, 0.9532, 0.9603, 0.9668, 0.9728, 0.9755, 0.9806, 0.9851, 0.9891, 0.9924, 0.9938, 0.9963, 0.9981, 0.9993, 0.9999};
float vc[201] = {0.0000, 0.0003, 0.0012, 0.0027, 0.0049, 0.0062, 0.0092, 0.0128, 0.0170, 0.0218, 0.0245, 0.0302, 0.0364, 0.0432, 0.0506, 0.0545, 0.0627, 0.0627, 0.0545, 0.0468, 0.0432, 0.0364, 0.0302, 0.0245, 0.0194, 0.0170, 0.0128, 0.0092, 0.0062, 0.0037, 0.0019, 0.0012, 0.0003, 0.0000, 0.0003, 0.0012, 0.0019, 0.0037, 0.0062, 0.0092, 0.0128, 0.0149, 0.0194, 0.0245, 0.0302, 0.0332, 0.0397, 0.0468, 0.0545, 0.0627, 0.0670, 0.0934, 0.1204, 0.1478, 0.1756, 0.1896, 0.2180, 0.2468, 0.2759, 0.3052, 0.3199, 0.3496, 0.3795, 0.4095, 0.4396, 0.4547, 0.4849, 0.5151, 0.5453, 0.5755, 0.5905, 0.6205, 0.6504, 0.6801, 0.7095, 0.7241, 0.7532, 0.7820, 0.8104, 0.8384, 0.8522, 0.8796, 0.9066, 0.9330, 0.9415, 0.9455, 0.9532, 0.9603, 0.9668, 0.9728, 0.9755, 0.9806, 0.9851, 0.9891, 0.9924, 0.9938, 0.9963, 0.9981, 0.9993, 0.9999, 1.0000, 0.9997, 0.9988, 0.9973, 0.9951, 0.9938, 0.9908, 0.9872, 0.9830, 0.9782, 0.9755, 0.9698, 0.9636, 0.9568, 0.9494, 0.9455, 0.9373, 0.9373, 0.9455, 0.9532, 0.9568, 0.9636, 0.9698, 0.9755, 0.9806, 0.9830, 0.9872, 0.9908, 0.9938, 0.9963, 0.9973, 0.9988, 0.9997, 1.0000, 0.9997, 0.9993, 0.9981, 0.9963, 0.9938, 0.9908, 0.9891, 0.9851, 0.9806, 0.9755, 0.9698, 0.9668, 0.9603, 0.9532, 0.9455, 0.9373, 0.9330, 0.9066, 0.8796, 0.8522, 0.8244, 0.8104, 0.7820, 0.7532, 0.7241, 0.6948, 0.6801, 0.6504, 0.6205, 0.5905, 0.5604, 0.5453, 0.5151, 0.4849, 0.4547, 0.4245, 0.4095, 0.3795, 0.3496, 0.3199, 0.2905, 0.2759, 0.2468, 0.2180, 0.1896, 0.1616, 0.1478, 0.1204, 0.0934, 0.0670, 0.0585, 0.0545, 0.0468, 0.0397, 0.0332, 0.0272, 0.0245, 0.0194, 0.0149, 0.0109, 0.0076, 0.0062, 0.0037, 0.0019, 0.0007, 0.0001};

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
//	  HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
//	  HAL_Delay(1000);
//	  printf("counter i = %ld\n", i);
	    s1 = ((va[i] - 0.5) * m + 0.5) * TOP0 + 0.5;           // PWM Pin A0
	    s2 = ((vb[i] - 0.5) * m + 0.5) * TOP0 + 0.5;           // PWM Pin A1
	    s3 = ((vc[i] - 0.5) * m + 0.5) * TOP0 + 0.5;           // PWM Pin A2

	  i++;
	  if (i >= 200) i = 0;
	  HAL_Delay(50);
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pins : PD12 PD13 PD14 PD15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
