/*
 * EXTI_Interface.h
 *
 *  Created on: Nov 15, 2021
 *      Author: gerges
 */

#ifndef MEXTI_INTERFACE_H_
#define MEXTI_INTERFACE_H

#define EXTI0  0U
#define EXTI1  1U
#define EXTI2  2U

#define  MEXTI_LOW_LEVEL                0U
#define  MEXTI_ANY_LOGICAL_CHANGE       1U
#define  MEXTI_FALLING_EDGE             2U
#define  MEXTI_RISING_EDGE              3U


/* EXTI0 Initialization*/
void MEXTI0_VidInit(void);
/* EXTI1 Initialization*/
void MEXTI1_VidInit(void);
/* EXTI2 Initialization*/
void MEXTI2_VidInit(void);

void EXTI0_Set_CallBackFunction(void (*Copy_VidCallBackFunction)(void));
void EXTI1_Set_CallBackFunction(void (*Copy_VidCallBackFunction)(void));
void EXTI2_Set_CallBackFunction(void (*Copy_VidCallBackFunction)(void));


void EXTI_VidSenseControl (unsigned char Copy_u8EXTI_ID , unsigned char Copy_u8SenseControl);
#endif /* MEXTI_INTERFACE_H_ */
