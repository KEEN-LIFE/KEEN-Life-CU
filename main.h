#ifndef _MAIN_H_
#define _MAIN_H_

/*! \file ********************************************************************
 * \file main.h
 *
 *  \brief API for exported functions in main.c for KEEN Ventilator V2
 *
 *  \author 
 *
 *  Copyright (c) 2021 CT Control Technology .
 ********************************************************************/

//---[ Includes ]---------------------------------------------------------------

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>

#include "CRC.h"
#include "CRC16.h"
#include "i2c.h"
#include "rom.h"
#include "sysctl.h"
#include "systick.h"
#include "pin_map.h"
#include "timer.h"
#include "adc.h"
#include "uart.h"
#include "rom_map.h"
#include "interrupt.h"
#include "debug.h"
#include "ssi.h"
#include "uart.h"
#include "gpio.h"
#include "timer.h"

#include "hw_types.h"
#include "hw_gpio.h"
#include "hw_uart.h"
#include "hw_ints.h"
#include "hw_memmap.h"
#include "uartstdio2.h"


//---[ Macros ]---------------------------------------------------------------

// Scheduler States
#define MAIN_STATE_STOP 		0x03
#define MAIN_STATE_RUN 			0x02
#define MAIN_STATE_STANDBY 	0x01
#define MAIN_STATE_DIAG 		0x04
#define MAIN_STATE_RESET 		0x05

#define FLAG_SET 0x01




//---[ Types ]------------------------------------------------------------------ 


typedef struct _ctrlReadReg
{
	float O2_Flow;
	float Air_Flow;
	float Total_Flow;
	float Exh_Flow;
	float Volume;
	float Insp_Pressure;
	float Exp_Pressure;
	float Aux_Pressure;
	float VG_Sense_5V;
	float VG_Sense_18V;
	float VG_Sense_12V;
	float O2_PV_Sense;
	float Air_PV_Sense;
	float EV_PV_Sense;
	float Ins_Zero_Valve_Sense;
	float Aux_Valve_Sense;
	float Ins_Effort_Valve_Sense;
	float Board_Temp;
	uint32_t Status;
	float Ctrl_DU_Comm;
}ctrlReadReg;

typedef struct _ctrlWriteReg
{
	float State;
	float Vent_Mode;
	float Set_FI02;
	float Set_PEEP;
	float Set_RespRate;
	float Set_TidalVol;
	float Set_InspTime;
	float Set_IE_Ratio;
	float Set_PauseTime;
	float Set_RiseTime;
	float Set_Pmax;
	float Pat_TrigMode;
	float Pres_TrigSenstvty;
	float Flow_TrigSenstvty;
	float Backup_RespRate;
	float Apnea_Time;
	float Insp_Flow;
	float Bias_Flow;
	float Set_Insp_Pressure;
	float Preusecheck;
	float Flow_Exp_trigger;
	float breath_cycle;
	float Set_pressuresupport;   //pc above peep for pressure support mode
	float SIMV_mandatory;				//selecting mandatory breath for simv
	float Diagnostics;
	float Set_AirFlow;
	float Set_O2Flow;
	float Set_AirValve;
	float Set_O2Valve;
	float Set_ExhValve;
	float Ins_Zero_Valve;
	float Aux_Valve;
	float Ins_Effort_Valve;
	float Medical_proc;
	float Hold_time;

}ctrlWriteReg;

typedef struct _ctrlServiceReg
{
	float Inh_Flow;
	float Exh_Flow;
	float Volume;
	float Insp_Pressure;
	float Exp_Pressure;
	float Aux_Pressure;
	uint32_t Status_Reg;
	float O2_Percent;
	float Time_Stamp;	
}ctrlServiceReg;

//This structure is defined for cecking and assigning technical alaram  flag
//created on 11th March 2022  by Tomin
typedef struct _tech_alarmReg
{
		uint32_t o2_flow_sensor; 			//1
		uint32_t air_flow_sensor; 		//2
		uint32_t total_flow_sensor;		//3
		uint32_t insp_pr_sensor; 			//4
		uint32_t exp_pre_sensor; 	    //5
		uint32_t aux_pr_sensor; 			//6
		uint32_t o2_valve; 						//7
		uint32_t air_valve; 					//8
		uint32_t exhal_valve; 				//9
		uint32_t insp_zero_valve; 		//10
		uint32_t aux_purge_valve; 		//11
		uint32_t exh_flow_sensor; 		//12
		uint32_t pre_diff; 					  //13
		uint32_t insp_safe_relief; 		//14
		uint32_t aux_safe_relief; 		//15
		uint32_t puc_update; 					//16
		uint32_t vg_5v_sense; 				//17
		uint32_t vg_18v_sense; 				//18
		uint32_t vg_12v_sense; 				//19
		uint32_t board_temp; 					//20
		uint32_t moni_comm; 					//21
		uint32_t prog_crc; 						//22
		uint32_t data_crc; 						//23
		uint32_t wdt_reset; 					//24
		uint32_t apnea_ventilation; 	//25
		uint32_t Self_Diagnostics;		//26
		uint32_t Insp_Effort_Valve;		//27
		uint32_t MedprocUpdate;		  	//28
		uint32_t Reserved_9; 					//29
		uint32_t Reserved_10; 				//30
		uint32_t Reserved_11; 				//31
		uint32_t Reserved_12; 				//32
}tech_alarmReg;

//---[ Public Variables ]-------------------------------------------------------

extern volatile uint32_t ulADC0_Value1[8];
extern volatile	uint32_t ulADC0_Value2[4];
extern volatile	uint32_t O2_Pressure_Pa;
extern volatile	uint32_t O2_Pressure_cmH2O;
extern uint8_t FlowSensorReset;

//---[ Private Functions ]---------------------------------------------

//---[ Public Function Prototypes ]---------------------------------------------

void Delay_ms(unsigned int ms);
void System_Init(void);
void PacketizingRemoteDataDebug(void);
//uint8_t UART_testing();
void InitConsole(void);
//void SSI0_Interrupt(void);
void DIO_Init(void);
void I2C_Init(void);
void UART_Init(void);
void FRAMInit(void);
void FlowSensorInit(void);
void 	DAC_Init(void);



uint32_t floatToHex(float);
float IEEE_HexToFloatPt(int32_t);

void Delay_Millis(uint32_t);
void Delay_MicroSeconds(unsigned int microsec);
void SysTick_Timer(void);
void SysTick_Handler(void);
void UART_RX_Test(void);

#endif

