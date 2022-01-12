/**
@file		Alloc.h
@brief		Functions to manage memory dynamic allocation.
@see 		Alloc.cpp
@author		
@date		18/01/2011
@version	01.00
*/


#ifndef _ALLOC_H_
#define _ALLOC_H_


#include "global.h"


/**
Errors start number.
*/
#define ALLOC_BASE_ERR		BASEMOD_ALLOC	


#ifdef ALLOC_ALIGNMENT
	#if (ALLOC_ALIGNMENT != 2 && ALLOC_ALIGNMENT != 4)
	#error "ALLOC_ALIGNMENT not supported!!!!"
	#endif
#else
/**
Memory defualt alignment. 
*/
#define ALLOC_ALIGNMENT	2						
#warning "ALLOC_ALIGNMENT set to 2 by default...."
#endif

/**
Allocation Errors
*/
enum ALLOCErrors{ MEM_ERR_NONE,							/**< No Error.*/
				  MEM_ERR_BLOCK_SIZE=ALLOC_BASE_ERR,	/**< Invalid block dimension.*/
				  MEM_ERR_END_OF_MEM,					/**< End of Memory.*/
				  MEM_ERR_INV_POINTER,					/**< Invalid pointer.*/
				  MEM_ERR_INV_BLOCK,					/**< Invalid block. Block can not be deallocated.*/
				};

typedef enum ALLOCErrors ALLOCErrors;


#ifdef __cplusplus
extern "C" {
#endif


void mem_init(void);

void mem_left(dword &freeMem, dword &topMem);
void mem_count(dword &numAllocBlock, dword &numFreeBlock);

bool mem_alloc(dword size, void **pMemBlock);
bool mem_free(void *pblock);

#ifdef __SMALL_HEAP__	

void small_mem_count(word &numAllocBlock, word &numFreeBlock);
void small_mem_left(word &freeMem, word &topMem);

bool small_mem_alloc(word size, void **pMemBlock);
bool small_mem_free(void *pblock);
#endif
#ifdef __cplusplus
}
#endif


#endif



























