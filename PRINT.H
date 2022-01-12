//---------------------------------------------------------------------------//
//                                                                           //
//                           P I C S E L                                     //
//                                                                           //
//---------------------------------------------------------------------------//
//                                                                             
//  Progetto:                MULTICH
//  Nome File:               PRINT.H
//  Descrizione:             Definizioni per modulo sw PRINT.CPP
//  Versione:                $Revision: 1.3 $
//  Data Versione:           $Date: 2006/10/03 13:08:09 $
//  
//---------------------------------------------------------------------------//

#ifndef _PRINT_H_
#define _PRINT_H_

#include "global.h"
#include "error.h"

//--------------------------------------------------------//
// Definizione delle costanti
//--------------------------------------------------------//
/*#define isxdigit(c) (((c) >= '0' && (c) <= '9') || \
                     ((c) >= 'a' && (c) <= 'f') || \
                     ((c) >= 'A' && (c) <= 'F'))
*/                     
/* we use this so that we can do without the ctype library */
/*#define is_digit(c)	((c) >= '0' && (c) <= '9') */
/*#define isdigit(c)  ((c) >= '0' && (c) <= '9') */
/*#define islower(c)  ((c) >= 'a')
#define toupper(c)  ((c) - 'a' + 'A')
*/

#define ZEROPAD	0x01		/* pad with zero */
#define SIGN	0x02		/* unsigned/signed long */
#define PLUS	0x04		/* show plus */
#define SPACE	0x08		/* space if plus */
#define LEFT	0x10		/* left justified */
#define SPECIAL	0x20		/* 0x */
#define LARGE	0x40		/* use 'ABCDEF' instead of 'abcdef' */


//--------------------------------------------------------//
// Definizione dei tipi
//--------------------------------------------------------//

//--------------------------------------------------------//
// Definizione delle classi 
//--------------------------------------------------------//

//--------------------------------------------------------//
// Definizione delle funzioni
//--------------------------------------------------------//
#ifdef __cplusplus
extern "C" {
#endif

unsigned long _strtoul(const char *cp, char **endp, unsigned int base);


#ifdef __cplusplus
}
#endif
//--------------------------------------------------------//
// Definizione delle variabili
//--------------------------------------------------------//
extern int sprintfMaxCount;


#endif

//@@-----------------------------------------------------------------------@@//
//@@                                                                     
//@@  $Source: /root/ConfTx/PRINT.H,v $
//@@  Note di revisione:                
//@@
//@@  $Revision: 1.3 $
//@@  $Date: 2006/10/03 13:08:09 $
//@@  			Viene aggiunta una variabile "sprintfMaxCount" per il 
//@@			controllo della lunghezza  max. di "formattazione" 
//@@			della funzione "sprintf".
//@@
//@@  $Log: PRINT.H,v $
//@@  Revision 1.3  2006/10/03 13:08:09  giovanni
//@@  *** empty log message ***
//@@
//@@
//@@  $Author: giovanni $
//@@  $RCSfile: PRINT.H,v $
//@@  $Revision: 1.3 $
//@@  $State: Exp $
//@@  $Date: 2006/10/03 13:08:09 $
//@@  $Name:  $
//@@-----------------------------------------------------------------------@@//
//@@-----------------------------------------------------------------------@@//
//@@                                                                       @@//
//@@                            P I C S E L                                @@//
//@@                                                                       @@//
//@@-----------------------------------------------------------------------@@//
