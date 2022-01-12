/**
@file		protocolADuCfwUpgrade.h
@brief		Classes that manages a uart communication protocol to upgrade the ADuC

			
@author		Nicola Molinazzi
@date		08/09/2011
@version	01.00
*/


#ifndef __PROTOCOL_ADuC_FW_UPGRADE_H__
#define __PROTOCOL_ADuC_FW_UPGRADE_H__

#include "global.h"
#include "UartAbstract.h"
#include "protocolStdUart.h"
#include "graph.h"

#include "sound.h"


#define ADuC_UPGRADE_STX 		0x07
#define ADuC_UPGRADE_DEVICE_ID	0x0E

#define ADuC_UPGRADE_ACK 		0x06
#define ADuC_UPGRADE_BEL 		0x07


#define ADuC_UPGRADE_PRODUCT_ID_LEN		9
#define ADuC_UPGRADE_MEMORY_SIZE_LEN	4
#define ADuC_UPGRADE_REVISION_LEN		4


class ADuCfwUpgrade_protocol: public stdUartProtocolAbstraction
{
public:

	enum ADuCUpgradeCmd
	{
		cmd_none = 0,
		cmd_syncro,
		cmd_eraseDevice,
		cmd_erasePage,
		cmd_progamFlash,
		cmd_verifyFlash,
		cmd_runDevice,
	};

	enum ADuCUpgradeMsg
	{
		ADuC_noMsg = 0,
		ADuC_info,
		ADuC_ack,
		ADuC_bel,

		Hw_ReceptionError,				/**< Reception error. Generate from QueueUart*/
		Hw_ParityError,					/**< Parity error. Generate from QueueUart*/
		Hw_FrameError,					/**< Frame error. Generate from QueueUart*/
		Hw_OverrunError,				/**< Overrun error. Generate from QueueUart*/
		Hw_NoDevice,					/**< No device error. Generate from QueueUart*/
		Hw_ErrorUnknown,				/**< Unknown error. Generate from QueueUart*/

		UnknowOpcode,					/**< Unknown opcode*/
		stxNotReceived,					/**< stx not recieved*/
		extNotReceived,					/**< etx not received*/
		ModuleResponseError,			/**< module response errore*/
		TimeOutErrorR,					/**< Timeout RX*/
		TimeOutErrorT,					/**< Timeout TX*/
		ChecksumError,					/**< checksum error*/ 
		TransmitBufferOverflowError,	/**< transmit buffer overflow*/ 
		ReceptionBufferOverflowError,	/**< reception buffer overflow*/ 
		DataReceivedBufferOverflow,		/**< data received buffer overflow*/ 
		UnknownError,
	};
	
	ADuCfwUpgrade_protocol(UartDeviceAbstract *__pUartDevice, int __rxProtoBufSize = __UART_MAX_MESSAGE_LENGHT__);
	~ADuCfwUpgrade_protocol();

	
	virtual void manager();

	virtual void TurnOn(){stdUartProtocolAbstraction::TurnOn();}
	virtual void TurnOff(){stdUartProtocolAbstraction::TurnOff();}
	
	/**
	@return the number of bytes in the transmission buffer.
	*/
	int sendSyncro();
	int eraseDevice();
	int erasePage(dword address, byte numOfPages, int timeout = 500);
	int programFlash(dword address, byte* code, byte codeLenght);
    int verifyFlash(dword address, byte* code, byte codeLenght);
	int runDevice();
	
	virtual int getMsg();

	char* getProductID(){return m_productID;}
	char* getMemorySize(){return m_memorySize;}
	char* getLoaderRevision(){return m_revision;}
	
	/**
	@return the last command sent to the uC 
	@see SauuCCmd_FromSauTOuC
	*/
	ADuCUpgradeCmd getLastCmdSent(){ return m_lastCmdSent;};

	void rstProtocol();

	void setUartDevice(UartDeviceAbstract *__pUartDevice){m_pUart = __pUartDevice;};
			
protected:
	enum OpCode
	{
		ERASE_PAGE  = 0x45,
		WRITE	    = 0x57,
		VERIFY		= 0x56,
		PROECT		= 0x50,
		RUN 		= 0x52,
	};
	
	virtual bool DataAnalyse();
	
	virtual int sendMsg(int i);

	void ADuC_upgr_getInfoCmdDecode();
	
private:
	/**
	Messages' fifo
	*/
	CSmallRingBuf<int , _stdUART_FIFOS_DEPTH_> m_message;
	
	ADuCUpgradeCmd m_lastCmdSent;

	byte m_messageReceived[__UART_MAX_MESSAGE_LENGHT__];

	int m_messageReceivedLenght;
	
	char m_productID[ADuC_UPGRADE_PRODUCT_ID_LEN];
	char m_memorySize[ADuC_UPGRADE_MEMORY_SIZE_LEN];
	char m_revision[ADuC_UPGRADE_REVISION_LEN];

	int m_opCodeDataAnalyse;
	int m_lastRemoteIDDataAnalyse;
};

















#endif


















