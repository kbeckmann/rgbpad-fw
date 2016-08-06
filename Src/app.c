#include "stm32f0xx_hal.h"

#include "apa102.h"
#include "board.h"
#include "buttons.h"

#define RGBTEST 0

uint32_t g_framebuffer[16];

#if RGBTEST
void rgb_test() {
    uint32_t t = 0;
    uint8_t rgb[16*4];
    uint8_t brightness = 255;

    while (1) {
        t++;
        read_buttons();
        uart_printf(".");
        for(uint8_t i = 0; i < BUTTONS_COUNT; i++) {
            if (!g_buttons[i]) {
                rgb[4*i + 0] = 31;
                rgb[4*i + 1] = (t % 3) == 0 ? brightness : 0;
                rgb[4*i + 2] = (t % 3) == 1 ? brightness : 0;
                rgb[4*i + 3] = (t % 3) == 2 ? brightness : 0;
            }
            else {
                rgb[4*i + 0] = 31;
                rgb[4*i + 1] = 0;
                rgb[4*i + 2] = 0;
                rgb[4*i + 3] = 0;
            }
        }

        apa102_send_buffer(GPIOA, GPIO_PIN_9,
                           GPIOA, GPIO_PIN_8,
                           (uint32_t*)rgb, sizeof(rgb) / 4);

        HAL_Delay(400);
    }
}
#endif

void i2c_slave() {
    // check keys
    uart_printf("i2c_slave\r\n");
    while(1) {
        uint8_t data[2] = {};
        HAL_StatusTypeDef ret = HAL_I2C_Slave_Receive(&hi2c1, data, 1, 1000);
        if (ret == HAL_OK) {
            switch(data[0]) {
                case 0x1:
                    read_buttons_bits();
                    ret = HAL_I2C_Slave_Transmit(&hi2c1, (uint8_t*)&g_buttons_bits, sizeof(g_buttons_bits), 1000);
                    uart_printf("TXd %x %x\r\n", ((uint8_t*)&g_buttons_bits)[0], ((uint8_t*)&g_buttons_bits)[1]);
                    break;
                case 0x2:
                    {
                        HAL_StatusTypeDef ret = HAL_I2C_Slave_Receive(&hi2c1, g_framebuffer, sizeof(g_framebuffer), 100);
                        apa102_send_buffer(GPIOA, GPIO_PIN_9,
                                           GPIOA, GPIO_PIN_8,
                                           g_framebuffer, sizeof(g_framebuffer) / sizeof(g_framebuffer[0]));
                        uart_printf("RX a lot! %s\r\n", data);
                        break;
                    }
                default:
                    break;
            }
        }
    }
}

void app_run() {
    uart_printf("RGBPAD ready to do awesome stuff!\r\n");

#if RGBTEST
    rgb_test();
#else
    i2c_slave();
#endif


}
