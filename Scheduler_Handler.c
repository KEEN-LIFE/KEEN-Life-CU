/*! \file **********************************************************************
 *
 *  \brief 
 *
 
 *  \author 
 *
 *  Copyright (c) 2021 CT Control Technology .
 ******************************************************************************/


//---[ Includes ]---------------------------------------------------------------

#include "Scheduler_Handler.h"
#include "VentAlgorithm_Handler.h"
#include "TechnicalAlarm_Handler.h"
#include "Diagnostics.h"
#include "Medical_procedure.h"

float pv[100]={0};
uint32_t VentMode;
//---[ Macros ]-----------------------------------------------------------------



//---[ Constants ]--------------------------------------------------------------


//---[ Types ]------------------------------------------------------------------


//---[ Private Functions ]------------------------------------------------------


//---[ Public Variables ]-------------------------------------------------------


//---[ Private Function Definitions ]-------------------------------------------
uint8_t mode_=0;
void stateRun()
{
	
		if(!ulExhalationFlag && !ulInhalationFlag && !ulPauseFlag && VentSettingsChange == 1)
		{			
			ctrlWriteRead = ctrlWriteParam;
			VentAlgorithmSettings();
			VentSettingsChange = 0;
		}
		
		if(flag_1ms == FLAG_SET)
		{
			medicalProce();			
			Vent_Algorithm();
			FlowSensor_dataAcq();	
			flag_1ms = 0;
		}
	 
		 
		if(flag_10ms == FLAG_SET)
		{
			ADC_DataAcq();
			SSI_Communication();
			ExhFlowSensor();
			
			if(ulInhalationFlag == 1)
			{
				GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0); //De-energizing free breathing valve.
				FlowControlLoop(0);
				
				if(ctrlReadParam.Insp_Pressure < 1 &&  BREATH_CNT > 1 && ctrlWriteRead.Pat_TrigMode == 3)  // checking for the drop in the pressure 
					BREATH_CNT=0;
			}
			if(ulExhalationFlag == 1)
			{ 			

//				PID_PressureControl_Loop();  //testing for NIV mode.
				
				comp_calculation();//compliance calculation
				
				PEEP_control();		//PEEP controlfunction			
				
				if(ctrlWriteRead.Pat_TrigMode == FLOW_TRIGGER_MODE)
				{						
					FlowTrigger();
					apneacount_flag=1;
					
				}
				else if(ctrlWriteRead.Pat_TrigMode == PRESSURE_TRIGGER_MODE && RC_ConstCount >700)
				{
					PressureTrigger();	
					apneacount_flag=1;
					
				}
				else if(ctrlWriteRead.Pat_TrigMode == NO_TRIGGER)
				{
				    apneaalarm_flag=0;  //clearing apnea alarm when patient effort is detected.
						apneacount_flag=0;	//clering apnea time flag
						apneacount_time=0;	//clearing apnea counter				
					  Tech_Alarmcheck.apnea_ventilation=0;
				}	
			 }
//apnea alarm counter cheecking
				if(apneacount_time >= 4*1000)
				{

					apneaalarm_flag=1;	
					Tech_Alarmcheck.apnea_ventilation=1;					
				}
				
			 
			Technical_Alarm();  //calling technical alarm
				
			//Volume calculation for service packets				
			ctrlReadParam.Volume=ctrlReadParam.Volume+(ctrlServParam.Inh_Flow-ctrlServParam.Exh_Flow)/6;
			ctrlServParam.Volume=ctrlReadParam.Volume;

			Modbus_Service_MessageTx(); //Sevice packets are sending in every 10ms		

/*********************************************************************************************************/
	
/*  Internal Inspiratory Tidal volume  calculation   */

//			if(ulInhalationFlag || ulPauseFlag)
//			{
//	
//			Insp_Tidal_vol = Insp_Tidal_vol+ctrlServParam.Inh_Flow/6;
//	
////		Insp_Tidal_vol_A = Insp_Tidal_vol_A + ctrlReadParam.Air_Flow/6;

//			}
/*****************************************************************************************************/

				
			flag_10ms = 0;
		}

			
			if(flag_1s == FLAG_SET)
			{
				
//				debug_print();
				FlowSensorReset=1; //for resetting the flow sensors
				flag_1s = 0;
			}			

	//Assigning Serv parameters to the DU 
			
	ctrlServParam.Insp_Pressure = ctrlReadParam.Insp_Pressure;
	ctrlServParam.Exp_Pressure  = ctrlReadParam.Exp_Pressure;
	ctrlServParam.Aux_Pressure  = ctrlReadParam.Aux_Pressure;
	ctrlServParam.Status_Reg    = ctrlReadParam.Status;
			
	//ctrlServParam.Time_Stamp is assigning durinng modbus communication
	
			
	if(Volume_read  ==  1 )
	{
		ctrlServParam.Inh_Flow = 0;
		ctrlServParam.Exh_Flow = 0;
	}
	else
	{
		ctrlServParam.Inh_Flow = ctrlReadParam.Total_Flow;
		ctrlServParam.Exh_Flow = ctrlReadParam.Exh_Flow;
	}
			
			
}

void stateStop()
{
	
  	DAC_Output_Set(&DAC1,DAC_CHANNEL_1,0,LTC2607_WR_AND_UPDATE); // AIR PV SET Function
    DAC_Output_Set(&DAC1,DAC_CHANNEL_2,0,LTC2607_WR_AND_UPDATE); // O2 PV SET Function
//   	DAC_Output_Set(&DAC2,DAC_CHANNEL_1,0,LTC2607_WR_AND_UPDATE);   // Exhal Valve

			if(flag_10ms == FLAG_SET)
			{
				//call aq m1
				// call al m1
				
								
				flag_10ms = 0;
			}
//			if(flag_100ms == FLAG_SET)
//			{
//				//call aq m2
//				// call al m2
//				
//			
//				
//				flag_100ms = 0;
//			}
			if(flag_1s == FLAG_SET)
			{
				//call aq m3
				// call al m3
				
			
				flag_1s = 0;
			}
			//Call comm
			// Vent Algorithm		
			
			ctrlWriteRead = ctrlWriteParam;
			ulExhalationFlag = 0;
			ulInhalationFlag = 0;
			ulPauseFlag=0;
			ulInhalationCounter=0;
			ulExhalationCounter =0;
			ulPuaseCounter = 0;		
			ctrlReadParam.Volume=0;		
			ctrlServParam.Volume=0;
			Tech_Alarmcheck.puc_update = 0;
			BREATH_CNT=0;
}

void stateStandBy()
{
		if(VentRunFlag == 0 || VentSettingsChange == 1)
		{
//			VentAlgorithmSettings();
//			
//			VentRunFlag = 1;
//			VentSettingsChange = 0;
		}
			if(flag_10ms == FLAG_SET)
			{
				//call aq m3
				//call al m3
				ADC_DataAcq();
				FlowSensor_dataAcq();
				SSI_Communication();
				
				ctrlServParam.Inh_Flow = ctrlReadParam.Total_Flow;
				ctrlServParam.Insp_Pressure = ctrlReadParam.Insp_Pressure;
				ctrlServParam.Aux_Pressure = ctrlReadParam.Aux_Pressure;				
				
				flag_10ms = 0;
			}
		DAC_Output_Set(&DAC1,DAC_CHANNEL_1,0,LTC2607_WR_AND_UPDATE); // AIR PV SET Function
    DAC_Output_Set(&DAC1,DAC_CHANNEL_2,0,LTC2607_WR_AND_UPDATE); // O2 PV SET Function
//   	DAC_Output_Set(&DAC2,DAC_CHANNEL_1,0,LTC2607_WR_AND_UPDATE);   // Exhal Valve

		BREATH_CNT=0;
			
		ctrlWriteRead = ctrlWriteParam;
		ulExhalationFlag = 0;
		ulInhalationFlag=0;
		ulPauseFlag=0;
		ulInhalationCounter=0;
		ulExhalationCounter =0;
		ulPuaseCounter = 0;		
		ctrlReadParam.Volume=0;		
		ctrlServParam.Volume=0;
		Tech_Alarmcheck.puc_update = 0;

}


void stateDiagnostic()
{
			if(flag_10ms == FLAG_SET)
			{
				
				FlowSensor_dataAcq();	
				ADC_DataAcq();
				SSI_Communication();
				ExhFlowSensor();
				Modbus_Service_MessageTx();	
				Technical_Alarm(); //calling technical alarms
				flag_10ms = 0;
				
			}
			
			if(flag_100ms == FLAG_SET)
			{
				
				Self_Diagnostics();

				//call the diagnostic function for control board 

				flag_100ms = 0;
			}
			if(flag_1s == FLAG_SET)
			{
				flag_1s = 0;
			}
}

void stateReset()
{
			if(flag_10ms == FLAG_SET)
			{
				//call aq m1
				// call al m1
				
							
				flag_10ms = 0;
			}
			if(flag_100ms == FLAG_SET)
			{
				//call aq m2
				// call al m2
				
				
				
				flag_100ms = 0;
			}
			if(flag_1s == FLAG_SET)
			{
				//call aq m3
				// call al m3
				
					
				flag_1s = 0;
			}
			//Call comm
			// Vent Algorithm		
}

