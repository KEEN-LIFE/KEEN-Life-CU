#ifndef _SYSTEMINIT_HANDLER_H_
#define _SYSTEMINIT_HANDLER_H_

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
#include "LTC2607_DAC_Handler.h"
#include "FM3164_Memory.h"
#include "SFM4200_FlowSensor.h"
#include "SSI_Handler.h"
#include "ADC_Handler.h"

//---[ Macros ]-----------------------------------------------------------------

//---[ Constants ]--------------------------------------------------------------


//---[ Types ]------------------------------------------------------------------


//---[ Private variables ]------------------------------------------------------


//---[ Public Variables ]-------------------------------------------------------

extern DACInfo DAC1,DAC2;
extern ctrlReadReg ctrlReadParam;
extern ctrlWriteReg ctrlWriteParam;
extern ctrlServiceReg ctrlServParam;
extern uint32_t ui32SysClock,millis;
extern bool Delay_Flag;
extern uint32_t SysTickCount_5ms,SysTickCount_10ms,SysTickCount_50ms, SysTickCount_100ms, SysTickCount_1s;
extern uint8_t flag_5ms,flag_10ms, flag_50ms,flag_100ms, flag_1s, flag_1ms;
extern volatile uint32_t ServResp_TimeoutCounter,DU_Resp_TimeoutCounter,ServCmd_Counter,ServResp_Counter;
extern volatile uint8_t ServCmd_TimeoutFlag;
extern uint32_t AirPVSetCount_1s,O2PVSetCount_1s,AirPVSetFlag_1s,O2PVSetFlag_1s;
extern uint32_t ulSimvperiodcounter,ulSpontperiodcounter,puc_count;
extern uint8_t  ulSimvperiodflag,ulSpontperiodflag,patienttriggerflag,puc_count_flag;
extern uint32_t Res_count, Res_Init;
extern uint32_t ModbusCount_100ms, flagMB_100ms;

//medical procedure counter variables
extern uint32_t medProcCounter ;
extern uint8_t medProc_Flag ; 
//---[ Public Function Prototypes ]---------------------------------------------

void Delay_ms(unsigned int ms);
void System_Init(void);
void PacketizingRemoteDataDebug(void);
void DIO_Init(void);
void I2C_Init(void);
void UART_Init(void);
void FRAMInit(void);
void FlowSensorInit(void);
void DAC_Init(void);
void Delay_Millis(uint32_t);
void SysTick_Timer(void);
void SysTick_Handler(void);

#endif

