/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "LoRa.h"
#include "lcd20x4.h"
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
SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */

//Lora
LoRa myLoRa;
uint32_t packet_size =0;
uint8_t received_data=0;
uint16_t teste;
uint8_t flag=0, verificacao=0;
uint16_t flagDILSON=0;
uint8_t meuTexto[21];

//lcd
uint16_t app;
uint16_t app_t, a;

//infravermelho:

typedef enum{
		DESLIGADO,
		BOTAO_1,
		BOTAO_2,
		BOTAO_3,
		BOTAO_4,
		BOTAO_5,
		BOTAO_6,
		BOTAO_7,
		BOTAO_8,
		BOTAO_9,
		OK,
		SETA_BAIXO,
		SETA_ESQUERDA,
		SETA_CIMA,
		SETA_DIREITA,
		ASTERISCO,
		HASHTAG
} botoes_e;

botoes_e recebeu;

uint32_t data, contagem;
uint8_t recebeu, cont=0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin){
	flagDILSON=1;

}
void __init_application(){
	  memset(&app, 0, sizeof(app_t));

	  HAL_TIM_Base_Start(&htim1);

	  lcd_attr_gpio(RS, RS_GPIO_Port, RS_Pin);
	  lcd_attr_gpio(RW, RW_GPIO_Port, RW_Pin);
	  lcd_attr_gpio(E, E_GPIO_Port, E_Pin);

	  lcd_attr_gpio(D4, D4_GPIO_Port, D4_Pin);
	  lcd_attr_gpio(D5, D5_GPIO_Port, D5_Pin);
	  lcd_attr_gpio(D6, D6_GPIO_Port, D6_Pin);
	  lcd_attr_gpio(D7, D7_GPIO_Port, D7_Pin);

	  lcd_init();
	  lcd_clear();

	  lcd_put_cur(3, 12);
	  lcd_send_string("PADOLABS");
	  HAL_Delay(1000);
}

void delay_us  ( uint16_t us)
{
	__HAL_TIM_SET_COUNTER(&htim1, 0 );   // define o valor do contador como 0
	while  (__HAL_TIM_GET_COUNTER(&htim1)  <  us);   // espera o contador alcançar a entrada us no parâmetro
}

uint32_t recebendo_data()
{
	uint32_t teste;
	//HAL_TIM_Base_Stop_IT(&htim2);
	while((HAL_GPIO_ReadPin(infra_GPIO_Port, infra_Pin))==0);
	while(HAL_GPIO_ReadPin(infra_GPIO_Port, infra_Pin)==1);

	 for(int i = 0; i < 32; i++)
		 {
			 while((HAL_GPIO_ReadPin(infra_GPIO_Port, infra_Pin))==0);
			 contagem=0;
			 while(HAL_GPIO_ReadPin(infra_GPIO_Port, infra_Pin)==1)
			 {
			 contagem++;
			 if(contagem>=200)
				 {
					 //HAL_TIM_Base_Start_IT(&htim2);
					 //flag=0;
					 return(0);
				 }
			 delay_us(100);
			 }

			 if(contagem > 12)
			 {
				 teste |= (1UL << (31-i));
			 }

			else
			{
				teste &= ~ (1UL << (31-i));
			}
		 }
	 return teste;
}


void convert_code (uint32_t code)
{
	switch (code)
	{
		case (0x807fa55a):
			recebeu = DESLIGADO;

			break;

		case (0x807fb44b):
			recebeu = BOTAO_1;
			sprintf((char*)meuTexto,"Sentido: horario    ");
			lcd_put_cur(2,0);
			lcd_send_string(meuTexto);
			break;

		case (0x807fcc33):
			recebeu = BOTAO_2;
			sprintf((char*)meuTexto,"Sentido: antihorario   ");
			lcd_put_cur(2,0);
			lcd_send_string(meuTexto);
			break;

		case (0x807fd827):
			recebeu = BOTAO_3;
			sprintf((char*)meuTexto,"Sentido: antihorario   ");
			lcd_put_cur(2,0);
			lcd_send_string(meuTexto);
			break;

		case (0x807f9867):
			recebeu = BOTAO_4;
			sprintf((char*)meuTexto,"Sentido: horario    ");
			lcd_put_cur(2,0);
			lcd_send_string(meuTexto);
			break;

		case (0x807f8c73):
			recebeu = BOTAO_5;
			sprintf((char*)meuTexto,"Sentido: horario    ");
			lcd_put_cur(2,0);
			lcd_send_string(meuTexto);
			break;

		case (0x807fbd42):
			recebeu = BOTAO_6;
			sprintf((char*)meuTexto,"Sentido: antihorario   ");
			lcd_put_cur(2,0);
			lcd_send_string(meuTexto);
			break;

		case (0x807f8877):
			recebeu = BOTAO_7;
			sprintf((char*)meuTexto,"Sentido: antihorario   ");
			lcd_put_cur(2,0);
			lcd_send_string(meuTexto);
			break;

		case (0x807f9c63):
			recebeu = BOTAO_8;
			sprintf((char*)meuTexto,"Sentido: antihorario   ");
			lcd_put_cur(2,0);
			lcd_send_string(meuTexto);
			break;

		case (0x807fad52):
			recebeu = BOTAO_9;
			sprintf((char*)meuTexto,"Sentido: horario    ");
			lcd_put_cur(2,0);
			lcd_send_string(meuTexto);
			break;

		case (0x807f817e):
			recebeu = OK;
			cont++;

			if(cont>1)
			{
				cont=0;
			}
			break;

		case (0x807fd42b):
			recebeu = SETA_BAIXO;
			break;

		case (0x807f916e):
			recebeu = SETA_ESQUERDA;
			sprintf((char*)meuTexto,"Sentido: horario    ");
			lcd_put_cur(2,0);
			lcd_send_string(meuTexto);
			break;

		case (0x807fb14e):
			recebeu = SETA_CIMA;
			sprintf((char*)meuTexto,"Sentido: horario    ");
			lcd_put_cur(2,0);
			lcd_send_string(meuTexto);
			break;

		case (0x807fe11e):
			recebeu = SETA_DIREITA;
			sprintf((char*)meuTexto,"Sentido: antihorario   ");
			lcd_put_cur(2,0);
			lcd_send_string(meuTexto);
			break;

		case (0x807fa15e):
			recebeu = ASTERISCO;
			sprintf((char*)meuTexto,"Sentido: parado   ");
			lcd_put_cur(2,0);
			lcd_send_string(meuTexto);
			break;

		case (0x807fa956):
			recebeu = HASHTAG;
			break;

		default :
			break;
		}

	verificacao=recebeu;
	LoRa_transmit(&myLoRa, &verificacao, 1 , 100);
	verificacao=0;

}

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
  MX_TIM1_Init();
  MX_SPI1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  __init_application();
  HAL_TIM_Base_Start_IT(&htim2);
  HAL_TIM_Base_Start(&htim1);

	  	myLoRa = newLoRa();

		myLoRa.hSPIx                 = &hspi1;
		myLoRa.CS_port               = NSS_GPIO_Port;
		myLoRa.CS_pin                = NSS_Pin;
		myLoRa.reset_port            = RESET_GPIO_Port;
		myLoRa.reset_pin             = RESET_Pin;
		myLoRa.DIO0_port						 = DIO0_GPIO_Port;
		myLoRa.DIO0_pin							 = DIO0_Pin;

		myLoRa.frequency             = 433;							  // default = 433 MHz
		myLoRa.spredingFactor        = SF_7;							// default = SF_7
		myLoRa.bandWidth			       = BW_125KHz;				  // default = BW_125KHz
		myLoRa.crcRate				       = CR_4_5;						// default = CR_4_5
		myLoRa.power					       = POWER_20db;				// default = 20db
		myLoRa.overCurrentProtection = 120; 							// default = 100 mA
		myLoRa.preamble				       = 10;		  					// default = 8;

		teste=LoRa_init(&myLoRa);

		LoRa_startReceiving(&myLoRa);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
 	while (1)
 	{
		  if(flagDILSON==1)
		  {
			  flagDILSON=0;
			  HAL_NVIC_DisableIRQ(EXTI4_15_IRQn);
			  data = recebendo_data ();
			  convert_code (data);
			  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
		  }


		  //HAL_NVIC_DisableIRQ(EXTI4_15_IRQn);

		  /*HAL_Delay(150);
		  packet_size = LoRa_receive(&myLoRa, &received_data, 1);*/

		  //HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);

		  /*if(packet_size!=0)
			{
				  sprintf((char*)meuTexto,"Corrente: %d mA   ", received_data);
				  lcd_put_cur(1,0);
				  lcd_send_string(meuTexto);
			}*/

			if(cont == 0)
			{
				sprintf((char*)meuTexto,"Desligado");
				lcd_put_cur(0,0);
				lcd_send_string(meuTexto);
				cont=0;
			}

			else
			{
				sprintf((char*)meuTexto,"Ligado   ");
				lcd_put_cur(0,0);
				lcd_send_string(meuTexto);
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
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 8;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
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
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 64-1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 0xffff-1;
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
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 639;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 999;
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
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

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
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, D7_Pin|D6_Pin|D5_Pin|RW_Pin
                          |RS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, RESET_Pin|DIO0_Pin|E_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(NSS_GPIO_Port, NSS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : bb_Pin */
  GPIO_InitStruct.Pin = bb_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(bb_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : infra_Pin */
  GPIO_InitStruct.Pin = infra_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(infra_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : D7_Pin D6_Pin D5_Pin RW_Pin
                           RS_Pin */
  GPIO_InitStruct.Pin = D7_Pin|D6_Pin|D5_Pin|RW_Pin
                          |RS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : RESET_Pin DIO0_Pin E_Pin */
  GPIO_InitStruct.Pin = RESET_Pin|DIO0_Pin|E_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : D4_Pin NSS_Pin */
  GPIO_InitStruct.Pin = D4_Pin|NSS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);

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

