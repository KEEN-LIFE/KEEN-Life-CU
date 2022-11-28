#ifndef _SSI_HANDLER_H_
#define _SSI_HANDLER_H_

////////////////////////////////////////////////////////////////////////////////
//! \file 
///
///  \brief 
///
///  \author 
///
///  Copyright (c) 2021 CT Control Technology .
////////////////////////////////////////////////////////////////////////////////


//---[ Includes ]---------------------------------------------------------------

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>

#include "main.h"
#include "sysctl.h"
#include "pin_map.h"
#include "ssi.h"
#include "gpio.h"
#include "uartstdio.h"
#include "interrupt.h"
#include "hw_memmap.h"
#include "hw_ints.h"
#include "hw_ssi.h"
#include "hw_types.h"
#include "ModbusCommn_Handler.h"
#include "Calib_Handler.h"




//---[ Macros ]-----------------------------------------------------------------

#define NUM_SSI_DATA            3

//SSIStdIO Library 
//*****************************************************************************
//
// If built for buffered operation, the following labels define the sizes of
// the transmit and receive buffers respectively.
//
//*****************************************************************************

#define SSI_RX_BUFFER_SIZE     100


#define SSI_TX_BUFFER_SIZE     100

#define PACKET_HEADER '$'
#define PACKET_FOOTER '#'

// FUNCTION CODE
#define SSI_READ_CMD 						0x05

//---[ Constants ]--------------------------------------------------------------


//---[ Types ]------------------------------------------------------------------


//---[ Private variables ]------------------------------------------------------


//---[ Public Variables ]-------------------------------------------------------

extern ctrlServiceReg ctrlServParam;
//extern	uint32_t ui32Status;
extern uint8_t u8SSITx[30],u8FirstByte;
extern uint8_t u8SSIDataRx[30];
extern uint32_t SSICorruptedPkt_Count,SSICorruptedPkt_Flag;


//---[ Public Function Prototypes ]---------------------------------------------


extern void Delay_Millis(unsigned int);
void SSI_Init(void);
void SSI_Communication(void);
void SSI1_Interrupt(void);

#endif

