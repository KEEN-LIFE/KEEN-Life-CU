/*! \file **********************************************************************
 *
 *  \brief 
 *
 
 *  \author 
 *
 *  Copyright (c) 2021 CT Control Technology .
 ******************************************************************************/


//---[ Includes ]---------------------------------------------------------------

#include "Calib_Handler.h"



//---[ Macros ]-----------------------------------------------------------------



//---[ Constants ]--------------------------------------------------------------


//---[ Types ]------------------------------------------------------------------


//---[ Private Functions ]------------------------------------------------------


//---[ Public Variables ]-------------------------------------------------------
uint32_t Air_FS = 0,O2_FS = 0, TimeStamp = 0,Dac_Val = 0,u32CalibParam[5] = {0},count = 0;;

//---[ Private Function Definitions ]-------------------------------------------


void Caliberation_Test()
{
	/*Air FS Caliberation*/
	if(!Air_FS)
		{
			O2_FS = 1;
			AirFlowSensor_Calib();
		}		

	
	/*O2 FS Caliberation*/
//	if(!O2_FS)
//		{
//			O2FlowSensor_Calib();
//		}	

	
	/*Total FS Caliberation*/
	
}

void AirFlowSensor_Calib()
{

	
	if(AirPVSetFlag_1s == FLAG_SET)
	{
		Dac_Val = count * 250;
//		Dac_Val = error_calc(count);
		DAC_Output_Set(&DAC1,DAC_CHANNEL_1,Dac_Val,LTC2607_WR_AND_UPDATE); // AIR PV SET Function
		Delay_Millis(2000);
		
		FlowSensor_dataAcq();	
		Calib_Pkt();
		count++;
		
		if(count > 100)
		{
			count = 0;
			Dac_Val = 0;
			DAC_Output_Set(&DAC1,DAC_CHANNEL_1,0,LTC2607_WR_AND_UPDATE); // AIR PV SET Function	
			TimeStamp = 0;
			Air_FS = 1;
			O2_FS =0;
			
			Delay_Millis(30000);
			
		}
		
		AirPVSetFlag_1s = 0;
	}

}

void O2FlowSensor_Calib()
{
	
	if(O2PVSetFlag_1s == FLAG_SET)
	{
		Dac_Val = count * 250;
		
		DAC_Output_Set(&DAC1,DAC_CHANNEL_2,0,LTC2607_WR_AND_UPDATE); // O2 PV SET Function	
		Delay_Millis(200);
		
		FlowSensor_dataAcq();	
		Calib_Pkt();
		count++;
		
		if(count > 100)
		{
			count = 0;
			Dac_Val = 0;
			DAC_Output_Set(&DAC1,DAC_CHANNEL_2,0,LTC2607_WR_AND_UPDATE); // O2 PV SET Function	
			TimeStamp = 0;
			O2_FS = 1;
			
		}
		
		O2PVSetFlag_1s = 0;
	}

}

void Calib_Pkt()
{
	unsigned char ucTxPktBuffer[50] = {0};
	unsigned long ulTxIndex = 0, ulServIndex = 0;
	unsigned int ulRegisterAddres = 0, ulNumRegisters = 0, ulNumRegLimit = 0, DataLength = 0, i = 0, ulTotalServRegBytes = 0;
	unsigned short crc_calculated = 0, crc_received = 0;
	
	/* Loging Data*/
	unsigned char ucLogBuffer[100] = {0};
	unsigned long ulLogIndex = 0 ;
	
	ulRegisterAddres = 0x01;	ulNumRegisters = 0x05;
	
	TimeStamp++;
//	Timestamp_Test++;
	
	u32CalibParam[0] = pack_IEEE754(Dac_Val);
	u32CalibParam[1] = pack_IEEE754(ctrlReadParam.Air_Flow);
	u32CalibParam[2] = pack_IEEE754(ctrlReadParam.O2_Flow);
	u32CalibParam[3] = pack_IEEE754(ctrlReadParam.Total_Flow);
	u32CalibParam[4] = pack_IEEE754(TimeStamp);
	
//	u32CalibParam[0] = pack_IEEE754(10);
//	u32CalibParam[1] = pack_IEEE754(15);
//	u32CalibParam[2] = pack_IEEE754(20);
//	u32CalibParam[3] = pack_IEEE754(25);
//	u32CalibParam[4] = pack_IEEE754(30);
//	
//	u32CalibParam[0] = pack_IEEE754(count);
//	u32CalibParam[1] = pack_IEEE754(ctrlReadParam.Air_Flow);
//	u32CalibParam[2] = pack_IEEE754(ctrlReadParam.O2_Flow);
//	u32CalibParam[3] = pack_IEEE754(ctrlReadParam.Total_Flow);
//	u32CalibParam[4] = pack_IEEE754(TimeStamp);
//	
//	u32CalibParam[0] = pack_IEEE754(ventAlgorithmSettings.ulAirFlow);
//	u32CalibParam[1] = pack_IEEE754(ctrlReadParam.Air_Flow);
//	u32CalibParam[2] = pack_IEEE754(Error_Test);
//	u32CalibParam[3] = pack_IEEE754(ventAlgParam.ulPVValueAir);
//	u32CalibParam[4] = pack_IEEE754(Timestamp_Test);

//	u32CalibParam[0] = pack_IEEE754(NumRxdSynPkt);
//	u32CalibParam[1] = pack_IEEE754(NumRxdNonSynPkt);
//	u32CalibParam[2] = pack_IEEE754(NumNonCorrPkt);
//	u32CalibParam[3] = pack_IEEE754(NumCorrPkt);
	
	ucTxPktBuffer[ulTxIndex++] = DU_BOARD_ID;
	ucTxPktBuffer[ulTxIndex++] = 0x15;
	ucTxPktBuffer[ulTxIndex++] = ulRegisterAddres >> 8;
	ucTxPktBuffer[ulTxIndex++] = ulRegisterAddres;
	ucTxPktBuffer[ulTxIndex++] = ulNumRegisters >> 8;
	ucTxPktBuffer[ulTxIndex++] = ulNumRegisters;	

	DataLength = ulNumRegisters * 4;
	
	for(ulTxIndex = 6 ; ulTxIndex < (DataLength + 4); ulTxIndex += 4, ulServIndex++) // Placing the individual bytes of data according Modbus protocol std structure(MSB 1st, LSB last)
	{
		ucTxPktBuffer[ulTxIndex] = (u32CalibParam[ulServIndex] >> 24);
		ucTxPktBuffer[ulTxIndex + 1] = (u32CalibParam[ulServIndex] >> 16);
		ucTxPktBuffer[ulTxIndex + 2] = (u32CalibParam[ulServIndex] >> 8);
		ucTxPktBuffer[ulTxIndex + 3] = u32CalibParam[ulServIndex];
	}
	
	crc_calculated = crc16(ucTxPktBuffer, ulTxIndex);
	ucTxPktBuffer[ulTxIndex++] = crc_calculated >> 8;
	ucTxPktBuffer[ulTxIndex++] = crc_calculated;
		
	UARTFlushTx(1);
	UARTwrite((const char *)ucTxPktBuffer, 28);	

//	UARTFlushTx(1);
//	UARTwrite((const char *)u8SSIDataRx, 20);	
}
