//---------------------------------------------------------------------------//
//                                                                           //
//                           C O M E T A                                     //
//                                                                           //
//---------------------------------------------------------------------------//
//                                                                             
//  Progetto:                GENLIB
//  Nome File:               DPYDRV.CPP
//  Descrizione:             Definizioni per la selezione del driver 
//							 per il display controller desiderato
//  Supporto Fisico:         Hw Generico dipendenza dal drv incluso
//  Versione:                $Revision: 1.1 $
//  Data Versione:           $Date: 2002/11/22 00:00:00 $
//  Descrizione Versione:    Stesura iniziale
//  
//---------------------------------------------------------------------------//

#ifdef __DPYDRV__
#error __DPYDRV__ already defined !!
#else
#define __DPYDRV__ DRV_ST7565_SPI
#endif
#include "global.h"
#include "iodef.h"
#include "siosg2.h"
#include "error.h"
#include "chrset.h"
#include "sound.h"
#include "dpydrv.h"

										// include il file giusto...

#if (DPY_DRV == DRV_SED1375)
	#include "sed1375.h"
#elif (DPY_DRV == DRV_SED1330)
	#include "sed1330.h"
#elif (DPY_DRV == DRV_SED1335)
	#include "sed1335.h"
#elif DPY_DRV == DRV_T6963
	#include "t6963.h"
#elif DPY_DRV == DRV_KS0108
	#include "ks108.h"
#elif DPY_DRV == DRV_TL0313
	#include "tl0313.h"
#elif DPY_DRV == DRV_UC1601
	#include "uC1601.h"
#elif DPY_DRV == DRV_ST7565_SPI
	#include "ST7565R_SPI.h"
#endif


//-------------------------------------------------------
// Variabili globali utili a tutti i moduli
//-------------------------------------------------------

//-------------------------------------------------------
// Funzioni statiche utili solo a questo modulo
//-------------------------------------------------------

//-------------------------------------------------------
// Costanti statiche utili solo a questo modulo
//-------------------------------------------------------

//-------------------------------------------------------
// Variabili statiche utili solo a questo modulo
//-------------------------------------------------------
#ifdef DPYDRV_ENABLE_TEXT_EMULATION

DrvTextInfo		textInfo;			// informazioni per il driver in modalità testo

void drv_getTextInfo(DrvTextInfo *info)
{
	*info = textInfo;
}

#endif

DrvGraphInfo 		graphInfo;			// informazioni per il driver in modalità grafica



void drv_getGraphInfo(DrvGraphInfo *info)
{
	*info = graphInfo;
}


												// include il driver giusto...
#if (DPY_DRV == DRV_SED1375)
	#include "sed1375.cpp"
#elif (DPY_DRV == DRV_SED1330)
	#include "sed1330.cpp"
#elif (DPY_DRV == DRV_SED1335)
	#include "sed1335.cpp"
#elif (DPY_DRV == DRV_T6963)
	#include "t6963.cpp"
#elif (DPY_DRV == DRV_KS0108)
	#include "ks108.cpp"
#elif DPY_DRV == DRV_TL0313
	#include "tl0313.cpp"
#elif DPY_DRV == DRV_UC1601
	#include "uC1601.cpp"
#elif DPY_DRV == DRV_ST7565_SPI
	#include "ST7565R_SPI.cpp"
#endif











