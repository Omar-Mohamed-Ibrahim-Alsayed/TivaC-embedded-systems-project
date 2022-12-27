#include "LCD.h"

//using commands from dmc1610a.h
void LCD_command(unsigned int command)
{
    GPIO_PORTA_DATA_R = 0x20;               //set RS to 1 to enable Command Register and RW to low to write to the LCD
    GPIO_PORTB_DATA_R = command;
    GPIO_PORTA_DATA_R = 0x80;
    delay_us(1);
    GPIO_PORTA_DATA_R = 0x00;                  //High to Low pulse to push the command to the LCD
    if (command < 4)
        delay_ms(2);                          // command 1 and 2 needs up to 1.64ms
    else
        delay_us(40);                         // all others 40 us
}

//print data on LCD
void LCD_data(unsigned char data)
{
    GPIO_PORTA_DATA_R = 0x20;               //set RS to 1 to enable Data Register and RW to low to write to the LCD
    GPIO_PORTB_DATA_R = data;
    GPIO_PORTA_DATA_R = 0x80 | 0x20;
    delay_us(1);
    GPIO_PORTA_DATA_R = 0;                  //High to Low pulse to push the data to the LCD
    delay_us(40);
}

//print int on lcd
void LCD_printInt(int no)
{
    char toprint[4] = {0};
    sprintf(toprint, "%d", no);
    int i = 0;
    while(toprint[i] != '\0') {
        LCD_data(toprint[i]);
        i++;
    }
}

//print float on lcd
void LCD_printFloat(float no)
{
    int len = snprintf('\0', 0, "%f", no);
    char* toprint = (char *)malloc(len + 1);
    snprintf(toprint, len + 1, "%f", no);
    int j = 0;
    while (j < len - 3 ) {
        LCD_data(toprint[j]);
        j++;
    }
    free(toprint);
}

//print string on lcd
void LCD_printString(char* str)
{
    LCD_command(DISPLAY_ON_CURSOR_OFF);
    int i = 0;
    while (str[i] != '\0') {
        LCD_data(str[i]);
        i++;
    }
}

//print char on lcd
void LCD_printChar(char str)
{
    LCD_command(DISPLAY_ON_CURSOR_OFF);
    LCD_data(str);
}

//print error message
void LCD_errormsg(void)
{
    LCD_command(CLEAR_DISPLAY);
    LCD_printString("ERROR");
}

//move cursor
void LCD_setcursorRowCol(unsigned int row, unsigned int col)
{
    int command = 0x00;
    if( row == 0) {
        command = 0x80;
        while(col <=  0x0F && col >= 0x00) {
            LCD_command(command + col);
            break;
        }
    }
    if( row == 1) {
        command = 0xC0;
        while(col <=  0x0F && col >= 0x00) {
            LCD_command(command + col);
            break;
        }
    }
}

//print time for stopwatch and timer
void displayTime(int m,int s)
{
    LCD_setcursorRowCol(0, 5);
    LCD_printInt(m);
    LCD_setcursorRowCol(0, 7);
    LCD_data(':');
    LCD_setcursorRowCol(0, 8);
    LCD_printInt(s);

}