#include <xc.h>
#include "app.h"
#include "adc1.h"
#include <stdbool.h>
#include <stdint.h>

#define ADC1_INPUTS 0x6100  // PORTB 8, 13, and 14

/**********************************************************
 * 
 * ADC Driver functions
 * 
*************************************************************/
void ADC1_init(void);
void ADC1_start(void);
void ADC1_stop( void );
bool ADC1_conversionComplete( void );
void ADC1_channelSelect( ADC1_CHANNEL );
uint16_t ADC1_getConversion( ADC1_CHANNEL  );
void ADC1_Tasks ( void );

uint16_t ctr = 0;
/****************************************************************
 * 
 * ADC1_Initialize must be call before all other ADC functions
 *  
 ***************************************************************/
void ADC1_init(void)
{
    
        AD1CON2bits.PVCFG = 0x0;
        AD1CON3bits.ADCS = 0xFF;
        AD1CON3bits.SAMC = 0xF;       
        AD1CON1bits.SSRC = 0x0;
        AD1CON1bits.FORM = 0;
        AD1CON2bits.SMPI = 0x0;
        AD1CON1bits.ADON = 1;
        ANSB |= ADC1_INPUTS;
        TRISB |= ADC1_INPUTS;   
}

void ADC1_start(void)
{
   AD1CON1bits.SAMP = 1;
   for( ctr = 1000; ctr  > 0; ctr--){}
   
}

void ADC1_stop( void )
{
   AD1CON1bits.SAMP = 0;
}


bool ADC1_conversionComplete( void )
{
    return AD1CON1bits.DONE; //Wait for conversion to complete   
}


void ADC1_channelSelect( ADC1_CHANNEL channel )
{
    AD1CHS = channel;
}

uint16_t ADC1_getConversion( ADC1_CHANNEL channel )
{

    ADC1_channelSelect(channel);
    
    ADC1_start();
    
    ADC1_stop();
    
    while (!ADC1_conversionComplete()){}
    
    return ADC1BUF0;
}


/**
  End of File
*/
