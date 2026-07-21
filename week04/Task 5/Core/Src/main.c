/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
/* Khai báo mảng chân kết nối cho Ma trận phím 2x2 trên Port A */
uint16_t ROW_PINS[2] = {GPIO_PIN_0, GPIO_PIN_1};
uint16_t COL_PINS[2] = {GPIO_PIN_2, GPIO_PIN_3};

/* Khai báo mảng chân kết nối cho 4 Đèn LED trên Port B */
uint16_t LED_PINS[4] = {GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_3, GPIO_PIN_4};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
/* Khai báo hàm quét ma trận phím */
uint8_t KEYPAD_2x2_Scan(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/**
  * @brief  Hàm quét ma trận phím 2x2
  * @retval Mã phím được nhấn (1-4), trả về 0 nếu không có phím nhấn
  */
uint8_t KEYPAD_2x2_Scan(void) {
    for (uint8_t r = 0; r < 2; r++) {
        // Kéo Hàng r xuống mức 0, hàng còn lại mức 1
        for (uint8_t i = 0; i < 2; i++) {
            HAL_GPIO_WritePin(GPIOA, ROW_PINS[i], (i == r) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        }
        HAL_Delay(1); // Chờ điện áp ổn định
        
        // Đọc trạng thái các Cột
        for (uint8_t c = 0; c < 2; c++) {
            if (HAL_GPIO_ReadPin(GPIOA, COL_PINS[c]) == GPIO_PIN_RESET) {
                HAL_Delay(20); // Khử nhiễu chống dội
                if (HAL_GPIO_ReadPin(GPIOA, COL_PINS[c]) == GPIO_PIN_RESET) {
                    return (r * 2 + c) + 1; // Trả về mã phím từ 1 đến 4
                }
            }
        }
    }
    return 0; // Không có phím nào được nhấn
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
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    // Đọc mã phím từ ma trận
    uint8_t key = KEYPAD_2x2_Scan();
    
    // Tắt toàn bộ 4 LED trước khi xử lý (Mức logic 0: Tắt LED)
    HAL_GPIO_WritePin(GPIOB, LED_PINS[0] | LED_PINS[1] | LED_PINS[2] | LED_PINS[3], GPIO_PIN_RESET);
    
    // Bật đèn LED tương ứng dựa trên mã phím thông qua Firmware
    if (key == 1) {
        HAL_GPIO_WritePin(GPIOB, LED_PINS[0], GPIO_PIN_SET); // Bật LED 1 (PB0)
    } else if (key == 2) {
        HAL_GPIO_WritePin(GPIOB, LED_PINS[1], GPIO_PIN_SET); // Bật LED 2 (PB1)
    } else if (key == 3) {
        HAL_GPIO_WritePin(GPIOB, LED_PINS[2], GPIO_PIN_SET); // Bật LED 3 (PB3)
    } else if (key == 4) {
        HAL_GPIO_WritePin(GPIOB, LED_PINS[3], GPIO_PIN_SET); // Bật LED 4 (PB4)
    }
  /* USER CODE END 3 */
  }
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
  /* Cấp nguồn Xung nhịp cho các Port A, B, C và khối AFIO */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_AFIO_CLK_ENABLE(); // Bật Clock cho AFIO để cho phép Remap chân

  /* Giải phóng chân PB3 và PB4 khỏi chức năng JTAG mặc định (Chỉ giữ lại SWD) */
  __HAL_AFIO_REMAP_SWJ_NOJTAG();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  
  // 1. Cấu hình 2 Hàng của Keypad (PA0, PA1) làm Output Push-Pull
  GPIO_InitStruct.Pin = ROW_PINS[0] | ROW_PINS[1];
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // 2. Cấu hình 2 Cột của Keypad (PA2, PA3) làm Input Pull-up
  GPIO_InitStruct.Pin = COL_PINS[0] | COL_PINS[1];
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // 3. Cấu hình 4 chân LED (PB0, PB1, PB3, PB4) làm Output Push-Pull
  HAL_GPIO_WritePin(GPIOB, LED_PINS[0] | LED_PINS[1] | LED_PINS[2] | LED_PINS[3], GPIO_PIN_RESET);
  GPIO_InitStruct.Pin = LED_PINS[0] | LED_PINS[1] | LED_PINS[2] | LED_PINS[3];
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

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
#ifdef USE_FULL_ASSERT
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
