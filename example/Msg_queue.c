#include "Msg_queue.h"

int modularIncrement(int counter, int mod) {
  return (counter + 1) % mod;
}

/**
 * bufferIncrement
 * Increments the current_pointer integer modulo the buffer size
 */
void bufferIncrement(CircularBuffer* buffer) {
  buffer->current_pointer = modularIncrement(
    buffer->current_pointer,
    CIRCULAR_BUFFER_SIZE
  );
}

/**
 * createBuffer
 * Initializes the buffer variables and clears the messages
 */
void createBuffer(CircularBuffer* buffer) {
  buffer->current_pointer = -1;
  bufferClearAll(buffer);
}

/**
 * bufferGetCurrentIndex
 * @returns: The current_pointer integer
 */
int bufferGetCurrentIndex(CircularBuffer* buffer) {
  return buffer->current_pointer;
}

/**
 * bufferGetArray
 * @returns: A reference to the buffer array. Note that since it's a reference,
 * it will be changed as the buffer state changes.
 */
msg** bufferGetArray(CircularBuffer* buffer) {
  return buffer->array;
}

/**
 * bufferGetAllMessages
 * @param result: Should be an empty array of messages identical in size to
 *                the buffer's array. The result value will be stored in this
 *                variable.
 * @returns: Array of message pointers
 * This method goes through the circular buffer starting from the oldest known
 * value in the circular array.
 */
msg** bufferGetAllMessages(CircularBuffer* buffer, msg** result) {
  int starting_index = (buffer->current_pointer + 1) % CIRCULAR_BUFFER_SIZE;
  int i;

  for(i = 0; i < CIRCULAR_BUFFER_SIZE; i++) {
    int index = (i + starting_index) % CIRCULAR_BUFFER_SIZE;
    result[i] = buffer->array[index];
  }
}

/**
 * bufferGetAtIndex
 * @param index: The index to retrieve the message at.
 * @returns: Message at index.
 * Gets the message at the specified array index.
 */
msg* bufferGetAtIndex(CircularBuffer* buffer, int index) {
  return buffer->array[index];
}

/**
 * bufferAdd
 * @param message: Message object to add to the buffer
 * Adds an element to the buffer.
 */
void bufferAdd(CircularBuffer* buffer, msg* message) {
  bufferIncrement(buffer);
  memset(buffer->array[buffer->current_pointer], 0,256);
  memcpy(buffer->array[buffer->current_pointer], message, strlen(message));
  buffer->num_added++;
}

/**
 * bufferGetLastMessage
 * @returns: Pointer to the last message added to the buffer
 */
msg* bufferGetLastMessage(CircularBuffer* buffer) {
  return buffer->array[buffer->current_pointer];
}

/**
 * bufferGetPrevious
 * @param n: The number of positions to go back in the buffer
 * @returns: The message at the position that is n positions back from the current head.
 */
msg* bufferGetPrevious(CircularBuffer* buffer, int n) {
  int pointer = (buffer->current_pointer - n) % CIRCULAR_BUFFER_SIZE;
  return buffer->array[pointer];
}

/**
 * bufferClearAll
 * Clears all elements from the buffer
 */
void bufferClearAll(CircularBuffer* buffer) {
  int i;
  for (i = 0; i < CIRCULAR_BUFFER_SIZE; i++) {
    buffer->array[i]->msgType = 0;
    memcpy(buffer->array[i]->msgTitle, 0, 40);
    memcpy(buffer->array[i]->msgText, 0, 215);
  }

  buffer->num_added = 0;
}

/**
 * bufferGetNumAdded
 * @returns: The total number of elements added to the buffer
 */
int bufferGetNumAdded(CircularBuffer* buffer) {
  return buffer->num_added;
}
