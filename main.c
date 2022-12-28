#include "DataTypes.h"
#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"
#include "inc/hw_gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/timer.h"
#include "DIO.h"
#include "LCD.h"
#include "KeyPad.h"
#include "calculator.h"
#include "stopwatch.h"
#include "timer.h"
#include "config.h"

//---------------------------------------------------Global variables---------------------------------------------------

//the chosen mode (1.calculator         2.stopwatch             3.timer)
int8 modeMain = '1';

//-----------------------------------------------------Main method---------------------------------------------------

void main()
{
    //initiate LCD,timer,push buttons
    LCD_Init();
    keypad_Init();
    GPIOF_init();

    //print welcoming screen
    LCD_command(CLEAR_DISPLAY);
    LCD_setcursorRowCol(0,0);
    LCD_printString("Tiva C Project");

    LCD_setcursorRowCol(1,3);
    LCD_printString("Team 10");
    delay_ms(3000);

    while(1) {
      
        //enter menu mode
        LCD_command(CLEAR_DISPLAY);
        LCD_setcursorRowCol(0,0);
        LCD_printString("Please Select");
        LCD_setcursorRowCol(1,5);
        LCD_printString("mode");
        modeMain = get_input();
        while(((int)(modeMain)-48)  !=1 &&((int)(modeMain)-48)  !=2 && ((int)(modeMain)-48)  !=3 ) {
            modeMain = get_input();
        }
        LCD_command(CLEAR_DISPLAY);
        switch(modeMain) {
        case('1'): {
            calc();
            break;
        }
        case('2'): {
            stopwatch();
            break;
        }
        case('3'): {
            timer();
            break;
        }
        }
    }
}