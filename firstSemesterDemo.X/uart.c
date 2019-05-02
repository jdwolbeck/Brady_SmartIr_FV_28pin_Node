#include<xc.h>
#include "app.h"
#include "uart.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define MAX 40

char U1RxBuffer[MAX];
char U1TxBuffer[MAX];

void uart_init(void);    
void uart_write_byte(char);
void uart_print(char[]);
bool clearBuffer(void);


void uart_init(void){
/************************************************************
 * Configure UART 1 SFR's 
 ************************************************************/    
    // set pin direction 
    TRISBbits.TRISB2 = 1;
    TRISBbits.TRISB7 = 0;
    
    // set pins to digital
    ANSBbits.ANSB2 = 0;
    ANSBbits.ANSB7 = 0;
    //LATBbits.LATB7 = 1;
    
    // disable UART
    U1MODE = 0x0000;

    // Baudrate = 9600;    clock 4MHz; U1BRG 103
    // Baudrate = 115200;  clock 4MHz; U1BRG 8
    U1MODEbits.BRGH = 1;
    U1STA = 0x0000;
    U1BRG = 103;
    IPC2bits.U1RXIP = 1;
    IEC0bits.U1RXIE = 1;         // Enable UART RX interrupt
    IFS0bits.U1RXIF = 0;        // clear flag  
    U1MODEbits.UARTEN = 1;       // Enable UART and wait to enable the TX
    U1STAbits.UTXEN = 1;         // enable Transmission

}



void uart_write_byte(char ch )
{
    U1TXREG  = ch;
}

void uart_print(char str[])
{
    int i = 0;
    while(str[i] != '\0')
    {
        while(!U1STAbits.TRMT);
        U1TXREG = str[i++];
    }
}
//void uart_print(char buf[])
//{
//    int i = 0;
//    
//    while(buf[i] != '\0')
//    {
//     
//        while(U1STAbits.UTXBF == 0)
//        {
//            uart_write_byte( buf[i] );
//            i++;
//        }
//    }
//    clearBuffer();
//}

bool clearBuffer(void)
{
   int i = 0;
   
   for(i=0; i<MAX; i++) 
   {
       U1TxBuffer[i]='\0';
   }
    return 1;
    
} 
