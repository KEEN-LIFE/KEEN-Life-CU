#ifndef _LTC2607_DAC_HANDLER_H_
#define _LTC2607_DAC_HANDLER_H_

////////////////////////////////////////////////////////////////////////////////
//! \file LTC2607_DAC_Handler.h
///
///  \brief API for LTC2607 DAC functions
///
///  \author 
///
///  Copyright (c) 2021 CT Control Technology .
////////////////////////////////////////////////////////////////////////////////


//---[ Includes ]---------------------------------------------------------------

#include <stdbool.h>
#include <stdint.h>

#include "i2c.h"
#include "rom.h"

//---[ Macros ]-----------------------------------------------------------------
#define SLAVE_ADDRS_LTC2607_DAC        0x41             //I2C SLAVE ADDRESS FOR DAC DEVICE WHEN J10,J12 AND J25 ARE FLAOTING ON-BOARD.

#define LTC2607_REGADDR_DAC_A          0x00             //DAC CHANNEL A REGISTER ADDRESS. (RECORDER OUTPUT)
#define LTC2607_REGADDR_DAC_B          0x01             //DAC CHANNEL B REGISTER ADDRESS. (PRIMARY CURRENT)
#define LTC2607_REGADDR_DAC_BOTH       0x0F             //DAC REGISTER ADDRESS FOR UPDATING THE VALUE ON BOTH CHANNELS.

#define DAC_CHANNEL_1                  0x01             //CHANNEL 1 ID(DAC A)
#define DAC_CHANNEL_2                  0x02             //CHANNEL 2 ID(DAC B)
#define DAC_CHANNEL_BOTH	             0x03             //CHANNEL ID FOR BOTH


#define LTC2607_WR_INPUT_REG           0x00             //DAC DEVICE INPUT REGISTER WRITE COMMAND.
#define LTC2607_UPDATE_DAC_REG         0x10             //DAC DEVICE POWER UP AS WELL AS UPDATION OF INPUT RESITER VALUE TO DAC REGISTER.
#define LTC2607_DAC_POWER_UP           0x10             //DAC DEVICE POWER UP AS WELL AS UPDATION OF INPUT RESITER VALUE TO DAC REGISTER.
#define LTC2607_WR_AND_UPDATE          0x30             //DAC DEVICE OUTPUT WRITE AND UPDATE.
#define LTC2607_DAC_POWER_DOWN         0x40             //DAC DEVICE POWER DOWN COMMAND.



//---[ Constants ]--------------------------------------------------------------


//---[ Types ]------------------------------------------------------------------

typedef enum
{
  DAC_OUTPUT_SET_OK       = 0x00,
	DAC_OUTPUT_I2C_BUS_ERR = 0x01,
	DAC_IN_POWER_DOWN_MODE = 0x03
}dacStatus;



typedef struct DACHandler
{
	uint32_t dacAddrs;
	uint32_t outputVal;
	uint16_t dacFun;
	uint32_t i2c_base;
	volatile uint32_t dac_err_status;
	volatile uint32_t dataPacket[3];
}DACInfo;

//---[ Public Variables ]-------------------------------------------------------

extern DACInfo DAC1,DAC2;


//---[ Public Function Prototypes ]---------------------------------------------

dacStatus DAC_Output_Set(DACInfo *,uint32_t,uint32_t,uint16_t);
void DAC_Set_Alarm(DACInfo *DACInfo, uint32_t DAC_Num,uint32_t Output_Value);
dacStatus dacErrorHandler(uint32_t);

//---[ Inline Functions ]-------------------------------------------------------




#endif
