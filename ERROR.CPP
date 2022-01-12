/**
@file		error.cpp
@brief		Module error manage. 
@author		Nicola Molinazzi
@date		18/01/2011
@version	01.00
*/

#include "global.h"		
#include "error.h"				

/**
Global errors counter.
*/
static word errorCount;

/**
Initialize the error module.
*/
void error_init(void)
{
	errorCount = 0;
}

/**
Shows an error and dispaly it on the video.
@param module that rose the error.
@param function whitin the module that rose the error.
@param error number.
*/
void error_trace(const char *sModule, const char *sFunction, int modErr)
{
//char s[200];

	errorCount++;										// uno in +
	
#ifdef __DEBUG__
	
	while (1)
	{// printa brutalmente						
		setcolor(12);
		charset_select("xm6x10");
		gprintfxy(10, 10, "Trace error: mod(%12s)", sModule);
		gprintfxy(10, 20, "fun(%16s) - cod(%5d)", sFunction, modErr);
	}
#else
	modErr = strcmp(sModule, sFunction);				// solo per fare tacere il compilatore

#endif

}













