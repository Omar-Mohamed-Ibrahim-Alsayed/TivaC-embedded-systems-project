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
#include "tm4c123gh6pm.h"
#include "LCD.h"
#include "KeyPad.h"

//---------------------------------------------------Global variables---------------------------------------------------
int16 SW_sec = 0;
int16 SW_min = 0;


//------------------------------------------------------Functions--------------------------------------------------------

//reset timer
void SWreset()
{
    SW_sec = 0;
    SW_min = 0;
}

//interrupt handler for each second
void handler(void)
{
    //clear interrupt
    MAP_TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    //Check if state is not pause and not reset else don't count
    if(get_pause()==false && get_reset() == false) {
        //if the seconds reached 60 increment the minuts
        if((SW_sec+1)%60 == 0 ) {
            if(get_mode(0) == 0) {
                LCD_command(CLEAR_DISPLAY);
                displayTime(SW_min, SW_sec);
            }
            SW_min = (SW_min+1)%60;
            SW_sec = 0;
        } else {
            //if the seconds did not reach 60 increment the second
            if(get_mode(0) == 0) {
                LCD_command(CLEAR_DISPLAY);
                displayTime(SW_min, SW_sec);
            }
            SW_sec = (SW_sec+1)%60;
        }
    }
    
  //if reset then reset seconds and mins
  else if(get_reset() == true){
    SW_sec =0;
    SW_min =0;
    set_reset();
    LCD_command(CLEAR_DISPLAY);
      displayTime(SW_min, SW_sec);
  }
}

//initialization of timer 0 periodic with interrupts
void periodic_SW(int32_t delay)
{

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER0_BASE , TIMER_A, (delay*16e3) -1);
    TimerEnable(TIMER0_BASE , TIMER_A);
    IntPrioritySet(INT_TIMER0A, 0x03);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerIntRegister(TIMER0_BASE, TIMER_A, handler);
    IntMasterEnable();
    __asm("CPSIE I");
}

//main stop watch function
void stopwatch()
{
    reset_mode(0);
    LCD_command(CLEAR_DISPLAY);
    displayTime(SW_min, SW_sec);
    periodic_SW(1000);
    while(get_mode(0) == 0) {
        __asm("wfi\n");
    }
}