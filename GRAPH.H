//---------------------------------------------------------------------------//
//                                                                           //
//                           C O M E T A                                     //
//                                                                           //
//---------------------------------------------------------------------------//
//                                                                             
//  Progetto:                GENLIB
//  Nome File:               GRAPH.H
//  Descrizione:             Definizioni gestione display in modalità grafica
//  Versione:                $Revision: 1.1 $
//  Data Versione:           $Date: 2005/09/03 20:54:47 $
//  
//---------------------------------------------------------------------------//

#ifndef __GRAPH_H__
#define __GRAPH_H__

#include "global.h"
#include "error.h"
#include "chrset.h"
#include "dpydrv.h"					// driver generico per il display...

//--------------------------------------------------------//
// Definizione delle costanti
//--------------------------------------------------------//


#if !defined(__VIDEOMODE)
#define __VIDEOMODE

#define 	TEXT_MODE			0x01
#define 	TEXT_RAMCHGEN_MODE	0x02
#define 	GRAPH_MODE			0x04
#define 	COLOR_MODE			0x10
#define 	ROTATE_MODE			0x20	// visualizzazione ruotata di 180gradi

#endif


#if !defined(__COLORS)
#define __COLORS

enum COLORS {
    BLACK,          /* dark colors */
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    LIGHTGRAY,
    DARKGRAY,           /* light colors */
    LIGHTBLUE,
    LIGHTGREEN,
    LIGHTCYAN,
    LIGHTRED,
    LIGHTMAGENTA,
    YELLOW,
    WHITE
};
#endif


enum line_styles {      /* Line styles for get/setlinestyle */
    SOLID_LINE   = 0,
    DOTTED_LINE  = 1,
    CENTER_LINE  = 2,
    DASHED_LINE  = 3,
    USERBIT_LINE = 4,   /* User defined line style */
};


enum line_widths {      /* Line widths for get/setlinestyle */
    NORM_WIDTH  = 1,
    THICK_WIDTH = 3,
};


enum font_names {
    DEFAULT_FONT    = 0,    /* 8x8 bit mapped font */
    TRIPLEX_FONT    = 1,    /* "Stroked" fonts */
    SMALL_FONT  	= 2,
    SANS_SERIF_FONT = 3,
    GOTHIC_FONT 	= 4,
    SCRIPT_FONT 	= 5,
    SIMPLEX_FONT 	= 6,
    TRIPLEX_SCR_FONT = 7,
    COMPLEX_FONT 	= 8,
    EUROPEAN_FONT 	= 9,
    BOLD_FONT 		= 10
};


#define HORIZ_DIR   0   /* left to right */
#define VERT_DIR    1   /* bottom to top */

#define USER_CHAR_SIZE  0   /* user-defined char size */


enum fill_patterns {        /* Fill patterns for get/setfillstyle */
    EMPTY_FILL,     /* fills area in background color */
    SOLID_FILL,     /* fills area in solid fill color */
    LINE_FILL,      /* --- fill */
    LTSLASH_FILL,       /* /// fill */
    SLASH_FILL,     /* /// fill with thick lines */
    BKSLASH_FILL,       /* \\\ fill with thick lines */
    LTBKSLASH_FILL,     /* \\\ fill */
    HATCH_FILL,     /* light hatch fill */
    XHATCH_FILL,        /* heavy cross hatch fill */
    INTERLEAVE_FILL,    /* interleaving line fill */
    WIDE_DOT_FILL,      /* Widely spaced dot fill */
    CLOSE_DOT_FILL,     /* Closely spaced dot fill */
    USER_FILL       /* user defined fill */
};


enum putimage_ops {     /* BitBlt operators for putimage */
    COPY_PUT,       /* MOV */
    XOR_PUT,        /* XOR */
    OR_PUT,         /* OR  */
    AND_PUT,        /* AND */
    NOT_PUT         /* NOT */
};


enum text_just {        /* Horizontal and vertical justification
                   for settextjustify */
    LEFT_TEXT   = 0,
    CENTER_TEXT = 1,
    RIGHT_TEXT  = 2,

    BOTTOM_TEXT = 0,
     /* CENTER_TEXT = 1,  already defined above */
    TOP_TEXT    = 2
};


#define MAXCOLORS 15

struct palettetype {
    unsigned char size;
    signed char colors[MAXCOLORS+1];
};


struct linesettingstype {
    int linestyle;
    unsigned upattern;
    int thickness;
};


struct textsettingstype {
    int font;
    int direction;
    int charsize;
    int horiz;
    int vert;
};


struct fillsettingstype {
    int pattern;
    int color;
};


struct pointtype {
    int x, y;
};

typedef struct pointtype pointtype;


struct viewporttype{
    int left, top, right, bottom;
    int clip;
};

typedef struct viewporttype viewporttype;


struct arccoordstype {
    int x, y;
    int xstart, ystart, xend, yend;
};

typedef struct arccoordstype arccoordstype;


//--------------------------------------------------------//
// Definizione dei tipi
//--------------------------------------------------------//



#ifndef __BCCGRX00_H
#define __BCCGRX00_H

//#define HOLD_GR_DEFS

#include <stddef.h>
#include <malloc.h>
#include <string.h>

//#ifdef GRX20
//  #include <grx20.h>
//#else
//  #include <grx.h>
//#endif
//#include <grdriver.h>
//#include "../include/libbcc.h"

#if !defined(GR_DRV_VER_GRD) && !defined(GRX_VERSION)
extern int _grx_arc_xc, _grx_arc_yc, _grx_arc_xs, _grx_arc_ys, _grx_arc_xe, _grx_arc_ye;
#endif

#define SWAP(a,b) ({typeof(a) tmp; tmp = a; a = b; b = tmp;})


#define IMAGE_CONTEXT_SIZE      (((sizeof(GrContext)+15)&~15)+4)


#endif


//--------------------------------------------------------//
// Definizione delle classi 
//--------------------------------------------------------//

//--------------------------------------------------------//
// Definizione delle funzioni
//--------------------------------------------------------//


#ifdef __cplusplus
extern "C" {
#endif


									// funzioni di inizializz. del set di caratteri (esterne)

									// funzioni di inizializz. e controllo del disegno
void	moveto(int __x, int __y);
void	moverel(int __dx, int __dy);
int		getx(void);
int		gety(void);
int    	getmaxx(void);
int    	getmaxy(void);

void	setcolor(int __color);
int    	getcolor(void);
int    	getmaxcolor(void);

void	setbkcolor(int __color);
int		getbkcolor(void);

void	setwritemode(int __mode);

void	setlinestyle(int __linestyle, unsigned __upattern, int __thickness);
void	getlinesettings(struct linesettingstype *__lineinfo);

void	setfillstyle(int __pattern, int __color);
void	getfillsettings(struct fillsettingstype *__fillinfo);

void	setfillpattern(const char *__upattern, int __color);
void	getfillpattern(char *__pattern);

void	setaspectratio(int __xasp, int __yasp );
void	getarccoords(struct arccoordstype *__arccoords);
void	getaspectratio(int *__xasp, int *__yasp);

int		getpalettesize( void );
void	setpalette(int __colornum, int __color);
void	getpalette(struct palettetype *__palette);
void	setrgbpalette(int __colornum, int __red, int __green, int __blue);

void	setallpalette(const struct palettetype *__palette);
struct 	palettetype *getdefaultpalette( void );

void	setactivepage(int __page);
void	setvisualpage(int __page);
void	setviewport(int __left, int __top, int __right, int __bottom, int __clip);
void	getviewsettings(struct viewporttype *__viewport);

void	settextstyle(int __font, int __direction, int __charsize);
void	settextjustify(int __horiz, int __vert);
void	setusercharsize(int __multx, int __divx, int __multy, int __divy);


								// funzioni di disegno basilari

void	cleardevice(void);
void	clearviewport(void);

int		getpixel(int __x, int __y);
void	putpixel(int __x, int __y, int __color);

void	line(int __x1, int __y1, int __x2, int __y2);

void 	hor_line(int __x1, int __y, int __x2);
void 	ver_line(int x, int y1,int y2);
void	linerel(int __dx, int __dy);
void	lineto(int __x, int __y);

void	rectangle(int __left, int __top, int __right, int __bottom);

void	arc(int __x, int __y, int __stangle, int __endangle, int __radius);
void	circle(int __x, int __y, int __radius);
void	ellipse(int __x, int __y, int __stangle, int __endangle, int __xradius, int __yradius);
void	pieslice(int __x, int __y, int __stangle, int __endangle, int __radius);
void	sector(int __X, int __Y, int __StAngle, int __EndAngle, int __XRadius, int __YRadius );

void	bar(int __left, int __top, int __right, int __bottom);
void	bar3d(int __left, int __top, int __right, int __bottom, int __depth, int __topflag);

void	drawpoly(int __numpoints, const int *__polypoints);

void	fillpoly(int __numpoints, const int *__polypoints);
void	fillellipse( int __x, int __y, int __xradius, int __yradius );
void	floodfill(int __x, int __y, int __border);

void	putimage(int __left, int __top, const void *bitmap, int option);
void	getimage(int __left, int __top, int __right, int __bottom, void *__bitmap);
int		imagesize(int __left, int __top, int __right, int __bottom);
int 	putpcxpicture(int x1, int y1, const byte *picture);
void 	puticon(int __left, int __top, const void *__bitmap);
void 	putsmallicon(int __left, int __top, const void *__bitmap);


								// funzioni di disegno del testo vettoriale
void	outtext(const char *__textstring);
void	outtextxy(int __x, int __y, const char *__textstring);
int		textheight(const char *__textstring);
int		textwidth(const char *__textstring);
void	gettextsettings(struct textsettingstype *__texttypeinfo);

												
								// funzioni di disegno del testo a bitmap
void	gputc(int c);
void	gputcxy(int __x, int __y, int c);

void	gputs(const char *__textstring);
void	gputsxy(int __x, int __y, const char *__textstring);

int 	gprintf(const char *__fmt, ...);
int 	gprintfxy(int __x, int __y, const char *__fmt, ...);

const char	*grapherrormsg(int __errorcode);

#ifdef __cplusplus
}
#endif


#endif
//--------------------------------------------------------//
// Definizione delle variabili
//--------------------------------------------------------//






//@@-----------------------------------------------------------------------@@//
//@@                                                                             
//@@  $Source: \\repos/Confer/GRAPH.H,v $
//@@  Note di revisione:                
//@@
//@@  $Log: GRAPH.H,v $
//@@  Revision 1.1  2005/09/03 20:54:47  b
//@@  Prima versione funzionante sulle schede conference
//@@
//@@
//@@  $Author: b $
//@@  $RCSfile: GRAPH.H,v $
//@@  $Revision: 1.1 $
//@@  $State: Exp $
//@@  $Date: 2005/09/03 20:54:47 $
//@@  $Name:  $
//@@-----------------------------------------------------------------------@@//
//@@-----------------------------------------------------------------------@@//
//@@                                                                       @@//
//@@                            C O M E T A                                @@//
//@@                                                                       @@//
//@@-----------------------------------------------------------------------@@//

