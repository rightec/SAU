/**
@file		SPI_ABSTRACT.CPP
@brief		Classes to manage SPI communication

@author		Nicola Molinazzi
@date		28/09/2011
@version	01.00
*/


#include "SPI_abstract.h"

SPI_Abstract::SPI_Abstract()
{
	setPowerEnable(false);
	setTransmissionEnable(false);
	setReceptionEnable(false);	
	setTransferModeDirection(tmd_MSB_first, true);
	setTransferModeSpecification(tms_SINGLE_transfer, true);
	setCommunicationType(ct_CLK_high_sampleOnFallingEdge, true);
	setCommunicationClock(cc_fxx_8, true);
	setBitTransferLenght(btl_8, true);
	
}
	
bool SPI_Abstract::setTransferModeDirection(TransferModeDirection d, bool forceModify)
{
	if(d != m_transferModeDir|| forceModify) 
	{
		m_transferModeDir = d;
		return true;
	}
	return true;
}

bool SPI_Abstract::setTransferModeSpecification(TransferModeSpecification s, bool forceModify)
{
	if(s != m_transferModeSpec|| forceModify) 
	{
		m_transferModeSpec = s;
		return true;
	}
	return true;
}

bool SPI_Abstract::setCommunicationType(CommunicationType ct, bool forceModify)
{
	if(ct != m_communicationType|| forceModify) 
	{
		m_communicationType = ct;
		return true;
	}
	return true;
}

bool SPI_Abstract::setCommunicationClock(CommunicationClock cc, bool forceModify)
{
	if(cc != m_communicationClock|| forceModify) 
	{
		m_communicationClock = cc;
		return true;
	}
	return true;
}

bool SPI_Abstract::setBitTransferLenght(BitTransferLenght btl, bool forceModify)
{
	if(btl != m_bitTransferLenght|| forceModify) 
	{
		m_bitTransferLenght = btl;
		return true;
	}
	return true;
}
	



















