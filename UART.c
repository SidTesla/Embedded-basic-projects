/*
 * File:   UART.c
 * Author: Siddharthan A
 *
 * Created on 28 May, 2021, 1:02 PM
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
#define _XTAL_FREQ 6000000

void init();
void delay(unsigned int);
unsigned char button;

void main(void)
{
    init();
    while(1)
    {
       button = PORTB&0xF0;
       if(button==0xE0)
       {
           TXREG = 'A';
       }
       else if(button==0xD0)
       {
           TXREG = 'B';
       }
       __delay_ms(200);
    }
    
    
}

void init()
{
    TRISB = 0xF0;
    PORTB = 0xF0;
    OPTION_REG = 0x7F;
    TRISC = 0x80;
    PORTC = 0xFF;
    TXSTA = 0x24; //Enable transmit and high speed baud rate
    RCSTA = 0x90; //enable serial ports and continuous receive
    SPBRG = 0x26; //baud rate of 9600 at high speed
}

void delay(unsigned int time)
{
    while(--time);
}