#ifndef _TECHNICALALARM_HANDLER_H_
#define _TECHNICALALARM_HANDLER_H_

/*! \file ********************************************************************
 * \file ADC_Handler.h
 *
 *  \brief 
 *
 *  \author 
 *
 *  Copyright (c) 2021 CT Control Technology .
 ********************************************************************/

//---[ Includes ]---------------------------------------------------------------

#include <stdint.h>
#include <stdbool.h>
#include "sysctl.h"
#include "gpio.h"
#include "adc.h"
#include "hw_memmap.h"
#include "rom.h"
#include "main.h"
#include "ModbusCommn_Handler.h"
#include "VentAlgorithm_Handler.h"


//---[ Macros ]---------------------------------------------------------------

#define VG_5V_LOWER_LIMIT			4.5  //Regulated V to ADC in mV
#define VG_5V_UPPER_LIMIT			5.5  //need to check
#define VG_18V_LOWER_LIMIT 		16.2 
#define VG_18V_UPPER_LIMIT 		19.8 
#define VG_12V_LOWER_LIMIT 		10.8 
#define VG_12V_UPPER_LIMIT 		13.2 
#define BOARD_TEMP_LIMIT 		  55    // Temp in °C 

#define GAS_SUPPLY_AIR_LOWER_LIMIT	2.0
#define GAS_SUPPLY_AIR_UPPER_LIMIT	6.0
#define GAS_SUPPLY_O2_LOWER_LIMIT		2.0
#define GAS_SUPPLY_O2_UPPER_LIMIT		6.0

#define O2_PERCENT_LOWER_LIMIT			18
#define O2_PERCENT_LOWER_CALIB_LIMIT			16
#define O2_PERCENT_UPPER_CALIB_LIMIT			105

#define INSP_PRESSURE_LOWER_LIMIT			200
#define INSP_PRESSURE_UPPER_LIMIT			1800

#define EXP_PRESSURE_LOWER_LIMIT			200
#define EXP_PRESSURE_UPPER_LIMIT			1800

#define SET_BIT           1
#define ALA_CLR						0

//---[ Types ]------------------------------------------------------------------ 



//---[ Public Variables ]-------------------------------------------------------
 
extern ctrlReadReg ctrlReadParam;
extern ctrlWriteReg ctrlWriteParam;
extern ctrlServiceReg ctrlServParam;
extern tech_alarmReg  Tech_Alarmcheck;

extern uint32_t *t_alarmptr;
extern uint32_t IntVar;
extern uint32_t Monitor_Commn_Flag, O2_FS_Commn_Flag, Air_FS_Commn_Flag, Total_FS_Commn_Flag;
extern uint32_t Insp_Pressure_Failure,Exp_Pressure_Failure,Aux_Pressure_Failure;
extern uint32_t apneacount_time,apneacount_flag,apneaalarm_flag;
extern uint32_t Time100mS_Init,Time100mS_Count;
extern uint32_t AirFS_Count,O2FS_Count, TotFS_Count,SSICorruptedPkt_Flag;
extern uint32_t AirPV_Sense_Failure, O2PV_Sense_Failure,ExhPV_Sense_Failure;
extern ctrlReadReg ctrlReadParam;
extern ctrlWriteReg ctrlWriteParam;
extern ctrlServiceReg ctrlServParam;
extern VentAlgSettings ventAlgorithmSettings;
extern VentAlgParameters ventAlgParam;

extern uint8_t VentRunFlag, VentSettingsChange;
extern uint8_t VentRunFlag , VentSettingsChange ;
extern uint32_t ulPVValueAir , ulPVValueO2 , ulPVValueInhale , ulPVValueEV ;
extern uint32_t ulInhalationFlag , ulExhalationFlag , ulPauseFlag ,ulFlowControllerFlag , ulRiseTimeFlag , ulPeepFlag ;
extern uint32_t ulInhalationCounter ,ulPuaseCounter,ulExhalationCounter , ulRiseTimeCounter;
extern float PV_SetValue_A,PV_SetValue_O,PV_SetValue;


//---[ Private Functions ]---------------------------------------------

//---[ Public Function Prototypes ]---------------------------------------------

void Technical_Alarm(void);
void PV_AlarmGeneration(void);

#endif
