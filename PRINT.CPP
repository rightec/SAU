/**
@file		Print.h
@brief		String formatting module
					
@author		Enrico
@date		18/01/2011
@version	01.4
*/


#include "print.h"

//-------------------------------------------------------
// Variabili globali utili a tutti i moduli
//-------------------------------------------------------
									// inizializzato con un valore consistente
int sprintfMaxCount = 80;
#ifdef __DEBUG__
/**
local counter
*/
static int  charCount;		
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
skip_atoi
*/
static int skip_atoi(const char **s);

/**
number
*/
static char *number(char * str, long num, int base, int size, int precision ,int type);

/**
abort
*/
void abort(void);

#ifdef __cplusplus
}
#endif

//-------------------------------------------------------
// Costanti statiche utili solo a questo modulo
//-------------------------------------------------------
static const char lowerDigits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
static const char upperDigits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const word decimalAdjTab[] = { 1, 10, 100, 1000, 10000 };

//-------------------------------------------------------
// Variabili statiche utili solo a questo modulo
//-------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif

//-------------------------------------------------------
// Corpo funzioni di questo modulo
//-------------------------------------------------------


/**
 sprintf 
 */
int sprintf(char * buf, const char *fmt, ...)
{
va_list args;
int i;

	va_start(args, fmt);

	i = vsprintf(buf,fmt,args);

	va_end(args);

	return( i );
}


/**
The function sprintf call this funtion.

It's a small version that does not parse floating point number
*/
int vsprintf(char *buf, const char *fmt, va_list args)
{
int len;
unsigned long num;
short short_num;
int i, base;
int flags;			/* flags to number() */
int field_width;	/* width of output field */
int precision;		/* min. # of digits for integers; max number of chars for from string */
int qualifier;		/* 'h', 'l', or 'L' for integer fields */

long int_value;
float flnum;
short frac_value;
word decimal_div;

const char *s;
register char  *str;

	for(str=buf; *fmt; ++fmt) 
	{
		
		
#ifdef __DEBUG__

		charCount = str - buf;

		if( charCount >= sprintfMaxCount )
		{
			error_trace("PRINT", "vsprintf", charCount);	// segnala la cappella sistina
			*str = '\0';
			return(charCount);
		}
#endif

		if( *fmt != '%') 
		{
			*str++ = *fmt;		// Just copy !!
			continue;
		}

		/* process flags */
		flags = 0;
		
repeat:
		++fmt;		/* this also skips first '%' */
		
		switch( *fmt ) 
		{
			case '-': flags |= LEFT; goto repeat;
			case '+': flags |= PLUS; goto repeat;
			case ' ': flags |= SPACE; goto repeat;
			case '#': flags |= SPECIAL; goto repeat;
			case '0': flags |= ZEROPAD; goto repeat;
		}
		
		/* get field width */
		field_width = -1;

		if( isdigit(*fmt) )
		{
			field_width = skip_atoi(&fmt);
		}	
		else 
		{
			if( *fmt == '*' )
			{
				++fmt;
	
				/* it's the next argument */
				field_width = va_arg(args, int);
			
				if( field_width < 0 ) 
				{
					field_width = -field_width;
					flags |= LEFT;
				}
			}
		}

		/* get the precision */
		precision = -1;
		
		if( *fmt == '.' ) 
		{
			++fmt;	
			
			if( isdigit(*fmt) )
			{
				precision = skip_atoi(&fmt);
			}
			else 
			{
				if (*fmt == '*') 
				{
					++fmt;
					/* it's the next argument */
					precision = va_arg(args, int);
				}
				
				if( precision < 0 )
					precision = 0;
			}
		}
		

		/* get the conversion qualifier */
		qualifier = -1;
		
		if( *fmt == 'h' || *fmt == 'l' || *fmt == 'L' || *fmt == 'f' )
		{
			qualifier = *fmt;
			
			++fmt;
		}

		/* default base */
		base = 10;

		switch( *fmt ) 
		{
			case 'c':
#ifdef __DEBUG__

				if( !(flags & LEFT) )
					while( --field_width > 0 )
					{
						if( charCount >= sprintfMaxCount )
						{
							error_trace("PRINT", "vsprintf", charCount);	// segnala la cappella sistina
							*str = '\0';
							return(charCount);
						}
						*str++ = ' ';
						charCount++;
					}
					
						
				*str++ = (unsigned char) va_arg(args, int);
				charCount++;
				
				while( --field_width > 0 )
				{
					if( charCount >= sprintfMaxCount )
					{
						error_trace("PRINT", "vsprintf", charCount);	// segnala la cappella sistina
						*str = '\0';
						return(charCount);
					}
					*str++ = ' ';
					charCount++;
				}
				
#else
				if( !(flags & LEFT) )
					while( --field_width > 0 )
						*str++ = ' ';
						
				*str++ = (unsigned char) va_arg(args, int);
				
				while( --field_width > 0 )
						*str++ = ' ';
						
#endif
				continue;
	
			case 's':
				s = va_arg(args, char *);

				if( !s )
					s = "<NULL>";
	
				len = strlen(s);
	                        
				if( len > precision && precision >= 0 )
	            	len = precision;
	
#ifdef __DEBUG__
				if( !(flags & LEFT) )
					while( len < field_width-- )
					{
						if( charCount >= sprintfMaxCount )
						{
							error_trace("PRINT", "vsprintf", charCount);	// segnala la cappella sistina
							*str = '\0';
							return(charCount);
						}

						*str++ = ' ';
						charCount++;
					}
					
						
				for(i = 0; i < len; ++i)
				{
					if( charCount >= sprintfMaxCount )
					{
						error_trace("PRINT", "vsprintf", charCount);	// segnala la cappella sistina
						*str = '\0';
						return(charCount);
					}
					*str++ = *s++;
					charCount++;
				}
					
				while(len < field_width--)
				{
					if( charCount >= sprintfMaxCount )
					{
						error_trace("PRINT", "vsprintf", charCount);	// segnala la cappella sistina
						*str = '\0';
						return(charCount);
					}

					*str++ = ' ';
					charCount++;
				}		
#else
				if( !(flags & LEFT) )
					while( len < field_width-- )
						*str++ = ' ';
						
				for(i = 0; i < len; ++i)
					*str++ = *s++;
					
				while(len < field_width--)
					*str++ = ' ';
#endif
				continue;
	
			case 'p':
				if (field_width == -1) 
				{
					field_width = 2 * sizeof(void *);
					flags |= ZEROPAD;
				}
				
				str = number(str,
					(unsigned long) va_arg(args, void *), 16,
					field_width, precision, flags);
				continue;
	
	
			case 'n':
				if(qualifier == 'l') 
				{
					long * ip = va_arg(args, long *);
					*ip = (str - buf);
				} 
				else 
				{
					int * ip = va_arg(args, int *);
					*ip = (str - buf);
				}
				continue;
	
			/* integer number formats - set up the flags and "break" */
			case 'o':
				base = 8;
				break;
	
			case 'X':
				flags |= LARGE;
			case 'x':
				base = 16;
				flags &= ~SIGN;
				break;
	
			case 'd':
			case 'i':
				flags |= SIGN;
			case 'u':
				break;
	
                    /* For GCC test gcc.c-torture/execute/ieee/920810-1.c */
            case 'g':
              *str++ = '0';
              continue;
	                  
			default:
				if( *fmt != '%' )
					*str++ = '%';
					
				if( *fmt )
					*str++ = *fmt;
				else
					--fmt;
					
				continue;
		}

		if( qualifier == 'f' )
		{
			flnum = va_arg(args, double);

			if( precision )
			{
				decimal_div = decimalAdjTab[precision];
			
				int_value = (int) (flnum / decimal_div);
			
				frac_value = ((long) flnum) % decimal_div;

				if( frac_value < 0 )
					frac_value = ~frac_value;

				str = number(str, int_value, 10, field_width-precision-1, 0, flags);
				
				*str++ = '.';
				
				str = number(str, frac_value, 10, field_width, 0, flags);
			}
			else
			{
				num = (long) flnum;
				
				str = number(str, num, base, field_width, precision, flags);
			}
		}
		else
		{
			if( qualifier == 'l' )
			{
				num = va_arg(args, unsigned long);
			}
			else 
			{
				if( qualifier == 'h' )
				{
					if( flags & SIGN )
						short_num = va_arg(args, int);
					else
						short_num = va_arg(args, unsigned int);
						
					num = short_num;
				}
				else 
				{
					if( flags & SIGN )
						num = va_arg(args, int);
					else
						num = va_arg(args, unsigned int);
				}
			}

			str = number(str, num, base, field_width, precision, flags);
		}					
	}
	
	*str = '\0';
	
	return( str - buf );
}


/**
strtol lighter version
*/
unsigned long _strtoul(const char *cp, char **endp, unsigned int base)
{
unsigned long result = 0,value;

	if( !base )  
	{
		base = 10;
	
		if( *cp == '0' )
		{
			base = 8;
			cp++;
			
			if( (*cp == 'x') && isxdigit(cp[1]) ) 
			{
				cp++;
				base = 16;
			}
		}
	}


	while( isxdigit(*cp) ) 
	{
		if( isdigit(*cp) )
			value = *cp - '0';
		else 
			value = toupper(*cp) - 'A' + 10;
		
		if( value < base )
			break;
	    
		result = result * base + value;
		cp++;
	}

	if( endp )
		*endp = (char *)cp;
		
	return( result );
}


static int skip_atoi(const char **s)
{
int i = 0;

	while( isdigit(**s) )
	{
		i = (i * 10) + *s[0] - '0';
		(*s)++;
	}
	
	return( i );
}

static char *number(char * str, long num, int base, int size, int precision ,int type)
{
char c;
char sign;
char tmp[33];
register char *pn;
register unsigned long absnum;
register unsigned long quot;
register unsigned long rem;

const char *digits = lowerDigits;
int i;

	if( type & LARGE )
		digits = upperDigits;

	if( type & LEFT )
		type &= ~ZEROPAD;

	if( base < 2 || base > 36 )
		return( 0 );

	c = (type & ZEROPAD) ? '0' : ' ';

	sign = 0;
	absnum = num;				// parte considerandolo positivo...

	if( type & SIGN ) 
	{
		if( num < 0 ) 
		{
			sign = '-';
			absnum = -num;
			size--;
		} 
		else 
			if( type & PLUS ) 
			{
				sign = '+';
				size--;
			} 
			else 
				if( type & SPACE )
				{
					sign = ' ';
					size--;
				}
	}
	
	if( type & SPECIAL ) 
	{
		if( base == 16 ) 
			size -= 2;
		else 
			if( base == 8 )
				size--;
	}
	
	i = 0;
	pn = &tmp[0];

	if( base == 16 )						// semplifichiamo per la base 16...
	{
		if( absnum == 0 )
		{
			*pn++ = '0';
			i++;
		}
		else 
			while( absnum != 0 )
			{
				quot = absnum >> 4;				// divide e ...piglia il resto
				rem = absnum & 0x0000000F;			// piglia il resto
				
				*pn++ = digits[rem];			// ricava la cifretta

				if( i++ > (int) sizeof(tmp) )			// security...
					break;

				absnum = quot;						// il resto è nuovamente divisibile
			}
	}
	else
	{
		if( absnum == 0 )
		{
			*pn++ = '0';
			i++;
		}
		else 
			while( absnum != 0 )
			{
				quot = absnum / base;				// divide e piglia il resto
				rem = absnum % base;
				
				*pn++ = digits[rem];			// ricava la cifretta

				if( i++ > (int) sizeof(tmp) )			// security...
					break;
				
				absnum = quot;						// il resto è nuovamente divisibile
			}
	}
	
	if( i > precision )
		precision = i;
		
	size -= precision;

	pn = str;				// passa all'ala..
	

#ifdef __DEBUG__

	if( !(type & (ZEROPAD+LEFT) ) )
		while( size-- > 0 )
		{
			if( charCount >= sprintfMaxCount )
			{
				error_trace("PRINT", "number", charCount);	// segnala la cappella sistina
				*pn = '\0';
				return(pn);
			}
			charCount++;

			*pn++ = ' ';
		}
		
			
	if( sign )
	{
		*pn++ = sign;
		charCount++;
	}
	
		
	if( type & SPECIAL )
		if( base == 8 )
		{
			*pn++ = '0';
			charCount++;
		}
		else 
			if( base == 16 ) 
			{
				*pn++ = '0';
				*pn++ = digits[33];
				charCount += 2;
			}

	if( !(type & LEFT) )
		while( size-- > 0 )
		{
			if( charCount >= sprintfMaxCount )
			{
				error_trace("PRINT", "number", charCount);	// segnala la cappella sistina
				*pn = '\0';
				return(str);
			}
			charCount++;

			*pn++ = c;
		}
			
	while( i < precision-- )
	{
		if( charCount >= sprintfMaxCount )
		{
			error_trace("PRINT", "number", charCount);	// segnala la cappella sistina
			*pn = '\0';
			return(str);
		}
		charCount++;

		*pn++ = '0';
	}
	
		
	while( i-- > 0 )
	{
		if( charCount >= sprintfMaxCount )
		{
			error_trace("PRINT", "number", charCount);	// segnala la cappella sistina
			*pn = '\0';
			return(str);
		}
		charCount++;

		*pn++ = tmp[i];
	}
	
		
	while( size-- > 0 )
	{
		if( charCount >= sprintfMaxCount )
		{
			error_trace("PRINT", "number", charCount);	// segnala la cappella sistina
			*pn = '\0';
			return(str);
		}
		charCount++;

		*pn++ = ' ';
	}
	
#else

	if( !(type & (ZEROPAD+LEFT) ) )
		while( size-- > 0 )
			*pn++ = ' ';
		
	if( sign )
		*pn++ = sign;
		
	if( type & SPECIAL )
	{
		if( base == 8 )
		{
			*pn++ = '0';
		}
		else 
		{
			if( base == 16 ) 
			{
				*pn++ = '0';
				*pn++ = digits[33];
			}
		}
	}

	if( !(type & LEFT) )
		while( size-- > 0 )
			*pn++ = c;
			
	while( i < precision-- )
		*pn++ = '0';
		
	while( i-- > 0 )
		*pn++ = tmp[i];
		
	while( size-- > 0 )
		*pn++ = ' ';

#endif		
	return( pn );
}

void abort(void)
{
	for(;;);
}



// extern "C" closing brace
#ifdef __cplusplus
}
#endif




//@@-----------------------------------------------------------------------@@//
//@@
//@@  $Source: /root/ConfTx/PRINT.CPP,v $
//@@  Note di revisione:
//@@
//@@  $Revision: 1.4 $
//@@  $Date: 2007/07/24 07:30:53 $
//@@  			Viene aggiunto un controllo sulla lunghezza max. di
//@@			"formattazione" alla funzione "sprintf" tramite una variabile
//@@			globale in modo da poter tracciare errori di "stampa" e,
//@@			sopratutto evitare il piantaggio del programma.
//@@
//@@  $Revision: 1.4 $
//@@  $Date: 2007/07/24 07:30:53 $
//@@  			Viene aggiunta una veriabile "unsigned long absnum" nella 
//@@			funzione "number" per ovviare all'inconveniente che
//@@			si verificava con numeri negativi: la divisione (o lo shift)
//@@			del numero negativo faceva si che non diventasse mai 0 restando
//@@			quindi nel loop e sforando il buffer predisposto.
//@@
//@@  $Revision: 1.4 $
//@@  $Date: 2007/07/24 07:30:53 $
//@@  			Viene tolto il flasg SIGN nel caso di formattazione con base 16
//@@			nella funzione "vsprintf"
//@@  $Log: PRINT.CPP,v $
//@@  Revision 1.4  2007/07/24 07:30:53  enrico
//@@  blanks only
//@@
//@@  Revision 1.3  2006/10/03 13:08:09  giovanni
//@@  *** empty log message ***
//@@
//@@
//@@  $Author: enrico $
//@@  $RCSfile: PRINT.CPP,v $
//@@  $Revision: 1.4 $
//@@  $State: Exp $
//@@  $Date: 2007/07/24 07:30:53 $
//@@  $Name:  $
//@@-----------------------------------------------------------------------@@//
//@@-----------------------------------------------------------------------@@//
//@@                                                                       @@//
//@@                            C O M E T A                                @@//
//@@                                                                       @@//
//@@-----------------------------------------------------------------------@@//
