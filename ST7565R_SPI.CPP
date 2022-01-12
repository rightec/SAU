/**
@file		ST7565R-SPI.cpp
@brief		driver controller ST7565R used in SPI mode

@author		
@date		07/07/2011
@version	01.0
*/

//-------------------------------------------------------
// Variabili globali utili a tutti i moduli
//-------------------------------------------------------

//-------------------------------------------------------
// Funzioni statiche utili solo a questo modulo
//-------------------------------------------------------

//-------------------------------------------------------
// Costanti statiche utili solo a questo modulo
//-------------------------------------------------------

#ifdef DPYDRV_ENABLE_TEXT_EMULATION			// se chiedono pure il supporto per l'emulazione della modalità testo

									// Pattern per la stampa dei caratteri in emulazione 
									// della modalità di scrittura testo
static const byte textEmuFont6x7[]  __attribute__ ((section (".message"))) = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,// (space)
		0x00, 0x00, 0x5F, 0x00, 0x00, 0x00,// !
		0x00, 0x07, 0x00, 0x07, 0x00, 0x00,// "
		0x14, 0x7F, 0x14, 0x7F, 0x14, 0x00,// #
		0x24, 0x2A, 0x7F, 0x2A, 0x12, 0x00,// $
		0x23, 0x13, 0x08, 0x64, 0x62, 0x00,// %
		0x36, 0x49, 0x55, 0x22, 0x50, 0x00,// &
		0x00, 0x05, 0x03, 0x00, 0x00, 0x00,// '
		0x00, 0x1C, 0x22, 0x41, 0x00, 0x00,// (
		0x00, 0x41, 0x22, 0x1C, 0x00, 0x00,// )
		0x08, 0x2A, 0x1C, 0x2A, 0x08, 0x00,// *
		0x08, 0x08, 0x3E, 0x08, 0x08, 0x00,// +
		0x00, 0x50, 0x30, 0x00, 0x00, 0x00,// ,
		0x08, 0x08, 0x08, 0x08, 0x08, 0x00,// -
		0x00, 0x30, 0x30, 0x00, 0x00, 0x00,// .
		0x20, 0x10, 0x08, 0x04, 0x02, 0x00,// /
		0x3E, 0x51, 0x49, 0x45, 0x3E, 0x00,// 0
		0x00, 0x42, 0x7F, 0x40, 0x00, 0x00,// 1
		0x42, 0x61, 0x51, 0x49, 0x46, 0x00,// 2
		0x21, 0x41, 0x45, 0x4B, 0x31, 0x00,// 3
		0x18, 0x14, 0x12, 0x7F, 0x10, 0x00,// 4
		0x27, 0x45, 0x45, 0x45, 0x39, 0x00,// 5
		0x3C, 0x4A, 0x49, 0x49, 0x30, 0x00,// 6
		0x01, 0x71, 0x09, 0x05, 0x03, 0x00,// 7
		0x36, 0x49, 0x49, 0x49, 0x36, 0x00,// 8
		0x06, 0x49, 0x49, 0x29, 0x1E, 0x00,// 9
		0x00, 0x36, 0x36, 0x00, 0x00, 0x00,// :
		0x00, 0x56, 0x36, 0x00, 0x00, 0x00,// ;
		0x00, 0x08, 0x14, 0x22, 0x41, 0x00,// <
		0x14, 0x14, 0x14, 0x14, 0x14, 0x00,// =
		0x41, 0x22, 0x14, 0x08, 0x00, 0x00,// >
		0x02, 0x01, 0x51, 0x09, 0x06, 0x00,// ?
		0x32, 0x49, 0x79, 0x41, 0x3E, 0x00,// @
		0x7E, 0x11, 0x11, 0x11, 0x7E, 0x00,// A
		0x7F, 0x49, 0x49, 0x49, 0x36, 0x00,// B
		0x3E, 0x41, 0x41, 0x41, 0x22, 0x00,// C
		0x7F, 0x41, 0x41, 0x22, 0x1C, 0x00,// D
		0x7F, 0x49, 0x49, 0x49, 0x41, 0x00,// E
		0x7F, 0x09, 0x09, 0x01, 0x01, 0x00,// F
		0x3E, 0x41, 0x41, 0x51, 0x32, 0x00,// G
		0x7F, 0x08, 0x08, 0x08, 0x7F, 0x00,// H
		0x00, 0x41, 0x7F, 0x41, 0x00, 0x00,// I
		0x20, 0x40, 0x41, 0x3F, 0x01, 0x00,// J
		0x7F, 0x08, 0x14, 0x22, 0x41, 0x00,// K
		0x7F, 0x40, 0x40, 0x40, 0x40, 0x00,// L
		0x7F, 0x02, 0x04, 0x02, 0x7F, 0x00,// M
		0x7F, 0x04, 0x08, 0x10, 0x7F, 0x00,// N
		0x3E, 0x41, 0x41, 0x41, 0x3E, 0x00,// O
		0x7F, 0x09, 0x09, 0x09, 0x06, 0x00,// P
		0x3E, 0x41, 0x51, 0x21, 0x5E, 0x00,// Q
		0x7F, 0x09, 0x19, 0x29, 0x46, 0x00,// R
		0x46, 0x49, 0x49, 0x49, 0x31, 0x00,// S
		0x01, 0x01, 0x7F, 0x01, 0x01, 0x00,// T
		0x3F, 0x40, 0x40, 0x40, 0x3F, 0x00,// U
		0x1F, 0x20, 0x40, 0x20, 0x1F, 0x00,// V
		0x7F, 0x20, 0x18, 0x20, 0x7F, 0x00,// W
		0x63, 0x14, 0x08, 0x14, 0x63, 0x00,// X
		0x03, 0x04, 0x78, 0x04, 0x03, 0x00,// Y
		0x61, 0x51, 0x49, 0x45, 0x43, 0x00,// Z
		0x00, 0x00, 0x7F, 0x41, 0x41, 0x00,// [
		0x02, 0x04, 0x08, 0x10, 0x20, 0x00,// "\"
		0x41, 0x41, 0x7F, 0x00, 0x00, 0x00,// ]
		0x04, 0x02, 0x01, 0x02, 0x04, 0x00,// ^
		0x40, 0x40, 0x40, 0x40, 0x40, 0x00,// _
		0x00, 0x01, 0x02, 0x04, 0x00, 0x00,// `
		0x20, 0x54, 0x54, 0x54, 0x78, 0x00,// a
		0x7F, 0x48, 0x44, 0x44, 0x38, 0x00,// b
		0x38, 0x44, 0x44, 0x44, 0x20, 0x00,// c
		0x38, 0x44, 0x44, 0x48, 0x7F, 0x00,// d
		0x38, 0x54, 0x54, 0x54, 0x18, 0x00,// e
		0x08, 0x7E, 0x09, 0x01, 0x02, 0x00,// f
		0x08, 0x14, 0x54, 0x54, 0x3C, 0x00,// g
		0x7F, 0x08, 0x04, 0x04, 0x78, 0x00,// h
		0x00, 0x44, 0x7D, 0x40, 0x00, 0x00,// i
		0x20, 0x40, 0x44, 0x3D, 0x00, 0x00,// j
		0x00, 0x7F, 0x10, 0x28, 0x44, 0x00,// k
		0x00, 0x41, 0x7F, 0x40, 0x00, 0x00,// l
		0x7C, 0x04, 0x18, 0x04, 0x78, 0x00,// m
		0x7C, 0x08, 0x04, 0x04, 0x78, 0x00,// n
		0x38, 0x44, 0x44, 0x44, 0x38, 0x00,// o
		0x7C, 0x14, 0x14, 0x14, 0x08, 0x00,// p
		0x08, 0x14, 0x14, 0x18, 0x7C, 0x00,// q
		0x7C, 0x08, 0x04, 0x04, 0x08, 0x00,// r
		0x48, 0x54, 0x54, 0x54, 0x20, 0x00,// s
		0x04, 0x3F, 0x44, 0x40, 0x20, 0x00,// t
		0x3C, 0x40, 0x40, 0x20, 0x7C, 0x00,// u
		0x1C, 0x20, 0x40, 0x20, 0x1C, 0x00,// v
		0x3C, 0x40, 0x30, 0x40, 0x3C, 0x00,// w
		0x44, 0x28, 0x10, 0x28, 0x44, 0x00,// x
		0x0C, 0x50, 0x50, 0x50, 0x3C, 0x00,// y
		0x44, 0x64, 0x54, 0x4C, 0x44, 0x00,// z
		0x00, 0x08, 0x36, 0x41, 0x00, 0x00,// {
		0x00, 0x00, 0x7F, 0x00, 0x00, 0x00,// |
		0x00, 0x41, 0x36, 0x08, 0x00, 0x00,// }
		0x08, 0x08, 0x2A, 0x1C, 0x08, 0x00,// ->
		0x08, 0x1C, 0x2A, 0x08, 0x08, 0x00 // <-
};

#endif

//-------------------------------------------------------
// Variabili statiche utili solo a questo modulo
//-------------------------------------------------------
								
/**
TP0 pwm frequency (Hz).
*/
unsigned long tp0_fc;
/**
TP0 value .
*/
unsigned short tp0_value;

static CURSDir cursDir;					// ultima direzione assegnata al cursore
static SingleLcdCtrl ST7565Coord;		// gestione accessi al bi-controller

#ifdef DPYDRV_ENABLE_GRAPH_IMAGEBUF
static byte displayRamImage[MAX_DISP_X * (MAX_DISP_Y/8)];		// buffer immagine elaborazioni, copia
static word displayGraphOffset;
#endif

#ifdef __cplusplus
extern "C" {
#endif


/* Inizializza il controller display come meglio crede seguendo le indicazioni
   richieste: TESTO - GRAFICA o entrambi
*/
void display_init(int mode, bool __warmInit)
{

int row;
int col;

#ifdef DPYDRV_ENABLE_TEXT_EMULATION			// se chiedono pure il supporto per l'emulazione della modalità testo

									// inizializza le var. di lavoro
	textInfo.curs.x = 0;
	textInfo.curs.y = 0;
	textInfo.curs.xsize = 6;
	textInfo.curs.ysize = 8;
	textInfo.curs.dir = CURSDIR_RIGHT;
	textInfo.curs.offset = 0;

	textInfo.screen.w = MAX_TEXT_COL;
	textInfo.screen.h = MAX_TEXT_ROW;
	textInfo.screen.enableScroll = False;

#endif

	graphInfo.curs.x = 0;
	graphInfo.curs.y = 0;
	graphInfo.curs.dir = CURSDIR_RIGHT;
	graphInfo.curs.offset = 0;

	graphInfo.screen.w = MAX_DISP_X;
	graphInfo.screen.h = MAX_DISP_Y;

	graphInfo.color.fgcolor = BLACK;
	graphInfo.color.bkcolor = WHITE;

	ST7565Coord.x_onWrite = 0;				// init cache coordinate impostate
	ST7565Coord.x_onRead = 0;				// init cache coordinate impostate

	ST7565Coord.x_cs1 = 0;					// init cache coordinate impostate
	ST7565Coord.y_cs1 = 0;					// sui due quadri

	ST7565Coord.dummy = 1;					// necessita del dummy read su cs1	

	for(row=0; row<8; row++)
	{
		for(col=0; col<128; col++)
		{
			displayRamImage[col + (row*128)] = 0x00;
		}
	}
	//;backlight pwm initialization
	// presaler set: fxx/8
	_TP0CKS0 = 1;
	_TP0CKS1 = 1;
	_TP0CKS2 = 0;
	tp0_fc = 2500000;
	
	//timer mode selection = pwm output
	_TP0MD0 = 0;
	_TP0MD1 = 0;
	_TP0MD2 = 1;
	_TP0EEE = 0;
	
	_TP0OL1 = 0; //pin starts level = low
	_TP0OE1 = 1; //enable pin

	tp0_value = 101;
	TP0CCR0 = tp0_value;// Interrupt Frequency --> fi =  fc / (TP0CCR0 + 1);
	TP0CCR1 = 100;//duty cycle;

	/*_PM33 = 0;
	_PMC33 = 1;
	_PFC33 = 1;
	_PF33 = 0;*/

	_TP0CE = 0;
	
	//;inizializza il display grafico Displaytech Ltd LCD MODULE 64128K SERIES
	clrbit(LCD_RESET);				// assert hw rst	
	ST7565_delay(2000);

	
	setbit(LCD_RESET);			// deassert hw rst
	ST7565_delay(2000);

	ST7565_cmd_wr(0xE2);	// // Sortware reset
	ST7565_delay(2000);
	
	ST7565_cmd_wr(0xA2);	// Bias select: 1/9

	ST7565_cmd_wr(0xA0);	// ADC (Address SEG output Correspondence) Select: SEG0 to SEG1 31 

	ST7565_cmd_wr(0xC8); 	// SHL Select: COM63 to COM0
	
	ST7565_cmd_wr(0x25); 	// Set V0 Voltage
	
	ST7565_cmd_wr(0x81);	// Electronic volume mode set ???
	
	ST7565_cmd_wr(0xF8);	// Set Boost ratio 4X
	
	ST7565_cmd_wr(0x2F);	// Power Setting (VF=ON,VR=ON,VC=ON)
	
	ST7565_cmd_wr(0x40);	// Line address set

	ST7565_cmd_wr(0xA6);	// Display normal

	/*ST7565_cmd_wr(0xAF);	// Display ON
	for(delay=0; delay<100; delay++);
	
	ST7565_cmd_wr(0xA5);	// Display all points
	for(delay=0; delay<100; delay++)
		ST7565_delay(10000);*/
	
	ST7565_cmd_wr(0xA4);	// Display normal

	ST7565_cmd_wr(0xAF);	// Display ON
	
	ST7565_cmd_wr(0x40); // Line address set
	ST7565_cmd_wr(0xB0); // Page address set
	ST7565_cmd_wr(0x10); //set column address MSB
	ST7565_cmd_wr(0x00); //set column address LSB
	
	display_contrast(63);
	display_brightness(100);
	if( __warmInit )		// se sta facendo una inizializzazione da caldo ...
	{
	}
	else
	{
		for(row=0; row<8; row++)
		{
			ST7565_cmd_wr(0xB0 | row);	// Page address set

			ST7565_cmd_wr(0x10);		// Set Column address MSB
			ST7565_cmd_wr(0x00);		// Set Column address LSB

			for(col=0; col<128; col++)
			{
				ST7565_data_wr(0xF0);			// fill ram 
			}
		}
	}
	
	ST7565_cmd_wr(CMD_DISPLAY_SETROW | ST7565Coord.y_cs1);			// Page address set
	ST7565_cmd_wr(CMD_DISPLAY_SET_YH | ST7565Coord.x_cs1);			// Set Column address MSB
	ST7565_cmd_wr(CMD_DISPLAY_SET_YL | ST7565Coord.x_cs1);			// Set Column address LSB
	
}



bool display_chrsetLoad(char *nameSet)
{

	if( *nameSet == 0 )
		return(False);
		
	return(True);

}

int display_testSignals(void)
{
	return(0);
}


/* Attiva come richiesto la lambada della retroilluminazione - NOT TO USE - INSTEAD USE display_brightness
#see display_brightness
*/
void display_backlight(int activate)
{
	if( activate == ON )
		setbit(LCD_BACKLT);// adesso è accesa
	else
		clrbit(LCD_BACKLT);// adesso è spenta
}

/* Imposta la percentuale di luminosità richiesta (modulando la lampada ?)
*/
void display_brightness(byte percent)
{
	unsigned long tp0ccr1;
	if( percent >= 100 )					// controllo limiti
	{
		_TP0CE = 0;
		percent = 100;
		setbit(LCD_BACKLT);
		_PMC33 = 0;
		_PFC33 = 0;
		_PF33 = 0;
		
	}else if(percent == 100)
	{
		_TP0CE = 0;
		percent = 100;
		clrbit(LCD_BACKLT);
		_PMC33 = 0;
		_PFC33 = 0;
		_PF33 = 0;
	}else
	{
		_TP0CE = 0;
		tp0ccr1 = (( ((unsigned long)tp0_value) *  (unsigned long)percent) /100);
		TP0CCR1 = (unsigned short)tp0ccr1;
		_PMC33 = 1;
		_PFC33 = 1;
		_PF33 = 0;
		_TP0CE = 1;
	}
	
}

/* Imposta il valore di contrasto richiesto
*/
void display_contrast(byte value)
{
	if( value > 63 )	
		value = 32;					// imposta il default

	
	ST7565_cmd_wr(0x81);  				// Set ref. voltage FIRST INSTRUCTION
	ST7565_delay(5);
	ST7565_cmd_wr(value);				//;0x2C		;Set ref. voltage SECOND INSTRUCTION (0x00 to 0x3F) (Contrasto)*/
}

	// aggiorna il contenuto del buffer immagine sul display esterno
void display_update()
{
#ifdef DPYDRV_ENABLE_GRAPH_IMAGEBUF

byte *pImage;
register int row;
register int col;

//	ST7565_cmd_wr(0x10);	//Set Column address MSB
//	ST7565_cmd_wr(0x00);	//Set Column address LSB

//;-------

//	ST7565_cmd_wr(0xAF);	//;Display ON

	pImage = displayRamImage;

	for(row=0; row<8; )
	{
		ST7565_cmd_wr(0xB0 | row);	// Page address set

		ST7565_cmd_wr(0x10);		// Set Column address MSB
		ST7565_cmd_wr(0x00);		// Set Column address LSB
			
		for(col=0; col<128; col++, pImage++)
		{
			ST7565_data_wr(*pImage);		// fuori il dato (senza controlli aggiuntivi...)
			
		}
		row++;
	}
#endif
}


//////////////////////////////////////////////////////////////////////////////////
//
// Funzioni a medio livello per la gestione del display in modalità testo
//
//////////////////////////////////////////////////////////////////////////////////

#ifdef DPYDRV_ENABLE_TEXT_EMULATION			// se chiedono pure il supporto per l'emulazione della modalità testo

// Posiziona il cursore dove vuoi tu
void drv_setTextCursPos(int x, int y)
{

	textInfo.curs.x = x;
	textInfo.curs.y = y;
	textInfo.curs.offset = textInfo.curs.x + textInfo.screen.w * textInfo.curs.y;		// calcola l'offset dall'inizio dello schermo

	drv_setTextPointer(textInfo.curs.offset);
}


/* Imposta la direzione di spostamento del cursore in modalità testo
*/
void drv_setTextCursDir(CURSDir reqDir)
{
	textInfo.curs.dir = reqDir;					// imposta la nuova direzione richiesta
}


// Spara un carattere a video in modalità testo
void drv_putTextChar(int c)
{

	if( c == '\r' )
	{
		drv_setTextCursPos(0, textInfo.curs.y);				// ritorna sulla prima colonna
	}
	else
	{
		if( c == '\n' )										// se deve scendere di riga
		{
			if( ++textInfo.curs.y < textInfo.screen.h )	// se non sfora
					drv_setTextCursPos(textInfo.curs.x, textInfo.curs.y);	
		}
		else
		{
			drv_setGraphCursPos(textInfo.curs.x * 6, textInfo.curs.y * 8);	// piazza il cursore sulla matricia

			if( c < 32 )
				c = '.';

			drv_putGraphBlock(&textEmuFont6x7[((c - 32) * 6)], 6);			// spara il bitmap paro paro
		
			if( ++textInfo.curs.x >= textInfo.screen.w )		// se superiamo l'ultima colonna
			{
				textInfo.curs.x = 0;						// torna a casa
				textInfo.curs.y++;
			}
		}
	}

	if( textInfo.curs.y >= textInfo.screen.h )	// se siamo fuori area
	{
		if( textInfo.screen.enableScroll )
		{
			drv_scrollTextScreen(1);			// scrolla tutto il testo di una riga
												// possiamo tornare sull'ultima riga
			drv_setTextCursPos(textInfo.curs.x, --textInfo.curs.y);
		}
		else
		{
			drv_setTextCursPos(0, 0);			// riparte dalla prima riga in alto
		}
	}
}

// Posiziona il cursore dove vuoi tu
void drv_setGraphCursPos(int x, int y)
{
	graphInfo.curs.x = x;
	graphInfo.curs.y = y;
	graphInfo.curs.offset = x + ((y / 8) * MAX_DISP_X);		// calcola l'offset dall'inizio dello schermo

	drv_setGraphPointer(graphInfo.curs.offset);
}


/* Esegue lo scroll di una pagina in modalità testo
   del numero di righe richiesto
*/
void drv_scrollTextScreen(int numRow)
{

	if( numRow > textInfo.screen.h )		// se scherzano
		return;								// sono proprio dei burloni

/*	
	La modalità testo è emulata per cui sarebbe necessario "sprecare" circa 200bytes
	di ram per emulare la mappa dei caratteri disegnati sul display ovviamente 
	in questa sede non realizziamo lo scroll del display in modalità testo.
	Peggio ancora non avendo layers separati non possiamo gestirlo in modo adeguato
*/

}

/* Piazza dove vuoi tu il puntatore all'area di testo
*/
void drv_setTextPointer(int addr)
{
	displayGraphOffset = addr;		// si allinea come richiesto
}


//////////////////////////////////////////////////////////////////////////////////
//
// Funzioni a basso livello per la gestione del display in modalità testo
//
//////////////////////////////////////////////////////////////////////////////////


/* Riempie l'area con il valore richiesto
*/
void drv_fillTextBlock(int count, int value)
{
	while( count-- )
		drv_putTextChar(value);

}


/* Copia dall'area di testo sul buffer richiesto
*/
void drv_getTextBlock(byte *buf, int count)
{
	buf[0] = count;
}

/* Copia i dati del buffer passato sull'area di testo 
*/
void drv_putTextBlock(byte *buf, int count)
{
	while( count-- )
		drv_putTextChar(*buf++);
}


#endif  		//text emulation



#ifdef DPYDRV_ENABLE_GRAPH_SUPPORT		// se chiedono pure il supporto per la grafica


/* Piazza dove vuoi il puntatore all'area grafica
*/
inline void drv_setGraphPointer(int addr)
{
	displayGraphOffset = addr;		// si allinea come richiesto
}


//////////////////////////////////////////////////////////////////////////////////
//
// Funzioni a basso livello per la gestione del display in modalità grafica
//
//////////////////////////////////////////////////////////////////////////////////


/* Riempie l'area con il valore richiesto
*/
void drv_fillGraphBlock(int count, int color)
{
byte filler;

	count /= 8;

	if( color == BLACK )								// botte piena o botte vuota ?
		filler = 0xFF;
	else
		filler = 0x00;
	

	while( count-- )
	{
#ifdef	DPYDRV_ENABLE_GRAPH_IMAGEBUF
		displayRamImage[displayGraphOffset++] = filler;

		if( displayGraphOffset >= sizeof(displayRamImage) )
			displayGraphOffset = 0;
#else
		if( ST7565Coord.x_onWrite == 128 )					// se è in fondo
			drv_setGraphCursPos(0, ST7565Coord.y + 8);		// passa alla riga successiva

		ST7565_data_wr(filler);
#endif

	}

}


/* Piglia un blocco di dati dall'area grafica
*/
void drv_getGraphBlock(byte *pb, int count)
{
#ifdef	DPYDRV_ENABLE_GRAPH_IMAGEBUF

	while( count-- )
	{
		*pb++ = displayRamImage[displayGraphOffset++];

		if( displayGraphOffset >= sizeof(displayRamImage) )
			displayGraphOffset = 0;
	}
#else

	while( count-- )
		*pb++ = ST7565_data_rd();						// vuota come da programma
#endif
}

/* Copia un blocco di dati sull'area grafica
*/
void drv_putGraphBlock(const byte *pb, int count)
{
#ifdef	DPYDRV_ENABLE_GRAPH_IMAGEBUF
	while( count-- )
	{
		displayRamImage[displayGraphOffset++] = *pb++;

		if( displayGraphOffset >= sizeof(displayRamImage) )
			displayGraphOffset = 0;
	}
			
#else
	while( count-- )
		ST7565_data_wr(*pb++);							// riempie come da programma
#endif
}


/* disegna solo linee orizzontali
*/
void drv_hline(int __x1, int __y, int __x2, int __color)
{
int pixCount;
int xLeft;
int xRight;
int n;
int mask;
int byteCount;
byte localBuf[32], *pb;

	if( __x1 > __x2 )
	{
		xLeft =  __x2;
		xRight = __x1;
	}
	else
	{
		xLeft =  __x1;
		xRight = __x2;
	}

	pixCount = xRight - xLeft + 1;	// numero complessivo di pixel da visualizzare

	mask = (0x01 << (__y & 7));							// bit di maschera..

	drv_setCursDir(CURSDIR_RIGHT);		// per comodità visualizziamo tutte le righe


	if( xLeft == 62 )
		__x2 = 0;
	
	while( pixCount )									// per la sfilza di pixel
	{
		drv_setGraphCursPos(xLeft, __y);

		if( pixCount > (int) sizeof(localBuf) )				// al massimo carica un buffer per volta
			byteCount = sizeof(localBuf);
		else
			byteCount = pixCount;

		pixCount -= byteCount;							// scala....
		
		drv_getGraphBlock(localBuf, byteCount);			// legge una sfetlina

		pb = localBuf;									// parte dal primo byte di pattern
		
		for(n=byteCount; n; n--)
		{
		  	if( __color == BLACK )						// riga nera ?
			    *pb++ |= mask;							// pixel on
		  	else
		    	*pb++ &= ~mask;							// pixel off
		}

		drv_setGraphPointer(graphInfo.curs.offset);		// ripiazza il puntatore
		drv_putGraphBlock(localBuf, byteCount);			// legge una sfetlina

		xLeft += byteCount;
 	}
}	




/* Disegna solo linee verticali
*/
void drv_vline(int __x, int __y1, int __y2, int color)
{
int pixCount;
int byteCount;
byte maskUp;
byte maskDn;
int pixUp;
int pixDown;
int yTop;
int yBot;
int c;

	if( __y1 < __y2 )
	{
		yTop =  __y1;
		yBot = __y2;
	}
	else
	{
		yTop =  __y2;
		yBot = __y1;
	}

								// griglia pixel sullo schermo in orizzontale/verticale
								// ------------------
								// *****	0
								// *****	1
								// *****	2
								// **.**	3	yTop = 3;
								// **.**	4
								// **.**	5
								// **.**	6
								// **.**	7	yTop = 7;
								// ------------------
								// **.**	0	(8)
								// **.**	1	(9)
								// **.**	2	(10)
								// **.**	3	(11)
								// **.**	4
								// **.**	5
								// **.**	6
								// **.**	7	(15)
								// ------------------
								// **.**	0	(16)
								// **.**	1
								// **.**	2
								// **.**	3
								// **.**	4	yBot = 20
								// *****	5
								// *****	6
								// *****	7	yBot = 23
								// ------------------
								// *****	0	(56)
								// *****	1	(57)
								// *****	2	(58)
								// *****	3	(59)
								// **.**	4	(60)
								// **.**	5	(61)
								// **.**	6	(62)
								// *****	7	(63)
								// ------------------
								// *****								

	pixCount = yBot - yTop + 1;			// numero complessivo di pixel da visualizzare
	
	pixUp = 8 - (yTop & 0x07);						// pixel lato up

	pixDown = 7 - (yBot & 0x07);					// pixel lato down
	
	if( pixUp == 8 )								// se il byte è pieno..
		pixUp = 0;									// lo ignora..

	if( pixDown == 8 )								// se l'ultimo byte è pieno..
		pixDown = 0;								// lo ignora..

	byteCount = (pixCount - pixUp - pixDown) / 8;	// bytes verticali interi

	if( byteCount == 0 )						// se ci sono solo dei rotti....
	{											// non vale nemmeno la pena fare maschere...
		while( yTop <= yBot )												
			drv_putpixel(__x, yTop++, color);	// disegna un pixel per volta
	}
	else
	{
		maskUp = 0xFF << (yTop & 0x07);					// maschera di ritaglio lato up
		maskDn = 0xFF >> (7 - (yBot & 0x07));			// maschera di ritaglio down
		
#ifdef	DPYDRV_ENABLE_GRAPH_IMAGEBUF

		if( pixUp )
		{
			drv_setGraphCursPos(__x, yTop);
		
			c = displayRamImage[displayGraphOffset];			// legge un byte di colonna

		  	if( color == BLACK )
			    c |= maskUp;									// pixel on
		  	else
		    	c &= ~maskUp;									// pixel off
	
			displayRamImage[displayGraphOffset++] = c;			// lo rimette dov'era

			if( displayGraphOffset >= sizeof(displayRamImage) )
				displayGraphOffset = 0;

			yTop += pixUp;
		}

												// disegna la parte centrale della riga
												//////////////////////////////////////
		while( byteCount )
		{
			drv_setGraphCursPos(__x, yTop);
		
			c = displayRamImage[displayGraphOffset];			// legge un byte di colonna

		  	if( color == BLACK )
			    c |= 0xFF;										// pixel on
		  	else
		    	c &= ~0xFF;										// pixel off

			displayRamImage[displayGraphOffset++] = c;			// lo rimette dov'era

			if( displayGraphOffset >= sizeof(displayRamImage) )
				displayGraphOffset = 0;

			byteCount--;
			yTop += 8;
		}
		
												// disegna la parte bassa della riga
												//////////////////////////////////////
		if( pixDown )
		{
			drv_setGraphCursPos(__x, yTop);
		
			c = displayRamImage[displayGraphOffset];			// legge un byte di colonna
			
		  	if( color == BLACK )
			    c |= maskDn;									// pixel on
		  	else
		    	c &= ~maskDn;									// pixel off
	
			displayRamImage[displayGraphOffset++] = c;			// lo rimette dov'era

			if( displayGraphOffset >= sizeof(displayRamImage) )
				displayGraphOffset = 0;

			yTop += pixDown;
		}
			
#else												// disegna la parte alta della riga
												//////////////////////////////////////
		if( pixUp )
		{
			drv_setGraphCursPos(__x, yTop);
		
			c = ST7565_data_rd();					// legge un byte di colonna
			
		  	if( color == BLACK )
			    c |= maskUp;									// pixel on
		  	else
		    	c &= ~maskUp;									// pixel off
	
			drv_setGraphPointer(graphInfo.curs.offset);			// riporta li'il puntatore
			ST7565_data_wr(c);									// aggiorna come da copione
			yTop += pixUp;
		}

												// disegna la parte centrale della riga
												//////////////////////////////////////
		while( byteCount )
		{
			drv_setGraphCursPos(__x, yTop);
		
			c = ST7565_data_rd();					// legge un byte di colonna

		  	if( color == BLACK )
			    c |= 0xFF;										// pixel on
		  	else
		    	c &= ~0xFF;										// pixel off

			drv_setGraphPointer(graphInfo.curs.offset);			// riporta li'il puntatore
			ST7565_data_wr(c);									// aggiorna come da copione

			byteCount--;
			yTop += 8;
		}
		
												// disegna la parte bassa della riga
												//////////////////////////////////////
		if( pixDown )
		{
			drv_setGraphCursPos(__x, yTop);
		
			c = ST7565_data_rd();					// legge un byte di colonna
			
		  	if( color == BLACK )
			    c |= maskDn;									// pixel on
		  	else
		    	c &= ~maskDn;									// pixel off
	
			drv_setGraphPointer(graphInfo.curs.offset);			// riporta li'il puntatore
			ST7565_data_wr(c);									// aggiorna come da copione

			yTop += pixDown;
		}
#endif			

	}
	
																// fine dell'opera...
}




/* Colora un pixel come vuoi tu
*/
void drv_putpixel(int __x, int __y, int color)
{
int c;
int mask;

	drv_setGraphCursPos(__x, __y);

#ifdef	DPYDRV_ENABLE_GRAPH_IMAGEBUF

	c = displayRamImage[displayGraphOffset];			// legge un byte di colonna
	
	mask = (0x01 << (__y & 7));							// bit di maschera..

  	if( color == BLACK )
	    c |= mask;										// pixel on
  	else
    	c &= ~mask;										// pixel off

	displayRamImage[displayGraphOffset++] = c;			// lo rimette dov'era

	if( displayGraphOffset >= sizeof(displayRamImage) )
		displayGraphOffset = 0;

#else

	c = ST7565_data_rd();
	
	mask = (0x01 << (__y & 7));							// bit di maschera..

  	if( color == BLACK )
	    c |= mask;										// pixel on
  	else
    	c &= ~mask;										// pixel off

	drv_setGraphPointer(graphInfo.curs.offset);			// riporta li'il puntatore

	ST7565_data_wr(c);									// aggiorna come da copione

#endif
}

/* Ritorna il colore del pixel richiesto
*/
int drv_getpixel(int __x, int __y)
{
int c;
int mask;
int color;

	drv_setGraphCursPos(__x, __y);

#ifdef	DPYDRV_ENABLE_GRAPH_IMAGEBUF

	c = displayRamImage[displayGraphOffset];			// legge un byte di colonna

#else
	c = ST7565_data_rd();
	
#endif
	
	mask = (0x01 << (__y & 7));							// bit di maschera..

  	if( (c & mask) != 0 )
  		color = BLACK;									// pixel on
  	else
    	color = WHITE;									// pixel off

	return( color );
}



/* Sparazza a video una immaginina dove vuoi tu
*/
void drv_putimage(int x, int y, const void *pattern, int option)
{
int   byteCount;
int   colCount;
int   rowCount;
int   maskBit;
int   resHiMask, resLoMask, resMidMask;
int   hiShift, loShift;
int	  local_y;
byte  localBuf[128], *pbuf;
bool  validBuf = False;
const byte *pChr, *pCpy;


	pChr = (const byte *) pattern;					// piglia il puntatore al bitmap

	colCount = pChr[0];							// numero totale di colonne
	rowCount = pChr[1];							// numero totale di righe

	pChr += 2;									// qui inizia il pattern

	if( colCount > (int) sizeof(localBuf) )			// se non basta..
	{
		// per piacere aumentate il buffer.....
		return;
	}

	maskBit = 0xFF << (rowCount & 0x07);
	
	local_y = y;

	drv_setGraphCursPos(x, y);


	if( (graphInfo.curs.y & 0x07) == 0 )			// se il primo byte è allineato...
	{
		pCpy = pChr;								// parte dal primo pattern

		while( rowCount >= 8 )						// se usano almeno 8 pixel
		{
			byteCount = colCount;						// numero di pixel in larghezza

			pbuf = localBuf;

			if( graphInfo.color.fgcolor == BLACK )		// se il colore deve essere nero
			{
				while( byteCount-- )
					*pbuf++ = *pCpy++;					// aggiunge il pattern reale
			}
			else
			{
				while( byteCount-- )
					*pbuf++ = ~(*pCpy++);				// aggiunge il pattern reale
			}

			drv_putGraphBlock(localBuf, colCount);		// riscrive la colonna

			rowCount -= 8;								// questi sono fatti....
														// passa alla riga successiva
			drv_setGraphCursPos(graphInfo.curs.x, graphInfo.curs.y + 8);
		}

		if( rowCount > 0 )							// se rimangono dei rotti
		{
			byteCount = colCount;					// numero di pixel in larghezza

			pbuf = localBuf;
			drv_getGraphBlock(localBuf, byteCount);	// legge una riga

			if( graphInfo.color.fgcolor == BLACK )		// se il colore deve essere nero
			{
				while( byteCount-- )
				{
					*pbuf &= maskBit;			// toglie la prima fettina
					*pbuf++ |= *pCpy++;				// aggiunge il pattern reale
				}
			}
			else
			{
				while( byteCount-- )
				{
					*pbuf &= maskBit;			// toglie la prima fettina
					*pbuf++ |= ~(*pCpy++ | maskBit);			// aggiunge il pattern reale
				}
			}

			drv_setGraphPointer(graphInfo.curs.offset);	// ripiazza il puntatore
			drv_putGraphBlock(localBuf, colCount);		// riscrive la colonna

			rowCount = 0;							// anche questi sono fatti....	
		}
													// avanza del giusto
		drv_setGraphCursPos(graphInfo.curs.x + colCount, local_y);
	}
	else											// se il primo byte non è allineato...
	{
		resHiMask = resLoMask = 0xFF;				// maschere di ritaglio

		hiShift = 8-(graphInfo.curs.y & 0x07);		// contatori di shift pattern
		loShift = graphInfo.curs.y & 0x07;			// 

		resLoMask <<= loShift;						// prepara per spaccare a metà il pattern
		resLoMask = ~resLoMask;						// inverte perchè va usata per togliere
		
		resHiMask >>= hiShift;						// 
		resHiMask = ~resHiMask;						// 
		
		pCpy = pChr;								// parte dal primo pattern

		while( rowCount >= 8 )						// se usano almeno 8 pixel
		{
			byteCount = colCount;						// numero di pixel in larghezza

			pbuf = localBuf;
			drv_getGraphBlock(localBuf, byteCount);	// legge una riga (fetta di pagina)

												// Scrive byte LSB pattern su lato LSB memoria
			if( graphInfo.color.fgcolor == BLACK )			// se il colore deve essere nero
			{
				while( byteCount-- )
				{
					*pbuf &= resLoMask;				// toglie la seconda fettina
					*pbuf++ |= *pCpy++ << loShift;	// aggiunge il pattern reale
				}
			}
			else
			{
				while( byteCount-- )
				{
					*pbuf &= resLoMask;				// toglie la prima fettina
					*pbuf++ |= ~(*pCpy++) << loShift;		// aggiunge il pattern reale
				}
			}


			drv_setGraphPointer(graphInfo.curs.offset);	// ripiazza il puntatore
			drv_putGraphBlock(localBuf, colCount);		// riscrive la fetta di riga

														// passa alla riga successiva
			drv_setGraphCursPos(graphInfo.curs.x, graphInfo.curs.y + 8);

			pCpy = pChr;								// parte dal primo pattern
			byteCount = colCount;						// numero di pixel in larghezza
			pbuf = localBuf;

			drv_getGraphBlock(localBuf, byteCount);		// legge una fetta di riga

												// Scrive pattern lato MSB
			if( graphInfo.color.fgcolor == BLACK )			// se il colore deve essere nero
			{
				while( byteCount-- )
				{
					*pbuf &= resHiMask;					// toglie la prima fettina
					*pbuf++ |= *pCpy++ >> hiShift;		// aggiunge il pattern reale
				}
			}
			else
			{
				while( byteCount-- )
				{
					*pbuf &= resHiMask;				// toglie la prima fettina
					*pbuf++ |= ~( (*pCpy++ >> hiShift) | resHiMask);	// aggiunge il pattern reale
				}
			}

			rowCount -= 8;								// questi sono fatti....	

			pChr = pCpy;								// salva l'ultimo pattern
			
			if( rowCount >= 8 || rowCount == 0 )			// piccola ottimizzazione...
			{
				drv_setGraphPointer(graphInfo.curs.offset);	// ripiazza il puntatore
				drv_putGraphBlock(localBuf, colCount);		// riscrive la colonna
	
															// resta sulla colonna attuale
				drv_setGraphCursPos(graphInfo.curs.x, graphInfo.curs.y);
			}
			else
			{
				validBuf = True;						// segnala che il buffer è già pieno...
			}
		}
		
		if( rowCount > 0 )								// se rimangono dei rotti
		{
			pCpy = pChr;								// continua....
			byteCount = colCount;						// numero di pixel in altezza

			resMidMask = maskBit << loShift;		// parte dai bits validi

			pbuf = localBuf;

			if( validBuf == True )						// piccola ottimizzazione sul caricamento
				validBuf = False;
			else
				drv_getGraphBlock(localBuf, byteCount);		// legge una fetta di riga (è l'ultima letta)

														// Scrive pattern lato MSB
			if( graphInfo.color.fgcolor == BLACK )		// se il colore deve essere nero
			{
				while( byteCount-- )
				{
					*pbuf &= resLoMask | resMidMask;	// toglie la seconda fettina
					*pbuf++ |= *pCpy++ << loShift;		// aggiunge il pattern reale
				}
			}
			else
			{
				while( byteCount-- )
				{
					*pbuf &= resLoMask | resMidMask;	// toglie la prima fettina
					*pbuf++ |= ~( (*pCpy++ << loShift) | resLoMask | resMidMask );		// aggiunge il pattern reale
				}
			}

			drv_setGraphPointer(graphInfo.curs.offset);		// ripiazza il puntatore
			drv_putGraphBlock(localBuf, colCount);			// riscrive la fettina di riga


			if( ((graphInfo.curs.y & 0x07) + rowCount) > 8 ) // se non basta una passata...
			{
	
														// passa alla riga successiva
				drv_setGraphCursPos(graphInfo.curs.x, graphInfo.curs.y + rowCount);

				pCpy = pChr;								// parte dal primo pattern
				byteCount = colCount;						// numero di pixel in larghezza
				pbuf = localBuf;

				resMidMask = maskBit >>	hiShift;		// parte dai bits validi

				drv_getGraphBlock(localBuf, byteCount);		// legge la riga succ.
	
															// Scrive byte MSB pattern su lato LSB memoria
				if( graphInfo.color.fgcolor == BLACK )		// se il colore deve essere nero
				{
					while( byteCount-- )
					{
						*pbuf &= resHiMask | resMidMask;	// toglie la fettina centrale
						*pbuf++ |= *pCpy++ >> hiShift;		// aggiunge il pattern reale
					}
				}
				else
				{
					while( byteCount-- )
					{
						*pbuf &= resHiMask | resMidMask;	// toglie la fettina centrale
						*pbuf++ |= ~( (*pCpy++ >> hiShift) | resHiMask | resMidMask );		// aggiunge il pattern reale
					}
				}

				drv_setGraphPointer(graphInfo.curs.offset);	// ripiazza il puntatore
				drv_putGraphBlock(localBuf, colCount);		// riscrive la riga
				rowCount = 0;								// anche questi sono fatti....	
			}
			pChr = pCpy;									// salva l'ultimo pattern
															// avanza del giusto
		}
		drv_setGraphCursPos(graphInfo.curs.x + colCount, local_y);
	}


}					

int drv_putPcxPicture(int __x1, int __y1, const byte *__picture)
{
	return( -1 );
}

/* Imposta la direzione di spostamento del cursore sul controller
*/
void drv_setCursDir(CURSDir reqDir)
{

	if( cursDir != reqDir )
	{
		switch( reqDir )					// questo controller fa molto poco....
		{
			case	CURSDIR_RIGHT:
				break;
				
			case	CURSDIR_LEFT:
				break;
				
			case	CURSDIR_UP:
				break;
				
			case	CURSDIR_DOWN:
				break;
		}
		cursDir = reqDir;					// imposta la nuova direzione richiesta
	}
}



/* Spara un carattere a video in modalità grafica utilizzando
   il set di caratteri attualmente selezionato
*/
void drv_putGraphChar(int c)
{
int   byteCount;
int   rowCount;
int   resHiMask, resLoMask, resMidMask;
int   hiShift, loShift;
int	  local_y;
byte  localBuf[32], *pbuf;
bool  validBuf = False;
const ChrSet *pc;
const byte *pChr, *pCpy;

	c &= 0xFF;								// seghiamo ?

	pc = charset_getCurrent();				// piglia il set attuale
	
	if( pc == NULL )						// controlli di routine...
		return;	

	if( c < pc->firstChar )					// se sono troppo scarsi
		return;								// si danno poi una calmata

	if( c > (pc->firstChar + pc->numChar) )	// se esagerano...
		return;								// stanno poi tranquilli

	if( pc->h > sizeof(localBuf) )			// se non basta..
	{
		// per piacere aumentate il buffer.....
		return;
	}

	pChr = &pc->pSet[(c - pc->firstChar) * pc->blockSize];	// piglia il puntatore al bitmap
	local_y = graphInfo.curs.y;

	drv_setGraphCursPos(graphInfo.curs.x, graphInfo.curs.y);
	rowCount = pc->h;								// numero di pixel in altezza

	if( (graphInfo.curs.y & 0x07) == 0 )			// se il primo byte è allineato...
	{
		
		pCpy = pChr;								// parte dal primo pattern

		while( rowCount >= 8 )						// se usano almeno 8 pixel
		{
			byteCount = pc->w;						// numero di pixel in larghezza
			pbuf = localBuf;
			drv_getGraphBlock(localBuf, byteCount);	// legge una riga

			if( graphInfo.color.fgcolor == BLACK )		// se il colore deve essere nero
			{
				while( byteCount-- )
					*pbuf++ = *pCpy++;				// aggiunge il pattern reale
			}
			else
			{
				while( byteCount-- )
					*pbuf++ = ~*pCpy++;				// aggiunge il pattern reale
			}

			drv_setGraphPointer(graphInfo.curs.offset);	// ripiazza il puntatore
			drv_putGraphBlock(localBuf, pc->w);			// riscrive la colonna
			rowCount -= 8;							// questi sono fatti....
													// passa alla riga successiva
			drv_setGraphCursPos(graphInfo.curs.x, graphInfo.curs.y + 8);
		}

		if( rowCount > 0 )							// se rimangono dei rotti
		{
			byteCount = pc->w;						// numero di pixel in altezza

			pbuf = localBuf;
			drv_getGraphBlock(localBuf, byteCount);	// legge una riga

			if( graphInfo.color.fgcolor == BLACK )		// se il colore deve essere nero
			{
				while( byteCount-- )
				{
					*pbuf &= pc->maskBit;			// toglie la prima fettina
					*pbuf++ |= *pCpy++;				// aggiunge il pattern reale
				}
			}
			else
			{
				while( byteCount-- )
				{
					*pbuf &= pc->maskBit;			// toglie la prima fettina
					*pbuf++ |= ~(*pCpy++ | pc->maskBit);			// aggiunge il pattern reale
				}
			}

			drv_setGraphPointer(graphInfo.curs.offset);	// ripiazza il puntatore
			drv_putGraphBlock(localBuf, pc->w);			// riscrive la colonna

			rowCount = 0;							// anche questi sono fatti....	
		}
													// avanza del giusto
		drv_setGraphCursPos(graphInfo.curs.x + pc->w, local_y);
	}
	else											// se il primo byte non è allineato...
	{
		resHiMask = resLoMask = 0xFF;				// maschere di ritaglio

		hiShift = 8-(graphInfo.curs.y & 0x07);		// contatori di shift pattern
		loShift = graphInfo.curs.y & 0x07;			// 

		resLoMask <<= loShift;						// prepara per spaccare a metà il pattern
		resLoMask = ~resLoMask;						// inverte perchè va usata per togliere
		
		resHiMask >>= hiShift;						// 
		resHiMask = ~resHiMask;						// 
		
		pCpy = pChr;								// parte dal primo pattern

		while( rowCount >= 8 )						// se usano almeno 8 pixel
		{
			byteCount = pc->w;						// numero di pixel in larghezza

			pbuf = localBuf;
			drv_getGraphBlock(localBuf, byteCount);	// legge una riga (fetta di pagina)

												// Scrive byte LSB pattern su lato LSB memoria
			if( graphInfo.color.fgcolor == BLACK )			// se il colore deve essere nero
			{
				while( byteCount-- )
				{
					*pbuf &= resLoMask;				// toglie la seconda fettina
					*pbuf++ |= *pCpy++ << loShift;	// aggiunge il pattern reale
				}
			}
			else
			{
				while( byteCount-- )
				{
					*pbuf &= resLoMask;				// toglie la prima fettina
					*pbuf++ |= ~(*pCpy++) << loShift;		// aggiunge il pattern reale
				}
			}


			drv_setGraphPointer(graphInfo.curs.offset);	// ripiazza il puntatore
			drv_putGraphBlock(localBuf, pc->w);			// riscrive la fetta di riga

														// passa alla riga successiva
			drv_setGraphCursPos(graphInfo.curs.x, graphInfo.curs.y + 8);

			pCpy = pChr;								// parte dal primo pattern
			byteCount = pc->w;							// numero di pixel in larghezza
			pbuf = localBuf;

			drv_getGraphBlock(localBuf, byteCount);		// legge una fetta di riga

												// Scrive pattern lato MSB
			if( graphInfo.color.fgcolor == BLACK )			// se il colore deve essere nero
			{
				while( byteCount-- )
				{
					*pbuf &= resHiMask;					// toglie la prima fettina
					*pbuf++ |= *pCpy++ >> hiShift;		// aggiunge il pattern reale
				}
			}
			else
			{
				while( byteCount-- )
				{
					*pbuf &= resHiMask;				// toglie la prima fettina
					*pbuf++ |= ~( (*pCpy++ >> hiShift) | resHiMask);	// aggiunge il pattern reale
				}
			}

			rowCount -= 8;								// questi sono fatti....	

			pChr = pCpy;								// salva l'ultimo pattern
			
			if( rowCount >= 8 || rowCount == 0 )			// piccola ottimizzazione...
			{
				drv_setGraphPointer(graphInfo.curs.offset);	// ripiazza il puntatore
				drv_putGraphBlock(localBuf, pc->w);			// riscrive la colonna
	
															// resta sulla colonna attuale
				drv_setGraphCursPos(graphInfo.curs.x, graphInfo.curs.y);
			}
			else
			{
				validBuf = True;						// segnala che il buffer è già pieno...
			}
		}
		
		if( rowCount > 0 )								// se rimangono dei rotti
		{
			pCpy = pChr;								// continua....
			byteCount = pc->w;							// numero di pixel in altezza

			resMidMask = pc->maskBit << loShift;		// parte dai bits validi

			pbuf = localBuf;

			if( validBuf == True )						// piccola ottimizzazione sul caricamento
				validBuf = False;
			else
				drv_getGraphBlock(localBuf, byteCount);		// legge una fetta di riga (è l'ultima letta)

														// Scrive pattern lato MSB
			if( graphInfo.color.fgcolor == BLACK )		// se il colore deve essere nero
			{
				while( byteCount-- )
				{
					*pbuf &= resLoMask | resMidMask;	// toglie la seconda fettina
					*pbuf++ |= *pCpy++ << loShift;		// aggiunge il pattern reale
				}
			}
			else
			{
				while( byteCount-- )
				{
					*pbuf &= resLoMask | resMidMask;	// toglie la prima fettina
					*pbuf++ |= ~( (*pCpy++ << loShift) | resLoMask | resMidMask );		// aggiunge il pattern reale
				}
			}

			drv_setGraphPointer(graphInfo.curs.offset);		// ripiazza il puntatore
			drv_putGraphBlock(localBuf, pc->w);				// riscrive la fettina di riga


			if( ((graphInfo.curs.y & 0x07) + rowCount) > 8 ) // se non basta una passata...
			{
	
														// passa alla riga successiva
				drv_setGraphCursPos(graphInfo.curs.x, graphInfo.curs.y + rowCount);

				pCpy = pChr;								// parte dal primo pattern
				byteCount = pc->w;							// numero di pixel in larghezza
				pbuf = localBuf;

				resMidMask = pc->maskBit >>	hiShift;		// parte dai bits validi

				drv_getGraphBlock(localBuf, byteCount);		// legge la riga succ.
	
															// Scrive byte MSB pattern su lato LSB memoria
				if( graphInfo.color.fgcolor == BLACK )		// se il colore deve essere nero
				{
					while( byteCount-- )
					{
						*pbuf &= resHiMask | resMidMask;	// toglie la fettina centrale
						*pbuf++ |= *pCpy++ >> hiShift;		// aggiunge il pattern reale
					}
				}
				else
				{
					while( byteCount-- )
					{
						*pbuf &= resHiMask | resMidMask;	// toglie la fettina centrale
						*pbuf++ |= ~( (*pCpy++ >> hiShift) | resHiMask | resMidMask );		// aggiunge il pattern reale
					}
				}

				drv_setGraphPointer(graphInfo.curs.offset);	// ripiazza il puntatore
				drv_putGraphBlock(localBuf, pc->w);			// riscrive la riga
				rowCount = 0;								// anche questi sono fatti....	
			}
			pChr = pCpy;									// salva l'ultimo pattern
															// avanza del giusto
		}
		drv_setGraphCursPos(graphInfo.curs.x + pc->w, local_y);
	}

}


/* Spara un carattere a video in modalità grafica utilizzando
   il set di caratteri attualmente selezionato
*/
void drv_putForeChar(int c)
{
int   byteCount;
int   colCount;
int   colByte;
int   clx, cly;
int   x, y;
const ChrSet *pc;
const byte *pChr, *pCpy;
byte charPattern, bitTestH;
dword fgcolor;
dword bkcolor;

	pc = charset_getCurrent();				// piglia il set attuale
	
	if( pc == NULL )						// controlli di routine...
		return;	

	if( c < pc->firstChar )					// se sono troppo scarsi
		return;								// si danno poi una calmata

	if( c > (pc->firstChar + pc->numChar) )	// se esagerano...
		return;								// stanno poi tranquilli

	fgcolor = graphInfo.color.fgcolor;				// colore di primo piano
	bkcolor = graphInfo.color.bkcolor;				// colore di sfondo
	
	pChr = &pc->pSet[(c - pc->firstChar) * pc->blockSize];	// piglia il puntatore al bitmap

	drv_setCursDir(CURSDIR_DOWN);					// probabilmente faremo + colonne che righe 
	drv_setGraphCursPos(graphInfo.curs.x, graphInfo.curs.y);
	colCount = pc->w;								// numero di pixel in larghezza


									// molto probabilmente non c'è una convenienza "tangibile"
									// nel caricare un "pacco" di long-word e mascherare prima
									// e dopo come nel driver per il SED1335
									// quindi agiamo di "puxpixel" direttamente
		clx = 0;
		
		x = graphInfo.curs.x;
		y = graphInfo.curs.y;
	
		while( colCount >= 8 )							// se usano almeno 8 pixel
		{
			byteCount = 0;								// numero di pixel in altezza
			pCpy = pChr;								// parte dal primo pattern
			cly = 0;
			
			while( byteCount < pc->h )
			{
				charPattern = *pCpy;						// parte dal primo pattern
				bitTestH = 0x80;
	
				for(colByte=0; colByte<8; colByte++)
				{
					if( (charPattern & bitTestH) )			// se è da accendere....
						drv_putpixel(x+colByte+clx, y+cly, fgcolor);
	
					bitTestH >>= 1;
				}

				byteCount++;
				cly++;
				pCpy += pc->lineSize;				// prossima colonna di pattern..
			}
	
			pChr++;								// prossima colonna di pattern..
			clx += 8;
	
			colCount -= 8;							// questi sono fatti....	
													// passa alla colonna successiva
		}
	
		if( colCount > 0 )							// se rimangono dei rotti
		{
			byteCount = 0;							// numero di pixel in altezza
			pCpy = pChr;							// parte dal primo pattern
			cly = 0;
	
			while( byteCount < pc->h )
			{
				charPattern = *pCpy;						// parte dal primo pattern
				bitTestH = 0x80;
	
				for(colByte=0; colByte<8; colByte++)
				{
					if( (charPattern & bitTestH) )
						drv_putpixel(x+colByte+clx, y+cly, fgcolor);
	
					bitTestH >>= 1;
				}

				cly++;
				byteCount++;
				pCpy += pc->lineSize;				// prossima colonna di pattern..
			}
			pChr++;									// prossima colonna di pattern..
	
			colCount -= 8;							// questi sono fatti....	
													// passa alla colonna successiva
		}

		drv_setGraphCursPos(graphInfo.curs.x + pc->w, graphInfo.curs.y);
}




/* Spara una icona a video in modalità grafica utilizzando
*/
void drv_puticon(int __x, int __y, const void *__bitmap, int __w, int __h)
{
int   byteCount;
int   rowCount;
int	  local_y;
byte  localBuf[32], *pbuf;
const byte *pChr, *pCpy;

int   maskBit = 0xE0;			// succedanei dei set di caratteri
register int colCount = __w;					// larghezza = 23pixel

	pChr = (byte *) __bitmap;	
	local_y = __y;

	drv_setGraphCursPos(__x, __y);
	rowCount = __h;									// numero di pixel in altezza

	if( (graphInfo.curs.y & 0x07) == 0 )			// se il primo byte è allineato...
	{
		
		pCpy = pChr;								// parte dal primo pattern

		while( rowCount >= 8 )						// se usano almeno 8 pixel
		{
			byteCount = colCount;					// numero di pixel in larghezza
			pbuf = localBuf;
			drv_getGraphBlock(localBuf, byteCount);	// legge una riga

			if( graphInfo.color.fgcolor == BLACK )		// se il colore deve essere nero
			{
				while( byteCount-- )
					*pbuf++ = *pCpy++;				// aggiunge il pattern reale
			}
			else
			{
				while( byteCount-- )
					*pbuf++ = ~*pCpy++;				// aggiunge il pattern reale
			}

			drv_setGraphPointer(graphInfo.curs.offset);	// ripiazza il puntatore
			drv_putGraphBlock(localBuf, colCount);		// riscrive la colonna

			rowCount -= 8;							// questi sono fatti....
													// passa alla riga successiva
			drv_setGraphCursPos(graphInfo.curs.x, graphInfo.curs.y + 8);
		}

		if( rowCount > 0 )							// se rimangono dei rotti
		{
			byteCount = colCount;					// numero di pixel in altezza

			pbuf = localBuf;
			drv_getGraphBlock(localBuf, byteCount);	// legge una riga

			if( graphInfo.color.fgcolor == BLACK )		// se il colore deve essere nero
			{
				while( byteCount-- )
				{
					*pbuf &= maskBit;				// toglie la prima fettina
					*pbuf++ |= *pCpy++;				// aggiunge il pattern reale
				}
			}
			else
			{
				while( byteCount-- )
				{
					*pbuf &= maskBit;				// toglie la prima fettina
					*pbuf++ |= ~(*pCpy++ | maskBit);			// aggiunge il pattern reale
				}
			}

			drv_setGraphPointer(graphInfo.curs.offset);	// ripiazza il puntatore
			drv_putGraphBlock(localBuf, colCount);			// riscrive la colonna

			rowCount = 0;							// anche questi sono fatti....	
		}
													// avanza del giusto
		drv_setGraphCursPos(graphInfo.curs.x + colCount, local_y);
	}

}


#endif //DPYDRV_ENABLE_GRAPH_SUPPORT





void ST7565_delay(unsigned int p) 	// 1- 4us      ...2- 15us     ...5 - 31us
{                               	// 10- 57us    	...50- 269us
unsigned int i;               		// 100- 534us  	...500- 2,6ms
byte j; 							// 1000- 5.3ms 	...5000- 29ms
									// 10000- 56ms 	...30000- 170ms
									// 50000- 295ms	...60000- 345ms

	if( p == 1 )
	{
		return;
	}	
	for (i = 0; i < p; i++) 
	{
		for (j = 0; j < 7; j++)
		{
			asm("nop");
		}
	}
}


//////////////////////////////////////////////////////////////////////////////////
//
// Funzioni a super basso livello per la gestione del controller ST7565
//
//////////////////////////////////////////////////////////////////////////////////


void ST7565_cmd_wr(byte Command)
{
//	ST7565_waitIfBusy();

	clrbit(LCD_CS1);						// cs 
	clrbit(LCD_A0);							// command mode
	ST7565_delay(2);						// un attimo che i dati arrivino all'LCD

	SPI_WRITE(Command);

	ST7565_delay(2);
	setbit(LCD_CS1);						// cs

}

void ST7565_data_wr(byte Data)
{
	clrbit(LCD_CS1);						// cs 
	setbit(LCD_A0); 						// command mode
	ST7565_delay(2);						// un attimo che i dati arrivino all'LCD

	SPI_WRITE(Data);

	ST7565_delay(2);
	setbit(LCD_CS1);						// cs
	

#ifdef	DPYDRV_ENABLE_GRAPH_IMAGEBUF

//	ST7565_waitIfBusy();

	

#else
#endif

}


byte ST7565_data_rd(void)
{


/*dword portMask = P0_LCD_D0 | P0_LCD_D1 | P0_LCD_D2 | P0_LCD_D3 | P0_LCD_D4 | P0_LCD_D5 | P0_LCD_D6 | P0_LCD_D7;
dword portVal;
byte c;

	ST7565_waitIfBusy();
	PIN_SET(P0_LCD_CD);					// seleziona registro dati
	PIN_CLR(P0_LCD_CS);						// giu' il cs

	ST7565_delay(4);						// un attimo che i dati arrivino all'LCD

	PIN_INP(portMask);						// questi devono essere in input
	PIN_CLR1(P1_LCD_RD);					// giu' il read

	portVal = PIN_STATUS(portMask);			// test' tutti...

	c = portVal >> 8;						// porta giu il byte del port

	ST7565_delay(1);							// un attimo che i dati arrivino all'LCD
	PIN_SET1(P1_LCD_RD);					// su' il read
	PIN_SET(P0_LCD_CS);						// su' di nuovo il cs

*/
	return(0);	
}


void ST7565_waitIfBusy(void)
{

/* Secondo il manualozzo con un tempo di accesso >400ns non è necessario attendere il busy
*/
/*dword portMask = P0_LCD_D0 | P0_LCD_D1 | P0_LCD_D2 | P0_LCD_D3 | P0_LCD_D4 | P0_LCD_D5 | P0_LCD_D6 | P0_LCD_D7;
dword portVal;
static byte c;
word timeout = 300;

	do{
		PIN_CLR(P0_LCD_CD);						// seleziona registro comandi
		PIN_SET(P0_LCD_WR);					// giu' il read
		PIN_INP(portMask);						// questi devono essere in input

		ST7565_delay(1);						// un attimo che arrivino i dati dell'LCD

		PIN_CLR(P0_LCD_CS);						// giu' il cs
		PIN_CLR1(P1_LCD_RD);					// giu' il read

		ST7565_delay(1);						// un attimo che arrivino i dati dell'LCD

		portVal = PIN_STATUS(portMask);			// test' tutti...

		c = portVal >> 8;						// porta giu il byte del port
		
		PIN_SET1(P1_LCD_RD);						// su' il read
		PIN_SET(P0_LCD_CS);						// su' di nuovo il cs

		if( --timeout == 0 )					// a tutto c'è un limite....
			break;

	}while( c & 0x10 );
*/

	ST7565_delay(10);
}



#ifdef __cplusplus
}
#endif



//@@-----------------------------------------------------------------------@@//
//@@
//@@  $Source: /root/Confer/UC1601.CPP,v $
//@@  Note di revisione:
//@@
//@@  $Revision: 1.1 $
//@@  $Date: 2006/03/01 09:54:12 $
//@@
//@@  $Log: UC1601.CPP,v $
//@@
//@@  Revision 1.1  2005/09/03 20:54:58  b
//@@  Prima versione ricavata a partire dal ST7565
//@@
//@@
//@@  $Author: enrico $
//@@  $RCSfile: UC1601.CPP,v $
//@@  $Revision: 1.5 $
//@@  $State: Exp $
//@@  $Date: 2006/03/01 09:54:12 $
//@@  $Name:  $
//@@-----------------------------------------------------------------------@@//
//@@-----------------------------------------------------------------------@@//
//@@                                                                       @@//
//@@                            C O M E T A                                @@//
//@@                                                                       @@//
//@@-----------------------------------------------------------------------@@//

