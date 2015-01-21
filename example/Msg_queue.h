#ifndef MSG_QUEUE
#define MSG_QUEUE

#include "stm32l1xx.h"

#define QUEUE_SIZE 8

typedef struct message{
	uint8_t msgType;
	uint8_t msgTitle[40];
	uint8_t msgText[215];
} msg;

msg msg_string[QUEUE_SIZE+1];
int first;
int last;
int count;

void initQueue();
void enqeue(msg *msg);
void dequeue();
msg* getMsg(int pos);

#endif
