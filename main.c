#include "LCD_Driver.h"
#include "Graphics.h"
#include "USART_Example.h"
#include "RTC_Driver.h"
#include "Msg_queue.h"
#include "Raleway_clock.h"
#include "Raleway_20.h"
#include "Roboto_24.h"
#include "chat_icon.h"

#define START_TIMEOUT TIM_Cmd(TIM2, ENABLE)
#define STOP_TIMEOUT TIM_Cmd(TIM2, DISABLE)

#define MSG_START_X 47
#define CLOCK_X 90

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

char timestring[8] = "??:??PM";

//Handler for msg display timeout
void TIM2_IRQHandler(void)
{
    //Check interrupt was triggered by update event of TIMn
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
    	rtc_time_flag = 1;
        //Clear the interrupt pending flag for timer update
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        TIM_Cmd(TIM2,DISABLE);
    }
}


void initTimeOut(){

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
	setFont(&raleway_48ptFontInfo,&raleway_48ptDescriptors,&raleway_48ptBitmaps);
	getTime(&hour, &minute);
	if(hour >=0 || minute >=0){
		timestring[5] = (hour < 12) ? 'A' : 'P';
		hour = hour % 12;
		hour = (hour == 0) ? 12 : hour;
		timestring[0] = hour >= 10 ? ((hour / 10) + 48) : '0';
		timestring[1] = (hour % 10) + 48;
		timestring[3] = (minute / 10) + 48;
		timestring[4] = (minute % 10) + 48;
	}
	setCursor(CLOCK_X, Y_PADDING*2);
	writeString(timestring,1);
	setCursor(X_PADDING, 75);
	setFont(&raleway_20ptFontInfo,&raleway_20ptDescriptors,&raleway_20ptBitmaps);
	setCursor(MSG_START_X, cursor_y);
	for (i = 0; i < 2; i++) {
		if(strlen(getMsg(i)) > 0){
			drawBitmap(X_PADDING, cursor_y ,chat, chat_width, chat_height,WHITE);
			writeString((char *) getMsg(i),1);
			setCursor(MSG_START_X, cursor_y + chat_height);
		}
	}
	fillRect(133, 210, 133, 30, BLACK);
	setCursor(170, 212);
	setTextColor(WHITE, BLACK);
	writeString("More",1);
	setTextColor(BLACK, WHITE);
	refresh();
}

void printNewMsg() {
	STOP_TIMEOUT;
	clear();
	setFont(&raleway_20ptFontInfo,&raleway_20ptDescriptors,&raleway_20ptBitmaps);
	fillRect(0, 0, 400, 33, BLACK);
	setTextColor(WHITE, BLACK);
	setCursor(X_PADDING, Y_PADDING );
	writeString("New Message!", 1);
	setFont(&roboto_24ptFontInfo,&roboto_24ptDescriptors,&roboto_24ptBitmaps);
	setCursor(MSG_START_X,36);
	setTextColor(BLACK, WHITE);
	writeString("Some App", 1);
	drawBitmap(X_PADDING, cursor_y ,chat, chat_width, chat_height,WHITE);
	setCursor(MSG_START_X,cursor_y+getFontInfo()->height);
	setFont(&raleway_20ptFontInfo,&raleway_20ptDescriptors,&raleway_20ptBitmaps);
	writeString((char*)(RxBuffer+1), 1);
	fillRect(0, 210, 400, 33, BLACK);
	fillRect(133, 210, 133, 33, WHITE);
	drawMsgBtns();
	refresh();
	setCursor(X_PADDING, 0);
	enqeue ((uint8_t *)(RxBuffer+1));
	memset(RxBuffer,0,RXBUFFERSIZE);
}

void drawMsgBtns() {
	setTextColor(WHITE, BLACK);
	setCursor(12, 212);
	writeString("Previous", 1);
	setCursor(305, 212);
	writeString("Next", 1);
	setCursor(17, 212);
	setTextColor(BLACK, WHITE);
	writeString("EXIT", 1);
}

int main(void)
{
	initTimeOut();
	initQueue();
	UART_init();
	LCDInit();
	clearMem(WHITE);
	LCDStart();
	while (1) {
		if (msg_flag) {
			if(RxBuffer[0] == 1){
				printNewMsg();
				START_TIMEOUT;
			} else if(RxBuffer[0] == 2){
				uint8_t h = RxBuffer[1]-1;
				uint8_t m = RxBuffer[2]-1;
				uint8_t s = RxBuffer[3]-1; //since it's probably a bad idea to have a data value that's the same as the string terminator
				RTCinit(h,m,s);
				rtc_time_flag = 1;
			}
			msg_flag = 0;
			RxBuffer[0] = 0;
		}if (rtc_time_flag) {
			printTime();
			rtc_time_flag = 0;
		}

	}

}
