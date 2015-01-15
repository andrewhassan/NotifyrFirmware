/**
 ******************************************************************************
 * @file    GPIO/IOToggle/main.c
 * @author  MCD Application Team
 * @version V1.1.1
 * @date    13-April-2012
 * @brief   Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
 *
 * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *        http://www.st.com/software_license_agreement_liberty_v2
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "GPIO_Example.h"

void initBtnGPIO(){
	EXTI_InitTypeDef   EXTI_InitStructure;
	    GPIO_InitTypeDef   GPIO_InitStructure;
	    NVIC_InitTypeDef   NVIC_InitStructure;

	  /* Enable GPIOA and GPIOB clock */
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

	  /* Configure PA0 pin as input floating */
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_3|GPIO_Pin_4;
	  GPIO_Init(GPIOB, &GPIO_InitStructure);

	  /* Enable SYSCFG clock */
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	  /* Connect EXTI0 Line to PA0 pin */
	  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource3);
	  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource4);
	  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource5);

	  /* Configure EXTI0 line */
	  EXTI_InitStructure.EXTI_Line = EXTI_Line5;
	  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	  EXTI_Init(&EXTI_InitStructure);

	  EXTI_InitStructure.EXTI_Line = EXTI_Line3;
	  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	  EXTI_Init(&EXTI_InitStructure);

	  EXTI_InitStructure.EXTI_Line = EXTI_Line4;
	  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	  EXTI_Init(&EXTI_InitStructure);

	  /* Enable and set EXTI5-9 Interrupt to the lowest priority */
	  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	  NVIC_Init(&NVIC_InitStructure);

	  /* Enable and set EXTI5-9 Interrupt to the lowest priority */
	  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	  NVIC_Init(&NVIC_InitStructure);

	  NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	  NVIC_Init(&NVIC_InitStructure);
}

void EXTI9_5_IRQHandler(void)
{
	if( EXTI_GetITStatus(EXTI_Line5) != RESET ){
		btn_click_code = 1;
		EXTI_ClearFlag(EXTI_Line5 );
		EXTI_ClearITPendingBit(EXTI_Line5 );
	}


}

void EXTI4_IRQHandler(void)
{
	if( EXTI_GetITStatus(EXTI_Line4) != RESET ){
		btn_click_code = 2;
		EXTI_ClearFlag(EXTI_Line4 );
		EXTI_ClearITPendingBit(EXTI_Line4 );
	}

}

void EXTI3_IRQHandler(void)
{
	if( EXTI_GetITStatus(EXTI_Line3) != RESET ){
		btn_click_code = 4;
		EXTI_ClearFlag(EXTI_Line3);
		EXTI_ClearITPendingBit(EXTI_Line3);
	}

}

#ifdef  USE_FULL_ASSERT

/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while (1)
	{
	}
}
#endif

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
