
#include "stm32f7xx_hal.h"
#include "string.h"

#include "UART.h"


extern UART_HandleTypeDef huart1;


static void UART_vTransmit(UART_HandleTypeDef *UARTTypedef, char *Data)
{
  HAL_UART_Transmit(UARTTypedef, (uint8_t *)Data, strlen(Data), 0xFFFF);
}


//Check Type of Parameter of Function (%c, %s, %d, %x)
static void UART_vPrintf(const char *Format, va_list args)
{
  char cBuffer[256] = {0};
  unsigned char uci = 0;//, ucNumber = 0;

  for(; *Format != 0; Format++)
  {
    if(*Format == '%')
    {
      Format++;
      if(*Format == '\0') break;
      if(*Format == '%') goto getchar;
      if(*Format == '.') Format++;

      if(*Format == 'c')			//Kieu ky tu
      {
        cBuffer[uci++] = (char)va_arg(args, int);
        continue;
      }
      if(*Format == 's')			//Kieu chuoi
      {
        register char *rcStr = (char *)va_arg(args, int);
        for(unsigned char uck = 0; uck < strlen(rcStr); uck++)
        {
          cBuffer[uci++] = rcStr[uck];
        }
        continue;
      }
      if((*Format >= '0') && (*Format <= '9'))			//Kiem tra
      {
        //ucNumber = atoi((char *)Format);
        Format++;
      }

      if(*Format == 'd')			//Kieu so nguyen
      {
        char cStr[32] = {""};
        register int iNB = va_arg(args, int);

        sprintf(cStr, "%d", iNB);
        //UART_vGetNumber(cStr, iNB, ucNumber);
        for(unsigned char uck = 0; uck < strlen(cStr); uck++)
        {
          cBuffer[uci++] = cStr[uck];
        }
        continue;
      }
      if(*Format == 'x')
      {
        char cHex[17] = "0123456789ABCDEF";
        char cStr[32] = {""};
        unsigned char ucn = 0;
        register int hNB = va_arg(args, int);

        //sprintf(cStr, "0x");
        if(hNB > 0xFF)
        {
          cStr[ucn++] = cHex[hNB/0x1000];
          cStr[ucn++] = cHex[(hNB%0x1000)/0x100];
          //sprintf(cStr, "%c%c", cHex[hNB/0x1000], cHex[(hNB%0x1000)/0x100]);
        }
        hNB = hNB % 0x100;
        cStr[ucn++] = cHex[hNB/0x10];
        cStr[ucn++] = cHex[hNB%0x10];
        //sprintf(cStr, "%s%c%c", cStr, cHex[hNB/0x10], cHex[hNB%0x10]);

        for(unsigned char uck = 0; uck < strlen(cStr); uck++)
        {
          cBuffer[uci++] = cStr[uck];
        }
        continue;
      }
    }
    else
    {
      getchar: cBuffer[uci++] = *Format;
    }
  }

  if(strlen(cBuffer)) cBuffer[uci++] = '\0';
  UART_vTransmit(&huart1, (char *)cBuffer);

  return;
}

//Printf to Serial
void UART_vPrintfSerial(const char *Format, ...)
{
#if (UART_DEBUG == TRUE)
  va_list args;

  va_start(args, Format);

  UART_vPrintf(Format, args);

  va_end(args);
#endif
}



