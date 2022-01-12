/**
@file		Alloc.cpp
@brief		Functions to manage memory dynamic allocation.

			Here are dcumented only the functions not present in the file Alloc.h 
@see 		Alloc.h
@author		
@date		18/01/2011
@version	01.00
*/


#include "alloc.h"


/**
Heap base address. Defined in linker module.
*/
extern byte _base_heap;
/**
Heap end address. Defined in linker module.
*/
extern byte _end_heap;

/**
Small heap base address. Defined in linker module.
*/
extern byte _base_small_heap;
/**
Small heap end address. Defined in linker module.
*/
extern byte _end_small_heap;

/**
Top of user heap.
*/
static byte *pTopHeap = &_base_heap;
/**
Top of user small heap.
*/
static byte *pTopHeapSmall = &_base_small_heap;

static ALLOCErrors memError;

/**
Small heap allocated blocks number.
*/
static word cntAllocSmall;
/**
Small heap freed blocks number.
*/
static word cntFreeSmall;					
/**
Small heap free memory.
*/
static word memLeftSmall;	

/**
Heap allocated blocks number.
*/
static dword cntAlloc;						
/**
Heap freed blocks number.
*/
static dword cntFree;						
/**
Heap free memory.
*/
static dword memLeft;						

/** 
Small heap and Heap memory initialization.
*/
void mem_init(void)
{

	memError = MEM_ERR_NONE;
	pTopHeap = &_base_heap;				// inizio heap definito dal linker
	memLeft = &_end_heap - &_base_heap;	// memoria attualmente libera

	cntAlloc = 0;						// reset blocchi allocati & liberati
	cntFree = 0;
	cntAllocSmall = 0;					// numero di blocchi di memoria allocati
	cntFreeSmall = 0;					// numero di blocchi di memoria liberati

	pTopHeapSmall = &_base_small_heap;				// inizio heap definito dal linker
	pTopHeapSmall += 2;
	memLeftSmall = &_end_small_heap - &_base_small_heap;	// memoria attualmente libera
}

/**
	Returns the number of allocation and deallocation executed on heap memory.
	
	@param numAllocBlock number of allocation done.
	@param numFreeBlock number of deallocation done.
*/
void mem_count(dword &numAllocBlock, dword &numFreeBlock)
{

	numAllocBlock = cntAlloc;
	numFreeBlock = cntFree;
}

/** 
	Returns the heap free memory.

	@param freeMem free memory NOT fragmented.
	@param topMem comprehensive free memory (fragmented).
*/
void mem_left(dword &freeMem, dword &topMem)
{

	topMem = (dword) (&_end_heap - pTopHeap);	// memoria ancora libera libera (non frammentata)
	freeMem = memLeft;							// memoria libera complessiva (frammentata)
}

/**
	Memory dynamic allocation - Heap.
	- the maximum allocatable block dimension is 2Gb (0x7FFFFFFF)
	- the most significant bit, if 1, marks the block as freed
	- only 4 bytes are used to each block beyond the block size.
	@param size block dimension. It has to be lower than or equal to 0x800000.
	@param pMemBlock memory pointer.
	
*/
bool mem_alloc(dword size, void **pMemBlock)
{
dword *pmem = NULL;
dword *pfree;
byte *pheap;
dword topMem, blockSize;


	*pMemBlock = NULL;							// iniziamo bene

#if (ALLOC_ALIGNMENT == 2)						// se la vogliono allineata alla solita word
	if( (size & 0x0001) )						// allinea alla word
		size++;
#elif (ALLOC_ALIGNMENT == 4)					// se sono un po' + spreconi
	if( (size & 0x0001) )						// allinea alla word
		size++;

	if( ((size + 2)& 0x0002) )					// se non basta (considera comunque gli extra)
		size += 2;								// allinea alla long word
#else
	if( (size & 0x0001) )						// allinea alla word
		size++;									// allineamento d'ufficio .....
#endif

	if( size > 0x800000 )						// controlla comunque la dim. massima del blocco
	{
#ifdef __DEBUG__
		memError = MEM_ERR_BLOCK_SIZE;
		error_trace("ALLOC", "mem_alloc", memError);	// segnala la cappella sistina
		return(False);
#else
		return(False);
#endif
	}


	if( memLeft - 4 < size )					// se complessivamente non basta
	{
#ifdef __DEBUG__
			memError = MEM_ERR_END_OF_MEM;		// non ce n'è proprio +
			error_trace("ALLOC", "mem_alloc", memError);	// segnala la cappella sistina
			return(False);
#else
			return(False);
#endif
	}


	topMem = (dword) (&_end_heap - pTopHeap);	// memoria ancora libera libera (non frammentata)

	if( topMem - 4 > size )						// se ci sta nel blocco non frammentato
	{
		pmem = (dword *) pTopHeap;				// punto di partenza del prossimo blocco
		*pmem = size;							// marca la dimensione del blocco

		pTopHeap += size + 4;					// nuovo punto di partenza heap
	}
	else
	{
		for(pheap=&_base_heap; pheap<pTopHeap; )	// cerca tra le frattaglie
		{
			pmem = (dword *) pheap;

			if( (*pmem & 0x80000000) != 0 )			// se il blocco è 
			{
				if( (*pmem & 0x7FFFFFFF) >= size )	// se è grande abbastanza
				{
					blockSize = *pmem & 0x7FFFFFFF;	// registra: lo rimette in gioco

					if( blockSize > size + 20 )	// se ce ne avanza un bel goccio
					{
						*pmem = size;			// alloca solo quello che serve

						pheap += size + 4;		// va all'inizio del prossimo blocco

						pfree = (dword *) pheap; 			// passa all'ala
												// marca come se fosse un blocco liberato
						*pfree = 0x80000000 | (blockSize - size);
					}
					else
					{
						*pmem = size = blockSize;	// tiene anche quello che avanza
					}
					break;						// fine della ricerca
				}
			}

			pheap += (*pmem & 0x7FFFFFFF) + 4;		// avanti all'inizio del prossimo blocco
		}

		if( pheap == pTopHeap )					// se non trova un buco libero
		{
#ifdef __DEBUG__
			memError = MEM_ERR_END_OF_MEM;		// non ce n'è proprio +
			error_trace("ALLOC", "mem_alloc", memError);	// segnala la cappella sistina
			return(False);
#else
			return(False);
#endif
		}

	}

	memLeft -= size + 4;						// la memoria cala...
	*pMemBlock = (void *) (pmem+1);				// inizio reale del blocco (4 bytes dopo)
	cntAlloc++;

	return( True );								// tutto ok!!
}

/**
Frees the memory at pblock - Heap.
@param pblock address of the block to free. 
*/
bool mem_free(void *pblock)
{
byte *pheap = (byte *) pblock;
dword *pmem = (dword *) pblock;

	if( pheap == NULL || pheap < &_base_heap || pheap > &_end_heap )	// se spara fuori
	{
#ifdef __DEBUG__
		memError = MEM_ERR_INV_POINTER;
		error_trace("ALLOC", "mem_free", memError);	// segnala la cappella sistina
		return(False);
#else
		return(False);								// si limita al dissenso
#endif
	}

	pmem--;											// punta al descrittore del blocco

	if( (*pmem & 0x80000000) != 0 )					// se era già cancellato
	{
#ifdef __DEBUG__
		memError = MEM_ERR_INV_BLOCK;
		error_trace("ALLOC", "mem_free", memError);	// segnala la cappella sistina
		return(False);
#else
		return(False);								// si limita al dissenso
#endif
	}


	pheap = (byte *) pmem;							// inizio del blocco

	pheap += *pmem + 4;								// punta all'inizio del blocco succ.

	if( pheap == pTopHeap )							// se è l'ultimo blocco allocato
	{

		memLeft += (*pmem) + 4;						// tiene conto della memoria recuperata

		do{											// verifica se qualche altro blocco
													// adiacente all'ultimo liberato era
													// stato liberato in precedenza
			pTopHeap = (byte *) pmem;				// nuovo punto di partenza heap libera
													// il blocco non solo e' libero ma non
													// esiste proprio +
			pmem = (dword *) &_base_heap;

			for(pheap=&_base_heap; pheap<pTopHeap;) // trova l'ultimo blocco prima del top
			{
				pmem = (dword *) pheap;

				pheap += (*pmem & 0x7FFFFFFF) + 4;	// avanza
			}
													// in pmem rimane l'inizio dell'ultimo blocco
													// prima del "tetto"
			if( (*pmem & 0x80000000) == 0 )				// se il penultimo blocco e' ancora in uso
				break;								// non lo posso buttare via

		}while( pmem != (dword *) pheap );			// fino a che ne trova uno

	}
	else
	{
		memLeft += *pmem + 4;						// tiene conto della memoria recuperata
		*pmem |= 0x80000000;						// marca il blocco come libero
	}

	cntFree++;										// un blocco in + liberato

	return(True);
}



#ifdef __SMALL_HEAP__
/**
	Returns the number of allocation and deallocation executed on small heap memory.
	
	@param numAllocBlock number of allocation done.
	@param numFreeBlock number of deallocation done.
*/
void small_mem_count(word &numAllocBlock, word &numFreeBlock)
{

	numAllocBlock = cntAllocSmall;
	numFreeBlock = cntFreeSmall;
}

/** 
	Returns the small heap free memory.

	@param freeMem free memory NOT fragmented.
	@param topMem comprehensive free memory (fragmented).
*/
void small_mem_left(word &freeMem, word &topMem)
{

	topMem = (word) (&_end_small_heap - pTopHeapSmall);	// memoria ancora libera libera (non frammentata)
	freeMem = memLeftSmall;							// memoria libera complessiva (frammentata)
}

/**
	Memory dynamic allocation - Small Heap.
	- the maximum allocatable block dimension is 32Kb (0x7FFF)
	- the most significant bit, if 1, marks the block as freed
	- only 2 bytes are used to each block beyond the block size.
	@param size block dimension. It has to be lower than or equal to 0x800000.
	@param pMemBlock memory pointer.
	
*/
bool small_mem_alloc(word size, void **pMemBlock)
{
word *pmem = NULL;
word *pfree;
byte *pheap;
word topMem, blockSize;


	*pMemBlock = NULL;							// iniziamo bene

#if (ALLOC_ALIGNMENT == 2)						// se la vogliono allineata alla solita word
	if( (size & 0x0001) )						// allinea alla word
		size++;
#elif (ALLOC_ALIGNMENT == 4)					// se sono un po' + spreconi
	if( (size & 0x0001) )						// allinea alla word
		size++;

	if( (size & 0x0002) == 0 )					// se è già allineato alla long word
		size += 2;								// allinea alla long word+2bytes
#else
	if( (size & 0x0001) )						// allinea alla word
		size++;									// allineamento d'ufficio .....
#endif

	if( size > 0x8000 )							// controlla comunque la dim. massima del blocco
	{
#ifdef __DEBUG__
		memError = MEM_ERR_BLOCK_SIZE;
		error_trace("ALLOC", "small_mem_alloc", memError);	// segnala la cappella sistina
		return(False);
#else
		return(False);
#endif
	}


	if( memLeftSmall - 4 < size )					// se complessivamente non basta
	{
#ifdef __DEBUG__
			memError = MEM_ERR_END_OF_MEM;		// non ce n'è proprio +
			error_trace("ALLOC", "small_mem_alloc", memError);	// segnala la cappella sistina
			return(False);
#else
			return(False);
#endif
	}


	topMem = (word) (&_end_small_heap - pTopHeapSmall);	// memoria ancora libera libera (non frammentata)

	if( topMem - 4 > size )						// se ci sta nel blocco non frammentato
	{
		pmem = (word *) pTopHeapSmall;				// punto di partenza del prossimo blocco
		*pmem = size;								// marca la dimensione del blocco

		pTopHeapSmall += size + 2;					// nuovo punto di partenza heap
	}
	else
	{
		for(pheap=&_base_small_heap; pheap<pTopHeapSmall; )	// cerca tra le frattaglie
		{
			pmem = (word *) pheap;

			if( (*pmem & 0x8000) != 0 )			// se il blocco è 
			{
				if( (*pmem & 0x7FFF) >= size )	// se è grande abbastanza
				{
					blockSize = *pmem & 0x7FFF;	// registra: lo rimette in gioco

					if( blockSize > size + 20 )	// se ce ne avanza un bel goccio
					{
						*pmem = size;			// alloca solo quello che serve

						pheap += size + 2;		// va all'inizio del prossimo blocco

						pfree = (word *) pheap; // passa all'ala
												// marca come se fosse un blocco liberato
						*pfree = 0x8000 | (blockSize - size);
					}
					else
					{
						*pmem = size = blockSize;	// tiene anche quello che avanza
					}
					break;						// fine della ricerca
				}
			}

			pheap += (*pmem & 0x7FFF) + 2;		// avanti all'inizio del prossimo blocco
		}

		if( pheap == pTopHeapSmall )					// se non trova un buco libero
		{
#ifdef __DEBUG__
			memError = MEM_ERR_END_OF_MEM;		// non ce n'è proprio +
			error_trace("ALLOC", "small_mem_alloc", memError);	// segnala la cappella sistina
			return(False);
#else
			return(False);
#endif
		}

	}

	memLeftSmall -= size + 2;					// la memoria cala...
	*pMemBlock = (void *) (pmem+1);				// inizio reale del blocco (2 words dopo)
	cntAllocSmall++;

	return( True );								// tutto ok!!
}

/**
Frees the memory at pblock - Small Heap.
@param pblock address of the block to free. 
*/
bool small_mem_free(void *pblock)
{
byte *pheap = (byte *) pblock;
word *pmem = (word *) pblock;

	if( pheap == NULL || pheap < &_base_small_heap || pheap > &_end_small_heap )	// se spara fuori
	{
#ifdef __DEBUG__
		memError = MEM_ERR_INV_POINTER;
		error_trace("ALLOC", "small_mem_free", memError);	// segnala la cappella sistina
		return(False);
#else
		return(False);								// si limita al dissenso
#endif
	}

	pmem--;											// punta al descrittore del blocco

	if( (*pmem & 0x8000) != 0 )						// se era già cancellato
	{
#ifdef __DEBUG__
		memError = MEM_ERR_INV_BLOCK;
		error_trace("ALLOC", "small_mem_free", memError);	// segnala la cappella sistina
		return(False);
#else
		return(False);								// si limita al dissenso
#endif
	}


	pheap = (byte *) pmem;							// inizio del blocco

	pheap += *pmem + 2;								// punta all'inizio del blocco succ.

	if( pheap == pTopHeapSmall )					// se è l'ultimo blocco allocato
	{

		memLeftSmall += (*pmem) + 2;						// tiene conto della memoria recuperata

		do{											// verifica se qualche altro blocco
													// adiacente all'ultimo liberato era
													// stato liberato in precedenza
			pTopHeapSmall= (byte *) pmem;				// nuovo punto di partenza heap libera
													// il blocco non solo e' libero ma non
													// esiste proprio +
			pmem = (word *) &_base_small_heap;
			pmem++;

			for(pheap= (byte *)pmem; pheap<pTopHeapSmall;) // trova l'ultimo blocco prima del top
			{
				pmem = (word *) pheap;

				pheap += (*pmem & 0x7FFF) + 2;		// avanza
			}
													// in pmem rimane l'inizio dell'ultimo blocco
													// prima del "tetto"
			if( (*pmem & 0x8000) == 0 )				// se il penultimo blocco e' ancora in uso
				break;								// non lo posso buttare via

		}while( pmem != (word *) pheap );			// fino a che ne trova uno

	}
	else
	{
		memLeftSmall += *pmem + 2;						// tiene conto della memoria recuperata
		*pmem |= 0x8000;							// marca il blocco come libero
	}

	cntFreeSmall++;									// un blocco in + liberato

	return(True);
}




//extern "C" {

void * malloc(size_t size)
{
void *p;

	if( mem_alloc(size, &p) == False )
		return(NULL);

	return(p);
}


void *calloc(size_t _nmemb, size_t size)
{
void *p;

	size *= _nmemb;
	
	if( mem_alloc(size, &p) == False )
		return(NULL);

	memset(p, 0, size);
	
	return(p);
}

void *_calloc_r(struct _reent *pr, size_t _nmemb, size_t size)
{
void *p;

	while( pr->_inc != 0 );
	
	pr->_inc++;

	p = calloc(_nmemb, size);
	
	pr->_inc--;
	
	return( p );
}

void free(void *p)
{
	mem_free(p);
}

//}		// extern "C"

#endif













