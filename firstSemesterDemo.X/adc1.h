#ifndef _ADC1_H
#define _ADC1_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif

/*
//    ADC1_SOIL_SENSOR =  AN20, 0x14, RB8
//    ADC1_PHOTORESISTOR = AN10, 0xA, RB14
//    ADC1_TEMPERATURE =  AN11, 0xB,  RB
 */

typedef enum 
{
    ADC1_SOIL_SENSOR =  0x14,
    ADC1_PHOTORESISTOR =  0xA,
    ADC1_TEMPERATURE =  0xB,
    ADC1_MAX_CHANNEL_COUNT = 6
            
} ADC1_CHANNEL;

/**
  Driver functions
*/

void ADC1_init(void);
void ADC1_start(void);
void ADC1_stop(void);
bool ADC1_conversionComplete(void);
void ADC1_Tasks(void);

void ADC1_channelSelect( ADC1_CHANNEL channel );
uint16_t ADC1_getConversion( ADC1_CHANNEL channel );

#ifdef	__cplusplus
}
#endif

#endif //_ADC1_H
    
/**
 End of File
*/
