/*! \file **********************************************************************
 *
 *  \brief 
 *
 
 *  \author 
 *
 *  Copyright (c) 2021 CT Control Technology .
 ******************************************************************************/


//---[ Includes ]---------------------------------------------------------------

#include "SSI_Handler.h"



//---[ Macros ]-----------------------------------------------------------------



//---[ Constants ]--------------------------------------------------------------


//---[ Types ]------------------------------------------------------------------


//---[ Private Functions ]------------------------------------------------------


//---[ Public Variables ]-------------------------------------------------------

uint32_t ui32IndexSSI = 0, u32TxInd = 0;
uint8_t  SSI_TxnFlag = 0,u8SSIDataProcess[22] = {0};
uint32_t SSI_Sync = 0,SSIRxdFlag = 0,u32RxdPktInd = 0,u32RxdIEEEPkt[3] = {0},SSI_Sync1 = 0;
//uint16_t crc_calculated = 0, crc_received = 0;
uint32_t TimeCount = 0, ulSyncTimeout = 0;
//float NumCorrPkt = 0,NumNonCorrPkt = 0,NumRxdSynPkt = 0,NumRxdNonSynPkt = 0;


//---[ Private Function Definitions ]-------------------------------------------


void SSI_Init()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI1);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

    GPIOPinConfigure(GPIO_PD0_SSI1CLK);
    GPIOPinConfigure(GPIO_PD1_SSI1FSS);
    GPIOPinConfigure(GPIO_PD2_SSI1RX);
    GPIOPinConfigure(GPIO_PD3_SSI1TX);

    GPIOPinTypeSSI(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 |
                   GPIO_PIN_3);
									 
    SSIConfigSetExpClk(SSI1_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0,
                       SSI_MODE_MASTER,100000, 8);  //1Mbps

//	  SSIFlushRx();
//    SSIFlushTx(true);
		
		MAP_SSIIntDisable(SSI1_BASE, 0xFFFFFFFF);
    MAP_SSIIntEnable(SSI1_BASE, SSI_RXFF);
    MAP_IntEnable(INT_SSI1);


    //
    // Enable the SSI operation.
    //
    MAP_SSIEnable(SSI1_BASE);		
	
    SSIIntEnable(SSI1_BASE,SSI_RXFF);

}


void SSI_Communication()
{
	uint32_t u32RegisterAddres = 0, u32NumRegisters = 0, i = 0,j = 0, u32Length = 0;
	uint16_t crc_calculated = 0, crc_received = 0;
	
	u32RegisterAddres = 0x0001;
	u32NumRegisters = 0x0001;

//	u8SSITx[0] = 0xFF;
//	u8FirstByte = 0xFF;
	ulSyncTimeout = 0;
	SSIDataPutNonBlocking(SSI1_BASE,0xFF);
//		SSIDataPutNonBlocking(SSI1_BASE,(uint32_t)u8FirstByte);


	TimeCount++;
	
		if(SSIRxdFlag == 1)  // Monitor Data Processing
		{
			u32Length = u32RxdPktInd-1;
//			NumRxdSynPkt+=1;

			u8SSIDataProcess[0] = CONTROL_BOARD_ID;	
			
			for(i = 1;i < u32RxdPktInd;i++)
			{
				u8SSIDataProcess[i] = u8SSIDataRx[i];
			}
			
			crc_received = (u8SSIDataProcess[u32Length-2] << 8) | u8SSIDataProcess[u32Length-1] ;
			crc_calculated = crc16(u8SSIDataProcess, u32Length-2);
			
			if(crc_received == crc_calculated)
			{

				SSICorruptedPkt_Count = 0;
				Tech_Alarmcheck.moni_comm=0;

				
				
				for(i = 0,j = 6; j < 18;j+=4,i++)
				{
					u32RxdIEEEPkt[i] = u8SSIDataProcess[j+3];
					u32RxdIEEEPkt[i] = u32RxdIEEEPkt[i] | (u8SSIDataProcess[j+2] << 8);
					u32RxdIEEEPkt[i] = u32RxdIEEEPkt[i] | (u8SSIDataProcess[j+1] << 16);
					u32RxdIEEEPkt[i] = u32RxdIEEEPkt[i] | (u8SSIDataProcess[j] << 24);
					
				}
				
				ctrlServParam.O2_Percent = unpack_IEEE754(u32RxdIEEEPkt[0]);
//				ctrlServParam.Exh_Flow = unpack_IEEE754(u32RxdIEEEPkt[1]);
				
				/*Exhalation FS calibration*/
//				ctrlServParam.Exh_Flow =  (1.1764 * ctrlServParam.Exh_Flow);// + 0.4672;
//				
//				ctrlServParam.Prox_Flow = unpack_IEEE754(u32RxdIEEEPkt[2]);
				
			}
			else
			{
				SSICorruptedPkt_Count++;	
			}
						
			u32RxdPktInd = 0;
			SSIRxdFlag = 0;			
		}
		else
		{
				SSICorruptedPkt_Count++;	
		}
		
		if(SSICorruptedPkt_Count >= 100)  // if we get 10 number of corrupted packet consecutively we will have generate an alarm signal
		{
			SSICorruptedPkt_Flag = 1;
			Tech_Alarmcheck.moni_comm=1;
			SSICorruptedPkt_Count = 0;
		}
		
		if(TimeCount == 100) // checking for num of pkt in 1Sec
		{
			
//			Calib_Pkt();
			
			TimeCount = 0;
//			NumRxdNonSynPkt = 0;
//			NumRxdSynPkt = 0;
//			NumNonCorrPkt = 0;
//			NumCorrPkt = 0;
		}


}


void SSI1_IntHandler(void)
{
	uint32_t ui32Status, u8SingleByte = 0;

	ui32Status = SSIIntStatus(SSI1_BASE,1);

	SSIDataGetNonBlocking(SSI1_BASE, (uint32_t *)&u8SingleByte);
	   
	
		  if(SSI_Sync == 0)
			{
				do
				{ 
					if(u8SingleByte == 0xFF)
					{
						ulSyncTimeout = 0;
						SSI_Sync = 1;
				ui32IndexSSI = 0x00;    //SP Added
				u32TxInd = 0;
						break;
					}
					else
					{
						 SSIDataPutNonBlocking(SSI1_BASE,0xFF);
					}
				}while(ulSyncTimeout++ <= 10);
				if(ulSyncTimeout >= 10)
				{
					ulSyncTimeout = 0;
				}
				
			}
			
			if(SSI_Sync == 1)
			{
				u8SSIDataRx[ui32IndexSSI] = u8SingleByte;
			  ui32IndexSSI++;
			  u8SSITx[u32TxInd] = ui32IndexSSI;
			  SSIDataPutNonBlocking(SSI1_BASE,(uint32_t)u8SSITx[u32TxInd++]);	
				
				if( u32TxInd > 21)
				{
				u32TxInd = 0; 
			  SSI_Sync = 0;
				u32RxdPktInd = ui32IndexSSI-1; 
			  ui32IndexSSI = 0;  //SP Added
				SSIRxdFlag = 1;			
				}		
			}
			
	
	SSIIntClear(SSI1_BASE, ui32Status);
}
