
/*! \file ********************************************************************
 * \file main.c
 *
 *  \brief 
 *
 *  \author 
 *
 *  Copyright (c) 2021 CT Control Technology .
 ********************************************************************/

//---[ Includes ]---------------------------------------------------------------

#include "main.h"
#include "SystemInit_Handler.h"
#include "ADC_Handler.h"
#include "FM3164_Memory.h"
#include "FM3164_WatchDog.h"
#include "LTC2607_DAC_Handler.h"
#include "SFM4200_FlowSensor.h"
#include "uartstdio.h"
#include "uartstdio1.h"
#include "uartstdio2.h"
#include "ModbusCommn_Handler.h"
#include "Scheduler_Handler.h"
#include "SSI_Handler.h"
#include "VentAlgorithm_Handler.h"
#include "TechnicalAlarm_Handler.h"
#include "Calib_Handler.h"
#include "VolumeCtrlMode_Handler.h"
#include "PressureCtrlMode_Handler.h"
#include "PressureSupport.h"
#include "cpap_mode.h"
#include "simv.h"
#include "preusecheck.h"
#include "patient_ckt_chk.h"
#include "Diagnostics.h"
#include "PRVC.h"

//---[ Macros ]-----------------------------------------------------------------

//---[ Constants ]--------------------------------------------------------------

//---[ Public Variables ]-------------------------------------------------------

FRAM_memoryInfo fRAM;
wdTimerInfo wdt;
flowSensorInfo AirFlowSensor;
flowSensorInfo O2FlowSensor;
flowSensorInfo TotalFlowSensor;
DACInfo DAC1,DAC2;

modBusCommn modBusCommnStruct;
modBusCommn modBusServMsg;

ctrlReadReg ctrlReadParam;
ctrlWriteReg ctrlWriteParam,ctrlWriteRead;
ctrlServiceReg ctrlServParam;
VentAlgSettings ventAlgorithmSettings;
VentAlgParameters ventAlgParam;
tech_alarmReg  Tech_Alarmcheck;
prvcsettings prvcVentSettings;

volatile float_t flow_read_temp;
bool Delay_Flag = 0;
uint32_t millis = 0;

uint32_t ucModbusReadReg[14] = {0}, ucModbusWriteReg[29] = {0}, ucModbusServReg[9] = {0};
uint8_t state = 0,flag_5ms=0, flag_10ms = 0,flag_50ms=0, flag_100ms = 0, flag_1s = 0,ucBufferArray[200] = {0}, flag_1ms = 0,puc_count_flag=0;
uint32_t SysTickCount_5ms=0,SysTickCount_10ms = 0,SysTickCount_50ms=0, SysTickCount_100ms, SysTickCount_1s = 0,ui32SysClock = 0,ulBuffIndex= 0;
uint32_t ModbusCount_100ms =0, flagMB_100ms =0;
uint32_t AirPVSetCount_1s = 0,O2PVSetCount_1s = 0,AirPVSetFlag_1s = 0,O2PVSetFlag_1s = 0;
uint32_t puc_count =0;

uint8_t medProc_Flag = 0;
uint32_t medProcCounter = 0;

float *readPtr,*writePtr, *servPtr;
uint32_t *t_alarmptr;

uint8_t SPulIndex_page = 0, ucWriteMultiple = 0;
uint8_t u8SSITx[30]= {0},u8SSIDataRx[30] = {0};

volatile uint32_t READ_TimeoutCounter = 0, WRITE_SIN_TimeoutCounter = 0,WRITE_MUL_TimeoutCounter = 0;
volatile uint8_t DU_CMD_InitTimerFlag = 0, WR_SIN_CMD_TimeoutFlag = 0, READ_CMD_TimeoutFlag = 0,WR_MUL_CMD_TimeoutFlag = 0;
volatile uint8_t DU_ServResp_InitTimerFlag = 0,ServResp_TimeoutFlag = 0,ServResp_Success = 0,ServResp_TimerInitFlag = 0,DU_Resp_TimeoutFlag = 0;
volatile uint32_t ServResp_TimeoutCounter = 0,DU_Resp_TimeoutCounter = 0,ServCmd_Counter,ServResp_Counter = 0;
volatile uint8_t ServCmd_TimeoutFlag = 0;
unsigned short crc_calc = 0, crc_recved = 0;
uint32_t TimeStampTest = 0;

/* Vent Algorithm*/
uint8_t VentRunFlag = 0, VentSettingsChange = 0;
uint32_t ulPVValueAir = 0, ulPVValueO2 = 0, ulPVValueInhale = 0, ulPVValueEV = 0;
uint32_t ulInhalationFlag = 0, ulExhalationFlag = 0, ulPauseFlag = 0,ulFlowControllerFlag = 0, ulRiseTimeFlag = 0, ulPeepFlag = 0;
uint32_t ulInhalationCounter = 0,ulPuaseCounter = 0,ulExhalationCounter = 0, ulRiseTimeCounter = 0;

/* Averaging*/
unsigned long u32AverageIndex = 0;
unsigned long u32AverageIndex_Exh = 0, u32AverageIndex_O2 = 0, u32AverageIndex_Air = 0, u32AverageIndex_Total = 0;
float_t  fAverageArray_Exh[10] = {0},fAverageArray_O2[10] = {0},fAverageArray_Air[10] = {0},fAverageArray_Total[10] = {0},fAverage = 0;
float_t  fAvgeArray_InspPressure[300] = {0},fAvgeArray_ExpPressure[10] = {0}, fAvg = 0,fAvgeArray_AuxPressure[10] = {0};
uint32_t u32AvgIndex_InspPressure = 0, u32AvgIndex_ExpPressure = 0,u32AvgIndex_AuxPressure = 0;

/* Alarms*/
uint32_t IntVar = 0;
uint32_t Monitor_Commn_Flag = 0, O2_FS_Commn_Flag = 0, Air_FS_Commn_Flag = 0, Total_FS_Commn_Flag = 0;
/* Technical Alarms Variables */
uint32_t AirFS_Count = 0,O2FS_Count = 0, TotFS_Count = 0,SSICorruptedPkt_Count = 0,SSICorruptedPkt_Flag = 0;
uint32_t Insp_Pressure_Failure = 0,Exp_Pressure_Failure = 0,Aux_Pressure_Failure = 0;
uint32_t AirPV_Sense_Failure = 0, O2PV_Sense_Failure = 0,ExhPV_Sense_Failure = 0;
uint32_t PV_Check = 0;

float Error_Test = 0,Timestamp_Test = 0;

float Kp = 2, Ki =0.005, Kd = 0,PropTerm = 0,IntegralTerm = 0,DeriveTerm = 0;
uint32_t  RC_ConstCount = 0,RC_Init = 0;
uint32_t Res_count=0, Res_Init=0;


float Tube_Compl = 1.7;

float P_Error = 0,P_Kp = 3500, P_Ki =20, P_Kd = 0,P_PropTerm = 0,P_IntegralTerm = 0,P_DeriveTerm = 0;//R=50,C=20
/*Patient Trigger Implementation*/
uint32_t SettlingFlag = 0, SettlingCounter = 0, SettlingTimoutFlag = 0;
uint32_t Backup_wait=0,Backup_init=0;
uint32_t apneacount_time=0,apneacount_flag=0,apneaalarm_flag=0;
/* PID Coefficient acquisition by using RC time constant value*/
uint32_t RC_TimeInit = 0,RC_TimeCount = 0, RC_TimeVar = 500;
float PCM_Flow = 0; 

/* Set Insp Time Experiment*/

//float Flow_  = 0;
uint32_t DAC_Val = 800,Time100mS_Init = 0,Time100mS_Count = 0,RiseTimeEnabled = 0;
uint8_t Peep_Flag = 0;
float RC_Time = 70,Exh_DACVal = 0;

//Exh.Flow sensor variables.
signed int f_rawdata=0;
float flowslpm = 0,EXH_FLOW=0;
uint8_t len=0,uart_array_1[8]={0},u8CRC_Cal,u8CRC_Rxd;
const uint8_t uart_array[4]={0x01,0x10,0x00,0x28};

//Valve alarm variables
float PV_SetValue_A,PV_SetValue_O,PV_SetValue;


//prvc
float prvcAvgComplainceArray[5] = {0} , prvcAvgComplaince =  0;
uint8_t prvcINDEX = 0 ;

//preusecheck variables
uint8_t puc_mode;


//---[ Global Variables ]-----------------------------------------------------


//---[ Private Functions ]------------------------------------------------------


//---[ Public Functions ]------------------------------------------------------

int main(void)
{
	
	System_Init();
	
	readPtr 		=		(float *) &ctrlReadParam;
	writePtr 		=		(float *) &ctrlWriteParam;
	servPtr 		= 	(float *)	&ctrlServParam;
	t_alarmptr 	=		(uint32_t *)&Tech_Alarmcheck;  //pointer for iterating through technical alarm variables, added: 11022022
	
	
//		Exh_DACVal = sysParam.DAC_Peep[0]*4+ sysParam.DAC_Peep[1]; //Dac vs PEEP curve
//		ventAlgorithmSettings.ulAirFlow= 8;
////		ventAlgorithmSettings.ulO2Flow=20;
//		ventAlgParam.ulPVValueAir = Air_Flow_calc(ventAlgorithmSettings.ulAirFlow);	
//		ventAlgParam.ulPVValueO2  = O2_Flow_calc( ventAlgorithmSettings.ulO2Flow);
//		DAC_Output_Set(&DAC1,DAC_CHANNEL_1,ventAlgParam.ulPVValueAir,LTC2607_WR_AND_UPDATE); // AIR PV SET Function
//		DAC_Output_Set(&DAC1,DAC_CHANNEL_2,ventAlgParam.ulPVValueO2,LTC2607_WR_AND_UPDATE); // O2 PV SET Function
//   	DAC_Output_Set(&DAC2,DAC_CHANNEL_1,Exh_DACVal,LTC2607_WR_AND_UPDATE);   // Exhal Valve
//  	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_PIN_0);
//		GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, GPIO_PIN_4);
//		GPIOPinWrite(GPIO_PORTH_BASE, GPIO_PIN_2, GPIO_PIN_2);
//  	GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, GPIO_PIN_5);
//	  int a=0;
//	
//	while(1)
//	{			
//		
//				if(flag_1ms == FLAG_SET)
//				{
//						FlowSensor_dataAcq();
//						flag_1ms=0;
//				}

//				if(flag_10ms == FLAG_SET)
//				{
//					ADC_DataAcq();
//					ExhFlowSensor();
//					flag_10ms = 0;
//				}
//}

	while(1)
	{
		
		
		if(flagMB_100ms == FLAG_SET) //checking in every 100ms
		{
			if(Modbus_Communication() != MODBUS_COMM_SUCCESSFULL) // this function receives the queries from DU and do the necessary actions
			{
				//Error Handler
			}
			flagMB_100ms=0;
		}

		if(ctrlWriteParam.State != DO_NOTHING && ctrlWriteParam.State != MAIN_STATE_STANDBY && ctrlWriteParam.State != MAIN_STATE_STOP) // checking for start / reset command
		{
			state = ctrlWriteParam.State;
		}
		else if(ctrlWriteParam.State == MAIN_STATE_STANDBY || ctrlWriteParam.State == MAIN_STATE_STOP)                        //checking for stand by command
		{
			if(!ctrlWriteRead.Set_IE_Ratio && !ctrlWriteRead.Set_RespRate && ctrlWriteParam.Vent_Mode != 3 )
			{
				state = ctrlWriteParam.State;
				ulExhalationFlag = 0;
			  ulInhalationFlag = 0;
			  ulPauseFlag=0;
			}
			else
			{
					if(!ulExhalationFlag && !ulInhalationFlag && !ulPauseFlag) //stoping of ventilator at the end of the breath
					{
						state = ctrlWriteParam.State;
					}
					if(ctrlWriteParam.Vent_Mode == SIMV) 													//stop function checking for simv mode
					{
						if(!ulSimvperiodflag && !ulInhalationFlag && !ulPauseFlag)
						{
							state = ctrlWriteParam.State;
						}
					}
				}
		}
		

//		state=0x02;
		
		
		switch(state)
		{
			case MAIN_STATE_RUN 			 : 
											stateRun();
											break;
			case MAIN_STATE_STOP 	  :
											stateStop();
											break;
			case MAIN_STATE_STANDBY  :
											stateStandBy();
											break;
			case MAIN_STATE_DIAG 		:
											stateDiagnostic();
											break;
			case MAIN_STATE_RESET 		:
											stateReset();
											break;			
			default :
											break;
		}

				
//	  PacketizingRemoteDataDebug();			
//		GPIOPinWrite(GPIO_PORTK_BASE, GPIO_PIN_3, 0x00);		
//		Delay_Millis(1);		
	
	}
		
}	

