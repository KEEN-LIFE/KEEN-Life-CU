/*! \file **********************************************************************
 *
 *  \brief 
 *
 
 *  \author 
 *
 *  Copyright (c) 2021 CT Control Technology .
 ******************************************************************************/


//---[ Includes ]---------------------------------------------------------------

#include "PRVC.h"



//---[ Macros ]-----------------------------------------------------------------

//---[ Constants ]--------------------------------------------------------------


//---[ Types ]------------------------------------------------------------------



//---[ Private Functions ]------------------------------------------------------


//---[ Public Variables ]-------------------------------------------------------

//---[ Private Function Definitions ]-------------------------------------------


void PRVC(void)
{
		//whenever user select prvc it should start with two volume control breaths with set tidal volume
    //measure the plateau pressure in the vcv breaths and that will be the peak pressure for the PRVC breaths
    //set pamax is the alarm limts used in the PRVC mode which can limit the peak pressure
    //In any case do not allow machine to rise pressure above set pmax
    //the error in volume is maintained by reading complaince value and changing the set pressure value
    //at stand by or stop ventilation condition make a the variable fresh to start a new breath,this will help 
    //to avoid the sudden rise or fall of the waveforms.
    //detect the change in the set tidal volume for recheck the varibles.
    //tracking of complaince is better for consistant volume.
    //evaluate the working varible with new set write values this will help to avoid mismatching set values and read values
	
		prvcVentSettings.Set_pmax = ctrlWriteRead.Set_Pmax;
		//storing set pmax 
		prvcVentSettings.Set_TidalVolume = ctrlWriteRead.Set_TidalVol;
		//storing set tidal volume
    
		if(BREATH_CNT == 0 && prvcVentSettings.BreathCnt  >  VCV_BREATHS) 
		//change of patient or large variation in complaince measurement
		{
				for(int index = 0 ; index < 5; index++)
				//making prvc complaince array zero
				{
					 prvcAvgComplainceArray[index] = 0;
				}
				P_compliance = 50;
				prvcINDEX = 0;
				prvcVentSettings.BreathCnt = 1;	
				ventAlgorithmSettings.ulTotalBreathTime = ((float)(60/ctrlWriteRead.Set_RespRate) * 1000);
				ventAlgorithmSettings.ulInhalationTime = ventAlgorithmSettings.ulTotalBreathTime * ctrlWriteRead.Set_IE_Ratio/(1+ctrlWriteRead.Set_IE_Ratio); 
				ventAlgorithmSettings.ulExhalationTime = ventAlgorithmSettings.ulTotalBreathTime - ventAlgorithmSettings.ulInhalationTime;
				ventAlgorithmSettings.ulPauseTime =  (10 * ventAlgorithmSettings.ulTotalBreathTime)/100; 
				ventAlgorithmSettings.ulInhalationTime = ventAlgorithmSettings.ulInhalationTime - ventAlgorithmSettings.ulPauseTime;
				ventAlgorithmSettings.ulRiseTime = (ctrlWriteRead.Set_RiseTime * ventAlgorithmSettings.ulInhalationTime) / 100; 
				ventAlgorithmSettings.ulInhalationFlow = (ctrlWriteRead.Set_TidalVol/ventAlgorithmSettings.ulInhalationTime)*60;
				ventAlgorithmSettings.ulAirFlow = ventAlgorithmSettings.ulInhalationFlow*(100-ctrlWriteRead.Set_FI02)/79;  	
				ventAlgorithmSettings.ulO2Flow = ventAlgorithmSettings.ulInhalationFlow-ventAlgorithmSettings.ulAirFlow;

		}	
	
	
		if(prvcVentSettings.BreathCnt  < VCV_BREATHS)   
		//controlling of first two prvc breaths
		{
			VolCtrl_Algorithm();              
			//calling volume control breaths
			
			prvcVentSettings.Set_pmax = ctrlWriteRead.Set_Pmax;
			//reading set pamx value from the user
			prvcVentSettings.Set_TidalVolume = ctrlWriteRead.Set_TidalVol;
			//reading set tidal volume from the user
			
			prvcVentSettings.PRVC_SetPressure = ventAlgParam.Pplatue;
			//taking measured plateau pressure value from the VCV mode
			
			prvcVentSettings.PmaxPressureError =  prvcVentSettings.Set_pmax - prvcVentSettings.PRVC_SetPressure ;
				
			if(prvcVentSettings.PmaxPressureError <  PRESSURE_DIF )
			//checking for whether pressure above set pmax or not
			{
			
				prvcVentSettings.PRVC_SetPressure =  prvcVentSettings.Set_pmax - PRESSURE_DIF;
				//PRESSURE_DIF value is 5
				//if the plateau pressure is high then it will take the pressure value according to the set pmax vlue
			}
			
			ctrlWriteRead.Set_Insp_Pressure = prvcVentSettings.PRVC_SetPressure - ctrlWriteRead.Set_PEEP ;
			//calculated pressur value for prvc working
			
			
			if(ctrlReadParam.Insp_Pressure < 1) prvcVentSettings.BreathCnt = 1;
			
		}
		else if(prvcVentSettings.BreathCnt >= VCV_BREATHS)
		//checking for PCV breaths
		{
			//here the code checking for the volume error with set tidal volume
			//according to the error this change the set pressure of the pcv algorithm
			//and also check the pressure with set pmax level and adjust the if it is above the limit
			
			if(!ulExhalationFlag  && !ulInhalationFlag && BREATH_CNT > 1)
			//breath to breath control of prvc
			{
				
				//to avoid wrong measurement of complaince
				if(compliance < 0 )compliance = 5;
				if(compliance > 200)compliance = 200;
						
				//complaince measurement in every consecutive five breaths 
				//running average of complaince values
				
				prvcAvgComplainceArray[prvcINDEX++] = compliance;
				
				if(BREATH_CNT  > 4)
				{
						for(int index = 0 ; index < 5; index++)
						{
							prvcAvgComplaince += prvcAvgComplainceArray[index];
						}
						
						if(prvcINDEX > 4) prvcINDEX = 0;
											
						prvcVentSettings.complaince = prvcAvgComplaince/5;
												
						prvcAvgComplaince = 0;
				}
				else if(BREATH_CNT >= 1 && BREATH_CNT <= 4)
				//waiting for atleast five five consecutive complaince values
				{
					
					 prvcVentSettings.complaince = P_compliance;
						
				}
				
				prvcVentSettings.errctrlCnst = 6 - 0.05*prvcVentSettings.complaince;
				//error constant equation
				
/******************************************************************************************************************************/
//testing purpose

				if(prvcVentSettings.errctrlCnst < 2)prvcVentSettings.errctrlCnst = 2; //this is to prevent the set pressure going to negative
/******************************************************************************************************************************/	
			
				prvcVentSettings.VolError = ctrlWriteRead.Set_TidalVol - Insp_Tidal_vol;
				//error calculation of tidal volume

				if(prvcVentSettings.VolError > P_VOL_ERROR_lIMIT || prvcVentSettings.VolError < N_VOL_ERROR_lIMIT)
				//error correction for prvc
				{
					prvcVentSettings.PRVC_SetPressure += prvcVentSettings.VolError/(prvcVentSettings.errctrlCnst*prvcVentSettings.complaince);
					//change in volume is corrected by changing pressure with complaince
				}
				
				prvcVentSettings.PmaxPressureError =  prvcVentSettings.Set_pmax - prvcVentSettings.PRVC_SetPressure ;
				
				if(prvcVentSettings.PmaxPressureError <  PRESSURE_DIF )
				//checking for whether pressure above set pmax or not
				{
					prvcVentSettings.PRVC_SetPressure = prvcVentSettings.Set_pmax - PRESSURE_DIF;
				}
				
				ctrlWriteRead.Set_Insp_Pressure = prvcVentSettings.PRVC_SetPressure - ctrlWriteRead.Set_PEEP;
			  //calculated pressur value for prvc working	
								
				PressureCtrl_Algorithm();
				//calling pressure control algorithm
				
							
			}
			
			else
			//entering into pressure controlled ventilation with set tidal volume
			{
				ventAlgorithmSettings.ulTotalBreathTime = ((float)(60/ctrlWriteRead.Set_RespRate) * 1000);
				//calculating breath time for prvc
				ventAlgorithmSettings.ulInhalationTime = ventAlgorithmSettings.ulTotalBreathTime * ctrlWriteRead.Set_IE_Ratio/(1+ctrlWriteRead.Set_IE_Ratio); 
				//calculating inspiration time
				ventAlgorithmSettings.ulExhalationTime = ventAlgorithmSettings.ulTotalBreathTime - ventAlgorithmSettings.ulInhalationTime;
				//exhaltion time
				ventAlgorithmSettings.ulRiseTime = (ctrlWriteRead.Set_RiseTime * ventAlgorithmSettings.ulInhalationTime) / 100; 
				//calculating rise time
			
				PressureCtrl_Algorithm();
				//calling pressure control algorithm
				
			}
		}
		
		
		if(prvcVentSettings.BreathCnt > 100)prvcVentSettings.BreathCnt = 10;
    
}