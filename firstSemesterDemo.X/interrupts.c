#include <xc.h>
#include "uart.h"
#include "gpio.h"
#include "app.h"
#include "bluetooth.h"

/******************************************************************************/
/* Interrupt Vector Options                                                   */
/******************************************************************************/
/*                                                                            */
/* Refer to the C30 (MPLAB C Compiler for PIC24F MCUs and dsPIC33F DSCs) User */
/* Guide for an up to date list of the available interrupt options.           */
/* Alternately these names can be pulled from the device linker scripts.      */
/*                                                                            */
/* PIC24F Primary Interrupt Vector Names:                                     */
/*                                                                            */
/* _INT0Interrupt      _IC4Interrupt                                          */
/* _IC1Interrupt       _IC5Interrupt                                          */
/* _OC1Interrupt       _IC6Interrupt                                          */
/* _T1Interrupt        _OC5Interrupt                                          */
/* _Interrupt4         _OC6Interrupt                                          */
/* _IC2Interrupt       _OC7Interrupt                                          */
/* _OC2Interrupt       _OC8Interrupt                                          */
/* _T2Interrupt        _PMPInterrupt                                          */
/* _T3Interrupt        _SI2C2Interrupt                                        */
/* _SPI1ErrInterrupt   _MI2C2Interrupt                                        */
/* _SPI1Interrupt      _INT3Interrupt                                         */
/* _U1RXInterrupt      _INT4Interrupt                                         */
/* _U1TXInterrupt      _RTCCInterrupt                                         */
/* _ADC1Interrupt      _U1ErrInterrupt                                        */
/* _SI2C1Interrupt     _U2ErrInterrupt                                        */
/* _MI2C1Interrupt     _CRCInterrupt                                          */
/* _CompInterrupt      _LVDInterrupt                                          */
/* _CNInterrupt        _CTMUInterrupt                                         */
/* _INT1Interrupt      _U3ErrInterrupt                                        */
/* _IC7Interrupt       _U3RXInterrupt                                         */
/* _IC8Interrupt       _U3TXInterrupt                                         */
/* _OC3Interrupt       _SI2C3Interrupt                                        */
/* _OC4Interrupt       _MI2C3Interrupt                                        */
/* _T4Interrupt        _U4ErrInterrupt                                        */
/* _T5Interrupt        _U4RXInterrupt                                         */
/* _INT2Interrupt      _U4TXInterrupt                                         */
/* _U2RXInterrupt      _SPI3ErrInterrupt                                      */
/* _U2TXInterrupt      _SPI3Interrupt                                         */
/* _SPI2ErrInterrupt   _OC9Interrupt                                          */
/* _SPI2Interrupt      _IC9Interrupt                                          */
/* _IC3Interrupt                                                              */
/******************************************************************************/
/* Interrupt Routines                                                         */
/******************************************************************************/
uint16_t ticker = 0;

/* Timer 1 ISR */
void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void) {
    
    IFS0bits.T1IF = 0;
    TMR1 = 0x0000;    
    ticker++;
    
    // counts seconds
    if ( ticker%10000 == 0 )   
        
    {  appData.sampleFlag = true;
       (appData.seconds == 59)? (appData.seconds=0): (appData.seconds++);
       appData.ms = 0;
       ticker = 0;      
    }
    
    // counts milliseconds
    if ( ticker%10 == 0 )
    {
        (appData.ms==999)? appData.ms=0: appData.ms++;        
    }
    
    //delay countdown
    if (appData.delay > 0)
    {
        appData.delay--;       
    }
    
}

/* UART1 U1RX interrupt routine code */
void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt( void )
{
    appData.U1RxByte = U1RXREG;
    
    if(appData.U1RxByte != '\0')
    {
        if(bleData.packetIndex == (PACKET_LEN - 1))
            bleData.packetIndex = 0;
        bleData.packetBuf[bleData.packetIndex++] = appData.U1RxByte;
        bleData.packetBuf[bleData.packetIndex] = '\0';
        bleData.dataReceived = true;
    }
    IFS0bits.U1RXIF = 0; // clear U1RX flag 
}
