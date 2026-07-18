#include "stm32f103xb.h"
#include "stm32f1xx_hal.h"
#include <stdint.h>

// 1. Khởi tạo ngoại vi (Đặt trước while(1))
void GPIO_Task3_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Bật APB2 Clock cho Port C (Bắt buộc theo quy trình cấu hình)
    __HAL_RCC_GPIOC_CLK_ENABLE();

    // Cấu hình chân PC13 làm Output Push-Pull
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

// 2. Vòng lặp điều khiển (Đặt trong while(1))
// Yêu cầu: 500ms Sáng, 500ms Tắt
HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
HAL_Delay(500);