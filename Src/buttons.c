#include "stm32f0xx_hal.h"

#include "buttons.h"

uint8_t g_buttons[16];
uint16_t g_buttons_bits;

void read_buttons() {
    g_buttons[0]  = !HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
    g_buttons[1]  = !HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1);
    g_buttons[2]  = !HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2);
    g_buttons[3]  = !HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3);
    g_buttons[4]  = !HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4);
    g_buttons[5]  = !HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5);
    g_buttons[6]  = !HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6);
    g_buttons[7]  = !HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7);
    g_buttons[8]  = !HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);
    g_buttons[9]  = !HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);
    g_buttons[10] = !HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10);
    g_buttons[11] = !HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11);
    g_buttons[12] = !HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12);
    g_buttons[13] = !HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13);
    g_buttons[14] = !HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14);
    g_buttons[15] = !HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15);

    g_buttons_bits = 0;
    for (int i = 0; i < 16; i++) {
        g_buttons_bits |= g_buttons[i] << i;
    }
}

void read_buttons_bits() {
    g_buttons_bits = (!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0))         |
                    ((!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1)) << 1)   |
                    ((!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2)) << 2)   |
                    ((!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3)) << 3)   |
                    ((!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4)) << 4)   |
                    ((!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5)) << 5)   |
                    ((!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6)) << 6)   |
                    ((!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7)) << 7)   |
                    ((!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0)) << 8)   |
                    ((!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1)) << 9)   |
                    ((!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10)) << 10) |
                    ((!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11)) << 11) |
                    ((!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12)) << 12) |
                    ((!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13)) << 13) |
                    ((!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14)) << 14) |
                    ((!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15)) << 15);
}
