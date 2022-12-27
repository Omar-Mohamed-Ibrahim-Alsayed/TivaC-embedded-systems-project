 #include "delay.h"

//delay with millisecond
void delay_ms(int time)
{
int i, j;
for(i = 0 ; i < time; i++)
  for(j = 0; j < 3180; j++){}
}

//delay with microsecond
void delay_us(int time)
{
int i, j;
for(i = 0 ; i < time; i++)
  for(j = 0; j < 3; j++){}
}