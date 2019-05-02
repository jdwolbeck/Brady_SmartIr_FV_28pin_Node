
/*---------------------------------------------------------------------
 * Brady Knaeble 
 * 
 * Devices:
 * PIC24FV16KM202 I-SO and/or SOIC wide
 * RN4871 RM
 * RN42 BlueSmirf Bluetooth module
 * momentary switch to drive external interrupt 0
 * teraTerm was used to write to, and read from the UART buffers
 *-----------------------------------------------------------------------*/
/* Device header file */

#include <xc.h>
#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>       /* Includes true/false definition                  */
#include "bluetooth.h"
#include "system.h"
#include "app.h"

int main() {

    system_init();
    application_init();
    while (1) 
    {     
        ms_delay(10);
        //getConversions();
        node_application();        
    }
    
    return 0;
}
