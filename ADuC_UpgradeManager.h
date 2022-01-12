/**
@file		ADuC_UpgradeManager.h
@brief		Classes that manages the upgrade process with the ADuC

			
@author		Nicola Molinazzi
@date		11/09/2011
@version	01.00
*/



#ifndef __ADuC_UPGRADE_MANAGER_H__
#define __ADuC_UPGRADE_MANAGER_H__

#include "global.h"
#include "protocolADuCfwUpgrade.h"


class ADuCUpgradeManger
{
public:
	enum UpgradeStatus
	{
		upgrade_Idle = 0,
		upgrade_syncronizing,
		upgrade_syncronized,
		upgrade_erasingDevice,
		upgrade_deviceErased,
		upgrade_upgradingDevice,
		upgrade_deviceUpgrated,
		upgrade_verifing,
		upgrade_verified,
		upgrade_aborted,
		upgrade_Error,
	};

	enum UpgradeErrors
	{
		upgrade_NoError = 0,
		upgrade_timeOutTx,
		upgrade_timeOutRx,
		upgrade_NotAck,
		upgrade_protocolErr,
		upgrade_UartHwErr,
	};
	
	ADuCUpgradeManger(ADuCfwUpgrade_protocol* __ADuCUpgradeProtocol);
	~ADuCUpgradeManger();

	void setUpgradeFile(byte* _upgradeFile, dword _upgradeFileLenght){m_upgradeFile = _upgradeFile; m_upgradeFileLenght = _upgradeFileLenght;}

	dword percentageUpgrated(){return (m_upgradedLenght *100) / m_upgradeFileLenght;}
	dword percentageVerified(){return (m_verifiedLenght *100) / m_upgradeFileLenght;}
	
	UpgradeStatus getUpgradeStatus(){return m_upgradeStatus;}
	UpgradeErrors getUpgradeError(){return m_upgradeError;}
	
	char* getProductID();
	char* getMemorySize();
	char* getLoaderRevision();	

	void manager();
	void abort();
	void startUpgrade();
	void hwReset();
	void keepReset();
	
	void setUartDevice(UartDeviceAbstract *setUartDevice){m_ADuCUpgradeProtocol->setUartDevice(setUartDevice);}

protected:
	void syncronize();
	void erase();
	void upgrade();
	void verify();
	void run();
	
	void ADuC_hwReset();
	void ADuC_hwSerialLoader();

	void cycle_delay(int a_msTimeDelay);
	
private:

	inline int upgrade_Idle_Handler();
	inline int upgrade_syncronizing_Handler();
	inline int upgrade_syncronized_Handler();
	inline int upgrade_erasingDevice_Handler();
	inline int upgrade_deviceErased_Handler();
	inline int upgrade_upgradingDevice_Handler();
	inline int upgrade_deviceUpgrated_Handler();
	inline int upgrade_verifing_Handler();
	inline int upgrade_verified_Handler();
	inline int upgrade_aborted_Handler();
	inline int upgrade_Error_Handler();


	inline int ADuC_info_Handler();
	inline int ADuC_ack_Handler();
	inline int ADuC_bel_Handler();

	inline int Hw_ReceptionError_Handler();
	inline int Hw_ParityError_Handler();
	inline int Hw_FrameError_Handler();
	inline int Hw_OverrunError_Handler();
	inline int Hw_NoDevice_Handler();
	inline int Hw_ErrorUnknown_Handler();

	inline int UnknowOpcode_Handler();
	inline int stxNotReceived_Handler();
	inline int extNotReceived_Handler();
	inline int ModuleResponseError_Handler();
	inline int TimeOutErrorR_Handler();
	inline int TimeOutErrorT_Handler();
	inline int ChecksumError_Handler();
	inline int TransmitBufferOverflowError_Handler();
	inline int ReceptionBufferOverflowError_Handler();
	inline int DataReceivedBufferOverflow_Handler();
	inline int UnknownError_Handler();
	
	ADuCfwUpgrade_protocol *m_ADuCUpgradeProtocol;

	byte* m_upgradeFile;
	dword m_upgradeFileLenght;

	UpgradeStatus m_upgradeStatus;
	dword m_upgradedLenght;
	dword m_verifiedLenght;
	
	UpgradeErrors m_upgradeError;
};

















#endif





















