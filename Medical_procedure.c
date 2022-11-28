/*! \file **********************************************************************
 *
 *  \brief 
 *
 
 *  \author 
 *
 *  Copyright (c) 2021 CT Control Technology .
 ******************************************************************************/


//---[ Includes ]---------------------------------------------------------------

#include "Medical_procedure.h"

//---[ Macros ]-----------------------------------------------------------------


//---[ Constants ]--------------------------------------------------------------


//---[ Types ]------------------------------------------------------------------

//---[ Private Functions ]------------------------------------------------------
uint8_t med_mode;

void medicalProce(void)
{
	
	Tech_Alarmcheck.MedprocUpdate = Tech_Alarmcheck.MedprocUpdate * (ctrlServParam.Time_Stamp == 2 ? 0: 1);
	
	med_mode = (uint8_t)ctrlWriteParam.Medical_proc;
	
	switch(med_mode)
	{
		case INSP_HOLD : InspiratoyHold();
										 break;
		case EXP_HOLD  : ExpiratoryHold();
										 break;
		case MAN_BREATH: ManualBreath();
										 break;
		default        : break;
		
	}
	
}

void InspiratoyHold(void)
//This function is for achieving inspiratory hold
//Inspiratory hold is started  after the inhalation period is over or in the pause time(VCV mode)
//hold should continue for a time of hold time from the user
//maximum hold time is 30s
//here the function is controlling the inhaltion,pause and exhalation flags and counter
{	
	if(ulInhalationCounter >= ventAlgorithmSettings.ulInhalationTime || ulPauseFlag)
	{
		ulInhalationFlag =0;
		medProc_Flag = 1;
		Tech_Alarmcheck.MedprocUpdate = 1;
		if(medProcCounter < ctrlWriteParam.Hold_time*1000)
		{
			DAC_Output_Set(&DAC1,DAC_CHANNEL_1,0,LTC2607_WR_AND_UPDATE); // AIR PV SET Function
			DAC_Output_Set(&DAC1,DAC_CHANNEL_2,0,LTC2607_WR_AND_UPDATE); // O2 PV SET Function
			DAC_Output_Set(&DAC2,DAC_CHANNEL_1,20000,LTC2607_WR_AND_UPDATE);   // Exhal Valve
		}
		else
		{
			medProc_Flag = 0;
			medProcCounter = 0;
			ulInhalationFlag =0;
			ulInhalationCounter = 0;
			ulExhalationFlag = 1;
			ctrlWriteParam.Hold_time =0;
			ctrlWriteParam.Medical_proc = 0;
		}
	}
}

void ExpiratoryHold(void)
//This function is for achieving expiratory hold
//Expiratory hold is started  in the end of  exhaltion period.
//hold should continue for a time of Hold_time from the user
//maximum hold time is 30s
//here the function is controlling the inhaltion,pause and exhalation flags and also the counters.
{
	if(ulExhalationCounter >= ventAlgorithmSettings.ulExhalationTime )
	{
		ulExhalationFlag =0;
		medProc_Flag = 1;
		Tech_Alarmcheck.MedprocUpdate = 1;
		if(medProcCounter < ctrlWriteParam.Hold_time*1000)
		{
			DAC_Output_Set(&DAC1,DAC_CHANNEL_1,0,LTC2607_WR_AND_UPDATE); // AIR PV SET Function
			DAC_Output_Set(&DAC1,DAC_CHANNEL_2,0,LTC2607_WR_AND_UPDATE); // O2 PV SET Function
			DAC_Output_Set(&DAC2,DAC_CHANNEL_1,15000,LTC2607_WR_AND_UPDATE);   // Exhal Valve
		}
		else
		{
			medProc_Flag = 0;
			medProcCounter = 0;
			DAC_Output_Set(&DAC1,DAC_CHANNEL_1,0,LTC2607_WR_AND_UPDATE); // AIR PV SET Function					
			ventAlgParam.PatientTriggerWindow=1;
			ulExhalationFlag = 0;
			ulExhalationCounter = 0;
			RC_TimeInit=0;
			RC_TimeCount=0;
			RC_Init=0;
			RC_ConstCount=0;
			Backup_init=0;
			Backup_wait=0;
			Tech_Alarmcheck.apnea_ventilation=0;
			apneaalarm_flag=0;  //clearing apnea alarm when patient effort is detected.
			apneacount_flag=0;	//clearing apnea time flag
			apneacount_time=0;	//clearing apnea counter
			ctrlWriteParam.Hold_time =0;
			ctrlWriteParam.Medical_proc = 0;
		}
	}
	
}

void ManualBreath(void)
//This function is for achieving manual breath
//manual breath is delivered only during expiration 
//no breath will deleiverd during inspiration
//manual breath will be given with current ventilator settings
{
	
	if(ulExhalationFlag && Tid_Vol  >  Insp_Tidal_vol*0.8 && peep_delay)
	{
		
			DAC_Output_Set(&DAC1,DAC_CHANNEL_1,0,LTC2607_WR_AND_UPDATE); // AIR PV SET Function					
			ventAlgParam.PatientTriggerWindow=1;
			ulExhalationFlag = 0;
			ulExhalationCounter = 0;
			RC_TimeInit=0;
			RC_TimeCount=0;
			RC_Init=0;
			RC_ConstCount=0;
			Backup_init=0;
			Backup_wait=0;
			Tech_Alarmcheck.apnea_ventilation=0;
			apneaalarm_flag=0;  //clearing apnea alarm when patient effort is detected.
			apneacount_flag=0;	//clearing apnea time flag
			apneacount_time=0;	//clearing apnea counter
			ctrlWriteParam.Hold_time =0;
			ctrlWriteParam.Medical_proc = 0;
	}
	
}