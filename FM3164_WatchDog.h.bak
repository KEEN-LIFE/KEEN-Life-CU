#ifndef _FM3164_WATCHDOG_H_
#define _FM3164_WATCHDOG_H_

/*! \file ********************************************************************
 * \file FM3164_WatchDog.h
 *
 *  \brief 
 *
 *  \author 
 *
 *  Copyright (c) 2021 CT Control Technology .
 ********************************************************************/

//---[ Includes ]---------------------------------------------------------------

#include <stdint.h>
#include <stdbool.h>

#include "i2c.h"
#include "rom.h"

//---[ Macros ]---------------------------------------------------------------


#define WATCHDOG_SLAVE_ADDRESS			0x68


#define WDT_RESET_CTRL_REG_ADDR		  0x09
#define WDT_FUNC_CTRL_REG_ADDR      0x0A
#define WDT_ENABLE							    0x80
#define WDT_DISABLE							    0x1F
#define WDT_RESET_SEQUENCE			    0x0A

#define WDT_TIME_SET_100MS    0x01          //Minimum default timeout value.
#define WDT_TIME_SET_200MS    0x02
#define WDT_TIME_SET_300MS    0x03
#define WDT_TIME_SET_400MS    0x04
#define WDT_TIME_SET_500MS    0x05
#define WDT_TIME_SET_1000MS   0x0A
#define WDT_TIME_SET_1500MS   0x0F
#define WDT_TIME_SET_2000MS   0x14
#define WDT_TIME_SET_2500MS   0x19
#define WDT_TIME_SET_3000MS   0x1E         //Maximum Counter Value.

//---[ Macros ]-----------------------------------------------------------------


//---[ Types ]------------------------------------------------------------------ 

typedef enum
{
    WDT_CTRL_FUN_OK       = 0x00,
    WDT_CTRL_FUN_I2C_BUS_ERR = 0x01
}wdCtrlFnStatus;

typedef enum
{
    WDT_INIT_OK       = 0x00,
    WDT_INIT_NOT_OK = 0x01
}wdInitStatus;

typedef enum
{
    WDT_READ_BACK_OK       = 0x00,
    WDT_READ_BACK_I2C_ERR = 0x01
}wdReadBackStatus;

typedef enum
{
    WDT_KICK_OK       = 0x00,
    WDT_KICK_NOT_OK = 0x01
}wdKickStatus;


typedef struct _wdTimerInfo
{
	uint32_t i2c_base;
	volatile uint32_t err_status; 	
}wdTimerInfo;

//---[ Public Variables ]-------------------------------------------------------


extern wdTimerInfo wdt;
//---[ Private Functions ]---------------------------------------------

//---[ Public Function Prototypes ]---------------------------------------------

wdCtrlFnStatus watchdog_ctrl_func(wdTimerInfo*,uint8_t,uint8_t);
wdReadBackStatus watchdog_readback(wdTimerInfo *,uint8_t);
wdKickStatus watchdog_kick(void);
wdInitStatus watchdog_init(void);

#endif
