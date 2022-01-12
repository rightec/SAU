//---------------------------------------------------------------------------//
//                                                                           //
//                           C O M E T A                                     //
//                                                                           //
//---------------------------------------------------------------------------//
//                                                                             
//  Progetto:                GENLIB 
//  Nome File:               DPYDRV.H
//  Descrizione:             Definizioni per la selezione del driver 
//							 per il display controller desiderato
//  Versione:                $Revision: 1.1 $
//  Data Versione:           $Date: 2002/11/22 00:00:00 $
//  
//---------------------------------------------------------------------------//

#ifndef __DPYDRV_H__
#define __DPYDRV_H__

//--------------------------------------------------------//
// Definizione delle costanti
//--------------------------------------------------------//

					// commentare per eliminare il codice per l'emulazione della
					// modalità testo (console)
#define DPYDRV_ENABLE_TEXT_EMULATION		

					// commentare per eliminare la gestione bufferizzata dell'immagine
					// grafica del display ed avere la gestione diretta del display
#define DPYDRV_ENABLE_GRAPH_IMAGEBUF

					// commentare per eliminare il supporto della modalità grafica
#define DPYDRV_ENABLE_GRAPH_SUPPORT


#define DRV_SED1330		1
#define DRV_SED1335		2
#define DRV_SED1375		3
#define DRV_T6963		4
#define DRV_KS0108		5
#define DRV_TL0313		6
#define DRV_UC1601		7
#define DRV_ST7565_SPI  8



#ifndef DPY_DRV	
#define DPY_DRV	DRV_ST7565_SPI        			/* configurazione driver di default */
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


//--------------------------------------------------------//
// Definizione dei tipi
//--------------------------------------------------------//

//enum DRV_AVAIL{                             /* driver disponibili */
//        DRV_SED1330,                        /* driver per il solito SED1330 */
//        DRV_SED1335,                        /* driver per suo cugino SED1335 */
//        DRV_T6963,                          /* driver per l'intramontabile T6963 */
//        DRV_KS108                        	/* driver per l'economico ks0108b by Samsung*/
//    };

enum CURSDir{
	CURSDIR_RIGHT,
	CURSDIR_LEFT,
	CURSDIR_UP,
	CURSDIR_DOWN,
};

typedef enum CURSDir CURSDir;


struct TextCursorInfo{					// informazioni cursore modalità testo
	byte 		x;						// coordinate cursore in modalità testo
	byte 		y;						
	byte		xsize;					// dimensione del cursore
	byte		ysize;					// dimensione del cursore
	CURSDir 	dir;					// direzione dello spostamento
	int			offset;					// offset del cursore nella ram testo
};

typedef struct TextCursorInfo TextCursorInfo;


struct TextScreenInfo{					// informazioni schermo in modalità testo
	byte	w;							// largezza massima della finestra di testo
	byte	h;							// altezza massimo della finestra di testo
	bool	enableScroll;				// consente o meno lo scroll di una riga
};

typedef struct TextScreenInfo TextScreenInfo;


struct DrvTextInfo{						// informazioni per il driver in modalità testo
	TextCursorInfo  curs;					// info relative al cursore
	TextScreenInfo	screen;				// info relative allo schermo in mod. testo
};

typedef struct DrvTextInfo DrvTextInfo;


struct GraphCursorInfo{					// informazioni cursore modalità grafica
	short 		x;						// coordinate cursore in modalità grafica
	short		y;						
	CURSDir 	dir;					// direzione dello spostamento
	long		offset;					// offset del cursore nella ram grafica
};

typedef struct GraphCursorInfo GraphCursorInfo;


struct GraphScreenInfo{					// informazioni schermo in modalità grafica
	short	w;							// largezza massima della finestra grafica
	short	h;							// altezza massimo della finestra grafica
};

typedef struct GraphScreenInfo GraphScreenInfo;

struct GraphColorInfo{					// informazioni schermo in modalità grafica
	short	fgcolor;					// colore di foreground
	short	bkcolor;					// colore di background
	short	maxcolor;					// ultimo colore disponibile
	short	fillcolor;					// colore di fill
};

typedef struct GraphColorInfo GraphColorInfo;

struct ViewPortInfo{					// informazioni area di lavoro (viewport)
	short   vpl;
	short	vpt;
	short 	vpr;
	short	vpb;
	int		clip;						// clipping .... on view port
};

typedef struct ViewPortInfo ViewPortInfo;

struct DrvGraphInfo{					// informazioni per il driver in modalità grafica
	GraphCursorInfo  curs;				// info relative al cursore
	GraphScreenInfo	 screen;			// info relative allo schermo in mod. grafica
	GraphColorInfo	 color;				// info relative alla gestione dei colori
	ViewPortInfo	 vport;				// info relative al viewport attuale
	CURSDir		drawDir;				// direzione del disegno

	int			drawMode;				// modalità
};

typedef struct DrvGraphInfo DrvGraphInfo;

/*
	int           __gr_lstyle = SOLID_LINE;         // Actual line style        
	int           __gr_fpatno = SOLID_FILL;         // Actual filling pattern   
	int           __gr_WR = GrWRITE;                // Write mode               
	int           __gr_Result = grOk;               // stores error code        
	int           __gr_Mode = 0;                    // actual graphics mode     
	int           __gr_INIT = FALSE;                // TRUE after initgraph()   
	int           __gr_X, __gr_Y;                   // graphics cursor pos      
	int           __gr_Y_page_offs = 0;             // Y offset for page simulation 

*/

struct PCXHEADER{

	char PcxManufacturer;
	char PcxVersion;
	char PcxEncoding;
	char PcxBitsPixel;

	unsigned short PcxXmin;
	unsigned short PcxYmin;
	unsigned short PcxXmax;
	unsigned short PcxYmax;
	unsigned short PcxHres;
	unsigned short PcxVres;

	char PcxPalette[48];
	
	unsigned char PcxReserved;
	unsigned char PcxPlanes;
	
	short PcxBytesLine;
	short PcxPaletteInfo;
	
	unsigned char PcxFiller[58];
};

typedef struct PCXHEADER PCXHEADER;
typedef unsigned char HPALETTE;



//--------------------------------------------------------//
// Definizione delle classi 
//--------------------------------------------------------//

//--------------------------------------------------------//
// Definizione delle funzioni
//--------------------------------------------------------//

#ifdef __cplusplus
extern "C" {
#endif

void display_init(int mode, bool __warmInit);
bool display_chrsetLoad(char *nameSet);
void display_backlight(int activate);
void display_brightness(byte percent);
void display_update(void);
void display_contrast(byte __value);
	
void drv_getTextInfo(DrvTextInfo *info);
void drv_getGraphInfo(DrvGraphInfo *info);

void drv_setTextCursPos(int x, int y);
void drv_setTextCursDir(CURSDir reqDir);
void drv_putTextChar(int c);
void drv_scrollTextScreen(int numRow);
void drv_fillTextBlock(int count, int value);
void drv_getTextBlock(byte *buf, int count);
void drv_putTextBlock(byte *buf, int count);
void drv_setTextPointer(int addr);
void drv_setGraphPointer(int addr);
void drv_setGraphCursPos(int x, int y);
void drv_setGraphCursDir(CURSDir reqDir);
void drv_putGraphChar(int c);
void drv_putForeChar(int c);
void drv_fillGraphBlock(int count, int color);
void drv_getGraphBlock(byte *pb, int count);
void drv_putGraphBlock(const byte *pb, int count);
void drv_putpixel(int __x, int __y, int color);
int  drv_getpixel(int __x, int __y);
void drv_hline(int __xl, int __y, int __xr, int color);
void drv_vline(int __x, int __yt, int __yb, int color);
void drv_putimage(int x, int y, const void *point, int option);
int  drv_putPcxPicture(int x1, int y1, const byte *picture);
void drv_setCursDir(CURSDir reqDir);
void drv_puticon(int __x, int __y, const void *__bitmap, int __width, int __height);


#ifdef __cplusplus
}
#endif


//--------------------------------------------------------//
// Definizione delle variabili
//--------------------------------------------------------//

#ifndef __DPYDRV__

#ifdef __SDATA__							// se utilizza il segmento sdata ....
									// informazioni per il driver in modalità testo
extern DrvTextInfo		textInfo 	__attribute__ ((section (".sdata")));		
									// informazioni per il driver in modalità grafica
extern DrvGraphInfo 	graphInfo 	__attribute__ ((section (".sdata")));		
#else
extern DrvTextInfo		textInfo;			// informazioni per il driver in modalità testo
extern DrvGraphInfo 	graphInfo;			// informazioni per il driver in modalità grafica
#endif

#endif 


#endif

//@@-----------------------------------------------------------------------@@//
//@@                                                                             
//@@  $Source:  $
//@@  Note di revisione:                
//@@
//@@  $Log:  $
//@@
//@@  $Author: junior $
//@@  $RCSfile: DPYDRV.H,v $
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

