/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define CS_H	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET)
#define CS_L	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void PS2_Cmd(uint8_t* TxData, uint8_t* RxData, uint8_t size)
{
	if (RxData == NULL)
		HAL_SPI_Transmit(&hspi1, TxData, size, 100);
	else
		HAL_SPI_TransmitReceive(&hspi1, TxData, RxData, size, 100);

}

void PS2_ShortPoll()
{
	CS_H; CS_L;
	PS2_Cmd(0x01, NULL, 1);
	PS2_Cmd(0x42, NULL, 1 );
	PS2_Cmd(0x00, NULL, 1);
	PS2_Cmd(0x00, NULL, 1);
	PS2_Cmd(0x00, NULL, 1);
	CS_H;
}

void PS2_EnterConfig()
{
	CS_H; CS_L;
	PS2_Cmd(0x01, NULL, 1);
	PS2_Cmd(0x43, NULL, 1);
	PS2_Cmd(0x00, NULL, 1);
	PS2_Cmd(0x01, NULL, 1);
	PS2_Cmd(0x00, NULL, 1);
	PS2_Cmd(0x00, NULL, 1);
	PS2_Cmd(0x00, NULL, 1);
	PS2_Cmd(0x00, NULL, 1);
	PS2_Cmd(0x00, NULL, 1);
	CS_H;
}

void PS2_Setup()
{
	CS_H; CS_L;
	PS2_Cmd(0x01, NULL,1);
	PS2_Cmd(0x44, NULL,1);
	PS2_Cmd(0x00, NULL,1);
	PS2_Cmd(0x01, NULL,1);
	PS2_Cmd(0xEE, NULL,1);
	PS2_Cmd(0x00, NULL,1);
	PS2_Cmd(0x00, NULL,1);
	PS2_Cmd(0x00, NULL,1);
	PS2_Cmd(0x00, NULL,1);
	CS_H;
}

void PS2_VibrationMode()
{
	CS_H; CS_L;
	PS2_Cmd(0x01, NULL,1);
	PS2_Cmd(0x4D, NULL,1);
	PS2_Cmd(0x00, NULL,1);
	PS2_Cmd(0x00, NULL,1);
	PS2_Cmd(0x01, NULL,1);
	CS_H;
}

void PS2_ExitConfig()
{
	CS_H; CS_L;
	PS2_Cmd(0x01, NULL,1);
	PS2_Cmd(0x43, NULL,1);
	PS2_Cmd(0x00, NULL,1);
	PS2_Cmd(0x00, NULL,1);
	PS2_Cmd(0x5A, NULL,1);
	PS2_Cmd(0x5A, NULL,1);
	PS2_Cmd(0x5A, NULL,1);
	PS2_Cmd(0x5A, NULL,1);
	PS2_Cmd(0x5A, NULL,1);
	CS_H;
}

void PS2_Init ()
{
	uint8_t ShortPoll[5] = {0x01, 0x42, 0x00, 0x00, 0x00};
	uint8_t EnterConfig[9] = {0x01, 0x43, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00};
	uint8_t Setup[9] = {0x01, 0x44, 0x00, 0x01, 0xEE, 0x00, 0x00, 0x00, 0x00};
	uint8_t VibrationMode[5] = {0x01, 0x04D, 0x00, 0x00, 0x01};
	uint8_t ExitConfig[9] = {0x01, 0x43, 0x00, 0x00, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A};

	PS2_Cmd(ShortPoll, NULL, 5);
	PS2_Cmd(ShortPoll, NULL, 5);
	PS2_Cmd(ShortPoll, NULL, 5);
	PS2_Cmd(EnterConfig, NULL, 9);
	PS2_Cmd(Setup, NULL, 9);
	PS2_Cmd(VibrationMode, NULL, 5);
	PS2_Cmd(ExitConfig, NULL, 9);
}

void PS2_Start(uint8_t* ID)
{
	CS_H; CS_L;
	PS2_Cmd(0x01, NULL, 1);
	PS2_Cmd(0x42, ID, 1);
	CS_H;
}

void PS2_ReadData(uint8_t *data, uint8_t* id)
{
	CS_H; CS_L;
	PS2_Start(id);
	HAL_SPI_Receive(&hspi1, data, 7, 100);
	CS_H;
}

void PS2_Vibration(uint8_t motor1, uint8_t motor2)
{
	CS_H, CS_L;
	uint8_t buff[7] = {0x01, 0x42, 0x00, motor1, motor2, 0x00, 0x00, 0x00, 0x00};
	PS2_Cmd(buff, NULL, 7);
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
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  PS2_Init();
  uint8_t Data[7] ={0};
  uint8_t str[64];
  uint8_t ID = 0;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	  PS2_ReadData(Data, &ID);
	  sprintf(str,"Режим: %x\n", ID);
	  HAL_UART_Transmit(&huart1, str, 64, 100);
	  sprintf(str,"Данные: %x %x %x %x %x %x %x\n", Data[0], Data[1], Data[2], Data[3], Data[4], Data[5], Data[6]);
	  HAL_UART_Transmit(&huart1, str, 64, 100);

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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
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
