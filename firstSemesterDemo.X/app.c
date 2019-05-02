 #include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "app.h"
#include "system.h"
#include "adc1.h"
#include "uart.h"
#include "gpio.h"
#include "bluetooth.h"


/**********************************************************
 * 
 * APP Macros
 * 
*************************************************************/

// SAMPLE_FRQ = 1, = 1 SECOND; 
#define SAMPLE_FRQ 5
#define ADC1_ENABLE 0x0020     // PORTB5
#define LED1        0x0040     // PORTB6


/**********************************************************
 * 
 * APP functions
 * 
*************************************************************/
void application_init(void);
bool getConversions(void);
void timer_init(void);
void ms_delay(int);

/**********************************************************
 * 
 * APP variables
 * 
*************************************************************/

APP_DATA appData;

void application_init(void)
{
    timer_init();
    ADC1_init();
    uart_init();
    BLE_init();
    GPIO_enable(LED1);
    GPIO_enable(ADC1_ENABLE);
    appData.ms = 0;
    appData.seconds = 0;
    appData.sampleFlag = false;
    appData.U1RxByte = 0x30;
    GPIO_lo(ADC1_ENABLE); 
}


bool getConversions(void)
{
    uint16_t i = 0;
    
    GPIO_hi(ADC1_ENABLE);
    for( i = 40 ; i > 0 ; i-- ){}

    // soil moisture conversion
    appData.soilConversion = ADC1_getConversion(ADC1_SOIL_SENSOR);

    // photo sensor conversion
    appData.liteConversion = ADC1_getConversion(ADC1_PHOTORESISTOR);           

    // temperature conversion
    appData.tempConversion = ADC1_getConversion(ADC1_TEMPERATURE);


    sprintf(U1TxBuffer,"%i,",(uint16_t)appData.soilConversion);
    uart_print(U1TxBuffer); 

    sprintf(U1TxBuffer,"%i,",(uint16_t)appData.liteConversion);   
    uart_print(U1TxBuffer);              

    sprintf(U1TxBuffer,"%i,",(uint16_t)appData.tempConversion);
    uart_print(U1TxBuffer);

    GPIO_lo(ADC1_ENABLE);                 

    return true;
}

// 
void timer_init(void)
{
    TMR1 = 0x0000;     
    T1CON = 0x8000;
    PR1 = 0x0185;
    IPC0bits.T1IP = 1;
    IFS0bits.T1IF = 0;
    IEC0bits.T1IE = 1;     
    T1CONbits.TON = 0;           
}

void ms_delay(int delay)
{
    int i, j;
    for(i = 0; i < delay; i++)
            for(j = 0; j < 4000; j++);
}