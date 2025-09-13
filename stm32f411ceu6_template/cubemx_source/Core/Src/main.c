#include "stm32f4xx_it.h"
#include "main.h"
/* USER CODE BEGIN 0 */
#define LED_PIN  GPIO_PIN_13
#define LED_PORT GPIOC
/* USER CODE END 0 */

int main(void)
{
  /* MCU 初始化 */
  HAL_Init();
  SystemClock_Config();   // 25 MHz HSE → 96 MHz
  MX_GPIO_Init();         // PC13 输出

  /* 先点亮 LED（PC13 低电平） */
  HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);

  /* 主循环 */
  while (1)
  {
    HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
    HAL_Delay(500);
  }
}

/* 时钟配置：HSE 25 MHz → PLL 96 MHz */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /* 1. 稳压器 */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* 2. 开 HSE + PLL */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM       = 25;  // 25 MHz / 25 = 1 MHz
  RCC_OscInitStruct.PLL.PLLN       = 384; // 1 MHz * 384 = 384 MHz
  RCC_OscInitStruct.PLL.PLLP       = RCC_PLLP_DIV4; // 384 / 4 = 96 MHz
  RCC_OscInitStruct.PLL.PLLQ       = 8;   // USB 可选 48 MHz
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  /* 3. 总线分频 */
  RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                   | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3);
}

/* GPIO 初始化：PC13 推挽输出 */
void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOC_CLK_ENABLE();

  GPIO_InitStruct.Pin   = LED_PIN;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);
}

/* 错误钩子 */
void Error_Handler(void)
{
  __disable_irq();
  while (1) {}
}