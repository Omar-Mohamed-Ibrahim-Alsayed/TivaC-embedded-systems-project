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

//---------------------------------------------------Global variables---------------------------------------------------
int16 TM_sec = 0;
int16 TM_min = 0;
int8 inp;
int16 buzz = 1;
int16 first_time = 0;


//------------------------------------------------------Functions--------------------------------------------------------
void TMset();

//interrupt handler for each second
void handler2(void)
{
    //clear interrupt and display
    MAP_TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    
    //check if not paused else don't count
    if(get_pause()==false) {
      
        //if we reach the end print done and fire the buzzer
        if(TM_min == 0 && TM_sec == 0) {
            if(get_mode(1) == 0) {
                LCD_command(CLEAR_DISPLAY);
                displayTime(TM_min, TM_sec);
            } 
            LCD_setcursorRowCol(1,5);
            LCD_printString("Done");
            DIO_WritePin(PORTD,7,buzz);
            if(buzz == 1) {
                buzz = 0;
            } else
                buzz = 1;
            delay_ms(1000);
        }
        
        //else decrement seconds if it did not reach 0 or decrement mins
        else {
            if((TM_sec)%60 == 0) {
                if(get_mode(1) == 0) {
                    displayTime(TM_min, TM_sec);
                }
                TM_min = (TM_min-1)%60;
                TM_sec = 60;
            }
            TM_sec = (TM_sec-1)%60;
            if(get_mode(1) == 0) {
                displayTime(TM_min, TM_sec);
            }
        }
    } 
    if(get_mode(1) == 0) {      
    LCD_command(CLEAR_DISPLAY);
    displayTime(TM_min, TM_sec);
            }
}

//initialization of timer 1 periodic with interrupts
void periodic_timer(int32_t delay)
{

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1);


    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
    TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER1_BASE , TIMER_A, (delay*16e3) -1);
    TimerEnable(TIMER1_BASE , TIMER_A);
    IntPrioritySet(INT_TIMER1A, 0x04);
    TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    TimerIntRegister(TIMER1_BASE, TIMER_A, handler2);
    IntMasterEnable();
    __asm("CPSIE I");
}

//reset timer
void TMreset()
{
    TM_sec = 0;
    TM_min = 0;
}

//get time from user
void TMset()
{
    TMreset();
    //get minutes
    inp = get_input();
    while(((int)(inp) - 48)>=6) {
        inp = get_input();
    }
    TM_min += 10*((double)(inp) - 48);
    LCD_command(CLEAR_DISPLAY);
    displayTime(TM_min,TM_sec);
    inp = get_input();
    while(((double)(inp) - 48)>=6) {
        inp = get_input();
    }
    TM_min += ((double)(inp) - 48);
    LCD_command(CLEAR_DISPLAY);
    displayTime(TM_min,TM_sec);
    //get seconds
    inp = get_input();
    while(((double)(inp) - 48)>=6) {
        inp = get_input();
    }
    TM_sec += 10*((double)(inp) - 48);
    LCD_command(CLEAR_DISPLAY);
    displayTime(TM_min,TM_sec);
    inp = get_input();
    while(((double)(inp) - 48)>=6) {
        inp = get_input();
    }
    TM_sec += ((double)(inp) - 48);
    LCD_command(CLEAR_DISPLAY);
    displayTime(TM_min,TM_sec);

}

//main function for timer
void timer()
{
  if( first_time == 0 ){
  LCD_command(CLEAR_DISPLAY);
    LCD_printString("Please enter time");
    TMset();
    LCD_setcursorRowCol(1,0);
    LCD_printString("Press any key to start");
    get_input();
    periodic_timer(1000);
  }
    reset_mode(1);
    first_time = 1;
    while(get_mode(1) == 0) {
        __asm("wfi\n");
    }
    DIO_WritePin(PORTD,7,0);
}