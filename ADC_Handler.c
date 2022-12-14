/*! \file **********************************************************************
 *
 *  \brief  Implementation of Module for Flow Sensors
 *
 
 *  \author 
 *
 *  Copyright (c) 2021 CT Control Technology .
 ******************************************************************************/

//---[ Includes ]---------------------------------------------------------------

#include "ADC_Handler.h"

//---[ Macros ]-----------------------------------------------------------------

//---[ Constants ]--------------------------------------------------------------

//---[ Public Variables ]-------------------------------------------------------

	//uint32_t ulADC0_Value[10];
volatile	uint32_t ADC0_Value1[8],ADC1_Value1[8];
volatile	uint32_t ADC0_Value2[4];
volatile	uint32_t O2_Pressure_Pa = 0;
volatile	uint32_t O2_Pressure_cmH2O = 0;
float xy = 0;
//extern 	volatile uint32_t Insp_Pressure,Aux_Pressure,Sense_VG_15,Sense_VG_12,Sense_VG_5,Sense_Buzzer,Sense_EV,Sense_OV,Sense_AV;
extern volatile uint32_t Insp_Pressure_cmH20, Aux_Pressure_cmH20;

uint32_t adc_sample=0;

//---[ Private Variables ]------------------------------------------------------

//---[ Private Functions ]------------------------------------------------------

//---[ Function Implementations ]-----------------------------------------------

void analog_channel_init()
{
  
	/* configuring sampling rate of adc */
	
	ROM_SysCtlADCSpeedSet(SYSCTL_ADCSPEED_125KSPS);  //	sampling rate of 250kbps
		
	/* Enable the ADC peripherals and the associated GPIO port */
	
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
 
	
	/* Configure the pins to be used as analog inputs */
	
	ROM_GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 );
	ROM_GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 );
	ROM_GPIOPinTypeADC(GPIO_PORTB_BASE, GPIO_PIN_4 );
	 
	
	/* Select the internal reference for greatest accuracy */
	
	ROM_ADCReferenceSet(ADC0_BASE, ADC_REF_INT);


	/* Initialize both ADC peripherals using SS0, SS1 and processor trigger */
		
	ROM_ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
	ROM_ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 1);


  /*Configuring over sampling of adc as x64 */
	
	ROM_ADCHardwareOversampleConfigure(ADC0_BASE,16);         //over sampling at x16

	
	ROM_ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH0); //Inspiratory_Pressure
  ROM_ADCSequenceStepConfigure(ADC0_BASE, 0, 1, ADC_CTL_CH1);	//Aux_Pressure
  ROM_ADCSequenceStepConfigure(ADC0_BASE, 0, 2, ADC_CTL_CH2);	//+15VA_SENSE
	ROM_ADCSequenceStepConfigure(ADC0_BASE, 0, 3, ADC_CTL_CH3);	//+12V_SENSE	
	ROM_ADCSequenceStepConfigure(ADC0_BASE, 0, 4, ADC_CTL_CH4); //Future_Valve_Sense1
	ROM_ADCSequenceStepConfigure(ADC0_BASE, 0, 5, ADC_CTL_CH5);	//+5V_SENSE	
	ROM_ADCSequenceStepConfigure(ADC0_BASE, 0, 6, ADC_CTL_CH6);	//BUZZER_SENSE		
	ROM_ADCSequenceStepConfigure(ADC0_BASE, 0, 7, ADC_CTL_CH7 | ADC_CTL_IE | ADC_CTL_END);	//EXH_VALVE_SENSE
	
	ROM_ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_CH8); //O2_VALVE_SENSE
	ROM_ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_CH9); //AIR_VALVE_SENSE
	ROM_ADCSequenceStepConfigure(ADC0_BASE, 1, 2, ADC_CTL_CH10); //Exp_Pressure 
	ROM_ADCSequenceStepConfigure(ADC0_BASE, 1, 3, ADC_CTL_TS | ADC_CTL_IE | ADC_CTL_END); 
	
	ROM_ADCSequenceEnable(ADC0_BASE, 0);
  ROM_ADCIntClear(ADC0_BASE, 0);
	ROM_ADCSequenceEnable(ADC0_BASE, 1);
  ROM_ADCIntClear(ADC0_BASE, 1);
	
	
}

void adc_conversion()
{
  uint32_t uIndex = 0;
	
	
	ADCIntClear(ADC0_BASE, 0);
	ADCProcessorTrigger(ADC0_BASE, 0);
	
	ADCIntClear(ADC0_BASE, 1);
	ADCProcessorTrigger(ADC0_BASE, 1);
	
  /* Wait for conversion to be completed */
     
  while(!ADCIntStatus(ADC0_BASE, 0, false))
  {
  }
			
	ADCSequenceDataGet(ADC0_BASE, 0,(uint32_t *)ADC0_Value1);
	

//	if(ADC0_Value1[0] < 1240)
//	{
//		ADC0_Value1[0] = 1240;
//	}
	/* Condition for Insp Alarm Gen.*/
	
	if(ADC0_Value1[0] < 200 || ADC0_Value1[0] > 2250)   // Max Possible pressure for HSCDRRN160MDAA% is 160cmH2O
	{
		Insp_Pressure_Failure = 1;
		Tech_Alarmcheck.insp_pr_sensor=1;
	}
	else
	{
		Insp_Pressure_Failure = 0;
		Tech_Alarmcheck.insp_pr_sensor=0;
	}

	/*Insp Pressure Averaging the data*/
	
	fAvgeArray_InspPressure[u32AvgIndex_InspPressure++] = (((float)ADC0_Value1[0] - 1285)/6.1168);	
	fAvg = 0;
				
	if(u32AvgIndex_InspPressure >= 15)
	{
		u32AvgIndex_InspPressure = 0;
	}
			
	for(uIndex = 0; uIndex < 15; uIndex++)
	{
		fAvg += fAvgeArray_InspPressure[uIndex]; 
	}
			
	ctrlReadParam.Insp_Pressure = (float)fAvg/15;	
//	ctrlReadParam.Insp_Pressure=(((float)ADC0_Value1[0] - 1263)/6.1168);
	ctrlReadParam.Insp_Pressure = sysParam.Insp_Presensor[0]*ctrlReadParam.Insp_Pressure + sysParam.Insp_Presensor[1]; // Calib on 03/02/22
//	ctrlServParam.Insp_Pressure =  	ctrlReadParam.Insp_Pressure;

//	if(ADC0_Value1[1] < 1249)
//	{
//		ADC0_Value1[1] = 1249;
//	}
//	
	/* Condition for Aux Alarm Gen.*/
	if((ADC0_Value1[1] < 1249) || (ADC0_Value1[1] > 2242))   // Max Possible pressure for HSCDRRN005PDAA5 is 351.535cmH2O
	{
		Aux_Pressure_Failure = 1;
		Tech_Alarmcheck.aux_pr_sensor=1;
	}
	else
	{
		Aux_Pressure_Failure = 0;	
		Tech_Alarmcheck.aux_pr_sensor=0;
		
	}
	
	/*Aux Pressure Averaging the data*/
	
	fAvgeArray_AuxPressure[u32AvgIndex_AuxPressure++] = ((((float)ADC0_Value1[1] -1285))/2.83571);    // Calculating the Running average
				
	fAvg = 0;
				
	if(u32AvgIndex_AuxPressure >= 10)
	{
		u32AvgIndex_AuxPressure = 0;
	}
			
	for(uIndex = 0; uIndex < 10; uIndex++)
	{
		fAvg += fAvgeArray_AuxPressure[uIndex]; 
	}
			
	ctrlReadParam.Aux_Pressure = (float)fAvg/10;	
//	ctrlServParam.Aux_Pressure =  	ctrlReadParam.Aux_Pressure;


	ctrlReadParam.VG_Sense_18V =(float)(ADC0_Value1[2]*7.52*0.8/1000);

	ctrlReadParam.VG_Sense_12V = (float)(ADC0_Value1[3]*7.521*0.8/1000);

	ctrlReadParam.VG_Sense_5V = (float)(ADC0_Value1[5]*2.521*0.8/1000);
	
	ctrlReadParam.EV_PV_Sense = (float)(ADC0_Value1[7]*0.8);
	
	ADCIntClear(ADC0_BASE, 1);
	ADCProcessorTrigger(ADC0_BASE, 1);
	
  /* Wait for conversion to be completed */     
	while(!ADCIntStatus(ADC0_BASE, 1, false))
  {			
  }

	
	ADCSequenceDataGet(ADC0_BASE, 1, (uint32_t *)ADC0_Value2);
	
	ctrlReadParam.O2_PV_Sense = (float)(ADC0_Value2[0] *0.8);
	ctrlReadParam.Air_PV_Sense = (float)(ADC0_Value2[1] *0.8); 
	

//	if(ADC0_Value2[0] < 1239)
//	{
//		ADC0_Value2[0] = 1239;
//	}
	
	/* Condition for Exp Alarm Gen.*/	
	if( ADC0_Value2[2] < 200 || ADC0_Value2[2] > 2250)    // Max Possible pressure for HSCDRRN160MDAA% is 160cmH2O
	{
		Exp_Pressure_Failure = 1;
		Tech_Alarmcheck.exp_pre_sensor=1; //exp pressure is not connected to control board.
	}
	else
	{
		Exp_Pressure_Failure = 0;	
		Tech_Alarmcheck.exp_pre_sensor=0;

	}
	
	
	/*Exp Pressure Averaging the data*/
		
	fAvgeArray_ExpPressure[u32AvgIndex_ExpPressure++] = (((float)ADC0_Value1[0]-1285)/6.1168);         	// Calculating the Running average
				
	fAvg = 0;
				
	if(u32AvgIndex_ExpPressure >= 10)
	{
		u32AvgIndex_ExpPressure = 0;
	}
			
	for(uIndex = 0; uIndex < 10; uIndex++)
	{
		fAvg += fAvgeArray_ExpPressure[uIndex]; 
	}
			
	ctrlReadParam.Exp_Pressure = (float)fAvg/10;	
	ctrlReadParam.Exp_Pressure = sysParam.Exp_Presensor[0]*ctrlReadParam.Exp_Pressure +sysParam.Exp_Presensor[1]; // Calib on 06/04/22
//	ctrlServParam.Exp_Pressure =  	ctrlReadParam.Exp_Pressure;
	
/* TEMP = 147.5 - ((75 * (VREFP - VREFN) ? ADCCODE) / 4096)	 */
	ctrlReadParam.Board_Temp = (float)147.5 - (75 * 3.3 *ADC0_Value2[3])/4096;	
		
	ctrlReadParam.Ins_Zero_Valve_Sense   = (float)(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_5)); //reading inspiratory zero valve feedback
	ctrlReadParam.Aux_Valve_Sense        = (float)(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4)); //reading auxiliary valve feedback
	ctrlReadParam.Ins_Effort_Valve_Sense = (float)(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_2)); //reading inspiratory effort valve feedback

	
}

void ADC_DataAcq()
{
	adc_conversion();
//	adcSerialPacketization();
}

