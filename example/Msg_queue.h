#ifndef MSG_QUEUE
#define MSG_QUEUE

#include "stm32l1xx.h"

#define QUEUE_SIZE 8

uint8_t msg_string[QUEUE_SIZE+1][256];
int first;
int last;
int count;

void initQueue();
void enqeue(uint8_t *msg);
void dequeue();
uint8_t* getMsg(int pos);

#endif
