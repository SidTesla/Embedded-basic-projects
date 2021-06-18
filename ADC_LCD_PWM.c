/*
 * File:   ADC_LCD_PWM.c
 * Author: Siddharthan A
 *
 * Created on 10 May, 2021, 9:58 PM
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

void init();                        //initialization function
void LCDOutput(unsigned int);       //Display message to LCD
void delay(unsigned int);           
void lcdCommand(unsigned char);     //send command to LCD
void lcdData(unsigned char);        //send data to LCD
unsigned int highOrder, lowOrder, value, lcdValue, duty;
unsigned char i, string[5]="rides";

void main()
{
    init();
    /*lcdCommand(0x80);
    LCDOutput(56);
    delay(100000);*/
    
    while(1)
    {
        ADCON0 = 0x81; //Configure A/D registers
        ADCON0|=0x04; //Start ADC conversion
        while(ADCON0&0x04);
        lowOrder = ADRESL;
        highOrder = ADRESH;
        value = ((unsigned int)highOrder<<8) + (unsigned int)lowOrder;
        duty = value/3;
        lcdCommand(0x80);
        LCDOutput(duty);
        CCPR1L = duty>>2;
        CCP1CON = ((duty&0x03)<<4) + (CCP1CON & 0x0C);
        delay(10000);
    }
}

void init()
{
    TRISA = 0x01;
    TRISB = 0x00;
    TRISC = 0x00;
    TRISD = 0x00;
    ADCON0 = 0x81;
    ADCON1 = 0x8E;
    PR2 = 0x5E;
    T2CON  = 0x06;
    CCP1CON = 0x0C;
    lcdCommand(0x38); //Initialization
    delay(100);
    lcdCommand(0x38);
    delay(100);
    lcdCommand(0x38);
    delay(100);
    lcdCommand(0x38); //2 lines and 5x7 font selection
    delay(100);
    lcdCommand(0x0C); //display ON
    delay(100);
    lcdCommand(0x06); //Increment cursor mode
    delay(100);
    lcdCommand(0x01); //clear display
    delay(100);
}

void lcdCommand(unsigned char c)
{     
    PORTB&=~0x01; //RS=0
    PORTD = c;
    PORTB|=0x02; //EN=1
    PORTB&=~0x02; //EN=0
    delay(100);
}

void lcdData(unsigned char d)
{
    PORTB|=0x01; //RS=1
    PORTD = d;
    PORTB|=0x02; //EN=1
    PORTB&=~0x02; //EN=0
    delay(100);
}

void LCDOutput(unsigned int out)
{
    unsigned char msg[5];
    i = 1;
    while(out!=0)
    {
        msg[i] = out%10;
        out = out/10;
        i++;
    }
    msg[i]='*';
    i--;
    if(msg[1]=='*')
    {
        lcdData(0x20);
        lcdData('0');
    }
    else if(msg[3]!='*')
    {
        while(i!=0)
        {
            lcdData(msg[i]+0x30);
            i--;
        }
    }
    else if(msg[2]!='*')
    {
        lcdData(0x20);
        while(i!=0)
        {
            lcdData(msg[i]+0x30);
            i--;
        }
    }
    else
    {
        lcdData(0x20);
        lcdData(0x20);
        while(i!=0)
        {
            lcdData(msg[i]+0x30);
            i--;
        }
    }
    lcdData('V');
}

void delay(unsigned int time)
{
    while(--time);
}
