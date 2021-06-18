/*
 * File:   UARTReceive.c
 * Author: Siddharthan A
 *
 * Created on 29 May, 2021, 8:25 PM
 */



// PIC16F877A Configuration Bit Settings

// 'C' source line config statements

// CONFIG
#pragma config FOSC = EXTRC     // Oscillator Selection bits (RC oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include<string.h>
#define _XTAL_FREQ 6000000

void init();
void lcdInit();
void lcdCommand(unsigned char);
void lcdData(unsigned char);
void delay(unsigned int);
unsigned char letter,i;
unsigned char msgA[]="LED 1 is ON and LED 2 is OFF\n";
unsigned char msgB[]="LED 2 is ON and LED 1 is OFF\n";

void main()
{
    init();
    lcdInit();
    while(1)
    {
        lcdCommand(0x80);
        delay(200);
        while(PIR1&0x20)
        {
            letter = RCREG;
            if(letter=='A')
            {
               for(i=0;i<16;i++)
                   lcdData(msgA[i]);
               lcdCommand(0xC0);
               for(i=16;i<strlen(msgA);i++)
                   lcdData(msgA[i]);
               PORTC = 0x02;               
            }
            else if(letter=='B')
            {
                for(i=0;i<strlen(msgB);i++)
                   lcdData(msgB[i]);
                lcdCommand(0xC0);
               for(i=16;i<strlen(msgB);i++)
                   lcdData(msgB[i]);
                PORTC = 0x04;
            }
        }    
    }
}

void init()
{
    TRISB = 0x00; //RS and E pins of lcd
    TRISD = 0x00; //data pins of lcd
    TRISC = 0x80; //led and serial pins
    PORTC = 0x80;
    TXSTA = 0x24; //Enable transmit and high speed baud rate
    RCSTA = 0x90; //enable serial ports and continuous receive
    SPBRG = 0x26; //baud rate of 9600 at high speed    
}

void lcdInit()
{
    lcdCommand(0x38);  //Initialization process
    delay(100);
    lcdCommand(0x38);
    delay(100);
    lcdCommand(0x38);
    delay(100);
    lcdCommand(0x38);  //2 lines and 5x7 font selection
    delay(100);
    lcdCommand(0x0c);  //Display ON
    delay(100);
    lcdCommand(0x06);  //Increment cursor mode
    delay(100);
    lcdCommand(0x01);  //Clear display
    delay(100);
}

void lcdData(unsigned char x)
{
    PORTB|=0x01;    //Make RS=1
    PORTD=x;
    PORTB|=0x02;    //Make EN=1
    PORTB&=~0x02;   //Make EN=0
    delay(100); 
}

void lcdCommand(unsigned char x)
{
    PORTB&=~0x01;   //Make RS=0
    PORTD=x;        //Enter data
    PORTB|=0x02;    //Make EN=1
    PORTB&=~0x02;   //Make EN=0
    delay(100);
}

void delay(unsigned int time)
{
    while(--time);
}