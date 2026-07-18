#include "stm32f103xb.h"
#include "stm32f1xx_hal.h"
#include <stdint.h>

// 1. Khởi tạo ngoại vi thêm cho PA0 (Đặt trước while(1))
void GPIO_Task4_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Bật APB2 Clock cho Port A
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // Cấu hình chân PA0 làm Input Pull-up
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

// 2. Logic điều khiển (Đặt trong while(1))
if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET) {
    // Chống dội phím (Debounce) bằng Delay theo lý thuyết
    HAL_Delay(20); 
    
    // Kiểm tra lại trạng thái để xác nhận chắc chắn phím đã được nhấn
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET) {
        
        // Chống giữ phím (Wait for release): Chờ người dùng thả nút ra
        while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET);
        
        // Chỉ tính 1 lần nhấn duy nhất, tiến hành đảo trạng thái LED PC13
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    }
}