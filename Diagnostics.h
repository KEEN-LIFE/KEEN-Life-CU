#ifndef _DIAGNOSTICS_H_
#define _DIAGNOSTICS_H_

////////////////////////////////////////////////////////////////////////////////
//! \file 
///
///  \brief 
///
///  \author 
///
///  Copyright (c) 2021 CT Control Technology .
////////////////////////////////////////////////////////////////////////////////

#define DG_023 23
#define DG_024 24
#define DG_025 25
#define DG_026 26
#define DG_027 27
#define DG_028 28
#define DG_029 29
#define DG_030 30
#define DG_031 31
#define DG_032 32
#define DG_033 33
#define DG_034 34
#define DG_040 40


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
#include "preusecheck.h"

void Self_Diagnostics(void);
void Ventilator_Test(void);

#endif
