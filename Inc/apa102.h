#ifndef _APA102_H
#define _APA102_H


void apa102_send_buffer(GPIO_TypeDef* clock_gpio, uint16_t clock_gpio_pin,
                        GPIO_TypeDef* data_gpio,  uint16_t data_gpio_pin,
                        uint32_t *buf, uint32_t nbr_frames);


#endif // _APA102_H