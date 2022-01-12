/**
@file		SPI_ABSTRACT.CPP
@brief		Classes to manage SPI communication

@author		Nicola Molinazzi
@date		28/09/2011
@version	01.00
*/


#ifndef __SPI_ABSTRACT_H__
#define __SPI_ABSTRACT_H__


#include "global.h"

#undef putc
#undef getc

class SPI_Abstract
{

public:
	typedef enum 
	{
		tmd_MSB_first = 0,
		tmd_LSB_first,
	}TransferModeDirection;

	enum TransferModeSpecification
	{
		tms_SINGLE_transfer = 0,
		tms_CONTINUOUS_transfer,
	};

	enum CommunicationType
	{
		ct_CLK_high_sampleOnRisingEdge = 0,
		ct_CLK_high_sampleOnFallingEdge,
		ct_CLK_low_sampleOnFallingEdge,
		ct_CLK_low_sampleOnRisingEdge,
	};

	enum CommunicationClock
	{
		cc_fxx_2 = 0,
		cc_fxx_4,
		cc_fxx_8,
		cc_fxx_16,
		cc_fxx_32,
		cc_fxx_64,
		cc_f_BRGm,
		cc_ext, //slave mode
		
	};

	enum BitTransferLenght
	{
		btl_8 = 0,
		btl_9,
		btl_10,
		btl_11,
		btl_12,
		btl_13,
		btl_14,
		btl_15,
		btl_16,
	};

	SPI_Abstract();
	~SPI_Abstract(){};
	
	virtual void setPowerEnable(bool en){ m_power = en;}
	virtual bool isPowerEnable(){return m_power;}

	virtual void setTransmissionEnable(bool en){m_enableTransmission = en;}
	virtual bool isTransmissionEnable(){return m_enableTransmission;}
	
	virtual void setReceptionEnable(bool en){m_enableReception = en;}
	virtual bool isReceptionEnable(){return m_enableReception;}

	virtual bool setTransferModeDirection(TransferModeDirection d, bool forceModify = false);
	virtual TransferModeDirection getTransferModeDirection(){return m_transferModeDir;}

	virtual bool setTransferModeSpecification(TransferModeSpecification s, bool forceModify = false);
	virtual TransferModeSpecification getTransferModeSpecification(){return m_transferModeSpec;}

	virtual bool setCommunicationType(CommunicationType ct, bool forceModify = false);
	virtual CommunicationType getCommunicationType(){return m_communicationType;}

	virtual bool setCommunicationClock(CommunicationClock cc, bool forceModify = false);
	virtual CommunicationClock getCommunicationClock(){return m_communicationClock;}

	virtual bool setBitTransferLenght(BitTransferLenght btl, bool forceModify = false);
	virtual BitTransferLenght getBitTransferLenght(){return m_bitTransferLenght;}

	virtual word putc(word c){return 0xFFFF;}
	virtual word getc(word filler){return 0xFFFF;}
	virtual bool busy(){return false;}
	
protected:

private:

	bool m_power;
	bool m_enableTransmission;
	bool m_enableReception;
	TransferModeDirection m_transferModeDir;
	TransferModeSpecification m_transferModeSpec;
	CommunicationType m_communicationType;
	CommunicationClock m_communicationClock;
	BitTransferLenght m_bitTransferLenght;
	
};
















#endif





