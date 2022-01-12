/**
@file		AdcAbstract.cpp

@brief		Abstract ADCs
		
@author		Nicola Molinazzi
@date		05/10/2011
@version	01.00
*/



#include "AdcAbstract.h"


AdcSpiAbstract::AdcSpiAbstract(SPI_Abstract * _spi):AdcAbstract()
{
	m_spi = NULL;
	if (_spi != NULL)
		m_spi = _spi;
}


















