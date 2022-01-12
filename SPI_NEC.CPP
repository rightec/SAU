/**
@file		SPI_NEC.CPP
@brief		Classes to manage SPI communication

@author		Nicola Molinazzi
@date		28/09/2011
@version	01.00
*/


#include "SPI_NEC.h"

SPI_NEC::SPI_NEC(volatile spi_nec_t *__pReg):SPI_Abstract()
{
	m_pReg = NULL;
	
	if(__pReg != NULL)
	{
		m_pReg = __pReg;
		
		setPowerEnable(false);
		
		setTransmissionEnable(false);
		
		setReceptionEnable(false);	
		
		setTransferModeDirection(tmd_MSB_first, true);
		
		setTransferModeSpecification(tms_SINGLE_transfer, true);
		
		setCommunicationType(ct_CLK_high_sampleOnFallingEdge, true);
		
		setCommunicationClock(cc_fxx_8, true);
		
		setBitTransferLenght(btl_8, true);
		
	}
}

void SPI_NEC::setPowerEnable(bool en)
{
	SPI_Abstract::setPowerEnable(en);
	
	if (en)
		m_pReg->CTL0 |= 0x80;
	else
		m_pReg->CTL0 &= ~0x80; 
}

void SPI_NEC::setTransmissionEnable(bool en)
{
	SPI_Abstract::setTransmissionEnable(en);
	
	if (en)
		m_pReg->CTL0 |= 0x40;
	else
		m_pReg->CTL0 &= ~0x40; 
}

void SPI_NEC::setReceptionEnable(bool en)
{
	SPI_Abstract::setReceptionEnable(en);
	
	if (en)
		m_pReg->CTL0 |= 0x20;
	else
		m_pReg->CTL0 &= ~0x20; 
}

bool SPI_NEC::setTransferModeDirection(TransferModeDirection d, bool forceModify)
{
	if( SPI_Abstract::setTransferModeDirection(d, forceModify) ) 
	{
		switch(getTransferModeDirection())
		{
			case tmd_MSB_first:
				m_pReg->CTL0 &= ~0x10;
				break;
			case tmd_LSB_first:
				m_pReg->CTL0 |= 0x10;
				break;
		}
	}
	return true;	
}

bool SPI_NEC::setTransferModeSpecification(TransferModeSpecification s, bool forceModify)
{
	if( SPI_Abstract::setTransferModeSpecification(s, forceModify) ) 
	{
		switch(getTransferModeSpecification())
		{
			case tms_SINGLE_transfer:
				m_pReg->CTL0 &= ~0x02;
				break;
			case tms_CONTINUOUS_transfer:
				m_pReg->CTL0 |= 0x02;
				break;
		}
	}
	return true;	
}

bool SPI_NEC::setCommunicationType(CommunicationType ct, bool forceModify )
{
	if( SPI_Abstract::setCommunicationType(ct, forceModify) ) 
	{
		m_pReg->CTL1 &= ~0x18;
		
		switch(getCommunicationType())
		{
			case ct_CLK_high_sampleOnRisingEdge:
				//m_pReg->CTL1 |= 0x00;
				break;
			case ct_CLK_high_sampleOnFallingEdge:
				m_pReg->CTL1 |= 0x08;
				break;
			case ct_CLK_low_sampleOnFallingEdge:
				m_pReg->CTL1 |= 0x10;
				break;
			case ct_CLK_low_sampleOnRisingEdge:
				m_pReg->CTL1 |= 0x18;
				break;
		}
	}
	return true;
}

bool SPI_NEC::setCommunicationClock(CommunicationClock cc, bool forceModify )
{
	if( SPI_Abstract::setCommunicationClock(cc, forceModify) ) 
	{
		m_pReg->CTL1 &= ~0x07;
		
		switch(getCommunicationClock())
		{
			case cc_fxx_2:
				//m_pReg->CTL1 |= 0x00;
				break;
			case cc_fxx_4:
				m_pReg->CTL1 |= 0x01;
				break;
			case cc_fxx_8:
				m_pReg->CTL1 |= 0x02;
				break;
			case cc_fxx_16:
				m_pReg->CTL1 |= 0x03;
				break;
			case cc_fxx_32:
				m_pReg->CTL1 |= 0x04;
				break;
			case cc_fxx_64:
				m_pReg->CTL1 |= 0x05;
				break;
			case cc_f_BRGm:
				m_pReg->CTL1 |= 0x06;
				break;
			case cc_ext: //slave mode
				m_pReg->CTL1 |= 0x07;
				break; 
		}
	}
	return true;
}

bool SPI_NEC::setBitTransferLenght(BitTransferLenght btl, bool forceModify)
{
	if( SPI_Abstract::setBitTransferLenght(btl, forceModify) ) 
	{
		m_pReg->CTL2 &= ~0x0F;
		
		switch(getBitTransferLenght())
		{
			case btl_8:
				m_pReg->CTL2 |= 0x00;
				break;
			case btl_9:
				m_pReg->CTL2 |= 0x01;
				break;
			case btl_10:
				m_pReg->CTL2 |= 0x02;
				break;
			case btl_11:
				m_pReg->CTL2 |= 0x03;
				break;
			case btl_12:
				m_pReg->CTL2 |= 0x04;
				break;
			case btl_13:
				m_pReg->CTL2 |= 0x05;
				break;
			case btl_14:
				m_pReg->CTL2 |= 0x06;
				break;
			case btl_15:
				m_pReg->CTL2 |= 0x07;
				break;
			case btl_16:
				m_pReg->CTL2 |= 0x08;
				break;
		}
	}
	return true;
}

word SPI_Poller_NEC::putc(word c)
{
	word d;
	while( ( (m_pReg->STR & 0x80) == 0x80) );					// aspetta che esca tutto
	
	d = m_pReg->RXD;

	m_pReg->TXD = c;						// spara il nuovo dato

	return( d );
}

word SPI_Poller_NEC::getc(word filler)
{
	while( ( (m_pReg->STR & 0x80) == 0x80) );					// wait last xfr pending

	m_pReg->TXD = filler;					// spara il nuovo dato

	while( ( (m_pReg->STR & 0x80) == 0x80) );					// wait xfr complete..

	return( m_pReg->RXD );      			// autoreturn inp data 
}

bool SPI_Poller_NEC::busy()
{
	return ( (m_pReg->STR & 0x80) == 0x80);
}




/**
Class to manage spi communication with interrput.
- Enable continuous transfer mode
- Set 8 bit transmission lenght
*/
SPI_Interrupt_NEC::SPI_Interrupt_NEC(volatile spi_nec_t* __pReg, volatile byte *_RIC, volatile byte *_TIC, int _txDim, int _rxDim)
	: m_txbuf(_txDim), m_rxbuf(_rxDim), SPI_NEC(__pReg)
{
	m_pReg = __pReg;
	m_RIC = _RIC;
	m_TIC = _TIC;

	*m_RIC = 0x47;
	*m_TIC = 0x47;

	SPI_NEC::setBitTransferLenght(btl_8, true);
	setTransferModeSpecification(tms_CONTINUOUS_transfer, true);
	
}

void SPI_Interrupt_NEC::setTxInterrupt(bool en, byte priority)
{
	if (priority > 0x07)
		priority = 0x07;

	if(en)
	{
		*m_TIC &= ~0x47;
		*m_TIC |= priority;
	}else
	{
		*m_TIC |= 0x40;
	}
}

void SPI_Interrupt_NEC::setRxInterrupt(bool en, byte priority)
{
	if (priority > 0x07)
		priority = 0x07;

	if(en)
	{
		*m_RIC &= ~0x47;
		*m_RIC |= priority;
	}else
	{
		*m_RIC |= 0x40;
	}
}

int SPI_Interrupt_NEC::tx_putc(byte c)
{
	if (m_txbuf.isfull())
		return 0;
	
	m_txbuf.putc(c);
	startTrasmission();
	return 1;
}

int SPI_Interrupt_NEC::tx_write(byte *data, int dim)
{
	if (m_txbuf.size() - m_txbuf.used() < dim)
		return 0;

	m_txbuf.write((const char*)data, dim);
	startTrasmission();
	return dim;
}

byte SPI_Interrupt_NEC::tx_getc_isr()
{
	return (byte)m_txbuf.getc_isr();
}

bool SPI_Interrupt_NEC::tx_isempty_isr()
{
	return m_txbuf.isempty();
}

bool SPI_Interrupt_NEC::rx_getc(byte &c)
{
	if(m_rxbuf.isempty())
		return false;

	c = m_rxbuf.getc();
	return true;
}

void SPI_Interrupt_NEC::startTrasmission()
{
	byte c;
	if((m_pReg->STR & 0x80) == 0x00)
	{
		c = (byte)m_txbuf.getc_isr();
		m_pReg->TXD = c;
	}
}

bool SPI_Interrupt_NEC::busy()
{
	return ( (m_pReg->STR & 0x80) == 0x80);
}
 
bool SPI_Interrupt_NEC::rx_read(byte *data, int dim)
{
	if(m_rxbuf.used() < dim)
		return false;

	m_rxbuf.read((char*)data, dim);
	return true;
	
}


bool SPI_Interrupt_NEC::rx_putc_isr(byte c)
{
	m_rxbuf.putc_isr((char)c);
}



















