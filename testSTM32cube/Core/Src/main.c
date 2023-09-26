#include "main.h"
#include "stdint.h"

ADC_HandleTypeDef hadc1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;


void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);


#define TOP0        3599.0
#define F           50.0
#define SQRT3	    1.7320508075688772935274463415059
#define GAIN		10.1			// Cau phan ap
#define VDC         85.5			// Nguon VDC nghich luu
#define ONE_DIV_VDC  0.01169590643 	// 1/VDC
#define PU          0.000244140625
#define ONE_DIV_200	0.0050
#define UMAX		0.8
#define UMIN		0.1

volatile uint16_t i = 0, analogA5 = 0;
volatile uint16_t S1, S2, S3;
volatile uint8_t STT;
volatile double t = 0.0, m = 0.8, VAVG = 1000.0, sumAnalog = 0.0, VREF = 2.60;

float va[201] = {0.5000, 0.5302, 0.5604, 0.5905, 0.6205, 0.6355, 0.6652, 0.6948, 0.7241, 0.7532, 0.7676, 0.7962, 0.8244, 0.8522, 0.8796, 0.8932, 0.9199, 0.9373, 0.9455, 0.9532, 0.9568, 0.9636, 0.9698, 0.9755, 0.9806, 0.9830, 0.9872, 0.9908, 0.9938, 0.9963, 0.9981, 0.9988, 0.9997, 1.0000, 0.9997, 0.9988, 0.9981, 0.9963, 0.9938, 0.9908, 0.9872, 0.9851, 0.9806, 0.9755, 0.9698, 0.9668, 0.9603, 0.9532, 0.9455, 0.9373, 0.9330, 0.9415, 0.9494, 0.9568, 0.9636, 0.9668, 0.9728, 0.9782, 0.9830, 0.9872, 0.9891, 0.9924, 0.9951, 0.9973, 0.9988, 0.9993, 0.9999, 0.9999, 0.9993, 0.9981, 0.9973, 0.9951, 0.9924, 0.9891, 0.9851, 0.9830, 0.9782, 0.9728, 0.9668, 0.9603, 0.9568, 0.9494, 0.9415, 0.9330, 0.9066, 0.8932, 0.8660, 0.8384, 0.8104, 0.7820, 0.7676, 0.7387, 0.7095, 0.6801, 0.6504, 0.6355, 0.6055, 0.5755, 0.5453, 0.5151, 0.5000, 0.4698, 0.4396, 0.4095, 0.3795, 0.3645, 0.3348, 0.3052, 0.2759, 0.2468, 0.2324, 0.2038, 0.1756, 0.1478, 0.1204, 0.1068, 0.0801, 0.0627, 0.0545, 0.0468, 0.0432, 0.0364, 0.0302, 0.0245, 0.0194, 0.0170, 0.0128, 0.0092, 0.0062, 0.0037, 0.0027, 0.0012, 0.0003, 0.0000, 0.0003, 0.0007, 0.0019, 0.0037, 0.0062, 0.0092, 0.0109, 0.0149, 0.0194, 0.0245, 0.0302, 0.0332, 0.0397, 0.0468, 0.0545, 0.0627, 0.0670, 0.0585, 0.0506, 0.0432, 0.0364, 0.0332, 0.0272, 0.0218, 0.0170, 0.0128, 0.0109, 0.0076, 0.0049, 0.0027, 0.0012, 0.0007, 0.0001, 0.0001, 0.0007, 0.0019, 0.0027, 0.0049, 0.0076, 0.0109, 0.0149, 0.0170, 0.0218, 0.0272, 0.0332, 0.0397, 0.0432, 0.0506, 0.0585, 0.0670, 0.0934, 0.1068, 0.1340, 0.1616, 0.1896, 0.2180, 0.2324, 0.2613, 0.2905, 0.3199, 0.3496, 0.3645, 0.3945, 0.4245, 0.4547, 0.4849};
float vb[201] = {1.0000, 0.9997, 0.9988, 0.9973, 0.9951, 0.9938, 0.9908, 0.9872, 0.9830, 0.9782, 0.9755, 0.9698, 0.9636, 0.9568, 0.9494, 0.9455, 0.9373, 0.9199, 0.8932, 0.8660, 0.8522, 0.8244, 0.7962, 0.7676, 0.7387, 0.7241, 0.6948, 0.6652, 0.6355, 0.6055, 0.5755, 0.5604, 0.5302, 0.5000, 0.4698, 0.4396, 0.4245, 0.3945, 0.3645, 0.3348, 0.3052, 0.2905, 0.2613, 0.2324, 0.2038, 0.1896, 0.1616, 0.1340, 0.1068, 0.0801, 0.0670, 0.0585, 0.0506, 0.0432, 0.0364, 0.0332, 0.0272, 0.0218, 0.0170, 0.0128, 0.0109, 0.0076, 0.0049, 0.0027, 0.0012, 0.0007, 0.0001, 0.0001, 0.0007, 0.0019, 0.0027, 0.0049, 0.0076, 0.0109, 0.0149, 0.0170, 0.0218, 0.0272, 0.0332, 0.0397, 0.0432, 0.0506, 0.0585, 0.0670, 0.0585, 0.0545, 0.0468, 0.0397, 0.0332, 0.0272, 0.0245, 0.0194, 0.0149, 0.0109, 0.0076, 0.0062, 0.0037, 0.0019, 0.0007, 0.0001, 0.0000, 0.0003, 0.0012, 0.0027, 0.0049, 0.0062, 0.0092, 0.0128, 0.0170, 0.0218, 0.0245, 0.0302, 0.0364, 0.0432, 0.0506, 0.0545, 0.0627, 0.0801, 0.1068, 0.1340, 0.1478, 0.1756, 0.2038, 0.2324, 0.2613, 0.2759, 0.3052, 0.3348, 0.3645, 0.3945, 0.4095, 0.4396, 0.4698, 0.5000, 0.5302, 0.5453, 0.5755, 0.6055, 0.6355, 0.6652, 0.6801, 0.7095, 0.7387, 0.7676, 0.7962, 0.8104, 0.8384, 0.8660, 0.8932, 0.9199, 0.9330, 0.9415, 0.9494, 0.9568, 0.9636, 0.9668, 0.9728, 0.9782, 0.9830, 0.9872, 0.9891, 0.9924, 0.9951, 0.9973, 0.9988, 0.9993, 0.9999, 0.9999, 0.9993, 0.9981, 0.9973, 0.9951, 0.9924, 0.9891, 0.9851, 0.9830, 0.9782, 0.9728, 0.9668, 0.9603, 0.9568, 0.9494, 0.9415, 0.9330, 0.9415, 0.9455, 0.9532, 0.9603, 0.9668, 0.9728, 0.9755, 0.9806, 0.9851, 0.9891, 0.9924, 0.9938, 0.9963, 0.9981, 0.9993, 0.9999};
float vc[201] = {0.0000, 0.0003, 0.0012, 0.0027, 0.0049, 0.0062, 0.0092, 0.0128, 0.0170, 0.0218, 0.0245, 0.0302, 0.0364, 0.0432, 0.0506, 0.0545, 0.0627, 0.0627, 0.0545, 0.0468, 0.0432, 0.0364, 0.0302, 0.0245, 0.0194, 0.0170, 0.0128, 0.0092, 0.0062, 0.0037, 0.0019, 0.0012, 0.0003, 0.0000, 0.0003, 0.0012, 0.0019, 0.0037, 0.0062, 0.0092, 0.0128, 0.0149, 0.0194, 0.0245, 0.0302, 0.0332, 0.0397, 0.0468, 0.0545, 0.0627, 0.0670, 0.0934, 0.1204, 0.1478, 0.1756, 0.1896, 0.2180, 0.2468, 0.2759, 0.3052, 0.3199, 0.3496, 0.3795, 0.4095, 0.4396, 0.4547, 0.4849, 0.5151, 0.5453, 0.5755, 0.5905, 0.6205, 0.6504, 0.6801, 0.7095, 0.7241, 0.7532, 0.7820, 0.8104, 0.8384, 0.8522, 0.8796, 0.9066, 0.9330, 0.9415, 0.9455, 0.9532, 0.9603, 0.9668, 0.9728, 0.9755, 0.9806, 0.9851, 0.9891, 0.9924, 0.9938, 0.9963, 0.9981, 0.9993, 0.9999, 1.0000, 0.9997, 0.9988, 0.9973, 0.9951, 0.9938, 0.9908, 0.9872, 0.9830, 0.9782, 0.9755, 0.9698, 0.9636, 0.9568, 0.9494, 0.9455, 0.9373, 0.9373, 0.9455, 0.9532, 0.9568, 0.9636, 0.9698, 0.9755, 0.9806, 0.9830, 0.9872, 0.9908, 0.9938, 0.9963, 0.9973, 0.9988, 0.9997, 1.0000, 0.9997, 0.9993, 0.9981, 0.9963, 0.9938, 0.9908, 0.9891, 0.9851, 0.9806, 0.9755, 0.9698, 0.9668, 0.9603, 0.9532, 0.9455, 0.9373, 0.9330, 0.9066, 0.8796, 0.8522, 0.8244, 0.8104, 0.7820, 0.7532, 0.7241, 0.6948, 0.6801, 0.6504, 0.6205, 0.5905, 0.5604, 0.5453, 0.5151, 0.4849, 0.4547, 0.4245, 0.4095, 0.3795, 0.3496, 0.3199, 0.2905, 0.2759, 0.2468, 0.2180, 0.1896, 0.1616, 0.1478, 0.1204, 0.0934, 0.0670, 0.0585, 0.0545, 0.0468, 0.0397, 0.0332, 0.0272, 0.0245, 0.0194, 0.0149, 0.0109, 0.0076, 0.0062, 0.0037, 0.0019, 0.0007, 0.0001};

	
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	m = VREF * GAIN * SQRT3 * ONE_DIV_VDC;
	//m = VREF * 0.3030303030;
    m = m > UMAX ? UMAX : m;
    m = m < UMIN ? UMIN : m;
	
    S1 = ((va[i] - 0.5) * m + 0.5) * TOP0 + 0.5;           // PWM Pin A0
    S2 = ((vb[i] - 0.5) * m + 0.5) * TOP0 + 0.5;           // PWM Pin A1
    S3 = ((vc[i] - 0.5) * m + 0.5) * TOP0 + 0.5;           // PWM Pin A2
	
		TIM2->CCR1 = S1;
        TIM2->CCR2 = S2;
        TIM2->CCR3 = S3;
	
	sumAnalog = sumAnalog + analogA5;
	i++;
    if (i >= 200){
        i = 0;
		VAVG = sumAnalog * ONE_DIV_200;
		sumAnalog = 0.0;
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, STT);
		STT = ~STT;
    }
}


int main(void)
{
  HAL_Init();
  SystemClock_Config();

  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
    HAL_TIM_Base_Start_IT(&htim1);
    // HAL_TIM_Base_Start_IT(&htim2);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);


  while (1)
  {
        HAL_ADC_Start(&hadc1);
        analogA5 = HAL_ADC_GetValue(&hadc1);
        HAL_ADC_Stop(&hadc1);
		VREF = (VAVG + 93.286) * 0.00081606;
  }
}


void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{
  ADC_ChannelConfTypeDef sConfig = {0};

  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_6;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};


  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 7199;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_CENTERALIGNED1;
  htim2.Init.Period = 3599;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 500;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.Pulse = 1000;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.Pulse = 1500;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

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
