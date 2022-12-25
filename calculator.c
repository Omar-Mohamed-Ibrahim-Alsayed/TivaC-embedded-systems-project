#include "KeyPad.h"
#include "DataTypes.h"
#include <stdbool.h>
#include "LCD.h"
#include <stdio.h>
#include <string.h>

char ops[4] = {'+','-', '*', '/'};
char nums[10] = { '1', '2',  '3','4', '5',  '6','7', '8',  '9', '0'};
float termsFirst[10] = {0};
uint32 current_term = 0;
char operators[9];
uint32 state = 0;
uint32 floating = 10;
char key;
float answer;
int tmp=10;

void calc();

void reset_calc(){
  state = 0;
  floating = 10;
  current_term = 0;
  tmp=10;
  for(int i = 0;i<10;i++){
    termsFirst[i]=0;
  }
}

void reArrange(int pos)
{
    for(int i=pos-1; i<10; i++) {
        if(i<tmp)
            termsFirst[i] = termsFirst[i + 1];
        else
            termsFirst[i] = 0;
    }
    for(int i=pos-1; i<9; i++) {
        if(i<tmp)
            operators[i-1] = operators[i ];
        else
            operators[i-1] = '+';
    }

    
    tmp--;
}

long float result()
{
    int length = sizeof(termsFirst)/sizeof(termsFirst[0]);
    int length2 = sizeof(operators)/sizeof(operators[0]);


    for(int i = 0; i<9; i++) {
        if(operators[i]=='*') {
            termsFirst[i] = termsFirst[i] * termsFirst[i+1];
            reArrange(i+2);
        } else if(operators[i]=='/') {
            termsFirst[i] = termsFirst[i] / termsFirst[i+1];
            reArrange(i+2);
        }
    }
    float total =termsFirst[0];
    int j = 0;
    for(int i = 1 ; i < tmp+1; i++) {
        float t2 = termsFirst[i];
        char op = operators[i-1];
        switch(op) {
        case('+'): {
            total +=t2;
            break;
        }
        case('-'): {
            total -=t2;
            break;
        }
        case('*'): {
            total *=t2;
            break;
        }
        case('/'): {
            total /=t2;
            break;
        }
        }
        j++;
    }
    return total;
}


bool isOp(int8 key){
  for(int i = 0;i<4;i++){
    if(key == ops[i])
       return true;
  }
  return false;
}
bool isNum(int8 key){
   for(int i = 0;i<10;i++){
    if(key == nums[i])
       return true;
  }
  return false; 
}

double80 getDouble(char k){
  return ((k) - 48);
}


void calc(){
  //LCD_Init();
  LCD_command(CLEAR_DISPLAY);
  //keypad_Init();
  reset_calc();

  bool end = true;
  while(end){
    switch(state){
      
      //state 0 term
      case(0): {
        key = get_input();
        LCD_data(key);
        if(isNum(key)){
          termsFirst[current_term] = (termsFirst[current_term]*10)+ (key-48);

        }
        else if(isOp(key)){
          state = 2;
          operators[current_term]=key;
          current_term++;
        }
        else if(key == '.'){
          state=1;
        }
        else{
          answer = result();
          LCD_setcursorRowCol(1,1);
          LCD_printFloat(answer);
          get_input();
          end = false;
        }
        break;
      }
      
      //state 1 floating point
      case(1): {
        key = get_input();
        LCD_data(key);
        if(isNum(key)){
          termsFirst[current_term] = (termsFirst[current_term])+ ((float)(key-48)/(float)floating);
          floating *=10;
        }
        else if(isOp(key)){
          floating = 10;
          state = 2;
          operators[current_term]=key;
          current_term++;
        }
        else if(key == '.'){
          //error
          LCD_errormsg();
          end = false;
        }
        else{
          answer = result();
          LCD_setcursorRowCol(1,1);
          LCD_printFloat(answer);
          get_input();
          end = false;
        }
        break;
      }
      case(2): {
        key = get_input();
        LCD_data(key);
        if(isNum(key)){
          state = 0;
          termsFirst[current_term] = (termsFirst[current_term]*10)+ ((key) - 48);
        }
        else if(isOp(key)){
          //error
          LCD_errormsg();
          end = false;
        }
        else if(key =='.'){
          termsFirst[current_term] = (termsFirst[current_term])+(((key) - 48)/floating);
        }
        else{
          //error
          LCD_errormsg();
          end = false;
        }
        break;
      }}}}