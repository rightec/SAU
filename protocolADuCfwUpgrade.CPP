/**
@file		protocolADuCfwUpgrade.cpp
@brief		Classes that manages a uart communication protocol to upgrade the ADuC

			
@author		Nicola Molinazzi
@date		08/09/2011
@version	01.00
*/



#include "protocolADuCfwUpgrade.h"



ADuCfwUpgrade_protocol::ADuCfwUpgrade_protocol(UartDeviceAbstract *__pUartDevice, int __rxProtoBufSize):
	stdUartProtocolAbstraction(__pUartDevice)
{
	setReceiverState(stdUart_waitStx);
	setLastDataReceivedBuffer(__rxProtoBufSize);
	setMultiMessagesAnalisyPerLoop(true);
	rstBytesReceived();

	m_opCodeDataAnalyse = 0;
	m_lastRemoteIDDataAnalyse = 0;
	rstProtocol();
	
	
}

ADuCfwUpgrade_protocol::~ADuCfwUpgrade_protocol()
{

}



void ADuCfwUpgrade_protocol::manager()
{
	int error, opCode;
	
	stdUartProtocolAbstraction::manager();

	//---- Remapping stdUartProtocolAbstraction error ----
	error = popError();
	
	while (error != stdUart_NoError)
	{
		switch(error)
		{
			case stdUart_Hw_ReceptionError:
				sendMsg(Hw_ReceptionError);
				break;
			case stdUart_Hw_ParityError:
				sendMsg(Hw_ParityError);
				break;
			case stdUart_Hw_FrameError:
				sendMsg(Hw_FrameError);
				break;
			case stdUart_Hw_OverrunError:
				sendMsg(Hw_OverrunError);
				break;
		    case stdUart_Hw_RxFifoOverrun:
			case stdUart_Hw_TxFifoOverrun:
				sendMsg(Hw_OverrunError);
				break;
			case stdUart_Hw_NoDevice:
				sendMsg(Hw_NoDevice);
				break;
			case stdUart_Hw_ErrorUnknown:
				sendMsg(Hw_ErrorUnknown);
				break;
			//	+ da Queue.h -> errori protocollo
			case stdUart_UnknowOpcode:
				sendMsg(UnknowOpcode);
				break;
			case stdUart_stxNotReceived:
				sendMsg(stxNotReceived);
				break;
			case stdUart_TimeOutErrorR:
				sendMsg(TimeOutErrorR);
				break;
			case stdUart_TimeOutErrorT:
				sendMsg(TimeOutErrorT);
				break;
			case stdUart_ChecksumError:
				sendMsg(ChecksumError);
				break;
			case stdUart_TransmitBufferOverflowError:
				sendMsg(TransmitBufferOverflowError);
				break;
			case stdUart_ReceptionBufferOverflowError:
				sendMsg(ReceptionBufferOverflowError);
				break;
			case stdUart_DataReceivedBufferOverflow:
				sendMsg(DataReceivedBufferOverflow);
				break;
			//case stdUart_ModuleResponseError:
			default:
				sendMsg(UnknownError); 
				break;

		}
		error = popError();
	}

	//------- Checking for packets receiving ----------------
	while( popPacketReceived(opCode) )
	{
		popMultiMessage(m_messageReceived, m_messageReceivedLenght);
		
		switch (opCode)
		{
			case ADuC_info:
				ADuC_upgr_getInfoCmdDecode();
				break;
			default:
				sendMsg(UnknowOpcode);
				break;
		}
	}
}


bool ADuCfwUpgrade_protocol::DataAnalyse()
{
	bool newdata;
	char dataBuff;
	
						// prima la gestione errori
	stdUartProtocolAbstraction::DataAnalyse();
						// succhia dalla uart dati ed eventuali stati di errore
	newdata = m_pUart->ReceiveData(dataBuff);
	if( newdata )
	{
		switch(getReceiverState())
		{
			case stdUart_waitStx:
				if (dataBuff == ADuC_UPGRADE_ACK)
				{
					stopTxTimeoutTimer();
					stopRxTimeoutTimer();
					setReceiverState(stdUart_waitStx);
					sendMsg(ADuC_ack);
					
				}else if (dataBuff == ADuC_UPGRADE_BEL)
				{
					stopTxTimeoutTimer();
					stopRxTimeoutTimer();
					setReceiverState(stdUart_waitStx);
					sendMsg(ADuC_bel);
				}else if (dataBuff == 'A')
				{
					stopTxTimeoutTimer();
					setTimeoutRx(110);
					startRxTimeoutTimer();
					setPacketlength(24);
					rstBytesReceived();
					
					setReceiverState(stdUart_receivingData);
					if (!addDataToReceivingBuffer( (byte)dataBuff, getNumBytesReceived()))
					{
						setReceiverState(stdUart_waitStx);
						stopRxTimeoutTimer();
						return true;
					}
					newByteReceived();
				}
				else if(getLastError() != stdUart_stxNotReceived)
				{
					setError((int)stdUart_stxNotReceived);
					return false;
				}
				break;
			case stdUart_waitDeviceID:
				
				break;
			case stdUart_waitNumPacketTransmit:
				
				break;
			case stdUart_waitOpCode1:
					
				break;
			case stdUart_waitOpCode2:
				
				break;
			case stdUart_waitDataLength:
				
				break;
			case stdUart_receivingData:	
				if (!addDataToReceivingBuffer( (byte *) &dataBuff, getNumBytesReceived() - 1, 1))
				{				
					setReceiverState(stdUart_waitStx);
					stopRxTimeoutTimer();
					return true;
				}
				
				if (getNumBytesReceived() >= getPacketLength())
				{
					setReceivedOpCode(ADuC_info);
					newPacketReceived();
					if(getMultiMessagesAnalisyPerLoop())
					{
						pushMultiMessages(bufferLastDataReceived(), getNumBytesReceived());
					}
					setReceiverState(stdUart_waitStx);
					stopRxTimeoutTimer();
				}
				
				break;
			case stdUart_waitChecksum:
				break;
			case stdUart_waitEtx:
				
				break;
			default:
				
				break;
		}

		newByteReceived();
	}
	
	return false;
}


int ADuCfwUpgrade_protocol::getMsg()
{
	int msg = ADuC_noMsg;
	if(!m_message.empty())
	{
		m_message.pop_front(msg);
	}
	return msg;
}

int ADuCfwUpgrade_protocol::sendMsg(int i)
{
	if(!m_message.full())
	{
		m_message.push_back(i);
		return 0;
	}	
	return -1;
}

int ADuCfwUpgrade_protocol::sendSyncro()
{
	byte packet[2];
	int i, k, n;
	int retVal;
	i = 0;
	packet[i] = 0x08; 
	i++;
	
	m_lastCmdSent = cmd_syncro;
	retVal = SendCommand(packet, i, 100);

	/*for(k = 0; k < 10; k++)
	{
		SendCommand(packet, i, 0);	
		for (n = 0; n < 100; n++)
		{
			asm("nop");
		}
	}*/
	return retVal;
}

int ADuCfwUpgrade_protocol::eraseDevice()
{
	int retVal = erasePage(0, 0, 5000);
	m_lastCmdSent = cmd_eraseDevice;
	return retVal;
}

int ADuCfwUpgrade_protocol::erasePage(dword address, byte numOfPages, int timeout)
{
	byte packet[20];
	int i;

	i = 0;
	packet[i] = ADuC_UPGRADE_STX; 
	i++;
	packet[i] = ADuC_UPGRADE_DEVICE_ID; 
	i++;
	
	packet[i] = 6; 
	i++;

	packet[i] = ERASE_PAGE; 
	i++;
	
	packet[i] = (byte)((address >> 24) & 0x000000FF); 
	i++;
	packet[i] = (byte)((address >> 16) & 0x000000FF); 
	i++;
	packet[i] = (byte)((address >> 8) & 0x000000FF); 
	i++;
	packet[i] = (byte)((address ) & 0x000000FF); 
	i++;

	packet[i] = numOfPages;
	i++;
	
	packet[i] = 0x00 - calculateChecksum(&packet[2], i - 2);
	i++;
	
	m_lastCmdSent = cmd_erasePage;
	return SendCommand(packet, i, timeout);
}

int ADuCfwUpgrade_protocol::programFlash(dword address, byte* code, byte codeLenght)
{
	byte packet[270];
	int i, k;

	i = 0;
	packet[i] = ADuC_UPGRADE_STX; 
	i++;
	packet[i] = ADuC_UPGRADE_DEVICE_ID; 
	i++;

	packet[i] = 5 + codeLenght; 
	i++;
	packet[i] = WRITE; 
	i++;
	
	packet[i] = (byte)((address >> 24) & 0x000000FF); 
	i++;
	packet[i] = (byte)((address >> 16) & 0x000000FF); 
	i++;
	packet[i] = (byte)((address >> 8) & 0x000000FF); 
	i++;
	packet[i] = (byte)((address ) & 0x000000FF); 
	i++;
	
	for (k = 0; k < codeLenght; k++)
	{
		packet[i] = code[k];
		i++;
	}
	
	packet[i] = 0x00 - calculateChecksum(&packet[2], i - 2);
	i++;
	
	m_lastCmdSent = cmd_progamFlash;
	return SendCommand(packet, i, 500);
}

int ADuCfwUpgrade_protocol::verifyFlash(dword address, byte* code, byte codeLenght)
{
	
	byte packet[270];
	int i, k;
	byte bit5, bit3;

	i = 0;
	packet[i] = ADuC_UPGRADE_STX; 
	i++;
	packet[i] = ADuC_UPGRADE_DEVICE_ID; 
	i++;

	packet[i] = 5 + codeLenght; 
	i++;
	packet[i] = VERIFY; 
	i++;
	
	packet[i] = (byte)((address >> 24) & 0x000000FF); 
	i++;
	packet[i] = (byte)((address >> 16) & 0x000000FF); 
	i++;
	packet[i] = (byte)((address >> 8) & 0x000000FF); 
	i++;
	packet[i] = (byte)((address ) & 0x000000FF); 
	i++;
	
	for (k = 0; k < codeLenght; k++)
	{
		bit5 = code[k] & 0x1F;
		bit3 = code[k] & 0xE0;
		
		packet[i] = (bit5 << 3) + (bit3 >> 5);
		
		i++;
	}
	
	packet[i] = 0x00 - calculateChecksum(&packet[2], i - 2);
	i++;
	
	m_lastCmdSent = cmd_verifyFlash;
	return SendCommand(packet, i, 300);
}

int ADuCfwUpgrade_protocol::runDevice()
{
	byte packet[30];
	int i;

	i = 0;
	packet[i] = ADuC_UPGRADE_STX; 
	i++;
	packet[i] = ADuC_UPGRADE_DEVICE_ID; 
	i++;

	packet[i] = 5; 
	i++;
	
	packet[i] = RUN; 
	i++;

	packet[i] = 0; 
	i++;
	packet[i] = 0; 
	i++;
	packet[i] = 0; 
	i++;
	packet[i] = 1; 
	i++;
	
	packet[i] = 0x00 - calculateChecksum(&packet[2], i - 2);
	i++;
	m_lastCmdSent = cmd_runDevice;
	return SendCommand(packet, i, 300);
}	


void ADuCfwUpgrade_protocol::rstProtocol()
{
	int i;

	for (i = 0; i < ADuC_UPGRADE_PRODUCT_ID_LEN; i++)
		m_productID[i] = '\0';
	
	for (i = 0; i < ADuC_UPGRADE_MEMORY_SIZE_LEN; i++)
		m_memorySize[i] = '\0';

	for (i = 0; i < ADuC_UPGRADE_PRODUCT_ID_LEN; i++)
		m_revision[i] = '\0';

	m_lastCmdSent = cmd_none;
	m_message.clear();

	setReceiverState(stdUart_waitStx);

}



void ADuCfwUpgrade_protocol::ADuC_upgr_getInfoCmdDecode()
{
	int i, k;
	k = 0;
	
	for (i = 0; i < ADuC_UPGRADE_PRODUCT_ID_LEN-1; i++)
	{
			m_productID[i] = m_messageReceived[k];
			k++;
	}	
	k++; //<space>
	k++; //<space>
	k++; //<space>
	for (i = 0; i < ADuC_UPGRADE_MEMORY_SIZE_LEN-1; i++)
	{
		m_memorySize[i] = m_messageReceived[k];
		k++;
	}
	k++; //<space>
	for (i = 0; i < ADuC_UPGRADE_PRODUCT_ID_LEN-1; i++)
	{
		m_revision[i] = m_messageReceived[k];
		k++;
	}
	k++; //<space>
	k++; //<space>
	k++; //<space>
	k++; //<space>
	k++; //\r
	k++; //\n

	sendMsg(ADuC_info);
}

















