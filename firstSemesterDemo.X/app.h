#ifndef _APP_H
#define _APP_H


#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif

typedef struct //
{
    uint16_t tempConversion; 
    uint16_t liteConversion; 
    uint16_t soilConversion; 
    uint16_t timer; 
    uint16_t ms; 
    uint16_t seconds;
    uint16_t delay;
    uint8_t U1RxByte;
    bool sampleFlag;
    
} APP_DATA;

extern APP_DATA appData;

    void application_init(void);
    extern bool getConversions(void);
    void ms_delay(int);
#ifdef	__cplusplus
}
#endif

#endif	/* APP_H */