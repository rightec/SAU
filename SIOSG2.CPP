/**
@file		Siosg2.h
@brief		Functions to manage the microntroller SPI.

@author		
@date		18/01/2011
@version	01.0
*/


#include "siosg2.h"

//-------------------------------------------------------
// Variabili globali utili a tutti i moduli
//-------------------------------------------------------

//-------------------------------------------------------
// Funzioni statiche utili solo a questo modulo
//-------------------------------------------------------

//-------------------------------------------------------
// Costanti statiche utili solo a questo modulo
//-------------------------------------------------------
									// Maschere flags per registri CSIM
#define CSIM_CSIE		0x80
#define CSIM_TRMD		0x40
#define CSIM_CCLN16		0x20			// char len = 16bit
#define CSIM_CCLN8		0x00			// char len = 8bit
#define CSIM_MSBFIRST	0x00
#define CSIM_LSBFIRST	0x10
#define CSIM_CSIT		0x08
#define CSIM_AUTO		0x04
#define CSIM_xxxx		0x02
#define CSIM_CSOT		0x01

									// Maschere flags per registri CSIM
#define CSIS_SPIMODE0	0x18
#define CSIS_SPIMODE1	0x08
#define CSIS_SPIMODE2	0x10
#define CSIS_SPIMODE3	0x00

#define CSIS_CLKDIV4	0x00
#define CSIS_CLKDIV128	0x01
#define CSIS_CLKBRG		0x02
#define CSIS_CLKDIV8	0x03
#define CSIS_CLKDIV16	0x04
#define CSIS_CLKDIV32	0x05
#define CSIS_CLKDIV64	0x06
#define CSIS_CLKEXT		0x07


//-------------------------------------------------------
// Variabili statiche utili solo a questo modulo
//-------------------------------------------------------

/* Inizializza il modulo seriali sincrone ad uso e consumo della scheda
*/
void sio_init_ch0(void)
{

							// Configurazione CSI0 
	CB0CTL1 = 0x1A;					// SCK = fsys/8 = 2.5Mhz
	CB0CTL2 = 0x00;					// 8bit data
	CB0CTL0 = 0xE1;					// abilita TX+RX + LSB first + enable clock

	CB0TIC = 0x47;					// imposta irq off
	CB0RIC = 0x47;					// imposta irq off

	//-- SIBO --
	_PMC40 = 1;
	_PFC40 = 0;

	//-- S0BO --
	_PMC41 = 1;
	_PFC41 = 0;

	//-- SCKO --
	_PMC42 = 1;

	
	//PMC4 |= 0x07;
	//PFC4 |=0x00;

}
void sio_init_ch1(void)
{
	
							// Configurazione CSI1 
	CB1CTL1 = 0x01;					// SCK = fsys/4 = 5Mhz
	CB1CTL2 = 0x00;					// 8bit data
	CB1CTL0 = 0xE1;					// abilita TX+ RX + MSB first + enable clock

	CB1TIC = 0x47;					// imposta irq off
	CB1RIC = 0x47;					// imposta irq off

	// SIB1
	_PMC97 = 1;
	_PFCE97 = 0;
	_PFC97 = 1;

	// SOB1
	_PMC98 = 1;
	_PFC98 = 1;

	//SCKB1
	_PMC99 = 1;
	_PFC99 = 1;
	
	PMC9H |= 0x03;
	PMC9L |= 0x80;
	
	PFC9H |= 0x03;
	PFC9L |= 0x80;

}


void sio_init_ch2(void)
{
	
	CB2CTL0 = 0x00;					// Configurazione CSI1 
	CB2CTL1 = 0x01;					// SCK = fsys/4 = 5Mhz
	CB2CTL2 = 0x00;					// 8bit data
	CB2CTL0 = 0xE1;					// abilita TX+RX + MSB first + enable clock

	CB2TIC = 0x47;					// imposta irq off
	CB2RIC = 0x47;					// imposta irq off
							// Configurazione CSI2: non disponibile xchè utilizzata come I2C

	
	PMC5 |= 0x38;
	PFCE5 &= 0x07;
	PFC5 &= 0x07;

}

void sio_init_ch3(void)
{
	CB3CTL0 = 0x00;
							// Configurazione CSI1 
	CB3CTL1 = 0x18 | 0x02;					// SCK = fsys/8 = 2.5Mhz
	CB3CTL2 = 0x00;					// 8bit data
	CB3CTL0 = 0xE1;					// abilita TX+RX + MSB first + enable clock

	CB3TIC = 0x47;					// imposta irq off
	CB3RIC = 0x47;					// imposta irq off
					
	
	_PMC910 = 1;
	_PFC910 = 1;
		
	_PMC911 = 1;
	_PFC911 = 1;

	_PMC912 = 1;
	_PFC912 = 1;
	
}


void sio_enable_rx_interrupt_ch0(byte priority)
{
	priority &= 0x0F;
	CB0RIC = 0x00 | priority;					// imposta irq off
	
}
void sio_enable_rx_interrupt_ch1(byte priority)
{
	priority &= 0x0F;
	CB1RIC = 0x00 | priority;					// imposta irq off	
}

void sio_enable_rx_interrupt_ch2(byte priority)
{
	priority &= 0x0F;
	CB2RIC = 0x00 | priority;					// imposta irq off
}
void sio_enable_rx_interrupt_ch3(byte priority)
{
	priority &= 0x0F;
	CB3RIC = 0x00 | priority;					// imposta irq off
}

void sio_disable_rx_interrupt_ch0()
{
	CB0RIC = 0x47;	
}
void sio_disable_rx_interrupt_ch1()
{
	CB1RIC = 0x47;	
}
void sio_disable_rx_interrupt_ch2()
{
	CB2RIC = 0x47;	
}
void sio_disable_rx_interrupt_ch3()
{
	CB3RIC = 0x47;	
}

void sio_enable_tx_interrupt_ch0(byte priority)
{
	priority &= 0x0F;
	CB0TIC = 0x00 | priority;
}
void sio_enable_tx_interrupt_ch1(byte priority)
{
	priority &= 0x0F;
	CB1TIC = 0x00 | priority;
}
void sio_enable_tx_interrupt_ch2(byte priority)
{
	priority &= 0x0F;
	CB2TIC = 0x00 | priority;
}

void sio_enable_tx_interrupt_ch3(byte priority)
{
	priority &= 0x0F;
	CB3TIC = 0x00 | priority;
}


void sio_disable_tx_interrupt_ch0()
{
	CB0TIC = 0x47;
}

void sio_disable_tx_interrupt_ch1()
{
	CB1TIC = 0x47;
}

void sio_disable_tx_interrupt_ch2()
{
	CB2TIC = 0x47;
}
void sio_disable_tx_interrupt_ch3()
{
	CB3TIC = 0x47;
}




void sio_clocksel_ch0(int mode)
{
	while( _CB0TSF );					// aspetta che esca tutto
}

int sio_putc_ch0(int c)
{
int d;

	while( _CB0TSF );					// aspetta che esca tutto
	
	d = CB0RXL;

	CB0TXL = c;						// spara il nuovo dato

	
	return( d );
}


int sio_busy_ch0(void)
{
	return( _CB0TSF );					// segnale di busy
}



int sio_getc_ch0(int filler)
{

	while( _CB0TSF );					// wait last xfr pending

	CB0TXL = filler;					// spara il nuovo dato

	while( _CB0TSF );					// wait xfr complete..

	return( CB0RXL );      			// autoreturn inp data 
}


void sio_clocksel_ch1(int mode)
{
	while( _CB1TSF );					// aspetta che esca tutto
}


int sio_putc_ch1(byte c)
{
byte d;

	while( _CB1TSF );					// aspetta che esca tutto
	
	d = CB1RXL;

	CB1TXL = c;						// spara il nuovo dato

	return( d );
}


int sio_busy_ch1(void)
{
	return( _CB1TSF );					// segnale di busy
}


int sio_getc_ch1(byte filler)
{
	while( _CB1TSF );					// wait last xfr pending

	CB1TXL = filler;						// spara il nuovo dato

	while( _CB1TSF );					// wait xfr complete..

	return( CB1RXL );      				// autoreturn inp data 
}

int sio_xfr_ch1(void *__dest, void *__source, int __size)
{
register byte *pd = (byte *) __dest;
register byte *ps = (byte *) __source;
register int n;

	while( _CB1TSF );						// aspetta che esca tutto

	for(n=0; n< __size; )
	{
		CB1RXL = *ps++;					// spara il nuovo dato
		
		n++;								// centralize increment
		
		while( _CB1TSF );					// wait xfr complete..

		*pd++ = CB1RXL;					// incamera il nuovo ricevuto
	}

	return( n );      						// autoreturn inp len
}


void sio_clocksel_ch2(int mode)
{
	while( _CB2TSF );					// aspetta che esca tutto
}


int sio_putc_ch2(int c)
{
int d;

	while( _CB2TSF );					// aspetta che esca tutto
	
	d = CB2RXL;

	CB2TXL = c;						// spara il nuovo dato

	return( d );
}


int sio_busy_ch2(void)
{
	return( _CB2TSF );					// segnale di busy
}


int sio_getc_ch2(int filler)
{
	while( _CB2TSF );					// wait last xfr pending

	CB2TXL = filler;						// spara il nuovo dato

	while( _CB2TSF );					// wait xfr complete..

	return( CB2RXL );      				// autoreturn inp data 
}

int sio_xfr_ch2(void *__dest, void *__source, int __size)
{
register byte *pd = (byte *) __dest;
register byte *ps = (byte *) __source;
register int n;

	while( _CB2TSF );						// aspetta che esca tutto

	for(n=0; n< __size; )
	{
		CB2RX = *ps++;					// spara il nuovo dato
		
		n++;								// centralize increment
		
		while( _CB2TSF );					// wait xfr complete..

		*pd++ = CB2RXL;					// incamera il nuovo ricevuto
	}

	return( n );      						// autoreturn inp len
}


void sio_clocksel_ch3(int mode)
{
	while( _CB3TSF );					// aspetta che esca tutto
}


int sio_putc_ch3(int c)
{
int d;
	
	while( _CB3TSF );					// aspetta che esca tutto
	
	d = CB3RXL;

	CB3TXL = c;						// spara il nuovo dato

	return( d );
}


int sio_busy_ch3(void)
{
	return( _CB3TSF );					// segnale di busy
}


int sio_getc_ch3(int filler)
{
	while( _CB3TSF );					// wait last xfr pending

	CB3TXL = filler;						// spara il nuovo dato

	while( _CB3TSF );					// wait xfr complete..

	return( CB3RXL );      				// autoreturn inp data 
}

int sio_xfr_ch3(void *__dest, void *__source, int __size)
{
register byte *pd = (byte *) __dest;
register byte *ps = (byte *) __source;
register int n;

	while( _CB3TSF );						// aspetta che esca tutto

	for(n=0; n< __size; )
	{
		CB3RXL = *ps++;					// spara il nuovo dato
		
		n++;								// centralize increment
		
		while( _CB3TSF );					// wait xfr complete..

		*pd++ = CB3RXL;					// incamera il nuovo ricevuto
	}

	return( n );      						// autoreturn inp len
}

int sio_fill_ch3(int __filler, int __len)
{
register int n;

	while( _CB3TSF );						// wait last xfr pending

	for(n=__len; n>0; )
	{
		CB3TXL = __filler;					// spara il nuovo dato
		
		n--;								// centralize decrement
		
		while( _CB3TSF );					// wait xfr complete..
	}

	_CB3TSF = 0;							// must reset !!

	return( n );      						// autoreturn inp len
}


int sio_write_ch3(void *__source, int __size)
{
register byte *ps = (byte *) __source;
register int n;

	while( _CB3TSF );						// wait last xfr pending

	for(n=0; n< __size; )
	{
		CB3TXL = *ps++;						// spara il nuovo dato
		
		n++;								// centralize increment
		
		while( _CB3TSF );					// wait xfr complete..
	}

	_CB3TSF = 0;							// must reset !!

	return( n );      						// autoreturn inp len
}


int sio_read_ch3(void *__dest, int __size)
{
register byte *pd = (byte *) __dest;
register int n;

	while( _CB3TSF );						// wait last xfr pending

	for(n=0; n< __size; )
	{
		CB3TXL = 0xFF;//n;							// spara il nuovo dato
		
		n++;								// centralize increment
		
		while( _CB3TSF );					// wait xfr complete..

		*pd++ = CB3RXL;						// incamera il nuovo ricevuto
	}

	_CB3TSF = 0;							// must reset !!

	return( n );      						// autoreturn inp len
}


word sio_putw_ch3(word c)
{
	sio_putc_ch3(c);		// put word from low byte
	
	c >>= 8;				// only high side

	sio_putc_ch3(c);		// to high byte

	return( c );
}

word sio_getw_ch3(word c)
{
word d, e;

	d = sio_getc_ch3(c);			// get word from low byte
	
	c >>= 8;					// only high side

	e = sio_getc_ch3(c);		// to high byte
	
	d += e << 8;				// addendum

	return( d );
}




//@@-----------------------------------------------------------------------@@//
//@@
//@@  $Source: /root/../SIOSG2.CPP,v $
//@@  Note di revisione:
//@@
//@@  $Revision: 1.1 $
//@@  $Date: 2007/05/29 13:35:55 $
//@@  			XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//@@
//@@  $Log: SIOSG2.CPP,v $
//@@  Revision 1.1  2007/05/29 13:35:55  enrico
//@@  Inclusione nel progetto, copiata da proj "ConfTx"
//@@
//@@  Revision 1.1  2006/10/03 13:08:09  giovanni
//@@  *** empty log message ***
//@@
//@@
//@@  $Author: enrico $
//@@  $RCSfile: SIOSG2.CPP,v $
//@@  $Revision: 1.2 $
//@@  $State: Exp $
//@@  $Date: 2007/05/29 13:35:55 $
//@@  $Name:  $
//@@-----------------------------------------------------------------------@@//
//@@-----------------------------------------------------------------------@@//
//@@                                                                       @@//
//@@                            C O M E T A                                @@//
//@@                                                                       @@//
//@@-----------------------------------------------------------------------@@//

