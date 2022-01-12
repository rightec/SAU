/**
@file		AdcAbstract.h

@brief		Abstract ADCs
		
@author		Nicola Molinazzi
@date		05/10/2011
@version	01.00
*/

#ifndef _ADC_ABSTRACT_H_
#define _ADC_ABSTRACT_H_

#include "Spi_Abstract.h"

class AdcAbstract
{	
	public:
		AdcAbstract(){};
		~AdcAbstract(){};

		virtual void setPower(bool pw){m_power = pw;}
		virtual bool isPowered(){return m_power;}
		
		virtual void setEnable(bool en){m_enable = en;}
		virtual bool isEnable(){return m_enable;}
		
	protected:
		
	private:
		bool m_enable;
		bool m_power;
};

class AdcSpiAbstract: public AdcAbstract
{
	public:
		AdcSpiAbstract(SPI_Abstract * _spi);
		~AdcSpiAbstract(){};
		
	protected:
		SPI_Abstract *m_spi;
		
	private:

};





















#endif


