#include "Msg_queue.h"

const uint8_t *nullString = "";


void initQueue(){
	first = 0;
	last = QUEUE_SIZE-1;
	count = 0;
}
void enqeue(uint8_t *msg){
	int msgLength = strlen(msg);
	if(count >= QUEUE_SIZE){
		dequeue();
	}
	last = (last+1) % QUEUE_SIZE;
	memset(msg_string[last],0,39);
	if(msgLength>35){
		memcpy(msg_string[last],msg,35);
		memset((uint8_t*)(msg_string[last]+35),'.',3);
	}else{
		memset(msg_string[last],0,256);
		memcpy(msg_string[last],msg,msgLength);
	}
	count++;
}

void dequeue(){
	if(count >0){
		first = (first+1) % QUEUE_SIZE;
		count--;
	}
}
uint8_t* getMsg(int pos){

	int final_position = 0;
	uint8_t* returnValue;
	if(pos > last && pos < QUEUE_SIZE && QUEUE_SIZE- (pos-last) > 0){
		final_position = QUEUE_SIZE-(pos-last);
	}else if(pos<=last){
		final_position = last-pos;
	}

	if(pos >=0 && pos < QUEUE_SIZE){
		returnValue = msg_string[final_position];
	}else{
		returnValue = nullString;
	}
	return returnValue;
}

