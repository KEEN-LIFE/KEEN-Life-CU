#ifndef _CALIB_HANDLER_H_
#define _CALIB_HANDLER_H_

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
#include "SFM4200_FlowSensor.h"
#include "LTC2607_DAC_Handler.h"
#include "ModbusCommn_Handler.h"
#include "VentAlgorithm_Handler.h"

//---[ Macros ]-----------------------------------------------------------------

//---[ Constants ]--------------------------------------------------------------


//---[ Types ]------------------------------------------------------------------


//---[ Private variables ]------------------------------------------------------


//---[ Public Variables ]-------------------------------------------------------

extern uint32_t AirPVSetFlag_1s,O2PVSetFlag_1s;
extern float Error_Test,Timestamp_Test;
extern uint32_t TimeCount ;
extern float 	NumCorrPkt,NumNonCorrPkt,NumRxdSynPkt,NumRxdNonSynPkt;
extern uint8_t u8SSITx[30],u8SSIDataRx[30];
//extern VentAlgSettings ventAlgorithmSettings;
//extern VentAlgParameters ventAlgParam;

//---[ Public Function Prototypes ]---------------------------------------------

void Caliberation_Test(void);
void AirFlowSensor_Calib(void);
void O2FlowSensor_Calib(void);
void Calib_Pkt(void);

#endif

