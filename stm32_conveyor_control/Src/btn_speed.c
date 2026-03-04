#include "btn_speed.h"
#include "btn_on_off.h"
#include <stdio.h>

// 전원을 켰을 때 처음 도달할 기본 목표 속도
static uint16_t current_speed = 100;

int Check_Speed_Buttons(void) {
    if (Get_Power_Status() == 1) { // 전원 ON 상태에서만 버튼 작동
        // 증가 버튼 (PG2)
        if (HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_2) == GPIO_PIN_RESET) {
            HAL_Delay(20);
            if (HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_2) == GPIO_PIN_RESET) {
                if (current_speed <= 100) { // 최대 약 100제한
                    current_speed += 5;
                    printf("[SPEED] Up: %d\r\n", current_speed);
                }
                while (HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_2) == GPIO_PIN_RESET);
                return 1;
            }
        }
        // 감소 버튼 (PG3)
        if (HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_3) == GPIO_PIN_RESET) {
            HAL_Delay(20);
            if (HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_3) == GPIO_PIN_RESET) {
                if (current_speed >= 70) { // 최소 75~80 유지 (모터 멈춤 방지)
                    current_speed -= 5;
                    printf("[SPEED] Down: %d\r\n", current_speed);
                }
                while (HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_3) == GPIO_PIN_RESET);
                return 2;
            }
        }
    }
    return 0;
}

uint16_t Get_Current_Speed(void) {
    return current_speed;
}
