#include <avr/io.h>
#include <util/delay.h>
#include "LCD.h"





void LCD_SEND_A_COMMAND (char command){

	LCD_CHECK_LCD_ISBUSY();

	LCD_control &= ~(1<<RS_switch);
	LCD_control &= ~(1<<ReadWrite);

	LCD_port = command ;
	LCD_KICK();
}





void LCD_KICK(void){
 DataDir_LCD_control |= (1<<EN_switch);
 LCD_control |= (1<<EN_switch) ;
 asm ("NOP");
 asm ("NOP");
 _delay_ms(10);
 LCD_control &= ~(1<<EN_switch);
}


 void LCD_CLEAR(void){

	 LCD_SEND_A_COMMAND(LCD_CLEAR_SCREEN);
 }



void LCD_INIT(void){

	_delay_ms(10);
	DataDir_LCD_control |= (1<<EN_switch | 1<<ReadWrite | 1<<RS_switch) ;
	DataDir_LCD_port = 0xff;
	LCD_control &= ~(1<<EN_switch | 1<<ReadWrite | 1<<RS_switch) ;
	_delay_ms(20);
	 LCD_CLEAR();
	#ifdef EIGHT_BIT_MODE
	LCD_SEND_A_COMMAND(LCD_FUNCTION_8BIT_2LINES);
    #endif



	LCD_SEND_A_COMMAND(LCD_ENTRY_MODE);
	LCD_SEND_A_COMMAND(LCD_BEGIN_AT_FIRST_RAW);
    LCD_SEND_A_COMMAND(LCD_DISP_ON  );
}



void LCD_SEND_A_CHAR (unsigned char character){

	#ifdef EIGHT_BIT_MODE
	LCD_CHECK_LCD_ISBUSY();
	LCD_control |= (1<<RS_switch);
	LCD_port = character;
	LCD_KICK();
    #endif
}



void LCD_CHECK_LCD_ISBUSY(){
	DataDir_LCD_port = 0;
	DataDir_LCD_control |= (1<<RS_switch | 1 <<EN_switch | 1<<ReadWrite);
	LCD_control &= ~(1<<RS_switch);
	LCD_control |= (1<<ReadWrite);
	LCD_KICK();
	DataDir_LCD_port = 0xff;
	LCD_control &= ~(1<<ReadWrite);
}


void LCD_GOTO_XY(unsigned char line , unsigned char position ) {
	if (line==0)
	{
		LCD_SEND_A_COMMAND(0x80+position);
	}
	if (line==1)
	{
		LCD_SEND_A_COMMAND(0xc0+position);
	}

}



void LCD_SEND_A_STRING(char* string) {

	int i=0 ;
	int count = 0 ;
	while (string[i] != 00 )
	{
		LCD_SEND_A_CHAR(string[i]);
		i++;
		count++;
		if (count==16 )
		{
			LCD_GOTO_XY(1,0);

		}
		if (count==32)
		{
			LCD_CLEAR();
			LCD_GOTO_XY(0,0);
			count = 0;

		}
	}



}


void LCD_SEND_INT (unsigned char number){

	char str[7];
	sprintf(str, "%02d" ,number);
	LCD_SEND_A_STRING(str);

}

void LCD_SEND_INT4 (unsigned int number){

	char str[7];
	sprintf(str, "%04d" ,number);
	LCD_SEND_A_STRING(str);

}


void LCD_SEND_REALNUM (double RealNum ){

	char str[16];
	char tmpsign = (RealNum<0) ?'-':' ';
	float tmpval =  (RealNum<0)? -RealNum : RealNum;
	int intNum = tmpval ;
	float tmpfrac = tmpval - intNum ;
	int frac = tmpfrac * 100000 ;
	sprintf(str, "%c%d.%05d", tmpsign,intNum,frac);

	LCD_SEND_A_STRING(str);

}


void LCD_ENABLE_CURSOR(void){

	LCD_SEND_A_COMMAND(0x0E);

}


void LCD_DISABLE_CURSOR(void){

	LCD_SEND_A_COMMAND(0x0C );

}



void LCD_SET_CURSOR(unsigned char col, unsigned char row) {
	static unsigned char offsets[] = { 0x00, 0x40, 0x14, 0x54 };
	if (row > 3) {
		row = 3;
	}

	LCD_SEND_A_COMMAND(0x80 | (col + offsets[row]));
}





