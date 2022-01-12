/**
@file		SPI_NEC.h
@brief		Classes to manage SPI communication

@author		Nicola Molinazzi
@date		28/09/2011
@version	01.00
*/



#ifndef __SPI_NEC_H__
#define __SPI_NEC_H__

#include "SPI_Abstract.h"
#include "UartAbstract.h"

struct spi_nec_t{
	byte	CTL0;
	byte	CTL1;
	byte 	CTL2;
	byte	STR;
	word	RXD;
	word	TXD;
};

struct spi_nec_interrupt_t{
	byte	CTL0;
	byte	CTL1;
	byte 	CTL2;
	byte	STR;
	word	RXD;
	word	TXD;
};


class SPI_NEC: public SPI_Abstract
{
public:
	SPI_NEC(volatile spi_nec_t* __pReg);
	~SPI_NEC(){};

	virtual void setPowerEnable(bool en);
	virtual bool isPowerEnable(){return SPI_Abstract::isPowerEnable();}

	virtual void setTransmissionEnable(bool en);
	virtual bool isTransmissionEnable(){return SPI_Abstract::isTransmissionEnable();}
	
	virtual void setReceptionEnable(bool en);
	virtual bool isReceptionEnable(){return SPI_Abstract::isReceptionEnable();}

	virtual bool setTransferModeDirection(TransferModeDirection d, bool forceModify = false);
	virtual TransferModeDirection getTransferModeDirection(){return SPI_Abstract::getTransferModeDirection();}

	virtual bool setTransferModeSpecification(TransferModeSpecification s, bool forceModify = false);
	virtual TransferModeSpecification getTransferModeSpecification(){return SPI_Abstract::getTransferModeSpecification();}

	virtual bool setCommunicationType(CommunicationType ct, bool forceModify = false);
	virtual CommunicationType getCommunicationType(){return SPI_Abstract::getCommunicationType();}

	virtual bool setCommunicationClock(CommunicationClock cc, bool forceModify = false);
	virtual CommunicationClock getCommunicationClock(){return SPI_Abstract::getCommunicationClock();}

	virtual bool setBitTransferLenght(BitTransferLenght btl, bool forceModify = false);
	virtual BitTransferLenght getBitTransferLenght(){return SPI_Abstract::getBitTransferLenght();}
	
protected:
	volatile spi_nec_t* m_pReg;

private:
	
	
};


class SPI_Poller_NEC: public SPI_NEC
{
public:
	SPI_Poller_NEC(volatile spi_nec_t* __pReg):SPI_NEC(__pReg){};
	~SPI_Poller_NEC(){};

	virtual word putc(word c);
	virtual word getc(word filler);	
	virtual bool busy();
	
protected:
	
private:


};

class SPI_Interrupt_NEC: public SPI_NEC
{
public:
	SPI_Interrupt_NEC(volatile spi_nec_t* __pReg, volatile byte *_RIC, volatile byte *_TIC, int _txDim, int rxDim);
	~SPI_Interrupt_NEC(){};

	void setTxInterrupt(bool en, byte priority = 0x07);
	void setRxInterrupt(bool en, byte priority = 0x07);

	virtual bool busy();
	
	int tx_putc(byte c);
	int tx_write(byte *data, int dim);
	byte tx_getc_isr();
	bool tx_isempty_isr();
	
	bool rx_getc(byte &c);
	bool rx_read(byte *data, int dim);

	bool rx_putc_isr(byte c);
	
protected:
	void startTrasmission();
	
	UartRingBuffer	m_txbuf;	// bufferini per rx & tx
	UartRingBuffer  m_rxbuf;

	
private:
	volatile byte *m_RIC;
	volatile byte *m_TIC;
};
















#endif































