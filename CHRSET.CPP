//---------------------------------------------------------------------------//
//                                                                           //
//                           C O M E T A                                     //
//                                                                           //
//---------------------------------------------------------------------------//
//                                                                             
//  Progetto:                GENLIB 
//  Nome File:               CHRSET.CPP
//  Descrizione:             Routines gestione set di caratteri 
//  Note:             		 Partiamo con un paio di set di sistema
//							 lasciando all'utente la possibilità
//							 di registrarne altri 
//  Supporto Fisico:         
//  Versione:                $Revision: 1.1 $
//  Data Versione:           $Date: 2002/11/19 00:00:00 $
//  Descrizione Versione:    Stesura iniziale
//  
//---------------------------------------------------------------------------//

#include "global.h"
#include "error.h"
#include "chrset.h"

//-------------------------------------------------------
// Variabili globali utili a tutti i moduli
//-------------------------------------------------------

//-------------------------------------------------------
// Funzioni statiche utili solo a questo modulo
//-------------------------------------------------------

//-------------------------------------------------------
// Costanti statiche utili solo a questo modulo
//-------------------------------------------------------

// occhio: set non valido
static const byte vgaSet[] __attribute__ ((section (".message"))) = {
				0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//     
				0x00,0xD8,0x6C,0x6C,0x6C,0x6C,0x6C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//     
				0x00,0x70,0x98,0x30,0x60,0xC8,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//     
				0x00,0x00,0x00,0x00,0x7C,0x7C,0x7C,0x7C,0x7C,0x7C,0x7C,0x00,0x00,0x00,0x00,0x00		//     
		};

static const ChrSet vgaCharSet = {
                "VGASET",   // nome del set di caratteri                               
                8, 			// width -> largezza del carattere (in pixel)              
                16,   		// height -> altezza del carattere (in pixel)              
                0,   		// primo carattere del set
                255,  		// numero di caratteri contenuti nel set                   
                0xFF, 		// maschera di bit per l'ultimo byte del pattern           
                1, 			// numero di byte usati per ogni riga di pattern carattere 
                16,   		// numero di byte usati per ogni carattere
                vgaSet 		// puntatore al pool di dati                               
            };








//-------------------------------------------------------                                                                                                                                                                               
// Variabili statiche utili solo a questo modulo                                                                                                                                                                                        
//-------------------------------------------------------                                                                                                                                                                               
static const ChrSet *chrSetTab[CHRSET_MAX];
static const ChrSet *currChrSet;
static CHRErrors chrError;
static word numChrSetReg;



/* Cerca di inizializzare la gestione dei set di caratteri
*/
void charset_init(void)
{
int i;

	for(i=0; i<CHRSET_MAX; i++)					// init statoli dei canali
		chrSetTab[i] = NULL;

	currChrSet = NULL;
	numChrSetReg = 0;
	chrError = CHR_ERR_NONE;		
	
	
	charset_register(&vgaCharSet);				// registra i due set di sistema
	
	charset_select("VGASET");					// e seleziona il primo....
}



/* Ritorna il puntatore al set di caratteri attualmente in uso
*/
const ChrSet *charset_getCurrent(void)
{
	return( currChrSet );						// puntatore corrente
}


/* Registra un set di caratteri tra quelli disponibili
*/
bool charset_register(const ChrSet *newSet)
{
	if( numChrSetReg >= CHRSET_MAX )
	{
		chrError = CHR_ERR_END_SPACE;

#ifdef __DEBUG__
		error_trace("CHRSET", "charset_register", chrError);		// aspetta che lo vengano a prendere
#endif
		return( False );
	}

	chrSetTab[numChrSetReg++] = newSet;								// aggiunge un ospite a tavola

	return(True);
}


/* Seleziona il set preregistrato come corrente
*/
bool charset_select(const char *nameSet)
{
int i;
const ChrSet **psel = &chrSetTab[0];


	if( nameSet == 0 || nameSet[0] == 0 )
	{
		chrError = CHR_ERR_INV_NAME;

#ifdef __DEBUG__
		error_trace("CHRSET", "charset_select", chrError);		// aspetta che lo vengano a prendere
#endif
		return( False );
	}

	for(i=0; i<numChrSetReg; i++, psel++)						// cerca tra quelli già registrati
		if( strcmp(nameSet, (*psel)->name) == 0 )				// se è questo...
		{
			currChrSet = *psel;									// fatto !!
			return( True );
		}

#ifdef __DEBUG__
		chrError = CHR_ERR_INV_SET;
		error_trace("CHRSET", "charset_select", chrError);		// aspetta che lo vengano a prendere
#endif

	return(False);												// sorry ...
}


/* Seleziona il set preregistrato come corrente
*/
int charset_getIndex(const char *nameSet)
{
int i;
const ChrSet **psel = &chrSetTab[0];


	if( nameSet == 0 || nameSet[0] == 0 )
	{
		chrError = CHR_ERR_INV_NAME;

#ifdef __DEBUG__
		error_trace("CHRSET", "charset_getIndex", chrError);		// aspetta che lo vengano a prendere
#endif
		return( 0 );
	}

	for(i=0; i<numChrSetReg; i++, psel++)						// cerca tra quelli già registrati
		if( strcmp(nameSet, (*psel)->name) == 0 )				// se è questo...
		{
			return( i );
		}

#ifdef __DEBUG__
		chrError = CHR_ERR_INV_SET;
		error_trace("CHRSET", "charset_getIndex", chrError);		// aspetta che lo vengano a prendere
#endif

	return(0);												// sorry ...
}

/* Seleziona il set preregistrato come corrente
*/
void charset_select_num(int numset)
{
	currChrSet = chrSetTab[numset];
}

int charset_width(const char *textstr)
{
int i = strlen(textstr);

	return( currChrSet->w * i );
}

int charset_height(const char *textstr)
{
	return( currChrSet->h );
}

