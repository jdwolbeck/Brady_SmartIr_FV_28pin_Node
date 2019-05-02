#include <xc.h> 
#include <stdbool.h>
#include <stdint.h>
#include "app.h"
#include "gpio.h"

/**********************************************************
 * 
 * masks
 * 
*************************************************************/

#define ADC1_ENABLE 0x0020     // PORTB5
#define LED1        0x0040     // PORTB6 

void GPIO_enable( uint16_t );
void GPIO_lo( uint16_t );
void GPIO_hi( uint16_t );

void GPIO_enable( uint16_t portSelect )
{   
    switch( portSelect )
    {
        case LED1 :
            ANSB &= ~(LED1);
            TRISB &= ~(LED1);
            LATB &= ~(LED1);
            break;
        case ADC1_ENABLE :
            ANSB &= ~(ADC1_ENABLE);
            TRISB &= ~(ADC1_ENABLE);
            LATB |= (ADC1_ENABLE);
            break;
            
        default:
            break;
    }
    
}

void GPIO_lo( uint16_t portSelect)
{
    LATB &= ~(portSelect);
}
void GPIO_hi( uint16_t portSelect )
{
    LATB |= (portSelect);    
}




