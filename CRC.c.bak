/*! \file **********************************************************************
 *
 *  \brief  Implementation of CRC Check
 *
 
 *  \author 
 *
 *  Copyright (c) 2021 CT Control Technology .
 ******************************************************************************/


//---[ Includes ]---------------------------------------------------------------

#include "CRC.h"



//---[ Macros ]-----------------------------------------------------------------



//---[ Constants ]--------------------------------------------------------------


//---[ Types ]------------------------------------------------------------------


//---[ Public Variables ]-------------------------------------------------------



//---[ Public Function Prototypes ]---------------------------------------------


//---[ Inline Functions ]-------------------------------------------------------

/*! \brief  This function calculates CRC checksum for SF05 Sensors
 *  \param  data[] pointer to flow sensor i2c_rxBuffer
 *  \param  bytelength represents the byte length to considered 
 *  \param  checksum actual CRC valued received
 *  \returns  returns calculated 8 bit CRC value
 */

uint8_t crc8CheckUpdateSF05(uint8_t data[], uint8_t bytelength,uint8_t checksum)
{
	uint8_t bit;
	uint8_t crc = 0;
	uint8_t byteCtr;
	
	/* Implementing 8 bit CRC algorithm  for SF05 */
	for(byteCtr = 0; byteCtr < bytelength; byteCtr++)
  {
    crc ^= (data[byteCtr]);
    for(bit = 8; bit > 0; --bit)
    {
      if(crc & 0x80) crc = (crc << 1) ^ 0x31;            // generator polynomial G(x) is 0x31
      else           crc = (crc << 1);
    }
  }

  // verify checksum
  if(crc != checksum) 
	{
		return 1;                                            // returns 1 if crc mismatches
	}		
  else 
	{
		return 0; 
	} 	
	                                                // returns 0 if crc matches 
}
