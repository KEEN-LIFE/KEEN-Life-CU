/*! \file **********************************************************************
 *
 *  \brief 
 *
 
 *  \author 
 *
 *  Copyright (c) 2021 CT Control Technology .
 ******************************************************************************/


//---[ Includes ]---------------------------------------------------------------

#include "preusecheck.h"

//---[ Macros ]-----------------------------------------------------------------


//---[ Constants ]--------------------------------------------------------------


//---[ Types ]------------------------------------------------------------------

//---[ Private Functions ]------------------------------------------------------
void preusecheck(void)
{
	if(puc_mode != ctrlWriteParam.Preusecheck) //this condition is for turn off all valves and test before going to next test
		puc_mode = DO_NOTHING;

	switch(puc_mode)
	{
		
		case GAS_SUPPPLY_CHK :Gas_supply();
														break;							//in this part checking the inlet and regulated pressure
																								//of the ventilator, and update the status to the DU,The data is 
																								//coming from the monitor board
		case VALVE_CHK			 :Valve_check();  			//pv valve and exhalation valve are checking
													break;
		case FLOW_SENSOR_CHK :Flow_Sensor_check(); 	//all four flow sensors are checking 
													break;
		case O2_SENSOR_CHK	 :O2_Sensor_check(); 		//O2 sensors are checking 
													break;
		case PRE_SENSOR_CHK  :Pre_Sensor_check(); 	//pressure sensor checking
													break;
		case SAF_RLF_VALVE 	 :Safety_Valve_check(); // safety relief valve checking
													break;
		case AUX_RLF_VALVE 	 :Aux_Valve_check(); 		// auxiliary path and valve are ckecking
													break;
		case BAT_TEST 			 :Battery_check();			// battery testing ,the signal is coming from the 
													break;								// power management board
		
		case DO_NOTHING	     :Normal_state();					//this is used to terminate the current test
													break;
													
		default 						 :Normal_state();
													break;
			
	}
	

}

void Gas_supply (void)
{
// This function is used to check the functionality of PV,exhalation valve and all ON_OFF valves
// the feedback from each valve is used for checking.
			if(puc_count_flag==0) 
			{
				Tech_Alarmcheck.puc_update=0; //preuse checking status make as zero
				puc_count=0;
				puc_count_flag=1;								//starting of preusecheck counter
			}
			
			
			if(puc_count >= CHK_TIME)				//preusecheck operation for PV and exhalation valve
			{
				Tech_Alarmcheck.puc_update=1;
				
			}

}

void Valve_check(void)
{
	// This function is used to check the functionality of PV,exhalation valve and all ON_OFF valves
	// the feedback from each valve is used for checking.
			if(puc_count_flag==0) 
			{
				Tech_Alarmcheck.puc_update=0; //preuse checking status make as zero
				puc_count=0;
				puc_count_flag=1;								//starting of preusecheck counter
			}
			
			
			if(puc_count <= CHK_TIME)				//preusecheck operation for PV and exhalation valve
			{			
					Exh_DACVal = 12000;
					ventAlgParam.ulPVValueAir = 12000;
					ventAlgParam.ulPVValueO2  = 11500;
					DAC_Output_Set(&DAC1,DAC_CHANNEL_1,ventAlgParam.ulPVValueAir,LTC2607_WR_AND_UPDATE); // AIR PV SET Function
					DAC_Output_Set(&DAC1,DAC_CHANNEL_2,ventAlgParam.ulPVValueO2,LTC2607_WR_AND_UPDATE); // O2 PV SET Function
					DAC_Output_Set(&DAC2,DAC_CHANNEL_1,Exh_DACVal,LTC2607_WR_AND_UPDATE);   // Exhal Valve
			}
			else
			{										
					Tech_Alarmcheck.puc_update=1;
	
			}
}
void Flow_Sensor_check(void)
{
// This function is used to check the functionality of all flow sensors by giving some 8lpm
// from the air and O2 line.	

			if(puc_count_flag==0) 
			{
				Tech_Alarmcheck.puc_update=0; //preuse checking status make as zero
				puc_count=0;
				puc_count_flag=1;								//starting of preusecheck counter

			}
			
			
			if(puc_count <= CHK_TIME)				//preusecheck operation for PV and exhalation valve
			{
			
					ventAlgorithmSettings.ulAirFlow= 5;
					ventAlgorithmSettings.ulO2Flow=5;
					Exh_DACVal = 0;
					flow_control();
					DAC_Output_Set(&DAC2,DAC_CHANNEL_1,Exh_DACVal,LTC2607_WR_AND_UPDATE);   // Exhal Valve
			}
			else
			{							
					Tech_Alarmcheck.puc_update=1;
			}

}
void O2_Sensor_check(void)
{

// This function is used to check the functionality of all O2 sensors by giving 50% FiO2
	
			if(puc_count_flag==0) 
			{
				Tech_Alarmcheck.puc_update=0; //preuse checking status make as zero
				puc_count=0;
				puc_count_flag=1;								//starting of preusecheck counter

			}
			
			
			if(puc_count <= 10000)				//10s for oxygen sensor checking
			{
			
					ventAlgorithmSettings.ulAirFlow = 	20*50/79;  	
					ventAlgorithmSettings.ulO2Flow	=		20 - ventAlgorithmSettings.ulAirFlow;
					Exh_DACVal = 0;
					flow_control();
					DAC_Output_Set(&DAC2,DAC_CHANNEL_1,Exh_DACVal,LTC2607_WR_AND_UPDATE);   // Exhal Valve
			}
			else
			{						
					Tech_Alarmcheck.puc_update=1;
			}

	
}

void Pre_Sensor_check(void)
{
	// This function is used to check the functionality of all pressure sensors by giving back pressure 
	// from using exhaltion valve
	
			if(puc_count_flag==0) 
			{
				Tech_Alarmcheck.puc_update=0; //preuse checking status make as zero
				puc_count=0;
				puc_count_flag=1;								//starting of preusecheck counter

			}
			
			
			if(puc_count <= 10000)				//preusecheck operation for PV and exhalation valve
			{
			
					ventAlgorithmSettings.ulAirFlow = 	10; 	
					ventAlgorithmSettings.ulO2Flow	=		0 ;

					Exh_DACVal = sysParam.DAC_Peep[0]*9+ sysParam.DAC_Peep[1]; //Dac vs PEEP curve
				
					ventAlgParam.ulPVValueAir = Air_Flow_calc(ventAlgorithmSettings.ulAirFlow);	
					ventAlgParam.ulPVValueO2  = O2_Flow_calc( ventAlgorithmSettings.ulO2Flow);
					DAC_Output_Set(&DAC1,DAC_CHANNEL_1,ventAlgParam.ulPVValueAir,LTC2607_WR_AND_UPDATE); // AIR PV SET Function
					DAC_Output_Set(&DAC1,DAC_CHANNEL_2,ventAlgParam.ulPVValueO2,LTC2607_WR_AND_UPDATE); // O2 PV SET Function
					DAC_Output_Set(&DAC2,DAC_CHANNEL_1,Exh_DACVal,LTC2607_WR_AND_UPDATE);   // Exhal Valve
				
					if((ctrlReadParam.Insp_Pressure - ctrlReadParam.Exp_Pressure) >= 5) 	//checking whether the safety relief valve is opened or not
					{
						Tech_Alarmcheck.pre_diff = 1;
					}
					else if(!Tech_Alarmcheck.insp_safe_relief && ctrlReadParam.Insp_Pressure < 5)
					{
						Tech_Alarmcheck.pre_diff = 0;			
					}

			}
			else
			{		
					Tech_Alarmcheck.puc_update=1;
			}

}

void Safety_Valve_check(void)
{
// This function is used to check the functionality of inspiratory safety relief valve  
// by closing the inspiratory path
	
			if(puc_count_flag==0) 
			{
				Tech_Alarmcheck.puc_update=0; //preuse checking status make as zero
				puc_count=0;
				puc_count_flag=1;								//starting of preusecheck counter

			}
			
			
			if(puc_count <= CHK_TIME)				//preusecheck operation for PV and exhalation valve
			{
			
					ventAlgorithmSettings.ulAirFlow = 	10; 	
					ventAlgorithmSettings.ulO2Flow	=		0 ;
					Exh_DACVal = 0;
				
					ventAlgParam.ulPVValueAir = Air_Flow_calc(ventAlgorithmSettings.ulAirFlow);	
					ventAlgParam.ulPVValueO2  = O2_Flow_calc( ventAlgorithmSettings.ulO2Flow);
					DAC_Output_Set(&DAC1,DAC_CHANNEL_1,ventAlgParam.ulPVValueAir,LTC2607_WR_AND_UPDATE); // AIR PV SET Function
					DAC_Output_Set(&DAC1,DAC_CHANNEL_2,ventAlgParam.ulPVValueO2,LTC2607_WR_AND_UPDATE); // O2 PV SET Function
					DAC_Output_Set(&DAC2,DAC_CHANNEL_1,Exh_DACVal,LTC2607_WR_AND_UPDATE);   // Exhal Valve
					
					if(ctrlReadParam.Insp_Pressure >= 130) 	//checking whether the safety relief valve is opened or not
					{
						Tech_Alarmcheck.insp_safe_relief =1;
					}
					else if(!Tech_Alarmcheck.insp_safe_relief && ctrlReadParam.Insp_Pressure < 130)
					{
						Tech_Alarmcheck.insp_safe_relief =0;			
					}
				
			}
			else
			{		
					Tech_Alarmcheck.puc_update=1;
			}

	
}
void Aux_Valve_check(void)
{
	// This function is used to check the functionality of auxiliary  safety relief valve  
	// by closing the auxilaiary  path
	
			if(puc_count_flag==0) 
			{
				Tech_Alarmcheck.puc_update=0; //preuse checking status make as zero
				puc_count=0;
				puc_count_flag=1;								//starting of preusecheck counter

			}
			
			
			if(puc_count <= CHK_TIME)				//preusecheck operation for PV and exhalation valve
			{
			
					GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, GPIO_PIN_4); //Auxiliary valve control,make valve open
				
					if(ctrlReadParam.Aux_Pressure >= 250) 	//checking whether the safety relief valve is opened or not
					{
						Tech_Alarmcheck.aux_safe_relief =1;
					}
					else if(!Tech_Alarmcheck.aux_safe_relief && ctrlReadParam.Aux_Pressure < 250)
					{
						Tech_Alarmcheck.aux_safe_relief =0;
						
					}
				
			}
			else
			{		
					Tech_Alarmcheck.puc_update=1;
			}

}

void Battery_check(void){
// This function is used to check the functionality of PV,exhalation valve and all ON_OFF valves
// the feedback from each valve is used for checking.
			if(puc_count_flag==0) 
			{
				Tech_Alarmcheck.puc_update=0; //preuse checking status make as zero
				puc_count=0;
				puc_count_flag=1;								//starting of preusecheck counter

			}
			
			
			if(puc_count >= CHK_TIME)				//preusecheck operation for PV and exhalation valve
			{
					Tech_Alarmcheck.puc_update=1;

			}
}

void Normal_state(void)
{
	DAC_Output_Set(&DAC1,DAC_CHANNEL_1,0,LTC2607_WR_AND_UPDATE); // AIR PV SET Function
	DAC_Output_Set(&DAC1,DAC_CHANNEL_2,0,LTC2607_WR_AND_UPDATE); // O2 PV SET Function
	DAC_Output_Set(&DAC2,DAC_CHANNEL_1,0,LTC2607_WR_AND_UPDATE);   // Exhal Valve
	GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0); //Auxiliary valve control,close		
	ventAlgParam.ulPVValueAir  = 0;
	ventAlgParam.ulPVValueO2   = 0;
	ventAlgParam.ulPVValueEV   = 0;
	Tech_Alarmcheck.puc_update = 0; //preuse checking status make as zero
	Exh_DACVal = 0;	
	puc_count_flag  = 0;
	puc_mode = ctrlWriteParam.Preusecheck;

}