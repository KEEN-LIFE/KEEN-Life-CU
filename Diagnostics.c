/*! \file **********************************************************************
 *
 *  \brief 
 *
 
 *  \author 
 *
 *  Copyright (c) 2021 CT Control Technology .
 ******************************************************************************/


//---[ Includes ]---------------------------------------------------------------

#include "Diagnostics.h"

//---[ Macros ]-----------------------------------------------------------------


//---[ Constants ]--------------------------------------------------------------


//---[ Types ]------------------------------------------------------------------

//---[ Private Functions ]------------------------------------------------------
uint8_t Diag_Flag = 0;
uint8_t IZV=0,IEV=0,AV=0;
void Self_Diagnostics(void)
{
	uint8_t Diagnostic = ctrlWriteParam.Diagnostics;
	
	if(!Diag_Flag)
	{
		ctrlWriteParam.Set_AirFlow = 20;
		ctrlWriteParam.Set_O2Flow  = 20;
		ctrlWriteParam.Set_AirValve = 12000;
		ctrlWriteParam.Set_O2Valve  = 12000;
		ctrlWriteParam.Set_ExhValve = 8000;
		ctrlWriteParam.Ins_Effort_Valve = 0;
		ctrlWriteParam.Ins_Zero_Valve   = 0;
		ctrlWriteParam.Aux_Valve        = 0;	
		Diag_Flag = 1;
	}
	if(Diagnostic > DG_028)
	{
		ctrlWriteParam.Ins_Effort_Valve = 1;
		ctrlWriteParam.Ins_Zero_Valve   = 1;
		ctrlWriteParam.Aux_Valve        = 1;	
	}
	
	IZV = (uint8_t)(GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_5));
	IEV = (uint8_t)(GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_4));
	AV  = (uint8_t)(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0));
	
	if(ventAlgorithmSettings.vent_test != Diagnostic)
		ventAlgorithmSettings.vent_test = 0;
	else
		ventAlgorithmSettings.vent_test = Diagnostic;
	
	Ventilator_Test();

	
}
void Ventilator_Test(void)
{ 
	uint8_t  Test_ID  =  ventAlgorithmSettings.vent_test;
	

	switch(Test_ID)
	{
		case DG_023 :
					ventAlgParam.ulPVValueAir = ctrlWriteParam.Set_AirValve;
					DAC_Output_Set(&DAC1,DAC_CHANNEL_1,ventAlgParam.ulPVValueAir,LTC2607_WR_AND_UPDATE); // AIR PV SET Function					DAC_Output_Set(&DAC2,DAC_CHANNEL_1,Exh_DACVal,LTC2607_WR_AND_UPDATE);   // Exhal Valve
					break;
		case DG_024:
					ventAlgParam.ulPVValueO2  = ctrlWriteParam.Set_O2Valve;
					DAC_Output_Set(&DAC1,DAC_CHANNEL_2,ventAlgParam.ulPVValueO2,LTC2607_WR_AND_UPDATE); // O2 PV SET Function
					break;
		case DG_025:
					ventAlgParam.ulPVValueEV = ctrlWriteParam.Set_ExhValve;
					DAC_Output_Set(&DAC2,DAC_CHANNEL_1,ventAlgParam.ulPVValueEV,LTC2607_WR_AND_UPDATE);   // Exhal Valve
					break;
		case DG_026:
					ventAlgParam.ulInspZeroValve = ctrlWriteParam.Ins_Zero_Valve == 0 ? 0 : GPIO_PIN_5 ;
					GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, ventAlgParam.ulInspZeroValve);
					break;
		case DG_027:
					ventAlgParam.ulAuxValve = ctrlWriteParam.Aux_Valve == 0 ? 0 : GPIO_PIN_4 ;
					GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, ventAlgParam.ulAuxValve);	
					break;
		case DG_028:
					ventAlgParam.ulInsEffortValve = ctrlWriteParam.Ins_Effort_Valve == 0 ? 0 : GPIO_PIN_0 ;
					GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, ventAlgParam.ulInsEffortValve);
					break;
		case DG_029:
						ctrlWriteParam.Diagnostics= 26;
					break;
		case DG_031:
						ctrlWriteParam.Diagnostics= 27;
					break;
		case DG_032:
					ctrlWriteParam.Diagnostics= 23;
					ctrlWriteParam.Set_AirValve = Air_Flow_calc(ctrlWriteParam.Set_AirFlow);	
					break;
		case DG_033:
					ctrlWriteParam.Diagnostics= 24;
					ctrlWriteParam.Set_O2Valve = O2_Flow_calc(ctrlWriteParam.Set_O2Flow);
					break;
		case DG_034:
						ctrlWriteParam.Diagnostics= 23;
						ctrlWriteParam.Set_AirValve  = Air_Flow_calc(ctrlWriteParam.Set_AirFlow);
					break;	
		case DG_040:
					break;		
		case DO_NOTHING:
					DAC_Output_Set(&DAC1,DAC_CHANNEL_1,DO_NOTHING,LTC2607_WR_AND_UPDATE); // AIR PV SET Function
					DAC_Output_Set(&DAC1,DAC_CHANNEL_2,DO_NOTHING,LTC2607_WR_AND_UPDATE); // O2 PV SET Function
					DAC_Output_Set(&DAC2,DAC_CHANNEL_1,DO_NOTHING,LTC2607_WR_AND_UPDATE);   // Exhal Valve
					GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, DO_NOTHING);
					GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, DO_NOTHING);	
					GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, DO_NOTHING);;
					ventAlgorithmSettings.vent_test = ctrlWriteParam.Diagnostics;
					break;
		default:
					break;
	
	}
}
