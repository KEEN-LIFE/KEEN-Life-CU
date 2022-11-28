/*! \file **********************************************************************
 *
 *  \brief 
 *
 
 *  \author 
 *
 *  Copyright (c) 2021 CT Control Technology .
 ******************************************************************************/


//---[ Includes ]---------------------------------------------------------------

#include "PressureCtrlMode_Handler.h"


//---[ Macros ]-----------------------------------------------------------------

//---[ Constants ]--------------------------------------------------------------


//---[ Types ]------------------------------------------------------------------


//---[ Private Functions ]------------------------------------------------------


//---[ Public Variables ]-------------------------------------------------------

//---[ Private Function Definitions ]-------------------------------------------

void PressureCtrl_Algorithm()
{
	
			if(!ulInhalationFlag && !ulExhalationFlag)
			{
				
//			 ctrlReadParam.Insp_Pressure = ctrlWriteRead.Set_Pmax;
				ventAlgParam.PressureTrigger = 0;
				ventAlgParam.PatientTriggerWindow = 0;
				ventAlgParam.FlowTrigger = 0;
				ctrlServParam.Time_Stamp = 0;
				ctrlReadParam.Volume=0;
				ctrlServParam.Volume=0;
				apneacount_time = 0;
				RC_TimeInit=0;
				RC_TimeCount=0;
				ulRiseTimeCounter = 0;
				ulRiseTimeFlag = 0;
				ulInhalationFlag = 1;
				Volume_read=0;
				Max_E_Tidal_vol=0;
				
/* changes  added for PCM mode checking*/
				if(ctrlWriteRead.Set_Insp_Pressure < 0)ctrlWriteRead.Set_Insp_Pressure = 0;
				
				if(BREATH_CNT != 0)
				{
					check_pressure[0]=ctrlWriteRead.Set_Insp_Pressure+ctrlWriteRead.Set_PEEP;
					pressure_autocorr();			      //fuction for PID AUTO CHANGE
				}
				else
				{	
					P_K=0.1,Ti=0.035,Ts=0.02,Td=0;
					s_INSP_Pressure=10;
				} //first breath to find the resistance
				
				P_Resistance=Resistance;
				Tid_Vol=0,flow=0,p_flow=0,compliance=0;
				Peep_Flag = 0;
				fl_tri_flag=0;
				PEF=0;
//				Insp_Tidal_vol=0;//making inspiratory tidal volume zero in every starting of new breath
				peep_delay=0;
				pre_point=0;
				RISE_FLAG=0;
				pressure_flow=20000;
				P_pre_rise=pre_rise;
				pre_rise_f=0;			
				pre_rise=0;				
				P_Error=0,P_Error_2=0,P_IntegralTerm=0,P_DeriveTerm=0,P_PropTerm=0,Flow_out_1=0,Flow_out=0;
				P_Error_1=ctrlWriteRead.Set_Insp_Pressure;
				pressure_max_flow=0;
				c_k=0;
				ventAlgParam.Ppeak=0;
				ventAlgParam.Peakflow=0;
				BREATH_CNT++;
				rise_error_f=0;
/* changes  added for PCM mode checking*/				

			DAC_Output_Set(&DAC2,DAC_CHANNEL_1,22000,LTC2607_WR_AND_UPDATE);   // Exhal Valve	
			Exh_DACVal=22000;

			}
			if(ulInhalationFlag == 1)
			{
				//added by tomin for pcm control 220122
				if(ctrlReadParam.Insp_Pressure>=check_pressure[0]*0.9 && pre_rise_f ==0)
				{
					pre_rise=ulInhalationCounter;
				  pre_rise_f=1;
				}
				if(ventAlgParam.Ppeak < ctrlReadParam.Insp_Pressure)
						ventAlgParam.Ppeak = ctrlReadParam.Insp_Pressure;
				
				if(ventAlgParam.Peakflow <= ctrlServParam.Inh_Flow) //Finding Peak Flow
				{
					ventAlgParam.Peakflow = ctrlServParam.Inh_Flow;
					PCM_Flow = Flow_out_1;
				}
				
				if(ulRiseTimeCounter >= ventAlgorithmSettings.ulRiseTime)
				{
					ulRiseTimeCounter = 0;
				  ulRiseTimeFlag = 1;
				}
				
				if(ulInhalationCounter >= ventAlgorithmSettings.ulInhalationTime)
				{
					ulInhalationFlag = 0;
					ulInhalationCounter = 0;					
					ventAlgParam.ulPVValueAir=0;
					DAC_Output_Set(&DAC1,DAC_CHANNEL_1,0,LTC2607_WR_AND_UPDATE); // AIR PV SET Function
					DAC_Output_Set(&DAC1,DAC_CHANNEL_2,0,LTC2607_WR_AND_UPDATE); // O2 PV SET Function		
					ventAlgParam.ulPVValueO2=0;
					ventAlgParam.ulPVValueAir=0;					
					ulExhalationFlag = 1;
					RC_Init = 1;
				}
				
				if(ctrlReadParam.Insp_Pressure >= ctrlWriteRead.Set_Pmax)
				{

				}			
			Exh_DACVal=22000;				
			}

			if(ulExhalationFlag == 1)
			{
	
				if(BREATH_CNT == 1)
				{
						resis_calculation();
						P_compliance=compliance;
				}
				
				if(PEF < ctrlServParam.Exh_Flow )  //peak expiratory flow for Resistance calculation.
					PEF=ctrlServParam.Exh_Flow;
				
				if(ulExhalationCounter >= ventAlgorithmSettings.ulExhalationTime)
				{
					ulPeepFlag = 0;
					ulExhalationFlag = 0;
					ulExhalationCounter = 0;
					if(ctrlWriteRead.Vent_Mode==5)						
						prvcVentSettings.BreathCnt++;
					else 
						prvcVentSettings.BreathCnt=0;
					RC_Init=0;
				  RC_ConstCount=0;	
				}
			}

}
