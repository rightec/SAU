/**
@file		ADuC_UpgradeManager.cpp
@brief		Classes that manages the upgrade process with the ADuC

			
@author		Nicola Molinazzi
@date		11/09/2011
@version	01.00
*/



#include "ADuC_UpgradeManager.h"



ADuCUpgradeManger::ADuCUpgradeManger(ADuCfwUpgrade_protocol* __ADuCUpgradeProtocol)
{
	m_ADuCUpgradeProtocol = __ADuCUpgradeProtocol;
	m_upgradeFile = NULL;
	m_upgradeFileLenght = 1;
	m_upgradedLenght = 0;
	m_verifiedLenght = 0;
	m_upgradeStatus = upgrade_Idle;
	m_upgradeError = upgrade_NoError;
	
}

ADuCUpgradeManger::~ADuCUpgradeManger()
{

}

char* ADuCUpgradeManger::getProductID()
{
	if(m_ADuCUpgradeProtocol == NULL)
	{
		return NULL;
	}

	return m_ADuCUpgradeProtocol->getProductID();
}

char* ADuCUpgradeManger::getMemorySize()
{
	if(m_ADuCUpgradeProtocol == NULL)
	{
		return NULL;
	}

	return m_ADuCUpgradeProtocol->getMemorySize();
}

char* ADuCUpgradeManger::getLoaderRevision()
{
	if(m_ADuCUpgradeProtocol == NULL)
	{
		return NULL;
	}

	return m_ADuCUpgradeProtocol->getLoaderRevision();

}

void ADuCUpgradeManger::manager()
{
	int msg;
	
	if(m_ADuCUpgradeProtocol == NULL)
		return;

	if (m_upgradeFile == NULL)
		return;

	m_ADuCUpgradeProtocol->manager();
	
	//--- messages from the protocol....
	msg = m_ADuCUpgradeProtocol->getMsg();
	while(msg != ADuCfwUpgrade_protocol::ADuC_noMsg)
	{
		switch (msg)
		{
			case ADuCfwUpgrade_protocol::ADuC_noMsg:
				
				break;
			case ADuCfwUpgrade_protocol::ADuC_info:
				ADuC_info_Handler();
				break;
			case ADuCfwUpgrade_protocol::ADuC_ack:
				ADuC_ack_Handler();
				break;
			case ADuCfwUpgrade_protocol::ADuC_bel:
				ADuC_bel_Handler();
				break;

			case ADuCfwUpgrade_protocol::Hw_ReceptionError:
				Hw_ReceptionError_Handler();
				break;
			case ADuCfwUpgrade_protocol::Hw_ParityError:
				Hw_ParityError_Handler();
				break;
			case ADuCfwUpgrade_protocol::Hw_FrameError:
				Hw_FrameError_Handler();
				break;
			case ADuCfwUpgrade_protocol::Hw_OverrunError:
				Hw_OverrunError_Handler();
				break;
			case ADuCfwUpgrade_protocol::Hw_NoDevice:
				Hw_NoDevice_Handler();
				break;
			case ADuCfwUpgrade_protocol::Hw_ErrorUnknown:
				Hw_ErrorUnknown_Handler();
				break;

			case ADuCfwUpgrade_protocol::UnknowOpcode:
				UnknowOpcode_Handler();
				break;
			case ADuCfwUpgrade_protocol::stxNotReceived:
				stxNotReceived_Handler();
				break;
			case ADuCfwUpgrade_protocol::extNotReceived:
				extNotReceived_Handler();
				break;
			case ADuCfwUpgrade_protocol::ModuleResponseError:
				ModuleResponseError_Handler();
				break;
			case ADuCfwUpgrade_protocol::TimeOutErrorR:
				TimeOutErrorR_Handler();
				break;
			case ADuCfwUpgrade_protocol::TimeOutErrorT:
				TimeOutErrorT_Handler();
				break;
			case ADuCfwUpgrade_protocol::ChecksumError:
				ChecksumError_Handler();
				break;
			case ADuCfwUpgrade_protocol::TransmitBufferOverflowError:
				TransmitBufferOverflowError_Handler();
				break;
			case ADuCfwUpgrade_protocol::ReceptionBufferOverflowError:
				ReceptionBufferOverflowError_Handler();
				break;
			case ADuCfwUpgrade_protocol::DataReceivedBufferOverflow:
				DataReceivedBufferOverflow_Handler();
				break;
			case ADuCfwUpgrade_protocol::UnknownError:
				UnknownError_Handler();
				break;
		}
		msg = m_ADuCUpgradeProtocol->getMsg();
	}

	//---- machine state ---
	switch(m_upgradeStatus)
	{
		case upgrade_Idle:
			upgrade_Idle_Handler();
			break;
		case upgrade_syncronizing:
			upgrade_syncronizing_Handler();
			break;
		case upgrade_syncronized:
			upgrade_syncronized_Handler();
			break;
		case upgrade_erasingDevice:
			upgrade_erasingDevice_Handler();
			break;
		case upgrade_deviceErased:
			upgrade_deviceErased_Handler();
			break;
		case upgrade_upgradingDevice:
			upgrade_upgradingDevice_Handler();
			break;
		case upgrade_deviceUpgrated:
			upgrade_deviceUpgrated_Handler();
			break;
		case upgrade_verifing:
			upgrade_verifing_Handler();
			break;
		case upgrade_verified:
			upgrade_verified_Handler();
			break;
		case upgrade_aborted:
			upgrade_aborted_Handler();
			break;
		case upgrade_Error:
			upgrade_Error_Handler();
			break;
	}
}

void ADuCUpgradeManger::hwReset()
{
	ADuC_hwReset();
}

void ADuCUpgradeManger::keepReset()
{
	PLD_JTAG_ITF->JTAG_OR = 0x02;	// su tutto meno reset 
}

void ADuCUpgradeManger::abort()
{
	if(m_ADuCUpgradeProtocol == NULL)
		return;

	m_ADuCUpgradeProtocol->rstProtocol();
	m_upgradeStatus = upgrade_aborted;
	m_upgradedLenght = 0;
	m_verifiedLenght = 0;
	keepReset();
}

void ADuCUpgradeManger::startUpgrade()
{
	int i;
	if(m_ADuCUpgradeProtocol == NULL)
		return;
	m_ADuCUpgradeProtocol->rstProtocol();
	m_ADuCUpgradeProtocol->TurnOn();
	m_ADuCUpgradeProtocol->clearTx();
	m_ADuCUpgradeProtocol->clearRx();
	ADuC_hwSerialLoader();
	m_upgradedLenght = 0;
	m_verifiedLenght = 0;
	syncronize();
	
}

void ADuCUpgradeManger::syncronize()
{
	if(m_ADuCUpgradeProtocol == NULL)
		return;

	m_upgradeStatus = upgrade_syncronizing;
	m_ADuCUpgradeProtocol->sendSyncro();
}

void ADuCUpgradeManger::erase()
{
	if(m_ADuCUpgradeProtocol == NULL)
		return;

	m_upgradeStatus = upgrade_erasingDevice;
	m_ADuCUpgradeProtocol->eraseDevice();
}

void ADuCUpgradeManger::upgrade()
{
	dword address; 
	byte* code;
	byte codeLenght;
	
	if(m_ADuCUpgradeProtocol == NULL)
		return;

	
	m_upgradeStatus = upgrade_upgradingDevice;
	
	if ((m_upgradedLenght + 250) > m_upgradeFileLenght)
	{
		codeLenght = m_upgradeFileLenght - m_upgradedLenght;
	}else
	{
		codeLenght = 250;
	}

	address = m_upgradedLenght;
	code = (byte*)(m_upgradeFile + address);
			
	m_ADuCUpgradeProtocol->programFlash(address, code, codeLenght);
	
	m_upgradedLenght += codeLenght;

}

void ADuCUpgradeManger::verify()
{
	dword address;
	byte* code;
	byte codeLenght;
	
	if(m_ADuCUpgradeProtocol == NULL)
		return;

	m_upgradeStatus = upgrade_verifing;

	if ((m_verifiedLenght + 250) > m_upgradeFileLenght)
	{
		codeLenght = m_upgradeFileLenght - m_verifiedLenght;
	}else
	{
		codeLenght = 250;
	}
	
	
	address = m_verifiedLenght;
	code = (byte*)(m_upgradeFile + address);
				
	m_ADuCUpgradeProtocol->verifyFlash(address, code, codeLenght);
	
	m_verifiedLenght += codeLenght;
}

void ADuCUpgradeManger::run()
{
	if(m_ADuCUpgradeProtocol == NULL)
		return;

	//m_upgradeStatus = upgrade_Idle;
	m_ADuCUpgradeProtocol->runDevice();
}
	
void ADuCUpgradeManger::ADuC_hwReset()
{
	//PLD_JTAG_ITF->JTAG_OR = 0; 	// giu' reset e ntrst per microbi aduc

	//cycle_delay(20);

	PLD_JTAG_ITF->JTAG_OR = 0x02;	// su tutto meno reset 

	cycle_delay(300);

	PLD_JTAG_ITF->JTAG_OR = 0x03;	// su tutto e pure il reset 

	cycle_delay(100);
}

void ADuCUpgradeManger::ADuC_hwSerialLoader()
{
	//PLD_JTAG_ITF->JTAG_AND = 0; 	// giu' reset e ntrst per microbi aduc

	//cycle_delay(100);

	PLD_JTAG_ITF->JTAG_OR = 0x03;	// reset: 1 , ntrst = 1
	
	cycle_delay(10);

	PLD_JTAG_ITF->JTAG_OR = 0x01;	// reset: 1 , ntrst = 0

	cycle_delay(20);

	PLD_JTAG_ITF->JTAG_OR = 0x00;	// reset: 0 , ntrst = 0
	
	cycle_delay(20);

	PLD_JTAG_ITF->JTAG_OR = 0x01;	// reset: 1 , ntrst = 0

	cycle_delay(200);

	PLD_JTAG_ITF->JTAG_OR = 0x03;	// reset: 1 , ntrst = 1

	cycle_delay(100);
}

void ADuCUpgradeManger::cycle_delay(int a_msTimeDelay)
{
	unsigned short loop_index     = 0;
	unsigned short ms_index       = 0;
	unsigned short us_index       = 0;
	unsigned short g_usCpu_Frequency  = 20;   /*Enter your CPU frequency here, unit in MHz.*/
	
	/*Users can replace the following section of code by their own*/
	for( ms_index = 0; ms_index < a_msTimeDelay; ms_index++)
	{
		/*Loop 1000 times to produce the milliseconds delay*/
		for (us_index = 0; us_index < 715; us_index++) // loop error around +4%
		{ /*each loop should delay for 1 microsecond or more.*/
			loop_index = 0;
			do {
				/*The NOP fakes the optimizer out so that it doesn't toss out the loop code entirely*/
				asm("nop");
				asm("nop");
			}while (loop_index++ < ((g_usCpu_Frequency/8)+(+ ((g_usCpu_Frequency % 8) ? 1 : 0))));/*use do loop to force at least one loop*/
		}
	}
}


int ADuCUpgradeManger::upgrade_Idle_Handler()
{
	return 1;
}

int ADuCUpgradeManger::upgrade_syncronizing_Handler()
{
	return 1;
}

int ADuCUpgradeManger::upgrade_syncronized_Handler()
{
	erase();
	return 1;
}

int ADuCUpgradeManger::upgrade_erasingDevice_Handler()
{
	return 1;
}

int ADuCUpgradeManger::upgrade_deviceErased_Handler()
{
	m_upgradeStatus  = upgrade_upgradingDevice;
	m_upgradedLenght = 0;
	m_verifiedLenght = 0;
	upgrade();
	
	return 1;
}

int ADuCUpgradeManger::upgrade_upgradingDevice_Handler()
{
	return 1;
}

int ADuCUpgradeManger::upgrade_deviceUpgrated_Handler()
{
	m_upgradeStatus = upgrade_verifing;
	m_verifiedLenght = 0;
	verify();
	return 1;
}

int ADuCUpgradeManger::upgrade_verifing_Handler()
{
	return 1;
}

int ADuCUpgradeManger::upgrade_verified_Handler()
{
	run();
	return 1;
}

int ADuCUpgradeManger::upgrade_aborted_Handler()
{
	return 1;
}

int ADuCUpgradeManger::upgrade_Error_Handler()
{
	return 1;
}

//--- MESSAGES FROM THE PROTOCOL ---
int ADuCUpgradeManger::ADuC_info_Handler()
{
	m_upgradeStatus = upgrade_syncronized;
	return 1;
}

int ADuCUpgradeManger::ADuC_ack_Handler()
{
	//---- machine state ---
	switch(m_upgradeStatus)
	{
		case upgrade_Idle:
			break;
		case upgrade_syncronizing:
			break;
		case upgrade_syncronized:
			break;
		case upgrade_erasingDevice:
			m_upgradeStatus = upgrade_deviceErased;
			
			break;
		case upgrade_deviceErased:
			break;
		case upgrade_upgradingDevice:
			if(m_upgradedLenght >= m_upgradeFileLenght)
			{
				m_upgradeStatus = upgrade_deviceUpgrated;
			}else
			{
				upgrade();
			}				
			break;
		case upgrade_deviceUpgrated:
			
			break;
		case upgrade_verifing:
			if(m_verifiedLenght >= m_upgradeFileLenght)
			{
				m_upgradeStatus = upgrade_verified;
			}else
			{				
				verify();
			}	
			break;
		case upgrade_verified:
			break;
		case upgrade_aborted:
			break;
		case upgrade_Error:
			break;
	}
	return 1;
}
int ADuCUpgradeManger::ADuC_bel_Handler()
{
	m_upgradeStatus = upgrade_Error;
	m_upgradeError = upgrade_NotAck;
	return 1;
}

int ADuCUpgradeManger::Hw_ReceptionError_Handler()
{
	m_upgradeStatus = upgrade_Error;
	m_upgradeError = upgrade_UartHwErr;
	return 1;
}

int ADuCUpgradeManger::Hw_ParityError_Handler()
{
	m_upgradeStatus = upgrade_Error;
	m_upgradeError = upgrade_UartHwErr;
	return 1;
}

int ADuCUpgradeManger::Hw_FrameError_Handler()
{
	m_upgradeStatus = upgrade_Error;
	m_upgradeError = upgrade_UartHwErr;
	return 1;
}


int ADuCUpgradeManger::Hw_OverrunError_Handler()
{
	m_upgradeStatus = upgrade_Error;
	m_upgradeError = upgrade_UartHwErr;
	return 1;
}


int ADuCUpgradeManger::Hw_NoDevice_Handler()
{
	m_upgradeStatus = upgrade_Error;
	m_upgradeError = upgrade_UartHwErr;
	return 1;
}


int ADuCUpgradeManger::Hw_ErrorUnknown_Handler()
{
	m_upgradeStatus = upgrade_Error;
	m_upgradeError = upgrade_UartHwErr;
	return 1;
}

int ADuCUpgradeManger::UnknowOpcode_Handler()
{
	m_upgradeStatus = upgrade_Error;
	m_upgradeError = upgrade_protocolErr;
	return 1;
}

int ADuCUpgradeManger::stxNotReceived_Handler()
{
	m_upgradeStatus = upgrade_Error;
	m_upgradeError = upgrade_protocolErr;
	return 1;
}

int ADuCUpgradeManger::extNotReceived_Handler()
{
	m_upgradeStatus = upgrade_Error;
	m_upgradeError = upgrade_protocolErr;
	return 1;
}

int ADuCUpgradeManger::ModuleResponseError_Handler()
{
	m_upgradeStatus = upgrade_Error;
	m_upgradeError = upgrade_protocolErr;
	return 1;
}

int ADuCUpgradeManger::TimeOutErrorR_Handler()
{
	m_upgradeStatus = upgrade_Error;
	m_upgradeError = upgrade_timeOutRx;
	return 1;
}

int ADuCUpgradeManger::TimeOutErrorT_Handler()
{
	m_upgradeStatus = upgrade_Error;
	m_upgradeError = upgrade_timeOutTx;
	return 1;
}

int ADuCUpgradeManger::ChecksumError_Handler()
{
	m_upgradeStatus = upgrade_Error;
	m_upgradeError = upgrade_protocolErr;
	return 1;
}

int ADuCUpgradeManger::TransmitBufferOverflowError_Handler()
{
	m_upgradeStatus = upgrade_Error;
	m_upgradeError = upgrade_protocolErr;
	return 1;
}

int ADuCUpgradeManger::ReceptionBufferOverflowError_Handler()
{
	m_upgradeStatus = upgrade_Error;
	m_upgradeError = upgrade_protocolErr;
	return 1;
}

int ADuCUpgradeManger::DataReceivedBufferOverflow_Handler()
{
	m_upgradeStatus = upgrade_Error;
	m_upgradeError = upgrade_protocolErr;
	return 1;
}

int ADuCUpgradeManger::UnknownError_Handler()
{
	m_upgradeStatus = upgrade_Error;
	m_upgradeError = upgrade_protocolErr;
	return 1;
}



















