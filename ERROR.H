/**
@file		error.h
@brief		Module error manage. 
@author		Nicola Molinazzi
@date		18/01/2011
@version	01.00
*/


#ifndef _ERROR_H_
#define _ERROR_H_

#ifdef __cplusplus
extern "C" {
#endif


void error_init(void);

void error_trace(const char *sModule, const char *sFunction, int modErr);

#ifdef __cplusplus
}
#endif

#endif






























