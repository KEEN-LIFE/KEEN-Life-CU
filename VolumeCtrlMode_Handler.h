#ifndef _VOLUMECTRLMODE_HANDLER_H_
#define _VOLUMECTRLMODE_HANDLER_H_

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

#include "VentAlgorithm_Handler.h"


//---[ Macros ]-----------------------------------------------------------------

//---[ Constants ]--------------------------------------------------------------


//---[ Types ]------------------------------------------------------------------


//---[ Private variables ]------------------------------------------------------


//---[ Public Variables ]-------------------------------------------------------

extern uint32_t ulInhalationFlag, ulExhalationFlag, ulPauseFlag,ulFlowControllerFlag, ulRiseTimeFlag, ulPeepFlag;
extern float Volume_comp,InhalCount;
extern float Tid_Vol,flow,p_flow,delta_pressure[2],compliance,Resistance,Resistance_C,P_Resistance,P_compliance,Insp_Tidal_vol;
extern float Exh_Error,Exh_Error_1,Exh_Error_2,Exh_DACVal_1,Exh_DACVal_0,E_K,E_Ts,E_Ti,E_Td;
extern float measurement,PEF,PCM_re,Pre_Peak,Pre_PCM_re,s_INSP_Pressure,re_50;
extern float F_Error[2],F_Error_1[2],F_Error_2[2],F_PropTerm,F_IntegralTerm,F_DeriveTerm,F_DAC_out[2],F_DAC_out_1[2];
extern float F_K,F_Ts,F_Ti,F_Td;
extern uint8_t peep_delay;
extern  float Flow_trigger_offset;
extern float Max_E_Tidal_vol,Volume_read,Insp_Tidal_vol_A;
//---[ Public Function Prototypes ]---------------------------------------------

void VolCtrl_Algorithm(void);
#endif

