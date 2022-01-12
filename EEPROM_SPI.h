/**
@file		EEPROM_SPI.h
@brief		Mobule to manage SPI EEPROM
					
@author		
@date		31/07/2011
@version	01.00

*/

#ifndef _EEPROM_SPI_
#define _EEPROM_SPI_

#include "global.h"
#include "SIOSG2.h"

/**
Spi EEPROM commands
*/
enum EE_SPI_CMD
{
	EE_SPI_READ = 0x03,  //0000 x011 /*< Read data from memory array beginning at selected address */
	EE_SPI_WRITE = 0x02, //0000 x010 /*<Write data to memory array beginning at selected address */
	EE_SPI_WRDI = 0x04,  //0000 x100 /*<Reset the write enable latch (disable write operations) */
	EE_SPI_WREN = 0x06,  //0000 x110 /*<Set the write enable latch (enable write operations) */
	EE_SPI_RDSR = 0x05,  //0000 x101 /*<Read STATUS register */
	EE_SPI_WRSR = 0x01,  //0000 x001 /*<Write STATUS register */
};

#define EE_SPI_CS_LOW()   (PinCSeeprom = 0)
#define EE_SPI_CS_HIGH()  (PinCSeeprom = 1)

#define EE_SPI_WAIT_BUFFER_EMPTY() (sio_clocksel_ch0(0))
#define EE_SPI_PUTC(c) sio_putc_ch0(c)
#define EE_SPI_GETC(c) sio_getc_ch0(c)

/**
EEPROM's size in byte
*/
#define EE_SPI_SIZE	256

/**
EEPROM pages size in byte
*/
#define EE_SPI_PAGE_SIZE 16	


#define EE_SPI_ADD_VALID_DISPLAY_SETTING_LOADED 		0x00
#define EE_SPI_ADD_DISPLAY_SETTING_BACKLIGHT		 	0x01
#define EE_SPI_ADD_DISPLAY_SETTING_BRIGTHNESS		 	0x02
#define EE_SPI_ADD_DISPLAY_SETTING_CONTRAST				0x03

#define EE_SPI_ADD_BT_ADDRESS_LOADED					0x04
#define EE_SPI_ADD_BT_ADDRESS							0x05

#define EE_SPI_ADD_VALID_WEIGHT_CAL_STORED				30
#define EE_SPI_ADD_VALID_PRS_CAL_STORED					31

#define EE_SPI_ADD_WEIGHT_GAIN_OFFSET					32
#define EE_SPI_WEIGHT_GAIN_OFFSET_LENGTH				18
#define EE_SPI_ADD_PRS_GAIN_OFFSET						(32 + EE_SPI_WEIGHT_GAIN_OFFSET_LENGTH)
#define EE_SPI_PRS_GAIN_OFFSET_LENGTH					72

#define EE_SPI_ADD_VALID_WTP_CAL_STORED					(EE_SPI_ADD_PRS_GAIN_OFFSET + EE_SPI_PRS_GAIN_OFFSET_LENGTH)		
#define EE_SPI_ADD_WTP_GAIN_OFFSET						(EE_SPI_ADD_VALID_WTP_CAL_STORED + 1)
#define EE_SPI_WTP_GAIN_OFFSET_LENGTH					28

#define EE_SPI_ADD_GEN1_GAIN_OFFSET						(EE_SPI_ADD_WTP_GAIN_OFFSET+ EE_SPI_WTP_GAIN_OFFSET_LENGTH)
#define EE_SPI_GEN_GAIN_OFFSET_LENGTH					9
#define EE_SPI_ADD_GEN2_GAIN_OFFSET						(EE_SPI_ADD_GEN1_GAIN_OFFSET+ EE_SPI_GEN_GAIN_OFFSET_LENGTH)



#define EE_SPI_ADD_BT_VALID_ADDRESS_LOADED  0xAA
#define EE_SPI_VALID_DISPLAY_SETTING_LOADED 0xAA
#define EE_SPI_VALID_CAL_STORED				0xAA

void init_eeprom_spi();

int ee_spi_read(byte address, byte *data, int num);
int ee_spi_write(byte address, byte *data, int num);

















#endif































