/**
@file		protocolSAUandEmgz.h
@brief		Class to manage the serial communication between the board SAU-7000 and SAU-7003.

@author		Nicola
@date		12/10/2011
@version	01.0
*/


#ifndef __PROTOCOL_SAU_AND_EMGZ_H__
#define __PROTOCOL_SAU_AND_EMGZ_H__

#define _MAX_PACKET_LENGTH_ 110

#include "global.h"
#include "UartAbstract.h"
#include "protocolStdUart.h"
#include "protocolSAUandPC.h"
#include "channels.h"
#include "keyboard.h"
#include "sound.h"

/**
STX value
*/
#define __STX_EMGZ '\n'

/**
Class to manage the serial communication between the board HRC-7000 and the PC.

When a packet is decode a messages is pushed in the messages' fifo as well as an error occured. 
The messages have to be popped from the board manager using the propriety Sau7003Protocol::getMsg().

The protocol packets are builded as follow:
- (A) STX = '\\n'  - byte
- (B) OPCODE	  - byte	
- (C) DATA LENGTH - byte 
- (D) DATA        - DATA LENGTH bytes
- (E) CHECKSUM = (B)+(C)+(D)  - byte

Data length can be also 0, in this case the next data received will be the checksum.
Data length is the length of the field data.

The uart communication specifications are the follows:
- Baud rate: 230400
- Data bits: 8
- Stop bits: 1
- Parity: None
- Flow Control: None
*/
class SauAndEmgzProtocol: public stdUartProtocolAbstraction
{
	public:
		/**
		Commands opcode sent from the SAU7000 to the board SAU7003
		*/
		typedef enum
        {
            NO_CMD_FROM_HOST = ' ',
            GET_INFO = 'V',					/**< request board info @see getInfoCmdDecode @see sendInfo*/
            GET_CAL_EMG = 'g',				/** not used*/
			GET_CAL_IMP = 'i',				/** not used*/
            GET_STATUS = 'S',				/**< request board status @see getStatusCmdDecode @see sendStatus*/
            SET_CAL_EMG = 'G',				/** not used*/
            SET_CAL_IMP = 'I',				/** not used*/
            START_ACQUISITION = 'B',		/**< start acquisition @see _startAcquisitionCmdDecode @see sendAcqStarted*/
            STOP_ACQUISITION = 'F',			/**< stop acquisition @see _stopAcquisitionCmdDecode @see sendAcqStopped*/
			CHECK_CONTACT_IMP ='C',			/**< sample the contact impedance */
			SET_FILTERING	= 'X',			/**< set filtering */
			JUMP_TO_LOADER = 'J',		/**< jump to loader @see _JumpToLoaderCmdDecode @see sendJumpedToLoader*/
        }Cmd_ToBoard;

		/**
		Commands opcode sent from the HRC-7000 to the PC
		*/
        typedef enum
        {
           	NO_CMD_FROM_BOARD = ' ',
            INFO = GET_INFO,								/**< board info @see sendInfo*/
            CAL_IMP = GET_CAL_IMP,							/**< not used*/
            CAL_EMG = GET_CAL_EMG,							/**< not used*/
            ACKNOWLEDGE = 'A',								/**< not used*/
            ACQUISITION_STARTED = START_ACQUISITION,		/**< acquisition started @see sendAcqStarted*/	
            ACQUISITION_STOPPED = STOP_ACQUISITION,			/**< acquisition stopped @see sendAcqStopped*/
            STATUS = GET_STATUS,							/**< board status @see sendStatus*/
            ERRORS = 'E',									/**< error packets @see sendErrors*/
            IMP_SAMPLES = '1',								/**< impedance samples packet @see sendImpSamples*/
            EMG_SAMPLES = '2',								/**< emg packet @see sendEmgSamples*/
            JUMPED_TO_LOADER = JUMP_TO_LOADER,				/**< application firmware jumped to loader @see sendJumpedToLoader*/
        }Cmd_FromBoard;

		/**
		Mesasges sent from the decoding protocol module to the board manager module.
		@see sendMsg
		@see getMsg
		*/
		enum Msg
		{
			Msg_noMsg = 0,

			Msg_infoReceived,					/**< info packet reveived*/
			Msg_emgCalReceived,					/**< not used*/
			Msg_impCalReceived,					/**< not used*/
			Msg_statusReceived,					/**< status packet received*/
            Msg_acknowledge,					/**< not used*/
            Msg_notAcknowledge,					/**< not used*/
            Msg_errorPacketReceived,
            Msg_acquisitionStarted,				/**< request to start acquisition*/
            Msg_acquisitionStopped,				/**< request to stop acquisition */
            Msg_emgSamplesReceived,
            Msg_contactImpSamplesReceived,
            Msg_jumpedToLoader,					/**< request to jump to loader*/
			
			Msg_packetsLostEmg,					/**< packets lost*/
			Msg_packetsLostImp,	
			Msg_timoutRx,						/**< timeout RX elapsed*/
			Msg_timoutTx,						/**< timeout TX elapsed*/
			Msg_wrongStx,						/**< received wrong STX*/
			Msg_opcodeUnknown,					/**< unknown opcode received*/
			Msg_checkSumError,					/**< checksum error*/
			Msg_HwError,						/**< uart hardware error @see HwErrors*/
			Msg_transmitBufferOverflowError,	/**< transmission buffer overflow error*/
			Msg_receptionBufferOverflowError,	/**< reception buffer overflow error*/
			Msg_dataReceivedBufferOverflow,		/**< data received buffer overflow errror*/	
			Msg_EmgSamplesBufferOverflow,
			Msg_ImpEmgSamplesBufferOverflow,
			Msg_unknownError					/**< unknown error*/
		};

		/**
		Reasons acquisition stopping
		*/
		enum AWhySamplingWasStopped
		{
			Stopped_byHostCommand = 0,						/**< acquisition stopped because received stop commad from host */
			Stopped_byTimeOutOnSampling,
			Stopped_bySerialAdcError,
			Stopped_byUartTxBufferOverflow,
		};

		/**
		uart hardware error
		@see m_lastHwError
		*/
		enum Uart_Hw_Errors
		{
			Uart_Hw_NoError 			= 0x00,
			Uart_Hw_ReceptionError 		= 0x01,
			Uart_Hw_ParityError    		= 0x02,
			Uart_Hw_FrameError 			= 0x04, 
			Uart_Hw_OverrunError 		= 0x08,
			Uart_Hw_NoDevice			= 0x10,
			Uart_Hw_ErrorUnknown		= 0x20,
			Uart_Hw_OverflowRxBuff		= 0x40,
			Uart_Hw_OverrunFpgaFifo		= 0x80
		};

		enum EmgzVoltageMeasures
		{
		    _VCC_EMGZ_ = 0,
		    _5VDC_EMGZ_,
		    _VREF_EMGZ_,
		    _NUM_VOLTAGE_MEASURE_EMGZ_,
		};


		enum EMGZ_Status
		{
			EMGZstatus_idle = 0,/**< idle status. The board is waiting for command*/
			EMGZstatus_sampling,/**< sampling status. The board is sampling and sending the data to the PC */
			EMGZstatus_error,/**< error status.*/
			EMGZstatus_wait,/**< wait status. An error occured and time has to elapse before recovering it.*/
			EMGZstatus_unknown,
		};
		
		SauAndEmgzProtocol(UartDeviceAbstract *__pUartDevice, int __rxProtoBufSize = __UART_MAX_MESSAGE_LENGHT__);
		~SauAndEmgzProtocol();

		inline void manager();

		virtual void TurnOn(){stdUartProtocolAbstraction::TurnOn();}
		virtual void TurnOff(){stdUartProtocolAbstraction::TurnOff();}

		virtual void SetBaudRate(UartDeviceAbstract::UARTBaudRate baudrate) { m_pUart->SetBaudRate(baudrate, true); }

		bool isBypassModeActive(){ return stdUartProtocolAbstraction::isBypassModeActive();}
		void setBypassMode(bool active , UartDeviceAbstract *__pUartDevice){ stdUartProtocolAbstraction::setBypassMode(active, __pUartDevice);}
		/**
		@return the number of bytes in the transmission buffer.
		*/
		int GetNumBytesInTransmissionBuffer(){return stdUartProtocolAbstraction::GetNumBytesInTransmissionBuffer();};

		
		int sendInfoRequest();
		int sendStartAcq();
		int sendStopAcq();
		int sendStatusRequest();
		int sendFilteringSetting(byte __LPF, byte __NOTCHF, byte __HPF);
		int sendJumpedToLoaderRequest();
		int sendCheckContactImpedances(bool val);

		char* getAppRevision(){ return m_appRevision;}
		char* getAppDate(){ return m_appDate;}
		char* getHwRevision(){return m_hwVersion;}

		word getVoltageMeasure(EmgzVoltageMeasures v){ return m_voltageMeasure[v];}
		word getCurrentMeasure(){ return m_currentMeasure;}

		byte getStatus(){return m_emgzStatus;}
		word getNumErrors(){ return m_emgzNumErrorOccurred;}
		byte getOverCurrent(){ return m_overcurrent;}
		
			
		virtual int getMsg();

		/**
		@return the last command sent to the PC 
		@see AcqHrCmd_FromBoardToPc
		*/
		Cmd_ToBoard getLastCmdSent(){ return m_lastCmdSent;};


		/**
		@return the last uart harware error
		*/
		byte getLastHwError(){return m_lastHwError;};
		/**
		resets the last uart hardware error
		*/
		void rstLastHwError(){m_lastHwError = Uart_Hw_NoError;};

		/**
		resets the number of impedance packets sent to the PC
		*/
		void rstImpPacketsReceived(){ m_numImpPacketsReceived = 0;};
		/**
		resets the number of pressure and pH packets sent to the PC
		*/
		void rstEmgPacketsReceived(){ m_numEmgPacketsReceived = 0;};

		bool getReadingElectrodeImpedanceActive(){return m_readElectrodeImpedanceActive;}

		bool sampleContactImpEnable(){return ((m_sampleContactImpEnable == 0) ? false : true);}

		byte getPacketsLostImp(){return m_packetLostImp;}
		byte getPacketsLostEmg(){return m_packetLostEmg;}

		void getLastEmgzBoardErrors(word &comErr, byte &hwErr, word &genErr);

		byte getLPFiltering(){ return m_LPF_type;}
		byte getNOTCHFiltering(){return m_NOTCHF_type;}
		byte getHPFiltering(){return m_HPF_type;}

		void setProtocolPc(SauPcProtocol *__sauPcProtocol){m_sauPcProtocol = __sauPcProtocol;}
		
		void registerClearAllBuffFunct(void (*__pFClearAllBuffers)()){m_pFClearAllBuffers = __pFClearAllBuffers;}
protected:
	
		void (*m_pFClearAllBuffers)();	

		virtual bool DataAnalyse();

		virtual int sendMsg(int i);
		
		bool checkOpCode (byte data);

		bool checksum(byte receivedChk, byte opCode);

		inline int _infoCmdDecode();
		inline int _calImpCmdDecode();
		inline int _calEmgCmdDecode();
		inline int _acknowledgeCmdDecode();
		inline int _errorsCmdDecode();
		inline int _statusCmdDecode();
		inline int _impSampleDecode();
		inline int _emgSampleDecode();	
		inline int _acquisitionStartedCmdDecode();
		inline int _acquisitionStoppedCmdDecode();
		inline int _jumpToLoaderCmdDecode();

		byte checkPacketsLost(byte numPacketsTx, byte numPacketsRx);
		
	private:
		/**
		Messages' fifo
		*/
		CSmallRingBuf<int , _stdUART_FIFOS_DEPTH_> m_message;

		/**
		last command sent to the PC
		*/
		Cmd_ToBoard m_lastCmdSent;

		/**
		used in debug to send the cathetr packet to the PC
		*/
		/**
		last uart hardware error.
		*/
		byte m_lastHwError;
		
		byte m_numImpPacketsReceived;
		byte m_numEmgPacketsReceived;

		/**
		Last Message recieved. Its length as to be as the longest message.
		*/
		byte m_messageReceived[__UART_MAX_MESSAGE_LENGHT__];

		int m_messageReceivedLenght;
		
		DecTimer m_testTimer;

		char m_appRevision[30];
		char m_appDate[30];
		char m_hwVersion[30];

		word m_voltageMeasure[_NUM_VOLTAGE_MEASURE_EMGZ_];
		word m_currentMeasure;

		byte m_emgzStatus;
		word m_emgzNumErrorOccurred;
		byte m_overcurrent;

		bool m_readElectrodeImpedanceActive;

		byte m_sampleContactImpEnable;

		byte m_packetLostImp;
		byte m_packetLostEmg;

		word m_emgzBoardLastComErrors;
		word m_emgzBoardLastGenericError;
		byte m_emgzBoardLastHwError;

		byte m_LPF_type;
		byte m_NOTCHF_type;
		byte m_HPF_type;

		SauPcProtocol *m_sauPcProtocol;
		word m_emgSampleDummy[_NUM_HR_EMG_SAMPLES_PER_PACKET_];

		int m_opCodeDataAnalyse;
		int m_lastRemoteIDDataAnalyse;
		int m_numBypassByteTransmittedDataAnalyse;
};


#endif










































