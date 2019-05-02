#include <xc.h>

// FBS boot segment
#pragma config BWRP = OFF // write protection
#pragma config BSS = OFF  // No boot program flash segment

// FGS  general segment code protection
#pragma config GWRP = OFF  // General segment may be written
#pragma config GCP = OFF   // No Protection

// FOSCSEL Oscillator selection configuration
#pragma config FNOSC = FRC // Fast RC Oscillator (FRC)
#pragma config IESO = ON   // internal switchover, two-speed start-up enabled

// FOSC Oscillator configuration
#pragma config POSCMOD = NONE   // Primary oscillator disabled 
#pragma config POSCFREQ = HS    // primary oscillator Hz range
#pragma config SOSCSEL = SOSCHP // secondary clock power selection
#pragma config FCKSM = CSDCMD   // clock switching disabled, clock monitor disabled

// FWDT Watchdog timer configuration
#pragma config WDTPS = PS32768  // Watchdog Timer Postscale Select bits:
#pragma config FWPSA = PR128    // WDT prescaler ratio of 1:128
#pragma config WINDIS = OFF     // Windowed Watchdog Timer Disable bit:
#pragma config FWDTEN = OFF     // WDT disabled in hardware; SWDTEN bit disabled

// FPOR
#pragma config BOREN = BOR3     // Brown-out Reset enabled in hardware, SBOREN bit disabled
#pragma config PWRTEN = ON      // Power-up Timer Enable bit on
#pragma config I2C1SEL = PRI    // Alternate I2C1 Pin Mapping bit: Use Default SCL1/SDA1 Pins For I2C1
#pragma config BORV = V18       // Brown-out Reset set to lowest voltage (1.8V)
#pragma config MCLRE = ON       // RA5 input pin disabled, MCLR pin enabled

// FICD
#pragma config ICS = PGx1       // ICD Pin Placement Select bits


#include "system.h"

void system_init(void)
{
    /****************************************************************************
     * Setting the Output Latch SFR(s)
     ***************************************************************************/
    LATA = 0x0000;
    LATB = 0x0000;

    /****************************************************************************
     * Setting the GPIO Direction SFR(s)
     ***************************************************************************/
    TRISA = 0x0011;
    TRISB = 0xFFFF;


    /****************************************************************************
     * Setting the Open Drain SFR(s)
     ***************************************************************************/
    ODCA = 0x0000;
    ODCB = 0x0000;
    
    /****************************************************************************
     * Setting the Analog/Digital Configuration SFR(s)
     ***************************************************************************/
    ANSA = 0x000F;
    ANSB = 0x0000;    

}
