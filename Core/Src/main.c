/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
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
#include "fdcan.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define RPM 666.0f
#define RPM_MIN 1000.0f
#define RPM_MAX 7000.0f
#define MAF 66.66f
#define ECT 66.0f
#define ECT_MIN 20.0f
#define ECT_MAX 120.0f
#define FUEL_LEVEL 66.0f
#define FUEL_LEVEL_MIN 0.0f
#define FUEL_LEVEL_MAX 100.0f
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
FDCAN_RxHeaderTypeDef rxHeader;
uint8_t rxData[8];
uint8_t canDatacheck = 0;

FDCAN_TxHeaderTypeDef txHeader;
uint8_t txData[8];

// https://www.csselectronics.com/pages/obd2-pid-table-on-board-diagnostics-j1979
uint8_t pids_supported_01_20[8] =
{ 0x06, 0x41, 0x00, 0x08, 0x11, 0x00, 0x01, 0xAA }; // RPM, ECT and MAF PIDs
uint8_t pids_supported_21_40[8] =
{ 0x06, 0x41, 0x20, 0x00, 0x02, 0x00, 0x00, 0xAA }; // Fuel level PID

float rpm = RPM;
float ect = ECT;
float maf = MAF;
float fuel_level = FUEL_LEVEL;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void Dec2HexTwoBytes(uint16_t dec_value, uint8_t *msb_value, uint8_t *lsb_value);
void SendCanFrame(void);
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
	MX_USART2_UART_Init();
	MX_FDCAN1_Init();
	/* USER CODE BEGIN 2 */
	if (HAL_FDCAN_Start(&hfdcan1) != HAL_OK)
	{
		Error_Handler();
	}

	if (HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE,
			0) != HAL_OK)
	{
		Error_Handler();
	}

	txHeader.IdType = FDCAN_STANDARD_ID;
	txHeader.TxFrameType = FDCAN_DATA_FRAME;
	txHeader.DataLength = FDCAN_DLC_BYTES_8;
	txHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	txHeader.BitRateSwitch = FDCAN_BRS_OFF;
	txHeader.FDFormat = FDCAN_CLASSIC_CAN;
	txHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{

		if (canDatacheck == 1)
		{
			canDatacheck = 0;

			if (rpm < RPM_MAX)
			{
				rpm += 10.0;
			}

			if (ect < ECT_MAX)
			{
				ect += 1.0;
			}

			if (fuel_level - 0.5 > FUEL_LEVEL_MIN)
			{
				fuel_level -= 0.5;
			}

			if ((rxData[0] == 0x02) && (rxData[1] == 0x01)
					&& (rxData[2] == 0x00)) //--------------- Welcome PIDs 0x01-0x20
			{
				txHeader.Identifier = 0x7E8;
				memcpy(txData, pids_supported_01_20, 8);

				SendCanFrame();
				HAL_GPIO_TogglePin(USER_BLUE_LED_GPIO_Port, USER_BLUE_LED_Pin);
			}

			else if ((rxData[0] == 0x02) && (rxData[1] == 0x01)
					&& (rxData[2] == 0x20)) //---------------- Welcome PIDs 0x21-0x40
			{
				txHeader.Identifier = 0x7E8;
				memcpy(txData, pids_supported_21_40, 8);

				SendCanFrame();
				HAL_GPIO_TogglePin(USER_BLUE_LED_GPIO_Port, USER_BLUE_LED_Pin);
			}

			else if ((rxData[0] == 0x02) && (rxData[1] == 0x01)
					&& (rxData[2] == 0x0C)) //----------------------------------- RPM
			{
				txHeader.Identifier = 0x7E8;
				txData[0] = 0x04;
				txData[1] = 0x41;
				txData[2] = 0x0C;
				Dec2HexTwoBytes((uint16_t) (rpm * 4), &txData[3], &txData[4]);
				txData[5] = 0xAA;
				txData[6] = 0xAA;
				txData[7] = 0xAA;

				SendCanFrame();
				HAL_GPIO_TogglePin(USER_GREEN_LED_GPIO_Port,
				USER_GREEN_LED_Pin);
			}

			else if ((rxData[0] == 0x02) && (rxData[1] == 0x01)
					&& (rxData[2] == 0x05)) //--------------------------------- ECT
			{

				txHeader.Identifier = 0x7E8;
				txData[0] = 0x03;
				txData[1] = 0x41;
				txData[2] = 0x05;
				txData[3] = (uint8_t) (ect + 40);
				txData[4] = 0xAA;
				txData[5] = 0xAA;
				txData[6] = 0xAA;
				txData[7] = 0xAA;

				SendCanFrame();
				HAL_GPIO_TogglePin(USER_GREEN_LED_GPIO_Port,
				USER_GREEN_LED_Pin);
			}

			else if ((rxData[0] == 0x02) && (rxData[1] == 0x01)
					&& (rxData[2] == 0x10)) //---------------------------------- MAF
			{

				txHeader.Identifier = 0x7E8;
				txData[0] = 0x04;
				txData[1] = 0x41;
				txData[2] = 0x10;
				Dec2HexTwoBytes((uint16_t) (maf * 100), &txData[3], &txData[4]);
				txData[5] = 0xAA;
				txData[6] = 0xAA;
				txData[7] = 0xAA;

				SendCanFrame();
				HAL_GPIO_TogglePin(USER_YELLOW_LED_GPIO_Port,
				USER_YELLOW_LED_Pin);
			}

			else if ((rxData[0] == 0x02) && (rxData[1] == 0x01)
					&& (rxData[2] == 0x2F)) //----------------- Fuel tank level input
			{
				txHeader.Identifier = 0x7E8;
				txData[0] = 0x03;
				txData[1] = 0x41;
				txData[2] = 0x2F;
				txData[3] = (uint8_t) (fuel_level * 2.55f);
				txData[4] = 0xAA;
				txData[5] = 0xAA;
				txData[6] = 0xAA;
				txData[7] = 0xAA;

				SendCanFrame();
				HAL_GPIO_TogglePin(USER_ORANGE_LED_GPIO_Port,
				USER_ORANGE_LED_Pin);
			}
			else
			{
				__NOP();
			}
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
	RCC_OscInitTypeDef RCC_OscInitStruct =
	{ 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct =
	{ 0 };

	/** Configure the main internal regulator output voltage
	 */
	HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
	RCC_OscInitStruct.PLL.PLLN = 10;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
	{
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
	if ((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET)
	{
		if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &rxHeader, rxData)
				!= HAL_OK)
		{
			Error_Handler();
		}
		else
		{
			canDatacheck = 1;
		}
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == WHITE_BUTTON_Pin)
	{
		ect = ECT_MIN;
	}
	if (GPIO_Pin == GREEN_BUTTON_Pin)
	{
		rpm = RPM_MIN;
		fuel_level = FUEL_LEVEL_MAX;
	}
}

void Dec2HexTwoBytes(uint16_t dec_value, uint8_t *msb_value, uint8_t *lsb_value)
{
	*lsb_value = dec_value & 0x00FF;
	*msb_value = dec_value >> 8;
}

void SendCanFrame(void)
{
	if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &txHeader, txData) != HAL_OK)
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
