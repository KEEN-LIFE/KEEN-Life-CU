#ifndef _SIMV_H_
#define _SIMV_H_

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

void simv(void);

#endif

