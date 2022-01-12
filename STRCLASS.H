/**
@file		strclass.h
@brief		Classes to manage dymanic strings.

@author		
@date		18/01/2011
@version	01.0
*/


#ifndef _STRCLASS_H_
#define _STRCLASS_H_


#include "global.h"

//--------------------------------------------------------//
// Definizione delle costanti
//--------------------------------------------------------//

//--------------------------------------------------------//
// Definizione dei tipi
//--------------------------------------------------------//

//--------------------------------------------------------//
// Definizione delle classi 
//--------------------------------------------------------//

/**
Class to manage dynamic strings.
*/
class String
{

public:

    String();
    String(const char *);
    String(const String &);

	/**
	Class destructor
	*/
    virtual ~String() { delete theString; }

    void init(char *);
    inline operator const char *() const { return( theString ); }
	char* getChar(){return theString;};
    inline word getStrLen(void) { return(buflen - 1); }

    String& operator = ( const String & );
    String& operator = ( const char * );

    inline int operator == (const char *s) { return( strcmp(s, theString) == 0 ); }
    inline int operator != (const char *s) { return( strcmp(s, theString) ); }

protected:

    word    buflen;
    word    maxlen;
    char    *theString;

};


//--------------------------------------------------------//
// Definizione delle funzioni
//--------------------------------------------------------//


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
//@@  $RCSfile: STRCLASS.H,v $
//@@  $Revision: 1.1 $
//@@  $State: Exp $
//@@  $Date: 2001/09/07 06:49:06 $
//@@  $Name:  $
//@@-----------------------------------------------------------------------@@//
//@@-----------------------------------------------------------------------@@//
//@@                                                                       @@//
//@@                            C O M E T A                                @@//
//@@                                                                       @@//
//@@-----------------------------------------------------------------------@@//

