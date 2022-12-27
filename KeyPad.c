#include "DIO.h"
#include "LCD.h"

//key layout
char key44[4][4] = {{ '1', '2',  '3', '+'},
    { '4', '5',  '6', '-'},
    { '7', '8',  '9', '*'},
    { '.', '0',  '=', '/'}
};


//getting input from keypad
int8 get_input()
{
    while(1) {
        for(int i = 0 ; i < 4; i++) {
            DIO_WritePin(PORTE, i, 0);
            for(int j = 0; j < 4; j++) {
                if((DIO_ReadPin(PORTC, j+4) == 0)) {
                    while((DIO_ReadPin(PORTC, j+4) == 0)) {
                        delay_ms(50);
                    }
                    return(key44[i][j]);
                }

            }
            DIO_WritePin(PORTE, i, 1);
        }
    }
}