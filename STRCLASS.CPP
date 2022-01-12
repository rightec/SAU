/**
@file		strclass.cpp
@brief		Classes to manage dynamic strings.

@author		
@date		18/01/2011
@version	01.0
*/

#include "strclass.h"

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


//-------------------------------------------------------
// Corpo funzioni di questo modulo
//-------------------------------------------------------
/**
	Class constructor.
	Constructs an empty string;
	
*/
String::String()
{
	theString = (char*)"";
	maxlen = buflen = 0;
}

/**
	Class constructor.
	Constructs a string initialized with the characters of the s array.
	@param s array of const char  
	
*/
String::String( const char *s )
{
    if( s == 0 )
        s = "";

    buflen = strlen( s ) + 1;
	theString = new char[ (unsigned int) buflen ];

    if( theString != NULL )
        strcpy( theString, s );
    else
        buflen = 0;

    maxlen = buflen;
}

/**
	Class constructor.
	Constructs a string initialized with the characters of the sourceString.
	@param sourceString String  
	
*/
String::String( const String& sourceString )
{
    buflen = sourceString.buflen;
    theString = new char[ buflen ];

    if( theString )
        strcpy( theString, sourceString.theString );
    else
        buflen = 0;

    maxlen = buflen;
}

/**
Copy into the string the array source. 
*/
void String::init(char *source)
{
word newlen;

    if( source == 0 )
        source = (char*)"";

    newlen = strlen( source ) + 1;

	if( maxlen < newlen )
    {
    	if( maxlen )					// valid-p?
	        delete theString;

        buflen = newlen;

        theString = new char[ buflen ];

        if( !theString )
        {
            buflen = maxlen = 0;
            theString = (char*)"";
        }
		else
            maxlen = buflen;
    }

    if( maxlen )
        strcpy(theString, source);
}

/**
Assigns sourceString to this string and returns a reference to this string.
*/
String& String::operator =( const String& sourceString )
{

    if( *this != sourceString )
    {
        if( maxlen < sourceString.buflen )
        {
	    	if( maxlen )					// valid-p?
    	        delete theString;

            buflen = sourceString.buflen;

            theString = new char[ buflen ];

	        if( !theString )
	        {
	            buflen = maxlen = 0;
	            theString = (char*)"";
	        }
			else
	            maxlen = buflen;
        }else
    	{
			buflen = sourceString.buflen;
		}

        if( maxlen )
            strcpy( theString, sourceString.theString );
    }
    return( *this );
}

/**
Assigns source to this string and returns a reference to this string.
*/
String& String::operator =( const char *source )
{
word newlen;

    if( source == 0 )
        source = (char*)"";

    newlen = strlen( source ) + 1;

    if( maxlen < newlen )
    {
    	if( maxlen )					// valid-p?
    	    delete theString;
	
        buflen = newlen;

        theString = new char[ buflen ];

        if( !theString )
        {
            buflen = maxlen = 0;
            theString = (char*)"";
        }
		else
            maxlen = buflen;
    }else
    {
		buflen = newlen;
	}
	
    if( maxlen )
        strcpy(theString, source);

    return( *this );
}



//@@-----------------------------------------------------------------------@@//
//@@
//@@  $Source:  $
//@@  Note di revisione:
//@@
//@@  $Revision: 1.1 $
//@@  $Date: 2004/01/08 00:00:00 $
//@@  	- Aggiunta primo porting dalla versione per PC-DOS
//@@
//@@
//@@  $Log:  $
//@@
//@@  $Author: junior $
//@@  $RCSfile: STRCLASS.CPP,v $
//@@  $Revision: 1.1 $
//@@  $State: Exp $
//@@  $Date: 2002/12/16 00:00:00 $
//@@  $Name:  $
//@@-----------------------------------------------------------------------@@//
//@@-----------------------------------------------------------------------@@//
//@@                                                                       @@//
//@@                            C O M E T A                                @@//
//@@                                                                       @@//
//@@-----------------------------------------------------------------------@@//
