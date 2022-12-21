#include "DIO.h"
#include "LCD.h"

#define GPIO_PORTB_CLK_EN  0x02 
  char key44[4][4] = {{ '1', '2',  '3', '+'},      
                      { '4', '5',  '6', '-'},      
                      { '7', '8',  '9', '*'},      
                      { '.', '0',  '=', '/'}};

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


int8 get_input(){
  while(1){
          for(int i = 0 ; i < 4; i++){
            DIO_WritePin(PORTE, i, 0);
            for(int j = 0; j < 4; j++){
              if((DIO_ReadPin(PORTC, j+4) == 0)){
                 while((DIO_ReadPin(PORTC, j+4) == 0)){
                 delay_ms(50);
                 }
                 return(key44[i][j]);
              }
                
            }
            DIO_WritePin(PORTE, i, 1);
          } 
        }  
}