/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
/* Includes ------------------------------------------------------------------*/
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
#define sframe 10
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
DAC_HandleTypeDef hdac;
DMA_HandleTypeDef hdma_dac1;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim6;

/* USER CODE BEGIN PV */
DAC_HandleTypeDef hdac, hdac_nor;
DMA_HandleTypeDef hdma_dac1;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim6;
LIS3DSH_InitTypeDef myLIS3DSH_conf;
LIS3DSH_DataScaled myRaw_Data;


//const uint8_t DAC_output[8] = {0, 64, 96, 128, 160, 192, 224, 255};
//const uint16_t DAC_output[9] = {511,1022,1533,2044,2555,3066,3577,4095,4096};
const uint32_t DAC_output = 8191; //4096 max 24v 
//const uint32_t DAC_output[9] = {4095,3577,3066,2555,2044,1533,1022,511,0};
uint8_t DAC_value = 0;
Motor_State myMotor_State = Motor_OFF;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_SPI1_Init(void);
static void MX_DAC_Init(void);
static void MX_TIM6_Init(void);
/* USER CODE BEGIN PFP */
void gpio_conf_dac_nor(void);
void DAC_nor_Init(void);
void motor_start_DMA(void);
void motor_start_withoutDMA(void);
void LIS3DSH_value();

void enable_controller(void);
void clockwise_rotation(void);
void counter_clockwise_rotation(void);
void brake_motor(void);
void rm_brake_motor(void);
void detect_slope();
float simple_moving_avg(float *ptrframe, int len, int pos, float nextnum, float *ptrsum);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_DAC_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
	enable_controller();
	//gpio_conf_dac_nor(); //without dma
	//DAC_nor_Init();      //without dma
	HAL_TIM_Base_Start(&htim6);
	
	myLIS3DSH_conf.antiAliasingBW = LIS3DSH_FILTER_BW_50;
	myLIS3DSH_conf.dataRate = LIS3DSH_DATARATE_25;
	myLIS3DSH_conf.enableAxes = LIS3DSH_XYZ_ENABLE;
	myLIS3DSH_conf.fullScale = LIS3DSH_FULLSCALE_4;
	myLIS3DSH_conf.interruptEnable = false;
	LIS3DSH_Init(&hspi1,&myLIS3DSH_conf);
	LIS3DSH_X_calibrate(-975.0,958.0);
	LIS3DSH_Y_calibrate(-1000.0,977.0);
	LIS3DSH_Z_calibrate(-975.0,1007.0);
	//HAL_DAC_Start(&hdac_nor,DAC1_CHANNEL_2); //without dma
	clockwise_rotation();
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	
  while (1)
  {
		LIS3DSH_value();
		detect_slope();
		switch(myMotor_State)
		{
			case Motor_ON:			
					 rm_brake_motor();
					 motor_start_DMA();
					 //motor_start_withoutDMA(); //without dma
					 break;
			case Motor_OFF:
					 brake_motor();
					 break;
			default:
					 myMotor_State = Motor_OFF;
					 break;
		}
    /* USER CODE END WHILE */

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
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 160;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief DAC Initialization Function
  * @param None
  * @retval None
  */
static void MX_DAC_Init(void)
{

  /* USER CODE BEGIN DAC_Init 0 */

  /* USER CODE END DAC_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC_Init 1 */
  /* USER CODE BEGIN 2 */
  /* USER CODE END DAC_Init 1 */
  /** DAC Initialization
  */
  hdac.Instance = DAC;
  if (HAL_DAC_Init(&hdac) != HAL_OK)
  {
    Error_Handler();
  }
  /** DAC channel OUT1 config
  */
  sConfig.DAC_Trigger = DAC_TRIGGER_T6_TRGO;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC_Init 2 */

  /* USER CODE END DAC_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 80-1;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 1000-1;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

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
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(MEMS_CS_GPIO_Port, MEMS_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15
                          |bk_pin_Pin|forward_rev_pin_Pin|enable_pin_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : MEMS_CS_Pin */
  GPIO_InitStruct.Pin = MEMS_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(MEMS_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PD12 PD13 PD14 PD15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : bk_pin_Pin forward_rev_pin_Pin enable_pin_Pin */
  GPIO_InitStruct.Pin = bk_pin_Pin|forward_rev_pin_Pin|enable_pin_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PD4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

}

/* USER CODE BEGIN 4 */
void detect_slope()
{
	static float sum_vlaue=0; 
	float y_avg=0;
  static float frame[sframe]={0};	
	static int pos=0;
	
			y_avg=simple_moving_avg(frame,sframe,pos,myRaw_Data.y,&sum_vlaue);
			if(pos>=sframe)
			{
				pos=0;
			}
			else
			{
				pos++;
			}
			if(y_avg>600 && y_avg<977) 
			{
				myMotor_State = Motor_ON;
			}
			else if(y_avg<-600 && y_avg>-1000)
			{
				myMotor_State = Motor_OFF;
			}
			else
			{
			}
		
}
float simple_moving_avg(float *ptrframe, int len, int pos, float nextnum, float *ptrsum)
{
	*ptrsum=*ptrsum-ptrframe[pos]+nextnum;
	ptrframe[pos]=nextnum;
	return *ptrsum/len;
}
void brake_motor(void)
{
	HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_12);
	HAL_GPIO_WritePin(GPIOD, bk_pin_Pin, GPIO_PIN_RESET); //pin pd1
}
void rm_brake_motor(void)
{
	HAL_GPIO_WritePin(GPIOD, bk_pin_Pin, GPIO_PIN_SET); //pin pd1
}
void counter_clockwise_rotation(void)
{
	HAL_GPIO_WritePin(GPIOD, forward_rev_pin_Pin, GPIO_PIN_RESET); //pin pd2
}
void clockwise_rotation(void)
{
	HAL_GPIO_WritePin(GPIOD, forward_rev_pin_Pin, GPIO_PIN_SET);
}
void enable_controller(void)
{
	HAL_GPIO_WritePin(GPIOD, enable_pin_Pin, GPIO_PIN_RESET); //pin pd3
}
void gpio_conf_dac_nor(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	GPIO_InitTypeDef gpio_dac_nor;
	gpio_dac_nor.Mode = GPIO_MODE_ANALOG;
	gpio_dac_nor.Pin = GPIO_PIN_4;
	gpio_dac_nor.Pull = GPIO_NOPULL;
	gpio_dac_nor.Speed = GPIO_SPEED_FREQ_LOW;
	
	HAL_GPIO_Init(GPIOA,&gpio_dac_nor);
	
}
void DAC_nor_Init(void)
{
	__HAL_RCC_DAC_CLK_ENABLE();
	
	//DAC_HandleTypeDef hdac_nor;
	hdac_nor.Instance = DAC1;
	HAL_DAC_Init(&hdac_nor);
	DAC_ChannelConfTypeDef hdac_channel_nor = {0};
	hdac_channel_nor.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
	hdac_channel_nor.DAC_Trigger = DAC_TRIGGER_NONE;
	if(HAL_DAC_ConfigChannel(&hdac_nor,&hdac_channel_nor,DAC_CHANNEL_2) != HAL_OK)
	{
		Error_Handler();
	}
}
void LIS3DSH_value()
{
	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
	if(LIS3DSH_PollDRDY(100) == true)
		{
			myRaw_Data = LIS3DSH_GetDataScaled();
		}
		else
		{
		}
}

/*void motor_start_withoutDMA(void) //withoutDMA CHANNEL 2
{
	uint32_t i=0;
	uint32_t conv_DAC_value = 0;
	uint32_t Vref = 5;
	for(i=0;i<255;i+=31)
	{
	DAC_value = i;
	//conv_DAC_value = (DAC_value * 4096UL)/Vref;
		conv_DAC_value = (DAC_value * 255UL)/Vref;
	HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_14);
	HAL_DAC_SetValue(&hdac_nor, DAC1_CHANNEL_2, DAC_ALIGN_8B_R, conv_DAC_value);
	}
}*/
void motor_start_DMA(void)
{
	
	HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_14);
	if(HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t *) DAC_output, 1, DAC_ALIGN_12B_R) != HAL_OK)
	{
		Error_Handler();
	}
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
