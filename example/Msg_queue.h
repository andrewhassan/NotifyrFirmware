#ifndef MSG_QUEUE
#define MSG_QUEUE

#include "stm32l1xx.h"

#define CIRCULAR_BUFFER_SIZE 8

typedef struct message{
	uint8_t msgType;
	uint8_t msgTitle[40];
	uint8_t msgText[215];
} msg;

typedef struct CircularBufferStruct {
  int current_pointer;
  msg array[CIRCULAR_BUFFER_SIZE];
  int num_added;
} CircularBuffer;

int hack_counter;

// Public methods
void createBuffer(CircularBuffer* buffer);
int bufferGetCurrentIndex(CircularBuffer* buffer);
void bufferAdd(CircularBuffer* buffer, msg* message);
msg* bufferGetAtIndex(CircularBuffer* buffer, int index);
msg* bufferGetLastMessage(CircularBuffer* buffer);
msg* bufferGetPreviousMessage(CircularBuffer* buffer, int n);
msg** bufferGetAllMessages(CircularBuffer* buffer, msg** result);
msg** bufferGetArray(CircularBuffer* buffer);
msg* bufferGetPrevious(CircularBuffer* buffer, int n);
void bufferClearAll(CircularBuffer* buffer);
int bufferGetNumAdded(CircularBuffer* buffer);


#endif
