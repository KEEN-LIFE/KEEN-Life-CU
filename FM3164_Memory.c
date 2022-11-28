/*! \file **********************************************************************
 *
 *  \brief  Implementation of Module for FRAM(FM3164)
 *
 
 *  \author 
 *
 *  Copyright (c) 2021 CT Control Technology .
 ******************************************************************************/

//---[ Includes ]---------------------------------------------------------------

#include "FM3164_Memory.h"

//---[ Macros ]-----------------------------------------------------------------

//---[ Constants ]--------------------------------------------------------------

//---[ Public Variables ]-------------------------------------------------------

//SystemParameters sysParam;
//SystemParameters userParam;


calibrationvalues sysParam;
calibrationvalues userParam;


//---[ Private Variables ]------------------------------------------------------

//---[ Private Functions ]------------------------------------------------------

//---[ Function Implementations ]-----------------------------------------------



/*********************************************************************************************/
//fatory variable setting
//Block B
void factory_variable_Read(void)
{
	//Air PV equation limits
	sysParam.Air_PV_Valve_limits[0]=2.74;
	sysParam.Air_PV_Valve_limits[1]=15.69;
	sysParam.Air_PV_Valve_limits[2]=70.99;
	sysParam.Air_PV_Valve_limits[3]=98.06;
	sysParam.Air_PV_Valve_limits[4]=110.3;
	
	
	//Air PV coefficients
	sysParam.Air_PV_Valve[0]=-0.488;
	sysParam.Air_PV_Valve[1]=21.077;
	sysParam.Air_PV_Valve[2]=-338.555;
	sysParam.Air_PV_Valve[3]=2628.1;
	sysParam.Air_PV_Valve[4]=750.58;
	sysParam.Air_PV_Valve[5]= 0.0321;
	sysParam.Air_PV_Valve[6]=-5.4389;
	sysParam.Air_PV_Valve[7]=367.12;
	sysParam.Air_PV_Valve[8]=5918.5;
	sysParam.Air_PV_Valve[9]= -0.0062;
	sysParam.Air_PV_Valve[10]=65.255;
	sysParam.Air_PV_Valve[11]= 11400;
	sysParam.Air_PV_Valve[12]= 45.046;
	sysParam.Air_PV_Valve[13]= -9171.8;
	sysParam.Air_PV_Valve[14]= 484964;
	
	
	
	//O2 PV equation limits
	sysParam.O2_PV_Valve_limits[0]=2.45;
	sysParam.O2_PV_Valve_limits[1]=14.91;
	sysParam.O2_PV_Valve_limits[2]=71.37;
	sysParam.O2_PV_Valve_limits[3]=106.4;
	
	//Air PV coefficients
	sysParam.O2_PV_Valve[0]=-0.8281;
	sysParam.O2_PV_Valve[1]= 34.163;
	sysParam.O2_PV_Valve[2]= -514.5;
	sysParam.O2_PV_Valve[3]= 3638.2;
	sysParam.O2_PV_Valve[4]= -1958.6;
	sysParam.O2_PV_Valve[5]= 0.009;
	sysParam.O2_PV_Valve[6]= -1.9501;
	sysParam.O2_PV_Valve[7]=201.57;
	sysParam.O2_PV_Valve[8]=7789.2;
	sysParam.O2_PV_Valve[9]= 0.1348;
	sysParam.O2_PV_Valve[10]= -34.057;
	sysParam.O2_PV_Valve[11]=2929.3;
	sysParam.O2_PV_Valve[12]=-69198;
	
	//Air flow sensor limits
	sysParam.Air_flowsensor_limits[0]=0;
	
	//air flow sensor coefficients
	sysParam.Air_flowsensor[0]=0.0011;
	sysParam.Air_flowsensor[1]=1.1598;
	sysParam.Air_flowsensor[2]=0.1297;
	
	
	//o2 flow sensor limits
	sysParam.O2_flowsensor_limits[0]=0;
	
	//o2 flow sensor coefficients
	sysParam.O2_flowsensor[0]=1.1403;
	sysParam.O2_flowsensor[1]=0.1755;
	
	
	//total flow sensor limits
	sysParam.Total_flowsensor_limits[0]=0;

	//total flow sensor
	sysParam.Total_flowsensor[0]=1.1417;
	sysParam.Total_flowsensor[1]=0.2926;
	
	//exhalational flow sensor	
	sysParam.Exh_flowsensor[0]=0.0014;
	sysParam.Exh_flowsensor[1]= 1.1183;
	sysParam.Exh_flowsensor[2]= 0.3257;
	
	
	//Inspiratory pressure sensor
	sysParam.Insp_Presensor[0]=0.9686;
	sysParam.Insp_Presensor[1]=0.0333;
	
	//Expiratory pressure sensor
	sysParam.Exp_Presensor[0]=0.9686;
	sysParam.Exp_Presensor[1]= 0.0333;
	
	//DAC_Pressure coeffients
	sysParam.DAC_Peep[0]=235.87;
	sysParam.DAC_Peep[1]=3592.5;

}

/*********************************************************************************************/

void FRAM_DataStoring(void)
{
	
  /************************************************************************************************/
//	factory_variable_Read();//Reading function to access calbrated coefficients and limits
	
	uint8_t data_ptr[sizeof(sysParam)]; //declaring  array for storing iee value of the system parameters

	uint32_t float_HEX_temp=0, byte4_1=0; //declaration of iterating varible for iee conversions
	
	uint32_t *syspa_ptr= (uint32_t *)(&sysParam); //pointer to the sysParam structure,Accessing the size of calibration parameter structure
						
	
	//This is the loop to convert float value to the HEX value.
	for(uint32_t  sys_index_1 =0;sys_index_1< sizeof(sysParam)/4;sys_index_1++)
	{
		
		float_HEX_temp	= pack_IEEE754( *(syspa_ptr+sys_index_1));  //collecting values from the calibration structure
		
		*(data_ptr+ byte4_1++) = float_HEX_temp >> 24;
		*(data_ptr+ byte4_1++) = float_HEX_temp >> 16;
		*(data_ptr+ byte4_1++) = float_HEX_temp >> 8;
		*(data_ptr+ byte4_1++) = float_HEX_temp ;
		
	}
	
	//writting values to FRAM
	if(Mem_Set_Get_Parm_Func(&fRAM, 0x0000, sizeof(sysParam), (uint8_t*)&data_ptr, FRAM_MEM_WRITE) == MEM_SET_GET_OK)
	{
	}
	
}
	
	/*****************************************************************************************************************************/
void FRAM_DataReading(void)
{
	
	uint8_t data_ptr[sizeof(userParam)]; //declaring  array for storing iee value of the system parameters

	uint32_t float_HEX_temp=0, byte4_1=0; //declaration of iterating varible for iee conversions
	
	//this is the loop to make the temporary array zero for reading operation.
	for(int sys_index=0; sys_index < sizeof(sysParam);sys_index++)
		*(data_ptr+sys_index)=0;
	

	
	//Reading variables from FRAM
	if(Mem_Set_Get_Parm_Func(&fRAM, 0x0000, sizeof(userParam), (uint8_t*)&data_ptr, FRAM_MEM_READ) == MEM_SET_GET_OK)
	{
	}
		
	//this part of the code is to convert HEX value from the FRAM  to float value for further use	
	uint32_t *uspa_ptr= (uint32_t *)(&userParam);  //accessing pointer for user parameter calibration structure
	
	//Thisc the loop to convert HEX value to float va;ue
	for(int  sys_index =0;sys_index< sizeof(userParam);sys_index++)
	{
		
		float_HEX_temp  =  *(data_ptr+ byte4_1++) << 24;
		float_HEX_temp  =  float_HEX_temp | *(data_ptr+ byte4_1++) << 16;
		float_HEX_temp  =  float_HEX_temp | *(data_ptr+ byte4_1++) << 8;
		float_HEX_temp  =  float_HEX_temp | *(data_ptr+ byte4_1++);

		*(uspa_ptr + sys_index) = unpack_IEEE754( float_HEX_temp);	
	}
	
//end of function
}



/*! \brief  This function do the operation of memory set/get according to the function parameter
 *  \param  FRAM_memoryInfo Pointer to struct with info about the memory in FM3164 hardware to use
 *  \param  local_addrss represents the local address
 *  \param  byte represents the size of memory
 *  \param  data_reg pointer to carry address of desired data
 *  \param  param_sel select the mode of operation(eg: FRAM_MEM_READ)
 *  \returns  returns status of memory set/get function process(eg; MEM_SET_GET_OK)
 */

memSetGetStatus Mem_Set_Get_Parm_Func(FRAM_memoryInfo *FRAM_memoryInfo, uint32_t local_addrss, uint32_t byte, uint8_t *data_reg, uint32_t param_sel)
{
	  /* converting local address into MSB and LSB for writing commands*/
    FRAM_memoryInfo->local_addrss_MSB = (local_addrss >> 8) & 0xFF;
	  FRAM_memoryInfo->local_addrss_LSB = (local_addrss) & 0xFF;
	
    switch (param_sel)
	  {
	      case FRAM_MEM_READ:
		    {
					  /*SEND READ LOCATION TO FRAM*/					
		        ROM_I2CMasterSlaveAddrSet(FRAM_memoryInfo->i2c_base, MEMORY_SLAVE_ADDRESS, false);		 // setting the slave address and I2C is in Master Transmit Mode
				
				    ROM_I2CMasterDataPut(FRAM_memoryInfo->i2c_base, FRAM_memoryInfo->local_addrss_MSB);		 // setting the MSB of local address
				    ROM_I2CMasterControl(FRAM_memoryInfo->i2c_base, I2C_MASTER_CMD_BURST_SEND_START);      // start the state of the Master module send operation.
				    while (ROM_I2CMasterBusy(FRAM_memoryInfo->i2c_base));                                  // wait until I2C Master is not busy
				    FRAM_memoryInfo->err_status = ROM_I2CMasterErr(FRAM_memoryInfo->i2c_base);             // update the error status of Master module
					
				    ROM_I2CMasterDataPut(FRAM_memoryInfo->i2c_base,   FRAM_memoryInfo->local_addrss_LSB);  // setting the LSB of local address
				    ROM_I2CMasterControl(FRAM_memoryInfo->i2c_base, I2C_MASTER_CMD_BURST_SEND_FINISH);     // finish the state of the Master module send operation.
				    while (ROM_I2CMasterBusy(FRAM_memoryInfo->i2c_base));
				    FRAM_memoryInfo->err_status = ROM_I2CMasterErr(FRAM_memoryInfo->i2c_base);
					
				    /*READ DATA FROM FRAM*/	
					  ROM_I2CMasterSlaveAddrSet(FRAM_memoryInfo->i2c_base, MEMORY_SLAVE_ADDRESS, true);		  // setting the slave address and I2C is in Master Receive Mode
				
				    ROM_I2CMasterControl(FRAM_memoryInfo->i2c_base, I2C_MASTER_CMD_BURST_RECEIVE_START);
				    while (ROM_I2CMasterBusy(FRAM_memoryInfo->i2c_base));
				    FRAM_memoryInfo->err_status = ROM_I2CMasterErr(FRAM_memoryInfo->i2c_base);
				
					  for (; FRAM_memoryInfo->index < byte-1; (FRAM_memoryInfo->index)++)            // loop for reading the data from the local address
				    {
					      *data_reg = ROM_I2CMasterDataGet(FRAM_memoryInfo->i2c_base);                     // individual data will be updated to the data register pointer
					      data_reg++;
					      ROM_I2CMasterControl(FRAM_memoryInfo->i2c_base, I2C_MASTER_CMD_BURST_RECEIVE_CONT);
					      while (ROM_I2CMasterBusy(FRAM_memoryInfo->i2c_base));
					      FRAM_memoryInfo->err_status = ROM_I2CMasterErr(FRAM_memoryInfo->i2c_base);
				    }
				
				    *data_reg = ROM_I2CMasterDataGet(FRAM_memoryInfo->i2c_base);                         // update the last data available in given the memory
				    FRAM_memoryInfo->index = 0;
						ROM_I2CMasterControl(FRAM_memoryInfo->i2c_base, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
				    while (ROM_I2CMasterBusy(FRAM_memoryInfo->i2c_base));
				    FRAM_memoryInfo->err_status = ROM_I2CMasterErr(FRAM_memoryInfo->i2c_base);
				
		    }
		    break;
				
		    case FRAM_MEM_WRITE:
		    {   
					  /*SEND WRITE LOCATION TO FRAM*/	
		        ROM_I2CMasterSlaveAddrSet(FRAM_memoryInfo->i2c_base, MEMORY_SLAVE_ADDRESS, false); 		// setting the slave address and I2C is in Master Transmit Mode
				
				    ROM_I2CMasterDataPut(FRAM_memoryInfo->i2c_base, FRAM_memoryInfo->local_addrss_MSB);   // setting the MSB of local address
				    ROM_I2CMasterControl(FRAM_memoryInfo->i2c_base, I2C_MASTER_CMD_BURST_SEND_START);     // start the state of the Master module send operation.
				    while (ROM_I2CMasterBusy(FRAM_memoryInfo->i2c_base));                                 // wait until I2C Master is not busy
				    FRAM_memoryInfo->err_status = ROM_I2CMasterErr(FRAM_memoryInfo->i2c_base);            // update the error status of Master module
				
				    ROM_I2CMasterDataPut(FRAM_memoryInfo->i2c_base, FRAM_memoryInfo->local_addrss_LSB);   // setting the MSB of local address
				    ROM_I2CMasterControl(FRAM_memoryInfo->i2c_base, I2C_MASTER_CMD_BURST_SEND_CONT);      // continue the state of the Master module send operation.
				    while (ROM_I2CMasterBusy(FRAM_memoryInfo->i2c_base));
				    FRAM_memoryInfo->err_status = ROM_I2CMasterErr(FRAM_memoryInfo->i2c_base);
				
						/*SEND DATA TO FRAM*/
				    for (; (FRAM_memoryInfo->index) < (byte-1); (FRAM_memoryInfo->index)++)            // loop for writing the data to the local address
				    {
					      ROM_I2CMasterDataPut(FRAM_memoryInfo->i2c_base, *data_reg);
					      data_reg++;                                                                      // incrementing data register pointer address
					      ROM_I2CMasterControl(FRAM_memoryInfo->i2c_base, I2C_MASTER_CMD_BURST_SEND_CONT);
					      while (ROM_I2CMasterBusy(FRAM_memoryInfo->i2c_base));
					      FRAM_memoryInfo->err_status = ROM_I2CMasterErr(FRAM_memoryInfo->i2c_base);
				    }
				
				    ROM_I2CMasterDataPut(FRAM_memoryInfo->i2c_base, *data_reg);
						FRAM_memoryInfo->index = 0;
				    ROM_I2CMasterControl(FRAM_memoryInfo->i2c_base, I2C_MASTER_CMD_BURST_SEND_FINISH);
				    while (ROM_I2CMasterBusy(FRAM_memoryInfo->i2c_base));
				    FRAM_memoryInfo->err_status = ROM_I2CMasterErr(FRAM_memoryInfo->i2c_base);

						
		    }
		    break;
		
		    default:
			      break;
	}
	
	return MEM_SET_GET_OK;                                                                          // returns the status of memory set/get function process to the function call   
}
