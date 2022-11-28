#ifndef _PRESSURESUPPORT_H_
#define _PRESSURESUPPORT_H_

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

extern uint8_t RISE_FLAG;
extern float pressure_max_flow;
extern float check_pressure[2],diff_pressure[2],delta_pressure[2];
extern float measurement,PEF,PCM_re,Pre_Peak,Pre_PCM_re,ps_INSP_Pressure,re_50;
extern float Max_E_Tidal_vol,Volume_read;

//---[ Public Variables ]-------------------------------------------------------

void pressuresupport(void);

#endif

