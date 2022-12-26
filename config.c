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

int16 SW_mode = 0;
int16 TM_mode = 0;
int16 calc_mode = 0;
bool pause = false;
bool reset = false;

bool get_pause(){
  return pause;
}

bool get_reset(){
  return reset;
}

void set_reset(){
  reset = false;
}

int16 get_mode(int16 m){
  if(m==0){
    return SW_mode;}
  else if(m == 1){
    return TM_mode;
  }
  else{
    return calc_mode;
  }
}



void reset_mode(int16 m){
  if(m==0){
    SW_mode = 0;}
  else if(m == 1){
    TM_mode = 0;
  }
  else{
    calc_mode = 0;
  }
}



void change_mode(){
  GPIOIntClear(GPIO_PORTA_BASE, GPIO_PIN_2);
     calc_mode =1;
     SW_mode =1;
     TM_mode =1;
    
}

void toggle_pause(){
    while((DIO_ReadPin(PORTE, 4) == 0)){
      delay_ms(50);}
    GPIOIntClear(GPIO_PORTE_BASE, GPIO_PIN_4);
    if(pause == true)
      pause = false;
    else
      pause = true;
    
}

void toggle_reset(){
    GPIOIntClear(GPIO_PORTD_BASE, GPIO_PIN_2);
    delay_ms(500);
    reset = true;
}

void GPIOF_init(){
// Enable the GPIOF peripheral
//
SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
//
// Wait for the GPIOF module to be ready.
//
while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA))
{
}
	
	// Pin A2 setup
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);		// Enable port A
	GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_2);	// Init PA2,3,4 as input
	GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_2,
		GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);	// Enable weak pullup resistor for PA2,3,4

	// Interrupt setuü
	GPIOIntDisable(GPIO_PORTA_BASE, GPIO_PIN_2);		// Disable interrupt for PA2,3,4 (in case it was enabled)
	GPIOIntClear(GPIO_PORTA_BASE, GPIO_PIN_2);		// Clear pending interrupts for PA2,3,4
	GPIOIntRegister(GPIO_PORTA_BASE, change_mode);		// Register our handler function for port F
	GPIOIntTypeSet(GPIO_PORTA_BASE, GPIO_PIN_2,
		GPIO_FALLING_EDGE);				// Configure PA2,3,4 for falling edge trigger
	IntPrioritySet(INT_GPIOA, 0x00);
        GPIOIntEnable(GPIO_PORTA_BASE, GPIO_PIN_2);		// Enable interrupt for PA2,3,4
        
// Enable the GPIOF peripheral
//
SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
//
// Wait for the GPIOF module to be ready.
//
while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE))
{
}
	
	// Pin A2 setup
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);		// Enable port A
	GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_4);	// Init PA2,3,4 as input
	GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_4,
		GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);	// Enable weak pullup resistor for PA2,3,4

	// Interrupt setuü
	GPIOIntDisable(GPIO_PORTE_BASE, GPIO_PIN_4);		// Disable interrupt for PA2,3,4 (in case it was enabled)
	GPIOIntClear(GPIO_PORTE_BASE, GPIO_PIN_4);		// Clear pending interrupts for PA2,3,4
	GPIOIntRegister(GPIO_PORTE_BASE, toggle_pause);		// Register our handler function for port F
	GPIOIntTypeSet(GPIO_PORTE_BASE, GPIO_PIN_4,
		GPIO_FALLING_EDGE);				// Configure PA2,3,4 for falling edge trigger
	IntPrioritySet(INT_GPIOE, 0x01);
        GPIOIntEnable(GPIO_PORTE_BASE, GPIO_PIN_4);
        
// Enable the GPIOF peripheral
//
SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
//
// Wait for the GPIOF module to be ready.
//
while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD))
{
}
	
	// Pin A2 setup
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);		// Enable port A
	GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_2);	// Init PA2,3,4 as input
	GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_2,
		GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);	// Enable weak pullup resistor for PA2,3,4

	// Interrupt setuü
	GPIOIntDisable(GPIO_PORTD_BASE, GPIO_PIN_2);		// Disable interrupt for PA2,3,4 (in case it was enabled)
	GPIOIntClear(GPIO_PORTD_BASE, GPIO_PIN_2);		// Clear pending interrupts for PA2,3,4
	GPIOIntRegister(GPIO_PORTD_BASE, toggle_reset);		// Register our handler function for port F
	GPIOIntTypeSet(GPIO_PORTD_BASE, GPIO_PIN_2,
		GPIO_FALLING_EDGE);				// Configure PA2,3,4 for falling edge trigger
	IntPrioritySet(INT_GPIOD, 0x02);
        GPIOIntEnable(GPIO_PORTD_BASE, GPIO_PIN_2);

}