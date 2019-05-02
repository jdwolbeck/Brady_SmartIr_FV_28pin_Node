
#ifndef _UART_H
#define _UART_H

#include <stdbool.h>
#include <stdint.h>


#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif

#define MAX 40

extern char U1RxBuffer[MAX];
extern char U1TxBuffer[MAX];

extern void uart_init(void);
extern void uart_print(char[]);

#ifdef	__cplusplus
}
#endif
#endif //_UART_H
    
