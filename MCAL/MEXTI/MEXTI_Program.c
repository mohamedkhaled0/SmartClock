/*
 * MEXTI_Program.c
 *
 *  Created on: Nov 15, 2021
 *      Author: gerges
 */

#include"..\..\LIB\LSTD_TYPES.h"
#include"..\..\LIB\LBIT_MATH.h"

#include"MEXTI_Private.h"
#include"MEXTI_Config.h"
#include"MEXTI_Interface.h"

/*Pointer To Function*/
void(*CallBackFunction0)(void);
void(*CallBackFunction1)(void);
void(*CallBackFunction2)(void);

/* EXTI0 Initialization*/
void MEXTI0_VidInit(void){
	/*Select Sense Control*/
#if MEXTI_SET_SENSE_CONTROL==MEXTI_LOW_LEVEL
	CLR_BIT(MCUCR,0);
	CLR_BIT(MCUCR,1);
#elif MEXTI_SET_SENSE_CONTROL==MEXTI_ANY_LOGICAL_CHANGE
	SET_BIT(MCUCR,0);
	CLR_BIT(MCUCR,1);
#elif MEXTI_SET_SENSE_CONTROL==MEXTI_FALLING_EDGE
	CLR_BIT(MCUCR,0);
	SET_BIT(MCUCR,1);
#elif MEXTI_SET_SENSE_CONTROL==MEXTI_RISING_EDGE
	SET_BIT(MCUCR,0);
	SET_BIT(MCUCR,1);
#else
	#error"Sense Control Option is not valid"
#endif
	/*Enable to EXTI0*/
	SET_BIT(GICR,6);
}
/* EXTI1 Initialization*/
void MEXTI1_VidInit(void)
{
	/*Select Sense Control*/
#if MEXTI_SET_SENSE_CONTROL==MEXTI_LOW_LEVEL
	CLR_BIT(MCUCR,2);
	CLR_BIT(MCUCR,3);
#elif MEXTI_SET_SENSE_CONTROL==MEXTI_ANY_LOGICAL_CHANGE
	SET_BIT(MCUCR,2);
	CLR_BIT(MCUCR,3);
#elif MEXTI_SET_SENSE_CONTROL==MEXTI_FALLING_EDGE
	CLR_BIT(MCUCR,2);
	SET_BIT(MCUCR,3);
#elif MEXTI_SET_SENSE_CONTROL==MEXTI_RISING_EDGE
	SET_BIT(MCUCR,2);
	SET_BIT(MCUCR,3);
#else
	#error"Sense Control Option is not valid"
#endif
	/*Enable to EXTI1*/
	SET_BIT(GICR,7);
}
/* EXTI2 Initialization*/
void MEXTI2_VidInit(void)
{
	/*Select Sense Control*/

#if MEXTI_SET_SENSE_CONTROL==MEXTI_FALLING_EDGE
	CLR_BIT(MCUCSR,6);
#elif MEXTI_SET_SENSE_CONTROL==MEXTI_RISING_EDGE
	SET_BIT(MCUCSR,6);
#else

#endif
	/*Enable to EXTI2*/
	SET_BIT(GICR,5);
}




void EXTI0_Set_CallBackFunction(void (*Copy_VidCallBackFunction)(void))
{
	CallBackFunction0=Copy_VidCallBackFunction;
}
void __vector_1(void)       __attribute__((signal));
void __vector_1(void)
{
	CallBackFunction0();
	//HLED_VidLed_Toggle(&LED1);
}

void EXTI1_Set_CallBackFunction(void (*Copy_VidCallBackFunction)(void))
{
	CallBackFunction1=Copy_VidCallBackFunction;
}
void __vector_2(void)       __attribute__((signal));
void __vector_2(void)
{
	CallBackFunction1();
	//HLED_VidLed_Toggle(&LED1);
}


void EXTI2_Set_CallBackFunction(void (*Copy_VidCallBackFunction)(void))
{
	CallBackFunction2=Copy_VidCallBackFunction;
}
void __vector_3(void)       __attribute__((signal));
void __vector_3(void)
{
	CallBackFunction2();
	//HLED_VidLed_Toggle(&LED1);
}




void MEXTI_VidSenseControl(unsigned char Copy_u8EXTI_ID ,unsigned char  Copy_u8SenseControl){
	switch(Copy_u8EXTI_ID){

	case EXTI0:

		if(Copy_u8SenseControl==MEXTI_LOW_LEVEL)
		{
			CLR_BIT(MCUCR,0);
			CLR_BIT(MCUCR,1);
		}
		else if(Copy_u8SenseControl==MEXTI_ANY_LOGICAL_CHANGE)
		{
			SET_BIT(MCUCR,0);
			CLR_BIT(MCUCR,1);
		}
		else if(Copy_u8SenseControl==MEXTI_FALLING_EDGE)
		{
			CLR_BIT(MCUCR,0);
			SET_BIT(MCUCR,1);
		}
		else if(Copy_u8SenseControl==MEXTI_RISING_EDGE)
		{
			SET_BIT(MCUCR,0);
			SET_BIT(MCUCR,1);
		}
		else
		{
			/*Do Nothing*/
		}
		break;
	case EXTI1:
		if(Copy_u8SenseControl==MEXTI_LOW_LEVEL)
		{
			CLR_BIT(MCUCR,2);
			CLR_BIT(MCUCR,3);
		}
		else if(Copy_u8SenseControl==MEXTI_ANY_LOGICAL_CHANGE)
		{
			SET_BIT(MCUCR,2);
			CLR_BIT(MCUCR,3);
		}
		else if(Copy_u8SenseControl==MEXTI_FALLING_EDGE)
		{
			CLR_BIT(MCUCR,2);
			SET_BIT(MCUCR,3);
		}
		else if(Copy_u8SenseControl==MEXTI_RISING_EDGE)
		{
			SET_BIT(MCUCR,2);
			SET_BIT(MCUCR,3);
		}
		else
		{
			/*Do Nothing*/
		}
		break;
	case EXTI2:
		if(Copy_u8SenseControl==MEXTI_FALLING_EDGE)
		{
			CLR_BIT(MCUCSR,6);
		}
		else if(Copy_u8SenseControl==MEXTI_RISING_EDGE)
		{
			SET_BIT(MCUCSR,6);
		}
		else
		{
			/*Do Nothing*/
		}
		break;

	default: break;

		//Do NoThing
	}
}
