#include "stm32f103xb.h"
#include "stm32f1xx_hal.h"
#include <stdint.h>

// Khai báo hàm nguyên mẫu
void GPIO_Init(void);
uint8_t Scan_Matrix_2x2(void);

int main(void) {
    // Khởi tạo thư viện HAL
    HAL_Init();
    
    // Cấu hình các chân GPIO
    GPIO_Init();

    // Tắt toàn bộ 4 LED lúc mới cấp điện để reset trạng thái
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_3 | GPIO_PIN_4, GPIO_PIN_RESET);

    uint8_t last_key = 0;

    while (1) {
        // Thuật toán quét ma trận tại hàm while(1)
        uint8_t key = Scan_Matrix_2x2();
        
        // CHỐNG DỘI VÀ KHÓA PHÍM: Chỉ xử lý khi có phím bấm MỚI
        if (key != 0 && key != last_key) {
            
            HAL_Delay(30); // Trễ 30ms để chống nhiễu cơ khí
            
            // Đọc lại để xác nhận trạng thái nút đã ổn định
            if (key == Scan_Matrix_2x2()) {
                
                // Phần mềm ra lệnh xuất mức logic tương ứng để bật/tắt LED (Cách 2)[cite: 1]
                if (key == 1) {
                    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0); // Nút 1 -> Đảo trạng thái LED Đỏ
                } 
                else if (key == 2) {
                    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4); // Nút 2 -> Đảo trạng thái LED Vàng
                } 
                else if (key == 3) {
                    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1); // Nút 3 -> Đảo trạng thái LED Xanh lá
                } 
                else if (key == 4) {
                    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3); // Nút 4 -> Đảo trạng thái LED Xanh dương
                }
                
                last_key = key; // Lưu trạng thái để khóa phím
            }
        }
        // Xử lý khi người dùng nhả phím hoàn toàn
        else if (key == 0) {
            HAL_Delay(20);
            if (Scan_Matrix_2x2() == 0) {
                last_key = 0; // Mở khóa chờ lần nhấn tiếp theo
            }
        }
    }
}

/**
 * @brief Cấu hình ngoại vi GPIO theo yêu cầu Task 5
 */
void GPIO_Init(void) {
    // 1. CHUYỂN KHAI BÁO BIẾN LÊN TRÊN CÙNG CỦA HÀM
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    // Bật Clock cho Port A (Ma trận), Port B (LED) và AFIO (để Remap)
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_AFIO_CLK_ENABLE(); 

    // Tắt JTAG để sử dụng PB3, PB4 làm I/O bình thường
    __HAL_AFIO_REMAP_SWJ_NOJTAG();

    // 1. Cấu hình 2 chân Hàng (PA1, PA2) làm Digital Output[cite: 1]
    GPIO_InitStruct.Pin = GPIO_PIN_1 | GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 2. Cấu hình 2 chân Cột (PA3, PA4) làm Digital Input Pull-up[cite: 1]
    GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    // 3. Cấu hình 4 chân LED (PB0, PB1, PB3, PB4) làm Digital Output (Phục vụ Cách 2)[cite: 1]
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_3 | GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/**
 * @brief Triển khai thuật toán quét ma trận 2x2
 */
uint8_t Scan_Matrix_2x2(void) {
    uint8_t pressed_key = 0;

    // Đưa cả 2 Hàng về mức logic mặc định (Mức 1 - 3.3V)
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1 | GPIO_PIN_2, GPIO_PIN_SET);
    
    // Lần lượt kéo từng hàng xuống mức 0, sau đó đọc trạng thái của 2 cột[cite: 1]
    // ---------------------------------------------------------
    // QUÉT HÀNG 1 (PA1)
    // ---------------------------------------------------------
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET); // Kéo Hàng 1 xuống 0V
    HAL_Delay(2); // Chờ điện áp trên đường truyền ổn định
    
    // Đọc trạng thái Cột 1 và Cột 2
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3) == GPIO_PIN_RESET) pressed_key = 1; // Hàng 1 chập Cột 1
    else if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) == GPIO_PIN_RESET) pressed_key = 2; // Hàng 1 chập Cột 2
    
    // Kéo Hàng 1 trở lại mức 1
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);   

    if (pressed_key != 0) return pressed_key;

    // ---------------------------------------------------------
    // QUÉT HÀNG 2 (PA2)
    // ---------------------------------------------------------
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET); // Kéo Hàng 2 xuống 0V
    HAL_Delay(2);
    
    // Đọc trạng thái Cột 1 và Cột 2
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3) == GPIO_PIN_RESET) pressed_key = 3; // Hàng 2 chập Cột 1
    else if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) == GPIO_PIN_RESET) pressed_key = 4; // Hàng 2 chập Cột 2
    
    // Kéo Hàng 2 trở lại mức 1
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);   
    
    return pressed_key; 
}

// Hàm xử lý ngắt SysTick cấp nhịp clock cho HAL_Delay()
void SysTick_Handler(void) {
    HAL_IncTick();
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Báo cáo tên file và số dòng nơi xảy ra lỗi assert_param.
  * @param  file: con trỏ tới tên file nguồn
  * @param  line: số dòng báo lỗi
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* Bạn có thể thêm code để báo lỗi tại đây (ví dụ: gửi qua UART hoặc nháy LED) */
  /* Vòng lặp vô hạn để giữ hệ thống dừng lại khi có lỗi nghiêm trọng */
  while (1)
  {
  }
}
#endif