/**
@file		Siosg2.h
@brief		Functions to manage the microntroller SPI.

@author		
@date		18/01/2011
@version	01.0
*/



//--------------------------------------------------------//
// Definizione delle costanti
//--------------------------------------------------------//

//--------------------------------------------------------//
// Definizione dei tipi
//--------------------------------------------------------//

//--------------------------------------------------------//
// Definizione delle classi 
//--------------------------------------------------------//

//--------------------------------------------------------//
// Definizione delle funzioni
//--------------------------------------------------------//
#ifndef _SIOSG2_H_
#define _SIOSG2_H_

#include "global.h"
#include "swtim.h"

#ifdef __cplusplus
extern "C" {
#endif

void sio_init_ch0(void);
void sio_init_ch1(void);
void sio_init_ch2(void);
void sio_init_ch3(void);

void sio_enable_rx_interrupt_ch0(byte priority);
void sio_enable_rx_interrupt_ch1(byte priority);
void sio_enable_rx_interrupt_ch2(byte priority);
void sio_enable_rx_interrupt_ch3(byte priority);

void sio_disable_rx_interrupt_ch0();
void sio_disable_rx_interrupt_ch1();
void sio_disable_rx_interrupt_ch2();
void sio_disable_rx_interrupt_ch3();

void sio_enable_tx_interrupt_ch0(byte priority);
void sio_enable_tx_interrupt_ch1(byte priority);
void sio_enable_tx_interrupt_ch2(byte priority);
void sio_enable_tx_interrupt_ch3(byte priority);

void sio_disable_tx_interrupt_ch0();
void sio_disable_tx_interrupt_ch1();
void sio_disable_tx_interrupt_ch2();
void sio_disable_tx_interrupt_ch3();


//-- Comadi da usare se gli interrup sono disabilitati -> polling sulla porta--
void sio_clocksel_ch0(int mode);
int sio_putc_ch0(int c);
int sio_getc_ch0(int filler);
int sio_busy_ch0(void);

void sio_clocksel_ch1(int mode);
int sio_putc_ch1(byte c);
int sio_getc_ch1(byte filler);
int sio_busy_ch1(void);
int sio_xfr_ch1(void *__dest, void *__source, int __size);

void sio_clocksel_ch2(int mode);
int sio_putc_ch2(int c);
int sio_getc_ch2(int filler);
int sio_busy_ch2(void);
int sio_xfr_ch2(void *__dest, void *__source, int __size);

void sio_clocksel_ch3(int mode);
int sio_putc_ch3(int c);
int sio_getc_ch3(int filler);
int sio_busy_ch3(void);
int sio_xfr_ch3(void *__dest, void *__source, int __size);

int sio_fill_ch3(int __filler, int __len);
int sio_write_ch3(void *__source, int __size);
int sio_read_ch3(void *__dest, int __size);
word sio_putw_ch3(word c);
word sio_getw_ch3(word c);


#ifdef __cplusplus
}
#endif

//--------------------------------------------------------//
// Definizione delle variabili
//--------------------------------------------------------//



#endif


//@@-----------------------------------------------------------------------@@//
//@@                                                                             
//@@  $Source: /root/../SIOSG2.H,v $
//@@  Note di revisione:                
//@@
//@@  $Revision: 1.2 $
//@@  $Date: 2008/03/14 13:35:55 $
//@@  	- Aggiunte definizioni per le nuove funzioni "sio_busy_ch0" 
//@@	  & "sio_clocksel_ch0"
//@@
//@@
//@@  $Log: SIOSG2.H,v $
//@@  Revision 1.1  2007/05/29 13:35:55  enrico
//@@  Inclusione nel progetto, copiata da proj "ConfTx"
//@@
//@@  Revision 1.1  2006/10/03 13:08:09  giovanni
//@@  *** empty log message ***
//@@
//@@
//@@  $Author: enrico $
//@@  $RCSfile: SIOSG2.H,v $
//@@  $Revision: 1.2 $
//@@  $State: Exp $
//@@  $Date: 2008/03/14 13:35:55 $
//@@  $Name:  $
//@@-----------------------------------------------------------------------@@//
//@@-----------------------------------------------------------------------@@//
//@@                                                                       @@//
//@@                            C O M E T A                                @@//
//@@                                                                       @@//
//@@-----------------------------------------------------------------------@@//

