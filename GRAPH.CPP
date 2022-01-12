//---------------------------------------------------------------------------//
//                                                                           //
//                             C O M E T A                                   //
//                                                                           //
//---------------------------------------------------------------------------//
//                                                                             
//  Progetto:                GENLIB
//  Nome File:               GRAPH.CPP
//  Descrizione:             Routines gestione display in modalità
//							 grafica
//  Note:             		 L'interfaccia reale col controller del
//							 display viene fatta esternamente in un
//							 modulo dedicato (sorta di driver video)
//  Supporto Fisico:         Hw Generico dipendenza dal "dpydrv" 
//  Versione:                $Revision: 1.2 $
//  Data Versione:           $Date: 2006/01/16 13:00:47 $
//  Descrizione Versione:    Stesura iniziale
//  
//---------------------------------------------------------------------------//
#include "graph.h"

//-------------------------------------------------------
// Variabili globali utili a tutti i moduli
//-------------------------------------------------------

//-------------------------------------------------------
// Funzioni statiche utili solo a questo modulo
//-------------------------------------------------------

//-------------------------------------------------------
// Costanti statiche utili solo a questo modulo
//-------------------------------------------------------

#define GR_MAX_POLYGON_POINTS   (8192)
#define GR_MAX_ELLIPSE_POINTS   (1024 + 5)

#define GR_MAX_ANGLE_VALUE      (3600)

#define GR_ARC_STYLE_OPEN       0
#define GR_ARC_STYLE_CLOSE1     1
#define GR_ARC_STYLE_CLOSE2     2


#define YR(r)  ((r)*__gr_Xasp/__gr_Yasp)
#define XR(r)  (r)

#define NULL_IS_EMPTY(s) ((s) ? : "")
#define user_len 32

#define xs _grx_arc_xs
#define ys _grx_arc_ys
#define xc _grx_arc_xc
#define yc _grx_arc_yc
#define xe _grx_arc_xe
#define ye _grx_arc_ye

#define MAXPTS  (GR_MAX_ELLIPSE_POINTS & (~15))
#define SEGLEN  5		/* preferred lenght of line segments on arc */
#define TRIGMGN 16384		/* scale factor for sine table */
#define PERIOD  1024		/* number of points in sine table */
#define PHALF	(PERIOD / 2)
#define PQUART  (PERIOD / 4)

static const int sintab[PQUART + 1] __attribute__ ((section (".message"))) = {
		0,   101,   201,   302,	  402,	 503,	603,   704,
      804,   904,  1005,  1105,  1205,  1306,  1406,  1506,
     1606,  1706,  1806,  1906,  2006,  2105,  2205,  2305,
     2404,  2503,  2603,  2702,  2801,  2900,  2999,  3098,
     3196,  3295,  3393,  3492,  3590,  3688,  3786,  3883,
     3981,  4078,  4176,  4273,  4370,  4467,  4563,  4660,
     4756,  4852,  4948,  5044,  5139,  5235,  5330,  5425,
     5520,  5614,  5708,  5803,  5897,  5990,  6084,  6177,
     6270,  6363,  6455,  6547,  6639,  6731,  6823,  6914,
     7005,  7096,  7186,  7276,  7366,  7456,  7545,  7635,
     7723,  7812,  7900,  7988,  8076,  8163,  8250,  8337,
     8423,  8509,  8595,  8680,  8765,  8850,  8935,  9019,
     9102,  9186,  9269,  9352,  9434,  9516,  9598,  9679,
     9760,  9841,  9921, 10001, 10080, 10159, 10238, 10316,
    10394, 10471, 10549, 10625, 10702, 10778, 10853, 10928,
    11003, 11077, 11151, 11224, 11297, 11370, 11442, 11514,
    11585, 11656, 11727, 11797, 11866, 11935, 12004, 12072,
    12140, 12207, 12274, 12340, 12406, 12472, 12537, 12601,
    12665, 12729, 12792, 12854, 12916, 12978, 13039, 13100,
    13160, 13219, 13279, 13337, 13395, 13453, 13510, 13567,
    13623, 13678, 13733, 13788, 13842, 13896, 13949, 14001,
    14053, 14104, 14155, 14206, 14256, 14305, 14354, 14402,
    14449, 14497, 14543, 14589, 14635, 14680, 14724, 14768,
    14811, 14854, 14896, 14937, 14978, 15019, 15059, 15098,
    15137, 15175, 15213, 15250, 15286, 15322, 15357, 15392,
    15426, 15460, 15493, 15525, 15557, 15588, 15619, 15649,
    15679, 15707, 15736, 15763, 15791, 15817, 15843, 15868,
    15893, 15917, 15941, 15964, 15986, 16008, 16029, 16049,
    16069, 16088, 16107, 16125, 16143, 16160, 16176, 16192,
    16207, 16221, 16235, 16248, 16261, 16273, 16284, 16295,
    16305, 16315, 16324, 16332, 16340, 16347, 16353, 16359,
    16364, 16369, 16373, 16376, 16379, 16381, 16383, 16384,
    16384
};


//-------------------------------------------------------
// Variabili statiche utili solo a questo modulo
//-------------------------------------------------------

static byte fillPatterns[][8] __attribute__ ((section (".message"))) = {                     /* BGI fill patterns        */
  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   /* EMPTY_FILL        */
  { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},   /* SOLID_FILL        */
  { 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00},   /* LINE_FILL         */
  { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80},   /* LTSLASH_FILL      */
  { 0xE0, 0xC1, 0x83, 0x07, 0x0E, 0x1C, 0x38, 0x70},   /* SLASH_FILL        */
  { 0xF0, 0x78, 0x3C, 0x1E, 0x0F, 0x87, 0xC3, 0xE1},   /* BKSLASH_FILL      */
  { 0xA5, 0xD2, 0x69, 0xB4, 0x5A, 0x2D, 0x96, 0x4B},   /* LTBKSLASH_FILL    */
  { 0xFF, 0x88, 0x88, 0x88, 0xFF, 0x88, 0x88, 0x88},   /* HATCH_FILL        */
  { 0x81, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42, 0x81},   /* XHATCH_FILL       */
  { 0xCC, 0x33, 0xCC, 0x33, 0xCC, 0x33, 0xCC, 0x33},   /* INTERLEAVE_FILL   */
  { 0x80, 0x00, 0x08, 0x00, 0x80, 0x00, 0x08, 0x00},   /* WIDE_DOT_FILL     */
  { 0x88, 0x00, 0x22, 0x00, 0x88, 0x00, 0x22, 0x00},   /* CLOSE_DOT_FILL    */
  { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}    /* USER_FILL         */
};

static byte *fill_pattern_data;

struct 	palettetype defaultpalette; 
struct  linesettingstype linesettings;
struct  fillsettingstype fillsettings;
struct  arccoordstype  ac;

static char line_user[user_len];
static short line_pattlen;
static const char *line_dashpat;

//static int last_xs,last_ys;
//static int last_xe,last_ye;
//static int last_xc,last_yc;

							// funzioni di inizializz. e controllo del disegno


/* Piazza il puntatore al pennello di disegno (cursore) dove richiesto
*/
void moveto(int __x, int __y)
{
	drv_setGraphCursPos(__x, __y);
}


/* Sposta in modo relativo (incrementale) il puntatore al pennello di disegno 
   (cursore) dove richiesto
*/
void moverel(int __dx, int __dy)
{
	drv_setGraphCursPos(getx() + __dx, gety() + __dy);
}


/* Ritorna la coordinata x del puntatore al pennello di disegno 
*/
int getx(void)
{
	return( graphInfo.curs.x );
}

int gety(void)
{
	return( graphInfo.curs.y );
}

int getmaxx(void)
{
	return( graphInfo.screen.w - 1 );
}

int getmaxy(void)
{
	return( graphInfo.screen.h - 1 );
}

void setcolor(int __color)
{
	graphInfo.color.fgcolor = __color;
}

int getcolor(void)
{
	return( graphInfo.color.fgcolor );
}

int getmaxcolor(void)
{
	return( graphInfo.color.maxcolor );
}

void setbkcolor(int __color)
{
	graphInfo.color.bkcolor = __color;
}

int getbkcolor(void)
{
	return( graphInfo.color.bkcolor );
}


void setwritemode(int __mode)
{

}


void setlinestyle(int __linestyle, unsigned __upattern, int __thickness)
{
int i, j;

	switch( __linestyle )
	{
		case SOLID_LINE  : 
			line_pattlen = 0;
	       	line_dashpat = NULL;
	       	break;
		
		case DOTTED_LINE : 
			line_pattlen = 3;
	       	line_dashpat = "\0\2\2";
	       	break;
		
		case CENTER_LINE : 
			line_pattlen = 5;
	       	line_dashpat = "\0\3\4\3\6";
	       	break;
		
		case DASHED_LINE : 
			line_pattlen = 5;
	       	line_dashpat = "\0\3\5\3\5";
	       	break;
	       	
		case USERBIT_LINE: 

			if( __upattern == 0xFFFF ) 
			{
				line_pattlen = 0;
				line_dashpat = NULL;
				break;
			}

			j = 0;
			line_user[0] = 0;

			for (i=0; i < 16; ++i) 
			{
				if( (__upattern & 1) == 0) 
				{
					if ( (j&1) == 0) 
					{
						++j;
						line_user[j] = 0;
					}
					
					++line_user[j];
				} 
				else 
				{
					if ( (j&1) != 0) 
					{
						++j;
						line_user[j] = 0;
					}
					
					++line_user[j];
				}
				
				__upattern >>= 1;
			}
		       
			line_pattlen = j+1;

			if( line_pattlen==2 && line_user[0] == 0 )
				line_user[1] = 0; /* non-drawing */
				
			line_dashpat = line_user;
			break;
			
		default          : 
			//ERR = grError;
			return;
	}

	linesettings.linestyle = __linestyle;
	linesettings.upattern = __upattern;
	linesettings.thickness = __thickness;
}

void getlinesettings(struct linesettingstype *__lineinfo)
{
  __lineinfo->linestyle = linesettings.linestyle;
  __lineinfo->upattern  = linesettings.upattern;
  __lineinfo->thickness = linesettings.thickness;

}


void setfillstyle(int __pattern, int __color)
{

	if( __pattern < 0 || __pattern > USER_FILL ) 
		return;
	
	fillsettings.pattern = __pattern;
	fillsettings.color = __color;
	
	fill_pattern_data = (unsigned char *)&fillPatterns[__pattern];
}


void getfillsettings(struct fillsettingstype *__fillinfo)
{
}


void setfillpattern(const byte *__upattern, int __color)
{

	fillsettings.pattern = USER_FILL;
	fillsettings.color = __color;
	
	memcpy(fillPatterns[USER_FILL], __upattern, 8);
	
	fill_pattern_data = (byte *)&fillPatterns[USER_FILL];
}


void getfillpattern(byte *__pattern)
{
	*__pattern = fillPatterns[USER_FILL][0];
}


void setaspectratio(int __xasp, int __yasp)
{
}

void getarccoords(struct arccoordstype *__arccoords)
{
	memcpy(__arccoords, &ac, sizeof(ac));
}

void getaspectratio(int *__xasp, int *__yasp)
{
}


int getpalettesize( void )
{
	return(0);
}

void setpalette(int __colornum, int __color)
{
}

void getpalette(struct palettetype *__palette)
{
}

void setrgbpalette(int __colornum, int __red, int __green, int __blue)
{
}


void setallpalette(const struct palettetype *__palette)
{
}

struct 	palettetype *getdefaultpalette( void )
{
	return(&defaultpalette);
}


void setactivepage(int __page)
{
}

void setvisualpage(int __page)
{
}

void setviewport(int __left, int __top, int __right, int __bottom, int __clip)
{
}

void getviewsettings(struct viewporttype *__viewport)
{
	__viewport->left   = graphInfo.vport.vpl;
	__viewport->right  = graphInfo.vport.vpt;
	__viewport->top    = graphInfo.vport.vpr;
	__viewport->bottom = graphInfo.vport.vpb;
	__viewport->clip   = graphInfo.vport.clip;
}


void settextstyle(int __font, int __direction, int __charsize)
{
}

void settextjustify(int __horiz, int __vert)
{
}

void setusercharsize(int __multx, int __divx, int __multy, int __divy)
{
}

								// funzioni di disegno basilari
void cleardevice(void)
{
	drv_setGraphCursPos(0, 0);

	drv_setCursDir(CURSDIR_RIGHT);
	drv_fillGraphBlock( (graphInfo.screen.w * graphInfo.screen.h), getbkcolor());  // riempie tutti i pixel disponibili
													          // col colore di background
	drv_setGraphCursPos(0, 0);
}

void clearviewport(void)
{
	setfillstyle(SOLID_FILL, getbkcolor());
	bar(graphInfo.vport.vpl, graphInfo.vport.vpt, graphInfo.vport.vpr, graphInfo.vport.vpb);
	moveto(0, 0);
}



int getpixel(int __x, int __y)
{
	return( drv_getpixel(__x, __y) );
}

void putpixel(int __x, int __y, int __color)
{
	drv_putpixel(__x, __y, __color);
}


void line(int __x1, int __y1, int __x2, int __y2)
{
int cnt, err, xoff, yoff, color;
int x, y, dx, dy;

	color = getcolor();

	if( __y1 == __y2  )
	{
		if( __x1 == __x2 )
			drv_putpixel(__x1, __y1, color);		/* Singolo punto */
		else
			drv_hline(__x1, __y1, __x2, color);		/* Orizzontale o Verticale */
			
		return;
	}

	if( __x1 == __x2 )		
	{
		drv_vline(__x1, __y1, __y2, color);			/* Orizzontale o Verticale */
		return;
	}

	if( __x2 > __x1 )
	{
		dx = __x2 - __x1;
		x = __x1;
		xoff = 1;
	}
	else	
	{
		dx = __x1 - __x2;
		x = __x1;
		xoff = -1;
	}

	if( __y2 > __y1 )
	{
		dy = __y2 - __y1;
		y = __y1;

		yoff = 1;
	}
	else
	{
		dy = __y1 - __y2;
		y = __y1;

	    yoff = (-1);
	}
	
	if( dx > dy ) 
	{
	    err = (cnt = dx) >> 1;
	    
	    do{
			drv_putpixel(x, y, color);
			
			if((err -= dy) < 0) 
			{
				err += dx;
				y += yoff;
			}
			
		x += xoff;

	    }while(--cnt >= 0);
	}
	else 
	{
	    err = (cnt = dy) >> 1;

	    do {

			drv_putpixel(x, y, color);

			if( (err -= dx) < 0 ) 
			{
				err += dy;
				x += xoff;
			}

			y += yoff;
			
	    }while(--cnt >= 0);
	}
}					
					

void linerel(int __dx, int __dy)
{
int x, y;


	x = getx();
	y = gety();
	
	line(x, y, x+__dx, y+__dy);
	
	drv_setGraphCursPos(x+__dx, y+__dy);
}


void lineto(int __x, int __y)
{
int x, y;


	x = getx();
	y = gety();
	
	line(x, y, __x, __y);

	drv_setGraphCursPos(__x, __y);
}


void rectangle(int __left, int __top, int __right, int __bottom)
{
int color;

	color = getcolor();

	drv_hline(__left, __top, __right, color);
	drv_vline(__left, __top, __bottom, color);
	drv_vline(__right, __top, __bottom, color);
	drv_hline(__left, __bottom, __right, color);
}
	

void arc(int __x, int __y, int __stangle, int __endangle, int __radius)
{
}

void circle(int __xc, int __yc, int __radius)
{
short tswitch, y, x;
short d;
int color;

	d = __yc - __xc;
	y = __radius;
	tswitch = 3 - 2 * __radius;
	
	color = getcolor();

	for(x=0; x < y; )
	{
											// quattro quadranti quattro
		drv_putpixel(__xc + x, __yc + y, color);
		drv_putpixel(__xc + x, __yc - y, color);
		
		drv_putpixel(__xc - x, __yc + y, color);     
		drv_putpixel(__xc - x, __yc - y, color);
		
		drv_putpixel(__yc + y - d, __yc + x, color); 
		drv_putpixel(__yc + y - d, __yc - x, color);
		
		drv_putpixel(__yc - y - d, __yc + x, color); 
		drv_putpixel(__yc - y - d, __yc - x, color);

	    if( tswitch < 0 )
	    	tswitch += (4 * x + 6);
	    else 
	    {
			tswitch += (4 * (x - y) + 10);
			y--;
	    }
    	x++;
  	}
}


void bar(int __left, int __top, int __right, int __bottom)
{
register int color;

//	color = getcolor();
	color = fillsettings.color;
	
	if( __top < __bottom )
	{
		while( __top <= __bottom )
		{
			drv_hline(__left, __top, __right, color);
	
			__top++;
		}
	}
	else
	{
		while( __top >= __bottom )
		{
			drv_hline(__left, __top, __right, color);
	
			__top--;
		}
	}
	
}


void bar3d(int __left, int __top, int __right, int __bottom, int __depth, int __topflag)
{
	bar(__left, __top, __right, __bottom);
	
	if( __topflag )
		bar(__left + __depth, __top + __depth, __right + __depth, __bottom + __depth);
		
}


/*
   - Take care : Borland C defines polypoints as int * but
   -             assumes struct pointtype *  .
   -             GRX requires int points[][2] !
   - The good news are : Both definitions are compatible !
*/
void drawpoly(int __numpoints, const int *__polypoints, int __close)
{
int x, y, sx, sy, nx, ny;
const int *pp;
int color;

	color = getcolor();

	pp = __polypoints;

	while( __numpoints > 0 )
	{
		x = sx = *(pp++);
		y = sy = *(pp++);
		
    	--__numpoints;
    
		while( __numpoints > 0 )
		{
			nx = *(pp++);
			ny = *(pp++);

			line(x, y, nx, ny);

			x = nx; 
			y = ny;
			
			--__numpoints;
			
			if( x == sx && y == sy )
				break;
		}
		
    	if( __close && (x != sx || y != sy) )
			line(x, y, sx, sy);
	}
}


/*
   - Take care : Borland C defines polypoints as int * but
   -             assumes struct pointtype *  .
   -             GRX requires int points[][2] !
   - The good news are : Both definitions are compatible !
*/
void fillpoly(int __numpoints, const int *__polypoints)
{

	drawpoly(__numpoints, __polypoints, 0);
}

void floodfill(int __x, int __y, int __border)
{
}

void putimage(int __left, int __top, const void *__bitmap, int __option)
{
	drv_putimage(__left, __top, __bitmap, __option);
}


void getimage(int __left, int __top, int __right, int __bottom, void *__bitmap)
{
}

int	imagesize(int __left, int __top, int __right, int __bottom)
{
	return(0);
}

int putpcxpicture(int x1, int y1, const byte *picture)
{
	return( drv_putPcxPicture(x1, y1, picture) );
}

/*
unsigned imagesize(int left, int top, int right, int bottom)
{
unsigned result;
int w, h;

  w = __ABS(right-left)+1;
  h = __ABS(bottom-top)+1;
  result = GrContextSize( w, h);

  if (result == 0)
    return 0;

  return  result + IMAGE_CONTEXT_SIZE;
}

*/

void puticon(int __left, int __top, const void *__bitmap)
{
	drv_puticon(__left, __top, __bitmap, 24, 24);
}

void putsmallicon(int __left, int __top, const void *__bitmap)
{
	drv_puticon(__left, __top, __bitmap, 26, 21);
}


								// funzioni di disegno del testo vettoriale
void outtext(const char *__textstring)
{
register const char *pstr = __textstring;

	while( *pstr )
		drv_putForeChar(*pstr++);
}


void outtextxy(int __x, int __y, const char *__textstring)
{
register const char *pstr = __textstring;
int h;
int c;

	moveto(__x, __y);
	
	h = textheight(__textstring);			// si tiene buona l'altezza

	c = *pstr++;
	
	while( c )
	{
		if( *pstr == '\n' )					// se vogliono tornare a capo		
		{
			__y += h;						// giu' sull'y
		
			moveto(__x, __y);				// ripiazza
		}
		else
		{
			drv_putForeChar(c);				// fuori zona
		}

		c = *pstr++;						// avanti il prossimo
	}
}


int	textheight(const char *__textstring)
{
	return( charset_height(__textstring) );
}


int	textwidth(const char *__textstring)
{
	return( charset_width(__textstring) );
}

void gettextsettings(struct textsettingstype *__texttypeinfo)
{
}

												
								// funzioni di disegno del testo a bitmap
void gputc(int c)
{
	drv_putGraphChar(c);
}


void gputcxy(int __x, int __y, int c)
{
	moveto(__x, __y);
	drv_putGraphChar(c);
}


void gputs(const char *__textstring)
{
	while( *__textstring )
		drv_putGraphChar(*__textstring++);
}


void gputsxy(int __x, int __y, const char *__textstring)
{
	moveto(__x, __y);
	gputs(__textstring);
}


int gprintf(const char *__fmt, ...)
{
va_list args;
int i;
char buf[100];

	va_start(args, __fmt);

	i = vsprintf(buf, __fmt, args);		// prima formatta 

	va_end(args);

	gputs(buf);

	return( i );
}


int gprintfxy(int __x, int __y, const char *__fmt, ...)
{
va_list args;
int i;
char buf[100];

	va_start(args, __fmt);

	i = vsprintf(buf, __fmt, args);		// prima formatta 

	va_end(args);

	gputsxy(__x, __y, buf);

	return( i );
}


const char *grapherrormsg(int __errorcode)
{

	switch( __errorcode )
	{
		case	0: 	return("No Errors");
		case	1:	return("Error 1");
		case	2:	return("Error 2");
		default:	return("Error ??");
	}

}


void ellipse(int __x, int __y, int __stangle, int __endangle, int __xradius, int __yradius)
{
}


void pieslice(int __x, int __y, int __stangle, int __endangle, int __radius)
{
}


void sector(int __x, int __y, int __stangle, int __endangle, int __xradius, int __yradius)
{

}

void fillellipse( int __x, int __y, int __xradius, int __yradius )
{

}


