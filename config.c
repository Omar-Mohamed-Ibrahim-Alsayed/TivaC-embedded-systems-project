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

int16 SW_mode = 0;
int16 TM_mode = 0;
int16 calc_mode = 0;
bool pause = false;
bool reset = false;

//-------------------------------------------------------Functions-------------------------------------------------------

//get the pause value
bool get_pause()
{
    return pause;
}

//get the reset value
bool get_reset()
{
    return reset;
}

//set the reset value
void set_reset()
{
    reset = false;
}


//get the current mode
int16 get_mode(int16 m)
{
    if(m==0) {
        return SW_mode;
    } else if(m == 1) {
        return TM_mode;
    } else {
        return calc_mode;
    }
}


//set the mode value
void reset_mode(int16 m)
{
    if(m==0) {
        SW_mode = 0;
    } else if(m == 1) {
        TM_mode = 0;
    } else {
        calc_mode = 0;
    }
}


//interrupt handler for the mode button returns you to the mode menu
void change_mode()
{
    GPIOIntClear(GPIO_PORTA_BASE, GPIO_PIN_2);
    calc_mode =1;
    SW_mode =1;
    TM_mode =1;

}

//interrupt handler for the pause button toggles the pause value
void toggle_pause()
{
    while((DIO_ReadPin(PORTE, 4) == 0)) {
        delay_ms(50);
    }
    GPIOIntClear(GPIO_PORTE_BASE, GPIO_PIN_4);
    if(pause == true)
        pause = false;
    else
        pause = true;

}

//interrupt handler for the reser button sets the reset value to true
void toggle_reset()
{
    GPIOIntClear(GPIO_PORTD_BASE, GPIO_PIN_2);
    delay_ms(500);
    reset = true;
}

//interrupts initialization for all buttons
void GPIOF_init()
{

// Enable the GPIOA peripheral(mode button)
//
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
//
// Wait for the GPIOA module to be ready.
//
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA)) {
    }

    // Pin A2 setup
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);		// Enable port A
    GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_2);	// Init PA2 as input
    GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_2,
                     GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);	// Enable weak pullup resistor for PA2

    // Interrupt setup
    GPIOIntDisable(GPIO_PORTA_BASE, GPIO_PIN_2);		// Disable interrupt for PA2 (in case it was enabled)
    GPIOIntClear(GPIO_PORTA_BASE, GPIO_PIN_2);		// Clear pending interrupts for PA2
    GPIOIntRegister(GPIO_PORTA_BASE, change_mode);		// Register our handler function for port A
    GPIOIntTypeSet(GPIO_PORTA_BASE, GPIO_PIN_2,
                   GPIO_FALLING_EDGE);				// Configure PA2 for falling edge trigger
    IntPrioritySet(INT_GPIOA, 0x00);                        // Set to highest priority
    GPIOIntEnable(GPIO_PORTA_BASE, GPIO_PIN_2);		// Enable interrupt for PA2

// Enable the GPIOE peripheral(pause button)
//
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
//
// Wait for the GPIOE module to be ready.
//
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE)) {
    }

    // Pin E4 setup
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);		// Enable port E
    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_4);	// Init PE4 as input
    GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_4,
                     GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);	// Enable weak pullup resistor for PE4

    // Interrupt setup
    GPIOIntDisable(GPIO_PORTE_BASE, GPIO_PIN_4);		// Disable interrupt for PE4 (in case it was enabled)
    GPIOIntClear(GPIO_PORTE_BASE, GPIO_PIN_4);		// Clear pending interrupts for PE4
    GPIOIntRegister(GPIO_PORTE_BASE, toggle_pause);		// Register our handler function for port E
    GPIOIntTypeSet(GPIO_PORTE_BASE, GPIO_PIN_4,             // Set to second highest priority
                   GPIO_FALLING_EDGE);				// Configure PE4 for falling edge trigger
    IntPrioritySet(INT_GPIOE, 0x01);
    GPIOIntEnable(GPIO_PORTE_BASE, GPIO_PIN_4);

// Enable the GPIOD peripheral (reset button)
//
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
//
// Wait for the GPIOD module to be ready.
//
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD)) {
    }
    
    // Pin D2 setup
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);		// Enable port D
    GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_2);	// Init PD2 as input
    GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_2,
                     GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);	// Enable weak pullup resistor for PD2

    // Interrupt setup
    GPIOIntDisable(GPIO_PORTD_BASE, GPIO_PIN_2);		// Disable interrupt for PD2 (in case it was enabled)
    GPIOIntClear(GPIO_PORTD_BASE, GPIO_PIN_2);		// Clear pending interrupts for PD2
    GPIOIntRegister(GPIO_PORTD_BASE, toggle_reset);		// Register our handler function for port D
    GPIOIntTypeSet(GPIO_PORTD_BASE, GPIO_PIN_2,             // Set to third highest priority
                   GPIO_FALLING_EDGE);				// Configure PD2 for falling edge trigger
    IntPrioritySet(INT_GPIOD, 0x02);
    GPIOIntEnable(GPIO_PORTD_BASE, GPIO_PIN_2);
    DIO_Init(PORTD, 7, OUT);
    DIO_WritePin(PORTD,7,0);
    

}

//keypad initialization
void keypad_Init(void)
{
  DIO_Init(PORTE, 0, OUT);
  DIO_Init(PORTE, 1, OUT);
  DIO_Init(PORTE, 2, OUT);
  DIO_Init(PORTE, 3, OUT);
  DIO_Init(PORTC, 4, IN);
  DIO_Init(PORTC, 5, IN);
  DIO_Init(PORTC, 6, IN);
  DIO_Init(PORTC, 7, IN);
}

void LCD_Init(void)
{
// LCD ports' Initialization:
    SYSCTL_RCGCGPIO_R |= 0x03;              // enable clock to port A & B
    while ((SYSCTL_RCGCGPIO_R&0x03)==0);    //wait for clock to be enabled
    GPIO_PORTA_CR_R  |= 0xE0;               // allow changes to all the bits in port A
    GPIO_PORTA_DIR_R |= 0xE0;               // set PORTA pin 7-5 as output for control
    GPIO_PORTA_DEN_R |= 0xE0;               // set PORTA pin 7-5 as digital pins
    GPIO_PORTB_CR_R  |= 0xFF;                // allow changes to all the bits in port B
    GPIO_PORTB_DIR_R |= 0xFF;               // set all PORTB pins as output for data
    GPIO_PORTB_DEN_R |= 0xFF;               // set all PORTB pins as digital pins

// LCD Initialization Commands:
    delay_ms(20);
    LCD_command(FUNC_SET_8BIT_2LINE);
    delay_ms(5);
    LCD_command(FUNC_SET_8BIT_2LINE);
    delay_us(100);
    LCD_command(FUNC_SET_8BIT_2LINE);
    LCD_command(DISPLAY_OFF_CURSOR_OFF);
    LCD_command(CLEAR_DISPLAY);
    LCD_command(SHIFT_CURSOR_RIGHT) ;
    LCD_command(DISPLAY_ON_CURSOR_ON);
}


