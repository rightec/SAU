/**
@file		SAU7000_Manager.h
@brief		SAU7000 states machine.

			This class manage the machine states of the device:
			- receives messages from the communication protocol decoding classes 
			- manages the wth and wtp
			- .....
			
@author		Nicola Molinazzi
@date		18/01/2011
@version	01.00
*/

#ifndef _SAU_MANAGER_H_
#define _SAU_MANAGER_H_


#include "global.h"
#include "UartAbstract.h"
#include "UartFPGA.h"
#include "protocolSAUandPC.h"
#include "protocol_uC.h"
#include "protocolSauAndEmgz.h"
#include "keyboard.h"
#include "adc_nec.h"
#include "EEPROM_SPI.h"
#include "appmenu.h"
#include "SAU7000_ADuC_Source.h"
#include "ADuC_UpgradeManager.h"
#include "iWRAP_BLUEGIGA.h"
#include "iWRAP_BLUEGIGA_SPP.h"
#include "wth.h"
#include "wtp.h"
#include "IR_remote.h"
#include "Adc_SAU7000.h"
#include "EEPROM_SPI.h"

//#include "main.h"

/**
Time in msec that have to elapse to ask the status to all the ADuC while the acquisition is NOT running.
*/
#define _TIME_CHECK_ADuC_STATUS_					1000

#define _TIME_CHECK_EMGZ_STATUS_					2000


/**
Time in msec that have to elapse to ask the status to all the ADuC while the acquisition is running.
*/
#define _TIME_CHECK_ADuC_STATUS_WHILE_SAMPLING_	2500
#define _TIME_CHECK_EMGZ_STATUS_WHILE_SAMPLING_	2500
#define _TIME_WAIT_ADuC_RESP_DEFAULT					200	   // Basically was 500 - 21/01/2022

/**
Manage the machine state of board.

@see VoltageHwChanManager
@see VoltageHwChanManager
@see CurrentHwChanManager
@see GenericHwChanManager
@see PrsHwChanManager
@see Keyboard
@see SauPcProtocol
*/
class SAU7000_Manager
{
	public:
		/**
		Communication error with the ADuC microcontrollers.

		This errors are raised from the ADuC communication protocol decode class.
		The attribute m_ADuCComErrors[ADuC_NUM] (array of words) contains the communication errors
		with the specific ADuC and its value is calculated:
		m_ADuCComErrors[i] |= (1 << "error type");
		@see ADuC7061_use
		*/
		enum ADuC_CommunicationErrors
		{
			ADuC_ComError_noError = 0,/**< No Error.*/
			ADuC_ComError_infoADuCCorrupted,/**< ADuC information packet corrupted @see ADuCMsg_infoCatheterCorruptedHandler*/
			ADuC_ComError_sampledDataCorrupted,/**< ADuC samples data packet corrupted @see ADuCMsg_sampledDataCorruptedHandler*/
			ADuC_ComError_packetsLost01,/**< packets sent from the ADuC were lost @see ADuCMsg_packetsLostHandler*/
			ADuC_ComError_timoutRx,/**< timeout RX elapsed @see ADuCMsg_timoutRxHandler*/
			ADuC_ComError_timoutTx,/**< timeout TX elapsed @see ADuCMsg_timoutTxHandler*/
			ADuC_ComError_wrongStx,/**< wrong STX was recived from the ADuC @see ADuCMsg_wrongStxHandler*/
			ADuC_ComError_opcodeUnknown,/**< unknown opcode was receive from the ADuC @see ADuCMsg_opcodeUnknownHandler*/
			ADuC_ComError_checkSumError,/**< checksum error in the received packet @see ADuCMsg_checkSumErrorHandler*/
			ADuC_ComError_HwError,/**< uart hw error (i.e.OverrunError) @see ADuCMsg_HwErrorHandler*/
			ADuC_ComError_transmitBufferOverflowError,/**< transmission buffer overflow @see ADuCMsg_transmitBufferOverflowErrorHandler*/
			ADuC_ComError_receptionBufferOverflowError,/**< reception buffer overflow @see ADuCMsg_receptionBufferOverflowErrorHandler*/
			ADuC_ComError_dataReceivedBufferOverflow,/**< data received buffer overflow @see ADuCMsg_dataReceivedBufferOverflowHandler*/
			ADuC_ComError_unknownError,	/**< unknow error raised from the ADuC communication class @see ADuCMsg_unknownErrorHandler*/
			ADuC_ComError_packetsLost02,/**< packets sent from the ADuC were lost @see ADuCMsg_packetsLostHandler*/
			ADuC_ComError_packetNotAck,	/**< last packet sent not Ack */
			ADuC_ComError_packetsLost03,

			ADuC__e_comm_UartRecErr,
			ADuC__e_comm_OpCodeUnknown,
 	        ADuC__e_comm_WrongNumData,
 	        ADuC__e_comm_CrcError,
 	        ADuC__e_comm_EtxNotReceived,
 	        ADuC__e_comm_StxNotReceived,
 	        ADuC__e_comm_TxFifoOverflow,
 	        ADuC__e_comm_RxFifoOverflow,
 	        ADuC__e_comm_TimeoutRx,
 	        ADuC__e_comm_TimeoutTx,
 	        ADuC__e_comm_OpcodeFifoOverflow,
 	        ADuC__e_comm_ReceivedDataFifoOverflow,
 	        ADuC__e_comm_pAdcFifoOverflow,
 	        ADuC__e_comm_aAdcFifoOverflow
		};

		/**
		Communication error with the PC.

		This errors are raised from the PC communication protocol decode class.
		The attribute m_sauPcComErrors (word) contains the communication errors
		with the unisensor catheter and its value is calculated:
		m_sauPcComErrors |= (1 << "error type");
		*/
		enum SauPcComErrors
		{
			SauPcComError_noError = 0, /**< No Error.*/
			SauPcComError_packetsLost,/**< packets sent from the PC were lost. @see SauPcMsg_packetsLostHandler*/
			SauPcComError_timoutRx,/**< timeout RX elapsed @see SauPcMsg_timoutRxHandler**/
			SauPcComError_timoutTx,/**< timeout TX elapsed @see SauPcMsg_timoutRxHandler*/
			SauPcComError_wrongStx,/**< wrong STX received @see SauPcMsg_wrongStxHandler*/
			SauPcComError_opcodeUnknown,/**< unknown opcode received from the PC @see SauPcMsg_opcodeUnknownHandler*/
			SauPcComError_checkSumError,/**< checksum error in the received packet @see SauPcMsg_checkSumErrorHandler*/
			SauPcComError_HwError,/**< uart hw error (i.e.OverrunError) @see SauPcMsg_HwErrorHandler*/
			SauPcComError_transmitBufferOverflowError,/**< transmission buffer overflow @see SauPcMsg_transmitBufferOverflowErrorHandler*/
			SauPcComError_receptionBufferOverflowError,/**< reception buffer overflow @see SauPcMsg_receptionBufferOverflowErrorHandler*/
			SauPcComError_dataReceivedBufferOverflow,/**< data received buffer overflow @see SauPcMsg_dataReceivedBufferOverflowHandler*/
			SauPcComError_unknownError,		/**< unknow error raised from the unisensor communication class @see SauPcMsg_unknownErrorHandler*/
		};

		enum GenericErrors
		{
			gErr_none = 0,
			gErr_ADuC_timeOutOnSampling,
			gErr_ADuC_mismatchOnStart,
			gErr_ADuC_mismatchOnAcq,
			gErr_ADuC_mismatchOnStop,
			gErr_Emgz_timeOutOnSampling,

			gErr_Emg_BufferOverflow,
			gErr_EmgImp_BufferOverflow,
			gErr_PrsChan_BufferOverflow,
			gErr_WeightChan_BufferOverflow,
			gErr_GenChan_BufferOverflow,

			gErr_errorFromEmgzBoard,
			
		};

		/**
		Reasons acquisition stopped
		*/
		enum SauPc_WhySamplingWasStopped
		{
			AcqStopped_pcCommand = 0,						/**< acquisition stopped because received stop commad from PC */
			AcqStopped_ADuCNotAnswerToStatusCmd,//1			/**< acquisition stopped because one of the ADuCs not responding to get status command */
			AcqStopped_EmgNotAnswerToStatusCmd,
			AcqStopped_BufferOverflow,
			AcqStopped_WatchdogError,
			AcqStopped_SauIsSwitchingOff, //5
			AcqStopped_SauCommTypeChanged,
			AcqStopped_jumpingToLoader,
			AcqStopped_BTbypassMode,
			AcqStopped_ADuCMismatchOnAcqStatus,
			AcqStopped_ADuC_timeOutOnSampling, //10
			AcqStopped_EMGZ_timeOutOnSampling,
			AcqStopped_SauPc_UartHwError,
			AcqStopped_SauPc_TransmitBufferOverflow,
			AcqStopped_SauPc_ReceptionBufferOverflow,
			AcqStopped_SauPc_DataReceivedBufferOverflow, //15
			AcqStopped_BTcommunicationDropped,
			AcqStopped_SauEmgz_UartHwError,
			AcqStopped_SauEmgz_TransmitBufferOverflow,
			AcqStopped_SauEmgz_ReceptionBufferOverflow,
			AcqStopped_SauEmgz_DataReceivedBufferOverflow, //20
			AcqStopped_SauEmgz_EmgSamplesBufferOverflow,
			AcqStopped_SauEmgz_EmgImpSamplesBufferOverflow,
			AcqStopped_ADuC_UartHwError,
			AcqStopped_ADuC_TransmitBufferOverflow,	//24
			AcqStopped_ADuC_ReceptionBufferOverflow,//25
			AcqStopped_ADuC_DataReceivedBufferOverflow,
			AcqStopped_weightsChanBufferOverflow,
			AcqStopped_prsChanBufferOverflow,
			AcqStopped_genericsChanBufferOverflow,
			AcqStopped_GenericError	//30
			
		};
		
		/**
		Board's states
		*/
		enum Sau7000_Status
		{
			Sau7000status_idle = 0,/**< idle status. The board is waiting for command*/
			Sau7000status_sendingSettings,	/**< Acquisition Setting were sent to the the ADuCs.. waiting all the answers*/
			Sau7000status_waitingADuCToStart, /**< Start command sent to all the ADuCs.. waiting answer... */
			Sau7000status_sampling,/**< sampling status. The board is sampling and sending the data to the PC */
			Sau7000status_waitingADuCToStop,/**< Stop command sent to all the ADuCs.. waiting answer... */
			Sau7000status_error,/**< error status.*/
			Sau7000status_wait,/**< wait status. An error occured and time has to elapse before recovering it.*/
			Sau7000status_startupChecks, /**< startup checks*/
			Sau7000status_bluetoothCheck,
			Sau7000status_checkADuCfwUpgrade, /**< upgrade the AduC firmware*/
			Sau7000status_ADuCfrequencyCalibration,
			Sau7000status_switchingOff,
			Sau7000status_emgzBoardUpgrade,
			Sau7000status_BT_byPassMode
			
		};

		enum Sau7000_startUpChecks_states
		{
			strUpChk_logo,
			strUpChk_fpga,
			strUpChk_getCalVal,
			strUpChk_ADuC,
		};

		enum strUpChk_fpga_states
		{
			fpgaChk_readBitsream,
			fpgaChk_bitstreamOK,
			fpgaChk_bitstreamERROR,
		};
		
		enum BTchk_status
		{
			BTchk_idle = 0,
			BTchk_verify_BR,
			BTchk_verifing_BR,
			BTchk_ask_Info,
			BTchk_waiting_Info,
			BTchk_set_Profile,
			BTchk_waiting_Profile,
			BTchk_set_MSC,
			BTchk_waiting_MSC,
			BTchk_set_BT_NAME,
			BTchk_waiting_set_BT_NAME,
			BTchk_set_BT_SSP,
			BTchk_waiting_set_BT_SSP,
			BTchk_set_BT_AUTH,
			BTchk_waiting_set_BT_AUTH,
			BTchk_RING,
			BTchk_waiting_RING,
			BTchk_CALL,
			BTchk_wating_CALL,
			BTchk_CLOSE,
			BTchk_waiting_CLOSE,
			BTchk_wait,
			
			
		};

		enum COMM_states
		{
			comm_UNKNOWN,
			comm_USB,
			comm_noUSB_noBT,
			comm_BT,
			comm_chekingBTconnection,
			comm_BT_connected,
			comm_BTconnectionError,
		};
		
		/**
		AduC micontrollers' states 
		*/
		enum ADuCStatus
		{
			ADuCStatus_idle = 0,				/**< ADuC idle status*/
			ADuCStatus_acquiring,				/**< ADuC is acquiring*/
			ADuCStatus_selfTest,
			ADuCStatus_notRespondingToCommand,  /**< ADuC is not responding to any command*/
			ADuCStatus_set,						/**< acquisition parameters were set*/
			ADuCStatus_error,					/**< error.......*/
			ADuCStatus_unknown,					/**< unknown. Startup condition*/
		};
		
		enum SauErrors
		{
			SauError_noError = 0,
			SauError_ADuCsCanNotbeUpgraded,
			SauError_watchdogError,
			SauError_fpgaUpgradeFailed,
		
		};
		
		/**
		HR EMG Board's states 
		*/
		enum emgzBoardStatus
		{
			emgzBoardStatus_idle = 0,/**< HR EMG Board idle status*/
			emgzBoardStatus_acquiring,/**< HR EMG Board is acquiring*/
			emgzBoardStatus_error,				/**< error.......*/
			emgzBoardStatus_wait,
			emgzBoardStatus_unknown,				/**< unknown. Startup condition*/
			emgzBoardStatus_notRespondingToCommand, /**< HR EMG Board is not responding to any command*/
		};

		/**
		Indicates if air's bubbles are prensent within the line
		*/
		enum BubbleAirStatus
		{
			BubbleAirState_unknown = 0,	/**< unknow*/
			BubbleAirState_NoAir,		/**< NO Air within the line*/
			BubbleAirState_AirPresent,	/**< Air with the line*/
		};
		
		/**
		Indicates the bubble's clamp status
		*/
		enum BubbleClampStatus
		{
			BubbleClampState_unknown = 0,	/**< unknow*/
			BubbleClampState_open,			/**< clam is open*/
			BubbleClampState_close,			/**< clamp is close*/
		};
		
		/**
		WTH Status
		*/
		enum WthStatus
		{
			WthState_unknown = 0,	/**< WTH unknow status*/
			WthState_stop,			/**< WTH is not running*/
			WthState_running,		/**< WTH is running - extracting*/
			WthState_error,			/**< error */
		};

		/**
		WTH End Switches Status
		*/
		enum WthEndSwitchesStatus
		{
			WthEndSwitchesStatus_unknown = 0,	/**< unknown*/
			WthEndSwitchesStatus_noOnesReached, /**< not swithched reached*/
			WthEndSwitchesStatus_tipReached,    /**< tip switch reached - puller back to home position*/
			WthEndSwitchesStatus_bottomReached, /**< button switch reached - end of extraction*/ 
			WthEndSwitchesStatus_errorBottom, 	/**< uncoerence in the switch - probably a switch is broken*/
			WthEndSwitchesStatus_errorTip,
			WthEndSwitchesStatus_error, 		/**< uncoerence in the switch - probably a switch is broken*/
		};
		
		/**
		WTP Status
		*/
		enum WtpStatus
		{
			WtpState_unknown = 0,	/**< unknown*/
			WtpState_stop,			/**< WTP is not running*/
			WtpState_running,		/**< WTP is running*/
			WtpState_error,			/**< error*/
			
		};

		/**
		WTP Lit Status
		*/
		enum WtpLitStatus
		{
			WtpLitStatus_unknown = 0, 	/**< unknown*/
			WtpLitStatus_open,		  	/**< WTP's lit is open*/	
			WtpLitStatus_close,			/**< WTP's lit is closed*/	
		};

		SAU7000_Manager(Keyboard *keyboard, SauPcProtocol *sauPcProtocol);
		~SAU7000_Manager();

		void manager();

		
		void setAdcNec(Adc_NEC_SAU7000* _adcNec){ m_adcNec = _adcNec;};

		/**
		Sets the class that manages the weight samples.
		@param w poiter to the class GenericHwChanManager
		@see setWeightChan
		*/
		void setWeightChan(GenericHwChanManager <long, _WEIGHT_BUFFER_LENGTH_> *w){ m_weightChan = w;};
		
		/**
		Sets the class that manage the pressure samples.
		@param prs poiter to the class PrsHwChanManager
		@see PrsHwChanManager
		*/
		void setPrsChan(PrsHwChanManager <short, _PRS_BUFFER_LENGTH_> *prs){ m_prsChan = prs;};
		

		void setGenChanGainAndOffset(gainOffset_t *__pGen1ChanGainOffset, gainOffset_t *__pGen2ChanGainOffset){m_pGenChanGainOffset[0] = __pGen1ChanGainOffset;m_pGenChanGainOffset[1] = __pGen2ChanGainOffset;}
		
		/**
		Sets istance of the class that manages the communication with the VOL ADuC .
		@param e poiter to the class ADuC_WEIGHT_protocol
		@see ADuC_WEIGHT_protocol
		@see m_ADuC_Protocols
		*/
		void setVinfProtocol(ADuC_WEIGHT_protocol *vol){m_ADuC_Protocols[ADuC_VINF] = (uC_Protocol*)vol;};

		/**
		Sets istance of the class that manages the communication with the FLW ADuC .
		@param e poiter to the class ADuC_WEIGHT_protocol
		@see ADuC_WEIGHT_protocol
		@see m_ADuC_Protocols
		*/
		void setVVProtocol(ADuC_WEIGHT_protocol *flw){m_ADuC_Protocols[ADuC_VV] = (uC_Protocol*)flw;};

		/**
		Sets istance of the class that manages the communication with the P1 and P2 ADuC .
		@param e poiter to the class ADuC_PRS_protocol
		@see ADuC_PRS_protocol
		@see m_ADuC_Protocols
		*/
		void setP12Protocol(ADuC_PRS_protocol *p){m_ADuC_Protocols[ADuC_P12] = (uC_Protocol*)p;};

		/**
		Sets istance of the class that manages the communication with the P3 and P4 ADuC .
		@param e poiter to the class ADuC_PRS_protocol
		@see ADuC_PRS_protocol
		@see m_ADuC_Protocols
		*/
		void setP34Protocol(ADuC_PRS_protocol *p){m_ADuC_Protocols[ADuC_P34] = (uC_Protocol*)p;};

		/**
		Sets the class istance of the class that manages the communication with the P5 and P6 ADuC .
		@param e poiter to the class ADuC_PRS_protocol
		@see ADuC_PRS_protocol
		@see m_ADuC_Protocols
		*/
		void setP56Protocol(ADuC_PRS_protocol *p){m_ADuC_Protocols[ADuC_P56] = (uC_Protocol*)p;};

		/**
		Sets istance of the class that manages the communication with the P7 and P8 ADuC .
		@param e poiter to the class ADuC_PRS_protocol
		@see ADuC_PRS_protocol
		@see m_ADuC_Protocols
		*/
		void setP78Protocol(ADuC_PRS_protocol *p){m_ADuC_Protocols[ADuC_P78] = (uC_Protocol*)p;};

		/**
		Sets istance of the class that manages the communication with the GEN ADuC .
		@param e poiter to the class ADuC_GEN_protocol
		@see ADuC_GEN_protocol
		@see m_ADuC_Protocols
		*/
		void setGenProtocol(ADuC_GEN_protocol *g){m_ADuC_Protocols[ADuC_GEN] = (uC_Protocol*)g;};

		/**
		Sets istance of the class that manages the communication with the EMG board .
		@param e poiter to the class ADuC_EMG_protocol
		@see ADuC_EMG_protocol
		@see m_emgProtocol
		*/
		void setEmgProtocol(SauAndEmgzProtocol *e){m_emgProtocol= e;};

		void setADuCUpgradeManger(ADuCUpgradeManger* upgrade){m_ADuCUpgradeManger = upgrade;};

		void setBluetoothProtocol(iWRAP_BLUEGIGA_SPP *bluetoothProtocol){m_bluetoothProtocol = bluetoothProtocol;}
		
		void setWth(WthMenfis *__wth){m_wth = __wth;}

		void setWtp(WtpMenfis *__wtp){m_wtp = __wtp;}
		
		void setIR_remote(IR_Remote *__remote){m_IR_Remote = __remote;}

		inline bool bubbleDetectionEn(){ return m_bubbleDetectionEn;}
		inline void airDetected(){m_bubbleAirDetected = true;}
		inline bool bubbleAirTestRunning(){return m_bubbleAirTestRunning;}
		
		void stopWth();
		void stopWtp();
		
	protected:  

		enum defualts
		{
			__WATCHDOG_TIME__ = 3000,
			__LED_BLINK_VALUE_IDLE__ = 40,
			__LED_BLINK_VALUE_ACQ__ = 20,
		};

		enum ADuC_frequencyCalibrationStates
		{
			ADuCfrqCal_init_forceStop = 0,
			ADuCfrqCal_init_set,	
			ADuCfrqCal_waitingADuCToBeSet,
			ADuCfrqCal_waitingADuCToStartAcq,
			ADuCfrqCal_calibrating,
			ADuCfrqCal_waitingADuCToStop,
			ADuCfrqCal_calculatingNewFrq,
			ADuCfrqCal_error
		};
		
		inline int SauPcMsg_infoAskedHandler();
		inline int SauPcMsg_statusAskedHandler();
		inline int SauPcMsg_startAcquisitionHandler();
        inline int SauPcMsg_stopAcquisitionHandler(byte cause);
		
		inline int SauPcMsg_wtpSpeedHandler();
        inline int SauPcMsg_wthSpeedHandler();
		inline int SauPcMsg_wtpStatusHandler();
		inline int SauPcMsg_wthStatusHandler();
		inline int SauPcMsg_setWtpSpeedCalibrationHandler();
		
		inline int SauPcMsg_bubbleStatusHandler();
		
		inline int SauPcMsg_jumpToLoaderHandler();
		inline int SauPcMsg_emgz_jumpToLoaderHandler();
		
		inline int SauPcMsg_WeightCalAskedHandler();
		inline int SauPcMsg_PrsCalAskedHandler();
		inline int SauPcMsg_GenericCalAskedHandler();
		inline int SauPcMsg_EmgCalAskedHandler();
		
        inline int SauPcMsg_WeightCalSetHandler();
        inline int SauPcMsg_Prs_CalSetHandler();
        inline int SauPcMsg_EmgCalSetHandler();
        inline int SauPcMsg_Generic_CalSetHandler();
        inline int SauPcMsg_zeroChanHandler();

		inline int SauPcMsg_setDisplayHandler();
		inline int SauPcMsg_getDisplayHandler();

		inline int SauPcMsg_emgzContactImpHandler();
		inline int SauPcMsg_emgzSetFilteringHandler();

		inline int SauPcMsg_BT_bypassModeHandler();
		
		inline int SauPcMsg_packetsLostHandler();
		inline int SauPcMsg_timoutRxHandler();
		inline int SauPcMsg_timoutTxHandler();
		inline int SauPcMsg_wrongStxHandler();
		inline int SauPcMsg_opcodeUnknownHandler();
		inline int SauPcMsg_checkSumErrorHandler();
		inline int SauPcMsg_HwErrorHandler();
		inline int SauPcMsg_transmitBufferOverflowErrorHandler();
		inline int SauPcMsg_receptionBufferOverflowErrorHandler();
		inline int SauPcMsg_dataReceivedBufferOverflowHandler();
		inline int SauPcMsg_unknownErrorHandler();
		inline int SauPcMsg_BTcommunicationDroppedHandler();
		inline int SauPcMsg_watchdogSettingHandler();
		inline int SauPcMsg_setMotorCalHandler();
		inline int SauPcMsg_getMotorCalHandler();
		inline int SauPcMsg_setOverpressureHandler();
		
		inline int emgBoard_infoReceivedHandler();
		inline int emgBoard_statusReceivedHandler();
		inline int emgBoard_acquisitionStartedHandler();
		inline int emgBoard_acquisitionStoppedHandler();
		inline int emgBoard_emgSamplesReceivedHandler();
		inline int emgBoard_contactImpSamplesReceivedHandler();
		inline int emgBoard_acknowledgeHandler();
		inline int emgBoard_notAcknowledgeHandler();
		inline int emgBoard_ErrorPacketReceivedHandler();
		inline int emgBoard_emgCalReceivedHandler();
		inline int emgBoard_impCalReceivedHandler();
		inline int emgBoard_packetsLostImp_Handler();
		inline int emgBoard_packetsLostEmg_Handler();
		inline int emgBoard_jumpedToLoaderHandler();
		inline int emgBoard_timoutRxHandler();
		inline int emgBoard_timoutTxHandler();
		inline int emgBoard_wrongStxHandler();
		inline int emgBoard_opcodeUnknownHandler();
		inline int emgBoard_checkSumErrorHandler();
		inline int emgBoard_HwErrorHandler();
		inline int emgBoard_transmitBufferOverflowErrorHandler();
		inline int emgBoard_receptionBufferOverflowErrorHandler();
		inline int emgBoard_dataReceivedBufferOverflowHandler();
		inline int emgBoard_EmgSamplesBufferOverflowHandler();
		inline int emgBoard_ImpEmgSamplesBufferOverflowHandler();
		inline int emgBoard_unknownErrorHandler();

		inline int ADuC_infoReceivedHandler(int i);
		inline int ADuC_statusReceivedHandler(int i);
        inline int ADuC_acquisitionStartedHandler(int i);
        inline int ADuC_acquisitionStoppedHandler(int i);
		inline int ADuC_packet01_receivedHandler(int i);
		inline int ADuC_packet02_receivedHandler(int i);
		inline int ADuC_packet03_receivedHandler(int i);
		inline int ADuC_received_SETTING_ADCHandler(int i);
		inline int ADuC_msg_received_SETTING_IEXCHandler(int i);
		inline int ADuC_received_SETTING_DACHandler(int i);
		inline int ADuC_received_SETTING_PWMHandler(int i);
		inline int ADuC_ALL_Setting_ACKHandler(int i);
		inline int ADuC_acquisitionNOTstartedHandler(int i);
		inline int ADuC_ErrorPacketReceivedHandler(int i);
		inline int ADuC_messageNotAckPacketReceivedHandler(int i);
		inline int ADuC_messageAckPacketReceivedHandler(int i);
		inline int ADuC_packetsLost_01Handler(int i);
		inline int ADuC_packetsLost_02Handler(int i);
		inline int ADuC_packetsLost_03Handler(int i);
		inline int ADuC_timoutRxHandler(int i);
		inline int ADuC_timoutTxHandler(int i);
		inline int ADuC_wrongStxHandler(int i);
		inline int ADuC_opcodeUnknownHandler(int i);
		inline int ADuC_checkSumErrorHandler(int i);
		inline int ADuC_HwErrorHandler(int i);
		inline int ADuC_transmitBufferOverflowErrorHandler(int i);
		inline int ADuC_receptionBufferOverflowErrorHandler(int i);
		inline int ADuC_dataReceivedBufferOverflowHandler(int i);
		inline int ADuC_weightChanBufferOverflowHandler(int i);
		inline int ADuC_prsChanBufferOverflowHandler(int i);
		inline int ADuC_genChanBufferOverflowHandler(int i);
		inline int ADuC_unknownErrorHandler(int i);

		inline int iWRAP_msg_CONNECTED_Handler(char *par, int len);
		inline int iWRAP_msg_DISCONNECTED_Handler(char *par, int len);

		inline int iWRAP_msg_INFO_Handler(char *par, int len);
		inline int iWRAP_msg_SET_BT_PROFILE_Handler(char *par, int len);
		inline int iWRAP_msg_SET_BT_NAME_Handler(char *par, int len);
		inline int iWRAP_msg_SET_BT_AUTH_Handler(char *par, int len);
		inline int iWRAP_msg_SET_BT_SSP_Handler(char *par, int len);
		inline int iWRAP_msg_SET_BAUD_Handler(char *par, int len);
		inline int iWRAP_msg_SET_CONTROL_MSC_Handler(char *par, int len);
		inline int iWRAP_msg_RING_Handler(char *par, int len);
		inline int iWRAP_msg_CALL_Handler(char *par, int len);

		inline int iWRAP_msg_moduleNotResponding_Handler(char *par, int len);
		inline int iWRAP_msg_syntaxErrorSyncroError_Handler(char *par, int len);
		inline int iWRAP_msg_timoutRx_Handler(char *par, int len);
		inline int iWRAP_msg_timoutTx_Handler(char *par, int len);
		inline int iWRAP_msg_HwError_Handler(char *par, int len);
		inline int iWRAP_msg_transmitBufferOverflowError_Handler(char *par, int len);
		inline int iWRAP_msg_receptionBufferOverflowError_Handler(char *par, int len);
		inline int iWRAP_msg_dataReceivedBufferOverflow_Handler(char *par, int len);
		inline int iWRAP_msg_unknownError_Handler(char *par, int len);

		
		int sendErrorsMsg();

		byte changeSau7000status(Sau7000_Status newStatus);
		inline Sau7000_Status getSau7000status();
		inline Sau7000_Status getSau7000previousStatus();

		byte changeBTcheckStatus(BTchk_status status);
		
		inline void checkSauPcProtocolMessages();
		inline void checkSauAduCsMessages();
		inline void checkSauEmgBoardMessages();
		inline void checkBluetoothProtocolMessages();

		inline void checkCOMM_status();
		
		inline void checkChannelPluggedToGenericChan();
		
		bool checkADuCStatus(byte status);
		void startADuCs();
		void setADuCsAquisitionParameters();
		void stopADuCs();
		void getADuCsStatus();
		void getADuCsInfo();

		void rstPrsSamples();
		void rstWeightSamples();

		inline void Sau7000status_idle_Handler();
		inline void Sau7000status_sendingSettings_Handler();
		inline void Sau7000status_waitingADuCToStart_Handler();
		inline void Sau7000status_sampling_Handler();
		inline void Sau7000status_waitingADuCToStop_Handler();
		inline void Sau7000status_error_Handler();
		inline void Sau7000status_wait_Handler();
		inline void Sau7000status_startupChecks_Handler();
		inline void Sau7000status_bluetoothCheck_Handler();
		inline void Sau7000status_ADuCfrequencyCalibration_Handler();
		inline void Sau7000status_checkADuCfwUpgrade_Handler();
		inline void Sau7000status_emgzBoardUpgrade_Handler();
		inline void Sau7000status_BT_byPassMode_Handler();

		void ADuC_startupChecks_Init();
		void ADuCsNormalMode();

		inline void WthManager();
		inline void WtpManager();
		void KeyboardManager();
		void IR_remoteManager();
		void watchDogManager();

		void stop_sampling_and_all_peripherals();

		inline void FlashLed();

		int writeToEE_prsCalValue();
		int readToEE_prsCalValue();
		int writeToEE_weightCalValue();
		int readToEE_weightCalValue();
		int writeToEE_genCalValue(int __chan);
		int readToEE_genCalValue(int __chan);
		
		void setOverpressureAdcLimit();
		
	private:
		/**
		Pointer to the class that manages the board's keyboard.
		*/
		Keyboard *m_keyboard;
		
		/**
		Pointer to the class that manages the communication with the PC.
		*/
		SauPcProtocol *m_sauPcProtocol;

		Adc_NEC_SAU7000* m_adcNec;
		
		/**
		Pointer to the instance of class that manages the weight samples.
		*/
		GenericHwChanManager <long, _WEIGHT_BUFFER_LENGTH_> *m_weightChan;
		/**
		Pointer to the instance of class tha manages the pressure samples.
		*/
		PrsHwChanManager <short, _PRS_BUFFER_LENGTH_> *m_prsChan;

		gainOffset_t *m_pGenChanGainOffset[_NUM_OF_GENERIC_CHAN_];
		
		/**
		Array of pointers to the istances of the classes that manage the communication with the ADuC7061 microcontrollers
		*/
		uC_Protocol *m_ADuC_Protocols[ADuC_NUM];
		
		/**
		Pointer to the instance of class that manages the communication with the Hr EMG board
		*/
		SauAndEmgzProtocol *m_emgProtocol;

		iWRAP_BLUEGIGA_SPP *m_bluetoothProtocol;
		/**
		Contains the ommunication errors with ADuC
		@see ADuC_CommunicationErrors
		*/
		dword m_ADuC_ComErrors[ADuC_NUM];
		
		/**
		Contains communication errors with the PC
		@see SauPcComErrors
		*/
		word m_sauPcComErrors;

		/**
		Contains  communication errors with EMG board
		@see SauEmgBoardComErrors
		*/
		word m_sauEmgBoardErrors;

		word m_genericErrors;
		
		/**
		Lean variable to read the power voltage sample from the class VoltageHwChanManager
		*/
		word m_lastVoltageValueSampled[_NUM_VOLTAGE_MEASURE_];
		/**
		Lean variable to read the power current consumption sample from the class CurrentHwChanManager
		*/
		word m_lastCurrentValueSampled;
		/**
		Lean array to read the Weight samples from the class ImpHwChanManager
		*/
		long m_weightSamples[_NUM_WEIGHT_SAMPLES_PER_PACKET_];
		/**
		Lean array to read the pressure samples from the class PrsHwChanManager
		*/
		short m_prsSamples[_NUM_PRS_SAMPLES_PER_PACKET_];
	
		/**
		@see Sau7000_Status
		*/
		Sau7000_Status m_sau7000status;
		Sau7000_Status m_sau7000previousStatus; 

		Sau7000_startUpChecks_states m_sauStrChk_state;
		
		BTchk_status m_BTcheckStatus;
		BTchk_status m_BTcheckPreviousStatus;

		char m_remoteBTaddr[20];
		
		DecTimer m_BTwaitStatusTimer;
		DecTimer m_BTidleStatusTimer;
		/**
		ADuC microcontroller status

		@see ADuCStatus
		@see ADuC7061_use
		*/
		ADuCStatus m_ADuCStatus[ADuC_NUM];
		short m_ADuC_numErrors[ADuC_NUM];
		/**
		Timer to verify the ADuCs are sending the data.
		*/
		DecTimer m_ADuCacqCheckTimer[ADuC_NUM];
		
		word m_numOfErrorsOccured;

		/**
		Hr Emg board status
		@see emgzBoardStatus
		*/
		emgzBoardStatus m_emgzBoardStatus;
		word m_emgzBoardVoltageMeasure[SauAndEmgzProtocol::_NUM_VOLTAGE_MEASURE_EMGZ_];
		word m_emgzBoardCurrentMeasure;
		byte m_emgzBoardOverCurrentError;
		word m_emgzBoardNumErrorOccurred;
		byte m_emgzBoardNumTimeoutTxOccurred;
		byte m_emgzBoardLPF_type;
		byte m_emgzBoardNOTCHF_type;
		byte m_emgzBoardHPF_type;
		/**
		Bubble Air status
		@see BubbleAirStatus
		*/
		BubbleAirStatus m_bubbleAirStatus;

		/**
		Bubble Clamp status
		@see BubbleClampStatus
		*/
		BubbleClampStatus m_bubbleClampStatus;

		/**
		Enable auto detection
		*/
		bool m_bubbleDetectionEn;
		/**
		if true the air test failed
		*/
		bool m_bubbleAirTestFailed;
		/**
		if true the air was detected
		*/	
		bool m_bubbleAirDetected;
		/**
		if true when the ait test runs
		*/	
		bool m_bubbleAirTestRunning;

		IncTimer m_bubbleAirTestTimer;
		/**
		WTH status
		@see WthStatus
		*/
		WthStatus m_wthStatus;

		/**
		WTH step
		@see WthStatus
		*/
		word m_wthStep;
		
		/**
		@see WthEndSwitchesStatus
		*/
		WthEndSwitchesStatus m_wthEndSwitchesStatus;

		/**
		WTP status
		@see WtpStatus
		*/
		WtpStatus m_wtpStatus;

		/**
		@see WtpLitStatus
		*/
		WtpLitStatus m_wtpLitStatus;
		
		/**
		Timer to check the status of the ADuCs while the acquisition is NOT running
		*/
		DecTimer m_timerCheckADuCStatus;

		DecTimer m_logoTimer;
		/**
		Timer to check the status of the Emg board while the acquisition is NOT running
		*/
		DecTimer m_timerCheckEmgBoardStatus;
		
		/**
		Timer to check the status of the ADuCs and Emg board while the acquisition is running
		*/
		DecTimer m_timerCheckADuCStatusWhileSampling;
		
		/**
		Wait timer after error is occured 
		*/
		DecTimer m_timerWaitStatus;

		/**
		Maximum time to wait becuase all the AduC will be in the same status
		*/
		DecTimer m_timerWaitAduCResponse;

		/**
		Next status of the ADuCs
		*/
		byte m_ADuCsWantedStatus;

		/**
		If true the high resolution emd board is conneted to the sau
		*/
		bool m_emgzBoardPresent;

		ADuCUpgradeManger* m_ADuCUpgradeManger;
		
		word m_AduC_firmwareVersion[ADuC_NUM];

		byte m_wthPowered;
		short m_wthSpeed;
		word m_wtpSpeed;

		byte m_numADuCToCheck;
		byte m_numADuCToUpgrade;
		byte m_numADuC_ERR;
		byte m_numADuC_OK;
		byte m_numReset;
		bool m_ADuCToUpgrade;
		DecTimer m_ADuCTimerStartupCheck;
		
		byte m_ADuCUpgradeErrors;

		SauErrors m_sauError;

		WthMenfis *m_wth;
		WtpMenfis *m_wtp;
		IR_Remote *m_IR_Remote;

		DecTimer m_watchDogTimer;
		bool m_watchdogEnabled;
		int m_watchdogTime;

		DecTimer m_timerCheckChanSyncro;
		
		COMM_states m_COMM_state;

		byte m_connectedToGEN_CHAN_1;
		byte m_connectedToGEN_CHAN_2;

		UartFPGA *m_pUART_PROTOCOL_PC;

		word m_fpgaBitstream;
		strUpChk_fpga_states m_fpgaChkState;

		int m_blinkLed;
		int m_blinkLedValue;
		bool m_getStatusReceived;

		ADuC_frequencyCalibrationStates m_ADuCFrqCalState;
		bool m_calibrateADuCFrq;
		
		DecTimer m_ADuCFrqCalTimer;

		byte m_whyAcqWasStopped;
		bool m_firstBtCheck;

		overpressure_bitF_t m_overpressureChanEn; // canali con controllo di sovrapressione abilitato
		overpressure_bitF_t m_overpressureChan;	  // canali in sovvrapressione
		overpressure_bitF_t m_overpressureChan_previous;
		short m_overpressureLimit; // in mmHg
		short m_overpressureLimitAdc[_NUM_OF_PRS_CHAN_];

		bool m_retryRestarComm;
		short m_numRetriesRestartComm;
		DecTimer m_timerRetryRestartComm;
		DecTimer m_timerResetRetries;
		
		//DecTimer m_test;
};





#endif








































