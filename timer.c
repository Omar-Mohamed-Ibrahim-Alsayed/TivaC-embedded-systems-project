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
//#include "config.h"


int16 TM_sec = 0;
int16 TM_min = 0;
char inp;
void TMset();


void handler2(void){
  LCD_command(CLEAR_DISPLAY);
  MAP_TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
  LCD_command(CLEAR_DISPLAY);
  LCD_printInt(get_pause());
  LCD_command(CLEAR_DISPLAY);
  LCD_printInt(get_reset());
  if(get_pause()==false && get_reset() == false){
  if(TM_min == 0 && TM_sec == 0){
    if(get_mode(1) == 0){     
    LCD_command(CLEAR_DISPLAY);
    displayTime(TM_min, TM_sec);}
    LCD_setcursorRowCol(1,5);
    LCD_printString("Done");
  }
  else{
  if((TM_sec)%60 == 0){
    if(get_mode(1) == 0){
    displayTime(TM_min, TM_sec);}
    TM_min = (TM_min-1)%60;
    TM_sec = 60;
  }
  TM_sec = (TM_sec-1)%60;
  if(get_mode(1) == 0){
    displayTime(TM_min, TM_sec);}
  }}
  else if(get_reset() == true){
  set_reset();
  LCD_command(CLEAR_DISPLAY);
  LCD_printString("Please enter time");
  TMset();
  LCD_setcursorRowCol(1,0);
  LCD_printString("Press any key to start");
  get_input();
  }
    LCD_command(CLEAR_DISPLAY);
    displayTime(TM_min, TM_sec);
}





void periodic_timer(int32_t delay){
   
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


void TMreset(){
  TM_sec = 0;
  TM_min = 0;
}

void TMset(){
  TMreset();
  //get minutes
  inp = get_input();
  while(((int)(inp) - 48)>=6){
  inp = get_input();
  }
  TM_min += 10*((double)(inp) - 48);
  LCD_command(CLEAR_DISPLAY);
  displayTime(TM_min,TM_sec);
  inp = get_input();
  while(((double)(inp) - 48)>=6){
  inp = get_input();
  }
  TM_min += ((double)(inp) - 48);
  LCD_command(CLEAR_DISPLAY);
  displayTime(TM_min,TM_sec);
  //get seconds
  inp = get_input();
  while(((double)(inp) - 48)>=6){
  inp = get_input();
  }
  TM_sec += 10*((double)(inp) - 48);
  LCD_command(CLEAR_DISPLAY);
  displayTime(TM_min,TM_sec);
  inp = get_input();
  while(((double)(inp) - 48)>=6){
  inp = get_input();
  }
  TM_sec += ((double)(inp) - 48);
  LCD_command(CLEAR_DISPLAY);
  displayTime(TM_min,TM_sec);
  
}

void timer(){
  LCD_command(CLEAR_DISPLAY);
  LCD_printString("Please enter time");
  //TMreset();
  TMset();
  reset_mode(1);
  LCD_setcursorRowCol(1,0);
  LCD_printString("Press any key to start");
  get_input();
  periodic_timer(1000);
    while(get_mode(1) == 0){
     __asm("wfi\n");
  }
}