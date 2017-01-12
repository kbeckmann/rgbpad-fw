#include "stm32f0xx_hal.h"

#include "apa102.h"
#include "board.h"
#include "buttons.h"

#define RGBTEST 1

uint32_t g_framebuffer[16];

static void hsvtorgb(unsigned char *r, unsigned char *g, unsigned char *b, unsigned char h, unsigned char s, unsigned char v)
{
  unsigned char region, fpart, p, q, t;

  if(s == 0) {
    /* color is grayscale */
    *r = *g = *b = v;
    return;
  }

  /* make hue 0-5 */
  region = h / 43;
  /* find remainder part, make it from 0-255 */
  fpart = (h - (region * 43)) * 6;

  /* calculate temp vars, doing integer multiplication */
  p = (v * (255 - s)) >> 8;
  q = (v * (255 - ((s * fpart) >> 8))) >> 8;
  t = (v * (255 - ((s * (255 - fpart)) >> 8))) >> 8;

  /* assign temp vars based on color cone region */
  switch(region) {
    case 0:
      *r = v; *g = t; *b = p; break;
    case 1:
      *r = q; *g = v; *b = p; break;
    case 2:
      *r = p; *g = v; *b = t; break;
    case 3:
      *r = p; *g = q; *b = v; break;
    case 4:
      *r = t; *g = p; *b = v; break;
    default:
      *r = v; *g = p; *b = q; break;
  }

  return;
}


#if RGBTEST
static void rgb_test() {
    uint32_t t = 0;
    uint8_t rgb[16*4];
    uint8_t brightness = 1;
    float val = 0, sval = 0, q = 0;

    while (1) {
        t++;
        read_buttons();




        int x;
        HAL_ADC_Start(&hadc);
        HAL_ADC_PollForConversion(&hadc, 1000);
        x = HAL_ADC_GetValue(&hadc);
        HAL_ADC_Stop(&hadc);


        sval = x*0.01 + sval*0.99;
        val = x*0.03 + val*0.97;
        float m = val > sval?val-sval:sval-val;
        q = (m) * 0.02 + q*0.98;
        brightness = (uint8_t) (q<0?0:q>255?255:q);

        t++;



        uint8_t r, g, b;
        hsvtorgb(&r, &g, &b, brightness + (t>>4), 255, 255);


        uart_printf("%d\r\n", brightness);
        for(uint8_t i = 0; i < BUTTONS_COUNT; i++) {
            rgb[4*i + 0] = 31;
            rgb[4*i + 1] = r;
            rgb[4*i + 2] = g;
            rgb[4*i + 3] = b;
        }

        apa102_send_buffer(GPIOA, GPIO_PIN_9,
                           GPIOA, GPIO_PIN_8,
                           (uint32_t*)rgb, sizeof(rgb) / 4);

//        HAL_Delay(250);
    }
}
#endif

static void push_led_framebuffer() {
    apa102_send_buffer(GPIOA, GPIO_PIN_9, GPIOA, GPIO_PIN_8,
                       g_framebuffer, sizeof(g_framebuffer) / sizeof(g_framebuffer[0]));
}

static void init_random_seed() {
    HAL_ADC_Start(&hadc);
    HAL_ADC_PollForConversion(&hadc, 1);
    srand(HAL_ADC_GetValue(&hadc));
    HAL_ADC_Stop(&hadc);
}

static void random_framebuffer() {
    init_random_seed();
    for (uint32_t i = 0; i < sizeof(g_framebuffer)/sizeof(g_framebuffer[0]); i++) {
        g_framebuffer[i] = rand();
    }
    push_led_framebuffer();
}

static void i2c_slave() {
    // check keys
    uart_printf("i2c_slave\r\n");
    while(1) {
        uint8_t data[2] = {};
        HAL_StatusTypeDef ret = HAL_I2C_Slave_Receive(&hi2c1, data, 1, 1000);
        if (ret == HAL_OK) {
            switch(data[0]) {
                case 0x1:
                    read_buttons_bits();
                    ret = HAL_I2C_Slave_Transmit(&hi2c1, (uint8_t*)&g_buttons_bits, sizeof(g_buttons_bits), 10);
                    uart_printf("0x01: TX %02x %02x\r\n", ((uint8_t*)&g_buttons_bits)[0], ((uint8_t*)&g_buttons_bits)[1]);
                    break;
                case 0x2:
                    ret = HAL_I2C_Slave_Receive(&hi2c1, (uint8_t*)g_framebuffer, sizeof(g_framebuffer), 100);
                    push_led_framebuffer();
                    break;
                default:
                    break;
            }
        }
    }
}

void app_run() {
    uart_printf("RGBPAD ready to do awesome stuff!\r\n");

    random_framebuffer();

#if RGBTEST
    rgb_test();
#else
    i2c_slave();
#endif


}
