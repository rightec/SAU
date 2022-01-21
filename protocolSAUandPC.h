/**
@file		protocolSauPc.h
@brief		Class to manage the serial communication between the board HRC-7000 and the PC.

@author		Nicola
@date		18/01/2011
@version	01.1
*/


#ifndef __PROTOCOL_SAUPC_H__
#define __PROTOCOL_SAUPC_H__

#include "global.h"
#include "UartAbstract.h"
#include "protocolStdUart.h"
#include "channels.h"
#include "keyboard.h"
#include "iWRAP_BLUEGIGA.h"
#include "wtp.h"
//#include "protocol_uC.h"


/**
STX value
*/
#define SAUPC_STX '\n'

#define _TIME_AFTER_STX	50 //Former was 100. With 50 it should fail. 21/01/2022

struct overpressure_bitF_t
{
    word prs_01 :1;
    word prs_02 :1;
    word prs_03 :1;
    word prs_04 :1;

    word prs_05 :1;
    word prs_06 :1;
    word prs_07 :1;
    word prs_08 :1;

    word gen_01 :1;
    word gen_02 :1;

    word unused_01 :1;
    word unused_02 :1;
    word unused_03 :1;
    word unused_04 :1;
    word unused_05 :1;
    word unused_06 :1;

    overpressure_bitF_t& operator= (const overpressure_bitF_t &__st)
    {
        for( word i = 0; i < sizeof(overpressure_bitF_t); i++)
            *((byte*)(((byte*)this)+i)) = *((byte*)(((byte*)&__st)+i));
        return *this;
    }
};


/**
Class to manage the serial communication between the board SAU-7000 and the PC.

When a packet is decode a messages is pushed in the messages' fifo as well as an error occured. 
The messages have to be popped from the board manager using the propriety SauPcProtocol::getMsg().

The protocol packets are built as follow:
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
class SauPcProtocol: public stdUartProtocolAbstraction
{
	public:
		
		enum COMM_type
		{
			comm_FTDI_USBtoSERIAL,
			comm_BT,
		};
		
		/**
		Commands opcode sent from the PC to the board HRC-7000
		*/
		typedef enum
        {
            SAUPC_NO_CMD_FROM_HOST = ' ',
            SAUPC_GET_INFO = 'I',				/**< request board info @see sauPc_getInfoCmdDecode @see sendSauPcInfo*/
            SAUPC_GET_STATUS = 'S',				/**< request board status @see sauPc_getStatusCmdDecode @see sendSauPcStatus*/
            SAUPC_START_ACQUISITION = 'A',		/**< start acquisition @see sauPc_startAcquisitionCmdDecode @see sendSauPcAcquisitionStarted*/
            SAUPC_STOP_ACQUISITION = 'C',		/**< stop acquisition @see sauPc_stopAcquuisitionCmdDecode @see sendSauPcAcquisitionStopped*/
			
			SAUPC_WTH_SET_SPEED = 'H',			/**< set the WTH speed  
												 @see sauPc_setWthSpeedCmdDecode @see sendSauPcWth*/
			SAUPC_WTH_SET_STATUS = 'h',			/**< set the status of the WTH   
												 @see sauPc_setWthStatusCmdDecode @see sendSauPcWth*/  
			SAUPC_WTP_SET_SPEED = 'W',			/**< set the WTP speed  
												 @see sauPc_setWtpSpeedCmdDecode @see sendSauPcWtp*/
			SAUPC_WTP_SET_STATUS = 'w',			/**< set the status of the WTP   
												 @see sauPc_setWtpStatusCmdDecode @see sendSauPcWtpSpeed*/
			SAUPC_BUBBLE_SET_STATUS = 'B',		/**< set the bubble status 
												@see sendSauPcBubble @see sauPc_setBubbleStatusCmdDecode*/	 

			SAU_PC_BT_BYPASS_MODE = 'Y',
			
			SAUPC_SET_DISPLAY = 'D',		/**< set the display backlight, contrast and brightness */
			SAUPC_GET_DISPLAY = 'd',		/**< pc asks for display backlight, contrast and brightness */

			SAUPC_EMGZ_CONTAC_IMP 		= 'z',
			SAUPC_EMGZ_SET_FILTERING	= 'X',
			
            SAUPC_JUMP_TO_LOADER = 'J',		/**< jump to loader @see sauPc_JumpToLoaderCmdDecode @see sendSauPcJumpedToLoader*/
            SAUPC_EMGZ_JUMP_TO_LOADER = 'j',
            
            SAUPC_GET_CAL_WEIGHT = 't',		
            SAUPC_GET_CAL_PRS = 'p',		
            SAUPC_GET_CAL_EMG = 'g',		/** not used*/
            SAUPC_GET_CAL_GENERIC = 'n',	/** not used*/

            SAUPC_SET_CAL_WEIGHT = 'V',		
            SAUPC_SET_CAL_PRS = 'P',		
            SAUPC_SET_CAL_EMG = 'G',		/** not used*/
            SAUPC_SET_CAL_GENERIC = 'N',	/** not used*/
            SAUPC_ZERO_CHAN = 'Z',              /** not used*/
            
			SAUPC_SET_WATCHDOG = 'T',

			SAUPC_SET_MOTOR_CAL = 'M',
			SAUPC_GET_MOTOR_CAL = 'm',

			SAUPC_SET_OVERPRESSURE = 'O',

			SAUPC_WTP_SET_SPEED_CALIBRATION = 'K'
			
		}SauPcCmd_FromPcToBoard;

		/**
		Commands opcode sent from the HRC-7000 to the PC
		*/
        typedef enum
        {
            SAUPC_NO_CMD_FROM_BOARD = ' ',
            SAUPC_INFO = SAUPC_GET_INFO,							/**< board info @see sendSauPcInfo*/
         	SAUPC_STATUS = SAUPC_GET_STATUS,						/**< board status @see sendSauPcStatus*/
         	
            SAUPC_ACQUISITION_STARTED = SAUPC_START_ACQUISITION,	/**< acquisition started @see sendSauPcAcquisitionStarted*/	
            SAUPC_ACQUISITION_STOPPED = SAUPC_STOP_ACQUISITION,		/**< acquisition stopped @see sendSauPcAcquisitionStopped*/
			
			SAUPC_WTH = 'H',									/**< current WTH speed & status @see sendSauPcWth*/
			SAUPC_WTP = 'W',									/**< current WTP speed & status @see sendSauPcWtp*/							 

			SAUPC_BUBBLE_STATUS = SAUPC_BUBBLE_SET_STATUS,		/**< current Bubble status @see sendSauPcBubble*/

			SAUPC_DISPLAY = SAUPC_GET_DISPLAY, 					/**< display setting: contrast, backlight and brightness*/

			SAUPC_ERRORS = 'E',										/**< error packets @see sendSauPcErrors*/
            SAUPC_GEN_SAMPLES = '1',								/**< packets containing emg e generic channels samples  @see sendSauEmgGenSamples*/
			SAUPC_PRS_SAMPLES = '2',									/**< packets containing pressure samples @see sendSauPrsSamples*/								 					
			SAUPC_WEIGHT_SAMPLES = '3',								/**< packets containing flow and volume samples @see sendSauPrsSamples*/
			SAUPC_EMG_SAMPLES = '4',
			SAUPC_EMG_CONTACT_IMP_SAMPLES = '5',
			SAUPC_REMOTE_CONTROL = 'R',								/**< packets containing the remote control's key pressed @see sendSauPcRemoteKey*/
												 						
            SAUPC_JUMPED_TO_LOADER = SAUPC_JUMP_TO_LOADER,         /**< application firmware jumped to loader @see sendSauPcJumpedToLoader*/
			SAUPC_EMGZ_JUMPED_TO_LOADER = SAUPC_EMGZ_JUMP_TO_LOADER,
			
			SAUPC_CAL_WEIGHT = SAUPC_GET_CAL_WEIGHT,		
            SAUPC_CAL_PRS = SAUPC_GET_CAL_PRS,		
            SAUPC_CAL_EMG = SAUPC_GET_CAL_EMG,		/** not used*/
            SAUPC_CAL_GENERIC = SAUPC_GET_CAL_GENERIC,		/** not used*/
            SAUPC_ACKNOWLEDGE = 'a',				/** command not ack*/
			SAUPC_CAL_MOTOR = SAUPC_GET_MOTOR_CAL,
			SAUPC_OVERPRESSURE_INFO = SAUPC_SET_OVERPRESSURE,

			SAUPC_WTP_SPEED_CALIBRATION = SAUPC_WTP_SET_SPEED_CALIBRATION
		}SauPcCmd_FromBoardToPc;

		/**
		Mesasges sent from the decoding protocol module to the board manager module.
		@see sendMsg
		@see getMsg
		*/
		enum SauPcMsg
		{
			SauPcMsg_noMsg = 0,
            
			SauPcMsg_infoAsked,						/**< info packet request*/
			SauPcMsg_statusAsked,					/**< status packet request*/
            SauPcMsg_startAcquisition,				/**< request to start acquisition*/
            SauPcMsg_stopAcquisition,				/**< request to stop acquisition */ 
            SauPcMsg_wtpSpeed,						/**< request to change the WTP speed */
            SauPcMsg_wthSpeed,						/**< request to change the WTH speed */
            SauPcMsg_wtpStatus,						/**< request to change the WTP status */
            SauPcMsg_wthStatus,						/**< request to change the WTH status */
            SauPcMsg_bubbleStatus,					/**< request to change the Bubble status */
            SauPcMsg_jumpToLoader,					/**< request to jump to loader*/
            SauPcMsg_emgz_jumpToLoader,
             
			SauPcMsg_WeightCalAsked,				/**< */
			SauPcMsg_PrsCalAsked,					/**< */
			SauPcMsg_GenericCalAsked,				/**< not used*/
			SauPcMsg_EmgCalAsked,					/**< not used*/
			
            SauPcMsg_WeightCalSet,						/**< */
            SauPcMsg_Prs_CalSet,					/**< */
            SauPcMsg_EmgCalSet,						/**< not used*/
            SauPcMsg_Generic_CalSet,				/**< not used*/
            SauPcMsg_zeroChan,  					/**< not used*/

			SauPcMsg_setDisplay,
			SauPcMsg_getDisplay,

			SauPcMsg_emgzContacImp,
			SauPcMsg_emgzSetFiltering,
			
			
			SauPcMsg_packetsLost,					/**< packets lost*/
			SauPcMsg_timoutRx,						/**< timeout RX elapsed*/
			SauPcMsg_timoutTx,						/**< timeout TX elapsed*/
			SauPcMsg_wrongStx,						/**< received wrong STX*/
			SauPcMsg_opcodeUnknown,					/**< unknown opcode received*/
			SauPcMsg_checkSumError,					/**< checksum error*/
			SauPcMsg_HwError,						/**< uart hardware error @see sauPc_HwErrors*/
			SauPcMsg_transmitBufferOverflowError,	/**< transmission buffer overflow error*/
			SauPcMsg_receptionBufferOverflowError,	/**< reception buffer overflow error*/
			SauPcMsg_dataReceivedBufferOverflow,	/**< data received buffer overflow errror*/	
			SauPcMsg_unknownError,					/**< unknown error*/

			SauPcMsg_BTcommunicationDropped,
			SauPcMsg_watchdogSetting,
			SauPcMsg_BT_bypassMode,
			SauPcMsg_setMotorCal,
			SauPcMsg_getMotorCal,

			SauPcMsg_setOverpressure,
			SauPcMsg_setWtpSpeedCalibration
		};


		/**
		uart hardware error
		@see m_lastHwError
		*/
		enum sauPc_HwErrors
		{
			sauPcUart_Hw_NoError 			= 0x00,
			sauPcUart_Hw_ReceptionError 	= 0x01,
			sauPcUart_Hw_ParityError    	= 0x02,
			sauPcUart_Hw_FrameError 		= 0x04, 
			sauPcUart_Hw_OverrunError 		= 0x08,
			sauPcUart_Hw_NoDevice			= 0x10,
			sauPcUart_Hw_ErrorUnknown		= 0x20,
			sauPcUart_Hw_OverflowRxBuff		= 0x40,
			sauPcUart_Hw_OverrunFpgaFifo	= 0x80
		};

		SauPcProtocol(UartDeviceAbstract *__pUartDevice, int __rxProtoBufSize = __UART_MAX_MESSAGE_LENGHT__);

		~SauPcProtocol();

		virtual void manager();

		virtual void TurnOn(){stdUartProtocolAbstraction::TurnOn();}
		virtual void TurnOff(){stdUartProtocolAbstraction::TurnOff();}

		virtual void setUartDevice(UartDeviceAbstract *pUart){m_pUart = pUart;}
		virtual void SetBaudRate(UartDeviceAbstract::UARTBaudRate baudrate) { m_pUart->SetBaudRate(baudrate, true); }
		virtual void SetHwFlwCtl(UartDeviceAbstract::UARThwFlwCnt flwCtl){m_pUart->SetHwFlwCtl(flwCtl, true);}
		
		bool isBypassModeActive(){ return stdUartProtocolAbstraction::isBypassModeActive();}
		void setBypassMode(bool active , UartDeviceAbstract *__pUartDevice){ stdUartProtocolAbstraction::setBypassMode(active, __pUartDevice);}
		/**
		@return the number of bytes in the transmission buffer.
		*/
		
		int sendSauPcInfo(word *ADuCFirmwareVersion, byte len, word FPGAinfo, char* emgAppRevision, char* emgAppDate, char* emgBoardHwRevision);
		int sendSauPcStatus(byte *status, int len);
		int sendSauPcAcquisitionStarted();
		int sendSauPcAcquisitionStopped(byte cause);
		
		int sendSauPcErrors(byte *err, int len);
		
		int sendSauPcGenSamples( word *genSamples, int genLen);
		int sendSauPcEmgSamples(word *emgSamples, int emgLen);
		int sendSauPcEmgContactImpSamples(word *emgContactImpSamples, int len);
		int sendSauPcPrsSamples(short *prsSamples, int prsLen);
		int sendSauPcWeightSamples(long *weightSamples, int weightLen);
		int sendSauPcWtp(word speed, byte status, byte litStatus, dword driverStatus);
		int sendSauPcWth(byte power, word speed, byte status, byte endSwitchesStatus, word step);
		
		int sendSauPcBubble(byte airStatus, byte clampStatus, bool detectionEn, bool airTestFailed, bool airDetected, bool airTestRunning);
		
		int sendSauPcRemoteKey(int key);
		int sendSauPcJumpedToLoader();
		int sendSauPc_emgz_JumpedToLoader();

		int sendAcknowledge(byte ack);

		int sendDisplaySetting(byte backlight, byte brightness, byte contrast);
		
		int sendPrsCalibrationValues(byte error);
		int sendWeightCalibrationValues(byte error);
		int sendGenericCalibrationValues(byte error);
		int sendEmgCalibrationValues(byte error);
		int sendMotorsCalibrationValues(wtpCalibration_t * __wtpCalValues, byte error);

		int sendOverpressureInfo(short __limit, overpressure_bitF_t __enChan, overpressure_bitF_t __overChan);

		int sendWtpSpeedCalibration(dword __speed, byte __error);
		
		virtual int getMsg();
		
		/**
		@return the last command sent to the PC 
		@see SauPcCmd_FromBoardToPc
		*/
		SauPcCmd_FromBoardToPc getLastCmdSent(){ return m_lastCmdSent;};

		/**
		@return the last uart hardware error
		*/
		byte getLastHwError(){return m_lastHwError;};
		/**
		resets the last uart hardware error
		*/
		void rstLastHwError(){m_lastHwError = sauPcUart_Hw_NoError;};

		/**
		resets the number of weight packets sent to the PC
		*/
		void rstWeightPacketsSent(){ m_numWeightPacketsSent = 0;};
		/**
		resets the number of pressure and pH packets sent to the PC
		*/
		void rstPrsPacketsSent(){ m_numPrsPacketsSent = 0;};
		/**
		resets the number of EMG and generic packets sent to the PC
		*/
		void rstEmgPacketsSent(){ m_numEmgPacketsSent = 0;};

		void rstGenPacketsSent(){ m_numGenPacketsSent = 0;};

		/**
		@return the last wth speed received 
		*/
		short lastWthSpeedReceived(){ return m_lastWthSpeed;};

		/**
		@return the last wtp speed received 
		*/
		word lastWtpSpeedReceived(){ return m_lastWtpSpeed;};

		byte lastWthPowered(){return m_lastWthPowered;}
		/**
		@return the last wth status received 
		*/
		byte lastWthStatusReceived(){ return m_lastWthStatus;};

		/**
		@return the last wth step received. 
		*/
		word lastWthStepReceived(){ return m_lastWthStep;};

		/**
		@return the last wtp status received 
		*/
		byte lastWtpStatusReceived(){ return m_lastWtpStatus;};

		/**
		@return the last Bubble status received 
		*/
		byte lastBubbleStatusReceived(){ return m_lastBubbleStatus;};

		bool bubbleDetectionEn(){return m_bubbleDetectionEn;}
		
		/**
		Sends a byte
		@return 1 if the byte was sent, 0 otherwise
		*/
		int sendByte(byte data){return stdUartProtocolAbstraction::SendCommand(&data, 1, 0);};

		byte getDisplayBacklight(){ return m_displayBacklight;};
		byte getDisplayBrightness(){ return m_displayBrightness;};
		byte getDisplayContrast(){return m_displayContrast;};
		
		byte getDisplayWriteSettingToEEPROM(){return m_displayWriteSettingToEEPROM;};

		byte enableEmgzToReadContactImp(){return m_enableEmgzToReadContactImp;}

		COMM_type getCOMM_type(){return m_COMM_type;}
		void setCOMM_type(COMM_type type){ m_COMM_type = type;}

		bool watchdogEnabled(){return m_watchdogIsEnable;}
		int getWatchDogTime(){ return m_watchdogTime;}
		
		byte getLPFiltering(){ return m_LPF_type;}
		byte getNOTCHFiltering(){return m_NOTCHF_type;}
		byte getHPFiltering(){return m_HPF_type;}

	
		void setWeightChan(GenericHwChanManager <long, _WEIGHT_BUFFER_LENGTH_> *w){ m_weightChan = w;};
		void setPrsChan(PrsHwChanManager <short, _PRS_BUFFER_LENGTH_> *prs){ m_prsChan = prs;};
		void setGenChanGainAndOffset(gainOffset_t *__pGen1ChanGainOffset, gainOffset_t *__pGen2ChanGainOffset){m_pGenChanGainOffset[0] = __pGen1ChanGainOffset;m_pGenChanGainOffset[1] = __pGen2ChanGainOffset;}
		
		overpressure_bitF_t *getOverpressureChanEn(){return &m_overpressureChanEn;}
		short getOverpressureLimit(){return m_overpressureLimit;} // in mmHg

		wtpCalibration_t *getWtpCalibrationValues(){ return m_wtpCalibrationValues;}

		dword getWtpSpeedCalibration(){return m_wtpSpeedCalibration;}
		
	protected:		
		inline int sauPc_getInfoCmdDecode();
		inline int sauPc_getStatusCmdDecode();
		inline int sauPc_startAcquisitionCmdDecode();
		inline int sauPc_stopAcquisitionCmdDecode();

		inline int sauPc_setWthSpeedCmdDecode();
		inline int sauPc_setWtpSpeedCmdDecode();
		inline int sauPc_setWthStatusCmdDecode();
		inline int sauPc_setWtpStatusCmdDecode();
		
		inline int sauPc_setBubbleStatusCmdDecode();

		inline int sauPc_JumpToLoaderCmdDecode();
		inline int sauPC_emgz_JumpToLoaderCmdDecode();
		
		inline int sauPc_getCalWeightCmdDecode();
		inline int sauPc_getCalPrsCmdDecode();
		inline int sauPc_getCalEmgCmdDecode();
		inline int sauPc_getCalGenericCmdDecode();
		
		inline int sauPc_setCalWeightCmdDecode();
		inline int sauPc_setCalPrs_CmdDecode();	
		inline int sauPc_setCalEmgCmdDecode();
		inline int sauPc_setCalGeneric_CmdDecode();
		
		inline int sauPc_zeroPrsCmdDecode();

		inline int sauPc_setDisplayCmdDecode();
		inline int sauPc_getDisplayCmdDecode();
		inline int sauPc_emgzContactImpCmdDecode();
		inline int sauPc_emgzSetFilteringCmdDecode();
		inline int sauPc_setWatchdogCmdDecode();

		inline int sauPc_setBT_bypassModeCmdDecode();

		inline int sauPc_setMotorCalCmdDecode();
		inline int sauPc_getMotorCalCmdDecode();

		inline int sauPc_setOverpressureCmdDecode();

		inline int sauPc_setWtpSpeedCalibrationCmdDecode();
		
		virtual bool DataAnalyse();

		virtual int sendMsg(int i);
		
		bool checkOpCode (byte data);

		bool checksum(byte receivedChk, byte opCode);
		
	private:
		/**
		Messages' fifo
		*/
		CSmallRingBuf<int, _stdUART_FIFOS_DEPTH_> m_message;

		/**
		last command sent to the PC
		*/
		SauPcCmd_FromBoardToPc m_lastCmdSent;

		/**
		last uart hardware error.
		*/
		byte m_lastHwError;
		
		byte m_numPrsPacketsSent;
		byte m_numWeightPacketsSent;
		byte m_numGenPacketsSent;
		byte m_numEmgPacketsSent;
		byte m_numEmgContactImpPacketsSent;
		
		short m_lastWthSpeed;
		word m_lastWtpSpeed;
		byte m_lastWthPowered;
		byte m_lastWthStatus;
		byte m_lastWtpStatus;
		word m_lastWthStep;
		byte m_lastBubbleStatus;
		bool m_bubbleDetectionEn;
		
		/**
		Last Message recieved. Its length as to be as the longest message.
		*/
		byte m_messageReceived[__UART_MAX_MESSAGE_LENGHT__];

		int m_messageReceivedLenght;

		byte m_displayBacklight;
		byte m_displayBrightness ;
		byte m_displayContrast;
		
		byte m_displayWriteSettingToEEPROM;
		byte m_enableEmgzToReadContactImp;

		COMM_type m_COMM_type;

		RingBuffer *m_ringBuffer;

		bool m_watchdogIsEnable;
		word m_watchdogTime;
		
		byte m_LPF_type;
		byte m_NOTCHF_type;
		byte m_HPF_type;

		int m_opCodeDataAnalyse;
		int m_lastRemoteIDDataAnalyse;
		int m_numBypassByteTransmittedDataAnalyse;

		GenericHwChanManager <long, _WEIGHT_BUFFER_LENGTH_> *m_weightChan;
		PrsHwChanManager <short, _PRS_BUFFER_LENGTH_> *m_prsChan;
		gainOffset_t *m_pGenChanGainOffset[_NUM_OF_GENERIC_CHAN_];

		overpressure_bitF_t m_overpressureChanEn;
		short m_overpressureLimit;

		wtpCalibration_t m_wtpCalibrationValues[WTP_CALIBRATION_BOUNDS];

		dword m_wtpSpeedCalibration;
};


#endif










































