#ifndef RTC_DRIVER
#define RTC_DRIVER

#include "misc.h"
#include "stm32l1xx.h"
#include "stm32l1xx_exti.h"
#include "stm32l1xx_rtc.h"
#include "stm32l1xx_pwr.h"
#include "stm32l1xx_gpio.h"
#include "stm32l1xx_rcc.h"

uint8_t rtc_time_flag;
uint8_t rtc_init;
void getTime(short *hour,short *minute);
void RTCinit(uint8_t hour,uint8_t minute, uint8_t second);

#endif
