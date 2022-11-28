#ifndef _PREUSECHECK_H_
#define _PREUSECHECK_H_

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
#include "TechnicalAlarm_Handler.h"

#define DO_NOTHING			 0x00
#define GAS_SUPPPLY_CHK  0x01
#define VALVE_CHK				 0x02
#define FLOW_SENSOR_CHK	 0x03
#define O2_SENSOR_CHK		 0x04
#define PRE_SENSOR_CHK   0x05
#define SAF_RLF_VALVE	   0x06
#define AUX_RLF_VALVE		 0x07
#define BAT_TEST				 0x08

#define CHK_TIME				 5000

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
extern uint8_t puc_mode;

void preusecheck(void);
void Gas_supply(void);
void Valve_check(void);
void Flow_Sensor_check(void);
void O2_Sensor_check(void);
void Pre_Sensor_check(void);
void Safety_Valve_check(void);
void Aux_Valve_check(void);
void Battery_check(void);
void Normal_state(void);

#endif
