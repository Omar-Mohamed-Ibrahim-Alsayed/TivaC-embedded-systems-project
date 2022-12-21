#include "KeyPad.h"
#include "DataTypes.h"
#include <stdbool.h>
#include "LCD.h"
#include <stdio.h>
#include <string.h>

char ops[4] = {'+','-', '*', '/'};
char nums[10] = { '1', '2',  '3','4', '5',  '6','7', '8',  '9', '0'};
double80 termsFirst[10] = {0};
uint32 current_term = 0;
char operators[9];
uint32 state = 0;
uint32 floating = 10;
char key;

void calc();

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
  return ((double)(k) - 48);
}



void result(){
  double80 total = termsFirst[0];
  int16 j = 0;
  for(int i = 1 ; i < 10; i++){
    double80 t2 = termsFirst[i];
    char op = operators[j];;
    switch(op){
      case('+'):{
        total +=t2;
        break;
      }
      case('-'):{
        total -=t2;
        break;
      }
      case('*'):{
        total *=t2;
        break;
      }
      case('/'):{
        total /=t2;
        break;
      }
    }
    j++;
  }
  LCD_setcursorRowCol(1,1);
  LCD_printFloat(total);

}

void calc(){
  LCD_Init();
  LCD_command(CLEAR_DISPLAY);
  keypad_Init();

  bool end = true;
  while(end){
    switch(state){
      
      //state 0 term
      case(0): {
        key = get_input();
        LCD_data(key);
        double64 tmp = getDouble(key);
        if(isNum(key)){
          termsFirst[current_term] = (termsFirst[current_term]*10)+ (tmp);
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
          result();
          end = false;
        }
        break;
      }
      
      //state 1 floating point
      case(1): {
        key = get_input();
        LCD_data(key);
        if(isNum(key)){
          double64 tmp = getDouble(key);
          termsFirst[current_term] = (termsFirst[current_term])+ (tmp/floating);
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
        }
        else{
          result();
          end = false;
        }
        break;
      }
      case(2): {
        key = get_input();
        LCD_data(key);
        if(isNum(key)){
          state = 0;
          termsFirst[current_term] = (termsFirst[current_term]*10)+ ((double)(key) - 48);
        }
        else if(isOp(key)){
          //error
          LCD_errormsg();
        }
        else if(key =='.'){
          termsFirst[current_term] = (termsFirst[current_term])+(((double)(key) - 48)/floating);
        }
        else{
          //error
          LCD_errormsg();
        }
        break;
      }
  }

  }
}