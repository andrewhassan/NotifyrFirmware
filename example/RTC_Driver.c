
#include "RTC_Driver.h"

RTC_TimeTypeDef RTC_TimeStructure;
RTC_InitTypeDef RTC_InitStructure;
RTC_AlarmTypeDef RTC_AlarmStructure;

__IO uint32_t AsynchPrediv = 0, SynchPrediv = 0;

void RTC_Config() {
	/* Enable the PWR clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	/* Allow access to RTC */
	PWR_RTCAccessCmd(ENABLE);

	/* Enable the LSE OSC */
	RCC_LSEConfig(RCC_LSE_ON);

	/* Wait till LSE is ready */
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);

	/* Select the RTC Clock Source */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

	SynchPrediv = 0xFF;
	AsynchPrediv = 0x7F;

	/* Enable the RTC Clock */
	RCC_RTCCLKCmd(ENABLE);

	/* Wait for RTC APB registers synchronisation */
	RTC_WaitForSynchro();
}

void RTC_TimeRegulate(uint8_t hour,uint8_t minute, uint8_t second)
{
	  RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
	  RTC_TimeStructure.RTC_Hours = (uint32_t)hour;
	  RTC_TimeStructure.RTC_Minutes = (uint32_t)minute;
	  RTC_TimeStructure.RTC_Seconds = (uint32_t)second;

	  /* Configure the RTC time register */
	  if(RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure) == ERROR){
		  while(1);
	  }
	  else
	  {	    /* Indicator for the RTC configuration */
		  rtc_time_flag = 1;
	  }

	/* Disable the Alarm A */
	RTC_AlarmCmd(RTC_Alarm_A, DISABLE);

	RTC_AlarmStructure.RTC_AlarmTime.RTC_H12 = RTC_H12_AM;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours = 0;

	RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = 0;

	RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = 0;
	/* Set the Alarm A */
	RTC_AlarmStructure.RTC_AlarmDateWeekDay = 0x31;
	RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
	RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay
			| RTC_AlarmMask_Hours | RTC_AlarmMask_Minutes;

	/* Configure the RTC Alarm A register */
	RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);

	/* Enable the RTC Alarm A Interrupt */
	RTC_ITConfig(RTC_IT_ALRA, ENABLE);

	/* Enable the alarm  A */
	RTC_AlarmCmd(RTC_Alarm_A, ENABLE);


}

void RTCinit(uint8_t hour,uint8_t minute, uint8_t second){
	NVIC_InitTypeDef  NVIC_InitStructure;
	EXTI_InitTypeDef  EXTI_InitStructure;

	RTC_Config();

	/* Configure the RTC data register and RTC prescaler */
	RTC_InitStructure.RTC_AsynchPrediv = AsynchPrediv;
	RTC_InitStructure.RTC_SynchPrediv = SynchPrediv;
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;

	/* Check on RTC init */
	if (RTC_Init(&RTC_InitStructure) == ERROR) {
		while (1);
	}

	/* Configure the time register */
	RTC_TimeRegulate(hour, minute, second);

	EXTI_ClearITPendingBit(EXTI_Line17);
	EXTI_InitStructure.EXTI_Line = EXTI_Line17;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable the RTC Alarm Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	rtc_init = 1;
}

void getTime(short *hour,short *minute){
	if(rtc_init){
		RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
		*hour = RTC_TimeStructure.RTC_Hours;
		*minute = RTC_TimeStructure.RTC_Minutes;
	} else{
		*hour = -1;
		*minute = -1;
	}
}


void RTC_Alarm_IRQHandler(){
  if(RTC_GetITStatus(RTC_IT_ALRA) != RESET)
  {
	RTC_ClearITPendingBit(RTC_IT_ALRA);
    EXTI_ClearITPendingBit(EXTI_Line17);
    rtc_time_flag = 1;
  }
}
