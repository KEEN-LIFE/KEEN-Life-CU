/*! \file **********************************************************************
 *
 *  \brief  Implementation of Module for WatchDog Timer
 *
 
 *  \author 
 *
 *  Copyright (c) 2021 CT Control Technology .
 ******************************************************************************/

//---[ Includes ]---------------------------------------------------------------

#include "FM3164_WatchDog.h"

//---[ Macros ]-----------------------------------------------------------------

//---[ Constants ]--------------------------------------------------------------

//---[ Public Variables ]-------------------------------------------------------
volatile uint8_t *read_back_val;

//---[ Private Variables ]------------------------------------------------------

//---[ Private Functions ]------------------------------------------------------

//---[ Function Implementations ]-----------------------------------------------

/*! \brief  This function sets the control register parameters 
 *  \param  wdTimerInfo Pointer to struct with info about the watchdog timer in FM3164 hardware to use
 *  \param  wd_ctrl_reg_addr represents the watchdog control register address(eg: WDT_FUNC_CTRL_REG_ADDR)
 *  \param  wd_data watchdog timeout set value(eg: WDT_TIME_SET_1000MS)
 *  \returns  returns status of watchdog control function process(eg; WDT_CTRL_FUN_OK)
 */

wdCtrlFnStatus watchdog_ctrl_func(wdTimerInfo *wdTimerInfo,uint8_t wd_ctrl_reg_addr,uint8_t wd_data)
{
    ROM_I2CMasterSlaveAddrSet(wdTimerInfo->i2c_base, WATCHDOG_SLAVE_ADDRESS, false);		// setting the slave address and I2C is in Master Transmit Mode
  
	  ROM_I2CMasterDataPut(wdTimerInfo->i2c_base, wd_ctrl_reg_addr);                      // setting the control register address
	  ROM_I2CMasterControl(wdTimerInfo->i2c_base, I2C_MASTER_CMD_BURST_SEND_START);       // start the state of the Master module send operation.
	  while (ROM_I2CMasterBusy(wdTimerInfo->i2c_base));                                   // wait until I2C Master is not busy
	  wdTimerInfo->err_status = ROM_I2CMasterErr(wdTimerInfo->i2c_base);                  // update the error status of Master module
	  if(wdTimerInfo->err_status != I2C_MASTER_ERR_NONE)                                  // check the status of error
	  { 
		    return WDT_CTRL_FUN_I2C_BUS_ERR;                                                // returns the error status to function call
	  }
	
	  ROM_I2CMasterDataPut(wdTimerInfo->i2c_base, wd_data);                               // setting the watchdog timeout value
	  ROM_I2CMasterControl(wdTimerInfo->i2c_base, I2C_MASTER_CMD_BURST_SEND_START);
	  while (ROM_I2CMasterBusy(wdTimerInfo->i2c_base));
	  wdTimerInfo->err_status = ROM_I2CMasterErr(wdTimerInfo->i2c_base);
	  if(wdTimerInfo->err_status != I2C_MASTER_ERR_NONE)                                 
	  { 
	      return WDT_CTRL_FUN_I2C_BUS_ERR;                                                               
	  }
	
    return WDT_CTRL_FUN_OK;                                                            // returns the status of watchdog control function to the function call
}

/*! \brief  This function is a test function to read back values for watchdog
 *  \param  wdTimerInfo Pointer to struct with info about the watchdog timer in FM3164 hardware to use
 *  \param  wd_ctrl_reg_addr represents the watchdog control register address(eg: WDT_FUNC_CTRL_REG_ADDR)
 *  \returns  returns status of watchdog read back function process(eg; WDT_READ_BACK_OK)
 */

wdReadBackStatus watchdog_readback(wdTimerInfo *wdTimerInfo,uint8_t wd_ctrl_reg_addr)
{
	
    ROM_I2CMasterSlaveAddrSet(wdTimerInfo->i2c_base, WATCHDOG_SLAVE_ADDRESS, false);							// setting the slave address and I2C is in Master Transmit Mode
		
	  ROM_I2CMasterDataPut(wdTimerInfo->i2c_base, wd_ctrl_reg_addr);																// setting the control register address
	  ROM_I2CMasterControl(wdTimerInfo->i2c_base, I2C_MASTER_CMD_SINGLE_SEND);                      // setting the control state of Master module with single command send.
	  while (ROM_I2CMasterBusy(wdTimerInfo->i2c_base));                                             // wait until I2C Master is not busy
	  wdTimerInfo->err_status = ROM_I2CMasterErr(wdTimerInfo->i2c_base);                            // update the error status of Master module
	  if(wdTimerInfo->err_status != I2C_MASTER_ERR_NONE)                                            // check the status of error
	  { 
		    return WDT_READ_BACK_I2C_ERR;                                                             // returns the error status to function call
	  }
		
	  ROM_I2CMasterSlaveAddrSet(wdTimerInfo->i2c_base, WATCHDOG_SLAVE_ADDRESS, true);						    // setting the slave address and I2C is in Master Receive Mode
	
	  ROM_I2CMasterControl(wdTimerInfo->i2c_base, I2C_MASTER_CMD_SINGLE_RECEIVE);                   // setting the control state of Master module with single command receive.
	  while (ROM_I2CMasterBusy(wdTimerInfo->i2c_base));
	  wdTimerInfo->err_status = ROM_I2CMasterErr(wdTimerInfo->i2c_base);
	  if(wdTimerInfo->err_status != I2C_MASTER_ERR_NONE)                                 
	  { 
		    return WDT_READ_BACK_I2C_ERR;                                                               
	  }
		
	 *read_back_val= ROM_I2CMasterDataGet(wdTimerInfo->i2c_base);                                  // update the read back val to a pointer
	
	  return WDT_READ_BACK_OK;                                                                     // returns the status of watchdog read back function process to the function call
	
}

/*! \brief  This function is for Initialize Watchdog Timer
 *  \returns  returns status of watchdog init function(eg; WDT_INIT_OK)
 */
wdInitStatus watchdog_init(void)
{
    if(watchdog_ctrl_func(&wdt,WDT_FUNC_CTRL_REG_ADDR,WDT_DISABLE) != WDT_INIT_NOT_OK)
    {
		}        // Disable the watchdog timer
	  if(watchdog_ctrl_func(&wdt,WDT_FUNC_CTRL_REG_ADDR,WDT_ENABLE | WDT_TIME_SET_3000MS) != WDT_INIT_NOT_OK)
    {
		} // Enable the watchdog timer with a timeout set of 3s
	  if(watchdog_ctrl_func(&wdt,WDT_RESET_CTRL_REG_ADDR,WDT_RESET_SEQUENCE) != WDT_INIT_NOT_OK)
    {
		} // Restart the watchdog timer
	
    return WDT_INIT_OK;	                                                                         // returns the status of watchdog initialization function to the function call
}

/*! \brief  This function is for reload/kick the Watchdog Timer
 *  \returns  returns status of watchdog init function(eg; WDT_INIT_OK)
 */
wdKickStatus watchdog_kick(void)
{
    if(watchdog_ctrl_func(&wdt,WDT_RESET_CTRL_REG_ADDR,WDT_RESET_SEQUENCE) == WDT_CTRL_FUN_OK){}
	
    return WDT_KICK_OK;                                                                         // returns the status of watchdog kick function to the function call
}
