#ifndef LCD1_H_
#define LCD1_H_

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>

#define LCD_port PORTC
#define DataDir_LCD_port DDRC
#define DATA_shift 0    //0:8bit data mode | 4:4bit data mode
#define LCD_control PORTD
#define DataDir_LCD_control DDRD
#define EN_switch 4
#define ReadWrite 1
#define RS_switch 0

#define	EIGHT_BIT_MODE
//#define FOUR_BIT_MODE



#define LCD_REGISTER_SELECT_PIN						(0)
#define LCD_READ_WRITE_PIN							(1)
#define LCD_ENABLE_PIN								(2)
#define LCD_REGISTER_SELECT_ENABLE					(1)
#define LCD_REGISTER_SELECT_DISABLE					(0)
#define READ_FROM_LCD								(1)
#define WRITE_TO_LCD								(0)
#define LCD_ENABLE									(1)
#define LCD_DISABLE									(0)
#define LCD_FIRST_LINE								(0)
#define LCD_SECOND_LINE								(1)
#define LCD_FUNCTION_8BIT_2LINES   					(0x38)
#define LCD_FUNCTION_4BIT_2LINES   					(0x28)
#define LCD_MOVE_DISP_RIGHT       					(0x1C)
#define LCD_MOVE_DISP_LEFT   						(0x18)
#define LCD_MOVE_CURSOR_RIGHT   					(0x14)
#define LCD_MOVE_CURSOR_LEFT 	  					(0x10)
#define LCD_DISP_OFF   								(0x08)
#define LCD_DISP_ON_CURSOR   						(0x0E)
#define LCD_DISP_ON_CURSOR_BLINK   					(0x0F)
#define LCD_DISP_ON_BLINK   						(0x0D)
#define LCD_DISP_ON   								(0x0C)
#define LCD_ENTRY_DEC   							(0x04)
#define LCD_ENTRY_DEC_SHIFT   						(0x05)
#define LCD_ENTRY_INC_   							(0x06)
#define LCD_ENTRY_INC_SHIFT   						(0x07)
#define LCD_BEGIN_AT_FIRST_RAW						(0x80)
#define LCD_BEGIN_AT_SECOND_RAW						(0xC0)
#define LCD_CLEAR_SCREEN							(0x01)
#define LCD_ENTRY_MODE								(0x06)







void LCD_INIT(void);
void LCD_CLEAR(void);
void LCD_SEND_A_COMMAND(char command);
void LCD_CHECK_LCD_ISBUSY();
void LCD_KICK();
void LCD_GOTO_XY(unsigned char line , unsigned char position);
void LCD_SEND_A_CHAR (unsigned char character);
void LCD_SEND_A_STRING (char *stringofchar);
void LCD_SEND_INT (unsigned char number);
void LCD_SEND_REALNUM (double RealNum );
void LCD_ENABLE_CURSOR(void);
void LCD_SET_CURSOR(unsigned char col, unsigned char row);
void LCD_DISABLE_CURSOR(void);
void LCD_SEND_INT4 (unsigned int number);


#endif /* LCD_H_ */
