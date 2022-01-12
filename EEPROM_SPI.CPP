/**
@file		EEPROM_SPI.CPP
@brief		Mobule to manage SPI EEPROM
					
@author		
@date		31/07/2011
@version	01.00

*/

#include "EEPROM_SPI.h"


static void write_enable();
static void write_disable();
static void wait(int n);
static bool write_in_progress();
/**
SPI EEPROM inizialize
*/
void init_eeprom_spi()
{
	sio_init_ch0();
	EE_SPI_CS_HIGH();
	wait(10);
	//write_disable();
}

/**
Reads num bytes from the eeprom, starting from the address, address.
@param address, initial reading address
@param data, array of the read data
@param num, number of data to read
@return number of read data if no error occurred, -1 otherwise 
*/
int ee_spi_read(byte address, byte *data, int num)
{
	int i;
	
	if((address + num) > EE_SPI_SIZE)
		return -1;

	EE_SPI_CS_LOW();
	wait(10);

	EE_SPI_PUTC(EE_SPI_READ);
	EE_SPI_PUTC(address);
	
	EE_SPI_WAIT_BUFFER_EMPTY();
	
	for (i = 0; i < num; i++)
	{
		data[i] = EE_SPI_GETC(0);
	}

	wait(5);
	EE_SPI_CS_HIGH();
	wait(5);

	return num;
}

/**
Write num bytes to the eeprom, starting from the address, address.
@param address, initial writing address
@param data, array of the data to wite
@param num, number of data to write
@return number of written data  if no error occurred, -1 otherwise 
*/
int ee_spi_write(byte address, byte *data, int num)
{
	int numFullPageToWrite = 0;
	int numSpareByteToWrite_01 = 0;
	int numSpareByteToWrite_02 = 0;
	int addressModPageSize = 0;
	int i, k, p;

	if((address + num) > EE_SPI_SIZE)
		return -1;

	addressModPageSize = address % EE_SPI_PAGE_SIZE;
	if((addressModPageSize + num) < EE_SPI_PAGE_SIZE) // all the bytes within the same page
	{
		numSpareByteToWrite_01 = num;
		numFullPageToWrite = 0;
		numSpareByteToWrite_02 = 0;
	}else // bytes in different pages
	{
		if(addressModPageSize != 0)
		{
			numSpareByteToWrite_01 = EE_SPI_PAGE_SIZE - addressModPageSize;
		}else
		{
			numSpareByteToWrite_01 = 0;
		}
		
		if((( num - numSpareByteToWrite_01) < EE_SPI_PAGE_SIZE)) // no full page to write
		{
			numSpareByteToWrite_02 = num - numSpareByteToWrite_01;
			numFullPageToWrite = 0;
		}else	// full page to writes
		{
			numSpareByteToWrite_02 = (num - numSpareByteToWrite_01) % EE_SPI_PAGE_SIZE;
			numFullPageToWrite = (num - numSpareByteToWrite_01 - numSpareByteToWrite_02) / EE_SPI_PAGE_SIZE;
		}
	}

	k = 0;
	
	if (numSpareByteToWrite_01 > 0)
	{
		write_enable();
		
		EE_SPI_CS_LOW();
		wait(10);
		
		EE_SPI_PUTC(EE_SPI_WRITE);
		EE_SPI_PUTC(address + k);
		
		for (i = 0; i < numSpareByteToWrite_01; i++)
		{
			EE_SPI_PUTC(data[k]);
			k++;	
		}
			
		EE_SPI_WAIT_BUFFER_EMPTY();
		wait(2);
		EE_SPI_CS_HIGH();
		wait(5);
		
		write_disable();
	}

	
	if (numFullPageToWrite > 0)
	{
		for(p = 0; p < numFullPageToWrite; p++)
		{
			write_enable();
			
			EE_SPI_CS_LOW();
			wait(10);
			
			EE_SPI_PUTC(EE_SPI_WRITE);
			EE_SPI_PUTC(address + k);
			
			for (i = 0; i < EE_SPI_PAGE_SIZE; i++)
			{
				EE_SPI_PUTC(data[k]);
				k++;	
			}
				
			EE_SPI_WAIT_BUFFER_EMPTY();
			wait(2);
			EE_SPI_CS_HIGH();
			wait(5);
			
			write_disable();
		}
	}

	if (numSpareByteToWrite_02 > 0)
	{
		write_enable();
		
		EE_SPI_CS_LOW();
		wait(10);
		
		EE_SPI_PUTC(EE_SPI_WRITE);
		EE_SPI_PUTC(address + k);
		
		for (i = 0; i < numSpareByteToWrite_02; i++)
		{
			EE_SPI_PUTC(data[k]);
			k++;	
		}

		EE_SPI_WAIT_BUFFER_EMPTY();
		wait(2);
		EE_SPI_CS_HIGH();
		wait(5);
		
		write_disable();
	}
	
	return num;
}

/**
Sends the command to enable eeprom write
*/
void write_enable()
{
	while(write_in_progress());
	
	EE_SPI_CS_LOW();
	wait(10);
	
	EE_SPI_PUTC(EE_SPI_WREN);
	
	EE_SPI_WAIT_BUFFER_EMPTY();
	wait(2);
	EE_SPI_CS_HIGH();
	wait(5);
	
}

/**
Sends the command to disable eeprom write
*/
void write_disable()
{
	while(write_in_progress());
	
	EE_SPI_CS_LOW();
	wait(10);
	
	EE_SPI_PUTC(EE_SPI_WRDI);

	EE_SPI_WAIT_BUFFER_EMPTY();
	wait(2);
	EE_SPI_CS_HIGH();
	wait(5);
}

void wait(int n)
{
	int i;
	for(i = 0; i < n; i++)
		asm("nop");
}

bool write_in_progress()
{
	byte status;
	EE_SPI_CS_LOW();
	wait(10);
	
	EE_SPI_PUTC(EE_SPI_RDSR);
	status = EE_SPI_GETC(0);
	EE_SPI_WAIT_BUFFER_EMPTY();

	wait(2);
	EE_SPI_CS_HIGH();
	wait(5);

	return (status & 0x01);
}



























