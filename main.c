#include "Free_RTOS/FreeRTOS.h"
#include "Free_RTOS/task.h"
#include "Free_RTOS/semphr.h"
#include "MCAL/MGIE/MGIE_interface.h"
#include "MCAL/MEXTI/MEXTI_interface.h"
#include "HAL/LCD.h"
#include "MCAL/MADC/MADC_Interface.h"
#include "MCAL/TIMER/TIMER_Interface.h"
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#include "LIB/LSTD_TYPEs.h"
#include "LIB/LBIT_MATH.h"

u8 EDIT_flag = 0U;
u8 Global_MOOD = 0U;

/***************Glopal Variables****************/
struct clock {
	u8 SEC;
	u8 MIN;
	u8 HOUR;
	char Mode[3];
	u8 DAY;
	u8 MONTH;
	long YEAR;
} CLOCK;

struct clock *P = &CLOCK;

/********** functions************/
void LCD_UPDATE_SEC(u8 s);
void LCD_UPDATE_MIN(u8 m);
void LCD_UPDATE_HOUR(u8 h);
void LCD_UPDATE_MODE(char* M);
void LCD_UPDATE_MONTH(u8 M);
void LCD_UPDATE_YEAR(u16 Y);
void LCD_UPDATE_DAY(u8 D);
void LCD_UPATE_CLOCK(u8 HOUR, u8 MIN, u8 SEC, char* Mode, u8 DAY, u8 MONTH,
		u16 YEAR);
void clock_EDITp(void);
void SELECT(void);
void ClockEditMode(void);
void CLK_SENSOR_SELECT(void);

/********* System Task ***********/
void Task_vDisplaytime(void* pv);
void Task_vDisplaySensors(void* pv);

int main(void) {

	/* interfaces                       */
	LCD_INIT();
	MADC_VidInit();
	//TIMER0_VidInit();

	/*External interrupt intialization*/
	CLR_BIT(DDRD, 2);
	CLR_BIT(DDRD, 3);
	CLR_BIT(DDRB, 2);
	SET_BIT(PORTD, 2);
	SET_BIT(PORTD, 3);
	SET_BIT(PORTB, 2);
	MEXTI0_VidInit();
	MEXTI1_VidInit();
	MEXTI2_VidInit();

	MEXTI_VidSenseControl( MEXTI_LOW_LEVEL, EXTI0);
	MEXTI_VidSenseControl( MEXTI_LOW_LEVEL, EXTI1);
	MEXTI_VidSenseControl( MEXTI_LOW_LEVEL, EXTI2);

	EXTI0_Set_CallBackFunction(ClockEditMode);
	EXTI1_Set_CallBackFunction(CLK_SENSOR_SELECT);
	EXTI2_Set_CallBackFunction(SELECT);

	MGIE_VidEnable();
	// clock initializations 
	P->HOUR = 11U;
	P->MIN = 59U;
	P->SEC = 50U;
	P->Mode[0] = 'A';
	P->Mode[1] = 'M';
	P->Mode[2] = '\0';

	P->DAY = 26U;
	P->MONTH = 3U;
	P->YEAR = 2022U;

	xTaskCreate(Task_vDisplaytime, NULL, 300, NULL, 1, NULL);
	xTaskCreate(Task_vDisplaySensors, NULL, 150, NULL, 0, NULL);

	vTaskStartScheduler();

	while (1) {

	}

}

/*  private APIS */
void LCD_UPDATE_SEC(u8 s) {

	LCD_GOTO_XY(1, 6);
	LCD_SEND_INT(s);

}

void LCD_UPDATE_MIN(u8 m) {

	LCD_GOTO_XY(1, 3);
	LCD_SEND_INT(m);

}

void LCD_UPDATE_HOUR(u8 h) {

	LCD_GOTO_XY(1, 0);
	LCD_SEND_INT(h);

}

void LCD_UPDATE_MODE(char * M) {
	LCD_GOTO_XY(1, 9);
	LCD_SEND_A_STRING(M);

}

void LCD_UPDATE_DAY(u8 D) {
	LCD_GOTO_XY(0, 0);
	LCD_SEND_INT(D);
}

void LCD_UPDATE_MONTH(u8 M) {
	LCD_GOTO_XY(0, 3);
	LCD_SEND_INT(M);
}
void LCD_UPDATE_YEAR(u16 Y) {

	LCD_GOTO_XY(0, 6);
	LCD_SEND_INT4(Y);

}

/* function to dispaly */
void LCD_UPATE_CLOCK(u8 HOUR, u8 MIN, u8 SEC, char* Mode, u8 DAY, u8 MONTH,
		u16 YEAR) {

	LCD_UPDATE_DAY(DAY);
	LCD_GOTO_XY(0U, 2U);
	LCD_SEND_A_STRING("/");
	LCD_UPDATE_MONTH(MONTH);
	LCD_GOTO_XY(0U, 5U);
	LCD_SEND_A_STRING("/");
	LCD_UPDATE_YEAR(YEAR);

	// TIME
	LCD_UPDATE_HOUR(HOUR);
	LCD_GOTO_XY(1U, 2U);
	LCD_SEND_A_STRING(":");
	LCD_UPDATE_MIN(MIN);
	LCD_GOTO_XY(1U, 5U);
	LCD_SEND_A_STRING(":");
	LCD_UPDATE_SEC(SEC);
	LCD_UPDATE_MODE(Mode);
}

/* Function that reads the sensors values and display them on the LCD*/
void Task_vDisplaySensors(void* pv) {

	u8 i;
	u16 Loc_u16Digital_Temp;
	u16 Loc_u16Digital_BBM;
	u16 Loc_u16BBM = 0;
	u16 Loc_u16TimeValue[10] = { 0 };
	u16 Analog;
	u16 Average = 0;
	u16 Loc_u8Temperature;
	u16 Loc_u16Threshold = 550; // Determine which Signal to "count as a beat" and which to ignore.
	static u8 Ticks = 0;
	static u8 FirstTimeFlag = 0;
	static u8_Time = 0;
	while (1) {
		if (Global_MOOD == 1) {
			u8_Time++;
			LCD_CLEAR();
			LCD_GOTO_XY(0, 0);
			LCD_SEND_A_STRING("H_Rate=");
			LCD_GOTO_XY(0, 9);
			LCD_SEND_A_STRING("BBM");
			LCD_GOTO_XY(1, 0);
			LCD_SEND_A_STRING("TEMP=");
			LCD_GOTO_XY(1, 9);
			LCD_SEND_A_STRING("C");

			Loc_u16Digital_Temp = MADC_u16ADC_StartConversion(CHANNEL_0);
			Analog = ((u32) (Loc_u16Digital_Temp * 5000UL)) / 1024;
			Loc_u8Temperature = Analog / 10;

			LCD_GOTO_XY(1, 5);
			LCD_SEND_INT(Loc_u8Temperature);

			Loc_u16Digital_BBM = MADC_u16ADC_StartConversion(CHANNEL_1);

			if (Loc_u16Digital_BBM > Loc_u16Threshold) {
				if (FirstTimeFlag == 0) {
					//TIMER0_VidSetPreload(0);
					FirstTimeFlag = 1;
				} else {
					//Loc_u16TimeValue[Ticks] = TCNT0;
					//TIMER0_VidSetPreload(0);
					Ticks++;
				}
				if (Ticks == 10) {
					for (i = 0; i < 10; i++) {
						Average += Loc_u16TimeValue[i];
					}
					Loc_u16BBM = Average / 10;
				}
			}

			LCD_GOTO_XY(0, 8);
			LCD_SEND_INT(Loc_u16BBM);

		}
		if (u8_Time == 10) {
			(P->SEC)++;
			clock_EDITp();
			u8_Time = 0;
		}
		vTaskDelay(100);
	}
}

/* function to handle the increasing cases */
void clock_EDITp(void) {

	if ((P->SEC) == 60U) {
		(P->SEC) = 1U;
		(P->MIN)++;
	} else {
	}

	if (P->MIN == 60U) {
		(P->MIN) = 1U;
		(P->HOUR)++;
		if (P->HOUR == 12U && P->Mode[0] == 'P' && P->Mode[1] == 'M') {
			P->Mode[0] = 'A';
			P->Mode[1] = 'M';
		} else if (P->HOUR == 12U && P->Mode[0] == 'A' && P->Mode[1] == 'M') {
			P->Mode[0] = 'P';
			P->Mode[1] = 'M';

		} else {
		}
	} else {
	}

	if (P->HOUR == 13U) {
		P->HOUR = 1U;
	} else {
	}

	if (P->HOUR == 12U && P->Mode[0] == 'A' && P->Mode[1] == 'M' && P->MIN == 1
			&& P->SEC == 1) {

		P->DAY++;

	} else {
	}

	if (P->DAY == 31) {
		P->MONTH++;
	} else {
	}

	if (P->MONTH == 13) {
		P->YEAR++;
	}

}

/*DISPLAY TASK */
void Task_vDisplaytime(void* pv) {

	while (1) {
		if (Global_MOOD == 0) {
			if (EDIT_flag == 0U) {

				(P->SEC)++;
				clock_EDITp();
				LCD_UPATE_CLOCK(P->HOUR, P->MIN, P->SEC, P->Mode, P->DAY,
						P->MONTH, P->YEAR);

			}
			// Edit hours
			else if (EDIT_flag >= 1U) {

				if (EDIT_flag == 1U) {
					LCD_ENABLE_CURSOR();
					LCD_SET_CURSOR(1U, 1U);
					LCD_DISABLE_CURSOR();
				}
				//Edit MIN
				else if (EDIT_flag == 2U) {

					LCD_ENABLE_CURSOR();
					LCD_SET_CURSOR(4U, 1U);
					LCD_DISABLE_CURSOR();

				}

				//Edit sec
				else if (EDIT_flag == 3U) {

					LCD_ENABLE_CURSOR();
					LCD_SET_CURSOR(7U, 1U);

					LCD_DISABLE_CURSOR();

				}

				else if (EDIT_flag == 4U) {

					LCD_ENABLE_CURSOR();
					LCD_SET_CURSOR(1U, 0U);
					LCD_DISABLE_CURSOR();
				}

				else if (EDIT_flag == 5U) {

					LCD_ENABLE_CURSOR();
					LCD_SET_CURSOR(4U, 0U);
					LCD_DISABLE_CURSOR();

				} else if (EDIT_flag == 6U) {

					LCD_ENABLE_CURSOR();
					LCD_SET_CURSOR(9U, 0U);
					LCD_DISABLE_CURSOR();

				}

				else {
				}
				LCD_UPATE_CLOCK(P->HOUR, P->MIN, P->SEC, P->Mode, P->DAY,
						P->MONTH, P->YEAR);
			} else {
			}
		}
		vTaskDelay(1000);
	}
}

/*Function to pass to callback of EXTI2 to select different mode */
void SELECT(void) {
	if (Global_MOOD == 0) {
		EDIT_flag++;
		if (EDIT_flag == 7U) {
			EDIT_flag = 0U;
		}
	}
}

void CLK_SENSOR_SELECT(void) {
	if (Global_MOOD == 0) {
		Global_MOOD = 1;
	} else {
		Global_MOOD = 0;
	}
}

/* Function to pass to callback of EXTI0 to update time debending on Edit_flag*/
void ClockEditMode(void) {
	if (Global_MOOD == 0) {
		if (EDIT_flag == 1U) {
			P->HOUR++;
			if (P->HOUR == 12U && P->Mode[0] == 'P' && P->Mode[1] == 'M') {
				P->Mode[0] = 'A';
				P->Mode[1] = 'M';
				P->DAY++;
			} else if (P->HOUR == 12U && P->Mode[0] == 'A'
					&& P->Mode[1] == 'M') {
				P->Mode[0] = 'P';
				P->Mode[1] = 'M';
			} else {
			}

			if (P->HOUR == 13)
				P->HOUR = 1;
			else {
			}

		} else if (EDIT_flag == 2U) {
			P->MIN++;
			clock_EDITp();
		} else if (EDIT_flag == 3U) {
			P->SEC++;
			clock_EDITp();
		} else if (EDIT_flag == 4U) {
			P->DAY++;
			clock_EDITp();
		} else if (EDIT_flag == 5U) {
			P->MONTH++;
			clock_EDITp();
		} else if (EDIT_flag == 6U) {
			P->YEAR++;
			clock_EDITp();
		} else {
		}
	}
}
