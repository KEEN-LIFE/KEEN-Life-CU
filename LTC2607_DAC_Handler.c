/*! \file **********************************************************************
 *
 *  \brief  Implementation of Module for Flow Sensors
 *
 *  \author 
 *
 *  Copyright (c) 2021 CT Control Technology .
 ******************************************************************************/

//---[ Includes ]---------------------------------------------------------------

#include "LTC2607_DAC_Handler.h"

//---[ Macros ]-----------------------------------------------------------------

//---[ Constants ]--------------------------------------------------------------

//---[ Public Variables ]-------------------------------------------------------

//---[ Private Variables ]------------------------------------------------------

//---[ Private Functions ]------------------------------------------------------

//---[ Function Implementations ]-----------------------------------------------

/*! \brief  This function sets the DAC output
 *  \param  DACInfo Pointer to struct with info about the DAC hardware to use
 *  \param  DAC_Num represents the type of DAC(DAC A, DAC B or Both)
 *  \param  Output_Value decimal equivalent value to be written to DAC
 *  \param  DAC_Function operation to be done(Write to Input Register, Write to and Update..etc).
 *  \returns  returns status of dac output set process(eg; DAC_OUTPUT_SET_OK)
 */

dacStatus DAC_Output_Set(DACInfo *DACInfo,uint32_t DAC_Num,uint32_t Output_Value,uint16_t DAC_Function)
{
	/* Updating DAC parameter wrt input values*/
	DACInfo->outputVal = Output_Value;
	DACInfo->dacFun = DAC_Function;
	
	/* It will create the Write Word for LTC2607 DAC */
	/* A write operation loads a 16-bit data word from the 32-bit shift register into the input register of the selected DAC*/	
	/* update operation copies the data word from the input register to the DAC register.*/

	if(DAC_Function != LTC2607_DAC_POWER_DOWN)
	{
		
		switch(DAC_Num)
		{
				case DAC_CHANNEL_1         :
															       DACInfo->dataPacket[0] = DAC_Function | LTC2607_REGADDR_DAC_A ;    // Command and Address Code Generation
 															       break;
				case DAC_CHANNEL_2         :
															       DACInfo->dataPacket[0] = DAC_Function | LTC2607_REGADDR_DAC_B ;    // Command and Address Code Generation
															       break;
				case DAC_CHANNEL_BOTH      :
						                         DACInfo->dataPacket[0] = DAC_Function | LTC2607_REGADDR_DAC_BOTH ; // Command and Address Code Generation
															       break;
				default:
									                   break;	
		}
					
		DACInfo->dataPacket[1] = ( DACInfo->outputVal >> 8 ) ; //MSB storing into index 1 of dataPacket array
		DACInfo->dataPacket[2] = ( DACInfo->outputVal & 0x0FF ) ; ////LSB storing into index 1 of dataPacket array
				
  } 
	else
	{
		return DAC_IN_POWER_DOWN_MODE; //return the status if DAC is in Power Down mode
	}
	
	/* Generating the DAC output data */
	/* Data word written in the DAC register becomes an active 16-bit input code */
	/* Input code converted to an analog voltage at the DAC output */
	
	
	I2CMasterSlaveAddrSet(DACInfo->i2c_base,DACInfo->dacAddrs , false);  // setting the slave address and I2C is in Master Transmit Mode
	I2CMasterDataPut(DACInfo->i2c_base, DACInfo->dataPacket[0]);               // setting the Command and Address Code
	I2CMasterControl(DACInfo->i2c_base, I2C_MASTER_CMD_BURST_SEND_START);      // start the state of the Master module send operation.
	
	while(ROM_I2CMasterBusy(DACInfo->i2c_base));                               //check whether the I2C Master is busy in transmitting or receiving data.
	DACInfo->dac_err_status = ROM_I2CMasterErr(DACInfo->i2c_base);               // returns the error status of the Master module
//	if(DACInfo->dac_err_status != I2C_MASTER_ERR_NONE)                           // check the status of error
//	{              
//	    return DAC_OUTPUT_I2C_BUS_ERR;                                                   // returns the error status to function call
//	}
	I2CMasterDataPut(DACInfo->i2c_base, DACInfo->dataPacket[1]);							 // setting MSB Byte of Write word
	I2CMasterControl(DACInfo->i2c_base, I2C_MASTER_CMD_BURST_SEND_CONT);       //continue Master module transmit operation.
	
	while(ROM_I2CMasterBusy(DACInfo->i2c_base));                               
	DACInfo->dac_err_status = ROM_I2CMasterErr(DACInfo->i2c_base);               
//	if(DACInfo->dac_err_status != I2C_MASTER_ERR_NONE)                           // check the status of error
//	{              
//	    return DAC_OUTPUT_I2C_BUS_ERR;                                                   // returns the error status to function call
//	}
				
	I2CMasterDataPut(DACInfo->i2c_base, DACInfo->dataPacket[2]);							 // setting LSB Byte of Write word
	I2CMasterControl(DACInfo->i2c_base, I2C_MASTER_CMD_BURST_SEND_FINISH);     // finish Master module transmit operation.
				
	while(ROM_I2CMasterBusy(DACInfo->i2c_base));                               
	DACInfo->dac_err_status = ROM_I2CMasterErr(DACInfo->i2c_base);                                         
//	if(DACInfo->dac_err_status != I2C_MASTER_ERR_NONE)                           // check the status of error
//	{              
//	    return DAC_OUTPUT_I2C_BUS_ERR;                                                   // returns the error status to function call
//	}
	
//	DAC_Set_Alarm(&DACInfo,DAC_Num,Output_Value);

//if(*DACInfo == &DAC2)
//{
//}
	
	return DAC_OUTPUT_SET_OK;                                                 // returns dac output status
		
}

void DAC_Set_Alarm(DACInfo *DACInfo, uint32_t DAC_Num,uint32_t Output_Value)
{
	
}
