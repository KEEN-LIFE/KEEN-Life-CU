#ifndef _SFM4200_FLOWSENSOR_H_
#define _SFM4200_FLOWSENSOR_H_

////////////////////////////////////////////////////////////////////////////////
//! \file SFM4200_FlowSensor.h
///
///  \brief API for SFM4200_FlowSensor functions
///
///  \author 
///
///  Copyright (c) 2021 CT Control Technology .
////////////////////////////////////////////////////////////////////////////////


//---[ Includes ]---------------------------------------------------------------

#include <stdint.h>
#include <math.h>
#include <stdbool.h>

#include "i2c.h"
#include "rom.h"
#include "main.h"
#include "uartstdio1.h"
#include "FM3164_Memory.h"
#include "VentAlgorithm_Handler.h"

//---[ Macros ]-----------------------------------------------------------------

#define SFM4200_SLAVE_ADDR 0x40

#define O2_FLOW			0
#define AIR_FLOW		1
#define TOTAL_FLOW	2
#define EXHALE_FLOW 3

//---[ Constants ]--------------------------------------------------------------


//---[ Types ]------------------------------------------------------------------
typedef enum
{
    FLOW_READ_OK       = 0x00,
    FLOW_READ_CRC_CHECK_ERR = 0x01,
	  FLOW_READ_I2C_MASTER_CLK_TOUT = 0x02
}flowSensorStatus;


typedef struct _flowSensorInfo
{
	uint32_t i2c_base;
	volatile uint32_t flowRead;
	volatile uint32_t flowSensor_err_status; 
	volatile uint8_t i2c_rxBuffer[3];
	uint16_t flowSensorError_cnt;

}flowSensorInfo;

//---[ Public Variables ]-------------------------------------------------------

extern flowSensorStatus flowErrorStatus;
extern flowSensorInfo AirFlowSensor;
extern flowSensorInfo O2FlowSensor;
extern flowSensorInfo TotalFlowSensor;

extern ctrlReadReg ctrlReadParam;
extern ctrlServiceReg ctrlServParam;
extern volatile float_t flow_read_temp;
extern calibrationvalues sysParam;
extern calibrationvalues userParam;

//extern volatile uint32_t flow_read[3];

extern unsigned long ulFlowSLPM[4];


extern unsigned long u32Average , u32AverageIndex;
extern unsigned long u32AverageIndex_Exh, u32AverageIndex_O2, u32AverageIndex_Air, u32AverageIndex_Total;
extern float_t  fAverageArray_Exh[10],fAverageArray_O2[10],fAverageArray_Air[10],fAverageArray_Total[10],fAverage;
extern uint32_t Monitor_Commn_Flag, O2_FS_Commn_Flag, Air_FS_Commn_Flag, Total_FS_Commn_Flag;
extern uint32_t AirFS_Count,O2FS_Count, TotFS_Count;
extern uint32_t SysTickCount_5ms,SysTickCount_10ms,SysTickCount_50ms, SysTickCount_100ms, SysTickCount_1s;
extern uint8_t flag_5ms,flag_10ms, flag_50ms,flag_100ms, flag_1s, flag_1ms;

extern signed int f_rawdata;
extern float flowslpm ;
extern uint8_t len,uart_array_1[8],u8CRC_Cal,u8CRC_Rxd;
extern const uint8_t uart_array[4];
extern uint8_t crc8CheckUpdateSF05(uint8_t data[], uint8_t bytelength,uint8_t checksum);

//---[ Public Function Prototypes ]---------------------------------------------

flowSensorStatus sfm4200_flow_read(flowSensorInfo *,float_t *);
//flowSensorStatus flowSensorErrorHandler(uint32_t);
void FlowSensorSerialPacketization(void);
void FlowSensor_dataAcq(void);



//Exhalation FS Testing
void FlowSensorDAQ_RS485(unsigned int uChannels);
void FlowDataProcessing(unsigned int uChannels);
unsigned int FlowCommandSend(unsigned int uSlaveId);
void ExhFlowSensor(void);

//---[ Inline Functions ]-------------------------------------------------------




#endif
