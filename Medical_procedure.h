#ifndef _MEDICAL_PROCEDURE_H_
#define _MEDICAL_PROCEDURE_H_

////////////////////////////////////////////////////////////////////////////////
//! \file 
///
///  \brief 
///
///  \author 
///
///  Copyright (c) 2021 CT Control Technology .
////////////////////////////////////////////////////////////////////////////////

#define INSP_HOLD  1
#define EXP_HOLD   2
#define MAN_BREATH 3
#define MAX_HOLD_TIME 300000


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

extern uint32_t ulInhalationFlag, ulExhalationFlag, ulPauseFlag,ulFlowControllerFlag, ulRiseTimeFlag, ulPeepFlag;
extern float Volume_comp,InhalCount;
extern float Tid_Vol,flow,p_flow,compliance,Resistance,Resistance_C,P_Resistance,P_compliance,Insp_Tidal_vol;
extern float Exh_Error,Exh_Error_1,Exh_Error_2,Exh_DACVal_1,Exh_DACVal_0,E_K,E_Ts,E_Ti,E_Td;
extern float measurement,PEF,PCM_re,Pre_Peak,Pre_PCM_re,s_INSP_Pressure,re_50;
extern float F_Error[2],F_Error_1[2],F_Error_2[2],F_PropTerm,F_IntegralTerm,F_DeriveTerm,F_DAC_out[2],F_DAC_out_1[2];
extern float F_K,F_Ts,F_Ti,F_Td;
extern int trigger_time,fl_tri_flag;

extern float	P_Error_1,P_Error_2,Flow_out_1,Flow_out;
extern float P_K,Ti,Ts,Td;
extern float measurement,PEF,PCM_re,Pre_Peak,Pre_PCM_re,s_INSP_Pressure,re_50;
extern float avg_pre,AVG_PRESSURE,pre_point,pre_poit_2,pre_rise,P_pre_rise;
extern float pressure_flow;
extern uint16_t pre_rise_f,corr_f,corr_f_chk;
extern float P_peak;
extern uint8_t RISE_FLAG;
extern float pressure_max_flow;
extern float check_pressure[2],diff_pressure[2],delta_pressure[2];

extern uint16_t c_k; //pcm error array index variable for testing

extern uint32_t BREATH_CNT;
extern uint16_t	rise_error_f;
extern float Max_E_Tidal_vol,Volume_read;

extern uint32_t medProcCounter ;
extern uint8_t medProc_Flag ;


void medicalProce(void);
void InspiratoyHold(void);
void ExpiratoryHold(void);
void ManualBreath(void);

#endif