//*****************************************************************************
//
// uartstdio.h - Prototypes for the UART console functions.
//
// Copyright (c) 2007-2013 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 1.1 of the Tiva Utility Library.
//
//*****************************************************************************

#ifndef __UARTSTDIO2_H__
#define __UARTSTDIO2_H__

#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>

#include "uart.h"
#include "hw_memmap.h"
#include "hw_ints.h"
#include "sysctl.h"
#include "debug.h"
#include "rom_map.h"


//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
//
// If built for buffered operation, the following labels define the sizes of
// the transmit and receive buffers respectively.
//
//*****************************************************************************
#ifdef UART_BUFFERED2
#ifndef UART_RX_BUFFER_SIZE2
#define UART_RX_BUFFER_SIZE2     100
#endif
#ifndef UART_TX_BUFFER_SIZE2
#define UART_TX_BUFFER_SIZE2     100
#endif
#endif
	
extern unsigned long	ulFirmwareUpdateFlag;

//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************
extern void UARTStdioConfig2(uint32_t ui32Port, uint32_t ui32Baud,
                            uint32_t ui32SrcClock);
//extern int UARTgets2(char *pcBuf, uint32_t ui32Len);
extern unsigned char UARTgetc2(void);
//extern void UARTprintf(const char *pcString, ...);
//extern void UARTvprintf(const char *pcString, va_list vaArgP);
extern int UARTwrite2(const char *pcBuf, uint32_t ui32Len);
//extern int UARTwrite3(const char *pcBuf, uint32_t ui32Len);
void Flush_UARTRxBufferArray2(void );
static void UARTPrimeTransmit2(uint32_t ui32Base);
int UARTwriteNonCommandLine(const char *pcBuf, uint32_t ui32Len);
#ifdef UART_BUFFERED2
extern int UARTPeek2(unsigned char ucChar);
extern void UARTFlushTx2(bool bDiscard);
extern void UARTFlushRx2(void);
extern int UARTRxBytesAvail2(void);
extern int UARTTxBytesFree2(void);
//extern void UARTEchoSet2(bool bEnable);
extern void UARTStdioIntHandler2(void);
#endif

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __UARTSTDIO_H__
