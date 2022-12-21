#include "operations.h"
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



int16 SW_sec = 0;
int16 SW_min = 0;

int16 TM_sec = 0;
int16 TM_min = 0;

void Led_Toggel(int16 led){
  if(led == 0)
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, ~GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1));
  else
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, ~GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2));
}

void handler(void){
  MAP_TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
  if((SW_sec+1)%60 == 0 ){
    displayTime(SW_min, SW_sec);
    SW_min = (SW_min+1)%60;
    SW_sec = 0;
  }
  displayTime(SW_min, SW_sec);
  SW_sec = (SW_sec+1)%60;
}

void handler2(void){
  LCD_command(CLEAR_DISPLAY);
  MAP_TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
  if(TM_min == 0 && TM_sec == 0){
    Led_Toggel(0);
    displayTime(TM_min, TM_sec);
    LCD_setcursorRowCol(1,3);
    LCD_printString("Messi keseb");
  }
  else{
  if((TM_sec)%60 == 0){
    displayTime(TM_min, TM_sec);
    TM_min = (TM_min-1)%60;
    TM_sec = 60;
  }
  TM_sec = (TM_sec-1)%60;
  displayTime(TM_min, TM_sec);
  }
}





void Delay(unsigned int delay)
{
	volatile unsigned int i, counter;
	counter = delay * 3180; 
	for(i=0;i<counter;i++);
}





void periodic_SW(int32_t delay){
   
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));
  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1);
  

  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
  TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
  TimerLoadSet(TIMER0_BASE , TIMER_A, (delay*16e3) -1);
  TimerEnable(TIMER0_BASE , TIMER_A);
  TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
  TimerIntRegister(TIMER0_BASE, TIMER_A, handler);
  IntMasterEnable();
   __asm("CPSIE I");
}

void periodic_timer(int32_t delay){
   
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));
  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1);
  

  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
  TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
  TimerLoadSet(TIMER1_BASE , TIMER_A, (delay*16e3) -1);
  TimerEnable(TIMER1_BASE , TIMER_A);
  TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
  TimerIntRegister(TIMER1_BASE, TIMER_A, handler2);
  IntMasterEnable();
   __asm("CPSIE I");
}


int main(void)
{
  while(1)
  {  calc();
      get_input();
  }
    
    
   
}

