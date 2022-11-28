/*! \file **********************************************************************
 *
 *  \brief 
 *
 
 *  \author 
 *
 *  Copyright (c) 2021 CT Control Technology .
 ******************************************************************************/


//---[ Includes ]---------------------------------------------------------------

#include "ModbusCommn_Handler.h"


//---[ Macros ]-----------------------------------------------------------------



//---[ Constants ]--------------------------------------------------------------


//---[ Types ]------------------------------------------------------------------


//---[ Private Functions ]------------------------------------------------------


//---[ Public Variables ]-------------------------------------------------------
uint16_t m_test=0;
//---[ Private Function Definitions ]-------------------------------------------
//SP START
/*
read RX Data
add to bUffer Array
call respondable functions
	- read the sl id
	- read fC
	- wrt. the above 2, determine the lenth of balance packet to be received
	- check if the current received packet is == calculatd packet above
	--true = process
	--false= continue to recive the packests till the calculated number of packets is recived
	- DO NOT REMOVE FROM THE BUFFER UNTIL  ALL THE REQUIRED PACKETS ARE RECIVED
	- if in an iteration a packet is not received start a timer and check for timeout
	- if timeout and the received packet is not functionallly usable EMPTY  the entire data.
if acceptable 	
	- clear buffer
else
	- go again and get the data
do the same
*/
//SP STOP

modBusCommnStatus Modbus_Communication(void)
{
	unsigned long ulLength = 0, ulMinLength = 0, ulIndex = 0, ulCmdStatus = 0;
	unsigned char ucRxdPacket[200] = {0}, i = 0;
//	unsigned char i = 0;
	unsigned short crc_calculated = 0, crc_received = 0, usNumAddr = 0;
	unsigned int ulSlaveId = 0, ulFunctionCode = 0;
	modBusCommnStruct.Error_Status = 0;
	
	if (UARTRxBytesAvail())
	{
		if(UARTRxBytesAvail() > 24)
     Delay_Millis(20);
		
		DU_CMD_InitTimerFlag = 1;
		do
		{
			ucBufferArray[ulBuffIndex++] = UARTgetc();	
		} while(UARTRxBytesAvail());		
		
		SPulIndex_page++;	
	}
	
	ulLength =  ulBuffIndex;
	
	if((ulLength == 8))// && (READ_CMD_TimeoutFlag == 1))
	{
		READ_CMD_TimeoutFlag = 0;
		DU_CMD_InitTimerFlag = 0;
		WR_SIN_CMD_TimeoutFlag = 0;
		WR_MUL_CMD_TimeoutFlag = 0;
		WRITE_SIN_TimeoutCounter = 0;
		WRITE_MUL_TimeoutCounter = 0;
		
		if((ucBufferArray[0] == CONTROL_BOARD_ID) && (ucBufferArray[1] == MODBUS_READ))
			{
				for(i=0;i<ulLength;i++)
				{
					ucRxdPacket[i] = ucBufferArray[i];				
				}
						
				for(i=0;i<ulLength;i++)
				ucBufferArray[i] = 0;				
				ulBuffIndex = 0;
				Flush_UARTRxBufferArray();
			}
			else
			{
				for(i=0;i<ulLength;i++)
				ucBufferArray[i] = 0;				
				ulBuffIndex = 0;
				Flush_UARTRxBufferArray();
			}			
	}
	
	if((ulLength == 12))//&&(WR_SIN_CMD_TimeoutFlag == 1))
	{
		WR_SIN_CMD_TimeoutFlag = 0;
		DU_CMD_InitTimerFlag = 0;
		READ_TimeoutCounter = 0;
		READ_CMD_TimeoutFlag = 0;
		WR_MUL_CMD_TimeoutFlag = 0;		
		WRITE_MUL_TimeoutCounter = 0;
		
		if((ucBufferArray[0] == CONTROL_BOARD_ID) && (ucBufferArray[1] == MODBUS_WRITE_SINGLE))
			{
				for(i=0;i<ulLength;i++)
				{
					ucRxdPacket[i] = ucBufferArray[i];				
				}
						
				for(i=0;i<ulLength;i++)
				ucBufferArray[i] = 0;
				
				ulBuffIndex = 0;
				Flush_UARTRxBufferArray();
			}
			else
			{
				for(i=0;i<ulLength;i++)
				ucBufferArray[i] = 0;				
				ulBuffIndex = 0;
				Flush_UARTRxBufferArray();
			}								
	}

	if(12 < ulLength)//&&(WR_MUL_CMD_TimeoutFlag == 1))
	{
		WR_MUL_CMD_TimeoutFlag = 0;
		DU_CMD_InitTimerFlag = 0;
		READ_CMD_TimeoutFlag = 0;
		WR_SIN_CMD_TimeoutFlag = 0;		
		READ_TimeoutCounter = 0;
		WRITE_SIN_TimeoutCounter = 0;
		
		if((ucBufferArray[0] == CONTROL_BOARD_ID) && (ucBufferArray[1] == MODBUS_WRITE_MULTIPLE))
			{
				for(i=0;i<ulLength;i++)
				{
					ucRxdPacket[i] = ucBufferArray[i];				
				}
						
				for(i=0;i<ulLength;i++)
				ucBufferArray[i] = 0;
				
				ulBuffIndex = 0;
				Flush_UARTRxBufferArray();
			}
			else
			{
				for(i=0;i<ulLength;i++)
				ucBufferArray[i] = 0;				
				ulBuffIndex = 0;
				Flush_UARTRxBufferArray();								
			}
	}	

	if((ulLength < 8 ))// &&(READ_CMD_TimeoutFlag == 1))
	{
		DU_CMD_InitTimerFlag = 0;
		READ_CMD_TimeoutFlag = 0;
		WR_SIN_CMD_TimeoutFlag = 0;
		WR_MUL_CMD_TimeoutFlag = 0;		
		WRITE_SIN_TimeoutCounter = 0;
		WRITE_MUL_TimeoutCounter = 0;
		
		for(i=0;i<ulLength;i++)
		ucBufferArray[i] = 0;
		
		ulBuffIndex = 0;
		Flush_UARTRxBufferArray();
	}	

	if((((8 < ulLength )&& (ulLength < 12))))// &&(WR_SIN_CMD_TimeoutFlag == 1))
	{
		DU_CMD_InitTimerFlag = 0;
		WR_SIN_CMD_TimeoutFlag = 0;
		READ_CMD_TimeoutFlag = 0;
		WR_MUL_CMD_TimeoutFlag = 0;		
		READ_TimeoutCounter = 0;
		WRITE_MUL_TimeoutCounter = 0;
		
		for(i=0;i<ulLength;i++)
		ucBufferArray[i] = 0;
		
		ulBuffIndex = 0;
		Flush_UARTRxBufferArray();
	}
	
	if( (ulLength > 160))// &&(WR_MUL_CMD_TimeoutFlag == 1))
	{
		DU_CMD_InitTimerFlag = 0;
		WR_MUL_CMD_TimeoutFlag = 0;
		READ_CMD_TimeoutFlag = 0;
		WR_SIN_CMD_TimeoutFlag = 0;		
		READ_TimeoutCounter = 0;
		WRITE_SIN_TimeoutCounter = 0;
		
		for(i=0;i<ulLength;i++)
		ucBufferArray[i] = 0;
		
		ulBuffIndex = 0;
		Flush_UARTRxBufferArray();
	}
	
	if(ucRxdPacket[0] == CONTROL_BOARD_ID ) 
	{
		
		crc_calculated = crc16(ucRxdPacket,ulLength-2);
		crc_received = (ucRxdPacket[ulLength-2] << 8) | ucRxdPacket[ulLength-1];
		
		if(crc_calculated == crc_received)
		{		
			ulSlaveId = ucRxdPacket[0];
			ulFunctionCode = ucRxdPacket[1];			
			if(ulFunctionCode == MODBUS_READ)
			{
				Modbus_ReadReg(&modBusCommnStruct,ucRxdPacket);
				modBusCommnStruct.Error_Status = MODBUS_COMM_SUCCESSFULL;
			}
			if(ulFunctionCode == MODBUS_WRITE_SINGLE)
			{
				Modbus_WriteSingleReg(&modBusCommnStruct,ucRxdPacket);
				modBusCommnStruct.Error_Status = MODBUS_COMM_SUCCESSFULL;
			}
			if(ulFunctionCode == MODBUS_WRITE_MULTIPLE)
			{
				Modbus_MultipleReg(&modBusCommnStruct,ucRxdPacket);
				modBusCommnStruct.Error_Status = MODBUS_COMM_SUCCESSFULL;				
			}
			else
			{
				modBusCommnStruct.Error_Status = MODBUS_ILLEGAL_FUNCTION_CODE;
			}		
		}
		else
		{
			modBusCommnStruct.Error_Status = MODBUS_CRC_MISMATCH_ERROR;			// CRC Error Occured			
		}
		
	}
	else
	{
		modBusCommnStruct.Error_Status = MODBUS_ILLEGAL_DESTINATION_ID;		 //Wrong Destination ID
	}
	
	return 	modBusCommnStruct.Error_Status;
	
}


void Modbus_ReadReg(modBusCommn *modBusStruct,unsigned char *RxdPktPtr)
{
	unsigned int ulSlaveId = 0, ulFunctionCode = 0;
	unsigned int ulRegisterAddres = 0, ulNumRegisters = 0, ulNumRegLimit = 0, DataLength = 0, i = 0, ulTotalReadRegBytes = 0;
	unsigned int ulIndexRead = 0, ulIndexTx = 0;
	unsigned char ucTxPacket[120] = {0};
	unsigned short crc_calculated = 0, crc_received = 0;	

	ulSlaveId = RxdPktPtr[0];
	ulFunctionCode = RxdPktPtr[1]; 
	ulRegisterAddres = RxdPktPtr[2];
	ulRegisterAddres <<= 8;
	ulRegisterAddres |= RxdPktPtr[3];				
	ulNumRegisters = RxdPktPtr[4];
	ulNumRegisters <<= 8;
	ulNumRegisters |= RxdPktPtr[5];
	
	ulNumRegLimit = (MODBUS_READ_REG_END_ADDR - ulRegisterAddres)+1; 
	ulTotalReadRegBytes = ((ulRegisterAddres - MODBUS_READ_REG_START_ADDR) + (ulNumRegisters * 4));
				
	if((ulRegisterAddres >= MODBUS_READ_REG_START_ADDR)&&(ulRegisterAddres <= MODBUS_READ_REG_END_ADDR )) // Address validation of DU-Query packet
	{
		
		for( i=0;i<20;i++)
		ucModbusReadReg[i] = pack_IEEE754(*(readPtr+i)); // converting the acquired Ctrl-PCB float data into IEEE754 std packet and stored into an array

		if((ulNumRegisters <= ulNumRegLimit) && ( ulTotalReadRegBytes <= MAX_NUM_MODBUS_READ_REG_BYTES)) // Validation of requested Number of registers
		{
//			VentSettingsChange = 1; // Flag required to change parameters in main algorithm
			ulIndexRead = ulRegisterAddres - MODBUS_READ_REG_START_ADDR; // index of the READ register
			ucTxPacket[0] = DU_BOARD_ID;
			ucTxPacket[1] = ulFunctionCode;
			ucTxPacket[2] = ulRegisterAddres >> 8;
			ucTxPacket[3] = ulRegisterAddres;
			ucTxPacket[4] = ulNumRegisters >> 8;
			ucTxPacket[5] = ulNumRegisters;	
			DataLength = ulNumRegisters * 4;
//			ucTxPacket[2] = DataLength >> 8;
//			ucTxPacket[3] = DataLength;
			
			
			for(ulIndexTx = 6; ulIndexTx < (DataLength + 4); ulIndexTx += 4, ulIndexRead++) // Placing the individual bytes of data according Modbus protocol std structure(MSB 1st, LSB last)
			{
				ucTxPacket[ulIndexTx] = (ucModbusReadReg[ulIndexRead] >> 24);
				ucTxPacket[ulIndexTx + 1] = (ucModbusReadReg[ulIndexRead] >> 16);
				ucTxPacket[ulIndexTx + 2] = (ucModbusReadReg[ulIndexRead] >> 8);
				ucTxPacket[ulIndexTx + 3] = ucModbusReadReg[ulIndexRead];
			}	
			crc_calculated = crc16(ucTxPacket, ulIndexTx);
			ucTxPacket[ulIndexTx++] = crc_calculated >> 8;
			ucTxPacket[ulIndexTx++] = crc_calculated;
			
//			UARTFlushTx(1);
			UARTwrite((const char *)ucTxPacket, ulIndexTx);
//			modBusStruct->Error_Status = MODBUS_COMM_SUCCESSFULL;
		}
		else
		{
			modBusStruct->Error_Status = MODBUS_ILLEGAL_READ_NUM_REG;			// Number of Register in Query cmd breached the limit
		}
		
	}
	else
	{
		modBusStruct->Error_Status = MODBUS_ILLEGAL_READ_REG_ADDR;// Resgister address in Query cmd breached the limit
		
	}	
}

void Modbus_WriteSingleReg(modBusCommn *modBusStruct,unsigned char *RxdPktPtr)
{
	unsigned int ulSlaveId = 0, ulFunctionCode = 0;
	unsigned int ulRegisterAddres = 0, ulNumRegisters = 0, ulNumRegLimit = 0, DataLength = 0, i = 0, ulTotalWriteRegBytes = 0;
	unsigned int ulIndexWrite = 0, ulIndexTx = 0, ulIndexRx = 0 ;
	unsigned char ucTxPacket[50] = {0}, ucWriteRegStatus = 0;
	unsigned short crc_calculated = 0, crc_received = 0;	
	
	ulSlaveId = RxdPktPtr[0];
	ulFunctionCode = RxdPktPtr[1];
	ulRegisterAddres = RxdPktPtr[2];
	ulRegisterAddres <<= 8;
	ulRegisterAddres |= RxdPktPtr[3];				
	ulNumRegisters = RxdPktPtr[4];
	ulNumRegisters <<= 8;
	ulNumRegisters |= RxdPktPtr[5];
	
	ulNumRegLimit = (MODBUS_WRITE_REG_END_ADDR - ulRegisterAddres)+1; 
	ulTotalWriteRegBytes = ((ulRegisterAddres - MODBUS_WRITE_REG_START_ADDR) + (ulNumRegisters * 4));
	
	if((ulRegisterAddres >= MODBUS_WRITE_REG_START_ADDR)&&(ulRegisterAddres <= MODBUS_WRITE_REG_END_ADDR )) // Address validation of DU-Query packet
	{
		
		ulIndexWrite = (ulRegisterAddres - MODBUS_WRITE_REG_START_ADDR); // WRITE_register_index
		
		if((ulNumRegisters <= ulNumRegLimit) && ( ulTotalWriteRegBytes <= MAX_NUM_MODBUS_WRITE_REG_BYTES)) // Validation of requested Number of registers
		{
			
			VentSettingsChange = 1; // Flag required to change parameters in main algorithm

			for(ulIndexRx = 6; ulIndexRx < (ulNumRegisters * 4) + 6; ulIndexRx += 4, ulIndexWrite++)
			{
				ucModbusWriteReg[ulIndexWrite] = RxdPktPtr[ulIndexRx + 3];
				ucModbusWriteReg[ulIndexWrite] = ucModbusWriteReg[ulIndexWrite] | (RxdPktPtr[ulIndexRx + 2] << 8);
				ucModbusWriteReg[ulIndexWrite] = ucModbusWriteReg[ulIndexWrite] | (RxdPktPtr[ulIndexRx + 1] << 16);
				ucModbusWriteReg[ulIndexWrite] = ucModbusWriteReg[ulIndexWrite] | (RxdPktPtr[ulIndexRx] << 24);
						
				*(writePtr+ulIndexWrite) = unpack_IEEE754(ucModbusWriteReg[ulIndexWrite]);						
			}	
			
//			UARTCharPut(UART5_BASE,ucModbusWriteReg[0]);		
			ucWriteRegStatus = WRITE_REG_OK;
//			modBusStruct->Error_Status = MODBUS_COMM_SUCCESSFULL;
		}
		else
		{
			modBusStruct->Error_Status = MODBUS_ILLEGAL_WRITE_SINGLE_NUM_REG; 	// Number of Register in Query cmd breached the limit
		}
	}
	else
	{
		modBusStruct->Error_Status = MODBUS_ILLEGAL_WRITE_SINGLE_REG_ADDR;// Resgister address in Query cmd breached the limit		// Resgister address in Query Cmd breached the limit		
	}
	
	/* Response packet transfer to DU side*/
	if(ucWriteRegStatus == WRITE_REG_OK) // Certain amount of response timeout needs to be added here, within the timeout period Ctrl-PCBA has to respond back to DU
	{
			ucTxPacket[ulIndexTx++] = DU_BOARD_ID;
			ucTxPacket[ulIndexTx++] = ulFunctionCode;
			ucTxPacket[ulIndexTx++] = ulRegisterAddres >> 8;
			ucTxPacket[ulIndexTx++] = ulRegisterAddres;
			ucTxPacket[ulIndexTx++] = ulNumRegisters >> 8;
			ucTxPacket[ulIndexTx++] = ulNumRegisters;

			crc_calculated = crc16(ucTxPacket, ulIndexTx);
			ucTxPacket[ulIndexTx++] = crc_calculated >> 8;
			ucTxPacket[ulIndexTx++] = crc_calculated;

//			UARTFlushTx(1);
//			UARTwrite((const char *)ucTxPacket, ulIndexTx);
//				UARTCharPut(UART5_BASE,'1');	
	}
	
//		UARTCharPut(UART5_BASE, '0');	
}

void Modbus_MultipleReg(modBusCommn *modBusStruct,unsigned char *RxdPktPtr)
{
	unsigned int ulSlaveId = 0, ulFunctionCode = 0;
	unsigned int ulRegisterAddres = 0, ulNumRegisters = 0, ulNumRegLimit = 0, DataLength = 0, i = 0, ulTotalWriteRegBytes = 0;
	unsigned int ulIndexWrite = 0, ulIndexTx = 0, ulIndexRx = 0 ;
	unsigned char ucTxPacket[50] = {0}, ucWriteRegStatus = 0;
	unsigned short crc_calculated = 0, crc_received = 0;	
	

	ulSlaveId = RxdPktPtr[0];
	ulFunctionCode = RxdPktPtr[1];
	ulRegisterAddres = RxdPktPtr[2];
	ulRegisterAddres <<= 8;
	ulRegisterAddres |= RxdPktPtr[3];				
	ulNumRegisters = RxdPktPtr[4];
	ulNumRegisters <<= 8;
	ulNumRegisters |= RxdPktPtr[5];
	
	ulNumRegLimit = (MODBUS_WRITE_REG_END_ADDR - ulRegisterAddres)+1; 
	ulTotalWriteRegBytes = ((ulRegisterAddres - MODBUS_WRITE_REG_START_ADDR) + (ulNumRegisters * 4));
	
	if((ulRegisterAddres >= MODBUS_WRITE_REG_START_ADDR)&&(ulRegisterAddres <= MODBUS_WRITE_REG_END_ADDR )) // Address validation of DU-Query packet
	{		
		ulIndexWrite = (ulRegisterAddres - MODBUS_WRITE_REG_START_ADDR); // WRITE register index
		
		if((ulNumRegisters <= ulNumRegLimit) && ( ulTotalWriteRegBytes <= MAX_NUM_MODBUS_WRITE_REG_BYTES)) // Validation of requested Number of registers
		{
			for(ulIndexRx = 6; ulIndexRx < (ulNumRegisters * 4) + 6; ulIndexRx += 4, ulIndexWrite++)
			{
				
				VentSettingsChange = 1; // Flag required to change parameters in main algorithm
				ucModbusWriteReg[ulIndexWrite] = RxdPktPtr[ulIndexRx + 3];
				ucModbusWriteReg[ulIndexWrite] = ucModbusWriteReg[ulIndexWrite] | (RxdPktPtr[ulIndexRx + 2] << 8);
				ucModbusWriteReg[ulIndexWrite] = ucModbusWriteReg[ulIndexWrite] | (RxdPktPtr[ulIndexRx + 1] << 16);
				ucModbusWriteReg[ulIndexWrite] = ucModbusWriteReg[ulIndexWrite] | (RxdPktPtr[ulIndexRx] << 24);
						
				*(writePtr+ulIndexWrite) = unpack_IEEE754(ucModbusWriteReg[ulIndexWrite]);						
			}	
			
//			UARTCharPut(UART5_BASE,ucModbusWriteReg[0]);		
			ucWriteRegStatus = WRITE_REG_OK;
//			modBusStruct->Error_Status = MODBUS_COMM_SUCCESSFULL;	
		}
		else
		{
			ucWriteRegStatus = WRITE_REG_NOT_OK; 
			modBusStruct->Error_Status = MODBUS_ILLEGAL_WRITE_MULTIPLE_NUM_REG;			// Number of Register in Query cmd breached the limit
		}
	}
	else
	{
				modBusStruct->Error_Status = MODBUS_ILLEGAL_WRITE_MULTIPLE_REG_ADDR; // Resgister address in Query Cmd breached the limit		
	}

	/* Response packet transfer to DU side*/	
	if(ucWriteRegStatus == WRITE_REG_OK) // Certain amount of response timeout needs to be added here, within the timeout period Ctrl-PCBA has to respond back to DU 
	{
			ucTxPacket[ulIndexTx++] = DU_BOARD_ID;
			ucTxPacket[ulIndexTx++] = ulFunctionCode;
			ucTxPacket[ulIndexTx++] = ulRegisterAddres >> 8;
			ucTxPacket[ulIndexTx++] = ulRegisterAddres;
			ucTxPacket[ulIndexTx++] = ulNumRegisters >> 8;
			ucTxPacket[ulIndexTx++] = ulNumRegisters;

			crc_calculated = crc16(ucTxPacket, ulIndexTx);
			ucTxPacket[ulIndexTx++] = crc_calculated >> 8;
			ucTxPacket[ulIndexTx++] = crc_calculated;

//			UARTFlushTx(1);
//			UARTwrite((const char *)ucTxPacket, ulIndexTx);
	}	
}

void Modbus_Service_MessageTx()
{	
	if(Modbus_Service_MessageCmd(&modBusServMsg,DU_BOARD_ID,MODBUS_SERVICE) == MODBUS_SERVICE_MSG_CMD_SUCCESS)
	{
		ServResp_TimerInitFlag = 1;
		
//		while(DU_Resp_TimeoutFlag == 1);
//		
//		if((Modbus_Service_MessageResp(&modBusServMsg,CONTROL_BOARD_ID,MODBUS_SERVICE) == MODBUS_SERVICE_MSG_RESP_SUCCESS))
//			{
//						ServResp_Counter++;
//						DU_Resp_TimeoutFlag = 0;
//						ServResp_TimerInitFlag = 0;
//			}

	}
}


modBusCommnStatus Modbus_Service_MessageCmd(modBusCommn *modBusServ,unsigned long ulSlaveId, unsigned long ulFunctionCode)
{

	unsigned char ucTxPktBuffer[50] = {0};
	unsigned long ulTxIndex = 0, ulServIndex = 0;
	unsigned int ulRegisterAddres = 0, ulNumRegisters = 0, ulNumRegLimit = 0, DataLength = 0, i = 0, ulTotalServRegBytes = 0;
	unsigned short crc_calculated = 0, crc_received = 0;
	
	/* Loging Data*/
	unsigned char ucLogBuffer[100] = {0};
	unsigned long ulLogIndex = 0 ;
	
	ulRegisterAddres = 0x01;	ulNumRegisters = 0x09;
	modBusServ->Error_Status = 0;
	
	ctrlServParam.Time_Stamp++ ;
	
	for( i=0;i<9;i++)
	{ 
		if(i==6)continue;//except condition for status register
		ucModbusServReg[i] = pack_IEEE754(*(servPtr+i)); // converting the float data values of Service parameters into IEEE754 std packet and stored into an array
	}
	ucModbusServReg[6] = ctrlServParam.Status_Reg; //assigning status register for sending data
	
	
	ucTxPktBuffer[ulTxIndex++] = ulSlaveId;
	ucTxPktBuffer[ulTxIndex++] = ulFunctionCode;
	ucTxPktBuffer[ulTxIndex++] = ulRegisterAddres >> 8;
	ucTxPktBuffer[ulTxIndex++] = ulRegisterAddres;
	ucTxPktBuffer[ulTxIndex++] = ulNumRegisters >> 8;
	ucTxPktBuffer[ulTxIndex++] = ulNumRegisters;	

	DataLength = ulNumRegisters * 4;
	
	for(ulTxIndex = 6 ; ulTxIndex < (DataLength + 4); ulTxIndex += 4, ulServIndex++) // Placing the individual bytes of data according Modbus protocol std structure(MSB 1st, LSB last)
	{
		ucTxPktBuffer[ulTxIndex] = (ucModbusServReg[ulServIndex] >> 24);
		ucTxPktBuffer[ulTxIndex + 1] = (ucModbusServReg[ulServIndex] >> 16);
		ucTxPktBuffer[ulTxIndex + 2] = (ucModbusServReg[ulServIndex] >> 8);
		ucTxPktBuffer[ulTxIndex + 3] = ucModbusServReg[ulServIndex];
	}
	
	crc_calculated = crc16(ucTxPktBuffer, ulTxIndex);
	ucTxPktBuffer[ulTxIndex++] = crc_calculated >> 8;
	ucTxPktBuffer[ulTxIndex++] = crc_calculated;
	UARTFlushTx(1);
	
//	
	
	
	UARTwrite((const char *)ucTxPktBuffer, ulTxIndex);
	
	
//	TestPacket();
	
	modBusServ->Error_Status = MODBUS_SERVICE_MSG_CMD_SUCCESS;
	
	return modBusServ->Error_Status;	
}

modBusCommnStatus Modbus_Service_MessageResp(modBusCommn *modBusServ,unsigned char ulSlaveId, unsigned char ulFunctionCode)
{

	unsigned int ulRegisterAddres = 0, ulNumRegisters = 0, ulNumRegLimit = 0, i =0;
	unsigned int ulIndex = 0, ulIndexRx = 0, ulLength = 0 ;
	unsigned char ucRxdPacket[30] = {0}, ucServRespStatus = 0;
	unsigned short crc_calculated = 0, crc_received = 0;
	modBusServ->Error_Status = 0;

//	while(UARTRxBytesAvail() <= 8);

//	ulLength = UARTRxBytesAvail();

	if (UARTRxBytesAvail())
	{
		DU_ServResp_InitTimerFlag = 1;
		do
		{
			ucBufferArray[ulBuffIndex++] = UARTgetc();	
		} while(UARTRxBytesAvail());		
		
		SPulIndex_page++;	
	}
	
	ulLength =  ulBuffIndex;
	
	if((ulLength == 8) && (ServResp_TimeoutFlag == 1))
	{

		ServResp_TimeoutFlag = 0;
		DU_ServResp_InitTimerFlag = 0;
		
		if((ucBufferArray[0] == ulSlaveId) && (ucBufferArray[1] == ulFunctionCode))
			{
				for(i=0;i<ulLength;i++)
				{
					ucRxdPacket[i] = ucBufferArray[i];				
				}
						
				for(i=0;i<ulLength;i++)
				ucBufferArray[i] = 0;				
				ulBuffIndex = 0;
				Flush_UARTRxBufferArray();
			}
			else
			{
				for(i=0;i<ulLength;i++)
				ucBufferArray[i] = 0;				
				ulBuffIndex = 0;
				Flush_UARTRxBufferArray();
				modBusServ->Error_Status = MODBUS_ILLEGAL_DATA_PKT_RXD;				
			}			
	}
	
	if(((ulLength < 8 ) || (ulLength > 8 )) && (ServResp_TimeoutFlag == 1))
	{
		DU_ServResp_InitTimerFlag = 0;
		ServResp_TimeoutFlag = 0;
		
		for(i=0;i<ulLength;i++)
		ucBufferArray[i] = 0;
		
		ulBuffIndex = 0;
		Flush_UARTRxBufferArray();
		modBusServ->Error_Status = MODBUS_ILLEGAL_DATA_PKT_RXD;
	}	
	
	if((ucRxdPacket[ulIndexRx++] == ulSlaveId) && (ucRxdPacket[ulIndexRx++] == ulFunctionCode))
	{

		crc_calculated = crc16(ucRxdPacket,ulLength-2);
		crc_received = (ucRxdPacket[ulLength-2] << 8) | ucRxdPacket[ulLength-1];
	
		if(crc_calculated == crc_received)
		{
			ServResp_Success = 2;
			ulRegisterAddres = ucRxdPacket[ulIndexRx++];
			ulRegisterAddres <<= 8;
			ulRegisterAddres |= ucRxdPacket[ulIndexRx++];;				
			ulNumRegisters = ucRxdPacket[ulIndexRx++];;
			ulNumRegisters <<= 8;
			ulNumRegisters |= ucRxdPacket[ulIndexRx++];
			
			modBusServ->Error_Status = MODBUS_SERVICE_MSG_RESP_SUCCESS;			
		}
		else
		{
			modBusServ->Error_Status = MODBUS_SERVICE_MSG_RESP_FAIL;
		}
		
	return modBusServ->Error_Status;		
	}			
}

void TestPacket()
{
	unsigned int uindex = 0;
	unsigned char packet[100] = {0};
	float Insp_f = 0, Exp_f  = 0, InhFlow_f = 0, ExhFlow_f = 0;
	unsigned int Insp_h = 0, Exp_h = 0,InhFlow_h = 0,ExhFlow_h = 0;
	
	TimeStampTest++;
	
	InhFlow_f = *(servPtr+0);
	ExhFlow_f = *(servPtr+1); 	
	Insp_f = *(servPtr+3);
	Exp_f = *(servPtr+4);
	
	InhFlow_h = InhFlow_f * 1000;
	ExhFlow_h = ExhFlow_f * 1000;
	Insp_h = Insp_f * 1000;
	Exp_h = Exp_f * 1000;

		packet[uindex++] = '*';
		packet[uindex++] = (InhFlow_h / 10000) + 0x30;
		packet[uindex++] = ((InhFlow_h / 1000)%10) + 0x30;
		packet[uindex++] = 0x2E;
		packet[uindex++] = ((InhFlow_h / 100)%10) + 0x30;
		packet[uindex++] = ((InhFlow_h / 10)%10) + 0x30;
		packet[uindex++] = (InhFlow_h %10) + 0x30;
		packet[uindex++] = '*';
		packet[uindex++] = '*';
		packet[uindex++] = (ExhFlow_h / 10000) + 0x30;
		packet[uindex++] = ((ExhFlow_h / 1000)%10) + 0x30;
		packet[uindex++] = 0x2E;
		packet[uindex++] = ((ExhFlow_h / 100)%10) + 0x30;
		packet[uindex++] = ((ExhFlow_h / 10)%10) + 0x30;
		packet[uindex++] = (ExhFlow_h %10) + 0x30;
		packet[uindex++] = '*';
		packet[uindex++] = '*';
		packet[uindex++] = (Insp_h / 10000) + 0x30;
		packet[uindex++] = ((Insp_h / 1000)%10) + 0x30;
		packet[uindex++] = 0x2E;
		packet[uindex++] = ((Insp_h / 100)%10) + 0x30;
		packet[uindex++] = ((Insp_h / 10)%10) + 0x30;
		packet[uindex++] = (Insp_h %10) + 0x30;
		packet[uindex++] = '*';
		packet[uindex++] = (Exp_h / 10000) + 0x30;
		packet[uindex++] = ((Exp_h / 1000)%10) + 0x30;
		packet[uindex++] = 0x2E;
		packet[uindex++] = ((Exp_h / 100)%10) + 0x30;
		packet[uindex++] = ((Exp_h / 10)%10) + 0x30;
		packet[uindex++] = (Exp_h %10) + 0x30;
		packet[uindex++] = '*';
		packet[uindex++] = '*';
		packet[uindex++] = (TimeStampTest/1000)+0x30;
		packet[uindex++] = ((TimeStampTest/100)%10)+0x30;
		packet[uindex++] = ((TimeStampTest/10)%10)+0x30;
		packet[uindex++] = (TimeStampTest%10)+0x30;
		packet[uindex++] = '*';
		packet[uindex++] = 'x';
		packet[uindex++] = '=';
//		packet[uindex++] = (x/1000)+0x30;
//		packet[uindex++] = ((x/100)%10)+0x30;
//		packet[uindex++] = ((x/10)%10)+0x30;
//		packet[uindex++] = (x%10)+0x30;		
		packet[uindex++] = '*';		
		packet[uindex++] = 'y';
		packet[uindex++] = '=';
//		packet[uindex++] = (y/1000)+0x30;
//		packet[uindex++] = ((y/100)%10)+0x30;
//		packet[uindex++] = ((y/10)%10)+0x30;
//		packet[uindex++] = (y%10)+0x30;	
		packet[uindex++] = '*';
		packet[uindex++] = 'z';
		packet[uindex++] = '=';
//		packet[uindex++] = (z/1000)+0x30;
//		packet[uindex++] = ((z/100)%10)+0x30;
//		packet[uindex++] = ((z/10)%10)+0x30;
//		packet[uindex++] = (z%10)+0x30;	
//		
//		packet[uindex++] = '\n';
//		
//		UARTFlushTx(1);
//		UARTwrite(packet,uindex);
	
}
uint32_t pack_IEEE754(float float_val)
{
	float rxd_float = float_val;
	uint8_t hex_arr[5], *temp;
	uint32_t hex_val;
	
	temp = (uint8_t*)&rxd_float;
	hex_arr[0] = *(temp+3);
	hex_arr[1] = *(temp+2);
	hex_arr[2] = *(temp+1);
	hex_arr[3] = *(temp+0);
	
	hex_val = hex_arr[0] << 24;
	hex_val |= hex_arr[1] << 16;
	hex_val |= hex_arr[2] << 8;
	hex_val |= hex_arr[3];
	
	return hex_val;
	
}

float unpack_IEEE754(int32_t hex_val)
{
	bool sign = 0;
	int8_t exponent = 0, temp;
	int32_t mantissa = 0;
	float dec_float = 0, dec_float1 = 0, dec_float2 = 0, dec_float3 = 0;
	
	sign = hex_val >> 31;
	exponent = (hex_val >> 23) & 0xFF;
	mantissa = hex_val & 0x7FFFFF;
	temp = exponent - 127;
	dec_float1 = pow(-1,sign);
	dec_float2 = pow(2,temp);
	dec_float3 = (float)(1 + ((float)mantissa/8388608));
	dec_float = dec_float1 * dec_float2 * dec_float3;
	
	return dec_float;	
}
