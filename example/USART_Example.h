#ifndef UART_DRIVER
#define UART_DRIVER

/* Includes ------------------------------------------------------------------*/
#include "misc.h"
#include "stm32l1xx.h"
#include "stm32l1xx_gpio.h"
#include "stm32l1xx_rcc.h"
#include "stm32l1xx_usart.h"

/** @addtogroup STM32L1xx_StdPeriph_Examples
 * @{
 */

/** @addtogroup USART_USART
 * @{
 */

#define RXBUFFERSIZE   512

/* Private variables ---------------------------------------------------------*/
USART_InitTypeDef USART_InitStructure;
extern uint8_t NbrOfDataToTransfer;



uint8_t RxBuffer[RXBUFFERSIZE];
uint8_t msg_flag;
volatile char in;
/* Private function prototypes -----------------------------------------------*/
void UART_init();
#endif
