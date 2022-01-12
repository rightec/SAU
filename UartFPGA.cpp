/**
@file		UartFPGA.cpp
@brief		Classes to manage the fpga UART.

@author		Enrico
@date		03/09/2011
@version	01.1
*/


#include "UartAbstract.h"
#include "UartNEC.h"
#include "UartFPGA.h"

//
// Definizione della classe UartFPGA
//


/**
Class constructor
@param device UART  device
*/
UartFPGA::UartFPGA(volatile uart_fpga_t *pRegUart, UARTParity __parity, 	UARTFrameBits __frame, 
										UARTStopBits __stopbit, UARTBaudRate __baudrate, UARThwFlwCnt __hwFlwCtl,
										int __rxbuf_size, int __txbuf_size) : 
											UartDeviceAbstract(__rxbuf_size, __txbuf_size)
{

			// se i buffer tx & rx non sono buoni 
	if( m_rxbuf.isvalid() && m_txbuf.isvalid() )
	{

		m_pRegUart = pRegUart;			// mette da parte il puntatore al device fisico

		HwInit();						// inizializza e attiva la periferica hw

		// + Inizilizzazioni di base
		// + Accende la periferica UART ed abilita le funzioni di ricezione e trasmissione
										// non disponibili 
		SetParity(__parity);
		SetFrameBits(__frame);
		SetStopBits(__stopbit);
										// unico parametro gestibile
		SetBaudRate(__baudrate, true);
		SetHwFlwCtl(__hwFlwCtl, true);
		TurnOn();
	}
	else
	{
		m_pRegUart = NULL;			// inutile proseguire
	}

}

/**
Class desctructor
*/
UartFPGA::~UartFPGA()
{
	TurnOff();

				// disattiva tx+rx
	m_pRegUart->CTL0 = UART_CTL0_PWR;

}

/**
Enables the UART device
*/
void UartFPGA::TurnOn()
{	
	if( m_pRegUart == NULL )
		return;

	m_pRegUart->CTL1 |= UART_CTL1_TXF_CLR | UART_CTL1_RXF_CLR;	// clear fifo

	usec_delay(100);
									// via il clear fifos
	m_pRegUart->CTL1 &= ~(UART_CTL1_TXF_CLR | UART_CTL1_RXF_CLR);

	usec_delay(100);

				// attiva tx+rx
	m_pRegUart->CTL0 = UART_CTL0_PWR | UART_CTL0_RXE | UART_CTL0_TXE;

	UartDeviceAbstract::TurnOn();
}

/**
Disables UART device.
*/
void UartFPGA::TurnOff()
{
	if( m_pRegUart == NULL )
		return;

				// disattiva tx+rx, lascia powered
	m_pRegUart->CTL0 = UART_CTL0_PWR;
				
	m_pRegUart->CTL1 |= UART_CTL1_TXF_CLR | UART_CTL1_RXF_CLR;	// clear fifo

	usec_delay(100);
									// via il clear fifos
	m_pRegUart->CTL1 &= ~(UART_CTL1_TXF_CLR | UART_CTL1_RXF_CLR);
	

	UartDeviceAbstract::TurnOff();
}


/**
Sets the UART hw defaults values.
*/
void UartFPGA::HwInit()
{
	if( m_pRegUart == NULL )
		return;

	m_pRegUart->CTL0 = 0x10;

	m_pRegUart->CTL0 = 0;			// reset modulo

	usec_delay(100);
									// power al modulo
	m_pRegUart->CTL0 = UART_CTL0_PWR;

	usec_delay(100);
	
	m_pRegUart->OPT0 = 0x00;		// reset modalità strane

	m_pRegUart->CTL1 = UART_CTL1_FLW_ENA;		// attiva controllo di flusso

									// se non c'è il marker di periferica giusto ..
	if( m_pRegUart->marker_f4cc != 0xF4CC )
	{
		m_pDebug = m_pRegUart;		// ne fa una copia per debug
		m_pRegUart = NULL;			// reset pointer .. non puo' andare in giro a fare porcellate
	}

}

volatile long cntTxTot;

/**
Sets the UART hw defaults values.
*/
void UartFPGA::HwTransfer()
{

	//if( m_pRegUart == NULL )		// disabilitazione hw
		//return;

	//if( m_Enabled == false )		// disabilitazione softw
		//return;

	while( !m_txbuf.isempty_isr() )		// se ho dati da inviare ...
	{
		if( (m_pRegUart->STR2 & UART_STR2_TFFL) == UART_STR2_TFFL )	// se la fifo tx è piena ...
			break;					// c'è poco da fare ...

		m_pRegUart->TXD = m_txbuf.getc_isr();		// gonfia la fifo ...
	}

	while( (m_pRegUart->STR1 & UART_STR1_RFFE) == 0 )		// fino a che la fifo rx non è vuota ...
	{
		if(!m_rxbuf.isfull_isr())
		{
			m_rxbuf.putc_isr(m_pRegUart->RXD);				// scarica ... alla peggio rolla 

		}else
		{
			SetReceptionError(OverrunError);
			break;
		}
	}
	if(m_pRegUart->STR1 & UART_STR1_RFERW )
	{
		SetReceptionError(OverrunRxFifo);
		m_pRegUart->CTL1 |= (UART_CTL1_RXF_CLR | UART_CTL1_TXF_CLR);			// clear fifo
		m_pRegUart->CTL1 &= ~(UART_CTL1_RXF_CLR | UART_CTL1_TXF_CLR);
	}
	if(m_pRegUart->STR2 & UART_STR2_TFERW )
	{
		SetReceptionError(OverrunTxFifo);
		m_pRegUart->CTL1 |= (UART_CTL1_RXF_CLR | UART_CTL1_TXF_CLR);			// clear fifo
		m_pRegUart->CTL1 &= ~(UART_CTL1_RXF_CLR | UART_CTL1_TXF_CLR);
	}
}



/**
Set the buad rate. 
@param baudrate
@param forcemodify if true the property value is overwritten also if equal to the previous one.
*/
bool UartFPGA::SetBaudRate(UARTBaudRate baudrate, bool forcemodify)
{
	if( m_pRegUart == NULL )
		return false;

	if( UartDeviceAbstract::SetBaudRate(baudrate, forcemodify) ) // se proprio va settato ...
	{
		byte oldCTL1 = m_pRegUart->CTL1;
		
		switch(m_BaudRate)
		{
			case BR1200:

				break;
			case BR2400:

				break;
			case BR4800:

				break;
			case BR9600:	// real baud: 9635, error(%): +0.37%
				m_pRegUart->CTL1 = 5;
				m_pRegUart->CTL2 = 87;//86; 
				break;
			case BR19200:	// real baud: 19 380, error(%): +0.94%
				m_pRegUart->CTL1 = 4;
				m_pRegUart->CTL2 = 51; 
				break;
			case BR38400:	// real baud: 39 070, error(%): +1.75%
				m_pRegUart->CTL1 = 4;
				m_pRegUart->CTL2 = 25; 
				break;
			case BR57600:
				// real baud: 56 800, error(%): -1,4%
				//m_pRegUart->CTL1 = 4;
				//m_pRegUart->CTL2 = 17; 
				// real baud: 57 480, error(%): -0,2%
				m_pRegUart->CTL1 = 6;
				m_pRegUart->CTL2 = 12; 
				break;
			case BR76800:
				break;	
			case BR115200:	// real baud: 116 300, error(%): +0.96% 
				m_pRegUart->CTL1 = 7;
				m_pRegUart->CTL2 = 5; 
				break;
			case BR230400:	// real baud: 238 095, error(%): +3.4%
				m_pRegUart->CTL1 = 2;
				m_pRegUart->CTL2 = 6; 
				break;
			case BR460800:	// real baud: 454 545, error(%): -1.36% 
				m_pRegUart->CTL1 = 3;
				m_pRegUart->CTL2 = 2; 					
				break;
		}

			// ripristina solo il bit di abilitazione del controllo flusso
			// con cts/rts
		if( oldCTL1 & UART_CTL1_FLW_ENA )
			m_pRegUart->CTL1 |= UART_CTL1_FLW_ENA;
		
	}

	return true;
}

bool UartFPGA::SetHwFlwCtl(UARThwFlwCnt flwCtl, bool forcemodify)
{
	if( m_pRegUart == NULL )
		return false;

	if (UartDeviceAbstract::SetHwFlwCtl( flwCtl, forcemodify ))
	{
		switch (m_hwFlwCnt)
		{
			case hwFlwCtl_None:
					m_pRegUart->CTL1 &= ~UART_CTL1_FLW_ENA;
				break;
			case hwFlwCtl_CtsRts:
					m_pRegUart->CTL1 |= UART_CTL1_FLW_ENA;
				break;
		}
	}
	
	return true;
}

























