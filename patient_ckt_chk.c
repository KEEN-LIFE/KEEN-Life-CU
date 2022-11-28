/*! \file **********************************************************************
 *
 *  \brief 
 *
 
 *  \author 
 *
 *  Copyright (c) 2021 CT Control Technology .
 ******************************************************************************/


//---[ Includes ]---------------------------------------------------------------

#include "patient_ckt_chk.h"

//---[ Macros ]-----------------------------------------------------------------


//---[ Constants ]--------------------------------------------------------------


//---[ Types ]------------------------------------------------------------------

//---[ Private Functions ]------------------------------------------------------

void patient_ckt_chk(void)
{
	
			ventAlgorithmSettings.ulInhalationTime = INH_TIME;
			ventAlgorithmSettings.ulPauseTime			 = HLD_TIME;
			ventAlgorithmSettings.ulExhalationTime = EXH_TIME;
			ventAlgorithmSettings.ulAirFlow				 = P_FLOW;
			ventAlgorithmSettings.ulO2Flow 				 = 0;
			
			VentRunFlag = 0;
	
			if(!ulInhalationFlag && !ulExhalationFlag && !ulPauseFlag)
			{
				ctrlServParam.Time_Stamp = 0;
				peep_delay = 0;
				Insp_Tidal_vol =0;
				Tid_Vol=0;
				Volume_read=0;
				Max_E_Tidal_vol=0;
				ctrlServParam.Volume = 0;
				ctrlReadParam.Volume = 0;

				ulInhalationFlag = 1;

				DAC_Output_Set(&DAC2,DAC_CHANNEL_1,22000,LTC2607_WR_AND_UPDATE);   // Exhal Valve
				DAC_Output_Set(&DAC1,DAC_CHANNEL_1,0,LTC2607_WR_AND_UPDATE); // AIR PV SET Function
				DAC_Output_Set(&DAC1,DAC_CHANNEL_2,0,LTC2607_WR_AND_UPDATE); // O2 PV SET Function					
				ventAlgParam.ulPVValueO2=0;
				ventAlgParam.ulPVValueAir=0;
				Exh_DACVal = 22000;
			  ventAlgParam.ulPVValueEV=22000;

			}
			if(ulInhalationFlag == 1)
			{
				
				
				
				if(ulInhalationCounter >= INH_TIME)
				{
					ulInhalationFlag = 0;
					ulInhalationCounter = 0;
					
					if(ventAlgParam.Ppeak < ctrlReadParam.Insp_Pressure)
					 ventAlgParam.Ppeak = ctrlReadParam.Insp_Pressure;

					DAC_Output_Set(&DAC1,DAC_CHANNEL_1,0,LTC2607_WR_AND_UPDATE); // AIR PV SET Function
					DAC_Output_Set(&DAC1,DAC_CHANNEL_2,0,LTC2607_WR_AND_UPDATE); // O2 PV SET Function					
					ulPauseFlag = 1;
					ventAlgParam.ulPVValueO2=0;
					ventAlgParam.ulPVValueAir=0;
					Exh_DACVal = 22000;
					ventAlgParam.ulPVValueEV=22000;		
				}	
			}
			
			if(ulPauseFlag == 1)
			{
				
				DAC_Output_Set(&DAC2,DAC_CHANNEL_1,22000,LTC2607_WR_AND_UPDATE);   // Exhal Valve
				DAC_Output_Set(&DAC1,DAC_CHANNEL_1,0,LTC2607_WR_AND_UPDATE); // AIR PV SET Function
				DAC_Output_Set(&DAC1,DAC_CHANNEL_2,0,LTC2607_WR_AND_UPDATE); // O2 PV SET Function					
				ventAlgParam.ulPVValueO2=0;
				ventAlgParam.ulPVValueAir=0;
				ventAlgParam.ulPVValueEV=22000;
				Exh_DACVal = 22000;

				if(ulPuaseCounter >= HLD_TIME)
				{
					ulPauseFlag = 0;
					ulPuaseCounter = 0;
					ventAlgParam.Pplatue = ctrlReadParam.Insp_Pressure ;
					ulExhalationFlag = 1;
					RC_Init = 1;
				}
			}
			
			if(ulExhalationFlag == 1)
			{				
					if(ulExhalationCounter >= EXH_TIME)
					{
						ulPeepFlag = 0;
						ulExhalationFlag = 0;
						ulExhalationCounter = 0;
					}
			}		

	
}