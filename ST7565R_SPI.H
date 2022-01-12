//---------------------------------------------------------------------------//
//                                                                           //
//                           C O M E T A                                     //
//                                                                           //
/**
@file		ST7565R_SPI.h
@brief		driver controller ST7565R used in SPI mode

@author		
@date		07/07/2011
@version	01.0
*/

#ifndef __ST7565R_SPI_H__
#define __ST7565R_SPI_H__

//--------------------------------------------------------//
// Definizione delle costanti
//--------------------------------------------------------//
										/* Alias Segnali Display */
#define outp(data, DirReg)	(DirReg=data)					// solo per compatibilità
#define inp(port)			(port)							// solo per compatibilità
#define setbit(bit)	(bit=1)							// solo per compatibilità
#define clrbit(bit)	(bit=0)							// solo per compatibilità
#define bit_is_set(dataport, bit)	((dataport & (1 << bit)))

#define LCD_DATAPORT									// gLCD Data Port

#define LCD_DATAOUT        LCD_DATAPORT 					// gLCD Data Port
#define LCD_DATADIR        									// gLCD Data Direction Port
#define LCD_DATAIN         LCD_DATAPORT						// gLCD Data Pin Port

#define LCD_BIT_BUSY       7     							// Busy Flag  : 1=active
#define LCD_BIT_ADC        6     							// Adc Flag  : 1=active
#define LCD_BIT_ONOFF      5     							// OnOff Flag : 1=on,0=off
#define LCD_BIT_RESET      4     							// Reset Flag : 1=active

#define LCD_CONTROL        0 								// gLCD Control Port
#define LCD_CONTDIR        0  								// gLCD Control Direction
#define LCD_CONTIN         0 								// gLCD Control Pin Port

#define LCD_BACKLT         Pin_EN_LCD_LED 					//BackLight       : 1=active
#define LCD_CS2            									//Chip Select 2   : 1=active
#define LCD_CS1            PinLCD_CS1						//Chip Select 1   : 1=active
#define LCD_A0             PinLCD_A0						//0: command,	1: data
#define LCD_ENABLE           								//Enable          : 1=active
#define LCD_RW             PinLCD_RD						//Read       	  : 0=read
#define LCD_WR             PinLCD_WR						//Write       	  : 0=write
#define LCD_RS               								//Register select : 1=data reg., 0=instr. reg.
#define LCD_RESET          PinLCD_RST						//Reset           : 0=active

#define LCD_PWM     										// ppg/pwm per la regolazione del contrasto


  															// modalità di scrittura per la putimage
#define	COPY_MODE			0								// copia di brutto
#define	XOR_MODE			1								// 
#define	OR_MODE				2
#define	AND_MODE			3

															// Limiti del display
#define MAX_DISP_X			128								// pixel asse X del display in uso
#define MAX_DISP_Y			64								// pixel asse Y del display in uso

#define MAX_TEXT_COL		MAX_DISP_X/6					// numero max. di colonne
#define MAX_TEXT_ROW		MAX_DISP_Y/8					// numero max. di righe

#define SPI_WRITE(data)	(sio_putc_ch1(data))
															// Modalità di configurazione
#if !defined(__VIDEOMODE)
#define __VIDEOMODE

#define 	TEXT_MODE			0x01
#define 	TEXT_RAMCHGEN_MODE	0x02
#define 	GRAPH_MODE			0x04
#define 	COLOR_MODE			0x10
#define 	ROTATE_MODE			0x20	// visualizzazione ruotata di 180gradi

#endif


															// Comandi per il controller
#define CMD_DISPLAY_ON			0xAF
#define CMD_DISPLAY_OFF			0xAE
#define CMD_DISPLAY_SETROW      0xB0
#define CMD_DISPLAY_SET_YH      0x10
#define CMD_DISPLAY_SET_YL      0x00
#define CMD_DISPLAY_START_LINE	0xC0


//--------------------------------------------------------//
// Definizione dei tipi
//--------------------------------------------------------//

						// dati per il singolo lcd controller
struct SingleLcdCtrl{
	word	x_onWrite;					// coordinata x assoluta (non relativa al singolo controller)
	word	x_onRead;					// coordinata x assoluta (non relativa al singolo controller)
	word	y;

	byte	x_cs1;						// coordinata x controller su cs1	(0..64)
	byte	y_cs1;						// coordinata y controller su cs1	(0..64)

	byte 	dummy;						// necessita del dummy read su cs1
};

typedef struct SingleLcdCtrl SingleLcdCtrl;

//--------------------------------------------------------//
// Definizione delle classi 
//--------------------------------------------------------//

//--------------------------------------------------------//
// Definizione delle funzioni
//--------------------------------------------------------//

#ifdef __cplusplus
extern "C" {
#endif


void ST7565_delay(unsigned int p);
void ST7565_startLine(byte start);
void ST7565_cmd_wr(byte Command);
void ST7565_data_wr(byte Data);
byte ST7565_data_rd(void);
void ST7565_waitIfBusy(void);
void ST7565_waitIfReset(void);
void ST7565_roll(void);




#ifdef __cplusplus
}
#endif

#endif

//--------------------------------------------------------//
// Definizione delle variabili
//--------------------------------------------------------//






//@@-----------------------------------------------------------------------@@//
//@@                                                                             
//@@  $Source: \\repos/Confer/ST7565.H,v $
//@@  Note di revisione:                
//@@
//@@  $Log: UC1601.H,v $
//@@  Revision 1.1  2005/09/03 20:54:59  b
//@@  Prima versione funzionante sulle schede conference
//@@
//@@
//@@  $Author: b $
//@@  $RCSfile: UC1601.H,v $
//@@  $Revision: 1.1 $
//@@  $State: Exp $
//@@  $Date: 2005/09/03 20:54:59 $
//@@  $Name:  $
//@@-----------------------------------------------------------------------@@//
//@@-----------------------------------------------------------------------@@//
//@@                                                                       @@//
//@@                            C O M E T A                                @@//
//@@                                                                       @@//
//@@-----------------------------------------------------------------------@@//

