/**
@file		util.cpp
@brief		Utilities functions.

@author		
@date		18/01/2011
@version	01.0
*/

#include "util.h"


#define EOS '\0'

double my_pow10(short value)
{

static const double	power10Tab[] = {		1.0,				// 10e0
											10.0,				// 10e1
											100.0,				// 10e2
											1000.0,				// 10e3
											10000.0,			// 10e4
											100000.0,			// 10e5
											1000000.0,			// 10e6
											10000000.0,			// 10e7
											100000000.0,		// 10e8
											1000000000.0,		// 10e9
											10000000000.0,		// 10e10
											100000000000.0,		// 10e11
											1000000000000.0	};	// 10e12

	if(	value <= 12	)
		return(	power10Tab[value] );
		
	return(	0.0	);
}

void strset(char *s, int filler)
{
	while( *s )
	{
		*s++ = filler;
	}
}


void IntToHex(int num, char* hex, int minNumChar)
{
int i = 0;
int r;
	
char c[20];
	
	while (num > 0)
	{
		r = num % 16;
		num = num /16;
		
		if (r > 9)
		{
			r -= 10;
			hex[i] = r + 65; 
		}else
		{
			hex[i] = r + 48; 
		}
		i++;
	}
	while(i < minNumChar)
	{
		hex[i] = '0';
		i++;
	}
	hex[i] = '\0';
	strcpy(c, hex);
	for (i = 0; i < (int) strlen(hex); i++)
	{
		hex[i] = c[strlen(hex) - 1 - i];
	}
}

int HexToInt(int& num, char* hex, int len)
{
	//int len = strlen(hex);
	int i,e,k;
	unsigned char r;
	int mul;
	num = 0;
	i = len - 1;
	e = 0;
	while (i >=0)
	{
		r = (unsigned char)hex[i];
		if ((r >=  '0') && (r <= '9'))
		{
			r-= '0';
		}else if ((r >= 'A') && (r <= 'F'))
		{
			r -= 'A';
			r += 10;
		}else if ((r >= 'a') && (r <= 'f'))
		{
			r -= 'a'; 
			r += 10;
		}else 
		{
			return -1;
		}
		if (e > 0)
		{
			mul = 1;
			for (k = 0; k < e; k++)
				mul = mul * 16;
			num += mul*r;
		}
		else
			num += r;
		i--;
		e++;
	}
	return 0;
}


int windowFilter(short* Val, short *ValToFilter, int numChan, int CurrentIndex, int FilterDim)
{
	int k, n;

	ValToFilter[CurrentIndex] = Val[numChan]; 
	
	n = 0;
	for (k = 0; k < CurrentIndex; k++)
	{
		n += ValToFilter[k];
	}
	for (k = CurrentIndex; k < FilterDim; k++)
	{
		n += ValToFilter[k];
	}
	Val[numChan] = n / FilterDim;

	return 0;
}


char *fmtReal(long realValue, int numChars,	int	decPoint)
{
long int_value;
short frac_value;
word decimal_div;
static char	s[50];

static const word decimalAdjTab[] =	{ 1, 10, 100, 1000 };

	if(	decPoint )
	{
		decimal_div	= decimalAdjTab[decPoint];
		
		int_value =	realValue /	decimal_div;
		
		frac_value = realValue % decimal_div;
	
		if(	frac_value < 0 )
			frac_value = ~frac_value;
		
		if(	sprintf(s, "%*ld.%0*d",	numChars - (decPoint + 1), int_value, decPoint,	frac_value)	> numChars )
		{
			strset(s, '*');
			s[numChars]	= 0;   // set EOS
		}
	}
	else
	{
		if(	sprintf(s, "%*ld", numChars, realValue)	> numChars )
		{
			strset(s, '*');
			s[numChars]	= 0;   // set EOS
		}
	}
	
	return(	s );
}


// Nuovo gestore eccezioni calcoli in virgola mobile
int matherr(struct exception *e)
{
    return( 1 );
}

/*
// Guarda se esiste il file richiesto
int  exist(char *name)
{
    return( access(name, 0) == 0 );     // se c'e` va bene 
}

// Cancella il file richiesto
int  erase(char *name)
{
    if( unlink(name) == -1 )     // se non ci riesce bene 
    {
		chmod(name, S_IFREG | S_IREAD | S_IWRITE);
		if( unlink(name) == -1 )
            return(-1);
    }
    return(0);
}

// Guarda se il file richiesto e` leggibile e scrivibile
int  may_r_w(char *name)
{
	return( access(name, 6) == 0 );      // se c'e` va bene 
}

// Fa una copia del file source su dest
int  fcopy(char *fdst, char *fsrc)
{
int  hdst, hsrc;
char *buf;
int  c;
long mem, len;
char s[512];

    if( !exist(fsrc) )      // non esiste 
	{
		OutErr(NOT_EXIST, fsrc);
		return(1);
	}

	if( !may_r_w(fsrc) )    // se non posso leggerlo e scriverlo 
	{
		OutErr(NOT_RW, fsrc);
		return(1);     // hanno toppato 
	}

	hsrc = open(fsrc, O_RDONLY | O_BINARY, S_IFREG | S_IREAD);  // prova ad aprire il file 

	if( hsrc == -1 )
	{
		OutErr(OPEN_ERR, fsrc);
		return(1);                  // errore in apertura del file 
	}

    hdst = open(fdst, O_RDWR | O_CREAT | O_BINARY, S_IFREG | S_IREAD | S_IWRITE);  // prova ad aprire il file

	if( hdst == -1 )
    {
		OutErr(OPEN_ERR, fsrc);
        return(1);                  // errore in apertura del file 
    }

	mem = coreleft();
	len = mem / 2;

	if( len > 512 )	// se abbiamo + di 1k 
	{
		if( len > 60000L )
			len = 60000L;

		buf = (char *) malloc( (size_t) len);
		if( buf == NULL )
		{
			buf = s;		// bufferino minimo 
			len = 512;
		}
	}
	else
	{
		buf = s;			// bufferino minimo 
		len = 512;
	}

    for(;;)
	{
		c = _read(hsrc, (void *) buf, len);

		if( c == 0 || c == -1 )
			break;

		c = _write(hdst, (void *) buf, c);

		if( c == -1 )
			break;
    }

	if( buf != s )		// se non e` la stringa 
		free(buf);		// libera la memoria (che diamine) 

	close(hdst);
	close(hsrc);
	return(0);          // tutto ok 
}

// Funzione base per l'output degli errori
void OutErr(int  err, char *fname)
{

	switch( err )
    {
        case    NOERR:
			break;

        case    NOT_EXIST:
			printf("\nNon trovo il file: %s", fname);
            break;

        case    NOT_RW:
			printf("\nErrore in scrittura del file: %s", fname);
			break;

		case    OPEN_ERR:
			printf("\nErrore in apertura del file: %s", fname);
			break;

		case    NO_DBF_FILE:
			printf("\nErrore: il file %s non e` un database", fname);
			break;

		case    NO_MEM:
			printf("\nErrore: Fine memoria disponibile");
			break;

		case    READ_ERR:
			printf("\nErrore durante la lettura del file: %s", fname);
	}
}

*/


// Toglie gli spazi alla sinistra della stringa
void ltrim(char *s)
{
char *p;

    for(p=s; *p == ' '; p++);
    while(*p)
        *s++ = *p++;

    *s = 0;         // finestringa
}

// Toglie gli spazi alla destra della stringa
void rtrim(char *s)
{
    for(; *s; s++);
    for(--s; *s == ' '; s--);

    *(s+1) = 0;     // finestringa
}

// Toglie tutti gli spazi vuoti
byte *skipWhites(byte *buf)
{
byte *sk, *bg;

    bg = sk = buf;

	while( *buf != EOS )           // copia tutto meno gli spazi vuoti
    {
        if( *buf != ' ' )
            *sk++ = *buf++;
        else
            buf++;
	}
	*sk = EOS;
	return(bg);
}

// Compatta la stringa tenedo i blanks in fondo
byte *packWhites(byte *buf)
{
byte *sk, *bg;

    bg = sk = buf;

	while( *buf != EOS )           // copia tutto meno gli spazi vuoti
    {
        if( *buf != ' ' )
            *sk++ = *buf++;
        else
            buf++;
	}

    while( sk != buf )              // rifinisce con dei blanks
        *sk++ = ' ';

	*sk = EOS;
	return(bg);
}

// corregge il formato della data
void cdate(char *s)
{
char date[10];

    date[0] = s[6];     // giorno
    date[1] = s[7];
    date[2] = '-';
    date[3] = s[4];     // mese
    date[4] = s[5];
    date[5] = '-';
    date[6] = s[2];     // anno
    date[7] = s[3];
    date[8] = '\0';

    strcpy(s, date);
}

// corregge il formato della data con la scritta del mese
void cdaten(char *s)
{
char date[20];
int mese;
static const char *mesi[] = { "Gen", "Feb", "Mar", "Apr", "Mag", "Giu",
						"Lug", "Ago", "Set", "Ott", "Nov", "Dic", };

	date[0] = s[3];
	date[1] = s[4];
	date[2] = 0;

	mese = atoi(date);

	if( mese )
		mese--;

	date[0] = s[0];     // giorno
	date[1] = s[1];
	date[2] = ' ';
	date[3] = mesi[mese][0];     // mese
	date[4] = mesi[mese][1];     // mese
	date[5] = mesi[mese][2];     // mese
	date[6] = ' ';
	date[7] = s[6];     // anno
	date[8] = s[7];
	date[9] = '\0';

    strcpy(s, date);
}

// Somma due stringhe e aggiunge "val" per arrivare a "len" caratteri
int  sumstr(char *dst, char *src, int  len, int  val)
{
int  i;

    for(i=len; i; i--)
        if( *src )
            *dst++ = *src++;
        else
            *dst++ = val;

    return(len);
}

// Somma e riempie l'inizio
int  fsumstr(char *dst, char *src, int  maxlen, int  minlen, int  val)
{
int  i;

	for(i=maxlen; i > minlen; i--, dst++ )
		*dst = val;

	for(; i; i--)
		if( *src )
			*dst++ = *src++;
		else
			*dst++ = val;

	return(maxlen);
}

// Somma due stringhe
int  addstr(char *dest, const char *source)
{
int     sum;

    for(sum=0; *source; sum++)
            *dest++ = *source++;

    return(sum);    // ritorna il numero di caratteri copiati
}

// Cambia l'estensione al file richiesto
void chgext(char *fname, char *ext)
{
int  i;

                    // cerca il finestringa o il punto
    for(i=0; fname[i] != 0 && fname[i] != '.'; i++);

    if( fname[i] == '.' )   // Se ha gia` un'estensione
        i++;                // avanti pure

    strcpy(&fname[i], ext); // aggiunge l'estensione
}

// Stampa la percentuale su di una stringa
int  pperc(char *s, double base, double scost)
{

	if( base )
		sprintf(s, "%5.2lf", 100.0 - ((scost * 100.0) / base) );
	else
		strcpy(s, "  0.0");

	s[5] = 0;
	return(5);
}
