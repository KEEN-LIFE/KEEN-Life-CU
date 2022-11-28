/*! \file **********************************************************************
 *
 *  \brief  
 *
 
 *  \author 
 *
 *  Copyright (c) 2021 CT Control Technology .
 ******************************************************************************/



//---[ Includes ]---------------------------------------------------------------

#include "TechnicalAlarm_Handler.h"

//---[ Macros ]-----------------------------------------------------------------

//---[ Constants ]--------------------------------------------------------------

//---[ Public Variables ]-------------------------------------------------------

uint32_t status_collect=0,clear_bit=0,alarm_index=0;
uint16_t test=1; 																				//testing variable

//---[ Private Variables ]------------------------------------------------------

//---[ Private Functions ]------------------------------------------------------

//---[ Function Implementations ]-----------------------------------------------

void Technical_Alarm(void)
{
		
		PV_AlarmGeneration();  //calling Valve's alarm
	
//	AL005 - Internal Power failure 	
//	This alarm signal gets triggered if any of the following 3 condition occur
	
	if((ctrlReadParam.VG_Sense_5V < VG_5V_LOWER_LIMIT) || (ctrlReadParam.VG_Sense_5V > VG_5V_UPPER_LIMIT) )
		Tech_Alarmcheck.vg_5v_sense=SET_BIT;
	else
		Tech_Alarmcheck.vg_5v_sense=ALA_CLR;
		
	
	if(	(ctrlReadParam.VG_Sense_18V < VG_18V_LOWER_LIMIT) || (ctrlReadParam.VG_Sense_18V > VG_18V_UPPER_LIMIT) )
		Tech_Alarmcheck.vg_18v_sense=SET_BIT;
	else
		Tech_Alarmcheck.vg_18v_sense=ALA_CLR;
		
		if(!GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_5)  && !ctrlReadParam.Ins_Zero_Valve_Sense)
		Tech_Alarmcheck.insp_zero_valve = SET_BIT;
	else
		Tech_Alarmcheck.insp_zero_valve = ALA_CLR;
	
	if(!GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_4)  && !ctrlReadParam.Aux_Valve_Sense)
		Tech_Alarmcheck.aux_purge_valve = SET_BIT;
	else
		Tech_Alarmcheck.aux_purge_valve = ALA_CLR;

	if(!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)  && !ctrlReadParam.Ins_Effort_Valve_Sense)
		Tech_Alarmcheck.Insp_Effort_Valve = SET_BIT;
	else
		Tech_Alarmcheck.Insp_Effort_Valve = ALA_CLR;
	
	if(	(ctrlReadParam.VG_Sense_12V < VG_12V_LOWER_LIMIT) || (ctrlReadParam.VG_Sense_12V > VG_12V_UPPER_LIMIT)  )
		Tech_Alarmcheck.vg_12v_sense=SET_BIT;
	else
		Tech_Alarmcheck.vg_12v_sense=ALA_CLR;
		
	

//	AL032 - Over Temperature failure		
//	When the board temp breaches the limit this alarm will be triggered
	
	if(ctrlReadParam.Board_Temp > BOARD_TEMP_LIMIT) // above 55 degree C
		Tech_Alarmcheck.board_temp=SET_BIT;
	else
		Tech_Alarmcheck.board_temp=ALA_CLR;
		


////Technical alarm new status assigning new logic
/************************************************************************************/

	clear_bit=SET_BIT;																	//make sure that clear bit is always 1
	for(alarm_index=0; alarm_index < 32; alarm_index++) //checkng for 32 alarm registers
	{
		status_collect = *(t_alarmptr+alarm_index);  			//collecting individual alarm
		status_collect <<= alarm_index;										//shifting to position of the register
		clear_bit  <<= alarm_index;												//shifting clearbit to position of the alarm register
		IntVar &= (~clear_bit);														//clearing alarm position
		IntVar |= status_collect;													//assigning alarm bit (0/1)	
		clear_bit=SET_BIT;
	}	
	
		ctrlReadParam.Status = IntVar;		
//		ctrlServParam.Status_Reg=ctrlReadParam.Status;    //Assign to controlwrite status register.

/************************************************************************************/
////Technical alarm new status assigning new logic
	
//	for(alarm_index=0; alarm_index < 32; alarm_index++) 			//checkng for 32 alarm registers
//		*(t_alarmptr+alarm_index)=ALA_CLR;                 		  //clearing all alarm registers for new check;
	
}
void PV_AlarmGeneration()
{
	
	if(ulInhalationFlag)
	{
		if(ventAlgParam.ulPVValueAir > 8000 && ventAlgParam.ulPVValueAir < 25000)
		{
				PV_SetValue_A = ventAlgParam.ulPVValueAir/65.535;
					
				if((ctrlReadParam.Air_PV_Sense >= (PV_SetValue_A - PV_SetValue_A*0.2)) && (ctrlReadParam.Air_PV_Sense <= (PV_SetValue_A+ PV_SetValue_A*0.2)))
				{
					AirPV_Sense_Failure = 0;	
					Tech_Alarmcheck.air_valve=0;
				}
				else
				{
					AirPV_Sense_Failure++;
					if(AirPV_Sense_Failure > 50) 
						{Tech_Alarmcheck.air_valve=1;}
				}	
	 }

		
		if(ventAlgParam.ulPVValueO2 > 8000 && ventAlgParam.ulPVValueO2 < 25000)
		{
			PV_SetValue_O = (ventAlgParam.ulPVValueO2/65.535);
				
			if((ctrlReadParam.O2_PV_Sense >= (PV_SetValue_O - PV_SetValue_O*0.2)) && (ctrlReadParam.O2_PV_Sense <= (PV_SetValue_O + PV_SetValue_O*0.2)))
			{
				O2PV_Sense_Failure = 0;	
				Tech_Alarmcheck.o2_valve=0;
			}
			else
			{
				O2PV_Sense_Failure ++;
					if(O2PV_Sense_Failure > 50) 
						{Tech_Alarmcheck.o2_valve=1;}			
			}	
		}
		if(Exh_DACVal > 12000 && Exh_DACVal < 22000)
		{
			PV_SetValue =(Exh_DACVal/65.535);
				
			if((ctrlReadParam.EV_PV_Sense >= (PV_SetValue - PV_SetValue*0.2)) && (ctrlReadParam.EV_PV_Sense <= (PV_SetValue + PV_SetValue*0.2)))
			{
				ExhPV_Sense_Failure = 0;	
				Tech_Alarmcheck.exhal_valve=0;
			}
			else
			{
				ExhPV_Sense_Failure ++;
				Tech_Alarmcheck.exhal_valve=(ExhPV_Sense_Failure > 50) ? 1 : 0;
			}
	  }
	}
}
