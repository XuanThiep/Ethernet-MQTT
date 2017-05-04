
#ifndef __UART_H
#define __UART_H

#include "stdarg.h"


void UART_vPrintfSerial(const char *Format, ...);


#define APP_DEBUG(Format, ...)					UART_vPrintfSerial(Format, ##__VA_ARGS__)



#endif
