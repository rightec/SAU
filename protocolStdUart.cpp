/**
@file		protocolStdUart.cpp
@brief		Class to manage a general uart protocol
					
@author		Nicola
@date		18/01/2011
@version	01.1
*/

#include "protocolStdUart.h"


//---------------------------------------------------------------------------------------
//----------------------- stdUartProtocolAbstraction ------------------------------------
//---------------------------------------------------------------------------------------

/**
Class constructor.

@param device uart device
@see UARTDevice
*/
stdUartProtocolAbstraction::stdUartProtocolAbstraction(UartDeviceAbstract *__pUartDevice)
{

	m_pUart = __pUartDevice;
	
	m_timeoutTx = 100;
	m_timeoutRx = 100;
	m_TxTimeoutTimer.Stop();
	m_RxTimeoutTimer.Stop();
	m_lastError = stdUart_NoError;

	m_error.clear();

	m_numBytesRecieved = 0;
	m_packetLength = 0;

	m_receiverState = stdUart_waitStx;
	m_lastReceivedOpCode = 0;
	m_numPacketsReceived = 0;
	m_numPacketsTransmitted = 0;
	m_lastRemoteNodeID = 0;
	m_localNodeID = 0;
	m_lastDataReceived = NULL;
	m_bufferLastDataReceivedDimension = 0;

	m_receivedOpCode.clear();
	m_numDataReceived = 0;
	m_enableMultiMessagesAnalisy = false;

	m_multiMessages.data.clear();
	m_multiMessages.dataLength.clear();
	setBypassMode(false, NULL);
	m_rstErrorTimer.Preset(1000);
}

/**
Class destructor
*/
stdUartProtocolAbstraction::~stdUartProtocolAbstraction()
{
	//if (m_lastDataReceived != NULL)
	//{
	//	delete m_lastDataReceived;
	//}
}


/**
Restores the initial condition.
*/
void stdUartProtocolAbstraction::restoreIntialCondition()
{
	m_receiverState = stdUart_waitStx;
	m_TxTimeoutTimer.Stop();
	m_RxTimeoutTimer.Stop();
}

/**
It has to be called in the sons class. 

It manages the timer and rise thier overruns errors and the protocol's decode function. 
*/
void stdUartProtocolAbstraction::manager()
{
int whileCycles = 0;
bool analized;

	if(m_pUart == NULL)
		return;

	// + Analizzo i dati ricevuti
	do{

		analized = DataAnalyse();
		
		whileCycles++;

		if (whileCycles >= _stdUART_FIFOS_DEPTH_)
		{
			analized = true;//break;
		}
		else if (m_enableMultiMessagesAnalisy)
		{
			analized = false;
		}
		checkTimeOutElapsed();

		if (m_pUart->TransmitBufferOverflow())
		{
			setError(stdUart_TransmitBufferOverflowError);
			m_pUart->rstTransmitBufferOverflow();
			//stdUartProtocolAbstraction::restoreIntialCondition();
			
		}
		if(m_pUart->ReceptionBufferOverflow())
		{
			setError(stdUart_ReceptionBufferOverflowError);
			m_pUart->rstReceptionBufferOverflow();
			stdUartProtocolAbstraction::restoreIntialCondition();
		}
	}while(!analized && !m_pUart->ReceptionBufferIsEmpty());

	if(m_rstErrorTimer.Match())
	{
		rstError();
		m_rstErrorTimer.Preset(1000);
	}
}


bool stdUartProtocolAbstraction::DataAnalyse()
{
int errorBuf; 
bool hwErrorRecognised; 

 	if(m_pUart == NULL)
		return false;
 
	errorBuf = m_pUart->GetReceptionError(); 

	if( errorBuf )
	{
		hwErrorRecognised = false;	
		
		if((errorBuf & UartDeviceAbstract::ReceptionError) != 0 )
		{
			hwErrorRecognised = true;
			setError((int)stdUart_Hw_ReceptionError);
		}
		if((errorBuf & UartDeviceAbstract::ParityError) != 0)
		{
			hwErrorRecognised = true;
			setError((int)stdUart_Hw_ParityError);
		}
		if((errorBuf & UartDeviceAbstract::FrameError) != 0)
		{
			hwErrorRecognised = true;
			setError((int)stdUart_Hw_FrameError);
		}
		if((errorBuf & UartDeviceAbstract::OverrunError) != 0)
		{
			hwErrorRecognised = true;
			setError((int)stdUart_Hw_OverrunError);
		}
		if((errorBuf & UartDeviceAbstract::OverrunRxFifo) != 0)
		{
			hwErrorRecognised = true;
			setError((int)stdUart_Hw_RxFifoOverrun);
		}
		if((errorBuf & UartDeviceAbstract::OverrunTxFifo) != 0)
		{
			hwErrorRecognised = true;
			setError((int)stdUart_Hw_TxFifoOverrun);
		}
		if((errorBuf & UartDeviceAbstract::NoDevice) != 0)
		{
			hwErrorRecognised = true;
			setError((int)stdUart_Hw_NoDevice);
		}

		if((errorBuf & UartDeviceAbstract::OverflowRxBuff) != 0)
		{
			hwErrorRecognised = true;
			setError((int)stdUart_Hw_OverflowRxBuff);
			
		}
		if (!hwErrorRecognised)
		{
			setError((int)stdUart_Hw_ErrorUnknown);
		}

		m_pUart->ClearReceptionError();
		setReceiverState(stdUart_waitStx);
		stopRxTimeoutTimer();	
	}
	return (errorBuf != 0);
}


bool stdUartProtocolAbstraction::checkTimeOutElapsed()
{
	bool error = false;
	// + ricezioni dati in modalità Listening
	if (timeoutRxElapsed())
	{	
	
		/*cleardevice();
		setcolor(BLACK);
		charset_select("xm5x6");
		int i;
		
		gprintfxy(0,0, "I'm in timeout rx");
		gprintfxy(0,6,"Lr:%d,Lp:%d,Lb:%d", getNumDataReceived(), getPacketLength(), getNumBytesReceived());
	
		i = getPacketLength();
		gprintfxy(0,12,"%d,%d,%d,%d,%d,%d,%d,%d",bufferLastDataReceived()[i-1],bufferLastDataReceived()[i-2],bufferLastDataReceived()[i-3],bufferLastDataReceived()[i-4],bufferLastDataReceived()[i-5],bufferLastDataReceived()[i-6],bufferLastDataReceived()[i-7],bufferLastDataReceived()[i-8]);
		gprintfxy(0,18,"%d,%d,%d,%d,%d,%d,%d,%d",bufferLastDataReceived()[i-9] ,bufferLastDataReceived()[i-10],bufferLastDataReceived()[i-11],bufferLastDataReceived()[i-12],bufferLastDataReceived()[i-13],bufferLastDataReceived()[i-14],bufferLastDataReceived()[i-15],bufferLastDataReceived()[i-16]);
		gprintfxy(0,24,"%d,%d,%d,%d,%d,%d,%d,%d",bufferLastDataReceived()[i-17],bufferLastDataReceived()[i-18],bufferLastDataReceived()[i-19],bufferLastDataReceived()[i-20],bufferLastDataReceived()[i-21],bufferLastDataReceived()[i-22],bufferLastDataReceived()[i-23],bufferLastDataReceived()[i-24]);

		
		gprintfxy(0,30,"%d,%d,%d,%d,%d,%d,%d,%d",bufferLastDataReceived()[i-25],bufferLastDataReceived()[i-26],bufferLastDataReceived()[i-27],bufferLastDataReceived()[i-28],bufferLastDataReceived()[i-29],bufferLastDataReceived()[i-30],bufferLastDataReceived()[i-31],bufferLastDataReceived()[i-32]);
		gprintfxy(0,36,"%d,%d,%d,%d,%d,%d,%d,%d",bufferLastDataReceived()[i-33],bufferLastDataReceived()[i-34],bufferLastDataReceived()[i-35],bufferLastDataReceived()[i-36],bufferLastDataReceived()[i-37],bufferLastDataReceived()[i-38],bufferLastDataReceived()[i-39],bufferLastDataReceived()[i-40]);
		gprintfxy(0,42,"%d,%d,%d,%d,%d,%d,%d,%d",bufferLastDataReceived()[i-41],bufferLastDataReceived()[i-42],bufferLastDataReceived()[i-43],bufferLastDataReceived()[i-44],bufferLastDataReceived()[i-45],bufferLastDataReceived()[i-46],bufferLastDataReceived()[i-47],bufferLastDataReceived()[i-48]);
		gprintfxy(0,48,"%d,%d,%d,%d,%d,%d,%d,%d",bufferLastDataReceived()[i-49],bufferLastDataReceived()[i-50],bufferLastDataReceived()[i-51],bufferLastDataReceived()[i-52],bufferLastDataReceived()[i-53],bufferLastDataReceived()[i-54],bufferLastDataReceived()[i-55],bufferLastDataReceived()[i-56]);
		gprintfxy(0,54,"%d,%d,%d,%d,%d,%d,%d,%d",bufferLastDataReceived()[i-57],bufferLastDataReceived()[i-58],bufferLastDataReceived()[i-59],bufferLastDataReceived()[i-60],bufferLastDataReceived()[i-61],bufferLastDataReceived()[i-62],bufferLastDataReceived()[i-63],bufferLastDataReceived()[i-64]);
		*/
		setError(stdUart_TimeOutErrorR, true);
		stdUartProtocolAbstraction::restoreIntialCondition();
		error = true;
		
	// + Invio comando al modulo e attesa dellla risposta
	}if (timeoutTxElapsed())
	{
		setError(stdUart_TimeOutErrorT, true);
		stdUartProtocolAbstraction::restoreIntialCondition();
		//ClearTransmissionBuffer();
		error = true;
	}

	return error;
}

/**
Sets le last error occured.
@param error king of error
@param pushToQueue if true push the current error to the fifo's errors, if false just set the last error occurred. 
*/
void stdUartProtocolAbstraction::setError(int error, bool pushToQueue)
{

	m_rstErrorTimer.Preset(1000);	
	if (pushToQueue || error != m_lastError)
	{
		m_lastError = error;
		if (!m_error.full())
		{
			m_error.push_back(m_lastError);
		}else
		{
		
			m_error.pop_front(error);
			m_error.push_back(m_lastError);
		}
	}else
	{
		m_lastError = error;
	}
		
}

/**
Resets the last error and its fifo.
*/
void stdUartProtocolAbstraction::rstError()
{
	m_lastError = stdUart_NoError;
	m_error.clear();
}

/**
@return the last error occurred.
*/
int stdUartProtocolAbstraction::getLastError()
{
	return m_lastError;
}

/**
Pop the error from the fifo's errors.

@return the error, if the fifo is empty returns stdUart_NoError
*/
int stdUartProtocolAbstraction::popError()
{
	int error = stdUart_NoError;
	if(!m_error.empty())
	{
		m_error.pop_front(error);
	}
	return error;
}


/**
@return always true
*/
bool stdUartProtocolAbstraction::IsTransmissionAvailable()
{
	return true;
}

/**
@return always true
*/
bool stdUartProtocolAbstraction::IsReceptionAvailable()
{
	return true;
}

/**
Sends a command
@param pdata pointer to the command data
@param len command length
@param timeout time to wait before raise timeout TX error. If 0 the timer does NOT start.
@return the number of bytes transmitted. If 0 no bytes were transmitted.
*/
int stdUartProtocolAbstraction::SendCommand(unsigned char *pdata, int len, int timeOut)
{
	if(m_pUart == NULL)
			return 0;

	if (m_pUart->GetFreeBytesInTransmissionBuffer() < len)
	{
		setError(stdUart_TransmitBufferOverflowError);
		return 0;
	}

	//QueueUart::TransmitLock();
	m_pUart->TransmitData(pdata, len);
	
	if (timeOut > 0)
	{
		stdUartProtocolAbstraction::setTimeoutTx(timeOut);
		stdUartProtocolAbstraction::startTxTimeoutTimer();
	}
	
	//QueueUart::TransmitUnlock();
	//m_pUart->StartTransmission();
	return len;
}

void stdUartProtocolAbstraction::setLastDataReceivedBuffer(int i)
{
	if (m_lastDataReceived != NULL)
	{
		delete m_lastDataReceived;
	}
	m_bufferLastDataReceivedDimension = i;
	m_lastDataReceived = new byte[i];
}

/**
Adds bytes to the reception buffer.
@param val pointer to the byte array to add
@param start_index index of insertion
@param dim number of bytes to add
@return true if the data can be added to the reception buffer, false otherwise. 
If return false the error stdUart_DataReceivedBufferOverflow is pushed to the error fifo
@see stdUartProtocolErrors
@see setError
*/
bool stdUartProtocolAbstraction::addDataToReceivingBuffer(byte *val, int startIndex, int dim)
{
	int i;
	if ((m_lastDataReceived == NULL) || ((startIndex + dim) >= m_bufferLastDataReceivedDimension) || (startIndex < 0))
	{
		setError(stdUart_DataReceivedBufferOverflow);
		return false;
	}
	for (i = startIndex; i < startIndex + dim; i++)
	{
		m_lastDataReceived[i] = val[i - startIndex];
	}
	return true; 
}

bool stdUartProtocolAbstraction::addDataToReceivingBuffer(byte val, int startIndex)
{
	if ((m_lastDataReceived == NULL) || ((startIndex + 1) >= m_bufferLastDataReceivedDimension) || (startIndex < 0))
	{
		setError(stdUart_DataReceivedBufferOverflow);
		return false;
	}
	m_lastDataReceived[startIndex] = val;
	
	return true; 
}

/**
Sets the received opcode. The opcode are pushed in a fifo.
@param code received opcode
*/
void stdUartProtocolAbstraction::setReceivedOpCode(int opCode)
{ 
	m_lastReceivedOpCode = opCode;
	if(!m_receivedOpCode.full())
	{
		m_receivedOpCode.push_back(m_lastReceivedOpCode);
	}else
	{
		m_receivedOpCode.pop_front(opCode);
		m_receivedOpCode.push_back(m_lastReceivedOpCode);
	}
}

/**
Pop the received opcode
@param opCode received opcode (valid only if the function returns true)
@return true if the fifo is not empty, false otherwise. 
*/
bool stdUartProtocolAbstraction::popPacketReceived(int &opCode)
{ 
	if(!m_receivedOpCode.empty())
	{
		m_receivedOpCode.pop_front(opCode);
		return true;
	}
	
	if(!m_multiMessages.data.empty() || !m_multiMessages.dataLength.empty())
	{
		m_multiMessages.data.clear();
		m_multiMessages.dataLength.clear();
		setError(stdUart_DataReceivedBufferOverflow);
	}
	return false;
}

/**
@return the number of opcode fifoed
*/
int stdUartProtocolAbstraction::getNewPacketReceived()
{
	return m_receivedOpCode.numItem();
}

/**
Resets the opcode fifo
*/
void stdUartProtocolAbstraction::rstNewPacketReceived()
{
	m_multiMessages.data.clear();
	m_multiMessages.dataLength.clear();
	return m_receivedOpCode.clear();
}

/**
Calculates the checksum as sum of bytes.
@param data pointer to the data to use to calculate the checksum
@param len length of data
@return the checksum value
*/
byte stdUartProtocolAbstraction::calculateChecksum(byte *data, int len)
{
	byte checksum = 0;
	int i;
	for(i = 0; i < len; i++)
	{
		checksum += data[i];
	}
	return checksum;
}


/**
Pops recived message from the fifo.
@param data array where the recived data will be stored
@param dataLength length of the popped message (data array dimension)
@return true is the array data are popped, false if the fifo are empty
*/
bool stdUartProtocolAbstraction::popMultiMessage(byte* data, int &dataLength)
{
	int i;
	if (m_multiMessages.dataLength.empty())
	{
		m_multiMessages.data.clear();
		m_receivedOpCode.clear();
		dataLength = 0;
		setError(stdUart_DataReceivedBufferOverflow);
		return false;
	}

	m_multiMessages.dataLength.pop_front(dataLength);

	for (i = 0; i < dataLength; i++)
	{
		if (m_multiMessages.data.empty())
		{
			dataLength = 0;
			m_multiMessages.dataLength.clear();
			m_receivedOpCode.clear();
			setError(stdUart_DataReceivedBufferOverflow);
			return false;
		}
		m_multiMessages.data.pop_front(data[i]);
	}

	return true;
}

/**
Pushes data into the multi messages fifo.
@param data array of the data to be stored
@param dataLength length of data that have to be stored
@return true is the array data are peshed correctly, false if the fifo are full
*/

bool stdUartProtocolAbstraction::pushMultiMessages(byte* data, int dataLength)
{
	int i;
	
	
	for (i = 0; i < dataLength; i++)
	{
		if(m_multiMessages.data.full())
		{
			m_multiMessages.dataLength.clear();
			m_multiMessages.data.clear(); 
			m_receivedOpCode.clear();
			setError(stdUart_DataReceivedBufferOverflow);
			return false;
		}
		m_multiMessages.data.push_back(data[i]);
	}

	if(m_multiMessages.dataLength.full())
	{
		m_multiMessages.dataLength.clear();
		m_multiMessages.data.clear(); 
		m_receivedOpCode.clear();
		setError(stdUart_DataReceivedBufferOverflow);
		return false;
	}
	m_multiMessages.dataLength.push_back(dataLength);

	return true;
}



void stdUartProtocolAbstraction::setBypassMode(bool active , UartDeviceAbstract *__pUartDevice)
{
	if (__pUartDevice != NULL)
	{
		m_bypassMode = active;
	}else
	{
		m_bypassMode = false;
	}
	m_pBypassUart = __pUartDevice;
}






































