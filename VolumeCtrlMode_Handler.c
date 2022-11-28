/*! \file **********************************************************************
 *
 *  \brief 
 *
 
 *  \author 
 *
 *  Copyright (c) 2021 CT Control Technology .
 ******************************************************************************/


//---[ Includes ]---------------------------------------------------------------

#include "VolumeCtrlMode_Handler.h"

//---[ Macros ]-----------------------------------------------------------------

//---[ Constants ]--------------------------------------------------------------


//---[ Types ]------------------------------------------------------------------


//---[ Private Functions ]------------------------------------------------------


//---[ Public Variables ]-------------------------------------------------------

//---[ Private Function Definitions ]-------------------------------------------

void VolCtrl_Algorithm()
{
	
			if(!ulInhalationFlag && !ulExhalationFlag && !ulPauseFlag)
			{

//				Volume_comp += (Tube_Compl * ventAlgParam.Ppeak);
//				ventAlgorithmSettings.ulInhalationFlow = (Volume_comp/ventAlgorithmSettings.ulInhalationTime)*60;
//				ventAlgorithmSettings.ulAirFlow = ventAlgorithmSettings.ulInhalationFlow*(100-ctrlWriteParam.Set_FI02)/79;  	
//				ventAlgorithmSettings.ulO2Flow = ventAlgorithmSettings.ulInhalationFlow-ventAlgorithmSettings.ulAirFlow;			
//			  Volume_comp=ctrlWriteRead.Set_TidalVol;
				

				ventAlgParam.PressureTrigger = 0;
				ventAlgParam.PatientTriggerWindow = 0;
				ventAlgParam.FlowTrigger = 0;
				ctrlServParam.Time_Stamp = 0;
				apneacount_time = 0;
				ctrlReadParam.Volume=0;
				ctrlServParam.Volume=0;
				RC_TimeInit=0;
				RC_TimeCount=0;
				
				
				TimeStampTest = 0;
				ventAlgParam.Ppeak=0;
				Timestamp_Test = 0;
				InhalCount++;
				Volume_read=0;
				Max_E_Tidal_vol=0;
				Insp_Tidal_vol_A=0;
				P_Resistance=Res_count/(1*compliance);
				Tid_Vol=0,flow=0,p_flow=0,delta_pressure[0]=0,compliance=0;
				Peep_Flag = 0;
				Res_count = 0;
				
				if(!F_Error[0] && !F_Error[1])
				{
					F_DAC_out_1[0]=ventAlgorithmSettings.ulAirFlow;
					F_DAC_out_1[1]=ventAlgorithmSettings.ulO2Flow;
				}
				
//				Insp_Tidal_vol=0;//making inspiratory tidal volume zero in every starting of new breath
				peep_delay=0;

				
				ulRiseTimeCounter = 0;
				ulRiseTimeFlag = 0;
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
				
				if(ulRiseTimeCounter >= ventAlgorithmSettings.ulRiseTime)
				{
					ulRiseTimeCounter = 0;
				  ulRiseTimeFlag = 0;
				}
				
				if(ulInhalationCounter >= ventAlgorithmSettings.ulInhalationTime)
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
				if(ctrlReadParam.Insp_Pressure >= ctrlWriteParam.Set_Pmax)
				{

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

				if(ulPuaseCounter >= ventAlgorithmSettings.ulPauseTime)
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
				
//				if( PEF < ctrlServParam.Exh_Flow )  //peak expiratory flow for Resistance calculation.
//						PEF=ctrlServParam.Exh_Flow;
//				if(BREATH_CNT == 1)
//				{
//						resis_calculation();
//						P_compliance=compliance;
//				}

					if(ulExhalationCounter >= ventAlgorithmSettings.ulExhalationTime)
					{
						ulPeepFlag = 0;
						ulExhalationFlag = 0;
						ulExhalationCounter = 0;
						RC_ConstCount	= 0;
						RC_Init = 0;
						if(ctrlWriteRead.Vent_Mode==5)						
							prvcVentSettings.BreathCnt++;
						else 
							prvcVentSettings.BreathCnt=0;
						RC_TimeInit=0;
						RC_TimeCount=0;
					}
			}		
			

}