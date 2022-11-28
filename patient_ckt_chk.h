#ifndef _PATIENT_CKT_CKHK_H_
#define _PATIENT_CKT_CKHK_H_

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

#include <stdint.h>
#include <stdint.h>
#include <stdbool.h>
#include "sysctl.h"
#include "gpio.h"
#include "adc.h"
#include "hw_memmap.h"
#include "rom.h"
#include "main.h"
#include "ModbusCommn_Handler.h"
#include "uartstdio.h"
#include "LTC2607_DAC_Handler.h"
#include "FM3164_Memory.h"
#include "VentAlgorithm_Handler.h"

#define INH_TIME     500       //ms
#define HLD_TIME		 1000			 //ms
#define EXH_TIME		 1500      //ms

#define VOL					 100       //ml
#define P_FLOW			 12        //lpm




extern uint8_t state, flag_10ms, flag_100ms, flag_1s , flag_4s;
extern volatile uint8_t Status_Check;
extern ctrlReadReg ctrlReadParam;
extern ctrlWriteReg ctrlWriteParam;
extern ctrlServiceReg ctrlServParam;
extern uint8_t VentRunFlag, VentSettingsChange,puc_count_flag;
extern tech_alarmReg  Tech_Alarmcheck;
extern calibrationvalues sysParam;
extern calibrationvalues userParam;
extern float PV_SetValue_A,PV_SetValue_O,PV_SetValue;
extern uint32_t Insp_Pressure_Failure,Exp_Pressure_Failure ,Aux_Pressure_Failure;
extern uint32_t AirPV_Sense_Failure,O2PV_Sense_Failure,ExhPV_Sense_Failure;


extern uint32_t ulPVValueAir, ulPVValueO2, ulPVValueInhale, ulPVValueEV,puc_count;
extern DACInfo DAC1,DAC2;
extern uint32_t ulInhalationFlag, ulExhalationFlag, ulPauseFlag,ulFlowControllerFlag, ulRiseTimeFlag, ulPeepFlag;
extern uint32_t ulInhalationCounter,ulPuaseCounter,ulExhalationCounter, ulRiseTimeCounter;



void patient_ckt_chk(void);


#endif