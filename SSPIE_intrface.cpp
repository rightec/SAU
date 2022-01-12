/************************************************************************
*
* intrface.c
* Version 4.0 - PC file system configuration
*
* This file has 3 parts:
* - algorithm utility functions
* - data utility functions
* - decompression utility functions
*
* The first 2 parts may require user modification to fit how algorithm
* and data files are retrieved by the embedded system.
*
************************************************************************/
#include "global.h"
#include "SSPIE_opcode.h"
#include "SSPIE_intrface.h"

/************************************************************************
*
* algorithm utility functions
*
************************************************************************/

	/************************************************************************
	*
	* algorithm utility functions
	*
	*		For those who do not wish to compile the algorithm with
	* the processing system, you are responsible to modify the
	* following functions or the processing system is not going to
	* run.
	*
	************************************************************************/

	/************************************************************************
	*
	* Design-dependent Global variable
	*
	* You may insert variable here to fit your design
	*
	************************************************************************/
	
	unsigned char *algoPtr    = 0;
	unsigned int algoSize     = 0;
	unsigned int algoIndex    = 0;

	/************************************************************************
	*
	* Design-dependent Functions
	*
	* Here is the list of functions you need to modify:
	*
	* algoPreset()  - This function allows you to set where the algorithm
	*					is prior to running SSPI Embedded.  
	*					If the embedded system has a file system, you
	*					may set the name of the file to SSPI Embedded,
	*					then implement opening the file in function
	*					dataInit().
	*
	* algoInit()	  - In this function, you are responsible to initialize
	*					the algorithm stream.  If the embedded system has a 
	*					file system, you may open the file in this function.
	*
	* algoGetByte() - This function is responsible to get a byte from
	*					algorithm.
	*
	* algoFinal()	  - This function allows you to finalize the algorithm.
	*					If the embedded system has a file system, you may 
	*					implement closing the file here.
	*
	************************************************************************/

	int algoPreset(unsigned char *setAlgoPtr, unsigned int setAlgoSize)
	{

		/************************************************************************
		* Start of design-dependent implementation
		*
		* You may put your code here.
		************************************************************************/

		if(setAlgoPtr)
			algoPtr = setAlgoPtr;
		else{
			algoPtr = NULL;
			return 0;
		}

		if(setAlgoSize != 0)
			algoSize = setAlgoSize;
		else{
			algoSize = 0;
			return 0;
		}

		/************************************************************************
		* End of design-dependent implementation
		************************************************************************/

		return 1;
	}

	int algoInit()
	{

		/************************************************************************
		* Start of design-dependent implementation
		*
		* You may put your code here.
		************************************************************************/
		
		if(!algoPtr)
			return 0;
		if(algoSize == 0)
			return 0;

		algoIndex = 0;

		/************************************************************************
		* End of design-dependent implementation
		*
		* After this initialization, the processing engine need to be able
		* to read algorithm byte by using algoGetByte()
		************************************************************************/
		
		return 1;
	}

	int algoGetByte(unsigned char *byteOut)	{

		/************************************************************************
		* Start of design-dependent implementation
		*
		* You may put your code here.
		************************************************************************/

		if(algoIndex >= algoSize)
			return 0;
		
		*byteOut = algoPtr[algoIndex];
		algoIndex ++;
		return 1;

		/************************************************************************
		* End of design-dependent implementation
		************************************************************************/
	}

	int algoFinal()
	{
		/********************************************************************
		* Start of design-dependent implementation
		*
		* You may put your code here.
		*********************************************************************/

		algoPtr = 0;
		algoSize = 0;
		algoIndex = 0;

		/********************************************************************
		* End of design-dependent implementation
		*********************************************************************/
		
		return 1;
	}

/************************************************************************
*
* data Utility functions
* Version 3.0
*
* Version 3.0 Update:
* Requires data reset or data reaches the end in order to reset data
* File system sample
*
* This code contains data utility functions.  Data may be stored in
* The following method:
* 1. Data is compiled to be part of the SSPI embedded system.  
* 2. Data is stored in internal / external storage
* 3. Data is transmitted over ethernet / wireless
*
************************************************************************/

	/****************************************************************
	*
	* Design-dependent Global variable
	*
	* You may insert variable here to fit your design
	*
	*****************************************************************/
	#define DATA_BUFFER_SIZE	5
	unsigned char *dataPtr    = 0;
	unsigned int dataSize     = 0;
	unsigned int dataIndex    = 0;
	DATA_BUFFER g_dataBufferArr[DATA_BUFFER_SIZE];

	/****************************************************************	
	*
	* Table of content definition
	*
	* This creates the number of table of content used to store
	* data set information.  Reducing the number may save some memory.
	* Depending on the device, the minimum number are different.  
	* 8 is the recommended minimum.
	*
	*
	****************************************************************/
	#define		D_TOC_NUMBER			16
	/****************************************************************
	*
	* Global variable / definition
	*
	****************************************************************/
	DATA_TOC d_toc[D_TOC_NUMBER];
	unsigned short int d_tocNumber        = 0;
	unsigned char		d_isDataInput     = 0;
	unsigned int		d_offset          = 0;
	unsigned int		d_currentAddress  = 0;
	unsigned char		d_requestNewData  = 0;
	unsigned int		d_currentSize     = 0;
	unsigned short int	d_currentDataSetIndex = 0;
	CSU					d_CSU;
	short int			d_SSPIDatautilVersion = 0;

	#define		SSPI_DATAUTIL_VERSION1		1
	#define		SSPI_DATAUTIL_VERSION2		2
	#define		SSPI_DATAUTIL_VERSION3		3

	/********************************************************************
	*
	* Design-dependent Functions
	*
	* Here is the list of functions you need to modify:
	*
	* dataPreset()  - This function allows you to set where the data
	*					is prior to running SSPI Embedded.  
	*					If the embedded system has a file system, you
	*					may set the name of the file to SSPI Embedded,
	*					then implement opening the file in function
	*					dataInit().
	*
	* dataInit()	  - In this function, you are responsible to initialize
	*					the data stream.  If the embedded system has a 
	*					file system, you may open the file in this function.
	*
	* dataReset()	  - In this function, you are responsible to reset the
	*					data to the same state as it is just been initialized.
	*
	* dataGetByte() - This function is responsible to get a byte from
	*					data.
	*
	* dataFinal()	  - This function allows you to finalize the data.  If
	*					the embedded system has a file system, you may 
	*					implement closing the file here.
	*
	**********************************************************************/

	int dataPreset(unsigned char *setDataPtr, unsigned int setDataSize){

		/********************************************************************
		* Start of design-dependent implementation
		* If no data as input, set d_isDataInput = 0, 
		* else set d_isDatatInput = 1
		*
		* You may put your code here.
		*********************************************************************/
		
		if(setDataPtr)
			dataPtr = setDataPtr;
		else{
			dataPtr = 0;
			d_isDataInput = 0;
		}

		if(setDataSize != 0)
			dataSize = setDataSize;
		else{
			dataSize = 0;
			d_isDataInput = 0;
		}

		/********************************************************************
		* End of design-dependent implementation
		*********************************************************************/
		
		d_isDataInput = 1;
		return 1;
	}

	int dataInit(){
		unsigned char currentByte = 0;
		int temp                  = 0;
		int i                     = 0;
		d_offset                  = 0;
		d_currentDataSetIndex     = 0;

		if(d_isDataInput == 0)
			return PROC_COMPLETE;

		/********************************************************************
		* Start of design-dependent implementation
		*
		* You may put your code here.
		*********************************************************************/
		
		if( !dataPtr || dataSize == 0)
			return PROC_FAIL;
		
		dataIndex = 0;

		/********************************************************************
		* End of design-dependent implementation
		*
		* After this initialization, the processing engine need to be able
		* to read data by using dataGetByte()
		*********************************************************************/

		for(i = 0; i < DATA_BUFFER_SIZE; i ++){
			g_dataBufferArr[i].ID = 0x00;
			g_dataBufferArr[i].address = 0;
		}

		if( !dataGetByte( &currentByte, 0, NULL ) )
			return PROC_FAIL;
		d_offset ++;
		if(currentByte == HCOMMENT){
			temp = dataReadthroughComment();
			if( !temp )
				return PROC_FAIL;
			d_offset += temp;
			if( !dataGetByte( &currentByte, 0, NULL ) )
				return PROC_FAIL;
			d_offset ++;
		}
		if(currentByte == HDATASET_NUM){
			d_SSPIDatautilVersion = SSPI_DATAUTIL_VERSION3;
			temp = dataLoadTOC(1);
			if( !temp )
				return PROC_FAIL;
			d_offset += temp;
			d_currentAddress = 0x00000000;
			d_requestNewData = 1;
			return PROC_COMPLETE;
		}
		else if(currentByte == 0x00 || currentByte == 0x01){
			d_SSPIDatautilVersion = SSPI_DATAUTIL_VERSION1;
			set_compression(currentByte);
			return PROC_COMPLETE;
		}
		else
			return PROC_FAIL;
	}

		
	int dataReset(unsigned char isResetBuffer)	
	{
		unsigned char currentByte    = 0;
		int i                        = 0;

		/********************************************************************
		* Start of design-dependent implementation
		*
		* You may put your code here.
		*********************************************************************/
		
		if( !dataPtr || dataSize == 0)
			return PROC_FAIL;

		dataIndex = 0;

		/********************************************************************
		* End of design-dependent implementation
		*
		* After this, the data stream should be in the same condition as
		* it was initialized.  What dataGetByte() function would get must be
		* the same as what it got when being called in dataInit().
		*********************************************************************/

		if(isResetBuffer){
			for(i = 0; i < DATA_BUFFER_SIZE; i ++){
				g_dataBufferArr[i].ID = 0x00;
				g_dataBufferArr[i].address = 0;
			}
		}

		if( !dataGetByte( &currentByte, 0, NULL ) )
			return PROC_FAIL;
		if(currentByte == HCOMMENT){
			if(!dataReadthroughComment())
				return PROC_FAIL;
			if( !dataGetByte( &currentByte, 0, NULL ) )
				return PROC_FAIL;
		}

		if(d_SSPIDatautilVersion == SSPI_DATAUTIL_VERSION3){
			dataLoadTOC(0);
			d_currentAddress = 0x00000000;
			d_currentDataSetIndex = 0;
		}
		return PROC_COMPLETE;
	}

	int dataGetByte(unsigned char *byteOut, 
		short int incCurrentAddr, CSU *checksumUnit)
	{

		/********************************************************************
		* Start of design-dependent implementation
		*
		* You may put your code here.
		*********************************************************************/
		
		if(dataIndex >= dataSize)
			return PROC_FAIL;

		/* read a byte and store in *byteOut */		
		*byteOut = dataPtr[dataIndex];
		dataIndex ++;

		/********************************************************************
		* End of design-dependent implementation
		*********************************************************************/
		
		if(checksumUnit)
			putChunk(checksumUnit, (unsigned int) (*byteOut) );
		if(incCurrentAddr)
			d_currentAddress ++;
		return PROC_COMPLETE;
	}

	int dataFinal(){

		/********************************************************************
		* Start of design-dependent implementation
		*
		* You may put your code here.
		*********************************************************************/

		dataPtr = NULL;
		dataSize = 0;
		dataIndex = 0;

		/********************************************************************
		* End of design-dependent implementation
		*********************************************************************/

		return 1;
	}

	/********************************************************************
	*
	* The following functions does not need user modification
	*
	*********************************************************************/

	unsigned char getRequestNewData(){ return d_requestNewData;}

	int HLDataGetByte(unsigned char dataSet, unsigned char *dataByte, unsigned int uncomp_bitsize){

		int retVal;
		unsigned char tempChar;
		unsigned int bufferSize = 0;
		unsigned int i;

		if(d_SSPIDatautilVersion == SSPI_DATAUTIL_VERSION1){
			if(get_compression()){
				if (uncomp_bitsize != 0 && !decomp_initFrame(uncomp_bitsize) )
					return PROC_FAIL;
				retVal = decomp_getByte(dataByte);
			}
			else
				retVal = dataGetByte(dataByte, 1, &d_CSU);
			return retVal;
		}
		else 
		{
			if(d_requestNewData || dataSet != d_toc[d_currentDataSetIndex].ID){
				if( !dataRequestSet( dataSet ) )
					return PROC_FAIL;
				d_currentSize = 0;
				/* check if buffer has any address */
				for (i = 0; i < DATA_BUFFER_SIZE; i ++){
					if(g_dataBufferArr[i].ID == dataSet){
						bufferSize = g_dataBufferArr[i].address;
						break;
					}
				}
				for(i = 0; i < bufferSize; i ++)
					HLDataGetByte(dataSet, &tempChar, uncomp_bitsize);
			}

			if(d_toc[d_currentDataSetIndex].uncomp_size == 0){
				*dataByte = 0xFF;
				return PROC_FAIL;
			}
			else if(d_currentSize < d_toc[d_currentDataSetIndex].uncomp_size){
				if(get_compression()){
					if(uncomp_bitsize != 0 && !decomp_initFrame(uncomp_bitsize) )
						return PROC_FAIL;
					retVal = decomp_getByte(dataByte);
				}
				else
					retVal = dataGetByte(dataByte, 1, &d_CSU);
				d_currentSize ++;
				/* store data buffer */
				for(i = 0; i < DATA_BUFFER_SIZE; i ++){
					if(g_dataBufferArr[i].ID == dataSet){
						if(d_currentSize != d_toc[d_currentDataSetIndex].uncomp_size)
							g_dataBufferArr[i].address = d_currentSize;
						else{
							g_dataBufferArr[i].ID = 0x00;
							g_dataBufferArr[i].address = 0;
						}
						break;
					}
				}
				if(i == DATA_BUFFER_SIZE){
					for(i = 0; i < DATA_BUFFER_SIZE; i ++){
						if(g_dataBufferArr[i].ID == 0x00){
							g_dataBufferArr[i].ID = dataSet;
							g_dataBufferArr[i].address = d_currentSize;
							break;
						}
					}
				}
				/* check 16 bit check sum */
				if(d_currentSize == d_toc[d_currentDataSetIndex].uncomp_size){
					d_currentDataSetIndex = 0;
					d_requestNewData = 1;
					if( !dataGetByte(&tempChar, 1, &d_CSU) || 
						(tempChar != ((d_CSU.csValue & 0xFF00) >> 8) && tempChar != 0xFF ) )
						return PROC_FAIL;	/* read upper check sum */
					if( !dataGetByte(&tempChar, 1, &d_CSU) || 
						(tempChar != (d_CSU.csValue & 0xFF) && tempChar != 0xFF ) )
						return PROC_FAIL;	/* read lower check sum */
					if( !dataGetByte(&tempChar, 1, &d_CSU) || tempChar != 0xB9 )
						return PROC_FAIL;	/* read 0xB9 */
					if( !dataGetByte(&tempChar, 1, &d_CSU) || tempChar != 0xB2 )
						return PROC_FAIL;	/* read 0xB2 */
				}
				return retVal;
			}
			else{
				d_requestNewData = 1;
				return HLDataGetByte(dataSet, dataByte, uncomp_bitsize);
			}
		}
	}

	int dataReadthroughComment(){
		unsigned char currentByte;
		int retVal = 0;

		init_CS(&d_CSU, 16, 8);

		do{
			if( !dataGetByte( &currentByte, 0, NULL ) )
				break;
			retVal ++;
		}
		while(currentByte != HENDCOMMENT);

		if(currentByte != HENDCOMMENT)
			return PROC_FAIL;

		return retVal;
	}

	int dataLoadTOC(short int storeTOC){
		unsigned char currentByte;
		int i, j;
		int retVal = 0;

		if(storeTOC){
			for (i = 0; i < D_TOC_NUMBER; i++){
				d_toc[i].ID = 0;
				d_toc[i].uncomp_size = 0;
				d_toc[i].compression = 0;
				d_toc[i].address = 0x00000000;
			}
		}

		if( !dataGetByte( &currentByte, 0, NULL ) )
			return PROC_FAIL;
		retVal ++;
		if(storeTOC)
			d_tocNumber = currentByte;

		for (i = 0; i < d_tocNumber; i++){
			/* read HTOC */
			if( !dataGetByte( &currentByte, 0, NULL ) || currentByte != HTOC )
				return PROC_FAIL;
			retVal ++;
			/* read ID */
			if( !dataGetByte( &currentByte, 0, NULL ) )
				return PROC_FAIL;
			if(storeTOC)
				d_toc[i].ID = currentByte;
			retVal ++;
			/* read status */
			if( !dataGetByte( &currentByte, 0, NULL ) )
				return PROC_FAIL;
			retVal ++;
			/* read uncompressed data set size */
			if(storeTOC)
				d_toc[i].uncomp_size = 0;
			j = 0;
			do{
				if( !dataGetByte(&currentByte, 0, NULL ) )
					return PROC_FAIL;
				else{
					retVal ++;
					if(storeTOC)
						d_toc[i].uncomp_size += (unsigned long) ((currentByte & 0x7F) << (7 * j));
					j++;
				}
			}
			while(currentByte & 0x80);

			/* read compression */
			if( !dataGetByte( &currentByte, 0, NULL ) )
				return PROC_FAIL;
			if(storeTOC)
				d_toc[i].compression = currentByte;
			retVal ++;
			/* read address */
			if(storeTOC)
				d_toc[i].address = 0x00000000;
			for(j = 0; j <4; j++){
				if( !dataGetByte(&currentByte, 0, NULL) )
					return PROC_FAIL;
				retVal ++;
				if(storeTOC){
					d_toc[i].address <<= 8;
					d_toc[i].address += currentByte;
				}
			}			
		}
		return retVal;
	}

	int dataRequestSet(unsigned char dataSet){
		short int i;
		unsigned char currentByte;

		for(i = 0; i < d_tocNumber; i++){
			if(d_toc[i].ID == dataSet){
				d_currentDataSetIndex = i;
				//requestAddress = d_toc[i].address;
				break;
			}
		}
		if(i == d_tocNumber)
			return PROC_FAIL;
		
		/****************************************************************** 
		* prepare data for reading
		* for streaming data, ignore data prior to the address
		* if the current address is bigger than requested address, reset
		* the stream
		******************************************************************/
		if(d_currentAddress > d_toc[d_currentDataSetIndex].address){
			i = d_currentDataSetIndex;
			dataReset(0);
			d_currentDataSetIndex = i;
		}

		set_compression(d_toc[d_currentDataSetIndex].compression);
		
		/* move currentAddress to requestAddress */
		while(d_currentAddress < d_toc[d_currentDataSetIndex].address){
			if( !dataGetByte( &currentByte, 1, NULL ) )
				return PROC_FAIL;
			
		}
		/* read BEGIN_OF_DATA */
		if( !dataGetByte( &currentByte, 1, &d_CSU ) || currentByte != 0xBA)
			return PROC_FAIL;
		if( !dataGetByte( &currentByte, 1, &d_CSU ) || currentByte != 0xBD)
			return PROC_FAIL;
		d_requestNewData = 0;
		return PROC_COMPLETE;
	}

	/********************************************************************
	*
	* End of Data Utility Functions
	*
	********************************************************************/


	/********************************************************************
	*
	* Decompression utility functions
	*
	********************************************************************/

	/********************************************************************
	* Global variable for decompression
	*********************************************************************/

	unsigned char compression          = 0;		
	unsigned char c_compByte           = 0;
	short int c_currentCounter         = 0;
	unsigned short int c_frameSize     = 0;
	unsigned short int c_frameCounter  = 0;

	void set_compression(unsigned char cmp){
		compression =  cmp;
	}

	unsigned char get_compression(){
		return compression;
	}


	/*********************************************************************
	* decomp_initFrame
	* Get a row of data from compressed data stream
	*
	*********************************************************************/

	short int decomp_initFrame(int bitSize){
		unsigned char compressMethod;
		
		if(!dataGetByte(&compressMethod, 1, &d_CSU)){
			return 0;
		}

		c_frameSize = (unsigned short int) (bitSize / 8);
		if(bitSize % 8 != 0)
			c_frameSize ++;	

		c_frameCounter = 0;

		switch(compressMethod){
		case 0x00:
			c_currentCounter = -1;
			break;

		case 0x01:
			c_currentCounter = 0;
			c_compByte = 0xFF;
			break;

		case 0x02:
			c_currentCounter = 0;
			c_compByte = 0x00;
			break;

		default:
			return 0;
		}
		return 1;
	}

	short int decomp_getByte(unsigned char *byteOut){
 		if(c_frameCounter >= c_frameSize)
			return 0;

		switch(c_currentCounter){
		case -1:
			if(!dataGetByte(byteOut, 1, &d_CSU))
				return 0;
			else {
				c_frameCounter ++;
				return 1;
			}
			break;

		case 0:
			if(!dataGetByte(byteOut, 1, &d_CSU))
				return 0;
			if(*byteOut == c_compByte){
				 if(! decomp_getNum())
					 return 0;

				 c_currentCounter --;
				 c_frameCounter ++;
				 return 1;
			}
			else{
				c_frameCounter ++;
				return 1;
			}
			break;

		default:
			*byteOut = c_compByte;
			c_currentCounter --;
			c_frameCounter ++;
			return 1;
		}
	}

	short int decomp_getNum(){
		unsigned char byteIn = 0x80;

		if(!dataGetByte(&byteIn, 1, &d_CSU)){
			return 0;
		}
		else{
			c_currentCounter = (short int) byteIn;
		}

		return 1;
	}

