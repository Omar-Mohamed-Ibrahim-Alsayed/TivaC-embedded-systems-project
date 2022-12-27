#include "KeyPad.h"
#include "DataTypes.h"
#include <stdbool.h>
#include "LCD.h"
#include <stdio.h>
#include <string.h>

//---------------------------------------------------Global variables---------------------------------------------------

//Arrays used to check the type of the input
int8 ops[4] = {'+','-', '*', '/'};
int8 nums[10] = { '1', '2',  '3','4', '5',  '6','7', '8',  '9', '0'};

//Array containing the terms of the equation
float32 termsFirst[10] = {0};
//Array containing the operators of the equation
int8 operators[9];
//Variable to detect the current element of the equation
uint32 current_term = 0;
//Current state of the FSM
uint32 state = 0;
//Used to add floating points
uint32 floating = 10;
//The input comming from the keypad
int8 key;
//The final answer
float32 answer;
//Pointer to separate the array while rearranging
int32 tmp=10;

//---------------------------------------------------Prototypes---------------------------------------------------

void calc();

//---------------------------------------------------Functions----------------------------------------------------

//function to reset the calculator for the next use
void reset_calc()
{
    state = 0;
    floating = 10;
    current_term = 0;
    tmp=10;
    for(int i = 0; i<10; i++) {
        termsFirst[i]=0;
    }
}

//Function to pop an element from the array and shift the elements after it to its place
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


//Function to calculate the result of the equation
long float result()
{
    //get the size of both terms array and operators array
    int length = sizeof(termsFirst)/sizeof(termsFirst[0]);
    int length2 = sizeof(operators)/sizeof(operators[0]);

    //solve the multiplication and division first and pop them from the arrays
    for(int i = 0; i<9; i++) {
        if(operators[i]=='*') {
            termsFirst[i] = termsFirst[i] * termsFirst[i+1];
            reArrange(i+2);
        } else if(operators[i]=='/') {
            termsFirst[i] = termsFirst[i] / termsFirst[i+1];
            reArrange(i+2);
        }
    }

    //start with the first element so that if the equation contains only one element
    float32 total =termsFirst[0];
    int j = 0;
    //Solve the equation depending on which operator is used
    for(int i = 1 ; i < tmp+1; i++) {
        float32 t2 = termsFirst[i];
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

//Function to check if the input is an operator
bool isOp(int8 key)
{
    for(int i = 0; i<4; i++) {
        if(key == ops[i])
            return true;
    }
    return false;
}

//Function to check if the input is a number
bool isNum(int8 key)
{
    for(int i = 0; i<10; i++) {
        if(key == nums[i])
            return true;
    }
    return false;
}

//The main function of calculator.c containing the FSM
void calc()
{

    //first clear the display and reset all elements
    LCD_command(CLEAR_DISPLAY);
    reset_calc();
    bool end = true;

    //then  while the equation didn't end or there wasn't any error move through the FSM
    while(end) {
        switch(state) {

        //state 0 term
        case(0): {
            key = get_input();
            LCD_data(key);

            //if it is a number multiply the current element by 10 and add the input to it
            if(isNum(key)) {
                termsFirst[current_term] = (termsFirst[current_term]*10)+ (key-48);

            }
            //if it is an operator add the input to the operators array, increment current element, move to state 2
            else if(isOp(key)) {
                state = 2;
                operators[current_term]=key;
                current_term++;
            }
            //if it is a point move to state 1
            else if(key == '.') {
                state=1;
            }
            //if it is equal call the result function to calculat current elments
            else {
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

            //if it is a number divide it by the current floating value and add it to the current term
            if(isNum(key)) {
                termsFirst[current_term] = (termsFirst[current_term])+ ((float)(key-48)/(float)floating);
                floating *=10;
            }
            //if it is an operator add the input to the operators array, increment current element, move to state 2 and reset the floating value
            else if(isOp(key)) {
                floating = 10;
                state = 2;
                operators[current_term]=key;
                current_term++;
            }
            //if it is a point this is an error (9..)
            else if(key == '.') {
                //error
                LCD_errormsg();
                end = false;
            }
            //if it is equal call the result function to calculat current elments
            else {
                answer = result();
                LCD_setcursorRowCol(1,1);
                LCD_printFloat(answer);
                get_input();
                end = false;
            }
            break;
        }

        //state 2 Operator
        case(2): {
            key = get_input();
            LCD_data(key);

            //if it is a number multiply the current element by 10 and add the input to it
            if(isNum(key)) {
                state = 0;
                termsFirst[current_term] = (termsFirst[current_term]*10)+ ((key) - 48);
            }
            //if it is an operator this is an error (9 +-)
            else if(isOp(key)) {
                //error
                LCD_errormsg();
                end = false;
            }
            //if it is a point move to state 1
            else if(key == '.') {
                state=1;
            }
            //if it is equal this is an error (9 +=)
            else {
                //error
                LCD_errormsg();
                end = false;
            }
            break;
        }
        }
    }
}