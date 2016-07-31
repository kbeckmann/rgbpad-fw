// Based on Robert Foss's apa102 driver in nodemcu
// https://github.com/nodemcu/nodemcu-firmware/blob/dev/app/modules/apa102.c

#include "stm32f0xx_hal.h"

static void apa102_send_byte(GPIO_TypeDef* clock_gpio, uint16_t clock_gpio_pin,
                             GPIO_TypeDef* data_gpio,    uint16_t data_gpio_pin,
                             uint8_t data) {
    int i;
    for (i = 0; i < 8; i++) {
        if (data & 0x80) {
            HAL_GPIO_WritePin(data_gpio, data_gpio_pin, GPIO_PIN_SET);
        } else {
            HAL_GPIO_WritePin(data_gpio, data_gpio_pin, GPIO_PIN_RESET);
        }

        HAL_GPIO_WritePin(clock_gpio, clock_gpio_pin, GPIO_PIN_SET);
        data <<= 1;
        HAL_GPIO_WritePin(clock_gpio, clock_gpio_pin, GPIO_PIN_RESET);
    }
}

void apa102_send_buffer(GPIO_TypeDef* clock_gpio, uint16_t clock_gpio_pin,
                        GPIO_TypeDef* data_gpio,    uint16_t data_gpio_pin,
                        uint32_t *buf, uint32_t nbr_frames) {
    int i;

    // Send 32-bit Start Frame that's all 0x00
    apa102_send_byte(clock_gpio, clock_gpio_pin, data_gpio, data_gpio_pin, 0x00);
    apa102_send_byte(clock_gpio, clock_gpio_pin, data_gpio, data_gpio_pin, 0x00);
    apa102_send_byte(clock_gpio, clock_gpio_pin, data_gpio, data_gpio_pin, 0x00);
    apa102_send_byte(clock_gpio, clock_gpio_pin, data_gpio, data_gpio_pin, 0x00);

    // Send 32-bit LED Frames
    for (i = 0; i < nbr_frames; i++) {
        uint8_t *byte = (uint8_t *) buf++;

        // Set the first 3 bits of that byte to 1.
        // This makes the interface easier to use since you
        // don't have to worry about creating invalid LED Frames.
        byte[0] |= 0xE0;
        apa102_send_byte(clock_gpio, clock_gpio_pin, data_gpio, data_gpio_pin, byte[0]);
        apa102_send_byte(clock_gpio, clock_gpio_pin, data_gpio, data_gpio_pin, byte[1]);
        apa102_send_byte(clock_gpio, clock_gpio_pin, data_gpio, data_gpio_pin, byte[2]);
        apa102_send_byte(clock_gpio, clock_gpio_pin, data_gpio, data_gpio_pin, byte[3]);
    }

    // Send 32-bit End Frames
    uint32_t required_postamble_frames = (nbr_frames + 1) / 2;
    for (i = 0; i < required_postamble_frames; i++) {
        apa102_send_byte(clock_gpio, clock_gpio_pin, data_gpio, data_gpio_pin, 0xFF);
        apa102_send_byte(clock_gpio, clock_gpio_pin, data_gpio, data_gpio_pin, 0xFF);
        apa102_send_byte(clock_gpio, clock_gpio_pin, data_gpio, data_gpio_pin, 0xFF);
        apa102_send_byte(clock_gpio, clock_gpio_pin, data_gpio, data_gpio_pin, 0xFF);
    }
}
