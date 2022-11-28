#ifndef _FM3164_MEMORY_H_
#define _FM3164_MEMORY_H_

/*! \file ********************************************************************
 * \file FM3164_FRAM.h
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
#include <string.h>
#include <stdlib.h>
#include "main.h"


#include "i2c.h"
#include "rom.h"

//---[ Macros ]---------------------------------------------------------------

#define MEMORY_SLAVE_ADDRESS				    0x50
#define FRAM_MEM_READ								    0x0A
#define FRAM_MEM_WRITE							    0x0B

#define CONFIG_SET_FLAG		0x55
#define ZERO_SET_FLAG			0x01
#define CALIB_SET_FLAG		0xF1

//---[ Types ]------------------------------------------------------------------ 

typedef enum
{
    MEM_SET_GET_OK       = 0x00,
    MEM_SET_GET_I2C_BUS_ERR = 0x01
}memSetGetStatus; 

typedef enum
{
    FACTORY_GET_PARAM_OK       = 0x00,
    FACTORY_GET_PARAM_NOT_OK   = 0x01
}factoryGetParamStatus;

typedef enum
{
    GET_STORED_SETTINGS_OK       = 0x00,
    GET_STORED_SETTINGS_NOT_OK   = 0x01
}getStoredsettingsStatus;

typedef struct _FRAM_memoryInfo
{
    uint32_t i2c_base;
	  volatile uint32_t err_status;
    uint8_t local_addrss_MSB;
	  uint8_t local_addrss_LSB;
	  uint32_t index;
 	
}FRAM_memoryInfo;

typedef struct
{
	float Air_PV_Valve[15];
	float Air_PV_Valve_limits[10];
	float O2_PV_Valve[15];
	float O2_PV_Valve_limits[10];
	float Air_flowsensor[5];
	float Air_flowsensor_limits[5];
	float O2_flowsensor[5];
	float O2_flowsensor_limits[5];
	float Total_flowsensor[5];
	float Total_flowsensor_limits[5];
	float Exh_flowsensor[5];
	float Exh_flowsensor_limits[5];
	float Insp_Presensor[5];
	float Insp_Presensor_limits[5];
	float Exp_Presensor[5];
	float Exp_Presensor_limits[5];
	float DAC_Peep[5];
	float DAC_Peep_limits[5];
}calibrationvalues;





//---[ Public Variables ]-------------------------------------------------------

extern FRAM_memoryInfo fRAM;		

//calibrationvalues sysParam;
//calibrationvalues userParam;


//extern SystemParameters sysParam;  
//extern SystemParameters userParam; 


//---[ Private Functions ]---------------------------------------------

//---[ Public Function Prototypes ]---------------------------------------------
extern uint32_t pack_IEEE754(float);
extern float unpack_IEEE754(int32_t);

memSetGetStatus Mem_Set_Get_Parm_Func(FRAM_memoryInfo *,uint32_t,uint32_t,uint8_t *,uint32_t);
//factoryGetParamStatus Factory_Get_Param_values(calibrationvalues *,uint32_t,uint8_t *);
//getStoredsettingsStatus Get_Stored_Settings(void);
//void Factory_Def_Values(calibrationvalues*);
//void	FRAM_Test(void);
//void FRAM_DataAcqusition(void);

void factory_variable_Read(void);
void FRAM_DataStoring(void);
void FRAM_DataReading(void);



#endif
