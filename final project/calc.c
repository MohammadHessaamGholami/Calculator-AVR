/*******************************************************
This program was created by the
CodeWizardAVR V3.12 Advanced
Automatic Program Generator
© Copyright 1998-2014 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : calc
Version : 
Date    : 1/30/2019
Author  : 
Company : 
Comments: 


Chip type               : ATmega32
Program type            : Application
AVR Core Clock frequency: 8.000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 512
*******************************************************/
// DB0 - PORTA Bit 0
// DB1 - PORTA Bit 1
// DB2 - PORTA Bit 2
// DB3 - PORTA Bit 3
// DB4 - PORTA Bit 4
// DB5 - PORTA Bit 5
// DB6 - PORTA Bit 6
// DB7 - PORTA Bit 7
// E - PORTB Bit 0
// RD /WR - PORTB Bit 1
// RS - PORTB Bit 2
// /RST - PORTB Bit 3
// CS1 - PORTB Bit 4
// CS2 - PORTB Bit 5

#include <mega32.h>
#include <glcd.h>
#include <font5x7.h>
#include <ctype.h>
#include <delay.h>
#include <math.h>
#include <stdbool.h>
#include "stdio.h"

const char digits[]="0123456789";
int number_of_input=0;
char res;
int table[2][4];
int index_number_1=0;
int index_number_2=0;
int number1;
int number2;
int resault;
int cursor=10;
int operation=4;
int ten;
int i=0;
int j=0;
int nDigits=0;
int temp_number=0;
bool is_True=true;

int countDigit(long n) 
{ 
    int count = 0; 
    while (n != 0) { 
        n = n / 10; 
        ++count; 
    } 
    return count; 
} 
int getdigit(int num_t, int n)
{
    int r=0;
    int t1=0;
    int t2=0;
 
    t1 = pow(10, n+1);
    r = num_t % t1;
 
    if (n > 0)
    {
        t2 = pow(10, n);
        r = r / t2;
    }
 
    return r;
}
int getMyDigit(int mynumber,int k){
    return (int)floor((mynumber/pow(10,k)))%10;


}
void reset(){
    number_of_input=0;
    index_number_1=0;
    index_number_2=0;
    cursor=10;
    operation=4; 
    i=0;
    j=0;
    number1=0;
    number2=0;
    resault=0;
    ten=0;
    nDigits=0;
    temp_number=0;
    for(i=0;i<2;i++){
        for(j=0;j<4;j++){
            table[i][j]=0;
        }
    }       
}
char int_to_char(int num){
    return digits[num%10];

}
void config_GLCD(){
        GLCDINIT_t glcd_init_data;
        glcd_init_data.font=font5x7;
        glcd_init_data.readxmem=NULL;
        glcd_init_data.writexmem=NULL;
        glcd_init(&glcd_init_data);
}
void config_port(){
    DDRC = 0x00;
    PORTC = 0xFF;
    DDRD = 0x00;
    PORTD = 0xFF;
}
void show_number(int number){
    switch(number_of_input)
    {
        case 0:{
            table[0][index_number_1]=number; 
            index_number_1=index_number_1+1;  
            res=int_to_char(number);
            glcd_putcharxy(cursor,10,res);
            cursor=cursor+10;
            break;    
        }
        case 1:{
            table[1][index_number_2]=number;
            index_number_2=index_number_2+1;  
            res=int_to_char(number);
            glcd_putcharxy(cursor,10,res);
            cursor=cursor+10;  
            break;  
        }
    }                      
}
void show_error(){
    glcd_clear();
    reset();
    glcd_outtextxyf(10,10,"Error"); 
    is_True=false;
}
void show_operation(int number_of_operation){
    operation=number_of_operation;
    number_of_input=number_of_input+1;
    if(number_of_input==2){
         show_error();
    }
    else{
         switch(number_of_operation){
        case 0: glcd_outtextxyf(cursor,10,"+");cursor=cursor+10;break;
        case 1: glcd_outtextxyf(cursor,10,"-");cursor=cursor+10;break;
        case 2: glcd_outtextxyf(cursor,10,"*");cursor=cursor+10;break;
        case 3: glcd_outtextxyf(cursor,10,"/");cursor=cursor+10;break;
    }
    }
   

}
void calculate_and_reset_data(){
if(number_of_input!=1){
    show_error();
}
else{

    
    int mouse=10;
    glcd_clear();       
    number1=0;
    number2=0;         
    resault=0;
    for(i=0;i<index_number_1;i++){
        ten=pow(10,index_number_1 -1 -i);
        number1=number1+(table[0][i]*ten);   
    }
    for(i=0;i<index_number_2;i++){
        ten=pow(10,index_number_2 -1 -i); 
        number2=number2+(table[1][i]*ten);
    }  
    switch(operation){
        case 0 :
            resault=number1+number2;
            break;
        case 1:
            resault=number1-number2;        
            break;
        case 2:   
            resault=number1*number2;
            break;
        case 3:
            resault=number1/number2;
            break;
    }     
    nDigits=countDigit(resault);
    
    for (i=0;i<nDigits;i++){
          temp_number=getMyDigit(resault,nDigits-1-i);
          res=int_to_char(temp_number);
          glcd_putcharxy(mouse,10,res);
          mouse=mouse+10;
    } 
    reset();
    is_True=false;



}
    
}
void newInput(int input1,int input2){
    if(input1==0b01111111){             //*
        show_operation(2);
    }
    if(input1==0b10111111){             //6
        show_number(6); 
    }
    if(input1==0b11011111){             //5
        show_number(5); 
    } 
    if(input1==0b11101111){             //4
        show_number(4); 
    }
    if(input1==0b11110111){             //   /
        show_operation(3);

    }
    if(input1==0b11111011){             //9
        show_number(9);
    }
    if(input1==0b11111101){             //8
        show_number(8);
    }
    if(input1==0b11111110){             //7
        show_number(7);
    }          
    //*******************************************************
    
    if(input2==0b01111111){             //+
        show_operation(0);
    }
    if(input2==0b10111111){             //=
        
    }
    if(input2==0b11011111){             //0
        show_number(0);
    } 
    if(input2==0b11101111){             //reset
        glcd_clear();
    }
    if(input2==0b11110111){             //-
        show_operation(1);
    }
    if(input2==0b11111011){             //3
        show_number(3);
    }
    if(input2==0b11111101){             //2
        show_number(2);        
    }
    if(input2==0b11111110){             //1
        show_number(1);
    }
}
void main(void)
{
int input1=0;
int input2=0;
config_GLCD();
config_port();
while (1)
      {
      while(1){
            
            input1=PINC;
            input2=PIND;
            if(input1!=255 || input2 !=255){
                if(input2==0b10111111){    //=
                    break;
                }
                if (!is_True){       //opt like +,-,*,/               
                    glcd_clear();
                    is_True=true;
                }
                newInput(input1,input2);
                              
        }
        delay_ms(30);
   
      }
      calculate_and_reset_data();
                      

      }
}
