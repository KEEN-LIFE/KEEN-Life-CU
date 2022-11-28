/*! \file **********************************************************************
 *
 *  \brief 
 *
 
 *  \author 
 *
 *  Copyright (c) 2021 CT Control Technology .
 ******************************************************************************/
 
 /*
 CPAP mode is derived from PSV mode.
 It will maintain same pressure on both inspiration and exhalation
 The inspiration is started by a patient trigger(flow/Pressure)
 The exhaltion also started from patient effort(Flow cyclng).
 Spontaneous mode ventilation.
 
 During inspiration pressure control algorithm is using with PC above PEEP=0.
 
 During exhaltion instead full opening of exhalationn valve,I am using static value and normal control so that get sine waveform in Flow.
 
 CPAP mode identification is 0x04.
 
 CPAP mode control,we can change by adjusting the PID control variablres,which are used in Pressure control algorithm.
  
 */


//---[ Includes ]---------------------------------------------------------------

#include "cpap_mode.h"



//---[ Macros ]-----------------------------------------------------------------

//---[ Constants ]--------------------------------------------------------------


//---[ Types ]------------------------------------------------------------------


//---[ Private Functions ]------------------------------------------------------


//---[ Public Variables ]-------------------------------------------------------

uint16_t cpap_flag=0,cpap_setflag=0;

//---[ Private Function Definitions ]-------------------------------------------

void CPAP_mode(void)
{
			if(!ulInhalationFlag && !ulExhalationFlag)
			{
				
				ventAlgParam.PressureTrigger = 0;
				ventAlgParam.PatientTriggerWindow = 0;
				ventAlgParam.FlowTrigger = 0;
				ctrlServParam.Time_Stamp = 0;
				ctrlReadParam.Volume=0;
				ctrlServParam.Volume=0;
				RC_TimeInit=0;
				RC_TimeCount=0;				
				ulRiseTimeCounter = 0;
				ulRiseTimeFlag = 0;
				ulInhalationFlag = 1;
				ventAlgParam.Peakflow=0;
				apneacount_time = 0;
				/* changes  added for PCM mode checking*/
				pressure_autocorr();			      //fuction for PID AUTO CHANGE
				P_compliance=compliance;
				Tid_Vol=0,flow=0,p_flow=0,delta_pressure[1]=0,compliance=0;
				Peep_Flag = 0;
				fl_tri_flag=0;
				prvcVentSettings.BreathCnt=0;

				/* changes  added for PCM mode checking*/	
				
				DAC_Output_Set(&DAC2,DAC_CHANNEL_1,20000,LTC2607_WR_AND_UPDATE);   // Exhal Valve				
			}
			if(ulInhalationFlag == 1)
			{
				//added by tomin for pcm control 220122
				if(ctrlReadParam.Insp_Pressure>=s_INSP_Pressure*0.8 && pre_rise_f == 0)
				{
					pre_rise=ulInhalationCounter;
				  pre_rise_f=1;
				}
				if(ventAlgParam.Ppeak < ctrlReadParam.Insp_Pressure)
					ventAlgParam.Ppeak = ctrlReadParam.Insp_Pressure;
				//added by tomin for pcm control 220122
				
				/* Pressure suuport mode checking parameters */
				if(ventAlgParam.Peakflow < ctrlServParam.Inh_Flow) //Finding Peak Flow
					ventAlgParam.Peakflow = ctrlServParam.Inh_Flow;
				
				
				if(ulRiseTimeCounter >= ventAlgorithmSettings.ulRiseTime)
				{
					ulRiseTimeCounter = 0;
				  ulRiseTimeFlag = 1;
				}
				if(ctrlServParam.Inh_Flow <= ventAlgParam.Peakflow*ctrlWriteRead.Flow_Exp_trigger/100)
//				if(ulInhalationCounter >= 1000)//ventAlgorithmSettings.ulInhalationTime)
				{
					ulInhalationFlag = 0;
					ulInhalationCounter = 0;				
					DAC_Output_Set(&DAC1,DAC_CHANNEL_1,0,LTC2607_WR_AND_UPDATE); // AIR PV SET Function
					DAC_Output_Set(&DAC1,DAC_CHANNEL_2,0,LTC2607_WR_AND_UPDATE); // O2 PV SET Function	
					ventAlgParam.ulPVValueO2=0;
					ventAlgParam.ulPVValueAir=0;

					ulExhalationFlag = 1;
					RC_Init = 1;
				  s_INSP_Pressure	= ctrlWriteRead.Set_PEEP;			//make insp_pressure=Peep+(Pc above peep=0)		
				}	
			}

			if(ulExhalationFlag == 1)
			{
//				comp_calculation();//compliance calculation
					
				if( PEF < ctrlServParam.Exh_Flow )  //peak expiratory flow for Resistance calculation.
						PEF=ctrlServParam.Exh_Flow;
				
//				if(ctrlWriteRead.Pat_TrigMode !=1 && ctrlWriteRead.Pat_TrigMode !=2 )							
//					if(ulExhalationCounter >= ventAlgorithmSettings.ulExhalationTime)
//					{
//						ulPeepFlag = 0;
//						ulExhalationFlag = 0;
//						ulExhalationCounter = 0;
//						RC_Init=0;
//						RC_ConstCount=0;	
//					}
			}


}
