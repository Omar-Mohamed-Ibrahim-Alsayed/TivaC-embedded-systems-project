#include "KeyPad.h"
#include "DataTypes.h"
#include <stdbool.h>
#include "LCD.h"

bool get_pause();

bool get_reset();

void set_reset();

int16 get_mode(int16 m);

void reset_mode(int16 m);

void GPIOF_init();

void keypad_Init(void);

void LCD_Init(void);

