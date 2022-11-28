/*! \file **********************************************************************
 *
 *  \brief  Implementation of Module for Flow Sensors
 *
 
 *  \author 
 *
 *  Copyright (c) 2021 CT Control Technology .
 ******************************************************************************/

//---[ Includes ]---------------------------------------------------------------

#include "SFM4200_FlowSensor.h"

//---[ Macros ]-----------------------------------------------------------------

//---[ Constants ]--------------------------------------------------------------

//---[ Public Variables ]-------------------------------------------------------

flowSensorStatus flowErrorStatus;

uint8_t FlowSensorReset;

//---[ Private Variables ]------------------------------------------------------

//---[ Private Functions ]------------------------------------------------------

//---[ Function Implementations ]-----------------------------------------------


/*! \brief  This function reads the flow data throgh flow sensor module
 *  \param  flowSensorInfo Pointer to struct with info about the flow sensor hardware to use
 *  \param  flowRate Pointer to flow rate data
 *  \returns  returns status of flowSensor read status(eg; FLOW_READ_OK)
 */

flowSensorStatus sfm4200_flow_read(flowSensorInfo *flowSensorInfo,float_t * flowRate)
{
	
	/* Flow data fetch by using TM4C123x I2C API */
    ROM_I2CMasterSlaveAddrSet(flowSensorInfo->i2c_base, SFM4200_SLAVE_ADDR, false);         // setting slave address and I2C is in Master Transmit Mode
	  ROM_I2CMasterDataPut(flowSensorInfo->i2c_base, 0x10);                                   //setting flow register measurement command(0x1000), set MSB first
	  ROM_I2CMasterControl(flowSensorInfo->i2c_base, I2C_MASTER_CMD_BURST_SEND_START);        //start Master module send operation.
	
	  while(ROM_I2CMasterBusy(flowSensorInfo->i2c_base));                                     //check whether I2C Master is busy transmitting or receiving data.
	  flowSensorInfo->flowSensor_err_status = ROM_I2CMasterErr(flowSensorInfo->i2c_base);              // returns the error status of Master module

	
	  I2CMasterDataPut(flowSensorInfo->i2c_base, 0x00);												                // setting LSB Byte 
	  I2CMasterControl(flowSensorInfo->i2c_base, I2C_MASTER_CMD_BURST_SEND_FINISH);           // finish Master module send operation.

	  while(I2CMasterBusy(flowSensorInfo->i2c_base)); 
	  flowSensorInfo->flowSensor_err_status = I2CMasterErr(flowSensorInfo->i2c_base);

	
	  ROM_I2CMasterSlaveAddrSet(flowSensorInfo->i2c_base, SFM4200_SLAVE_ADDR, true);	        // setting slave address and I2C is in Master Receive Mode
	  ROM_I2CMasterControl(flowSensorInfo->i2c_base, I2C_MASTER_CMD_BURST_RECEIVE_START);     // start Master module receive operation.
	
	  while (ROM_I2CMasterBusy(flowSensorInfo->i2c_base)); 
	  flowSensorInfo->flowSensor_err_status = ROM_I2CMasterErr(flowSensorInfo->i2c_base); 

	
	  flowSensorInfo->i2c_rxBuffer[0] = ROM_I2CMasterDataGet(flowSensorInfo->i2c_base);       // receives MSB of flow data from flow register
	  ROM_I2CMasterControl(flowSensorInfo->i2c_base, I2C_MASTER_CMD_BURST_RECEIVE_CONT);      // continue Master module receive operation.
	
	  while (ROM_I2CMasterBusy(flowSensorInfo->i2c_base)); 
	  flowSensorInfo->flowSensor_err_status = ROM_I2CMasterErr(flowSensorInfo->i2c_base); 

	  flowSensorInfo->i2c_rxBuffer[1] = ROM_I2CMasterDataGet(flowSensorInfo->i2c_base);       // receives LSB of flow data from flow register
	  ROM_I2CMasterControl(flowSensorInfo->i2c_base, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);    //finish Master module receive operation.
	
	  while (ROM_I2CMasterBusy(flowSensorInfo->i2c_base)); 
	  flowSensorInfo->flowSensor_err_status = ROM_I2CMasterErr(flowSensorInfo->i2c_base); 

	
	  flowSensorInfo->i2c_rxBuffer[2] = ROM_I2CMasterDataGet(flowSensorInfo->i2c_base);       // receives CRC byte of flow data from flow register
	
	
	/* 8 bit crc check in between received crc and calculated crc */
	/* 8 bit crc check in between received crc and calculated crc */
	  if(!crc8CheckUpdateSF05((uint8_t *)flowSensorInfo->i2c_rxBuffer,2,flowSensorInfo->i2c_rxBuffer[2]))
	  {
		    flowSensorInfo->flowRead = flowSensorInfo->i2c_rxBuffer[0]; 
		    flowSensorInfo->flowRead <<= 8;  
		    flowSensorInfo->flowRead |= flowSensorInfo->i2c_rxBuffer[1];
		    *flowRate = ((float)flowSensorInfo->flowRead/256); //flow data converting to predefined unit(SLM)
				flowSensorInfo->flowSensorError_cnt=0; //making flow sensor error count to zero
			return FLOW_READ_OK;
			
	  }
	  else
	  {
			//error_test++;
			flowSensorInfo->flowSensorError_cnt++; //counting flow sensor  error 
			
			if(flowSensorInfo->flowSensorError_cnt >= 50 ) //checking for continous fail of flow sensor data acquisition
			{				
				flag_50ms=1; //making 50ms flag high 
			//	error_test=0;
			}
			if(flag_50ms && FlowSensorReset)
			{
				ROM_GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5,false); //resetting the flow sensor by giving 50ms low pulse 
				flowSensorInfo->flowSensorError_cnt=0;
			}
			else
			{
				ROM_GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5,GPIO_PIN_5);
			}
			return FLOW_READ_CRC_CHECK_ERR;      // returns the CRC check error
		
	  }
	
}


void FlowSensor_dataAcq(void)
{
	uint32_t uIndex = 0;
	
	if(sfm4200_flow_read(&AirFlowSensor,(float_t*)&flow_read_temp) == FLOW_READ_OK)
		{
			if(flow_read_temp != 1)
			{
				Air_FS_Commn_Flag = 0;
				Tech_Alarmcheck.air_flow_sensor=0;
				AirFS_Count = 0;   // for non-consecutive failure occurence 
				
				fAverageArray_Air[u32AverageIndex_Air++] = flow_read_temp;           // Calculating the Running average
				
				fAverage = 0;
				
				if(u32AverageIndex_Air >= 10)
				{
					u32AverageIndex_Air = 0;
				}
			
				for(uIndex = 0; uIndex < 10; uIndex++)
				{
					fAverage += fAverageArray_Air[uIndex]; 
				}
			
				ctrlReadParam.Air_Flow = (float)fAverage/10;
				
				/* Recent Error Calc(28/03/2021)*/
//				if(ctrlReadParam.Air_Flow >sysParam.Air_flowsensor_limits[0])
				ctrlReadParam.Air_Flow =  sysParam.Air_flowsensor[0]*	pow(ctrlReadParam.Air_Flow,2)+ sysParam.Air_flowsensor[1]*ctrlReadParam.Air_Flow+sysParam.Air_flowsensor[2];
									
			}
			else
			{
				AirFS_Count++; // No. of unsuccessive AirFS read to generate FS Failure Alarm				
			}			
		}
		else
		{
			AirFS_Count++; // No. of unsuccessive AirFS read to generate FS Failure Alarm				
		}	
		
		if(AirFS_Count >= 40)
		{
			Air_FS_Commn_Flag = 1; // Alarm Gen. Purpose
			Tech_Alarmcheck.air_flow_sensor=1;
//			AirFS_Count = 0;				
		}

	if(sfm4200_flow_read(&O2FlowSensor, (float_t*)&flow_read_temp) == FLOW_READ_OK)
		{
			if(flow_read_temp != 1)
			{
				O2FS_Count = 0; // for non-consecutive failure occurence
				O2_FS_Commn_Flag = 0;	
				Tech_Alarmcheck.o2_flow_sensor=0; // alarm clearing
				fAverageArray_O2[u32AverageIndex_O2++] = flow_read_temp;           // Calculating the Running average
			
				fAverage = 0;
				
				if(u32AverageIndex_O2 >= 10)
				{
					u32AverageIndex_O2 = 0;
				}
			
				for(uIndex = 0; uIndex < 10; uIndex++)
				{
					fAverage += fAverageArray_O2[uIndex]; 
				}
						
				ctrlReadParam.O2_Flow = (float)fAverage/10;
				
				ctrlReadParam.O2_Flow = sysParam.O2_flowsensor[0]*ctrlReadParam.O2_Flow + sysParam.O2_flowsensor[1];

			}
			else
			{
			 O2FS_Count++;	
			}			
		}
		else
		{
			O2FS_Count++;	
		}			
		if(O2FS_Count >= 40)
		{
			O2_FS_Commn_Flag = 1; // Alarm Gen. Purpose
			Tech_Alarmcheck.o2_flow_sensor=1;
//			O2FS_Count = 0;
		}
		
		if(sfm4200_flow_read(&TotalFlowSensor, (float_t*)&flow_read_temp) == FLOW_READ_OK)
		{
			if(flow_read_temp != 1)
			{
				TotFS_Count = 0; // for non-consecutive failure occurence
				Total_FS_Commn_Flag = 0;
			  Tech_Alarmcheck.total_flow_sensor=0;//alarm clearing

				fAverageArray_Total[u32AverageIndex_Total++] = flow_read_temp;           // Calculating the Running average
			
				fAverage = 0;
				
				if(u32AverageIndex_Total >= 10)
				{
					u32AverageIndex_Total = 0;
				}
			
				for(uIndex = 0; uIndex < 10; uIndex++)
				{
					fAverage += fAverageArray_Total[uIndex]; 
				}
						
				ctrlReadParam.Total_Flow = (float)fAverage/10;

        ctrlReadParam.Total_Flow = sysParam.Total_flowsensor[0]*ctrlReadParam.Total_Flow +sysParam.Total_flowsensor[1];
			}
			else
			{
				TotFS_Count++;
			}
		}
		else
		{
			TotFS_Count++;
		}
		
		if(TotFS_Count >= 40)
		{
			Total_FS_Commn_Flag = 1; // Alarm Gen.Purpose
			Tech_Alarmcheck.total_flow_sensor=1;
//			TotFS_Count = 0;				
		}
		
//		FlowSensorSerialPacketization();
}




void FlowSensorSerialPacketization()
{
	unsigned int uindex = 0;
	unsigned char packet[50] = {0};
	
		packet[uindex++] = '$';
		packet[uindex++] = '$';
//		packet[uindex++] = (flow_read[AIR_FLOW]/10000)+0x30;
//		packet[uindex++] = ((flow_read[AIR_FLOW]/1000)%10)+0x30;
//		packet[uindex++] = ((flow_read[AIR_FLOW]/100)%10)+0x30;
//		packet[uindex++] = ((flow_read[AIR_FLOW]/10)%10)+0x30;
//		packet[uindex++] = (flow_read[AIR_FLOW]%10)+0x30;
//		packet[uindex++] = '*';
//		packet[uindex++] = '*';
//		packet[uindex++] = (flow_read[O2_FLOW]/10000)+0x30;	
//		packet[uindex++] = ((flow_read[O2_FLOW]/1000)%10)+0x30;
//		packet[uindex++] = ((flow_read[O2_FLOW]/100)%10)+0x30;
//		packet[uindex++] = ((flow_read[O2_FLOW]/10)%10)+0x30;
//		packet[uindex++] = (flow_read[O2_FLOW]%10)+0x30;
//		packet[uindex++] = '*';
//		packet[uindex++] = '*';
//		packet[uindex++] = (flow_read[TOTAL_FLOW]/10000)+0x30;
//		packet[uindex++] = ((flow_read[TOTAL_FLOW]/1000)%10)+0x30;
//		packet[uindex++] = ((flow_read[TOTAL_FLOW]/100)%10)+0x30;
//		packet[uindex++] = ((flow_read[TOTAL_FLOW]/10)%10)+0x30;
//		packet[uindex++] = (flow_read[TOTAL_FLOW]%10)+0x30;
		packet[uindex++] = '$';		
		packet[uindex++] = '$';	
		packet[uindex++] = '\n';
	

	UARTFlushTx(1);
	UARTwrite((const char*)packet,uindex);		
	
}

