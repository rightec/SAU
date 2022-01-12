/**
@file		UartNEC.cpp
@brief		Classes to manage the microntroller UART.

@author		Enrico
@date		18/01/2011
@version	01.1
*/


#include "UartAbstract.h"
#include "UartNEC.h"

//
// Definizione della classe UartNEC
//


/**
Class constructor
@param device UART  device
*/
UartNEC::UartNEC(volatile uart_nec_t *pRegUart, UARTParity __parity, 	UARTFrameBits __frame, 
										UARTStopBits __stopbit, UARTBaudRate __baudrate,
										int __rxbuf_size, 
										int __txbuf_size) : UartDeviceAbstract(__rxbuf_size, __txbuf_size)

{

			// se i buffer tx & rx non sono buoni 
	if( m_rxbuf.isvalid() && m_txbuf.isvalid() )
	{
		m_pRegUart = pRegUart;			// mette da parte il puntatore al device fisico

		HwInit();						// inizializza e attiva la periferica hw
			
		// + Inizilizzazioni di base
		// + Accende la periferica UART ed abilita le funzioni di ricezione e trasmissione
		TurnOff();

										// tutti gestiti, li usiamo
		SetParity(__parity, true);
		SetFrameBits(__frame, true);
		SetStopBits(__stopbit, true);
		SetBaudRate(__baudrate, true);

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
UartNEC::~UartNEC()
{
	TurnOff();

}


/**
Sets the UART hw defaults values.
*/
void UartNEC::HwInit()
{
	m_pRegUart->STR = 0x00;

	while(m_pRegUart->STR & 0x80);

	m_pRegUart->CTL0 = 0x10;
	m_pRegUart->OPT0 = 0x14;
}


/**
Set the Parity of the UART protocol. 
@param parity king of parity
@param forcemodify if true the property value is overwritten also if equal to the previous one.
*/
bool UartNEC::SetParity(UARTParity parity, bool forcemodify)
{

	if( UartDeviceAbstract::SetParity(parity, forcemodify) ) 
	{
		m_pRegUart->CTL0 &= 0xf3;
		
		switch(m_Parity)
		{
			case ParityNone:
				m_pRegUart->CTL0 |= 0x00;
				break;
			case ParityZero:
				m_pRegUart->CTL0 |= 0x04;
				break;
			case ParityOdd:
				m_pRegUart->CTL0 |= 0x08;
				break;
			case ParityEven:
				m_pRegUart->CTL0 |= 0x0c;
				break;
		}
	}
	return true;

}

/**
Set the number of bits per frame. 
@param framebits number of bit per frame
@param forcemodify if true the property value is overwritten also if equal to the previous one.
*/
bool UartNEC::SetFrameBits(UARTFrameBits framebits, bool forcemodify)
{

	if( UartDeviceAbstract::SetFrameBits(framebits, forcemodify) ) 
	{
		m_pRegUart->CTL0 &= 0xfd;
		
		switch(m_FrameBits)
		{
			case Frame7Bits:
				m_pRegUart->CTL0 |= 0x00;
				break;
			case Frame8Bits:
				m_pRegUart->CTL0 |= 0x02;
				break;
		}
	}
	return true;
}

/**
Set the number of stop bits. 
@param stopbits number of stop bits
@param forcemodify if true the property value is overwritten also if equal to the previous one.
*/
bool UartNEC::SetStopBits(UARTStopBits stopbits, bool forcemodify)
{

	if( UartDeviceAbstract::SetStopBits(stopbits,forcemodify) ) 
	{
		m_pRegUart->CTL0 &= 0xfe;	
		
		switch(m_StopBits)
		{
			case Stop1Bits:
				m_pRegUart->CTL0 |= 0x00;
				break;
			case Stop2Bits:
				m_pRegUart->CTL0 |= 0x01;
				break;
		}
	}
	return true;
}


/**
Set the buad rate. 
@param baudrate
@param forcemodify if true the property value is overwritten also if equal to the previous one.
*/
bool UartNEC::SetBaudRate(UARTBaudRate baudrate, bool forcemodify)
{

	if( UartDeviceAbstract::SetBaudRate(baudrate, forcemodify) ) 
	{
		switch(m_BaudRate)
		{
			case BR1200:
				m_pRegUart->CTL1 = 0x06;
				m_pRegUart->CTL2 = 0x82;
				break;
			case BR2400:
				m_pRegUart->CTL1 = 0x05;
				m_pRegUart->CTL2 = 0x82;
				break;
			case BR4800:
				m_pRegUart->CTL1 = 0x04;
				m_pRegUart->CTL2 = 0x82;
				break;
			case BR9600:
				m_pRegUart->CTL1 = 0x03;
				m_pRegUart->CTL2 = 0x82;
				break;
			case BR19200:
				m_pRegUart->CTL1 = 0x02;
				m_pRegUart->CTL2 = 0x82;
				break;
			case BR38400:
				m_pRegUart->CTL1 = 0x01;
				m_pRegUart->CTL2 = 0x82;
				break;
			case BR76800:
				m_pRegUart->CTL1 = 0x00;
				m_pRegUart->CTL2 = 0x82;
				break;
			case BR115200:
				m_pRegUart->CTL1 = 0x00;
				m_pRegUart->CTL2 = 0x57; // error (%) = -0.2234%
				break;
			case BR230400:
				m_pRegUart->CTL1 = 0x00;
				m_pRegUart->CTL2 = 0x2B; // error (%) = +0.937% 
				break;
			case BR460800:
				break;
		}
	}

	return true;
}



