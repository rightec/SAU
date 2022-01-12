/**
@file		Global.h
@brief		Global definitio and include. 
@author		Nicola Molinazzi
@date		18/01/2011
@version	01.00
*/



#ifndef _GLOBAL_H_
#define _GLOBAL_H_

/** GGG-4.3.1 define
*/
#define __GNUC__ 4
/** GGG-4.3.1 define
*/
#define __GNUG__ 4
/** GGG-4.3.1 define
*/
#define __GNUC_MINOR__ 3			
/** GGG-4.3.1 define
*/
#define __V851__
/** GGG-4.3.1 define
*/
#define __V850__
/** GGG-4.3.1 define
*/
#define __V851
/** GGG-4.3.1 define
*/
#define __V850
/** GGG-4.3.1 define
*/
#define ___cplusplus

#define lint

#define __IEEE_LITTLE_ENDIAN

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
//#include <math.h>
#include <fastmath.h>
#include "Arith.h"
/** 
	Loader start address.
*/
#define LOADER_FLASH_START_ADDRESS	0x00000000

#define LOADER_FLASH_STAY	0x00000008

/**
Application firmware start address.
*/
#define __APPLICATION_START_ADDRESS__	0x7000

/** 
	Firmware identification code. See file startup.s
*/
#define APPLICATION_MAGIC_NUMBER				0x8844CCEE	// memorizzarto all'indirizzo 0x600 - file startup.s
/** 
	Application firmware global checksum address. See file startup.s
*/
#define APPLICATION_GLOBAL_CHECKSUM_ADDRESS		0x0004
/** 
	Firmware identification code address. See file startup.s
*/
#define APPLICATION_MAGIC_NUMBER_ADDRESS		0x0600
/** 
	Application firmware number of segment address. See file startup.s
*/
#define APPLICATION_SEGMENT_NUMBER_ADDRESS  	0x0604
/** 
	Application firmware segment checksum address. See file startup.s
*/
#define APPLICATION_SEGMENT_CHECKSUM_ADDRESS	0x0614

/**
Application type
*/
#define APP_TYPE			APP_SAU		


//#define __DEBUG__							// debug mode oppure no
//#define __APP_IN_RAM__					// applicativo caricato in ram
//#define __LOG_ZIG_BEE_UART__			// comunicazione zig bee salvata su log file

//#define __POWER_SAFE_MODE__					// se utilizzare la modalità a basso consuno			
//#define __SIMUL__							// simulate mode oppure no
//#define __SDATA__							// utilizza il segmento sdata oppure no
//#define   __XTEXT__


#define True	true			
#define False	false

#define Enable	true			
#define Disable	false

#define ON		1
#define OFF		0

#define LOBYTE(x)             ((byte) ((x) & 0xff))
#define HIBYTE(x)             ((byte) ((x) >> 8))
#define BYTES_TO_WORD(hi,lo)  (word) (((word)(hi) << 8) | (word)(lo))

#define setBit(reg, bit)	  (reg |= (1 << bit))
#define clrBit(reg, bit)	  (reg &= ~(1 << bit)) 	

#define bitValue(reg, bit)	  (((reg & (1 << bit)) == (1 << bit)) ? 1 : 0)	
/**
Power voltage measured for diagnostic
*/
enum BoardVoltageMeasures
{
    _24VDC_ = 0,
    _12VDC_,
    _5VDC_DIGITAL_,
    _5VDC_ANALOG_,
    _5VDC_BUBBLE_,
    _V_2_5_,
    _V_1_2_,
    _VCC_VINF_,
    _VCC_VV_,
    _VCC_PRS14_,
    _VCC_PRS58_,
    _NUM_VOLTAGE_MEASURE_,
};

/**
Used of ADuC7061 microcontroller 
*/
enum ADuC7061_use 
{
	ADuC_VINF = 0, /**< ADuC7061 used to measure the infused volume*/
	ADuC_VV,     /**< ADuC7061 used to measure the voided volume*/	
	ADuC_P12,     /**< ADuC7061 used to measure P1 and P2*/
	ADuC_P34,     /**< ADuC7061 used to measure P3 and P4*/
	ADuC_P56,     /**< ADuC7061 used to measure P5 and P6*/
	ADuC_P78,     /**< ADuC7061 used to measure P7 and P8*/
	ADuC_GEN,     /**< ADuC7061 used to measure the generic channel 1 and 2*/
	ADuC_NUM,     /**< number of ADuC7061 microcontrolles used*/
};

/**
kind of weight chan
*/
enum kindOfWeightChan{
	_VINF_CHAN_ = 0,
	_VV_CHAN_,
	_NUM_OF_WEIGHT_CHAN_
};

/**
prs chan
*/
enum kindOfPrsChan{
	_PRS_01_ = 0,
	_PRS_02_,
	_PRS_03_,
	_PRS_04_,
	_PRS_05_,
	_PRS_06_,
	_PRS_07_,
	_PRS_08_,
	_NUM_OF_PRS_CHAN_ /**< Pressure channels number.*/
};

/**
kind of generic chan
*/
enum kindOfGenericChan{
	_GEN1_ = 0,
	_GEN2_,
	_NUM_OF_GENERIC_CHAN_
};


/**
High resolution EMG channel number.
*/
#define _MAX_HR_EMG_CHAN_	1

#define _MAX_HR_EIMP_CHAN_	3

/**
Weight buffer length.
*/
#define _WEIGHT_BUFFER_LENGTH_ 			4
/**
Pressure buffer length.
*/
#define _PRS_BUFFER_LENGTH_			  	40
/**
Generic channel buffer length.
*/
#define _GENERIC_BUFFER_LENGTH_  		200
/**
EMG buffer length.
*/
#define _HR_EMG_BUFFER_LENGTH_  		1000

/**
EMG buffer length.
*/
#define _HR_EIMP_BUFFER_LENGTH_  		30

/**
Power voltage buffer length.
*/
#define _PWR_VOLTAGE_BUFFER_LENGTH_  	5
/**
Current consumption buffer length.
*/
#define _PWR_CURRENT_BUFFER_LENGTH_  	5

/**
Number of weight samples per packets 
@see SauPcProtocol::sendSauPcWeightSamples
*/
#define _NUM_WEIGHT_SAMPLES_PER_PACKET_	2

/**
Number of prs samples per packets 
@see SauPcProtocol::sendSauPcPrsSamples
*/
#define _NUM_PRS_SAMPLES_PER_PACKET_	16

/**
Number of HR EMG samples per packets 
@see SauPcProtocol::sendSauPcEmgGenSamples
*/
#define _NUM_HR_EMG_SAMPLES_PER_PACKET_	60

/**
Number of Genric samples per packets 
@see SauPcProtocol::sendSauPcEmgGenSamples
*/
#define _NUM_GEN_SAMPLES_PER_PACKET_	20

#define MAX_GEN_GAIN_OFFSET				5 /*< Maximum number of different gain and offset that can be saved into the eeprom for each generic channel*/

struct gainOffset_t
{
	double gain;
	long offset;
};
typedef	gainOffset_t	gainOffset_t;

//const int powerConsumptionArray[5] = {/*LOGIC*/485.0, /*IMP*/570.0, /*pH*/760.0, /*PRS*/670.0, /*Sampling*/590.0}; // consumi in mW
//enum index_powerConsumptionArray{indexP_Logic = 0, indexP_IMP, indexP_pH, indexP_PRS, indexP_Sampling };

/*enum LANGUEGES
{
	ENGLISH = 0,
	ITALIAN,
	MAX_LANGUAGES
};*/

/**
word define
*/
typedef	unsigned short	word;
/**
byte define
*/
typedef unsigned char	byte;
/**
double word define
*/
typedef unsigned long	dword;

#ifdef __cplusplus
#else
typedef unsigned char	bool;
#endif

/**
It used to put in a const segment strings pointer array.
*/
typedef const char* textp;			

/**
It used to put in a const segment function pointer array.
*/
typedef void (*funp)();				
/**
It used to put in a const segment function pointer array.
*/
typedef void (*funpv)(void);	
/**
It used to put in a const segment function pointer array.
*/
typedef void (*funpi)(int);
/**
It used to put in a const segment function pointer array.
*/
typedef void (*funpb)(byte);

/**
Struct to manage bits variable.

16 bit are defined because the compilator reserves 2 bytes also if 8 bits variable was defined.
*/
struct BitVar{                       
    unsigned B0  : 1;                
    unsigned B1  : 1;                
    unsigned B2  : 1;                
    unsigned B3  : 1;
    unsigned B4  : 1;
    unsigned B5  : 1;
    unsigned B6  : 1;
    unsigned B7  : 1;
    unsigned B8  : 1;
    unsigned B9  : 1;
    unsigned B10 : 1;
    unsigned B11 : 1;
    unsigned B12 : 1;
    unsigned B13 : 1;
    unsigned B14 : 1;
    unsigned B15 : 1;
};

typedef struct BitVar BitVar;

union IoVar{                        /* aggiunta per gestione a word e char */
    BitVar  bits;
    word    var;
    char    sing[2];
};

typedef union IoVar IoVar;


typedef char             BYTE;
typedef unsigned char   UBYTE;
typedef short            WORD;
typedef unsigned short  UWORD;
typedef long             LONG;
typedef unsigned long   ULONG;


//extern LONG  tot[NUM_TOT];
//extern LONG  tot_cum[NUM_TOT];

/**
Display setting parametrs
*/
typedef struct
{
    byte brightness;
    byte contrast;
    byte backlight;
	byte writeSettingToEEPROM;
}DisplaySetting;


/**
Implements a fifo.
@param T type of data to fifo.
@param capacity fifo length.
*/
template <class T, int capacity>
class  CSmallRingBuf{

public:
	/**
	Class constructor.
	*/
	CSmallRingBuf(){clear(); }

	/**
	Returns if the fifo is empty.
	@return true if the fifo is empty, false otherwise.
	*/
	inline bool empty() const {return count == 0; }
	/**
	Returns if the fifo is full.
	@return true if the fifo is full, false otherwise.
	*/
	inline bool full() const { return count == capacity; }
	/**
	@return the size of T. 
	*/
	inline int size() const { return sizeof(T); }
	/**
	@return the number of items fifoed. 
	*/
	inline int numItem() const {return count;}
	/**
	@return the memrization index.
	*/
	inline int getMemIndex() const{return mem;}
	/**
	@return the fifo capacity. 
	*/
	inline int getCapacity() const{return capacity;}

	/**
	Push a data in the fifo. NOTE: it not check if the fifo is full.
	@param x data to push
	@see full
	*/
	inline void push_back(const T& x) 
								{ 
									datacpy( (byte *) &data[mem], (byte *) &x, (byte *) &data[mem+1]);  
									mem++; 
									count++; 
									if( mem >= capacity ) mem = 0; 
								};
	/**
	Pop a data from the fifo. NOTE: it not check if the fifo is empty.
	@param x data popped
	@see empty
	*/							
	inline void pop_front(T& x) { 
									x = data[get++];
									count--; 
									if( get >= capacity ) 
										get = 0; 
								};
	/**
	Reads a data from the fifo.
	@param x read data.
	@param index reading index
	*/
	inline void read(T& x, int index) { 
										if( index >= capacity ) 
											index = capacity -1;
										if (index < 0)
											index = 0;
										x = data[index];								
									 };
	/**
	Reads the last data pushed.
	@param x data read.
	*/
	inline void readLastPushed(T& x) { 
										int i;
										i = mem;
										if (i == 0)
										{
											i = capacity;
										}
										i--;
										x = data[i];
									};
	/**
	Empties the fifo
	*/
	inline void clear() { mem = get = count = 0; };

protected:
	/**
	Copies data in the fifo.
	@param pd pointer to the start position address within the fifo.
	@param ps poiter to the data to copy.
	@param pe poniter to the end address to copy 
	*/
	inline void datacpy(byte *pd, byte *ps, byte *pe) { 
								for(; pd<pe; ) 		
									*pd++=*ps++; }

	T		data[capacity];
	/**
	Memorization index
	*/
	volatile int	mem; 	
	/**
	Reading index.
	*/
	volatile int	get;	
	/**
	Number of element.
	*/
	volatile int	count;
};

class RingBuffer
{
	public:
		RingBuffer(unsigned size)
		{
			m_BufferSize = size;
			m_pRingBuffer = new unsigned char[m_BufferSize];
			Clear(true);
		}
		
		~RingBuffer()
		{
			if(m_pRingBuffer)
			{
				delete [] m_pRingBuffer;
			}
		}

		void Clear(bool reset = false)
		{
			//if(m_pRingBuffer)
			//{
				if(reset)
				{
					for(unsigned counter = 0; counter < m_BufferSize; counter++)
					{
						*(m_pRingBuffer + counter) = '\0';
					}
				}
				m_pCurrent = m_pRingBuffer;
				m_count = 0;
			//}
		}
		
		void Insert(unsigned char data)
		{
			//if(m_pRingBuffer)
			//{
				*m_pCurrent = data;
				m_pCurrent++;
				m_count++;
				if(m_pCurrent == (m_pRingBuffer + m_BufferSize))
				{
					m_pCurrent = m_pRingBuffer;
				}
			//}
		}

		bool Equals(const char *pdata, unsigned depth)
		{
			bool IsEqual = false;

			//if(m_pRingBuffer)
			//{
				if(m_count == 0)
					return false;
				
				unsigned char *pCurrent = m_pCurrent;

				for(; depth; depth--)
				{
					if(pCurrent == m_pRingBuffer)
					{
						pCurrent = m_pRingBuffer + m_BufferSize;
					}
					pCurrent--;
					if(*(pdata + depth - 1) != *pCurrent)
					{
						break;
					}
				}
				if(!depth)
				{
					IsEqual = true;
				}
			//}
			return IsEqual;
		}

		unsigned char* bufferData(){ return m_pRingBuffer;}
		
		int getNumData()
		{
			int len;
			if (m_count > m_BufferSize)
				len = m_BufferSize; 
			else 
				len = m_count; 
			return len;
		}
		 
	private:
		unsigned m_BufferSize;
		unsigned char *m_pCurrent;
		unsigned char *m_pRingBuffer;
		unsigned m_count;
};

				
#include "v850sg2.h"						// selezione in base al microbo..
#include "iodef.h"							// ridefinizione degli I/O in base al target
#include "irqsg2.h"
#include "error.h"

#endif
































