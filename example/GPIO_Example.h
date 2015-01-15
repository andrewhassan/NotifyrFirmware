#ifndef BTN_DRIVER
#define BTN_DRIVER

#include "stm32l1xx.h"
#include "stm32l1xx_rcc.h"
#include "stm32l1xx_gpio.h"
#include "stm32l1xx_exti.h"
#include "stm32l1xx_syscfg.h"
#include "misc.h"

volatile uint8_t btn_click_code;

void initBtnGPIO();

void clearMem(int16_t color);
void drawPixel(int16_t x, int16_t y, uint16_t color);
void refresh();

#endif
