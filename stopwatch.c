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
#include "tm4c123gh6pm.h"
#include "LCD.h"
#include "KeyPad.h"
//#include "config.h"

int16 SW_sec = 0;
int16 SW_min = 0;



void SWreset(){
  SW_sec = 0;
  SW_min = 0;
}

void handler(void){
  MAP_TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
  if(get_pause()==false){
  if((SW_sec+1)%60 == 0 ){
   if(get_mode(0) == 0){
    LCD_command(CLEAR_DISPLAY);
    displayTime(SW_min, SW_sec);}
    SW_min = (SW_min+1)%60;
    SW_sec = 0;
  }else{
     if(get_mode(0) == 0){
  LCD_command(CLEAR_DISPLAY);
      displayTime(SW_min, SW_sec);}
    SW_sec = (SW_sec+1)%60;
  }}
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

void stopwatch(){\
  reset_mode(0);
  LCD_command(CLEAR_DISPLAY);
  //SWreset();
  displayTime(SW_min, SW_sec);
  periodic_SW(1000);
    while(get_mode(0) == 0){
     __asm("wfi\n");
  }
}
