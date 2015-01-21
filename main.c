#include "LCD_Driver.h"
#include "Graphics.h"
#include "USART_Example.h"
#include "RTC_Driver.h"
#include "GPIO_Example.h"
#include "Msg_queue.h"
#include "Raleway_clock.h"
#include "Raleway_20.h"
#include "Roboto_24.h"
#include "chat_icon.h"
#include "email_icon.h"
#include "phone_icon.h"
#include "snapchat_icon.h"

#define START_TIMEOUT TIM_Cmd(TIM2, ENABLE)
#define STOP_TIMEOUT TIM_Cmd(TIM2, DISABLE)

#define MSG_START_X 47
#define CLOCK_X 90

#define SNAPCHAT_ICON 1
#define EMAIL_ICON 2
#define CHAT_ICON 3
#define PHONE_ICON 4

extern int16_t cursor_x;
extern int16_t cursor_y;
extern uint16_t textcolor;
extern uint16_t textbgcolor;
extern const uint8_t *nullString;
extern uint8_t textsize;
extern uint8_t wrap;

short hour = 0;
short minute = 0;
short i = 0;

uint8_t showHistoryFlag = 0;
uint8_t lockoutCommands = 0;

char timestring[8] = "??:??PM";

//Handler for msg display timeout
void TIM2_IRQHandler(void) {
	//Check interrupt was triggered by update event of TIMn
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {

		rtc_time_flag = 1;
		//Clear the interrupt pending flag for timer update
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		TIM_Cmd(TIM2, DISABLE);
	}
}

void writeSummary(char *message) {
	int i = 0;
	uint8_t continueFlag = 0;
	for (i = 0; i < 35; i++) {
		if (message[i] != 0) {
			write(message[i], 1);
		} else {
			return;
		}
	}
	if (message[35] != '\0') {
		for (i = 0; i < 3; i++) {
			write('.', 1);
		}
	}

}

void drawIcon(uint8_t code) {
	switch (code) {
	case (CHAT_ICON):
		drawBitmap(X_PADDING, cursor_y, chat, chat_width, chat_height, WHITE);
		break;
	case (EMAIL_ICON):
		drawBitmap(X_PADDING, cursor_y, email, email_width, email_height,
				WHITE);
		break;
	case (PHONE_ICON):
		drawBitmap(X_PADDING, cursor_y, phone, phone_width, phone_height,
				WHITE);
		break;
	case (SNAPCHAT_ICON):
		drawBitmap(X_PADDING, cursor_y, snapchat, snapchat_width,
				snapchat_height, WHITE);
		break;
	default:
		drawBitmap(X_PADDING, cursor_y, chat, chat_width, chat_height, WHITE);
		break;
	}
}

void drawMsgBtns() {
	fillRect(0, 210, 400, 33, BLACK);
	fillRect(133, 210, 133, 33, WHITE);
	setTextColor(WHITE, BLACK);
	setCursor(12, 212);
	writeString("Previous", 1);
	setCursor(305, 212);
	writeString("Next", 1);
	setCursor(175, 212);
	setTextColor(BLACK, WHITE);
	writeString("Exit", 1);
}

void initTimeOut() {

	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = 4000 - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = 32000 - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	//init TIM2 interrupt
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void printTime() {
	clear();
	setFont(&raleway_48ptFontInfo, &raleway_48ptDescriptors,
			&raleway_48ptBitmaps);
	getTime(&hour, &minute);
	if (hour >= 0 || minute >= 0) {
		timestring[5] = (hour < 12) ? 'A' : 'P';
		hour = hour % 12;
		hour = (hour == 0) ? 12 : hour;
		timestring[0] = hour >= 10 ? ((hour / 10) + 48) : '0';
		timestring[1] = (hour % 10) + 48;
		timestring[3] = (minute / 10) + 48;
		timestring[4] = (minute % 10) + 48;
	}
	setCursor(CLOCK_X, Y_PADDING * 2);
	writeString(timestring, 1);
	setCursor(X_PADDING, 75);
	setFont(&raleway_20ptFontInfo, &raleway_20ptDescriptors,
			&raleway_20ptBitmaps);
	setCursor(MSG_START_X, cursor_y);
	for (i = 0; i < 2; i++) {
		if (strlen(getMsg(i)) > 0) {
			drawBitmap(X_PADDING, cursor_y, chat, chat_width, chat_height,
					WHITE);
			writeSummary((msg *) getMsg(i)->msgText);
			setCursor(MSG_START_X, cursor_y + chat_height);
		}
	}
	fillRect(133, 210, 133, 30, BLACK);
	setCursor(170, 212);
	setTextColor(WHITE, BLACK);
	writeString("More", 1);
	setTextColor(BLACK, WHITE);
	refresh();
}

void printMsg(msg *message, uint8_t isHistoryMode) {
	clear();
	setFont(&raleway_20ptFontInfo, &raleway_20ptDescriptors,
			&raleway_20ptBitmaps);
	fillRect(0, 0, 400, 33, BLACK);
	setTextColor(WHITE, BLACK);
	setCursor(X_PADDING, Y_PADDING);
	writeString(isHistoryMode ? "" : "New Message!", 1);
	setFont(&roboto_24ptFontInfo, &roboto_24ptDescriptors, &roboto_24ptBitmaps);
	setCursor(MSG_START_X, 36);
	setTextColor(BLACK, WHITE);
	writeString(message->msgTitle, 1);
	drawIcon(message->msgType);
	setCursor(MSG_START_X, cursor_y + getFontInfo()->height);
	setFont(&raleway_20ptFontInfo, &raleway_20ptDescriptors,
			&raleway_20ptBitmaps);
	writeString(message->msgText, 1);
	if (isHistoryMode) {
		drawMsgBtns();
	}
	refresh();
	setCursor(X_PADDING, 0);
}

void printNewMsg() {
	lockoutCommands = 1;
	showHistoryFlag = 0;
	msg *message = (msg*) (RxBuffer + 1);
	STOP_TIMEOUT;
	printMsg(message, 0);
	enqeue((msg*) (RxBuffer + 1));
	memset(RxBuffer, 0, RXBUFFERSIZE);
}

void handleBtnClick() {
	if (lockoutCommands) {
		return;
	}
	switch(btn_click_code){
	case 1:
		writeString("1", 0);
		refresh();
		break;
	case 2:
		if (!showHistoryFlag) {
			btn_click_code = 0;
			if (count > 0) {
				showHistoryFlag = 1;
				printMsg(getMsg(0), 1);
			}
		} else {
			btn_click_code = 0;
			showHistoryFlag = 0;
			printTime();
		}
		break;
	case 4:
		writeString("4", 0);
		refresh();
		break;
	}

	btn_click_code = 0;
}

int main(void) {
	initTimeOut();
	initQueue();
	UART_init();
	LCDInit();
	clearMem(WHITE);
	LCDStart();
	initBtnGPIO();
	setCursor(0, 0);
	setTextColor(BLACK, WHITE);
	while (1) {
		if (msg_flag) {
			if (RxBuffer[0] == 1) {
				printNewMsg();
				START_TIMEOUT;
			} else if (RxBuffer[0] == 2) {
				uint8_t h = RxBuffer[1] - 1;
				uint8_t m = RxBuffer[2] - 1;
				uint8_t s = RxBuffer[3] - 1; //since it's probably a bad idea to have a data value that's the same as the string terminator
				RTCinit(h, m, s);
				rtc_time_flag = 1;
			}
			msg_flag = 0;
			RxBuffer[0] = 0;
		}
		if (rtc_time_flag) {
			lockoutCommands = 0;
			printTime();
			rtc_time_flag = 0;
		}
		if (btn_click_code > 0) {
			handleBtnClick();
		}
#ifdef PRODUCTION
		PWR_EnterSleepMode(PWR_Regulator_ON,PWR_SLEEPEntry_WFI);
#endif
	}

}
