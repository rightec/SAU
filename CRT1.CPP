/**
@file		Crti1.h
@brief		Initialize the data memory area coping from rodata.
@author		Nicola Molinazzi
@date		18/01/2011
@version	01.00
*/


#include "global.h"
/**
Defined in linker module.
*/
extern void (**__ctors)(void);
/**
Defined in linker module.
*/
extern void (**__ctors_end)(void);

/**
Defined in linker module.

Data area start address.
*/
extern char _data_start;
/**
Defined in linker module.

Data area end address.
*/
extern char _data_end;

/**
Defined in linker module.

Data area start load address. This is a flash address.
*/
extern char _rodata_end;

static short initialized;			


/** C++__main function.

   It comes before main function and calls all the constructor for the global object 
   and, if there are initialized data, it copies them from their flash immage to the ram.   
*/
extern "C" void __main ()
{
//void (**p)(void);
int data_size;
char *pdata, *promdata;

	#ifdef __APP_IN_RAM__
		initialized = 1;
	#endif
	
	if( !initialized )
	{
/*
		for(p=__ctors_end; p>__ctors; )				// anche i costruttori
			(*--p)();
*/
		if( &_data_start != &_data_end )
		{
			pdata = &_data_start;
			promdata = &_rodata_end;
			
			data_size = &_data_end - &_data_start;
			
			while( data_size > 0 )
			{
				*pdata++ = *promdata++;
				data_size--;
			}
		}
	}
}
























									
