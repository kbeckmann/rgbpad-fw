#include "stm32f0xx_hal.h"

#include "apa102.h"
#include "board.h"

uint32_t fulsin(uint32_t x, uint32_t max) {
    uint32_t y = x % (max*2 - 2);
    if (y >= max) {
        return 2 * max - 2 - y;
    } else {
        return y;
    }
}

/*
#define FOO(GPIOx, GPIO_Pin) (GPIOx->IDR & GPIO_Pin)
DigitalIn pins[16] =
{
    DigitalIn(PA_0, PullUp),  DigitalIn(PA_1, PullUp),  DigitalIn(PA_2, PullUp),  DigitalIn(PA_3, PullUp),
    DigitalIn(PA_7, PullUp),  DigitalIn(PA_6, PullUp),  DigitalIn(PA_5, PullUp),  DigitalIn(PA_4, PullUp),
    DigitalIn(PB_0, PullUp),  DigitalIn(PB_1, PullUp),  DigitalIn(PB_10, PullUp), DigitalIn(PB_11, PullUp),
    DigitalIn(PB_15, PullUp), DigitalIn(PB_14, PullUp), DigitalIn(PB_13, PullUp), DigitalIn(PB_12, PullUp)
};
*/

uint8_t rgb[4 * 16];
void app_run() {
    uint32_t t = 0;

    uart_printf("RGBPAD ready to do awesome stuff!\r\n");

    HAL_Delay(500);
    while (1) {
        if ( HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 0) {
            t++;
            uint32_t sint = 31;//fulsin(t, 32);
            uint8_t high = sint*sint/4;
            uint8_t low = high / 2;
            for(int i = 0; i < sizeof(rgb) / 4; i++) {
                rgb[4*i + 0] = 3;
                rgb[4*i + 1] = i%4 == 0 ? high : 0;
                rgb[4*i + 2] = i%4 == 1 ? high : i%4 == 3 ? low : 0;
                rgb[4*i + 3] = i%4 == 2 ? high : i%4 == 3 ? low : 0;
            }
        }
        else {
            for(int i = 0; i < sizeof(rgb) / 4; i++) {
                rgb[4*i + 0] = 3;
                rgb[4*i + 1] = 255;
                rgb[4*i + 2] = 255;
                rgb[4*i + 3] = 255;
            }
            t = 0;
        }

        apa102_send_buffer(GPIOA, GPIO_PIN_9,
                           GPIOA, GPIO_PIN_8,
                           (uint32_t*)rgb, sizeof(rgb) / 4);

        HAL_Delay(20);
    }
}
