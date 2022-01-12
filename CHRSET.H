//---------------------------------------------------------------------------//
//                                                                           //
//                           C O M E T A                                     //
//                                                                           //
//---------------------------------------------------------------------------//
//                                                                             
//  Progetto:                GENLIB  
//  Nome File:               CHRSET.H
//  Descrizione:             Definizioni per la selezione del set di caratteri
//							 per le modalità grafiche
//  Versione:                $Revision: 1.1 $
//  Data Versione:           $Date: 2002/11/19 00:00:00 $
//  
//---------------------------------------------------------------------------//

#ifndef __CHARSET_H__
#define __CHARSET_H__
//--------------------------------------------------------//
// Definizione delle costanti
//--------------------------------------------------------//
#define CHR_BASE_ERR	BASEMOD_CHRSET	// numero di partenza per gli errori
#define CHRSET_MAX		10				// prevediamo fino a 10 diversi set di caratteri


//--------------------------------------------------------//
// Definizione dei tipi
//--------------------------------------------------------//

enum CHRErrors{ CHR_ERR_NONE,
				CHR_ERR_END_SPACE=CHR_BASE_ERR,		// fine spazio disponibile
				CHR_ERR_INV_NAME,					// nome set car. non valido
				CHR_ERR_INV_SET						// set car. non valido
				};

typedef enum CHRErrors CHRErrors;



struct ChrSet{					// set di caratteri a bitmap
    char    name[16];			// nome del set di caratteri
    byte    w;					// width -> largezza del carattere (in pixel)
    byte    h;					// height -> altezza del carattere (in pixel)
    byte    firstChar;			// primo carattere del set
    byte    numChar;			// numero di caratteri contenuti nel set
    byte    maskBit;			// maschera di bit per l'ultimo byte del pattern 
    byte    lineSize;			// numero di byte usati per ogni riga di pattern carattere
    byte    blockSize;			// numero di byte usati per ogni carattere
    const byte 	*pSet;			// puntatore al pool di dati
};

typedef struct ChrSet ChrSet;

//--------------------------------------------------------//
// Definizione delle classi 
//--------------------------------------------------------//

//--------------------------------------------------------//
// Definizione delle funzioni
//--------------------------------------------------------//

#ifdef __cplusplus
extern "C" {
#endif

void charset_init(void);
bool charset_register(const ChrSet *pSet);
bool charset_select(const char *name);
const ChrSet *charset_getCurrent(void);
void charset_select_num(int numset);
int charset_width(const char *textstr);
int charset_height(const char *textstr);
int charset_getIndex(const char *nameSet);

#ifdef __cplusplus
}
#endif


//--------------------------------------------------------//
// Definizione delle variabili
//--------------------------------------------------------//

#endif




//@@-----------------------------------------------------------------------@@//
//@@                                                                             
//@@  $Source:  $
//@@  Note di revisione:                
//@@
//@@  $Log:  $
//@@
//@@  $Author: junior $
//@@  $RCSfile: CHRSET.H,v $
//@@  $Revision: 1.1 $
//@@  $State: Exp $
//@@  $Date: 2001/09/07 06:49:06 $
//@@  $Name:  $
//@@-----------------------------------------------------------------------@@//
//@@-----------------------------------------------------------------------@@//
//@@                                                                       @@//
//@@                            M E N F I S                                @@//
//@@                                                                       @@//
//@@-----------------------------------------------------------------------@@//

