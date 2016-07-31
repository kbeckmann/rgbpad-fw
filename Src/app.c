#include "stm32f0xx_hal.h"

#include "apa102.h"
#include "board.h"
#include "buttons.h"

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

#define STEP(x, y) (x < y   ? (x + 1) : (x > y) ? (x - 1) : y)
uint8_t rgb[4 * BUTTONS_COUNT];
uint8_t col[4 * BUTTONS_COUNT];

void generate_colors() {
    for (uint8_t i = 0; i < BUTTONS_COUNT; i++) {
        col[3*i + 0] = (rand() % 4) * 64;
        col[3*i + 1] = (rand() % 2) * 64;
        col[3*i + 2] = (rand() % 4) * 64;
    }
}

void app_run() {
    uint32_t t = 0;

    uart_printf("RGBPAD ready to do awesome stuff!\r\n");

    while (1) {
        t++;
        if ((t % 64) == 0)
        {
            generate_colors();
        }

        read_buttons();
        for(uint8_t i = 0; i < BUTTONS_COUNT; i++) {
            if (g_buttons[i]) {
                rgb[4*i + 0] = 31;
                rgb[4*i + 1] = STEP(rgb[4*i + 1], col[3*i + 0]);
                rgb[4*i + 2] = STEP(rgb[4*i + 2], col[3*i + 1]);
                rgb[4*i + 3] = STEP(rgb[4*i + 3], col[3*i + 2]);
            }
            else {
                rgb[4*i + 0] = 31;
                rgb[4*i + 1] = 0;STEP(rgb[4*i + 1], 0);
                rgb[4*i + 2] = 0;STEP(rgb[4*i + 2], 0);
                rgb[4*i + 3] = 0;STEP(rgb[4*i + 3], 0);
            }
        }

        apa102_send_buffer(GPIOA, GPIO_PIN_9,
                           GPIOA, GPIO_PIN_8,
                           (uint32_t*)rgb, sizeof(rgb) / 4);

        HAL_Delay(10);
    }
}
