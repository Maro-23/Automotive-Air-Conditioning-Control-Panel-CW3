#include <avr/io.h>			  /* Include AVR std. library file */
#include <util/delay.h>	  /* Include Delay header file */
#include "LCD.h"
#if !defined(__AVR_ATmega328P__)
#include <avr/iom328p.h>
#endif

#define LCD_Dir  DDRD			/* Define LCD data port direction */
#define LCD_Port PORTD		/* Define LCD data port */

#define RS_EN_Dir  DDRB		/* Define RS and En data port direction */
#define RS_EN_Port PORTB  /* Define RS and En port */
#define RS PB0				    /* Define Register Select pin */
#define EN PB1 				    /* Define Enable signal pin */

void LCD_Command( unsigned char cmnd )
{
	LCD_Port = (LCD_Port & 0x0F) | (cmnd & 0xF0); /* sending upper nibble */
	RS_EN_Port &= ~ (1<<RS);		/* RS=0, command reg. */
	RS_EN_Port |= (1<<EN);		/* Enable pulse */
	_delay_us(1);
	RS_EN_Port &= ~ (1<<EN);

	_delay_us(200);

	LCD_Port = (LCD_Port & 0x0F) | (cmnd << 4);  /* sending lower nibble */
	RS_EN_Port |= (1<<EN);
	_delay_us(1);
	RS_EN_Port &= ~ (1<<EN);
	_delay_ms(2);
}


void LCD_Char( unsigned char data )
{
	LCD_Port = (LCD_Port & 0x0F) | (data & 0xF0); /* sending upper nibble */
	RS_EN_Port |= (1<<RS);		/* RS=1, data reg. */
	RS_EN_Port|= (1<<EN);
	_delay_us(1);
	RS_EN_Port &= ~ (1<<EN);

	_delay_us(200);

	LCD_Port = (LCD_Port & 0x0F) | (data << 4); /* sending lower nibble */
	RS_EN_Port |= (1<<EN);
	_delay_us(1);
	RS_EN_Port &= ~ (1<<EN);
	_delay_ms(2);
}

void LCD_Init (void)			/* LCD Initialize function */
{
	LCD_Dir = 0xFF;			    /* Make LCD port direction as o/p */
  	RS_EN_Dir |= (1 << EN) | (1 << RS);

	_delay_ms(20);			    /* LCD Power ON delay always >15ms */
	
	LCD_Command(0x02);		  /* send for 4 bit initialization of LCD  */
	LCD_Command(0x28);      /* 2 line, 5*7 matrix in 4-bit mode */
	LCD_Command(0x0c);      /* Display on cursor off*/
	LCD_Command(0x06);      /* Increment cursor (shift cursor to right)*/
	LCD_Command(0x01);      /* Clear display screen*/
	_delay_ms(2);
}

void LCD_String (char *str)		/* Send string to LCD function */
{
	int i;
	for(i=0;str[i]!=0;i++)		/* Send each char of string till the NULL */
	{
		LCD_Char (str[i]);
	}
}

void LCD_Clear()
{
	LCD_Command (0x01);		/* Clear display */
	_delay_ms(2);
	LCD_Command (0x80);		/* Cursor at home position */
}

// A utility function to reverse a string
void reverse(char str[], int length)
{
  int start = 0;
  int end = length - 1;
  while (start < end) {
      char temp = str[start];
      str[start] = str[end];
      str[end] = temp;
      end--;
      start++;
  }
}

char* IntToStr(int num, char* str)
{
    int count = 0;

    /* Handle 0 explicitly, otherwise empty string is
     * printed for 0 */
    if (num == 0) {
        str[count++] = '0';
        str[count] = '\0';
        return str;
    }
 
    // Process individual digits
    while (num != 0) {
        int remainder = num % 10;
        str[count++] = remainder + '0';
        num = num / 10;
    }
 
    str[count] = '\0'; // Append string terminator
 
    // Reverse the string
    reverse(str, count);
 
    return str;
}