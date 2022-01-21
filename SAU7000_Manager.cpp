/**
@file		SAU7000_Manager.cpp
@brief		SAU7000 states machine
@author		Nicola Molinazzi
@date		18/01/2011
@version	01.00
*/


#include "SAU7000_Manager.h"

#include "SSPIE_util.h"

/**
Used to jump at the loader start address
*/
void (*fp)(void);


extern const word fpgaWantedBitstream; 
extern UartFPGA *pUART_PC;
extern UartFPGA *pUART_BLUE;
extern UartFPGA *pUART_EMG;
extern UartFPGA *pUART_ADuC[ADuC_NUM];
extern DisplaySetting displaySetting;


/**
Class constructor.

@param keyboard class tha magage the board's keyboad
@param sauPcProtocol class tha manage the communication protocol with PC
*/
SAU7000_Manager::SAU7000_Manager(Keyboard *keyboard, SauPcProtocol *sauPcProtocol)
{
	int i;
	m_keyboard = keyboard;
	m_sauPcProtocol = sauPcProtocol;

	m_sauPcComErrors = 0;
	m_genericErrors = 0;
	
	for (i = 0; i < _NUM_VOLTAGE_MEASURE_; i++)
	{
		m_lastVoltageValueSampled[i] = 0;
	}
	m_lastCurrentValueSampled = 0;
	m_numOfErrorsOccured = 0;
	m_sau7000status = Sau7000status_startupChecks;
	m_sau7000previousStatus = Sau7000status_startupChecks;
	m_sauStrChk_state = strUpChk_logo;
	
	changeSau7000status(Sau7000status_startupChecks);

	for (i = 0; i < ADuC_NUM; i++)
	{
		m_ADuC_ComErrors[i] = 0;
		m_ADuCStatus[i] = ADuCStatus_unknown;
		m_ADuC_numErrors[i] = 0;
		m_AduC_firmwareVersion[i] = 0xFFFF;
		m_ADuCacqCheckTimer[i].Stop();
	}

	m_emgzBoardStatus = emgzBoardStatus_unknown;
	for(i = 0; i < SauAndEmgzProtocol::_NUM_VOLTAGE_MEASURE_EMGZ_; i++)
	{
		m_emgzBoardVoltageMeasure[i] = 0;
	}

	m_emgzBoardCurrentMeasure = 0;
	m_emgzBoardOverCurrentError = 0;
	m_emgzBoardNumErrorOccurred = 0;
	m_emgzBoardNumTimeoutTxOccurred = 0;
	
	m_timerCheckADuCStatus.Preset(_TIME_CHECK_ADuC_STATUS_);
	m_timerCheckADuCStatusWhileSampling.Stop();
	m_timerCheckEmgBoardStatus.Preset(_TIME_CHECK_EMGZ_STATUS_);
	m_timerWaitStatus.Stop();

	m_bubbleAirStatus = BubbleAirState_unknown;
	m_bubbleDetectionEn = false;
	m_bubbleAirTestFailed = false;
	m_bubbleAirDetected = false;
	m_bubbleAirTestRunning = false;
	m_bubbleClampStatus = BubbleClampState_close;	
	m_wthStatus	= WthState_stop;
	m_wthEndSwitchesStatus = WthEndSwitchesStatus_unknown;	
	m_wtpStatus = WtpState_stop;	
	m_wtpLitStatus = WtpLitStatus_unknown;

	m_timerWaitAduCResponse.Preset(5000);
	m_ADuCsWantedStatus = ADuCStatus_idle;

	m_emgzBoardPresent = false;
	m_wthSpeed = 0;
	m_wtpSpeed = 0;
	m_wthPowered = 0;
	
	m_numADuCToCheck = 0;
	m_numADuCToUpgrade = 0;
	m_ADuCToUpgrade = false;
	m_ADuCTimerStartupCheck.Stop();
	m_ADuCUpgradeErrors = 0;
	m_numADuC_ERR = 0;
	m_numADuC_OK = 0;
	m_numReset = 0;
						
	m_sauError = SauError_noError;

	
	rstPrsSamples();
	rstWeightSamples();

	m_IR_Remote = NULL;
	m_wth = NULL;
	m_wtp = NULL;

	m_remoteBTaddr[0] = '\0';
	m_COMM_state = comm_UNKNOWN;

	m_logoTimer.Preset(3500);

	m_getStatusReceived = false;
	m_watchDogTimer.Stop();
	m_watchdogEnabled = true;
	m_watchdogTime = __WATCHDOG_TIME__;
	
	m_connectedToGEN_CHAN_1 = 0;
	m_connectedToGEN_CHAN_2 = 0;

	m_pUART_PROTOCOL_PC = NULL;
	
	m_fpgaBitstream = 0xFFFF;
	m_fpgaChkState = fpgaChk_readBitsream;

	m_emgzBoardLPF_type = 0;
	m_emgzBoardNOTCHF_type = 0;
	m_emgzBoardHPF_type = 0;

	m_blinkLed = 0;
	m_blinkLedValue = __LED_BLINK_VALUE_IDLE__; 
	m_timerCheckChanSyncro.Stop();
	m_ADuCFrqCalState = ADuCfrqCal_init_forceStop;
	m_calibrateADuCFrq = true;
	m_ADuCFrqCalTimer.Stop();

	m_whyAcqWasStopped = AcqStopped_pcCommand;
	m_BTcheckStatus = BTchk_idle;
	m_firstBtCheck = true;

	m_overpressureChanEn.prs_01 = 1;
	m_overpressureChanEn.prs_02 = 1;
	m_overpressureChanEn.prs_03 = 1;
	m_overpressureChanEn.prs_04 = 1;
	m_overpressureChanEn.prs_05 = 1;
	m_overpressureChanEn.prs_06 = 1;
	m_overpressureChanEn.prs_07 = 1;
	m_overpressureChanEn.prs_08 = 1;
	m_overpressureChanEn.gen_01 = 0;
	m_overpressureChanEn.gen_02 = 0;
	m_overpressureChanEn.unused_01 = 0;
	m_overpressureChanEn.unused_02 = 0;
	m_overpressureChanEn.unused_03 = 0;
	m_overpressureChanEn.unused_04 = 0;
	m_overpressureChanEn.unused_05 = 0;
	m_overpressureChanEn.unused_06 = 0;
	
	m_overpressureChan.prs_01 = 0;
	m_overpressureChan.prs_02 = 0;
	m_overpressureChan.prs_03 = 0;
	m_overpressureChan.prs_04 = 0;
	m_overpressureChan.prs_05 = 0;
	m_overpressureChan.prs_06 = 0;
	m_overpressureChan.prs_07 = 0;
	m_overpressureChan.prs_08 = 0;
	m_overpressureChan.gen_01 = 0;
	m_overpressureChan.gen_02 = 0;
	m_overpressureChan.unused_01 = 0;
	m_overpressureChan.unused_02 = 0;
	m_overpressureChan.unused_03 = 0;
	m_overpressureChan.unused_04 = 0;
	m_overpressureChan.unused_05 = 0;
	m_overpressureChan.unused_06 = 0;

	m_overpressureChan_previous = m_overpressureChan;
	
	m_overpressureLimit = 100; // in mmHg
	m_overpressureLimitAdc[_PRS_01_] = 0;
	m_overpressureLimitAdc[_PRS_02_] = 0;
	m_overpressureLimitAdc[_PRS_03_] = 0;
	m_overpressureLimitAdc[_PRS_04_] = 0;
	m_overpressureLimitAdc[_PRS_05_] = 0;
	m_overpressureLimitAdc[_PRS_06_] = 0;
	m_overpressureLimitAdc[_PRS_07_] = 0;
	m_overpressureLimitAdc[_PRS_08_] = 0;

	m_retryRestarComm = false;
	m_numRetriesRestartComm = 3;
	m_timerRetryRestartComm.Stop();
	m_timerResetRetries.Stop();
	//m_test.Preset(60000);
}

/**
Class destructor.
*/
SAU7000_Manager::~SAU7000_Manager()
{

}

/**
Manages the machine state.

It has to be called within the main cycle loop. 
*/
void SAU7000_Manager::manager()
{
	bool errorADuC;
	int i;

	FlashLed();

	/*if(m_test.Match() && getSau7000status() != Sau7000status_ADuCfrequencyCalibration && getSau7000status() != Sau7000status_startupChecks)
	{
		m_test.Preset(2000);
		stopADuCs();
		goBuzzer(KEY_LIKE);
	}*/
	//-- verify if the usb is plugged ---
	checkCOMM_status();

	//-- verify which channel are plugged to the generic chan
	checkChannelPluggedToGenericChan();
	
	//-------  Calls to all the managers ---------
	m_sauPcProtocol->manager();

	if ((getSau7000status() != Sau7000status_checkADuCfwUpgrade) && (m_sauError != SauError_ADuCsCanNotbeUpgraded) &&
		!(getSau7000status() == Sau7000status_wait && getSau7000previousStatus() == Sau7000status_checkADuCfwUpgrade))
	{
		for (i = 0; i < ADuC_NUM; i++)
		{
			m_ADuC_Protocols[i]->manager();
		}
	}
	
	m_emgProtocol->manager();
	
	m_adcNec->manager();
	m_adcNec->getCurrentAdcData(&m_lastCurrentValueSampled);
	m_adcNec->getVoltageAdcData(m_lastVoltageValueSampled);

	watchDogManager();
	
	WthManager();

	WtpManager();
	
	KeyboardManager();

	IR_remoteManager();
	
	//--- checking for SauPcProtocol messages ----
	checkSauPcProtocolMessages();

	//--- checking for ADuCs messages ----
	checkSauAduCsMessages();
	
	//--- checking for EMG board messages ----
	checkSauEmgBoardMessages();

	//--- checking for Bluetooth messages ----
	m_bluetoothProtocol->manager();
	checkBluetoothProtocolMessages();
	
	//--- checking for errors to send -----
	errorADuC = false;
	i = 0;
	while((i < ADuC_NUM) && (!errorADuC))
	{
		if (m_ADuC_ComErrors[i] != 0)
		{
			errorADuC = true;
			break;
		}
		i++;
	}
	
	if( errorADuC || (m_sauPcComErrors != 0) || (m_sauEmgBoardErrors != 0) || (m_genericErrors != 0))
	{
		m_numOfErrorsOccured++;
		sendErrorsMsg();
	}
	
	//--- checking for new weight samples ---
	while(m_weightChan->popAdcData_isr(m_weightSamples))
	{
		if(getSau7000status() == Sau7000status_sampling)
		{
			if (m_sauPcProtocol->sendSauPcWeightSamples(m_weightSamples, _NUM_WEIGHT_SAMPLES_PER_PACKET_) == 0)
			{
				m_whyAcqWasStopped = AcqStopped_BufferOverflow;
				stop_sampling_and_all_peripherals();
			}
			m_ADuCacqCheckTimer[ADuC_VV].Preset(500);
			m_ADuCacqCheckTimer[ADuC_VINF].Preset(500);
			
		}else
		{
			m_weightChan->clearFifo_isr();
		}
	}

	//--- checking for new prs samples ---
	while(m_prsChan->popAdcData_isr(m_prsSamples, 2))
	{
		if(getSau7000status() == Sau7000status_sampling)
		{
			if( m_sauPcProtocol->sendSauPcPrsSamples(m_prsSamples, _NUM_PRS_SAMPLES_PER_PACKET_) == 0)
			{
				m_whyAcqWasStopped = AcqStopped_BufferOverflow;
				stop_sampling_and_all_peripherals();
			}
			m_ADuCacqCheckTimer[ADuC_P12].Preset(500);
			m_ADuCacqCheckTimer[ADuC_P34].Preset(500);
			m_ADuCacqCheckTimer[ADuC_P56].Preset(500);
			m_ADuCacqCheckTimer[ADuC_P78].Preset(500);

			//check over pressure
			if(m_overpressureChanEn.prs_01)
			{
				if(m_prsSamples[0] > m_overpressureLimitAdc[_PRS_01_] || m_prsSamples[1] > m_overpressureLimitAdc[_PRS_01_])
					m_overpressureChan.prs_01 = 1;
				else
					m_overpressureChan.prs_01 = 0;
			}

			if(m_overpressureChanEn.prs_02)
			{
				if(m_prsSamples[2] > m_overpressureLimitAdc[_PRS_02_] || m_prsSamples[3] > m_overpressureLimitAdc[_PRS_02_])
					m_overpressureChan.prs_02 = 1;
				else
					m_overpressureChan.prs_02 = 0;
			}

			if(m_overpressureChanEn.prs_03)
			{
				if(m_prsSamples[4] > m_overpressureLimitAdc[_PRS_03_] || m_prsSamples[5] > m_overpressureLimitAdc[_PRS_03_])
					m_overpressureChan.prs_03 = 1;
				else
					m_overpressureChan.prs_03 = 0;
			}

			if(m_overpressureChanEn.prs_04)
			{
				if(m_prsSamples[6] > m_overpressureLimitAdc[_PRS_04_] || m_prsSamples[7] > m_overpressureLimitAdc[_PRS_04_])
					m_overpressureChan.prs_04 = 1;
				else
					m_overpressureChan.prs_04 = 0;
			}

			if(m_overpressureChanEn.prs_05)
			{
				if(m_prsSamples[8] > m_overpressureLimitAdc[_PRS_05_] || m_prsSamples[9] > m_overpressureLimitAdc[_PRS_05_])
					m_overpressureChan.prs_05 = 1;
				else
					m_overpressureChan.prs_05 = 0;
			}

			if(m_overpressureChanEn.prs_06)
			{
				if(m_prsSamples[10] > m_overpressureLimitAdc[_PRS_06_] || m_prsSamples[11] > m_overpressureLimitAdc[_PRS_06_])
					m_overpressureChan.prs_06 = 1;
				else
					m_overpressureChan.prs_06 = 0;
			}

			if(m_overpressureChanEn.prs_07)
			{
				if(m_prsSamples[12] > m_overpressureLimitAdc[_PRS_07_] || m_prsSamples[13] > m_overpressureLimitAdc[_PRS_07_])
					m_overpressureChan.prs_07 = 1;
				else
					m_overpressureChan.prs_07 = 0;
			}

			if(m_overpressureChanEn.prs_08)
			{
				if(m_prsSamples[14] > m_overpressureLimitAdc[_PRS_08_] || m_prsSamples[15] > m_overpressureLimitAdc[_PRS_08_])
					m_overpressureChan.prs_08 = 1;
				else
					m_overpressureChan.prs_08 = 0;
			}

			if(*((short*)(&m_overpressureChan)) != 0)
			{
				stopWth();
				stopWtp();
				if(*((short*)(&m_overpressureChan)) != *((short*)(&m_overpressureChan_previous)))
				{
					m_sauPcProtocol->sendOverpressureInfo(m_overpressureLimit, m_overpressureChanEn, m_overpressureChan);
					goBuzzerTimes(ALARM_LIKE, 2);
				}
			}

			m_overpressureChan_previous = m_overpressureChan;
		}else
		{
			m_prsChan->clearFifo_isr();
		}
	}
	
	//--- Checking ADuCs states timer ---
	if (m_timerWaitAduCResponse.Match())
	{
		m_timerWaitAduCResponse.Preset(_TIME_WAIT_ADuC_RESP_DEFAULT);
		if (!checkADuCStatus(m_ADuCsWantedStatus))
		{
			switch (getSau7000status())
			{
				case Sau7000status_idle:
				
					break;
					
				case Sau7000status_sendingSettings:
					changeSau7000status(Sau7000status_idle);
					break;
					
				case Sau7000status_waitingADuCToStart:
					stopADuCs();
					
					if(m_emgzBoardPresent)
						m_emgProtocol->sendStopAcq();
					
					changeSau7000status(Sau7000status_waitingADuCToStop);
					break;
					
				case Sau7000status_sampling:
					stop_sampling_and_all_peripherals();
					
					break;
	
				case Sau7000status_waitingADuCToStop:
					changeSau7000status(Sau7000status_idle);
					
					break;
					
				case Sau7000status_error:
					break;
					
				case Sau7000status_wait:
					break;
					
				case Sau7000status_startupChecks:
					break;
				case Sau7000status_ADuCfrequencyCalibration:
					m_ADuCFrqCalState = ADuCfrqCal_init_forceStop;
					break;
				case Sau7000status_bluetoothCheck:
					break;
					
				case Sau7000status_checkADuCfwUpgrade:
					break;
					
				case Sau7000status_switchingOff:
					break;
					
				case Sau7000status_emgzBoardUpgrade:
					break;
					
				case Sau7000status_BT_byPassMode:
					break;
			
			}
		}else
		{
			m_timerWaitAduCResponse.Stop();
		}
	}

	//---- Sau7000 state machine --------
	switch (getSau7000status())
	{
		case Sau7000status_idle:
			Sau7000status_idle_Handler();
			break;
			
		case Sau7000status_sendingSettings:
			Sau7000status_sendingSettings_Handler();
			break;
			
		case Sau7000status_waitingADuCToStart:
			Sau7000status_waitingADuCToStart_Handler();
			break;
			
		case Sau7000status_sampling:
			Sau7000status_sampling_Handler();
			break;
			
		case Sau7000status_waitingADuCToStop:
			Sau7000status_waitingADuCToStop_Handler();
			break;
			
		case Sau7000status_error:
			Sau7000status_error_Handler();
			break;
			
		case Sau7000status_wait:
			Sau7000status_wait_Handler();
			break;	
			
		case Sau7000status_startupChecks:
			Sau7000status_startupChecks_Handler();
			break;
			
		case Sau7000status_ADuCfrequencyCalibration:
			Sau7000status_ADuCfrequencyCalibration_Handler();
			break;
			
		case Sau7000status_bluetoothCheck:
			Sau7000status_bluetoothCheck_Handler();
			break;
				
		case Sau7000status_checkADuCfwUpgrade:
			Sau7000status_checkADuCfwUpgrade_Handler();
			break;

		case Sau7000status_switchingOff:
			break;
					
		case Sau7000status_emgzBoardUpgrade:
			Sau7000status_emgzBoardUpgrade_Handler();
			break;
		case Sau7000status_BT_byPassMode:
			Sau7000status_BT_byPassMode_Handler();
			break;
	}

	//---- verify that the maximum  time to restar coomunication is not elapsded
	if(m_timerRetryRestartComm.Match())
	{
		stop_sampling_and_all_peripherals();
	}
	if(m_timerResetRetries.Match())
	{
		m_numRetriesRestartComm = 0;
	}

	//--- In an aduc is not responding we start from the beginning ---
	if(getSau7000status() != Sau7000status_startupChecks && getSau7000status() != Sau7000status_checkADuCfwUpgrade)
	{
		for(i = 0; i < ADuC_NUM; i++)
		{
			if(m_ADuC_numErrors[i] >= 9)
			{
				m_ADuC_numErrors[i] = 0;
				stop_sampling_and_all_peripherals();
				changeSau7000status(Sau7000status_startupChecks);
				pageMenu->ChangePage(pADuCstatus);
				m_sauStrChk_state = strUpChk_ADuC;
				ADuC_startupChecks_Init();
				m_numADuC_ERR = 0;
				m_ADuCToUpgrade = false;
				m_calibrateADuCFrq = false;
				break;
			}
		}
	}

	//-------------- Bubble detector -------
	if(m_bubbleDetectionEn)
	{
		//------------ bubble test -----------------
		if(m_bubbleAirTestRunning)
		{
			if(m_bubbleAirTestTimer.getMsec() > 4 && PinTXE_BUBBLE == 0)
			{
				if(PinCMP_BUBBLE == 0)  // pin went low... wait 2ms before changing state 
				{
					m_bubbleAirTestTimer.Reset();
					PinTXE_BUBBLE = 1; //disable test
				}
			}
			

			if(m_bubbleAirTestTimer.getMsec() > 2 && PinTXE_BUBBLE == 1) // if the test passes, wait 2 ms before changing state
			{
				m_bubbleAirTestRunning = false;
				m_bubbleAirTestTimer.Reset();

			}

			if(m_bubbleAirTestTimer.getMsec() > 12) // if in 12 ms the pin does not go low the test fails
			{
				PinTXE_BUBBLE = 1; //disable test
				m_bubbleAirTestFailed = true;
				m_bubbleAirTestRunning = false;
				m_bubbleAirTestTimer.Reset();
			}
			
		}else  // if air test does not run, wait the right to time to run it
		{
			if(m_bubbleAirTestTimer.getMsec() >= 500)
			{
				PinTXE_BUBBLE = 0;//enable test
				m_bubbleAirTestRunning = true;
				m_bubbleAirTestTimer.Reset();
			}
		}

		//---------- Bubble errors conditions -------------------
		if(m_bubbleAirDetected || m_bubbleAirTestFailed || (PinCMP_BUBBLE == 0 && !m_bubbleAirTestRunning))
		{
			stopWtp();
			stopWth();
			PinCMD_PINZA = 1;  // close
			
		}
		
	}
}

void SAU7000_Manager::FlashLed()
{

	m_blinkLed++;

	if(	m_blinkLed == m_blinkLedValue )
	{
		PinLED1 = LED_LIFE_ON;
		PinLED_LIFE_PLD	 = 0;
	}					

	if(	m_blinkLed == (m_blinkLedValue << 1) )
	{
		PinLED1 = LED_LIFE_OFF;
		PinLED_LIFE_PLD	 = 1;
		m_blinkLed = 0;
	}
	
}


void SAU7000_Manager::Sau7000status_idle_Handler()
{
	//---- checking for ADuC microcontroller status ----
	if(m_timerCheckADuCStatus.Match())
	{
		m_timerCheckADuCStatus.Preset(_TIME_CHECK_ADuC_STATUS_);
		getADuCsInfo();
	}
	
	if(m_timerCheckEmgBoardStatus.Match())
	{
		m_emgProtocol->sendInfoRequest();
		m_emgProtocol->sendStatusRequest();
		m_timerCheckEmgBoardStatus.Preset(_TIME_CHECK_EMGZ_STATUS_);
	}

	if(m_COMM_state == comm_chekingBTconnection)
	{
		changeSau7000status(Sau7000status_bluetoothCheck);
	}
}

void SAU7000_Manager::Sau7000status_sendingSettings_Handler()
{
	if(checkADuCStatus(ADuCStatus_set))
	{
		startADuCs();
		
		if(m_emgzBoardPresent)
		{
			m_emgProtocol->sendStartAcq();
		}

		m_timerCheckADuCStatusWhileSampling.Preset(500);
		m_timerCheckEmgBoardStatus.Preset(500);
		
		changeSau7000status(Sau7000status_waitingADuCToStart);
	}else if(m_timerCheckADuCStatusWhileSampling.Match())
	{
		m_genericErrors |= (1 << gErr_ADuC_mismatchOnStart);
		stop_sampling_and_all_peripherals();
	}
}

void SAU7000_Manager::Sau7000status_waitingADuCToStart_Handler()
{	
	if(checkADuCStatus(ADuCStatus_acquiring) && ( m_emgzBoardStatus == emgzBoardStatus_acquiring || !m_emgzBoardPresent))
	{
		m_sauPcProtocol->sendSauPcAcquisitionStarted();

		changeSau7000status(Sau7000status_sampling);

		m_timerRetryRestartComm.Stop();
		
		m_timerCheckADuCStatusWhileSampling.Preset(_TIME_CHECK_ADuC_STATUS_WHILE_SAMPLING_);
		m_timerCheckEmgBoardStatus.Preset(_TIME_CHECK_EMGZ_STATUS_);
		m_timerCheckChanSyncro.Preset(100);
	}else if (m_timerCheckADuCStatusWhileSampling.Match() || (m_timerCheckEmgBoardStatus.Match() && m_emgzBoardPresent))
	{
		m_genericErrors |= (1 << gErr_ADuC_mismatchOnStart);
		SauPcMsg_stopAcquisitionHandler(AcqStopped_ADuCNotAnswerToStatusCmd);
		m_timerCheckADuCStatus.Preset(500);
		m_timerCheckEmgBoardStatus.Preset(500);
	}
}

void SAU7000_Manager::Sau7000status_sampling_Handler()
{
	int i, k;
	int numSamplesFifoed[_NUM_OF_PRS_CHAN_];
	int minP, maxP, numP, diffP;
	int minW, maxW, numW, diffW;
	short dataW;
	long dataDW;
	
	if(m_timerCheckADuCStatusWhileSampling.Match())
	{
		m_timerCheckADuCStatusWhileSampling.Preset(_TIME_CHECK_ADuC_STATUS_WHILE_SAMPLING_);

		getADuCsStatus();
		if (!checkADuCStatus(ADuCStatus_acquiring))
		{
			m_genericErrors |= (1 << gErr_ADuC_mismatchOnAcq);
			m_whyAcqWasStopped = AcqStopped_ADuCMismatchOnAcqStatus;
			stop_sampling_and_all_peripherals();
		}else
		{
		//---- check ADuC Acquisition Timer ---
			for (i = ADuC_VINF; i < ADuC_NUM; i++ )
			{
				if(m_ADuCacqCheckTimer[i].Match())
				{
					m_genericErrors |= (1 << gErr_ADuC_timeOutOnSampling);
					m_whyAcqWasStopped = AcqStopped_ADuC_timeOutOnSampling;
					stop_sampling_and_all_peripherals();
				}
			}
		}
	}
	
	if(m_timerCheckEmgBoardStatus.Match())
	{
		m_emgProtocol->sendStatusRequest();
		m_timerCheckEmgBoardStatus.Preset(_TIME_CHECK_EMGZ_STATUS_);
		if((m_emgzBoardStatus != emgzBoardStatus_acquiring) && m_emgzBoardPresent)
		{
			m_genericErrors |= (1 << gErr_Emgz_timeOutOnSampling);
			m_whyAcqWasStopped = AcqStopped_EMGZ_timeOutOnSampling;
			stop_sampling_and_all_peripherals();
		}
	}

	if (m_timerCheckChanSyncro.Match())
	{
		//--- check the syncro among the prs channels ---
		for (i = 0; i < _NUM_OF_PRS_CHAN_; i++)
		{
			numSamplesFifoed[i] = m_prsChan->getNumDataFifoed_isr(i);
		}
		maxP = 0;
		minP = 10000;
		for (i = 0; i < _NUM_OF_PRS_CHAN_; i++)
		{
			if (numSamplesFifoed[i] < minP)
				minP = numSamplesFifoed[i];
			if (numSamplesFifoed[i] > maxP)
				maxP = numSamplesFifoed[i];
		}
		diffP = maxP - minP;
		//--- check the syncro between weights channels ---
		for (i = 0; i < _NUM_OF_WEIGHT_CHAN_; i++)
		{
			numSamplesFifoed[i] = m_weightChan->getNumDataFifoed_isr(i);
		}
		maxW = 0;
		minW = 10000;
		for (i = 0; i < _NUM_OF_WEIGHT_CHAN_; i++)
		{
			if (numSamplesFifoed[i] < minW)
				minW = numSamplesFifoed[i];
			if (numSamplesFifoed[i] > maxW)
				maxW = numSamplesFifoed[i];
		}
		diffW = maxW- minW;

		if(maxP >= (maxW*10) )
		{
			if (diffP > 10) // maximum delay of 100ms 
			{
				for (i = 0; i < _NUM_OF_PRS_CHAN_; i++)
				{
					numP = m_prsChan->getNumDataFifoed_isr(i);
					if (numP > minP)
					{
						for(k = 0; k < (numP-minP); k++)
							m_prsChan->popAdcDataChan_isr(i,dataW); 
					}			
				}
				minP = (minP+5) / 10;	// syncronizing with weight channels (PRS sampling frq 100 Hz, wight channel sampling frq 10 Hz)
				for (i = 0; i < _NUM_OF_WEIGHT_CHAN_; i++)
				{
					numP = m_weightChan->getNumDataFifoed_isr(i) - minP;
					if (numP < 0)
						numP = 0;
					for (k = 0; k < numP; k++)
						m_weightChan->popAdcDataChan_isr(i,dataDW); 
				}
			}
		}else if (diffW > 1) // maximum delay of 100ms 
		{
			for (i = 0; i < _NUM_OF_WEIGHT_CHAN_; i++)
			{
				numW = m_weightChan->getNumDataFifoed_isr(i);
				if (numW > minW)
				{
					for(k = 0; k < (numW-minW); k++)
						m_weightChan->popAdcDataChan_isr(i,dataDW); 
				}			
			}
				// syncronizing with prs channels (PRS sampling frq 100 Hz, wight channel sampling frq 10 Hz)
			for (i = 0; i < _NUM_OF_PRS_CHAN_; i++)
			{
				numW = ((m_prsChan->getNumDataFifoed_isr(i)+5)/10) - minW;
				if (numW < 0)
					numW = 0;
				for (k = 0; k < numW; k++)
					m_prsChan->popAdcDataChan_isr(i,dataW); 
			}
		}
		
		m_timerCheckChanSyncro.Preset(2000);
	}
}

void SAU7000_Manager::Sau7000status_waitingADuCToStop_Handler()
{
	if(!m_timerWaitStatus.getStatus())
		m_timerWaitStatus.Preset(1);

	
	
	if(checkADuCStatus(ADuCStatus_idle) && (m_emgzBoardStatus == emgzBoardStatus_idle|| !m_emgzBoardPresent))
	{	
		if(!m_retryRestarComm)
		{
			m_sauPcProtocol->sendSauPcAcquisitionStopped(m_whyAcqWasStopped);
			changeSau7000status(Sau7000status_idle);
			m_timerCheckADuCStatus.Preset(_TIME_CHECK_ADuC_STATUS_);
			m_timerCheckEmgBoardStatus.Preset(_TIME_CHECK_EMGZ_STATUS_);
			
			m_retryRestarComm = false;
			m_numRetriesRestartComm = 3;
			m_timerRetryRestartComm.Stop();
			m_timerResetRetries.Stop();
		}else
		{
			if(!m_timerWaitStatus.Match())
				return;
			
			changeSau7000status(Sau7000status_sendingSettings);
	
			setADuCsAquisitionParameters();
			
			m_weightChan->clearFifo();
			m_prsChan->clearFifo();	
			m_timerCheckChanSyncro.Preset(100);
			
			m_sauPcProtocol->rstGenPacketsSent();
			m_sauPcProtocol->rstEmgPacketsSent();
			m_sauPcProtocol->rstPrsPacketsSent();
			m_sauPcProtocol->rstWeightPacketsSent();
			
			m_emgProtocol->rstEmgPacketsReceived();
			m_emgProtocol->rstImpPacketsReceived();

			m_timerCheckADuCStatusWhileSampling.Preset(500);
			m_timerCheckADuCStatus.Stop();
			m_timerCheckEmgBoardStatus.Preset(_TIME_CHECK_EMGZ_STATUS_);
		}
		
	}else if (m_timerCheckADuCStatusWhileSampling.Match() || (m_timerCheckEmgBoardStatus.Match() && m_emgzBoardPresent))
	{
		m_genericErrors |= (1 << gErr_ADuC_mismatchOnStop);
		m_whyAcqWasStopped = AcqStopped_GenericError;
		m_sauPcProtocol->sendSauPcAcquisitionStopped(m_whyAcqWasStopped);
		changeSau7000status(Sau7000status_idle);
		m_timerCheckADuCStatus.Preset(500);
		m_timerCheckEmgBoardStatus.Preset(500);

		m_retryRestarComm = false;
		m_numRetriesRestartComm = 3;
		m_timerRetryRestartComm.Stop();
		m_timerResetRetries.Stop();
	}
}

void SAU7000_Manager::Sau7000status_error_Handler()
{
	switch(m_sauError)
	{
		case SauError_noError:
			break;
		case SauError_ADuCsCanNotbeUpgraded:
			break;
		case SauError_watchdogError:
			break;
		case SauError_fpgaUpgradeFailed:
			break;
	}
	return;
}

void SAU7000_Manager::Sau7000status_wait_Handler()
{
	if(m_timerWaitStatus.Match())
	{
		m_timerWaitStatus.Stop();
		changeSau7000status(getSau7000previousStatus());
	}
}

void SAU7000_Manager::Sau7000status_startupChecks_Handler()
{
	int i;
	char str[40];
	int upgrade_result;
	DecTimer wait;
	
	switch (m_sauStrChk_state)
	{
		case strUpChk_logo:
			if(m_logoTimer.Match())
			{
				m_logoTimer.Stop();
				pageMenu->ChangePage(pageMessages);
				pageMessages->setMessage(0,"Verifing fpga..");
				pageMessages->setMessage(1," ");
				pageMessages->setMessage(2," ");
				pageMessages->setMessage(3," ");
				m_sauStrChk_state = strUpChk_fpga;
				m_fpgaChkState = fpgaChk_readBitsream;
			}
			break;
		case strUpChk_fpga:
			switch (m_fpgaChkState)
			{
				case fpgaChk_readBitsream:
					m_fpgaBitstream = FPGA_BITSTREAM_VERSION;
					
					sprintf(str, "L:%d W:%d",m_fpgaBitstream, fpgaWantedBitstream);
					pageMessages->setMessage(1, str);
					
					if(m_fpgaBitstream == fpgaWantedBitstream)
					{
						pageMessages->setMessage(2,"OK...");
						pageMessages->setMessage(3," ");
						m_fpgaChkState = fpgaChk_bitstreamOK;
					}else
					{
						pageMessages->setMessage(2,"ERROR...");
						pageMessages->setMessage(3," ");
						m_fpgaChkState = fpgaChk_bitstreamERROR;
					}
					m_logoTimer.Preset(2000);
					break;
				case fpgaChk_bitstreamOK:
					if(m_logoTimer.Match())
					{
						
						m_sauStrChk_state = strUpChk_getCalVal;
						
						m_logoTimer.Stop();
					}
					break;
				case fpgaChk_bitstreamERROR:
					if(m_logoTimer.Match())
					{
						m_sauError = SauError_fpgaUpgradeFailed;
						changeSau7000status(Sau7000status_error);
						
						m_logoTimer.Stop();

						pageMenu->ChangePage(pageMessages);
						pageMessages->setMessage(0, "FPGA error!");
						pageMessages->setMessage(1, "---------------");
						pageMessages->setMessage(2, "Contact Menfis'");
						pageMessages->setMessage(3, "technical support!");
					}
					
					break;
			}
			
			break;
		case strUpChk_getCalVal:
			readToEE_prsCalValue();
			readToEE_weightCalValue();
			setOverpressureAdcLimit();
			pageMenu->ChangePage(pADuCstatus);
			m_sauStrChk_state = strUpChk_ADuC;
			ADuC_startupChecks_Init();
			m_numADuC_ERR = 0;
			m_ADuCToUpgrade = false;
			break;
		case strUpChk_ADuC:
			
			//---- checking for ADuC microcontroller info ----
			if(m_timerCheckADuCStatus.Match())
			{
				m_timerCheckADuCStatus.Preset(500);
				
				getADuCsInfo();
			}
			
			if (pageMenu->GetCurrent() == ((CMenuPage*)pADuCstatus) )
			{
				/*if(!m_ADuCTimerStartupCheck.getStatus())
				{
					m_ADuCTimerStartupCheck.Preset(500);
				}*/
			
				if(m_ADuCTimerStartupCheck.Match()) // waiting...
				{
				
					if(m_numADuCToCheck >= ADuC_NUM + 1)//.. wait to check all the ADuCs
					{
						m_numADuCToCheck = 0;
						m_ADuCTimerStartupCheck.Stop();
						if(!m_ADuCToUpgrade)// everything is fine..
						{
							m_numADuCToCheck = 0;
							m_numADuC_ERR = 0;
							if(m_calibrateADuCFrq)
							{
								m_ADuCFrqCalState = ADuCfrqCal_init_forceStop;
								changeSau7000status(Sau7000status_ADuCfrequencyCalibration);
								pageMenu->ChangePage(pageADuCfrqCal);
							}else
							{
								changeSau7000status(Sau7000status_idle);
								pageMenu->ChangePage(pMain);
							}
						}else // we have to upgrade the ADuCs...
						{
							changeSau7000status(Sau7000status_checkADuCfwUpgrade);
							m_ADuCUpgradeManger->keepReset();
							for(i = 0; i <  ADuC_NUM; i++)
							{
								m_ADuC_Protocols[i]->clearRx();
								m_ADuC_Protocols[i]->clearTx();
								m_ADuC_Protocols[i]->setUartDevice((UartDeviceAbstract*)NULL);
								pADuCstatus->setADuCstate(i, " ");
								pUART_ADuC[i]->TurnOff();
								pUART_ADuC[i]->SetBaudRate(UartDeviceAbstract::BR38400);
								pUART_ADuC[i]->ClearTransmissionBuffer();
								pUART_ADuC[i]->ClearReceptionBuffer();
								pUART_ADuC[i]->TurnOn();
								m_watchDogTimer.Stop();
								
							}
							m_numADuCToUpgrade = 0;
							m_ADuCUpgradeErrors = 0;
							m_ADuCUpgradeManger->setUartDevice((UartDeviceAbstract*)pUART_ADuC[m_numADuCToUpgrade]);
							m_ADuCUpgradeManger->startUpgrade();
						}
					}else if (m_numADuCToCheck < ADuC_NUM)//checking ADuCs..
					{
						if( m_ADuCStatus[m_numADuCToCheck] != ADuCStatus_notRespondingToCommand && // ADuCs OK
								m_ADuCStatus[m_numADuCToCheck] != ADuCStatus_error && 
								m_ADuCStatus[m_numADuCToCheck] != ADuCStatus_unknown)
						{
							word ADuC_fw = (word)(ADuC_REV_MAJOR << 8);
							ADuC_fw += ADuC_REV_MINOR; 
							
							if ( ADuC_fw == m_AduC_firmwareVersion[m_numADuCToCheck])
							{
								pADuCstatus->setADuCstate(m_numADuCToCheck, "OK");
								m_numADuC_OK++;
							}else
							{
								m_ADuCToUpgrade = true;
								pADuCstatus->setADuCstate(m_numADuCToCheck, "UPG");
							}
							m_ADuCTimerStartupCheck.Preset(500);
							m_numADuCToCheck++;
						}else	// ADuC not responding...
						{
							pADuCstatus->setADuCstate(m_numADuCToCheck, "ERR");
							if(m_numReset < 10)
							{
								ADuC_startupChecks_Init();
							}else
							{
								m_ADuCTimerStartupCheck.Preset(500);
								m_numADuCToCheck++;
								m_ADuCToUpgrade = true;
								m_numADuC_ERR++;
							}
						}
					}else // wait one moment so the user can see the check result....
					{
						m_ADuCTimerStartupCheck.Preset(500);
						m_numADuCToCheck++;
					}
					
				}
			}
			break;
	}
}

void SAU7000_Manager::Sau7000status_ADuCfrequencyCalibration_Handler()
{
	int i = 0;
	char str[20];
	switch(m_ADuCFrqCalState)
	{
		case ADuCfrqCal_init_forceStop:
			if(!checkADuCStatus(ADuCStatus_idle))
			{
				stopADuCs();
			}
			
			for (i = 0; i < ADuC_NUM; i++)
			{
				m_ADuC_Protocols[i]->setSamplingFrqCalibrationTime(20000);
				pageADuCfrqCal->setADuCinfo(i,"force stop");
			}
			m_ADuCFrqCalState = ADuCfrqCal_init_set;
			break;
		case ADuCfrqCal_init_set:
			if(checkADuCStatus(ADuCStatus_idle))
			{
				setADuCsAquisitionParameters();
				m_ADuCFrqCalState = ADuCfrqCal_waitingADuCToBeSet;
				for (i = 0; i < ADuC_NUM; i++)
					pageADuCfrqCal->setADuCinfo(i,"setting");
			}
			break;
		case ADuCfrqCal_waitingADuCToBeSet:
			if(checkADuCStatus(ADuCStatus_set))
			{
				startADuCs();
				m_ADuCFrqCalState = ADuCfrqCal_waitingADuCToStartAcq;
				for (i = 0; i < ADuC_NUM; i++)
					pageADuCfrqCal->setADuCinfo(i,"starting");
			}
			break;
		case ADuCfrqCal_waitingADuCToStartAcq:
			if(checkADuCStatus(ADuCStatus_acquiring))
			{
				for (i = 0; i < ADuC_NUM; i++)
				{
					m_ADuC_Protocols[i]->enableFrequencyCal(true);
					pageADuCfrqCal->setADuCinfo(i,"calibrating");
				}
				m_ADuCFrqCalTimer.Preset(m_ADuC_Protocols[0]->getSamplingFrqCalibrationTime());
				m_ADuCFrqCalState = ADuCfrqCal_calibrating;
			}
			break;
		case ADuCfrqCal_calibrating:
			for (i = 0; i < ADuC_NUM; i++)
			{
				sprintf(str,"%d", m_ADuC_Protocols[i]->getNumPacketsReceived());
				pageADuCfrqCal->setADuCinfo(i,str);
			}
			if(m_ADuCFrqCalTimer.Match())
			{
				stopADuCs();
				for (i = 0; i < ADuC_NUM; i++)
				{
					m_ADuC_Protocols[i]->enableFrequencyCal(false);
				}	
				m_ADuCFrqCalTimer.Preset(5000);
				m_ADuCFrqCalState = ADuCfrqCal_waitingADuCToStop;
			}
			break;
		case ADuCfrqCal_waitingADuCToStop:
			if(checkADuCStatus(ADuCStatus_idle) && m_ADuCFrqCalTimer.Match())
			{
				for (i = 0; i < ADuC_NUM; i++)
				{
					m_ADuC_Protocols[i]->calculateSamplingFrq();
					sprintf(str,"frq=%d,frqC=%d",m_ADuC_Protocols[i]->getWantedSamplingFrq(), m_ADuC_Protocols[i]->getCorrectedSamplingFrq());
					pageADuCfrqCal->setADuCinfo(i,str);
				}	
				m_ADuCFrqCalTimer.Preset(5000);
				m_ADuCFrqCalState = ADuCfrqCal_calculatingNewFrq;
			}
			break;
		case ADuCfrqCal_calculatingNewFrq:
			if( m_ADuCFrqCalTimer.Match())
			{
				for (i = 0; i < ADuC_NUM; i++)
				{
					pageADuCfrqCal->setADuCinfo(i," ");
				}	
				m_ADuCFrqCalTimer.Stop();
				changeSau7000status(Sau7000status_idle);
				pageMenu->ChangePage(pMain);
				m_ADuCFrqCalState = ADuCfrqCal_init_forceStop;
			}
			break;
		case ADuCfrqCal_error:
			break;
	}
}

void SAU7000_Manager::Sau7000status_bluetoothCheck_Handler()
{
	int i;
	byte data;
	char str[30];
	int btError = 0; // 21/01/2022
	
	switch (m_BTcheckStatus)
	{
		case BTchk_idle:
			if(m_BTidleStatusTimer.Match())
				changeBTcheckStatus(BTchk_verify_BR);
			break;
		case BTchk_verify_BR:
			if (m_bluetoothProtocol->VERIFY_BAUD_RATE() == 1)
				changeBTcheckStatus(BTchk_verifing_BR);
			break;
		case BTchk_verifing_BR:
			break;
			
		case BTchk_ask_Info:
			if (m_bluetoothProtocol->INFO() == 1)
				changeBTcheckStatus(BTchk_waiting_Info);
			break;
		case BTchk_waiting_Info:
			break;
				
		case BTchk_set_BT_NAME:
			if (m_bluetoothProtocol->SET_BT_NAME() == 1)
				changeBTcheckStatus(BTchk_waiting_set_BT_NAME);
			break;
		case BTchk_waiting_set_BT_NAME:
			break;

		case BTchk_set_Profile:
			if (m_bluetoothProtocol->SET_PROFILE() == 1)
				changeBTcheckStatus(BTchk_waiting_Profile);
			break;
		case BTchk_waiting_Profile:
			break;
				
		case BTchk_set_BT_SSP:
			//m_bluetoothProtocol->SET_BTchk_SSP(iWRAP_BLUEGIGA_Protocol::spp_cap_none, iWRAP_BLUEGIGA_Protocol::mitm_protectionDisable, 1000);
			if (m_bluetoothProtocol->SET_BT_SSP() == 1)
				changeBTcheckStatus(BTchk_waiting_set_BT_SSP);
			break;
		case BTchk_waiting_set_BT_SSP:
			break;

		case BTchk_set_MSC:
			if (m_bluetoothProtocol->SET_CONTROL_MSC() == 1)
				changeBTcheckStatus(BTchk_waiting_MSC);
			break;
		case BTchk_waiting_MSC:
			break;
			
		case BTchk_set_BT_AUTH:
			if (m_bluetoothProtocol->SET_BT_AUTH() == 1)
				changeBTcheckStatus(BTchk_waiting_set_BT_AUTH);
			break;
		case BTchk_waiting_set_BT_AUTH:
			break;	
			
		case BTchk_CALL:
			/*ee_spi_read(EE_SPI_ADD_BT_ADDRESS_LOADED, &data, 1);

			if(data == EE_SPI_ADD_BT_VALID_ADDRESS_LOADED)
			{
				ee_spi_read(EE_SPI_ADD_BT_ADDRESS, (byte*)m_remoteBTaddr, 17);
				m_remoteBTaddr[17] = '\0';

				strcpy(str, "CALLING:");
				strcat(str, m_remoteBTaddr);
				pageInfo->addStr(str);
				if (m_bluetoothProtocol->CALL(m_remoteBTaddr) == 1)
				changeBTcheckStatus(BTchk_wating_CALL);
					
			}else
			{
				changeBTcheckStatus(BTchk_RING);
			}*/
			changeBTcheckStatus(BTchk_RING);
			
			break;
		case BTchk_wating_CALL:
			break;

		case BTchk_CLOSE:
			if(m_bluetoothProtocol->CLOSE() == 1)
				changeBTcheckStatus(BTchk_waiting_CLOSE);
			break;
		case BTchk_waiting_CLOSE:
			break;

		case BTchk_RING:
			pageInfo->addStr("Waiting RING....");
			changeBTcheckStatus(BTchk_waiting_RING);
			break;
		case BTchk_waiting_RING:
			/* #21012021 - Sometimes it stops here and it never goes out 
			1 - Check if there is a BT error*/
			
			// btError = m_bluetoothProtocol->popBlueError();
			
			// (int)(stdUart_Hw_OverrunError)
			/*
			if (btError == 4)
			{
				// pageInfo->addStr("RING ABSENT - Overrun...");
				// Clear the uart buffer 
				//  m_bluetoothProtocol->clearRx();
				//  m_bluetoothProtocol->clearTx();
				//  changeSau7000status(Sau7000status_bluetoothCheck);
			}
			*/
			
			break;

	/*	case BTchk_PAIR:
			ee_spi_read(EE_SPI_ADD_BT_ADDRESS_LOADED, &data, 1);

			if(data == EE_SPI_ADD_BT_VALID_ADDRESS_LOADED)
			{
				ee_spi_read(EE_SPI_ADD_BT_ADDRESS, (byte*)m_remoteBTaddr, 17);
				m_remoteBTaddr[17] = '\0';

				strcpy(str, "PAIRING:");
				strcat(str, m_remoteBTaddr);
				pageInfo->addStr(str);
				pageInfo->addStr("Waiting PAIR....");
				if (m_bluetoothProtocol->PAIR(m_remoteBTaddr) == 1)
					changeBTcheckStatus(BTchk_waiting_PAIR);
					
			}else
			{
				changeBTcheckStatus(BTchk_RING);
			}
			
			break;
		case BTchk_waiting_PAIR:
			break;
			*/
		case BTchk_wait:
			if (m_BTwaitStatusTimer.Match())
			{
				m_BTwaitStatusTimer.Stop();
				changeBTcheckStatus(m_BTcheckPreviousStatus);
			}
			break;
	}
}

void SAU7000_Manager::Sau7000status_checkADuCfwUpgrade_Handler()
{
	char str[8];
	int i;
	DecTimer wait;
	
	if (m_numADuCToUpgrade < ADuC_NUM)
	{
		m_ADuCUpgradeManger->manager();
		
		switch (m_ADuCUpgradeManger->getUpgradeStatus())
		{
			case ADuCUpgradeManger::upgrade_Idle:
				
				break;
			case ADuCUpgradeManger::upgrade_syncronizing:
				pADuCstatus->setADuCstate(m_numADuCToUpgrade, "SYN");
				break;
			case ADuCUpgradeManger::upgrade_syncronized:
				pADuCstatus->setADuCstate(m_numADuCToUpgrade, "SYN");
				break;
			case ADuCUpgradeManger::upgrade_erasingDevice:
				
				pADuCstatus->setADuCstate(m_numADuCToUpgrade, "ERS");
				break;
			case ADuCUpgradeManger::upgrade_deviceErased:
				pADuCstatus->setADuCstate(m_numADuCToUpgrade, "ERS");
				break;
			case ADuCUpgradeManger::upgrade_upgradingDevice:
				sprintf(str,"U%3d", m_ADuCUpgradeManger->percentageUpgrated());
				pADuCstatus->setADuCstate(m_numADuCToUpgrade, str);
				break;
			case ADuCUpgradeManger::upgrade_deviceUpgrated:
				pADuCstatus->setADuCstate(m_numADuCToUpgrade, "UPR");
				break;
			case ADuCUpgradeManger::upgrade_verifing:
				sprintf(str,"V%3d", m_ADuCUpgradeManger->percentageVerified());
				pADuCstatus->setADuCstate(m_numADuCToUpgrade, str);
				break;
			case ADuCUpgradeManger::upgrade_verified:
				pADuCstatus->setADuCstate(m_numADuCToUpgrade, "OK");
				m_numADuCToUpgrade++;
				if(m_numADuCToUpgrade < ADuC_NUM)
				{
					m_ADuCUpgradeErrors = 0;
					m_ADuCUpgradeManger->setUartDevice((UartDeviceAbstract*)pUART_ADuC[m_numADuCToUpgrade]);
					m_ADuCUpgradeManger->startUpgrade();
				}else
				{
					m_ADuCUpgradeManger->setUartDevice((UartDeviceAbstract*)NULL);
				}
				break;
			case ADuCUpgradeManger::upgrade_aborted:
				m_ADuCUpgradeManger->startUpgrade();
				break;
			case ADuCUpgradeManger::upgrade_Error:
				if (m_ADuCUpgradeErrors < 5)
				{
					pADuCstatus->setADuCstate(m_numADuCToUpgrade, "ERR");
					m_ADuCUpgradeManger->abort();
					m_timerWaitStatus.Preset(1500);
					while(!m_timerWaitStatus.Match());
				}else
				{
					ADuCsNormalMode();
					m_sauError = SauError_ADuCsCanNotbeUpgraded;
					changeSau7000status(Sau7000status_error);
					pageMenu->ChangePage(pageMessages);
					pageMessages->setMessage(0, "ADuCs upgrade error!");
					pageMessages->setMessage(1, "---------------");
					pageMessages->setMessage(2, "Contact Menfis'");
					pageMessages->setMessage(3, "technical support!");
				}
				m_ADuCUpgradeErrors++;
				break;
		}
	}else
	{
		setMessage(0, "-------------------------");
		setMessage(1, "-------------------------");
		setMessage(2, "------   UPGRADE   ------");
		setMessage(3, "------   SUCCEED   ------");
		setMessage(4, "-------------------------");
		setMessage(5, "-------------------------");
		setMessage(6, "----  THE DEVICE IS -----");
		setMessage(7, "----  SWITCHING OFF -----");
		setMessage(8, "-------------------------");
		setMessage(9, "-------------------------");	
		display_update();
		
		wait.Preset(5000);
		while(!wait.Match());		
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");

		/*while (1)
		{
			PinEN_POWER = 1;
			PinEN_POWER = 1;
		}	*/
		
		fp = (void (*)()) LOADER_FLASH_START_ADDRESS;				// inizio dell'applicone
	    (*fp)();	
		/*ADuCsNormalMode();
		changeSau7000status(Sau7000status_startupChecks);
		m_timerWaitStatus.Preset(2000);
		changeSau7000status(Sau7000status_wait);
		for(i = 0; i <	ADuC_NUM; i++)
		{	
			getADuCsInfo();
			pADuCstatus->setADuCstate(i, " ");
		}*/
	}

}

void SAU7000_Manager::Sau7000status_emgzBoardUpgrade_Handler()
{

}

void SAU7000_Manager::Sau7000status_BT_byPassMode_Handler()
{

}

void SAU7000_Manager:: ADuC_startupChecks_Init()
{
	int i;
	m_numReset++;
	m_numADuC_OK = 0;
	m_numADuCToCheck = 0;
	for (i = 0; i < ADuC_NUM; i++)
	{
		//m_ADuC_Protocols[i]->clearTx();
		//m_ADuC_Protocols[i]->clearRx();
		//m_ADuC_Protocols[i]->TurnOff();
		pADuCstatus->setADuCstate(i, " ");
		m_ADuCStatus[i] = ADuCStatus_unknown;
		m_ADuC_numErrors[i] = 0;

		m_ADuC_Protocols[i]->clearTx();
		m_ADuC_Protocols[i]->clearRx();
		m_ADuC_Protocols[i]->rstNewPacketReceived();
	}
	//for (i = 0; i < ADuC_NUM; i++)
	//{
		//m_ADuC_Protocols[i]->TurnOn();
	//}
	
	m_ADuCUpgradeManger->hwReset();
	
	m_ADuCTimerStartupCheck.Preset(1000);
	m_timerCheckADuCStatus.Preset(100);
}

void SAU7000_Manager::watchDogManager()
{
	if(m_watchDogTimer.Match() && m_watchdogEnabled)
	{
		if (getSau7000status() != Sau7000status_bluetoothCheck && getSau7000status() != Sau7000status_startupChecks 
			&& getSau7000status() != Sau7000status_checkADuCfwUpgrade && getSau7000status() != Sau7000status_emgzBoardUpgrade
			&& getSau7000status() != Sau7000status_BT_byPassMode && getSau7000status() != Sau7000status_error 
			&& getSau7000status() != Sau7000status_ADuCfrequencyCalibration)
		{
			m_whyAcqWasStopped = AcqStopped_WatchdogError;
			
			m_retryRestarComm = false;
			m_numRetriesRestartComm = 3;
			m_timerRetryRestartComm.Stop();
			m_timerResetRetries.Stop();
			
			stop_sampling_and_all_peripherals();
			m_sauError = SauError_watchdogError;
			changeSau7000status(Sau7000status_error);
			m_watchDogTimer.Stop();
			m_watchdogEnabled = false;
			pageMenu->ChangePage(pageMessages);
			pageMessages->setMessage(0, "Watchdog error!");
			pageMessages->setMessage(1, "---------------");
			pageMessages->setMessage(2, "Verify communication");
			pageMessages->setMessage(3, "with PC!");
			goBuzzerTimes(ALARM_LIKE, 2);
		}
	}
}

void SAU7000_Manager::stop_sampling_and_all_peripherals()
{	
	int i;
	if(getSau7000status() == Sau7000status_startupChecks ||
	   getSau7000status() == Sau7000status_checkADuCfwUpgrade)
	return;

	if(!m_retryRestarComm || m_numRetriesRestartComm >= 3 || m_timerRetryRestartComm.Match())
	{
		//--- WTP ---	
		m_wtpSpeed = 0;
		m_wtp->setVelocity(m_wtpSpeed);
		m_wtp->setEnable(false);

		m_wtpStatus = WtpState_stop;
		pMain->setWTPisMoving(false);
		pMain->setWTPspeed(m_wtpSpeed);
		
		//--- WTH ---
		m_wth->power(false);
		m_wth->setEnable(false);
		m_wthStep = 0;
		m_wthSpeed = m_wth->setLinearVelocity(0.0);
		m_wth->setStep((double)m_wthStep);
		
		m_wthPowered = 0;
		m_wthStatus = WthState_stop;
		m_wthEndSwitchesStatus = WthEndSwitchesStatus_unknown;
		
		pMain->setLimitSwitch(CMenuMain::lmtsw_notReached, CMenuMain::lmtsw_notReached);
		pMain->setWTHisMoving(m_wth->isEnable());
		pMain->setWTHspeed(m_wthSpeed);
		
		//--- BUBBLE ----
		m_bubbleClampStatus = BubbleClampState_close;
		
		//--- SAMPLING ---
		changeSau7000status(Sau7000status_waitingADuCToStop);
	
		stopADuCs();

		m_weightChan->clearFifo();
		m_prsChan->clearFifo();

		if(m_emgzBoardPresent)
			m_emgProtocol->sendStopAcq();
		
		m_timerCheckADuCStatusWhileSampling.Stop();
		
		m_timerCheckADuCStatus.Preset(_TIME_CHECK_ADuC_STATUS_);

		m_retryRestarComm = false;
		m_numRetriesRestartComm = 3;
		m_timerRetryRestartComm.Stop();
		m_timerResetRetries.Stop();
		
		m_numRetriesRestartComm++;
	}else if (getSau7000status() == Sau7000status_sampling)
	{
		if(!m_timerRetryRestartComm.getStatus())
			m_timerRetryRestartComm.Preset(500);
		if(!m_timerResetRetries.getStatus())
			m_timerResetRetries.Preset(10000);
		//--- SAMPLING ---
		changeSau7000status(Sau7000status_waitingADuCToStop);

		for (i = 0; i < ADuC_NUM; i++)
		{
			m_ADuC_Protocols[i]->clearRx();
			m_ADuC_Protocols[i]->clearTx();
		}
		stopADuCs();

		m_weightChan->clearFifo();
		m_prsChan->clearFifo();

		if(m_emgzBoardPresent)
			m_emgProtocol->sendStopAcq();
		
		m_timerCheckADuCStatusWhileSampling.Stop();
		
		m_timerCheckADuCStatus.Preset(_TIME_CHECK_ADuC_STATUS_);

		m_numRetriesRestartComm++;
		m_timerRetryRestartComm.Preset(1000);
		m_timerWaitStatus.Preset(500);
	}else
	{
		if(!m_timerRetryRestartComm.getStatus())
			m_timerRetryRestartComm.Preset(500);
	}
	//goBuzzer(LONG_LIKE);
	
}


void SAU7000_Manager::WthManager()
{
	if (m_wth == NULL)
		return;
	
	if (!m_wth->isPowered())
		return;
	
	m_wth->manger();
	if ((m_wth->getLmtSwLowStatus() == WthAbstract::lmtSw_error) || (m_wth->getLmtSwHighStatus() == WthAbstract::lmtSw_error))
	{
		m_wthStatus = WthState_error;
		pMain->setWTHisMoving(false);
	}else
	{
		if (m_wth->isEnable())
		{
			m_wthStatus = WthState_running;
			pMain->setWTHisMoving(true);
		}else
		{
			m_wthStatus = WthState_stop;
			pMain->setWTHisMoving(false);
		}
	}
	
	if ((m_wth->getLmtSwLowStatus() == WthAbstract::lmtSw_error) || (m_wth->getLmtSwHighStatus() == WthAbstract::lmtSw_error))
	{
		if ((m_wth->getLmtSwLowStatus() == WthAbstract::lmtSw_error) && (m_wth->getLmtSwHighStatus() == WthAbstract::lmtSw_error))
		{
			m_wthEndSwitchesStatus = WthEndSwitchesStatus_error;
			pMain->setLimitSwitch(CMenuMain::lmtsw_error, CMenuMain::lmtsw_error);
		}
		else if ((m_wth->getLmtSwLowStatus() == WthAbstract::lmtSw_error) && (m_wth->getLmtSwHighStatus() != WthAbstract::lmtSw_error))
		{
			m_wthEndSwitchesStatus = WthEndSwitchesStatus_errorBottom;
			pMain->setLimitSwitch(CMenuMain::lmtsw_error, CMenuMain::lmtsw_notReached);
		}
		else if ((m_wth->getLmtSwLowStatus() != WthAbstract::lmtSw_error) && (m_wth->getLmtSwHighStatus() == WthAbstract::lmtSw_error))
		{
			pMain->setLimitSwitch(CMenuMain::lmtsw_notReached, CMenuMain::lmtsw_error);
			m_wthEndSwitchesStatus = WthEndSwitchesStatus_errorTip;
		}
	}else
	{	
		if ((m_wth->getLmtSwLowStatus() == WthAbstract::lmtSw_notReached) && (m_wth->getLmtSwHighStatus() == WthAbstract::lmtSw_notReached))
		{
			m_wthEndSwitchesStatus = WthEndSwitchesStatus_noOnesReached;
			pMain->setLimitSwitch(CMenuMain::lmtsw_notReached, CMenuMain::lmtsw_notReached);
		}else if (m_wth->getLmtSwLowStatus() == WthAbstract::lmtSw_reached)
		{
			pMain->setLimitSwitch(CMenuMain::lmtsw_reached, CMenuMain::lmtsw_notReached);
			m_wthEndSwitchesStatus = WthEndSwitchesStatus_bottomReached;
		}else if (m_wth->getLmtSwHighStatus() == WthAbstract::lmtSw_reached)
		{
			pMain->setLimitSwitch(CMenuMain::lmtsw_notReached, CMenuMain::lmtsw_reached);
			m_wthEndSwitchesStatus = WthEndSwitchesStatus_tipReached;
		}else 
		{
			pMain->setLimitSwitch(CMenuMain::lmtsw_reached, CMenuMain::lmtsw_reached);
			m_wthEndSwitchesStatus = WthEndSwitchesStatus_unknown;
		}
	}
	
}

void SAU7000_Manager::WtpManager()
{
	if(m_wtp == NULL)
		return;

	m_wtp->manager();

	if (m_wtp->driverStatusChanged())
	{
		m_sauPcProtocol->sendSauPcWtp(m_wtpSpeed, m_wtpStatus, m_wtpLitStatus, m_wtp->getDriverStatus());
	}
}

void SAU7000_Manager::KeyboardManager()
{
	int key = NO_KEY;
	static int switchOff = 0;

	if (m_keyboard == NULL)
		return;
	//---- Checking for new keys pressed -----

	m_keyboard->manager();
	
	if (key == NO_KEY)
	{
		if(!m_keyboard->pop(key))
		{
			key = NO_KEY;
		}
	}
	
	if (key == KEY_ON_OFF)
	{
		if(getSau7000status() == Sau7000status_bluetoothCheck)
		{
			if(m_BTcheckStatus == BTchk_idle)
			{
				changeBTcheckStatus(BTchk_verify_BR);
			}
		}else if (getSau7000status() == Sau7000status_emgzBoardUpgrade)
		{
			//--- re-set UARTs ---
			m_emgProtocol->setBypassMode(false, NULL);
			m_sauPcProtocol->setBypassMode(false, NULL);
			
			m_emgProtocol->SetBaudRate( UartDeviceAbstract::BR230400);
			m_sauPcProtocol->SetBaudRate( UartDeviceAbstract::BR460800);
			m_sauPcProtocol->SetHwFlwCtl(UartDeviceAbstract::hwFlwCtl_CtsRts);
				
			m_timerCheckEmgBoardStatus.Preset(2000);
			changeSau7000status(Sau7000status_idle);
			pageMenu->ChangePage(pMain);
			m_watchDogTimer.Stop();
			goBuzzerTimes(LONG_LIKE, 1);
			
		}else if (getSau7000status() == Sau7000status_BT_byPassMode)
		{
			m_bluetoothProtocol->setBypassMode(false, NULL);
			m_sauPcProtocol->setBypassMode(false, NULL);

			changeSau7000status(Sau7000status_idle);
			pageMenu->ChangePage(pMain);
			m_watchDogTimer.Stop();
			goBuzzerTimes(LONG_LIKE, 1);
		}
		switchOff++;
		/*if (m_sauPcProtocol->sendSauPcRemoteKey(key) > 0)
		{
			key = NO_KEY;				
		}*/
	}else if (key == KEY_NUM_OF_KEYS)
	{
		switchOff = 0;
	}
		
	if(switchOff == 4)
	{
		pageMenu->ChangePage(pageSwitchOff);
		pageSwitchOff->startSwitchOffTimer(4500);
		
		m_keyboard->setEnable(KEY_ON_OFF, false);

		m_whyAcqWasStopped = AcqStopped_SauIsSwitchingOff;
		
		m_retryRestarComm = false;
		m_numRetriesRestartComm = 0;
		m_timerRetryRestartComm.Stop();
		m_timerResetRetries.Stop(); 
		
		stop_sampling_and_all_peripherals();
		m_watchDogTimer.Stop();
		m_timerWaitStatus.Stop();
		changeSau7000status(Sau7000status_switchingOff);

		if (m_COMM_state == comm_BT)
		{
			m_sauPcProtocol->setUartDevice(pUART_PC);
			m_sauPcProtocol->setCOMM_type(SauPcProtocol::comm_FTDI_USBtoSERIAL);
			m_bluetoothProtocol->setUartDevice(pUART_BLUE);

			pUART_BLUE->ClearTransmissionBuffer();
			pUART_BLUE->ClearReceptionBuffer();
			pUART_PC->ClearTransmissionBuffer();
			pUART_PC->ClearReceptionBuffer();
	
			m_bluetoothProtocol->clearTx();
			m_bluetoothProtocol->clearRx();
			m_bluetoothProtocol->CLOSE();
		}
		switchOff++;
	}
}

void SAU7000_Manager::IR_remoteManager()
{

	static int key = IRk_none;
	
	if(m_IR_Remote == NULL)
		return;

	m_IR_Remote->manager();

	if (key == IRk_none)
	{
		if(!m_IR_Remote->pop(key))
		{
			key = IRk_none;
		}
	}

	if(key != IRk_none)
	{
		switch(key)
		{
			case IRk_27: // stop WTP
				stopWtp();
				break;
				
			case IRk_34: // stop WTH
				stopWth();
				break;
			default:
				break;
		}
		if (m_sauPcProtocol->sendSauPcRemoteKey(key) > 0)
		{
			key = IRk_none;				
		}
	}
}

void SAU7000_Manager::ADuCsNormalMode()
{
	int i;
	
	
	m_ADuCUpgradeManger->setUartDevice((UartDeviceAbstract*)NULL);
	for(i = 0; i <	ADuC_NUM; i++)
	{			
		pUART_ADuC[i]->TurnOff();
		pUART_ADuC[i]->SetBaudRate(UartDeviceAbstract::BR115200);
		pUART_ADuC[i]->ClearTransmissionBuffer();
		pUART_ADuC[i]->ClearReceptionBuffer();
		pUART_ADuC[i]->TurnOn();
	}
	m_ADuCUpgradeManger->hwReset();
}

/**
Checks if messages from the class that manage the communication with the EMG Board are present within the fifo. 
@see uC_Protocol
*/
void SAU7000_Manager::checkSauEmgBoardMessages()
{
	int msg;
			
	msg = m_emgProtocol->getMsg();
	while (msg != SauAndEmgzProtocol::Msg_noMsg)
	{
		switch (msg)
		{
			case SauAndEmgzProtocol::Msg_infoReceived:
				emgBoard_infoReceivedHandler();
				break;
			case SauAndEmgzProtocol::Msg_statusReceived:
				emgBoard_statusReceivedHandler();
				break;
		    case SauAndEmgzProtocol::Msg_acquisitionStarted:
				emgBoard_acquisitionStartedHandler();
				break;
		    case SauAndEmgzProtocol::Msg_acquisitionStopped:
				emgBoard_acquisitionStoppedHandler();
				break;
			case SauAndEmgzProtocol::Msg_emgSamplesReceived:
				emgBoard_emgSamplesReceivedHandler();
				break;
			case SauAndEmgzProtocol::Msg_contactImpSamplesReceived:
				emgBoard_contactImpSamplesReceivedHandler();
				break;
			case SauAndEmgzProtocol::Msg_acknowledge:
				emgBoard_acknowledgeHandler();
				break;
			case SauAndEmgzProtocol::Msg_notAcknowledge:
				emgBoard_notAcknowledgeHandler();
				break;
			case SauAndEmgzProtocol::Msg_errorPacketReceived:
				emgBoard_ErrorPacketReceivedHandler();
				break;
			case SauAndEmgzProtocol::Msg_emgCalReceived:
				emgBoard_emgCalReceivedHandler();
				break;
			case SauAndEmgzProtocol::Msg_impCalReceived:
				emgBoard_impCalReceivedHandler();
				break;
            case SauAndEmgzProtocol::Msg_jumpedToLoader:
				emgBoard_jumpedToLoaderHandler();
				break;
			case SauAndEmgzProtocol::Msg_packetsLostImp:
				emgBoard_packetsLostImp_Handler();
				break;
			case SauAndEmgzProtocol::Msg_packetsLostEmg:
				emgBoard_packetsLostEmg_Handler();
				break;
			case SauAndEmgzProtocol::Msg_timoutRx:
				emgBoard_timoutRxHandler();
				break;
			case SauAndEmgzProtocol::Msg_timoutTx:
				emgBoard_timoutTxHandler();
				break;
			case SauAndEmgzProtocol::Msg_wrongStx:
				emgBoard_wrongStxHandler();
				break;
			case SauAndEmgzProtocol::Msg_opcodeUnknown:
				emgBoard_opcodeUnknownHandler();
				break;
			case SauAndEmgzProtocol::Msg_checkSumError:
				emgBoard_checkSumErrorHandler();
				break;
			case SauAndEmgzProtocol::Msg_HwError:
				emgBoard_HwErrorHandler();
				break;
			case SauAndEmgzProtocol::Msg_transmitBufferOverflowError:
				emgBoard_transmitBufferOverflowErrorHandler();
				break;
			case SauAndEmgzProtocol::Msg_receptionBufferOverflowError:
				emgBoard_receptionBufferOverflowErrorHandler();
				break;
			case SauAndEmgzProtocol::Msg_dataReceivedBufferOverflow:
				emgBoard_dataReceivedBufferOverflowHandler();
				break;
			case SauAndEmgzProtocol::Msg_EmgSamplesBufferOverflow:
				emgBoard_EmgSamplesBufferOverflowHandler();
				break;
			case SauAndEmgzProtocol::Msg_ImpEmgSamplesBufferOverflow:
				emgBoard_ImpEmgSamplesBufferOverflowHandler();
				break;
			case SauAndEmgzProtocol::Msg_unknownError:
				emgBoard_unknownErrorHandler();
				break;
		}
		msg = m_emgProtocol->getMsg();
	}
}

void SAU7000_Manager::checkBluetoothProtocolMessages()
{
	iWRAP_BLUEGIGA_SPP::iWRAP_SSP_msg_struct msg;

	char str[200];
	int len;
	
	if(m_bluetoothProtocol->strMsg(str, len))
	{
		pageInfo->addStr(str);
	}
		
	while (m_bluetoothProtocol->getMsg(msg))
	{
		switch (msg.code)
		{
			case iWRAP_BLUEGIGA_SPP::msg_CONNECTED:
				iWRAP_msg_CONNECTED_Handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_SPP::msg_DISCONNECTED:
				iWRAP_msg_DISCONNECTED_Handler(msg.par, msg.len);
				break;
				
			case iWRAP_BLUEGIGA_SPP::msg_INFO:
				iWRAP_msg_INFO_Handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_SPP::msg_SET_BT_PROFILE:
				iWRAP_msg_SET_BT_PROFILE_Handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_SPP::msg_SET_BT_NAME:
				iWRAP_msg_SET_BT_NAME_Handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_SPP::msg_SET_BT_AUTH:
				iWRAP_msg_SET_BT_AUTH_Handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_SPP::msg_SET_BT_SSP:
				iWRAP_msg_SET_BT_SSP_Handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_SPP::msg_SET_BT_BAUD:
				iWRAP_msg_SET_BAUD_Handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_SPP::msg_SET_CONTROL_MSC:
				iWRAP_msg_SET_CONTROL_MSC_Handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_SPP::msg_RING:
				iWRAP_msg_RING_Handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_SPP::msg_CALL:
				iWRAP_msg_CALL_Handler(msg.par, msg.len);
				break;

			case iWRAP_BLUEGIGA_SPP::msg_moduleNotResponding:
				iWRAP_msg_moduleNotResponding_Handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_SPP::msg_syntaxErrorSyncroError:
				iWRAP_msg_syntaxErrorSyncroError_Handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_SPP::msg_timoutRx:
				iWRAP_msg_timoutRx_Handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_SPP::msg_timoutTx:
				iWRAP_msg_timoutTx_Handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_SPP::msg_HwError:
				iWRAP_msg_HwError_Handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_SPP::msg_transmitBufferOverflowError:
				iWRAP_msg_transmitBufferOverflowError_Handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_SPP::msg_receptionBufferOverflowError:
				iWRAP_msg_receptionBufferOverflowError_Handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_SPP::msg_dataReceivedBufferOverflow:
				iWRAP_msg_dataReceivedBufferOverflow_Handler(msg.par, msg.len);
				break;
			case iWRAP_BLUEGIGA_SPP::msg_unknownError:
				iWRAP_msg_unknownError_Handler(msg.par, msg.len);
				break;
		}
	}
}

void SAU7000_Manager::checkCOMM_status()
{
	if(getSau7000status() == Sau7000status_startupChecks || getSau7000status() == Sau7000status_error 
	   || getSau7000status() == Sau7000status_checkADuCfwUpgrade || getSau7000status() == Sau7000status_wait
		|| getSau7000status() == Sau7000status_ADuCfrequencyCalibration || getSau7000status() == Sau7000status_waitingADuCToStop ||
		getSau7000status() == Sau7000status_switchingOff)
	{
		return ;
	}
	
	if( PinUSB_PWR )	
	{
		if(m_COMM_state != comm_USB)
		{
			//--- stop sampling and all peripherals ---
		
			pageMenu->ChangePage(pMain);
			m_whyAcqWasStopped = AcqStopped_SauCommTypeChanged;

			m_retryRestarComm = false;
			m_numRetriesRestartComm = 0;
			m_timerRetryRestartComm.Stop();
			m_timerResetRetries.Stop();
			
			stop_sampling_and_all_peripherals();

			m_pUART_PROTOCOL_PC = pUART_PC;
			m_sauPcProtocol->setUartDevice(pUART_PC);
			m_sauPcProtocol->setCOMM_type(SauPcProtocol::comm_FTDI_USBtoSERIAL);
			m_bluetoothProtocol->setUartDevice(pUART_BLUE);
			//pUART_BLUE->SetHwFlwCtl(UartDeviceAbstract::hwFlwCtl_None);

			pUART_BLUE->ClearTransmissionBuffer();
			pUART_BLUE->ClearReceptionBuffer();
			pUART_PC->ClearTransmissionBuffer();
			pUART_PC->ClearReceptionBuffer();
				
			if(PinPLUG_BLUE == BT_MODULE_PRESENT)
			{
				m_bluetoothProtocol->CLOSE();
			}
			pMain->setCOMMico(CMenuGeneralInfo::comm_usb, false);
			m_COMM_state = comm_USB;
			m_watchDogTimer.Stop();
		}
	}else
	{	
		if(PinPLUG_BLUE == BT_MODULE_PRESENT)
		{
			if(m_COMM_state == comm_UNKNOWN || m_COMM_state == comm_USB || m_COMM_state == comm_noUSB_noBT)
			{
				m_pUART_PROTOCOL_PC = pUART_PC;
				m_sauPcProtocol->setUartDevice(pUART_PC);
				m_sauPcProtocol->setCOMM_type(SauPcProtocol::comm_FTDI_USBtoSERIAL);
				m_bluetoothProtocol->setUartDevice(pUART_BLUE);

				pUART_BLUE->ClearTransmissionBuffer();
				pUART_BLUE->ClearReceptionBuffer();
				pUART_PC->ClearTransmissionBuffer();
				pUART_PC->ClearReceptionBuffer();
				
				//--- stop sampling and all peripherals....
				m_whyAcqWasStopped = AcqStopped_SauCommTypeChanged;
				
				m_retryRestarComm = false;
				m_numRetriesRestartComm = 0;
				m_timerRetryRestartComm.Stop();
				m_timerResetRetries.Stop();
				
				stop_sampling_and_all_peripherals();	
				
				pMain->setCOMMico(CMenuGeneralInfo::comm_BT, true);

				m_COMM_state = comm_chekingBTconnection;

				m_watchDogTimer.Stop();
			}else if(m_COMM_state == comm_BT_connected)
			{
				m_COMM_state = comm_BT;
				pMain->setCOMMico(CMenuGeneralInfo::comm_BT, false);
				pageMenu->ChangePage(pMain);

				m_pUART_PROTOCOL_PC = pUART_BLUE;
				m_sauPcProtocol->setUartDevice(pUART_BLUE);
				m_sauPcProtocol->setCOMM_type(SauPcProtocol::comm_BT);
				m_bluetoothProtocol->setUartDevice(NULL);

				pUART_BLUE->ClearTransmissionBuffer();
				pUART_BLUE->ClearReceptionBuffer();
				pUART_PC->ClearTransmissionBuffer();
				pUART_PC->ClearReceptionBuffer();

				m_retryRestarComm = false;
				m_numRetriesRestartComm = 0;
				m_timerRetryRestartComm.Stop();
				m_timerResetRetries.Stop();
				
				stop_sampling_and_all_peripherals();	
			}
		}else
		{
			if (m_COMM_state != comm_noUSB_noBT)
			{
				
				pageMenu->ChangePage(pMain);
	
				m_pUART_PROTOCOL_PC = pUART_PC;
				m_sauPcProtocol->setUartDevice(pUART_PC);
				m_sauPcProtocol->setCOMM_type(SauPcProtocol::comm_FTDI_USBtoSERIAL);
				m_bluetoothProtocol->setUartDevice(pUART_BLUE);

				pUART_BLUE->ClearTransmissionBuffer();
				pUART_BLUE->ClearReceptionBuffer();
				pUART_PC->ClearTransmissionBuffer();
				pUART_PC->ClearReceptionBuffer();

				m_whyAcqWasStopped = AcqStopped_SauCommTypeChanged;

				m_retryRestarComm = false;
				m_numRetriesRestartComm = 0;
				m_timerRetryRestartComm.Stop();
				m_timerResetRetries.Stop();
				
				stop_sampling_and_all_peripherals();
				//pUART_BLUE->SetHwFlwCtl(UartDeviceAbstract::hwFlwCtl_None);
				
				pMain->setCOMMico(CMenuGeneralInfo::comm_noUsb, true);
				m_COMM_state = comm_noUSB_noBT;

				m_watchDogTimer.Stop();
			}
		}
	}
}
		
void SAU7000_Manager::checkChannelPluggedToGenericChan()
{
	m_connectedToGEN_CHAN_1 = ID_GEN1; 
	m_connectedToGEN_CHAN_2 = ID_GEN2;
}


/**
Sets to 0xFFFF all the prs samples into the m_prsSamples array
*/
void SAU7000_Manager::rstPrsSamples()
{
	int i;
	for(i = 0; i < _NUM_PRS_SAMPLES_PER_PACKET_; i++)
		m_prsSamples[i] = 0xFFFF;
}

/**
Sets to 0xFFFFFF all the weight samples into the m_weightSamples array
*/
void SAU7000_Manager::rstWeightSamples()
{
	int i;
	for(i = 0; i < _NUM_WEIGHT_SAMPLES_PER_PACKET_; i++)
		m_weightSamples[i] = 0xFFFFFF;

}

		

/**
@return true is all the ADuCs are in the sample status 
@see ADuCStatus
*/
bool SAU7000_Manager::checkADuCStatus(byte status)
{
	if (m_ADuCStatus[ADuC_VINF] == status)
		if (m_ADuCStatus[ADuC_VV] == status)
			if (m_ADuCStatus[ADuC_P12] == status)
				if (m_ADuCStatus[ADuC_P34] == status)
					if (m_ADuCStatus[ADuC_P56] == status)
						if (m_ADuCStatus[ADuC_P78] == status)
							if (m_ADuCStatus[ADuC_GEN] == status)
								return true;
								
	return false;
}

/**
sends the start command to all the ADuCs
*/
void SAU7000_Manager::startADuCs()
{
	int i;
	for (i = ADuC_VINF; i < ADuC_NUM; i++ )
	{
		m_ADuC_Protocols[i]->sendStartAcquisition();
		m_ADuCacqCheckTimer[i].Preset(500);
	}

	m_timerWaitAduCResponse.Preset(_TIME_WAIT_ADuC_RESP_DEFAULT);
	
	m_ADuCsWantedStatus = ADuCStatus_acquiring;
}

/**
sends the setting command to all the ADuCs
*/
void SAU7000_Manager::setADuCsAquisitionParameters()
{
	int i;
	for (i = ADuC_VINF; i < ADuC_NUM; i++ )
	{
		m_ADuC_Protocols[i]->sendSetting();
	}
	
	m_timerWaitAduCResponse.Preset(_TIME_WAIT_ADuC_RESP_DEFAULT);
	
	m_ADuCsWantedStatus = ADuCStatus_set;
}

/**
sends the stop command to all the ADuCs
*/
void SAU7000_Manager::stopADuCs()
{
	int i;
	for (i = ADuC_VINF; i < ADuC_NUM; i++ )
	{
		m_ADuC_Protocols[i]->sendStopAcquisition();
		m_ADuCacqCheckTimer[i].Stop();
	}

	m_timerWaitAduCResponse.Preset(_TIME_WAIT_ADuC_RESP_DEFAULT);
	
	m_ADuCsWantedStatus = ADuCStatus_idle;
}		

/**
sends the get status command to all the ADuCs
*/
void SAU7000_Manager::getADuCsStatus()
{
	int i;
	for (i = ADuC_VINF; i < ADuC_NUM; i++ )
	{
		m_ADuC_Protocols[i]->sendRequest_Status();
		//m_ADuC_Protocols[i]->sendRequest_Info();
	}

	//m_timerWaitAduCResponse.Preset(500);
}

void SAU7000_Manager::getADuCsInfo()
{
	int i;
	for (i = ADuC_VINF; i < ADuC_NUM; i++ )
	{
		//m_ADuC_Protocols[i]->sendRequest_Status();
		m_ADuC_Protocols[i]->sendRequest_Info();
	}

	//m_timerWaitAduCResponse.Preset(500);
}

/**
Checks if messages from the class that manage the communication with the ADuCs are present within the fifo. 
@see uC_Protocol
*/
void SAU7000_Manager::checkSauAduCsMessages()
{
	int i, msg;

	for (i = 0; i < ADuC_NUM ; i++)
	{
		msg = m_ADuC_Protocols[i]->getMsg();
		while(msg != uC_Protocol::msg_noMsg)
		{
			switch (msg)
			{
				case uC_Protocol::msg_infoReceived:
					ADuC_infoReceivedHandler(i);
					break;
				case uC_Protocol::msg_statusReceived:
					ADuC_statusReceivedHandler(i);
					break;
			    case uC_Protocol::msg_acquisitionStarted:
					ADuC_acquisitionStartedHandler(i);
					break;
			    case uC_Protocol::msg_acquisitionStopped:
					ADuC_acquisitionStoppedHandler(i);
					break;
				case uC_Protocol::msg_packet01_received:
					ADuC_packet01_receivedHandler(i);
					break;
				case uC_Protocol::msg_packet02_received:
					ADuC_packet02_receivedHandler(i);
					break;
				case uC_Protocol::msg_packet03_received:
					ADuC_packet03_receivedHandler(i);
					break;
				case uC_Protocol::msg_received_SETTING_ADC:
					ADuC_received_SETTING_ADCHandler(i);
					break;
				case uC_Protocol::msg_received_SETTING_IEXC:
					ADuC_msg_received_SETTING_IEXCHandler(i);
					break;
				case uC_Protocol::msg_received_SETTING_DAC:
					ADuC_received_SETTING_DACHandler(i);
					break;
				case uC_Protocol::msg_received_SETTING_PWM:
					ADuC_received_SETTING_PWMHandler(i);
					break;
				case uC_Protocol::msg_ALL_Setting_ACK:
					ADuC_ALL_Setting_ACKHandler(i);
					break;
				case uC_Protocol::msg_acquisitionNOTstarted:
					ADuC_acquisitionNOTstartedHandler(i);
					break;
				case uC_Protocol::msg_ErrorPacketReceived:
					ADuC_ErrorPacketReceivedHandler(i);
					break;
				case uC_Protocol::msg_messageSentAck:
					ADuC_messageAckPacketReceivedHandler(i);
					break;
				case uC_Protocol::msg_messageSentNotAck:
					ADuC_messageNotAckPacketReceivedHandler(i);
					break;
				case uC_Protocol::msg_packetsLost_01:
					ADuC_packetsLost_01Handler(i);
					break;
				case uC_Protocol::msg_packetsLost_02:
					ADuC_packetsLost_02Handler(i);
					break;
				case uC_Protocol::msg_packetsLost_03:
					ADuC_packetsLost_03Handler(i);
					break;
				case uC_Protocol::msg_timoutRx:
					ADuC_timoutRxHandler(i);
					break;
				case uC_Protocol::msg_timoutTx:
					ADuC_timoutTxHandler(i);
					break;
				case uC_Protocol::msg_wrongStx:
					ADuC_wrongStxHandler(i);
					break;
				case uC_Protocol::msg_opcodeUnknown:
					ADuC_opcodeUnknownHandler(i);
					break;
				case uC_Protocol::msg_checkSumError:
					ADuC_checkSumErrorHandler(i);
					break;
				case uC_Protocol::msg_HwError:
					ADuC_HwErrorHandler(i);
					break;
				case uC_Protocol::msg_transmitBufferOverflowError:
					ADuC_transmitBufferOverflowErrorHandler(i);
					break;
				case uC_Protocol::msg_receptionBufferOverflowError:
					ADuC_receptionBufferOverflowErrorHandler(i);
					break;
				case uC_Protocol::msg_dataReceivedBufferOverflow:
					ADuC_dataReceivedBufferOverflowHandler(i);
					break;
				case uC_Protocol::msg_weightChanBufferOverflow:
					ADuC_weightChanBufferOverflowHandler(i);
					break;
				case uC_Protocol::msg_prsChanBufferOverflow:
					ADuC_prsChanBufferOverflowHandler(i);
					break;
				case uC_Protocol::msg_genChanBufferOverflow:
					ADuC_genChanBufferOverflowHandler(i);
					break;
				case uC_Protocol::msg_unknownError:
					ADuC_unknownErrorHandler(i);
					break;
			}
			msg = m_ADuC_Protocols[i]->getMsg();
		}
	}

}

/**
Checks if messages from the class that manage the communication with the PC are present within the fifo. 
@see SauPcProtocol
*/
void SAU7000_Manager::checkSauPcProtocolMessages()
{
	int msg;
	
	msg = m_sauPcProtocol->getMsg();
	while(msg != SauPcProtocol::SauPcMsg_noMsg)
	{
		switch (msg)
		{
			case SauPcProtocol::SauPcMsg_noMsg:
				break;
			case SauPcProtocol::SauPcMsg_infoAsked:
				SauPcMsg_infoAskedHandler();
				break;
			case SauPcProtocol::SauPcMsg_statusAsked:
				SauPcMsg_statusAskedHandler();
				break;
			case SauPcProtocol::SauPcMsg_startAcquisition:
				SauPcMsg_startAcquisitionHandler();
				break;
			case SauPcProtocol::SauPcMsg_stopAcquisition:
				SauPcMsg_stopAcquisitionHandler(AcqStopped_pcCommand);
				break;
			case SauPcProtocol::SauPcMsg_wtpSpeed:
				SauPcMsg_wtpSpeedHandler();
				break;
			case SauPcProtocol::SauPcMsg_wthSpeed:
				SauPcMsg_wthSpeedHandler();
				break;
			case SauPcProtocol::SauPcMsg_wtpStatus:
				SauPcMsg_wtpStatusHandler();
				break;
			case SauPcProtocol::SauPcMsg_wthStatus:
				SauPcMsg_wthStatusHandler();
				break;
			case SauPcProtocol::SauPcMsg_setWtpSpeedCalibration:
				SauPcMsg_setWtpSpeedCalibrationHandler();
				break;
			case SauPcProtocol::SauPcMsg_bubbleStatus:
				SauPcMsg_bubbleStatusHandler();
				break;
			case SauPcProtocol::SauPcMsg_jumpToLoader:
				SauPcMsg_jumpToLoaderHandler();
				break;
			case SauPcProtocol::SauPcMsg_emgz_jumpToLoader:
				SauPcMsg_emgz_jumpToLoaderHandler();
				break;
			case SauPcProtocol::SauPcMsg_WeightCalAsked:	
				SauPcMsg_WeightCalAskedHandler();
				break;
			case SauPcProtocol::SauPcMsg_PrsCalAsked:
				SauPcMsg_PrsCalAskedHandler();
				break;
			case SauPcProtocol::SauPcMsg_GenericCalAsked:
				SauPcMsg_GenericCalAskedHandler();
				break;
			case SauPcProtocol::SauPcMsg_EmgCalAsked:
				SauPcMsg_EmgCalAskedHandler();
				break;
			case SauPcProtocol::SauPcMsg_WeightCalSet:
				SauPcMsg_WeightCalSetHandler();
				break;
			case SauPcProtocol::SauPcMsg_Prs_CalSet:
				SauPcMsg_Prs_CalSetHandler();
				break;
			case SauPcProtocol::SauPcMsg_EmgCalSet:
				SauPcMsg_EmgCalSetHandler();
				break;
			case SauPcProtocol::SauPcMsg_Generic_CalSet:
				SauPcMsg_Generic_CalSetHandler();
				break;
			case SauPcProtocol::SauPcMsg_zeroChan:
				SauPcMsg_zeroChanHandler();
				break;
			case SauPcProtocol::SauPcMsg_setDisplay:
				SauPcMsg_setDisplayHandler();
				break;
			case SauPcProtocol::SauPcMsg_getDisplay:
				SauPcMsg_getDisplayHandler();
				break;
			case SauPcProtocol::SauPcMsg_emgzContacImp:
				SauPcMsg_emgzContactImpHandler();
				break;
			case SauPcProtocol::SauPcMsg_emgzSetFiltering:
				SauPcMsg_emgzSetFilteringHandler();
				break;
			case SauPcProtocol::SauPcMsg_BT_bypassMode:
				SauPcMsg_BT_bypassModeHandler();
				break;
			//--------------
			case SauPcProtocol::SauPcMsg_packetsLost:
				SauPcMsg_packetsLostHandler();
				break;
			case SauPcProtocol::SauPcMsg_timoutRx:
				SauPcMsg_timoutRxHandler();
				break;
			case SauPcProtocol::SauPcMsg_timoutTx:
				SauPcMsg_timoutTxHandler();
				break;
			case SauPcProtocol::SauPcMsg_wrongStx:
				SauPcMsg_wrongStxHandler();
				break;
			case SauPcProtocol::SauPcMsg_opcodeUnknown:
				SauPcMsg_opcodeUnknownHandler();
				break;
			case SauPcProtocol::SauPcMsg_checkSumError:
				SauPcMsg_checkSumErrorHandler();
				break;
			case SauPcProtocol::SauPcMsg_HwError:
				SauPcMsg_HwErrorHandler();
				break;
			case SauPcProtocol::SauPcMsg_transmitBufferOverflowError:
				SauPcMsg_transmitBufferOverflowErrorHandler();
				break;
			case SauPcProtocol::SauPcMsg_receptionBufferOverflowError:
				SauPcMsg_receptionBufferOverflowErrorHandler();
				break;
			case SauPcProtocol::SauPcMsg_dataReceivedBufferOverflow:
				SauPcMsg_dataReceivedBufferOverflowHandler();
				break;
			case SauPcProtocol::SauPcMsg_unknownError:
				SauPcMsg_unknownErrorHandler();
				break;
			case SauPcProtocol::SauPcMsg_BTcommunicationDropped:
				SauPcMsg_BTcommunicationDroppedHandler();
				break;
			case SauPcProtocol::SauPcMsg_watchdogSetting:
				SauPcMsg_watchdogSettingHandler();
				break;
			case SauPcProtocol::SauPcMsg_setMotorCal:
				SauPcMsg_setMotorCalHandler();
				break;
			case SauPcProtocol::SauPcMsg_getMotorCal:
				SauPcMsg_getMotorCalHandler();
				break;
			case SauPcProtocol::SauPcMsg_setOverpressure:
				SauPcMsg_setOverpressureHandler();
				break;
			default:
				break;
		}
		msg = m_sauPcProtocol->getMsg();
	}
}
/**
Manager of the command board information asked.

The command is received and decoded from the class SauPcProtocol which sends a message to the board manager.
@return always 1
@see SauPcProtocol::sendSauPcInfo
*/
int SAU7000_Manager::SauPcMsg_infoAskedHandler()
{
	m_sauPcProtocol->sendSauPcInfo(m_AduC_firmwareVersion, ADuC_NUM, m_fpgaBitstream, m_emgProtocol->getAppRevision(), m_emgProtocol->getAppDate(), m_emgProtocol->getHwRevision());
	return 1;
}

/**
Manager of the command board status asked.

The command is received and decoded from the class SauPcProtocol which sends a message to the board manager.
@return always 1
@see SauPcProtocol::sendSauPcStatus
*/
int SAU7000_Manager::SauPcMsg_statusAskedHandler()
{
	byte status[120];
	int i;
	int k;
	i = 0;

	m_getStatusReceived = true;
	
	m_watchDogTimer.Preset(m_watchdogTime);
	if (getSau7000status() == Sau7000status_error && m_sauError == SauError_watchdogError)
	{
		m_sauError = SauError_noError;
		changeSau7000status(Sau7000status_idle);
		
		pageMenu->ChangePage(pMain);
		pageMessages->setMessage(0, " ");
		pageMessages->setMessage(1, " ");
		pageMessages->setMessage(2, " ");
		pageMessages->setMessage(3, " ");
	}
	
	//--- Current consumption ---
	//m_lastCurrentValueSampled = 500;
	*((word*)&status[i]) = m_lastCurrentValueSampled; 
	i++;
	i++;
	
	//--- Voltage ---
	//m_lastVoltageValueSampled = 900;
	for (k = 0; k < _NUM_VOLTAGE_MEASURE_; k++)
	{
		*((word*)&status[i]) = m_lastVoltageValueSampled[k]; 
		i++;
		i++;
	}

	//--- Sau7000 status ----
	status[i] = getSau7000status();
	i++;

	//--- Sau7000 number of errors occured ----
	*((word*)&status[i]) = m_numOfErrorsOccured; 
	i++;
	i++;

	//--- ADuC status ---
	for (k = 0; k < ADuC_NUM; k++)
	{
		status[i] = m_ADuCStatus[k];
		i++;
	}

	//---- HR EMG Board Status ---
	status[i] = m_emgzBoardStatus;
	i++;

	//--- Wtp  status -----
	status[i] = ((m_wtpStatus << 4) & 0xF0) +  (m_wtpLitStatus & 0x0F);
	i++;

	//--- Wtp speed ------------
	*((word*)&status[i]) = m_wtpSpeed; 
	i++;
	i++;

	//--- Wth status -----
	status[i] = m_wthPowered;
	i++;
	status[i] = ((m_wthStatus << 4) & 0xF0) +  (m_wthEndSwitchesStatus & 0x0F);
	i++;
	
	//--- Wth speed ------------
	*((word*)&status[i]) = m_wthSpeed; 
	i++;
	i++;
	
	//--- Wth step ------------
	*((word*)&status[i]) = m_wthStep; 
	i++;
	i++;

	//--- Bubble Status ---
	m_bubbleClampStatus = (PinCMD_PINZA == 0) ? BubbleClampState_open : BubbleClampState_close;
	m_bubbleAirStatus =   (PinCMP_BUBBLE == 0)	? BubbleAirState_AirPresent : BubbleAirState_NoAir;
	
	status[i] = ((m_bubbleAirStatus << 6) & 0xC0) | ((m_bubbleClampStatus << 4 )& 0x30)| 
				((m_bubbleDetectionEn ? 1:0)<< 3) | ((m_bubbleAirTestFailed ? 1:0)<< 2) | 
				((m_bubbleAirDetected ? 1:0)<< 1) | (m_bubbleAirTestRunning ? 1:0);
	i++;

	for (k = 0; k < SauAndEmgzProtocol::_NUM_VOLTAGE_MEASURE_EMGZ_; k++)
	{
		*((word*)(&status[i])) = m_emgzBoardVoltageMeasure[k] ;
		i += 2;
	}

	*((word*)(&status[i])) = m_emgzBoardCurrentMeasure;
	i += 2;

	status[i] = m_emgzBoardOverCurrentError;
	if(m_emgProtocol->sampleContactImpEnable())
	{
		status[i] += 0x10;
	}
	
	i++;
	
	*((word*)(&status[i])) = m_emgzBoardNumErrorOccurred;
	i+= 2;

	status[i] = m_watchdogEnabled ? 1 : 0;
	i++;
	status[i] = (m_watchdogTime >> 8) & 0x00FF;
	i++;
	status[i] = m_watchdogTime & 0x00FF;
	i++;
	
	status[i] = ((m_connectedToGEN_CHAN_2 & 0x0F) << 4)+ (m_connectedToGEN_CHAN_1 & 0x0F);
	i++;

	status[i] = m_emgzBoardLPF_type;
	i++;
	status[i] = m_emgzBoardNOTCHF_type;
	i++;
	status[i] = m_emgzBoardHPF_type;
	i++;
	status[i] = m_whyAcqWasStopped;
	i++;

	//-- ovepressure info
	*((short*)(&status[i])) = m_overpressureLimit;
	i+=2;
	*((overpressure_bitF_t*)(&status[i])) = m_overpressureChanEn;
	i+=2;
	*((overpressure_bitF_t*)(&status[i])) = m_overpressureChan;
	i+=2;
	
	m_sauPcProtocol->sendSauPcStatus(status, i);
	return 1;
}

/**
Manager of the command start acquisition.

The command is received and decoded from the class SauPcProtocol which sends a message to the board manager.
@return always 1

@see SauPcProtocol::sendSauPcAcquisitionStarted
*/
int SAU7000_Manager::SauPcMsg_startAcquisitionHandler()
{
	if(getSau7000status() == Sau7000status_startupChecks || getSau7000status() == Sau7000status_checkADuCfwUpgrade || 
		getSau7000status() == Sau7000status_error || getSau7000status() == Sau7000status_wait || !m_getStatusReceived
		|| getSau7000status() == Sau7000status_ADuCfrequencyCalibration || getSau7000status() != Sau7000status_idle
		|| getSau7000status() == Sau7000status_switchingOff)
	{
		m_sauPcProtocol->sendAcknowledge(0);
		return 0;
	}
	
	changeSau7000status(Sau7000status_sendingSettings);
	
	setADuCsAquisitionParameters();
	
	m_weightChan->clearFifo();
	m_prsChan->clearFifo();	
	m_timerCheckChanSyncro.Preset(100);
	
	m_sauPcProtocol->rstGenPacketsSent();
	m_sauPcProtocol->rstEmgPacketsSent();
	m_sauPcProtocol->rstPrsPacketsSent();
	m_sauPcProtocol->rstWeightPacketsSent();
	
	m_emgProtocol->rstEmgPacketsReceived();
	m_emgProtocol->rstImpPacketsReceived();

	m_timerCheckADuCStatusWhileSampling.Preset(500);
	m_timerCheckADuCStatus.Stop();
	m_timerCheckEmgBoardStatus.Preset(_TIME_CHECK_EMGZ_STATUS_);

	m_blinkLed = 0;
	m_blinkLedValue = __LED_BLINK_VALUE_ACQ__; 

	m_retryRestarComm = true;
	//m_numRetriesRestartComm = 0;
	m_timerRetryRestartComm.Stop();
	m_timerResetRetries.Stop();
	
	return 1;
}

/**
Manager of the command stop acquisition.

The command is received and decoded from the class SauPcProtocol which sends a message to the board manager.

@param cause the cause that stopped the acquisition.
@return always 1
@see SauPcProtocol::sendSauPcAcquisitionStopped
*/
int SAU7000_Manager::SauPcMsg_stopAcquisitionHandler(byte cause)
{

	if(getSau7000status() == Sau7000status_startupChecks || getSau7000status() == Sau7000status_checkADuCfwUpgrade || 
		getSau7000status() == Sau7000status_error || getSau7000status() == Sau7000status_wait || !m_getStatusReceived
		|| getSau7000status() == Sau7000status_ADuCfrequencyCalibration || getSau7000status() == Sau7000status_switchingOff)
	{
		m_sauPcProtocol->sendAcknowledge(0);
		return 0;
	}

	m_whyAcqWasStopped = AcqStopped_pcCommand;
	
	changeSau7000status(Sau7000status_waitingADuCToStop);
	
	stopADuCs();

	if(m_emgzBoardPresent)
		m_emgProtocol->sendStopAcq();
	
	//m_sauPcProtocol->sendSauPcAcquisitionStopped(cause);
	m_timerCheckADuCStatusWhileSampling.Stop();
	
	m_timerCheckADuCStatus.Preset(_TIME_CHECK_ADuC_STATUS_);
	m_timerCheckEmgBoardStatus.Preset(_TIME_CHECK_EMGZ_STATUS_);

	m_blinkLed = 0;
	m_blinkLedValue = __LED_BLINK_VALUE_IDLE__; 

	m_retryRestarComm = false;
	//m_numRetriesRestartComm = 0;
	m_timerRetryRestartComm.Stop();
	m_timerResetRetries.Stop();
	
	return 1;
}

/**
Manager of the command set wtp speed.

The command is received and decoded from the class SauPcProtocol which sends a message to the board manager.
@return always 1

@see SauPcProtocol::sendSauPcWtp
*/
int SAU7000_Manager::SauPcMsg_wtpSpeedHandler()
{
	if(getSau7000status() == Sau7000status_startupChecks || getSau7000status() == Sau7000status_checkADuCfwUpgrade || 
		getSau7000status() == Sau7000status_error || getSau7000status() == Sau7000status_wait || !m_getStatusReceived
		|| getSau7000status() == Sau7000status_ADuCfrequencyCalibration || getSau7000status() == Sau7000status_switchingOff
		|| (*((short*)(&m_overpressureChan)) != 0 && *((short*)(&m_overpressureChanEn))) )
	{
		m_sauPcProtocol->sendSauPcWtp(m_wtpSpeed, m_wtpStatus, m_wtpLitStatus, m_wtp->getDriverStatus());
		m_sauPcProtocol->sendAcknowledge(0);
		return 0;
	}
	
	m_wtpSpeed = m_sauPcProtocol->lastWtpSpeedReceived();

	// set the wtp speed and status....
	// ................
	// ................
	m_wtpSpeed = m_wtp->setVelocity(m_wtpSpeed);
	pMain->setWTPspeed(m_wtpSpeed);
	
	m_sauPcProtocol->sendSauPcWtp(m_wtpSpeed, m_wtpStatus, m_wtpLitStatus, m_wtp->getDriverStatus());
	return 1;
}

int SAU7000_Manager::SauPcMsg_setWtpSpeedCalibrationHandler()
{
	if(getSau7000status() == Sau7000status_startupChecks || getSau7000status() == Sau7000status_checkADuCfwUpgrade || 
		getSau7000status() == Sau7000status_error || getSau7000status() == Sau7000status_wait || !m_getStatusReceived
		|| getSau7000status() == Sau7000status_ADuCfrequencyCalibration || getSau7000status() == Sau7000status_switchingOff
		|| (*((short*)(&m_overpressureChan)) != 0 && *((short*)(&m_overpressureChanEn))) )
	{
		m_sauPcProtocol->sendWtpSpeedCalibration(0, 1);
		m_sauPcProtocol->sendAcknowledge(0);
		return 0;
	}

	pMain->setWTPspeed(m_sauPcProtocol->getWtpSpeedCalibration()/1000);
	m_wtp->setVelocityPulse(m_sauPcProtocol->getWtpSpeedCalibration());
	m_sauPcProtocol->sendWtpSpeedCalibration(m_wtp->getTargetPulse(), 0);
	return 1;
}

/**
Manager of the command set wth speed.

The command is received and decoded from the class SauPcProtocol which sends a message to the board manager.
@return always 1

@see SauPcProtocol::sendSauPcWth

*/
int SAU7000_Manager::SauPcMsg_wthSpeedHandler()
{
	if(getSau7000status() == Sau7000status_startupChecks || getSau7000status() == Sau7000status_checkADuCfwUpgrade || 
		getSau7000status() == Sau7000status_error || getSau7000status() == Sau7000status_wait || !m_getStatusReceived
		|| getSau7000status() == Sau7000status_ADuCfrequencyCalibration || getSau7000status() == Sau7000status_switchingOff
		|| (*((short*)(&m_overpressureChan)) != 0 && *((short*)(&m_overpressureChanEn))) || m_bubbleAirDetected || m_bubbleAirTestFailed)
	{
		m_sauPcProtocol->sendSauPcWth(m_wthPowered, m_wthSpeed, m_wthStatus, m_wthEndSwitchesStatus, m_wthStep);
		m_sauPcProtocol->sendAcknowledge(0);
		return 0;
	}

	// set the wth speed and status....
	// ................
	// ................
	m_wthSpeed = m_wth->setLinearVelocity((double) m_sauPcProtocol->lastWthSpeedReceived());
	pMain->setWTHspeed(m_wthSpeed);
	
	m_sauPcProtocol->sendSauPcWth(m_wthPowered, m_wthSpeed, m_wthStatus, m_wthEndSwitchesStatus, m_wthStep);
	return 1;
}

/**
Manager of the command set wtp status.

The command is received and decoded from the class SauPcProtocol which sends a message to the board manager.
@return always 1

@see SauPcProtocol::sendSauPcWtp
*/
int SAU7000_Manager::SauPcMsg_wtpStatusHandler()
{
	if(getSau7000status() == Sau7000status_startupChecks || getSau7000status() == Sau7000status_checkADuCfwUpgrade || 
		getSau7000status() == Sau7000status_error || getSau7000status() == Sau7000status_wait || !m_getStatusReceived
		|| getSau7000status() == Sau7000status_ADuCfrequencyCalibration || getSau7000status() == Sau7000status_switchingOff
		|| (*((short*)(&m_overpressureChan)) != 0 && *((short*)(&m_overpressureChanEn))) || m_bubbleAirDetected || m_bubbleAirTestFailed)
	{
		m_sauPcProtocol->sendSauPcWtp(m_wtpSpeed, m_wtpStatus, m_wtpLitStatus, m_wtp->getDriverStatus());
		m_sauPcProtocol->sendAcknowledge(0);
		return 0;
	}

	switch((WtpStatus) m_sauPcProtocol->lastWtpStatusReceived())
	{
		case WtpState_stop:			/**< WTP is not running*/
			m_wtpStatus = WtpState_stop;
			break;
		case WtpState_running:		/**< WTP is running*/
			m_wtpStatus = WtpState_running;
			break;
		case WtpState_error: 		/**< error*/
		case WtpState_unknown:
		default:
			m_wtpStatus = WtpState_stop;
			break;

	}
	// set the wtp  status....
	// ................
	// ................
	if(m_wtp->isCalibrate())
	{
		if (m_wtpStatus == WtpState_running )
		{
			if(!pMain->getWTPisMoving())
				pMain->setWTPisMoving(true);

			m_wtp->setEnable(true);
		}else
		{
			if(pMain->getWTPisMoving())
				pMain->setWTPisMoving(false);

			m_wtp->setEnable(false);
		}
	}else
	{
		m_wtpStatus = WtpState_error;
	}
	m_sauPcProtocol->sendSauPcWtp(m_wtpSpeed, m_wtpStatus, m_wtpLitStatus, m_wtp->getDriverStatus());
	return 1;
}

/**
Manager of the command set wth status.

The command is received and decoded from the class SauPcProtocol which sends a message to the board manager.
@return always 1

@see SauPcProtocol::sendSauPcWth

*/
int SAU7000_Manager::SauPcMsg_wthStatusHandler()
{
	if(getSau7000status() == Sau7000status_startupChecks || getSau7000status() == Sau7000status_checkADuCfwUpgrade || 
		getSau7000status() == Sau7000status_error || getSau7000status() == Sau7000status_wait || !m_getStatusReceived
		|| getSau7000status() == Sau7000status_ADuCfrequencyCalibration || getSau7000status() == Sau7000status_switchingOff
		|| (*((short*)(&m_overpressureChan)) != 0 && *((short*)(&m_overpressureChanEn))) || m_bubbleAirDetected || m_bubbleAirTestFailed)
	{
		m_sauPcProtocol->sendSauPcWth(m_wthPowered, m_wthSpeed, m_wthStatus, m_wthEndSwitchesStatus, m_wthStep);
		m_sauPcProtocol->sendAcknowledge(0);
		return 0;
	}

	// set the wth step and status....
	// ................
	// ................
	m_wthPowered = m_sauPcProtocol->lastWthPowered();
	
	switch((WthStatus) m_sauPcProtocol->lastWthStatusReceived())
	{
		
		case WthState_stop:			/**< WTH is not running*/
			m_wthStatus = WthState_stop;
			break;
		case WthState_running:		/**< WTH is running - extracting*/
			m_wthStatus = WthState_running;
			break;
		case WthState_unknown:	/**< WTH unknow status*/
		case WthState_error:
		default:
			m_wthStatus = WthState_stop;
			break;
	}
	m_wthStep = m_sauPcProtocol->lastWthStepReceived();
	
	if (m_wthPowered == 1)
	{
		m_wth->power(true);
	}
	else
	{
		m_wthStatus = WthState_stop;
		m_wthEndSwitchesStatus = WthEndSwitchesStatus_unknown;
		pMain->setLimitSwitch(CMenuMain::lmtsw_notReached, CMenuMain::lmtsw_notReached);
		m_wth->power(false);
	}
	
	if(m_wthStatus == WthState_running)
		m_wth->setEnable(true);
	else
		m_wth->setEnable(false);

	pMain->setWTHisMoving(m_wth->isEnable());
	
	m_wth->setLinearVelocity((double)m_wthSpeed);
	m_wth->setStep((double)m_wthStep);
	
	m_sauPcProtocol->sendSauPcWth(m_wthPowered, m_wthSpeed, m_wthStatus, m_wthEndSwitchesStatus, m_wthStep);
	return 1;
}

/**
Manager of the command set bubble status.

The command is received and decoded from the class SauPcProtocol which sends a message to the board manager.
@return always 1

@see SauPcProtocol::sendSauPcBubble

*/
int SAU7000_Manager::SauPcMsg_bubbleStatusHandler()
{
	if(getSau7000status() == Sau7000status_startupChecks || getSau7000status() == Sau7000status_checkADuCfwUpgrade || 
		getSau7000status() == Sau7000status_error || getSau7000status() == Sau7000status_wait || !m_getStatusReceived
		|| getSau7000status() == Sau7000status_ADuCfrequencyCalibration || getSau7000status() == Sau7000status_switchingOff)
	{
		m_sauPcProtocol->sendAcknowledge(0);
		return 0;
	}

	m_bubbleDetectionEn = m_sauPcProtocol->bubbleDetectionEn();
	if(!m_bubbleDetectionEn)
	{
		m_bubbleAirDetected = false;
		m_bubbleAirTestFailed = false;
		m_bubbleAirTestRunning = false;
		m_bubbleAirTestTimer.Stop();
		PinTXE_BUBBLE = 1; //disable test
		
		switch(m_sauPcProtocol->lastBubbleStatusReceived())
		{
			case BubbleClampState_unknown:
				break;
			case BubbleClampState_open:
				PinCMD_PINZA = 0;
				break;
			case BubbleClampState_close:
				PinCMD_PINZA = 1;
				break;
		}
	}else
	{
		if(!m_bubbleAirTestRunning)
		{
			if((PinCMP_BUBBLE == 1) && !m_bubbleAirDetected && !m_bubbleAirTestFailed)
			{
				PinTXE_BUBBLE = 1;
				PinCMD_PINZA = 0;
				m_bubbleAirTestTimer.Reset();
			}else
			{
				PinCMD_PINZA = 1;  // close
			}
		}
	}
	m_bubbleClampStatus = (PinCMD_PINZA == 0) ? BubbleClampState_open : BubbleClampState_close;
	m_bubbleAirStatus =   (PinCMP_BUBBLE == 0)	? BubbleAirState_AirPresent : BubbleAirState_NoAir;
	m_sauPcProtocol->sendSauPcBubble(m_bubbleAirStatus, m_bubbleClampStatus, m_bubbleDetectionEn, m_bubbleAirTestFailed, m_bubbleAirDetected, m_bubbleAirTestRunning);
	return 1;
}

/**
Manager of the command board jump to loader.

The command is received and decoded from the class SauPcProtocol which sends a message to the board manager.
@return always 1
*/
int SAU7000_Manager::SauPcMsg_jumpToLoaderHandler()
{
	DecTimer wait;

	if( m_COMM_state == comm_BT ||
		m_COMM_state == comm_chekingBTconnection ||
		m_COMM_state == comm_BT_connected ||
		m_COMM_state == comm_BTconnectionError)
	{
		m_sauPcProtocol->sendAcknowledge(0);
		return 0;
	}
	
	
	
	// sent stop command to ADuC microcontroller and emg board and stop the motors
	//.....
	//.....
	m_whyAcqWasStopped = AcqStopped_jumpingToLoader;

	m_retryRestarComm = false;
	m_numRetriesRestartComm = 0;
	m_timerRetryRestartComm.Stop();
	m_timerResetRetries.Stop();
	
	stop_sampling_and_all_peripherals();
	
	m_timerCheckADuCStatusWhileSampling.Stop();
	
	m_sauPcProtocol->rstGenPacketsSent();
	m_sauPcProtocol->rstEmgPacketsSent();
	m_sauPcProtocol->rstPrsPacketsSent();
	m_sauPcProtocol->rstWeightPacketsSent();
	
	m_timerCheckADuCStatus.Preset(_TIME_CHECK_ADuC_STATUS_);
	
	m_sauPcProtocol->sendSauPcJumpedToLoader();

	wait.Preset(500);
	while(!wait.Match());

	changeSau7000status(Sau7000status_idle);
	
	while (m_sauPcProtocol->GetNumBytesInTransmissionBuffer() > 0)
	{
		wait.Preset(100);
		while(!wait.Match());
	}		
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	fp = (void (*)()) LOADER_FLASH_STAY;				// inizio dell'applicone
    (*fp)();	
	return 1;
}

int SAU7000_Manager::SauPcMsg_emgz_jumpToLoaderHandler()
{
	DecTimer wait;

	if(getSau7000status() == Sau7000status_startupChecks || getSau7000status() == Sau7000status_checkADuCfwUpgrade || 
		/*getSau7000status() == Sau7000status_error ||*/ getSau7000status() == Sau7000status_wait || m_COMM_state != comm_USB || !m_getStatusReceived
		|| getSau7000status() == Sau7000status_ADuCfrequencyCalibration || getSau7000status() == Sau7000status_switchingOff)
	{
		m_sauPcProtocol->sendAcknowledge(0);
		return 0;
	}
	
	
	
	// sent stop command to ADuC microcontroller and emg board and stop the motors
	//.....
	//.....
	m_whyAcqWasStopped = AcqStopped_jumpingToLoader;

	m_retryRestarComm = false;
	m_numRetriesRestartComm = 0;
	m_timerRetryRestartComm.Stop();
	m_timerResetRetries.Stop();
	
	stop_sampling_and_all_peripherals();
	
	m_timerCheckADuCStatusWhileSampling.Stop();

	m_watchDogTimer.Stop();
		
	m_sauPcProtocol->rstGenPacketsSent();
	m_sauPcProtocol->rstEmgPacketsSent();
	m_sauPcProtocol->rstPrsPacketsSent();
	m_sauPcProtocol->rstWeightPacketsSent();
	
	m_timerCheckADuCStatus.Preset(_TIME_CHECK_ADuC_STATUS_);
	
	m_sauPcProtocol->sendSauPc_emgz_JumpedToLoader();
	m_timerCheckEmgBoardStatus.Stop();
	
	wait.Preset(500);
	while(!wait.Match());

	m_emgProtocol->sendJumpedToLoaderRequest();
	changeSau7000status(Sau7000status_emgzBoardUpgrade);
	
	while (m_sauPcProtocol->GetNumBytesInTransmissionBuffer() > 0)
	{
		wait.Preset(100);
		while(!wait.Match());
	}
	
	return 1;
}


/**
Manager of the command weight channels calibration asked.

The command is received and decoded from the class SauPcProtocol which sends a message to the board manager.
@return always 1

@see SauPcProtocol::sendWeightCalibrationValues
*/
int SAU7000_Manager::SauPcMsg_WeightCalAskedHandler()
{
	int error = readToEE_weightCalValue();
	m_sauPcProtocol->sendWeightCalibrationValues(error);
	return 1;
}

/**
Manager of the command pressure calibration asked.

The command is received and decoded from the class SauPcProtocol which sends a message to the board manager.
@return always 1

@see SauPcProtocol::sendPrsCalibrationValues
*/
int SAU7000_Manager::SauPcMsg_PrsCalAskedHandler()
{
	int error = readToEE_prsCalValue();
	setOverpressureAdcLimit();
	m_sauPcProtocol->sendPrsCalibrationValues(error);
	return 1;
}

/**
Manager of the command generic channels calibration asked.

The command is received and decoded from the class SauPcProtocol which sends a message to the board manager.
@return always 1

@see SauPcProtocol::sendGenericCalibrationValues
*/
int SAU7000_Manager::SauPcMsg_GenericCalAskedHandler()
{

	int error0 = readToEE_genCalValue(0);
	int error1 = readToEE_genCalValue(1);
	m_sauPcProtocol->sendGenericCalibrationValues(error0+error1);
	return 1;
}

/**
Manager of the command High Resolution EMG channel calibration asked.

The command is received and decoded from the class SauPcProtocol which sends a message to the board manager.
@return always 1

@see SauPcProtocol::sendEmgCalibrationValues
*/
int SAU7000_Manager::SauPcMsg_EmgCalAskedHandler()
{
	
	return 1;
}

/**
Manager of the command set FLW calibration values.

The command is received and decoded from the class SauPcProtocol which sends a message to the board manager.
@return always 1
*/
int SAU7000_Manager::SauPcMsg_WeightCalSetHandler()
{
	int error = writeToEE_weightCalValue();
	if(error == 0)
	{
		goBuzzer(KEY_LIKE);
	}
	
	m_sauPcProtocol->sendWeightCalibrationValues(error);
	return 1;
}

/**
Manager of the command set PRS calibration values. All the prs on the second connector (P5, P6, P7, P8)

The command is received and decoded from the class SauPcProtocol which sends a message to the board manager.
@return always 1
*/

int SAU7000_Manager::SauPcMsg_Prs_CalSetHandler()
{
	int error = writeToEE_prsCalValue();
	if(error == 0)
	{
		goBuzzer(KEY_LIKE);
	}
	setOverpressureAdcLimit();
	m_sauPcProtocol->sendPrsCalibrationValues(error);
	return 1;
}

/**
Manager of the command set EMG calibration values.

The command is received and decoded from the class SauPcProtocol which sends a message to the board manager.
@return always 1
*/
int SAU7000_Manager::SauPcMsg_EmgCalSetHandler()
{
	m_sauPcProtocol->sendAcknowledge(1);
	return 1;
}

/**
Manager of the command set generic 01 calibration values. 

The command is received and decoded from the class SauPcProtocol which sends a message to the board manager.
@return always 1
*/
int SAU7000_Manager::SauPcMsg_Generic_CalSetHandler()
{
	int error0 = writeToEE_genCalValue(0);
	int error1 = writeToEE_genCalValue(1);
	m_sauPcProtocol->sendGenericCalibrationValues(error0+error1);
	return 1;
}

/**
Manager of the command set ZERO channels.

The command is received and decoded from the class SauPcProtocol which sends a message to the board manager.
@return always 1
*/
int SAU7000_Manager::SauPcMsg_zeroChanHandler()
{
	m_sauPcProtocol->sendAcknowledge(1);
	return 1;
}

/**
Manager of the command set Display.
*/
int SAU7000_Manager::SauPcMsg_setDisplayHandler()
{
	byte valid = EE_SPI_VALID_DISPLAY_SETTING_LOADED;
	
	displaySetting.backlight = m_sauPcProtocol->getDisplayBacklight();
	displaySetting.brightness = m_sauPcProtocol->getDisplayBrightness();
	displaySetting.contrast = m_sauPcProtocol->getDisplayContrast();
	displaySetting.writeSettingToEEPROM = m_sauPcProtocol->getDisplayWriteSettingToEEPROM();
	
	if (displaySetting.backlight == 0)
		displaySetting.brightness = 0;
	
	display_brightness(displaySetting.brightness);
	display_contrast(displaySetting.contrast);
	
	if (displaySetting.writeSettingToEEPROM)	
	{
		goBuzzer(KEY_LIKE);
		ee_spi_write(EE_SPI_ADD_VALID_DISPLAY_SETTING_LOADED, &valid, 1);
		ee_spi_write(EE_SPI_ADD_DISPLAY_SETTING_BACKLIGHT, &(displaySetting.backlight), 1);
		ee_spi_write(EE_SPI_ADD_DISPLAY_SETTING_BRIGTHNESS, &(displaySetting.brightness), 1);
		ee_spi_write(EE_SPI_ADD_DISPLAY_SETTING_CONTRAST, &(displaySetting.contrast), 1);
	}
	
	return (m_sauPcProtocol->sendDisplaySetting(displaySetting.backlight, displaySetting.brightness, displaySetting.contrast) );

}

/**
Manager of the command get Display.
*/
int SAU7000_Manager::SauPcMsg_getDisplayHandler()
{
	return( m_sauPcProtocol->sendDisplaySetting(displaySetting.backlight, displaySetting.brightness, displaySetting.contrast) );
}

int SAU7000_Manager::SauPcMsg_emgzContactImpHandler()
{ 
	return m_emgProtocol->sendCheckContactImpedances(((m_sauPcProtocol->enableEmgzToReadContactImp()) == 1 ? true : false));
}

int SAU7000_Manager::SauPcMsg_emgzSetFilteringHandler()
{
	m_emgProtocol->sendFilteringSetting(m_sauPcProtocol->getLPFiltering(), m_sauPcProtocol->getNOTCHFiltering(), m_sauPcProtocol->getHPFiltering());
	return m_sauPcProtocol->sendAcknowledge(1);
}

int SAU7000_Manager::SauPcMsg_BT_bypassModeHandler()
{
	
	DecTimer wait;
	
	
	
	// sent stop command to ADuC microcontroller and emg board and stop the motors
	//.....
	//.....
	m_whyAcqWasStopped = AcqStopped_BTbypassMode;

	m_retryRestarComm = false;
	m_numRetriesRestartComm = 0;
	m_timerRetryRestartComm.Stop();
	m_timerResetRetries.Stop();
	
	stop_sampling_and_all_peripherals();
	
	m_timerCheckADuCStatusWhileSampling.Stop();
	
	m_sauPcProtocol->rstGenPacketsSent();
	m_sauPcProtocol->rstEmgPacketsSent();
	m_sauPcProtocol->rstPrsPacketsSent();
	m_sauPcProtocol->rstWeightPacketsSent();
	
	m_timerCheckADuCStatus.Preset(_TIME_CHECK_ADuC_STATUS_);

	m_timerCheckEmgBoardStatus.Preset(2000);

	m_watchDogTimer.Stop();

	m_sauPcProtocol->sendAcknowledge(1);
	
	wait.Preset(500);
	while(!wait.Match());

	
	changeSau7000status(Sau7000status_BT_byPassMode);

	
	while (m_sauPcProtocol->GetNumBytesInTransmissionBuffer() > 0)
	{
		wait.Preset(100);
		while(!wait.Match());
	}

	m_bluetoothProtocol->setBypassMode(true, pUART_PC);
	m_sauPcProtocol->setBypassMode(true, pUART_BLUE);

	pUART_BLUE->ClearTransmissionBuffer();
	pUART_BLUE->ClearReceptionBuffer();
	pUART_PC->ClearTransmissionBuffer();
	pUART_PC->ClearReceptionBuffer();
	
	return 1;
	
}
/**
Manages the error packets sent from the PC were lost raised from the PC communication class.
@return always 1
*/
int SAU7000_Manager::SauPcMsg_packetsLostHandler()
{
	return 1;
}

/**
Manages the error timeout RX raised from the PC communication class.
@return always 1
*/
int SAU7000_Manager::SauPcMsg_timoutRxHandler()
{
	m_sauPcComErrors |= (1 << SauPcComError_timoutRx);
	return 1;
}

/**
Manages the error timeout TX raised from the PC communication class.
@return always 1
*/
int SAU7000_Manager::SauPcMsg_timoutTxHandler()
{
	m_sauPcComErrors |= (1 << SauPcComError_timoutTx);
	return 1;
}

/**
Manages the error wrong STX received raised from the PC communication class.
@return always 1
*/
int SAU7000_Manager::SauPcMsg_wrongStxHandler()
{
	m_sauPcComErrors |= (1 << SauPcComError_wrongStx);
	return 1;
}

/**
Manages the error unknown received opcode raised from the PC communication class.
@return always 1
*/
int SAU7000_Manager::SauPcMsg_opcodeUnknownHandler()
{
	m_sauPcComErrors |= (1 << SauPcComError_opcodeUnknown);
	return 1;
}

/**
Manages the error checksum error raised from the PC communication class.
@return always 1
*/
int SAU7000_Manager::SauPcMsg_checkSumErrorHandler()
{
	m_sauPcComErrors |= (1 << SauPcComError_checkSumError);
	return 1;
}

/**
Manages the error uart hardware error raised from the PC communication class.
@return always 1
*/
int SAU7000_Manager::SauPcMsg_HwErrorHandler()
{
	if (((m_sauPcProtocol->getLastHwError() & SauPcProtocol::sauPcUart_Hw_OverflowRxBuff) == SauPcProtocol::sauPcUart_Hw_OverflowRxBuff) ||
	    ((m_sauPcProtocol->getLastHwError() & SauPcProtocol::sauPcUart_Hw_OverrunError) == SauPcProtocol::sauPcUart_Hw_OverrunError))
	{
		m_whyAcqWasStopped = AcqStopped_SauPc_UartHwError;
		m_sauPcProtocol->clearTx();
		m_sauPcProtocol->clearRx();
		m_timerWaitStatus.Preset(10);
		while (!m_timerWaitStatus.Match())
		{
			asm("nop");
		}
		m_timerWaitStatus.Stop();
		
		if(getSau7000status() == Sau7000status_sampling)
			m_retryRestarComm = true;
		stop_sampling_and_all_peripherals();
		
	}
	
	m_sauPcComErrors |= (1 << SauPcComError_HwError);
	return 1;
}

/**
Manages the error transmit buffer overflow raised from the PC communication class.
@return always 1
*/
int SAU7000_Manager::SauPcMsg_transmitBufferOverflowErrorHandler()
{
	m_whyAcqWasStopped = AcqStopped_SauPc_TransmitBufferOverflow;
	m_timerWaitStatus.Preset(100);
	while (!m_timerWaitStatus.Match())
	{
		asm("nop");
	}
	m_sauPcProtocol->clearTx();
	m_timerWaitStatus.Stop();
	
	if(getSau7000status() == Sau7000status_sampling)
		m_retryRestarComm = true;
	stop_sampling_and_all_peripherals();

	m_sauPcComErrors |= (1 << SauPcComError_transmitBufferOverflowError);
	return 1;
}

/**
Manages the error reception buffer overflow raised from the PC communication class.
@return always 1
*/
int SAU7000_Manager::SauPcMsg_receptionBufferOverflowErrorHandler()
{
	m_whyAcqWasStopped = AcqStopped_SauPc_ReceptionBufferOverflow;
	if(getSau7000status() == Sau7000status_sampling)
		m_retryRestarComm = true;
	stop_sampling_and_all_peripherals();
						
	m_sauPcComErrors |= (1 << SauPcComError_receptionBufferOverflowError);
	return 1;
}

/**
Manages the errordata received buffer overflow raised from the PC communication class.
@return always 1
*/
int SAU7000_Manager::SauPcMsg_dataReceivedBufferOverflowHandler()
{
	m_whyAcqWasStopped = AcqStopped_SauPc_DataReceivedBufferOverflow;
	if(getSau7000status() == Sau7000status_sampling)
		m_retryRestarComm = true;
	stop_sampling_and_all_peripherals();
	
	m_sauPcComErrors |= (1 << SauPcComError_dataReceivedBufferOverflow);
	return 1;
}

/**
Manages the error unknow error raised from the PC communication class.
@return always 1
*/
int SAU7000_Manager::SauPcMsg_unknownErrorHandler()
{
	m_sauPcComErrors |= (1 << SauPcComError_unknownError);	
	return 1;
}

int SAU7000_Manager::SauPcMsg_BTcommunicationDroppedHandler()
{
	if(getSau7000status() != Sau7000status_startupChecks)
	{
		m_whyAcqWasStopped = AcqStopped_BTcommunicationDropped;

		m_retryRestarComm = false;
		m_numRetriesRestartComm = 0;
		m_timerRetryRestartComm.Stop();
		m_timerResetRetries.Stop();
		
		stop_sampling_and_all_peripherals();
		m_watchDogTimer.Stop();
	}
	
	m_sauPcProtocol->setUartDevice(pUART_PC);
	m_sauPcProtocol->setCOMM_type(SauPcProtocol::comm_FTDI_USBtoSERIAL);
	m_bluetoothProtocol->setUartDevice(pUART_BLUE);
	//pUART_BLUE->SetHwFlwCtl(UartDeviceAbstract::hwFlwCtl_None);

	pUART_BLUE->ClearTransmissionBuffer();
	pUART_BLUE->ClearReceptionBuffer();
	pUART_PC->ClearTransmissionBuffer();
	pUART_PC->ClearReceptionBuffer();
	
	pMain->setCOMMico(CMenuGeneralInfo::comm_BT, true);
	
	m_COMM_state = comm_chekingBTconnection;
	goBuzzerTimes(ALARM_LIKE, 2);
	return 1;
}

int SAU7000_Manager::SauPcMsg_watchdogSettingHandler()
{
	m_watchdogEnabled = m_sauPcProtocol->watchdogEnabled();
	m_watchdogTime = m_sauPcProtocol->getWatchDogTime();
	m_watchDogTimer.Preset(m_watchdogTime);
	m_sauPcProtocol->sendAcknowledge(1);
	return 1;
}

int SAU7000_Manager::SauPcMsg_setMotorCalHandler()
{
	int error = m_wtp->writeCalibrationValue(m_sauPcProtocol->getWtpCalibrationValues());
	m_sauPcProtocol->sendMotorsCalibrationValues(m_wtp->getCalibration(), error);
	return 1;
}

int SAU7000_Manager::SauPcMsg_getMotorCalHandler()
{
	int error = m_wtp->readCalibrationValue();
	m_sauPcProtocol->sendMotorsCalibrationValues(m_wtp->getCalibration(), error);
	return 1;
}

int SAU7000_Manager::SauPcMsg_setOverpressureHandler()
{

	m_overpressureChanEn = *m_sauPcProtocol->getOverpressureChanEn();
	m_overpressureLimit = m_sauPcProtocol->getOverpressureLimit();

	setOverpressureAdcLimit();
	
	m_sauPcProtocol->sendOverpressureInfo(m_overpressureLimit, m_overpressureChanEn, m_overpressureChan);
	return 1;
}

int SAU7000_Manager::emgBoard_infoReceivedHandler()
{
	m_emgzBoardPresent = true;
	m_emgzBoardNumTimeoutTxOccurred = 0;
	return 1;
}

int SAU7000_Manager::emgBoard_statusReceivedHandler()
{
	int i;
	
	m_emgzBoardStatus = (emgzBoardStatus) m_emgProtocol->getStatus();
	
	if(m_emgzBoardStatus == emgzBoardStatus_acquiring && getSau7000status() == Sau7000status_idle)
	{
		m_emgProtocol->sendStopAcq();
	}
	
	for (i = 0; i < SauAndEmgzProtocol::_NUM_VOLTAGE_MEASURE_EMGZ_; i++)
	{
		m_emgzBoardVoltageMeasure[i] = m_emgProtocol->getVoltageMeasure((SauAndEmgzProtocol::EmgzVoltageMeasures)i);
	}

	m_emgzBoardCurrentMeasure = m_emgProtocol->getCurrentMeasure();
	m_emgzBoardOverCurrentError = m_emgProtocol->getOverCurrent();
	m_emgzBoardNumErrorOccurred = m_emgProtocol->getNumErrors();

	m_emgzBoardLPF_type = m_emgProtocol->getLPFiltering();
	m_emgzBoardNOTCHF_type = m_emgProtocol->getNOTCHFiltering();
	m_emgzBoardHPF_type = m_emgProtocol->getHPFiltering();
		
	m_emgzBoardPresent = true;
	m_emgzBoardNumTimeoutTxOccurred = 0;
	return 1;
}

int SAU7000_Manager::emgBoard_acquisitionStartedHandler()
{
	m_emgzBoardStatus = emgzBoardStatus_acquiring;
	m_emgzBoardPresent = true;
	m_emgzBoardNumTimeoutTxOccurred = 0;
	return 1;
}

int SAU7000_Manager::emgBoard_acquisitionStoppedHandler()
{
	m_emgzBoardStatus = emgzBoardStatus_idle;
	m_emgzBoardPresent = true;
	m_emgzBoardNumTimeoutTxOccurred = 0;
	return 1;
}

int SAU7000_Manager::emgBoard_emgSamplesReceivedHandler()
{
	if(getSau7000status() == Sau7000status_idle || getSau7000status() == Sau7000status_startupChecks
		|| getSau7000status() == Sau7000status_ADuCfrequencyCalibration)
	{
		m_emgProtocol->sendStopAcq();
	}
	return 1;
}

int SAU7000_Manager::emgBoard_contactImpSamplesReceivedHandler()
{
	return 1;
}

int SAU7000_Manager::emgBoard_acknowledgeHandler()
{
	m_sauPcProtocol->sendAcknowledge(1);
	m_emgzBoardPresent = true;
	m_emgzBoardNumTimeoutTxOccurred = 0;
	return 1;
}

int SAU7000_Manager::emgBoard_notAcknowledgeHandler()
{	
	m_sauPcProtocol->sendAcknowledge(0);
	m_emgzBoardPresent = true;
	m_emgzBoardNumTimeoutTxOccurred = 0;
	return 1;
}

int SAU7000_Manager::emgBoard_ErrorPacketReceivedHandler()
{
	m_genericErrors |= (1 << gErr_errorFromEmgzBoard);
	m_emgzBoardPresent = true;
	m_emgzBoardNumTimeoutTxOccurred = 0;
	return 1;
}

int SAU7000_Manager::emgBoard_emgCalReceivedHandler()	
{
	
	m_emgzBoardPresent = true;
	m_emgzBoardNumTimeoutTxOccurred = 0;
	return 1;
}

int SAU7000_Manager::emgBoard_impCalReceivedHandler()	
{
	
	m_emgzBoardPresent = true;
	m_emgzBoardNumTimeoutTxOccurred = 0;
	return 1;
}

int SAU7000_Manager::emgBoard_jumpedToLoaderHandler()	
{

	if(m_COMM_state == comm_USB)
	{
		m_emgzBoardPresent = true;
		m_emgzBoardNumTimeoutTxOccurred = 0;

		m_emgProtocol->SetBaudRate( UartDeviceAbstract::BR38400);
		m_sauPcProtocol->SetBaudRate( UartDeviceAbstract::BR38400);
		m_sauPcProtocol->SetHwFlwCtl(UartDeviceAbstract::hwFlwCtl_None);
		
		m_emgProtocol->setBypassMode(true, pUART_PC);
		m_sauPcProtocol->setBypassMode(true, pUART_EMG);
		
		goBuzzerTimes(LONG_LIKE, 1);
	}else
	{
		 m_sauPcProtocol->sendAcknowledge(0);
	}
	return 1;
}

/**
Manages the packet lost error message raised from the Emg board communication class.
@return always 1
*/
int SAU7000_Manager::emgBoard_packetsLostImp_Handler()	
{
	m_sauEmgBoardErrors |=  (1 << ADuC_ComError_packetsLost01);
	return 1;
}


int SAU7000_Manager::emgBoard_packetsLostEmg_Handler()	
{
	m_sauEmgBoardErrors |=  (1 << ADuC_ComError_packetsLost02);
	return 1;
}


/**
Manages the timeout Rx error message raised from the Emg board communication class.
@return always 1
*/
int SAU7000_Manager::emgBoard_timoutRxHandler()	
{
	m_sauEmgBoardErrors |= (1 << ADuC_ComError_timoutRx);
	return 1;
}

/**
Manages the timeout Tx error message raised from the Emg board communication class.
@return always 1
*/
int SAU7000_Manager::emgBoard_timoutTxHandler()
{
	switch(m_emgProtocol->getLastCmdSent())
	{
		case SauAndEmgzProtocol::NO_CMD_FROM_HOST:
			break;
        case SauAndEmgzProtocol::GET_INFO:
			m_emgProtocol->sendInfoRequest();
			break;
        case SauAndEmgzProtocol::GET_CAL_EMG:
			
			break;
		case SauAndEmgzProtocol::GET_CAL_IMP:
			
			break;
        case SauAndEmgzProtocol::GET_STATUS:
			m_emgProtocol->sendStatusRequest();
			break;
        case SauAndEmgzProtocol::SET_CAL_EMG:
			
			break;
        case SauAndEmgzProtocol::SET_CAL_IMP:
			
			break;
        case SauAndEmgzProtocol::START_ACQUISITION:
			m_emgProtocol->sendStartAcq();
			break;
        case SauAndEmgzProtocol::STOP_ACQUISITION:
			m_emgProtocol->sendStopAcq();
			break;
		case SauAndEmgzProtocol::CHECK_CONTACT_IMP:
			m_emgProtocol->sendCheckContactImpedances(m_emgProtocol->getReadingElectrodeImpedanceActive());
			break;
		case SauAndEmgzProtocol::JUMP_TO_LOADER:
			//m_emgProtocol->sendJumpedToLoaderRequest();
			break;
	}

	if(getSau7000status() == Sau7000status_emgzBoardUpgrade)
	{
		m_emgProtocol->SetBaudRate( UartDeviceAbstract::BR38400);
		m_sauPcProtocol->SetBaudRate( UartDeviceAbstract::BR38400);
		m_sauPcProtocol->SetHwFlwCtl(UartDeviceAbstract::hwFlwCtl_None);
		
		m_emgProtocol->setBypassMode(true, pUART_PC);
		m_sauPcProtocol->setBypassMode(true, pUART_EMG);
		
		goBuzzerTimes(LONG_LIKE, 1);
	}
	
	if (m_emgzBoardNumTimeoutTxOccurred < 5)
	{
		m_emgzBoardNumTimeoutTxOccurred++;
		m_sauEmgBoardErrors |= (1 << ADuC_ComError_timoutTx);

		if(m_emgzBoardNumTimeoutTxOccurred == 4)
		{
			m_emgzBoardPresent = false;
			m_emgzBoardNumTimeoutTxOccurred++;
			m_emgzBoardStatus = (emgzBoardStatus) m_emgProtocol->getStatus();
		}
		
		if((m_emgzBoardNumTimeoutTxOccurred == 4) && (getSau7000status() != Sau7000status_idle))
		{
			m_whyAcqWasStopped = AcqStopped_EmgNotAnswerToStatusCmd;

			m_retryRestarComm = false;
			m_numRetriesRestartComm = 0;
			m_timerRetryRestartComm.Stop();
			m_timerResetRetries.Stop();
			
			stop_sampling_and_all_peripherals();
		} 
		
		
	}
	m_emgzBoardStatus = emgzBoardStatus_unknown;
	return 1;
}

/**
Manages the stx not received error message raised from the Emg board communication class.
@return always 1
*/
int SAU7000_Manager::emgBoard_wrongStxHandler()	
{
	m_sauEmgBoardErrors |= (1 << ADuC_ComError_wrongStx);
	return 1;
}

/**
Manages the unknown opcode error message raised from the Emg board communication class.
@return always 1
*/
int SAU7000_Manager::emgBoard_opcodeUnknownHandler()
{
	m_sauEmgBoardErrors |= (1 << ADuC_ComError_opcodeUnknown);
	return 1;
}

/**
Manages the checksum error message raised from the Emg board communication class.
@return always 1
*/
int SAU7000_Manager::emgBoard_checkSumErrorHandler()	
{
	m_sauEmgBoardErrors |= (1 << ADuC_ComError_checkSumError);
	return 1;
}

/**
Manages the hw error error message raised from the Emg board communication class.
@return always 1
*/
inline int SAU7000_Manager::emgBoard_HwErrorHandler()	
{
	if (m_emgzBoardNumTimeoutTxOccurred < 5)
	{
		if (((m_emgProtocol->getLastHwError() & SauAndEmgzProtocol::Uart_Hw_OverflowRxBuff) == SauAndEmgzProtocol::Uart_Hw_OverflowRxBuff)||
			((m_emgProtocol->getLastHwError() & SauAndEmgzProtocol::Uart_Hw_OverrunError) == SauAndEmgzProtocol::Uart_Hw_OverrunError))
		{
			m_whyAcqWasStopped = AcqStopped_SauEmgz_UartHwError;
			m_emgProtocol->clearTx();
			m_emgProtocol->clearRx();
			while (!m_timerWaitStatus.Match())
			{
				asm("nop");
			}
			m_timerWaitStatus.Stop();


			if(getSau7000status() == Sau7000status_sampling)
				m_retryRestarComm = true;
			stop_sampling_and_all_peripherals();
			m_sauEmgBoardErrors |= (1 << ADuC_ComError_HwError);
		}
	}
	return 1;
}

/**
Manages the transmission buffer overflow error message raised from the Emg board communication class.
@return always 1
*/
int SAU7000_Manager::emgBoard_transmitBufferOverflowErrorHandler()	
{
	if (m_emgzBoardNumTimeoutTxOccurred < 5)
	{
		m_whyAcqWasStopped = AcqStopped_SauEmgz_TransmitBufferOverflow;
		m_timerWaitStatus.Preset(10);
		while (!m_timerWaitStatus.Match())
		{
			asm("nop");
		}
		m_emgProtocol->clearTx();
		m_timerWaitStatus.Stop();
		
		if(getSau7000status() == Sau7000status_sampling)
			m_retryRestarComm = true;
		stop_sampling_and_all_peripherals();
		m_sauEmgBoardErrors |= (1 << ADuC_ComError_transmitBufferOverflowError);
	}
	return 1;
}

/**
Manages the reception buffer overflow error message raised from the Emg board communication class.
@return always 1
*/
int SAU7000_Manager::emgBoard_receptionBufferOverflowErrorHandler()	
{
	m_whyAcqWasStopped = AcqStopped_SauEmgz_ReceptionBufferOverflow;
	if(getSau7000status() == Sau7000status_sampling)
		m_retryRestarComm = true;
	stop_sampling_and_all_peripherals();
	
	m_sauEmgBoardErrors |= (1 << ADuC_ComError_receptionBufferOverflowError);
	return 1;
}

/**
Manages the overflow data buffer (decoded data) error message raised from the Emg board communication class.
@return always 1
*/
int SAU7000_Manager::emgBoard_dataReceivedBufferOverflowHandler()	
{
	if (m_emgzBoardNumTimeoutTxOccurred < 5)
	{
		m_whyAcqWasStopped = AcqStopped_SauEmgz_DataReceivedBufferOverflow;
		if(getSau7000status() == Sau7000status_sampling)
			m_retryRestarComm = true;
		stop_sampling_and_all_peripherals();
		
		m_sauEmgBoardErrors |= (1 << ADuC_ComError_dataReceivedBufferOverflow);
	}
	return 1;
}

int SAU7000_Manager::emgBoard_EmgSamplesBufferOverflowHandler()
{
	if (m_emgzBoardNumTimeoutTxOccurred < 5)
	{
		m_whyAcqWasStopped = AcqStopped_SauEmgz_EmgSamplesBufferOverflow;
		if(getSau7000status() == Sau7000status_sampling)
			m_retryRestarComm = true;
		stop_sampling_and_all_peripherals();
		
		m_genericErrors |= (1 << gErr_Emg_BufferOverflow);
	}
	return 1;
}


int SAU7000_Manager::emgBoard_ImpEmgSamplesBufferOverflowHandler()
{
	if (m_emgzBoardNumTimeoutTxOccurred < 5)
	{
		m_whyAcqWasStopped = AcqStopped_SauEmgz_EmgImpSamplesBufferOverflow;
		if(getSau7000status() == Sau7000status_sampling)
			m_retryRestarComm = true;
		stop_sampling_and_all_peripherals();
		
		m_genericErrors |= (1 << gErr_EmgImp_BufferOverflow);
	}
	return 1;
}

/**
Manages the unknown error message raised from the Emg board communication class.
@return always 1
*/
int SAU7000_Manager::emgBoard_unknownErrorHandler()	
{
	
	m_sauEmgBoardErrors |= (1 << ADuC_ComError_unknownError);
	return 1;
}


int SAU7000_Manager::ADuC_infoReceivedHandler(int i)
{
	m_AduC_firmwareVersion[i] = (m_ADuC_Protocols[i]->getADuCFwRevision());
	switch(m_ADuC_Protocols[i]->getADuCState())
	{
		case uC_Protocol::ADuC_unknownState:
			m_ADuCStatus[i] = ADuCStatus_unknown;
			break;
		case uC_Protocol::ADuC_idleState:
			m_ADuCStatus[i] = ADuCStatus_idle;
			break;
		case uC_Protocol::ADuC_AquiringState:
			m_ADuCStatus[i] = ADuCStatus_acquiring;
			break;
		case uC_Protocol::ADuC_SelfTestState:
			m_ADuCStatus[i] = ADuCStatus_selfTest;
			break;
		
	}
	m_ADuC_numErrors[i] = 0;
	return 1;
}

int SAU7000_Manager::ADuC_statusReceivedHandler(int i)	
{
	switch(m_ADuC_Protocols[i]->getADuCState())
	{
		case uC_Protocol::ADuC_unknownState:
			m_ADuCStatus[i] = ADuCStatus_unknown;
			break;
		case uC_Protocol::ADuC_idleState:
			m_ADuCStatus[i] = ADuCStatus_idle;
			break;
		case uC_Protocol::ADuC_AquiringState:
			m_ADuCStatus[i] = ADuCStatus_acquiring;
			break;
		case uC_Protocol::ADuC_SelfTestState:
			m_ADuCStatus[i] = ADuCStatus_selfTest;
			break;
		
	}
	m_ADuC_numErrors[i] = 0;
	return 1;
}

inline int SAU7000_Manager::ADuC_acquisitionStartedHandler(int i)
{
	m_ADuCStatus[i] = ADuCStatus_acquiring;
	m_ADuC_numErrors[i] = 0;
	return 1;
}
	
int SAU7000_Manager::ADuC_acquisitionStoppedHandler(int i)	
{
	m_ADuCStatus[i] = ADuCStatus_idle;
	m_ADuC_numErrors[i] = 0;
	return 1;
}

int SAU7000_Manager::ADuC_packet01_receivedHandler(int i)
{
	m_ADuCacqCheckTimer[i].Preset(500);
	return 1;
}

int SAU7000_Manager::ADuC_packet02_receivedHandler(int i)
{
	m_ADuCacqCheckTimer[i].Preset(500);
	return 1;
}

int SAU7000_Manager::ADuC_packet03_receivedHandler(int i)
{
	m_ADuCacqCheckTimer[i].Preset(500);
	return 1;
}

int SAU7000_Manager::ADuC_received_SETTING_ADCHandler(int i)
{
	return 1;
}

int SAU7000_Manager::ADuC_msg_received_SETTING_IEXCHandler(int i)
{
	return 1;
}

int SAU7000_Manager::ADuC_received_SETTING_DACHandler(int i)
{
	return 1;
}

int SAU7000_Manager::ADuC_received_SETTING_PWMHandler(int i)
{
	return 1;
}

int SAU7000_Manager::ADuC_ALL_Setting_ACKHandler(int i)
{
	m_ADuCStatus[i] = ADuCStatus_set;
	m_ADuC_numErrors[i] = 0;
	return 1;
}

int SAU7000_Manager::ADuC_acquisitionNOTstartedHandler(int i)	
{
	return 1;
}

int SAU7000_Manager::ADuC_ErrorPacketReceivedHandler(int i)	
{
	bool stop = false;
	if((m_ADuC_Protocols[i]->getLastADuC_commError() & (1 << uC_Protocol::__e_comm_uart_overrun)) == (1 << uC_Protocol::__e_comm_uart_overrun))
	{
		m_ADuC_ComErrors[i] |= (1 << ADuC__e_comm_UartRecErr);
	}
	if((m_ADuC_Protocols[i]->getLastADuC_commError() & (1 << uC_Protocol::__e_comm_uart_parity)) == (1 << uC_Protocol::__e_comm_uart_parity))
	{
		m_ADuC_ComErrors[i] |= (1 << ADuC__e_comm_UartRecErr);
	}
	if((m_ADuC_Protocols[i]->getLastADuC_commError() & (1 << uC_Protocol::__e_comm_uart_frame)) == (1 << uC_Protocol::__e_comm_uart_frame))
	{
		m_ADuC_ComErrors[i] |= (1 << ADuC__e_comm_UartRecErr);
	}
	if((m_ADuC_Protocols[i]->getLastADuC_commError() & (1 << uC_Protocol::__e_comm_OpCodeUnknown)) == (1 << uC_Protocol::__e_comm_OpCodeUnknown))
	{
		m_ADuC_ComErrors[i] |= (1 << ADuC__e_comm_OpCodeUnknown);
	}
	if((m_ADuC_Protocols[i]->getLastADuC_commError() & (1 << uC_Protocol::__e_comm_WrongNumData)) == (1 << uC_Protocol::__e_comm_WrongNumData))
	{
		m_ADuC_ComErrors[i] |= (1 << ADuC__e_comm_WrongNumData);
	}
	if((m_ADuC_Protocols[i]->getLastADuC_commError() & (1 << uC_Protocol::__e_comm_CrcError)) == (1 << uC_Protocol::__e_comm_CrcError))
	{
		m_ADuC_ComErrors[i] |= (1 << ADuC__e_comm_CrcError);
	}
	if((m_ADuC_Protocols[i]->getLastADuC_commError() & (1 << uC_Protocol::__e_comm_EtxNotReceived)) == (1 << uC_Protocol::__e_comm_EtxNotReceived))
	{
		m_ADuC_ComErrors[i] |= (1 << ADuC__e_comm_EtxNotReceived);
	}
	if((m_ADuC_Protocols[i]->getLastADuC_commError() & (1 << uC_Protocol::__e_comm_StxNotReceived)) == (1 << uC_Protocol::__e_comm_StxNotReceived))
	{
		m_ADuC_ComErrors[i] |= (1 << ADuC__e_comm_StxNotReceived);
	}
	if((m_ADuC_Protocols[i]->getLastADuC_commError() & (1 << uC_Protocol::__e_comm_TxFifoOverflow)) == (1 << uC_Protocol::__e_comm_TxFifoOverflow))
	{
		m_ADuC_ComErrors[i] |= (1 << ADuC__e_comm_TxFifoOverflow);
		stop = true;
	}
	if((m_ADuC_Protocols[i]->getLastADuC_commError() & (1 << uC_Protocol::__e_comm_RxFifoOverflow)) == (1 << uC_Protocol::__e_comm_RxFifoOverflow))
	{
		m_ADuC_ComErrors[i] |= (1 << ADuC__e_comm_RxFifoOverflow);
		stop = true;
	}
	if((m_ADuC_Protocols[i]->getLastADuC_commError() & (1 << uC_Protocol::__e_comm_TimeoutRx)) == (1 << uC_Protocol::__e_comm_TimeoutRx))
	{
		m_ADuC_ComErrors[i] |= (1 << ADuC__e_comm_TimeoutRx);
	}
	if((m_ADuC_Protocols[i]->getLastADuC_commError() & (1 << uC_Protocol::__e_comm_TimeoutTx)) == (1 << uC_Protocol::__e_comm_TimeoutTx))
	{
		m_ADuC_ComErrors[i] |= (1 << ADuC__e_comm_TimeoutTx);
	}
	if((m_ADuC_Protocols[i]->getLastADuC_commError() & (1 << uC_Protocol::__e_comm_OpcodeFifoOverflow)) == (1 << uC_Protocol::__e_comm_OpcodeFifoOverflow))
	{
		m_ADuC_ComErrors[i] |= (1 << ADuC__e_comm_OpcodeFifoOverflow);
	}
	if((m_ADuC_Protocols[i]->getLastADuC_commError() & (1 << uC_Protocol::__e_comm_ReceivedDataFifoOverflow)) == (1 << uC_Protocol::__e_comm_ReceivedDataFifoOverflow))
	{
		m_ADuC_ComErrors[i] |= (1 << ADuC__e_comm_ReceivedDataFifoOverflow);
	}
	if((m_ADuC_Protocols[i]->getLastADuC_commError() & (1 << uC_Protocol::__e_comm_pAdcFifoOverflow)) == (1 << uC_Protocol::__e_comm_pAdcFifoOverflow))
	{
		m_ADuC_ComErrors[i] |= (1 << ADuC__e_comm_pAdcFifoOverflow);
		stop = true;
	}
	if((m_ADuC_Protocols[i]->getLastADuC_commError() & (1 << uC_Protocol::__e_comm_aAdcFifoOverflow)) == (1 << uC_Protocol::__e_comm_aAdcFifoOverflow))
	{
		m_ADuC_ComErrors[i] |= (1 << ADuC__e_comm_aAdcFifoOverflow);
		stop = true;
	}
	m_ADuC_numErrors[i] = 0;

	if(stop)
	{
		m_ADuC_Protocols[i]->clearTx();
		m_timerWaitStatus.Preset(10);
		while (!m_timerWaitStatus.Match())
		{
			asm("nop");
		}
		m_timerWaitStatus.Stop();
		m_ADuC_Protocols[i]->clearRx();
		if(getSau7000status() == Sau7000status_sampling)
			m_retryRestarComm = true;
		stop_sampling_and_all_peripherals();

	}
	return 1;
}

int SAU7000_Manager::ADuC_messageAckPacketReceivedHandler(int i)
{	
	return 1;
}

int SAU7000_Manager::ADuC_messageNotAckPacketReceivedHandler(int i)
{
	m_ADuC_ComErrors[i] |= (1 << ADuC_ComError_packetNotAck);
	return 1;
}

/**
Manages the packet lost error message raised from the ADuCs communications classes.
@return always 1
*/
int SAU7000_Manager::ADuC_packetsLost_01Handler(int i)
{
	m_ADuCacqCheckTimer[i].Preset(500);
	m_ADuC_ComErrors[i] |= (1 << ADuC_ComError_packetsLost01);
	return 1;
}

/**
Manages the packet lost error message raised from the ADuCs communications classes.
@return always 1
*/
int SAU7000_Manager::ADuC_packetsLost_02Handler(int i)	
{
	m_ADuC_ComErrors[i] |= (1 << ADuC_ComError_packetsLost02);
	return 1;
}

int SAU7000_Manager::ADuC_packetsLost_03Handler(int i)	
{
	m_ADuC_ComErrors[i] |= (1 << ADuC_ComError_packetsLost03);
	return 1;
}

/**
Manages the timeout Tx error message raised from the ADuCs communications classes.
@return always 1
*/
int SAU7000_Manager::ADuC_timoutRxHandler(int i)	
{
	if(m_ADuC_numErrors[i] >= 3)
	{
		m_ADuCStatus[i] = ADuCStatus_notRespondingToCommand;
	}
	m_ADuC_ComErrors[i] |= (1 << ADuC_ComError_timoutRx);
	m_ADuC_numErrors[i]++;
	return 1;
}

/**
Manages the timeout Rx error message raised from the ADuCs communications classes.
@return always 1
*/
inline int SAU7000_Manager::ADuC_timoutTxHandler(int i)
{
	
	if(m_ADuC_numErrors[i] >= 3)
	{
		m_ADuCStatus[i] = ADuCStatus_notRespondingToCommand;
	}
	m_ADuC_ComErrors[i] |= (1 << ADuC_ComError_timoutTx);
	m_ADuC_numErrors[i]++;
	return 1;
}

/**
Manages the wrong ST>X error message raised from the ADuCs communications classes.
@return always 1
*/
int SAU7000_Manager::ADuC_wrongStxHandler(int i)	
{
	m_ADuC_ComErrors[i] |= (1 << ADuC_ComError_wrongStx);
	return 1;
}

/**
Manages the unknown opcode error message raised from the ADuCs communications classes.
@return always 1
*/
inline int SAU7000_Manager::ADuC_opcodeUnknownHandler(int i)	
{
	m_ADuC_ComErrors[i] |= (1 << ADuC_ComError_opcodeUnknown);
	return 1;
}

/**
Manages the checksum error message raised from the ADuCs communications classes.
@return always 1
*/
int SAU7000_Manager::ADuC_checkSumErrorHandler(int i)	
{
	m_ADuC_ComErrors[i] |= (1 << ADuC_ComError_checkSumError);
	return 1;
}

/**
Manages the packet HwError message raised from the ADuCs communications classes.
@return always 1
*/
int SAU7000_Manager::ADuC_HwErrorHandler(int i)	
{
	if (((m_ADuC_Protocols[i]->getLastHwError() & uC_Protocol::sauuCUart_Hw_OverflowRxBuff) == uC_Protocol::sauuCUart_Hw_OverflowRxBuff) ||
	    ((m_ADuC_Protocols[i]->getLastHwError() & uC_Protocol::sauuCUart_Hw_OverrunError) == uC_Protocol::sauuCUart_Hw_OverrunError) ||
	    ((m_ADuC_Protocols[i]->getLastHwError() & uC_Protocol::sauuCUart_OverrunFpgaFifo) == uC_Protocol::sauuCUart_OverrunFpgaFifo))    
	{
		m_whyAcqWasStopped = AcqStopped_ADuC_UartHwError;
		m_ADuC_Protocols[i]->clearTx();
		m_ADuC_Protocols[i]->clearRx();
		m_timerWaitStatus.Preset(10);
		while (!m_timerWaitStatus.Match())
		{
			asm("nop");
		}
		m_timerWaitStatus.Stop();
		
		if(getSau7000status() == Sau7000status_sampling)
			m_retryRestarComm = true;
		stop_sampling_and_all_peripherals();
		
	}
	
	m_ADuC_ComErrors[i] |= (1 << ADuC_ComError_HwError);
	return 1;
}

/**
Manages the transmission buffer overflow message raised from the ADuCs communications classes.
@return always 1
*/
int SAU7000_Manager::ADuC_transmitBufferOverflowErrorHandler(int i)	
{
	m_whyAcqWasStopped = AcqStopped_ADuC_TransmitBufferOverflow;
	m_timerWaitStatus.Preset(10);
	while (!m_timerWaitStatus.Match())
	{
		asm("nop");
	}
	m_ADuC_Protocols[i]->clearTx();
	m_timerWaitStatus.Stop();
	
	if(getSau7000status() == Sau7000status_sampling)
		m_retryRestarComm = true;
	stop_sampling_and_all_peripherals();
	m_ADuC_ComErrors[i] |= (1 << ADuC_ComError_transmitBufferOverflowError);
	return 1;
}

/**
Manages the reception buffer overflow message raised from the ADuCs communications classes.
@return always 1
*/
int SAU7000_Manager::ADuC_receptionBufferOverflowErrorHandler(int i)	
{
	m_whyAcqWasStopped = AcqStopped_ADuC_ReceptionBufferOverflow;
	if(getSau7000status() == Sau7000status_sampling)
		m_retryRestarComm = true;
	stop_sampling_and_all_peripherals();
	
	m_ADuC_ComErrors[i] |= (1 << ADuC_ComError_receptionBufferOverflowError);
	return 1;
}

/**
Manages the data received buffer overflow (decoded data) error message raised from the ADuCs communications classes.
@return always 1
*/
int SAU7000_Manager::ADuC_dataReceivedBufferOverflowHandler(int i)
{
	m_whyAcqWasStopped = AcqStopped_ADuC_DataReceivedBufferOverflow;
	if(getSau7000status() == Sau7000status_sampling)
		m_retryRestarComm = true;
	stop_sampling_and_all_peripherals();
	
	m_ADuC_ComErrors[i] |= (1 << ADuC_ComError_dataReceivedBufferOverflow);
	return 1;
}

int SAU7000_Manager::ADuC_weightChanBufferOverflowHandler(int i)
{
	m_whyAcqWasStopped = AcqStopped_weightsChanBufferOverflow;
	if(getSau7000status() == Sau7000status_sampling)
		m_retryRestarComm = true;
	stop_sampling_and_all_peripherals();
	
	m_genericErrors |= (1<< gErr_WeightChan_BufferOverflow);
	return 1;
}

int SAU7000_Manager::ADuC_prsChanBufferOverflowHandler(int i)
{
	m_whyAcqWasStopped = AcqStopped_prsChanBufferOverflow;
	if(getSau7000status() == Sau7000status_sampling)
		m_retryRestarComm = true;
	stop_sampling_and_all_peripherals();
	
	m_genericErrors |= (1<< gErr_PrsChan_BufferOverflow);
	return 1;
}

int SAU7000_Manager::ADuC_genChanBufferOverflowHandler(int i)
{
	m_whyAcqWasStopped = AcqStopped_genericsChanBufferOverflow;
	if(getSau7000status() == Sau7000status_sampling)
		m_retryRestarComm = true;
	stop_sampling_and_all_peripherals();
	
	m_genericErrors |= (1<< gErr_GenChan_BufferOverflow); 
	return 1;
}

/**
Manages the unknown error message raised from the ADuCs communications classes.
@return always 1
*/
int SAU7000_Manager::ADuC_unknownErrorHandler(int i)	
{
	m_ADuC_ComErrors[i] |= (1 << ADuC_ComError_unknownError);
	return 1;
}

int SAU7000_Manager::iWRAP_msg_CONNECTED_Handler(char *par, int len)
{	
	//goBuzzer(LONG_LIKE);
	if (m_COMM_state == comm_chekingBTconnection)
	{
		m_firstBtCheck = false;	
		m_COMM_state = comm_BT_connected;
		pMain->setCOMMico(CMenuGeneralInfo::comm_BT, false);
		
		m_pUART_PROTOCOL_PC = pUART_BLUE;
		m_sauPcProtocol->setUartDevice(pUART_BLUE);
		m_sauPcProtocol->setCOMM_type(SauPcProtocol::comm_BT);
		m_bluetoothProtocol->setUartDevice(NULL);

		pUART_BLUE->ClearTransmissionBuffer();
		pUART_BLUE->ClearReceptionBuffer();
		pUART_PC->ClearTransmissionBuffer();
		pUART_PC->ClearReceptionBuffer();
	
		goBuzzerTimes(LONG_LIKE, 1);
		
		/*if(m_watchdogEnabled)
		{
			m_watchDogTimer.Preset(m_watchdogTime);
		}*/
	}
	return 1;
}

int SAU7000_Manager::iWRAP_msg_DISCONNECTED_Handler(char *par, int len)
{

	if(m_COMM_state != comm_USB && getSau7000status() != Sau7000status_switchingOff)//(m_BTcheckStatus == BTchk_wating_CALL)
	{
		pageInfo->addStr((char*)str_NO_CARRIER);

		if(getSau7000status() != Sau7000status_startupChecks)
		{
			m_whyAcqWasStopped = AcqStopped_BTcommunicationDropped;

			m_retryRestarComm = false;
			m_numRetriesRestartComm = 0;
			m_timerRetryRestartComm.Stop();
			m_timerResetRetries.Stop();
			
			stop_sampling_and_all_peripherals();
			m_watchDogTimer.Stop();
		}
		
		m_sauPcProtocol->setUartDevice(pUART_PC);
		m_sauPcProtocol->setCOMM_type(SauPcProtocol::comm_FTDI_USBtoSERIAL);
		m_bluetoothProtocol->setUartDevice(pUART_BLUE);
		
		pUART_BLUE->ClearTransmissionBuffer();
		pUART_BLUE->ClearReceptionBuffer();
		pUART_PC->ClearTransmissionBuffer();
		pUART_PC->ClearReceptionBuffer();
		
		//pUART_BLUE->SetHwFlwCtl(UartDeviceAbstract::hwFlwCtl_None);
		pMain->setCOMMico(CMenuGeneralInfo::comm_BT, true);
		m_COMM_state = comm_chekingBTconnection;
		goBuzzerTimes(LONG_LIKE, 1);
	
	}
	return 1;
}

int SAU7000_Manager::iWRAP_msg_INFO_Handler(char *par, int len)
{
	changeBTcheckStatus(BTchk_set_Profile);
	return 1;
}

int SAU7000_Manager::iWRAP_msg_SET_BT_PROFILE_Handler(char *par, int len)
{
	changeBTcheckStatus(BTchk_set_BT_SSP);
	return 1;
}

int SAU7000_Manager::iWRAP_msg_SET_BT_NAME_Handler(char *par, int len)
{
	changeBTcheckStatus(BTchk_set_BT_AUTH);
	return 1;
}

int SAU7000_Manager::iWRAP_msg_SET_BT_AUTH_Handler(char *par, int len)
{
	changeBTcheckStatus(BTchk_CALL);
	return 1;
}

int SAU7000_Manager::iWRAP_msg_SET_BT_SSP_Handler(char *par, int len)
{
	//changeBTcheckStatus(BTchk_set_MSC);
	changeBTcheckStatus(BTchk_set_BT_NAME);
	return 1;
}

int SAU7000_Manager::iWRAP_msg_SET_BAUD_Handler(char *par, int len)
{
	changeBTcheckStatus(BTchk_ask_Info);
	return 1;
}

int SAU7000_Manager::iWRAP_msg_SET_CONTROL_MSC_Handler(char *par, int len)
{
	//changeBTcheckStatus(BTchk_set_BT_NAME);
	return 1;
}

int SAU7000_Manager::iWRAP_msg_RING_Handler(char *par, int len)
{
	byte *pData;
	char str[30];
	char str2[20];
	byte data;
	int i;
	bool knownCallingBTAddress = false;

	pData = (byte*)(&par[2]);
	
	ee_spi_read(EE_SPI_ADD_BT_ADDRESS_LOADED, &data, 1);
	if(data == EE_SPI_ADD_BT_VALID_ADDRESS_LOADED)
	{
		knownCallingBTAddress = true;
		ee_spi_read(EE_SPI_ADD_BT_ADDRESS, (byte*)m_remoteBTaddr, 17);
		m_remoteBTaddr[17] = '\0';
		for(i = 0; i < 17; i++)
		{
			if (m_remoteBTaddr[i] != pData[i])
				knownCallingBTAddress = false;
		}
	} 

	memcpy(str2, pData, 17);
	str2[17] = '\0';

	strcpy(str, "RING:");
	strcat(str, str2);
	pageInfo->addStr(str);
	pageInfo->addStr(" ");
	
	if(knownCallingBTAddress)
	{
		//m_BTcheckStatus = BTchk_verify_BR;
	}else
	{
		data = EE_SPI_ADD_BT_VALID_ADDRESS_LOADED;
		ee_spi_write(EE_SPI_ADD_BT_ADDRESS_LOADED, &data, 1);

		
		ee_spi_write(EE_SPI_ADD_BT_ADDRESS, pData, 17);

		//pageInfo->addStr("Press ON/OFF to join the PC..");
		//m_BTcheckStatus = BTchk_idle;
	}

	m_firstBtCheck = false;	
	m_COMM_state = comm_BT_connected;
	
	pMain->setCOMMico(CMenuGeneralInfo::comm_BT, false);

	m_pUART_PROTOCOL_PC = pUART_BLUE;
	m_sauPcProtocol->setUartDevice(pUART_BLUE);
	m_sauPcProtocol->setCOMM_type(SauPcProtocol::comm_BT);
	m_bluetoothProtocol->setUartDevice(NULL);
	
	pUART_BLUE->ClearTransmissionBuffer();
	pUART_BLUE->ClearReceptionBuffer();
	pUART_PC->ClearTransmissionBuffer();
	pUART_PC->ClearReceptionBuffer();
	
	goBuzzerTimes(KEY_LIKE, 1);
	
	return 1;
}

int SAU7000_Manager::iWRAP_msg_CALL_Handler(char *par, int len)
{
	if (len < 5)
	{	
		pageInfo->addStr("CALLING....");
		pageInfo->addStr(" ");
	}
	return 1;
}

int SAU7000_Manager::iWRAP_msg_moduleNotResponding_Handler(char *par, int len)
{
	if(m_firstBtCheck)
	{
		changeBTcheckStatus(BTchk_idle);
		m_BTidleStatusTimer.Preset(4000);
	}else
	{
		m_COMM_state = comm_chekingBTconnection;
	}
	return 1;
}

int SAU7000_Manager::iWRAP_msg_syntaxErrorSyncroError_Handler(char *par, int len)
{
	
	return 1;
}

int SAU7000_Manager::iWRAP_msg_timoutRx_Handler(char *par, int len)
{
	
	return 1;
}

int SAU7000_Manager::iWRAP_msg_timoutTx_Handler(char *par, int len)
{
	
	return 1;
}

int SAU7000_Manager::iWRAP_msg_HwError_Handler(char *par, int len)
{
	
	return 1;
}

int SAU7000_Manager::iWRAP_msg_transmitBufferOverflowError_Handler(char *par, int len)
{
	
	return 1;
}

int SAU7000_Manager::iWRAP_msg_receptionBufferOverflowError_Handler(char *par, int len)
{
	
	return 1;
}

int SAU7000_Manager::iWRAP_msg_dataReceivedBufferOverflow_Handler(char *par, int len)
{
	
	return 1;
}

int SAU7000_Manager::iWRAP_msg_unknownError_Handler(char *par, int len)
{
	
	return 1;
}
		

/**
Sends the error message.

@return always 1
@see m_ADuC_ComErrors
@see m_sauPcComErrors
#see SauPcProtocol::sendSauPcErrors
*/
int SAU7000_Manager::sendErrorsMsg()
{
	byte err[110];
	int i, k, retVal;
	word sau7003_comErr, sau7003_genErr;
	byte sau7003_hwErr;
	
	i = 0;
	//--- Is ADuC working? ----
	err[i] = 0xFF;
	i++;

	//------ ADuC errors ---
	for (k = 0; k < ADuC_NUM; k++)
	{
		err[i] = m_ADuC_Protocols[k]->getLastHwError();//getLastuCError();
		i++;
	}
	
	//------ ADuC packets lost ---
	for (k = 0; k < ADuC_NUM; k++)
	{
		err[i] = m_ADuC_Protocols[k]->getNumPacketsLost01();
		i++;
	}
	for (k = 0; k < ADuC_NUM; k++)
	{
		err[i] = m_ADuC_Protocols[k]->getNumPacketsLost02();
		i++;
	}
	for (k = 0; k < ADuC_NUM; k++)
	{
		err[i] = m_ADuC_Protocols[k]->getNumPacketsLost03();
		i++;
	}
	for (k = 0; k < ADuC_NUM; k++)
	{
		err[i] = m_ADuC_Protocols[k]->getLastADuC_notAckReason();
		i++;
		err[i] = m_ADuC_Protocols[k]->getLastADuC_consistencyError();
		i++;
		err[i] = m_ADuC_Protocols[k]->getLastADuC_settingReceived();
		i++;
	}
	//-- ADuC errors --
	for (k = 0; k < ADuC_NUM; k++)
	{
		*((dword*)&err[i]) = m_ADuC_ComErrors[k];
		i += 4;

	}
	
	//------ EMG board error ----
	err[i] = m_emgProtocol->getLastHwError();
	i++;

	//------ EMG packets lost error ----
	err[i] = m_emgProtocol->getPacketsLostImp();
	i++;
	err[i] = m_emgProtocol->getPacketsLostEmg();
	i++;
	
	//--- communication error with emg board ----
	*((word*)&err[i]) = m_sauEmgBoardErrors;
	i++;
	i++;

	//--- SAU errors ---
	err[i] = m_sauPcProtocol->getLastHwError();
	i++;
	
	//---- communication error with pc ----
	*((word*)&err[i]) = m_sauPcComErrors;
	i++;
	i++;

	//--- SAU-7000's generic error -----
	*((word*)&err[i]) = m_genericErrors;
	i++;
	i++;

	//-- SAU-7003's errors ----
	if((m_genericErrors & (1 << gErr_errorFromEmgzBoard)) == (1 << gErr_errorFromEmgzBoard))
	{
		m_emgProtocol->getLastEmgzBoardErrors(sau7003_comErr, sau7003_hwErr, sau7003_genErr);

		*((word*)&err[i]) = sau7003_comErr;
		i++;
		i++;
		
		err[i] = sau7003_hwErr;
		i++;
		
		*((word*)&err[i]) = sau7003_genErr;
		i++;
		i++;
		
	}else
	{
		*((word*)&err[i]) = 0;
		i++;
		i++;
		
		err[i] = 0;
		i++;
		
		*((word*)&err[i]) = 0;
		i++;
		i++;
	}
	
	
	//---- sending errors ----
	retVal = m_sauPcProtocol->sendSauPcErrors(err, i);
	if (retVal != 0)
	{
		//--- rst errors ----
		m_sauEmgBoardErrors = 0;
		m_sauPcComErrors = 0;
		for (i = 0; i < ADuC_NUM; i++)
		{
			m_ADuC_ComErrors[i] = 0;
			m_ADuC_Protocols[i]->rstLastHwError();
		}
		m_genericErrors =  0;
	}

	
	return retVal;
}

/**
Changes the machine status

@param newStatus state
@return always 0 
@see Sau7000_Status
*/
byte SAU7000_Manager::changeSau7000status(Sau7000_Status newStatus)
{
	if(newStatus != m_sau7000status)
	{
		m_sau7000previousStatus = m_sau7000status;
		m_sau7000status = newStatus;
	}

	if(m_sau7000previousStatus == Sau7000status_switchingOff ||
	   m_sau7000previousStatus == Sau7000status_checkADuCfwUpgrade)
		return 0;
	
	switch(m_sau7000status)
	{
		case Sau7000status_idle:
			pMain->setSAUstate("IDLE");
			
			break;
		case Sau7000status_sendingSettings:
			pMain->setSAUstate("SETTING");
			
			break;
		case Sau7000status_waitingADuCToStart:
			pMain->setSAUstate("ADuC STARTING");
			
			break;
		case Sau7000status_sampling:
			pMain->setSAUstate("ACQUIRING");
			
			break;
		case Sau7000status_waitingADuCToStop:
			pMain->setSAUstate("ADuC STOPPING");
			
			break;
		case Sau7000status_error:
			pMain->setSAUstate("ERROR");
			switch(m_sauError)
			{
				case SauError_noError:
					pMain->setSAUstate("ERROR ??");
					break;
				case SauError_ADuCsCanNotbeUpgraded:
					break;
				case SauError_watchdogError:
					break;
				case SauError_fpgaUpgradeFailed:
					break;
			}
			break;
		case Sau7000status_wait:
			pMain->setSAUstate("WAIT");
			
			break;
		case Sau7000status_startupChecks:
			pMain->setSAUstate("STARTUP CHKs");
			
			break;
		case Sau7000status_ADuCfrequencyCalibration:
			pMain->setSAUstate("ADuCs FRQ CAL");
			
			break;
		case Sau7000status_bluetoothCheck:
			//m_BTcheckPreviousStatus = BTchk_verify_BR;
			m_BTcheckPreviousStatus = BTchk_idle;
			if(m_firstBtCheck)
			{
				changeBTcheckStatus(BTchk_verify_BR);
				m_firstBtCheck = false;
			}
			else
			{
				changeBTcheckStatus(BTchk_RING);
			}
			//changeBTcheckStatus(BTchk_verify_BR);
			pageInfo->strClear();
			pageMenu->ChangePage(pageInfo);
			break;
		case Sau7000status_checkADuCfwUpgrade:
			pMain->setSAUstate("ADuC UPGRADE");
			
			break;
		case Sau7000status_emgzBoardUpgrade:
			pMain->setSAUstate("EMGZ UPGRADE");

			pageMenu->ChangePage(pageMessages);
			pageMessages->setMessage(0,"To exit EMGZ upgrade");
			pageMessages->setMessage(1,"mode, press ON/OFF");
			pageMessages->setMessage(2,"key..");
			pageMessages->setMessage(3,"-------------------");
			
			break;
		case Sau7000status_BT_byPassMode:
			pMain->setSAUstate("BT BYPASS MODE");

			pageMenu->ChangePage(pageMessages);
			pageMessages->setMessage(0,"To exit BT bypass");
			pageMessages->setMessage(1,"mode, press ON/OFF");
			pageMessages->setMessage(2,"key..");
			pageMessages->setMessage(3,"-------------------");
			break;
		case Sau7000status_switchingOff:
			pMain->setSAUstate("SWITCHING OFF");
			break;
	}
	return 0;
}

byte SAU7000_Manager::changeBTcheckStatus(BTchk_status status)
{
	char str[20];

	if(status != m_BTcheckStatus)
	{
		m_BTcheckPreviousStatus = m_BTcheckStatus;
		m_BTcheckStatus = status;
	}

	if (m_BTcheckStatus == BTchk_wait)
	{
		m_BTwaitStatusTimer.Preset(1500);
	}
	
	m_BTcheckStatus = status;
	sprintf(str, "BT CHECK %d", m_BTcheckStatus);
	pMain->setSAUstate(str);
	
	return 0;
}

/**
@return the current state 
*/
SAU7000_Manager::Sau7000_Status SAU7000_Manager::getSau7000status()
{
	return m_sau7000status;
}

/**
@return the previous state 
*/
SAU7000_Manager::Sau7000_Status SAU7000_Manager::getSau7000previousStatus()
{
	return m_sau7000previousStatus;
}


int SAU7000_Manager::writeToEE_prsCalValue()
{
	int chan, i, k;
	long offset;
	double gain;
	byte crc;
	byte error = 0;
	byte ee_writeContent[EE_SPI_PRS_GAIN_OFFSET_LENGTH];

	for (i = 0, chan = 0; chan < _NUM_OF_PRS_CHAN_; chan++)
	{
		gain = m_prsChan->getGain(chan);
		offset = m_prsChan->getOffset(chan);
		crc = 0;
		for(k = 0; k < sizeof(double); k++)
		{
			ee_writeContent[i++] = ((byte*)&gain)[k];
			crc += ((byte*)&gain)[k];
		}
		for(k = 0; k < sizeof(long); k++)
		{
			ee_writeContent[i++] = ((byte*)&offset)[k];
			crc += ((byte*)&offset)[k];
		}
		ee_writeContent[i++] = crc;
	}
	
	ee_spi_write(EE_SPI_ADD_PRS_GAIN_OFFSET, ee_writeContent, EE_SPI_PRS_GAIN_OFFSET_LENGTH);
	ee_writeContent[0] = EE_SPI_VALID_CAL_STORED;
	ee_spi_write(EE_SPI_ADD_VALID_PRS_CAL_STORED, ee_writeContent, 1);
	
	return readToEE_prsCalValue();
}

int SAU7000_Manager::readToEE_prsCalValue()
{
	int i, k;
	long offset;
	double gain;
	byte crc;
	byte error = 0;
	byte ee_readContent[EE_SPI_PRS_GAIN_OFFSET_LENGTH];

	ee_spi_read(EE_SPI_ADD_VALID_PRS_CAL_STORED, ee_readContent, 1);
	if(ee_readContent[0] != EE_SPI_VALID_CAL_STORED)
		return 100;
	
	ee_spi_read(EE_SPI_ADD_PRS_GAIN_OFFSET, ee_readContent, EE_SPI_PRS_GAIN_OFFSET_LENGTH);
	for(k = 0, i = 0; i < _NUM_OF_PRS_CHAN_; i++)
	{
		crc = 0;
		((byte*)&gain)[0] = ee_readContent[k];
		crc += ee_readContent[k++];
		((byte*)&gain)[1] = ee_readContent[k];
		crc += ee_readContent[k++];
		((byte*)&gain)[2] = ee_readContent[k];
		crc += ee_readContent[k++];
		((byte*)&gain)[3] = ee_readContent[k];
		crc += ee_readContent[k++];
		((byte*)&offset)[0] = ee_readContent[k];
		crc += ee_readContent[k++];
		((byte*)&offset)[1] = ee_readContent[k];
		crc += ee_readContent[k++];
		((byte*)&offset)[2] = ee_readContent[k];
		crc += ee_readContent[k++];
		((byte*)&offset)[3] = ee_readContent[k];
		crc += ee_readContent[k++];

		if(crc == ee_readContent[k++])
		{
			m_prsChan->setGain(i, gain);
			m_prsChan->setOffset(i, offset);
		}else
		{
			error++;
		}
	}
	return error;
}

int SAU7000_Manager::writeToEE_weightCalValue()
{
	int chan, i, k;
	long offset;
	double gain;
	byte crc;
	byte error = 0;
	byte ee_writeContent[EE_SPI_WEIGHT_GAIN_OFFSET_LENGTH];

	for (i = 0, chan = 0; chan < _NUM_OF_WEIGHT_CHAN_; chan++)
	{
		gain = m_weightChan->getGain(chan);
		offset = m_weightChan->getOffset(chan);
		
		crc = 0;
		for(k = 0; k < sizeof(double); k++)
		{
			ee_writeContent[i++] = ((byte*)&gain)[k];
			crc += ((byte*)&gain)[k];
		}
		for(k = 0; k < sizeof(long); k++)
		{
			ee_writeContent[i++] = ((byte*)&offset)[k];
			crc += ((byte*)&offset)[k];
		}
		ee_writeContent[i++] = crc;
	}
	
	ee_spi_write(EE_SPI_ADD_WEIGHT_GAIN_OFFSET, ee_writeContent, EE_SPI_WEIGHT_GAIN_OFFSET_LENGTH);
	ee_writeContent[0] = EE_SPI_VALID_CAL_STORED;
	ee_spi_write(EE_SPI_ADD_VALID_WEIGHT_CAL_STORED, ee_writeContent, 1);
	
	return readToEE_weightCalValue();

}

int SAU7000_Manager::readToEE_weightCalValue()
{
	int i, k;
	long offset;
	double gain;
	byte crc;
	byte error = 0;
	byte ee_readContent[EE_SPI_WEIGHT_GAIN_OFFSET_LENGTH];
	
	ee_spi_read(EE_SPI_ADD_VALID_WEIGHT_CAL_STORED, ee_readContent, 1);
	if(ee_readContent[0] != EE_SPI_VALID_CAL_STORED)
		return 100;
	
	ee_spi_read(EE_SPI_ADD_WEIGHT_GAIN_OFFSET, ee_readContent, EE_SPI_WEIGHT_GAIN_OFFSET_LENGTH);
	for(k = 0, i = 0; i < _NUM_OF_WEIGHT_CHAN_; i++)
	{
		crc = 0;
		((byte*)&gain)[0] = ee_readContent[k];
		crc += ee_readContent[k++];
		((byte*)&gain)[1] = ee_readContent[k];
		crc += ee_readContent[k++];
		((byte*)&gain)[2] = ee_readContent[k];
		crc += ee_readContent[k++];
		((byte*)&gain)[3] = ee_readContent[k];
		crc += ee_readContent[k++];
		((byte*)&offset)[0] = ee_readContent[k];
		crc += ee_readContent[k++];
		((byte*)&offset)[1] = ee_readContent[k];
		crc += ee_readContent[k++];
		((byte*)&offset)[2] = ee_readContent[k];
		crc += ee_readContent[k++];
		((byte*)&offset)[3] = ee_readContent[k];
		crc += ee_readContent[k++];
		
		if(crc == ee_readContent[k++])
		{
			m_weightChan->setGain(i, gain);
			m_weightChan->setOffset(i, offset);
		}else
		{
			error++;
		}
	}
	return error;
}

int SAU7000_Manager::writeToEE_genCalValue(int __chan)
{
	int i, k;
	int addOffset;
	long offset;
	double gain;
	byte crc;
	byte error = 0;
	byte ee_writeContent[EE_SPI_GEN_GAIN_OFFSET_LENGTH];

	if (__chan > MAX_GEN_GAIN_OFFSET)
		return -1;

	addOffset = __chan * 9;

	gain = m_pGenChanGainOffset[__chan]->gain ;
	offset = m_pGenChanGainOffset[__chan]->offset;
	
	crc = 0;
	for(k = 0; k < sizeof(double); k++)
	{
		ee_writeContent[i++] = ((byte*)&gain)[k];
		crc += ((byte*)&gain)[k];
	}
	for(k = 0; k < sizeof(long); k++)
	{
		ee_writeContent[i++] = ((byte*)&offset)[k];
		crc += ((byte*)&offset)[k];
	}
	ee_writeContent[i++] = crc;
	
	ee_spi_write(EE_SPI_ADD_GEN1_GAIN_OFFSET+addOffset, ee_writeContent, EE_SPI_GEN_GAIN_OFFSET_LENGTH);
	
	return readToEE_genCalValue(__chan);
}

int SAU7000_Manager::readToEE_genCalValue(int __chan)
{
	int i, k;
	long offset;
	int addOffset;
	double gain;
	byte crc;
	byte error = 0;
	byte ee_readContent[EE_SPI_GEN_GAIN_OFFSET_LENGTH];

	if (__chan > MAX_GEN_GAIN_OFFSET)
		return -1;

	addOffset = __chan * 9;
	
	ee_spi_read(EE_SPI_ADD_GEN1_GAIN_OFFSET+addOffset, ee_readContent, EE_SPI_GEN_GAIN_OFFSET_LENGTH);
	
	crc = 0;
	((byte*)&gain)[0] = ee_readContent[k];
	crc += ee_readContent[k++];
	((byte*)&gain)[1] = ee_readContent[k];
	crc += ee_readContent[k++];
	((byte*)&gain)[2] = ee_readContent[k];
	crc += ee_readContent[k++];
	((byte*)&gain)[3] = ee_readContent[k];
	crc += ee_readContent[k++];
	((byte*)&offset)[0] = ee_readContent[k];
	crc += ee_readContent[k++];
	((byte*)&offset)[1] = ee_readContent[k];
	crc += ee_readContent[k++];
	((byte*)&offset)[2] = ee_readContent[k];
	crc += ee_readContent[k++];
	((byte*)&offset)[3] = ee_readContent[k];
	crc += ee_readContent[k++];
	
	if(crc == ee_readContent[k++])
	{
		m_pGenChanGainOffset[__chan]->gain = gain;
		m_pGenChanGainOffset[__chan]->offset = offset;
	}else
	{
		m_pGenChanGainOffset[__chan]->gain = 1.0;
		m_pGenChanGainOffset[__chan]->offset = 0;
		error = 1 + __chan;
		
	}
	
	return error;

}


void SAU7000_Manager::stopWth()
{
	if(m_wthSpeed == 0 && m_wthStatus == WthState_stop && m_wthStep == 0)
		return;
	
	m_wthSpeed = 0;
	pMain->setWTHspeed(m_wthSpeed);
	m_wthPowered = 0;
	m_wthStatus = WthState_stop;
	m_wthStep = 0;
	
	m_wthEndSwitchesStatus = WthEndSwitchesStatus_unknown;
	pMain->setLimitSwitch(CMenuMain::lmtsw_notReached, CMenuMain::lmtsw_notReached);
	m_wth->power(false);
	
	m_wth->setEnable(false);

	pMain->setWTHisMoving(m_wth->isEnable());
	m_wth->setLinearVelocity((double)m_wthSpeed);
	m_wth->setStep((double)m_wthStep);
	
	m_sauPcProtocol->sendSauPcWth(m_wthPowered, m_wthSpeed, m_wthStatus, m_wthEndSwitchesStatus, m_wthStep);
}

void SAU7000_Manager::stopWtp()
{
	if(/*m_wtpSpeed == 0 &&*/ m_wtpStatus == WtpState_stop)
		return;
	
	//m_wtpSpeed = 0;
	//m_wtp->setVelocity(m_wtpSpeed);
	//pMain->setWTPspeed(m_wtpSpeed);

	m_wtpStatus = WtpState_stop; 
	if(pMain->getWTPisMoving())
		pMain->setWTPisMoving(false);

	m_wtp->setEnable(false);

	m_sauPcProtocol->sendSauPcWtp(m_wtpSpeed, m_wtpStatus, m_wtpLitStatus, m_wtp->getDriverStatus());

}

void SAU7000_Manager::setOverpressureAdcLimit()
{
	if(m_overpressureChanEn.prs_01)
	{
		m_overpressureLimitAdc[_PRS_01_] = ((float)m_overpressureLimit / m_prsChan->getGain(_PRS_01_)) +  m_prsChan->getOffset(_PRS_01_);
	}else
	{
		m_overpressureChan.prs_01 = 0;
	}

	if(m_overpressureChanEn.prs_02)
	{
		m_overpressureLimitAdc[_PRS_02_] = ((float)m_overpressureLimit / m_prsChan->getGain(_PRS_02_)) +  m_prsChan->getOffset(_PRS_02_);
	}else
	{
		m_overpressureChan.prs_02 = 0;
	}

	if(m_overpressureChanEn.prs_03)
	{
		m_overpressureLimitAdc[_PRS_03_] = ((float)m_overpressureLimit / m_prsChan->getGain(_PRS_03_)) +  m_prsChan->getOffset(_PRS_03_);
	}else
	{
		m_overpressureChan.prs_03 = 0;
	}

	if(m_overpressureChanEn.prs_04)
	{
		m_overpressureLimitAdc[_PRS_04_] = ((float)m_overpressureLimit / m_prsChan->getGain(_PRS_04_)) +  m_prsChan->getOffset(_PRS_04_);
	}else
	{
		m_overpressureChan.prs_04 = 0;
	}

	if(m_overpressureChanEn.prs_05)
	{
		m_overpressureLimitAdc[_PRS_05_] = ((float)m_overpressureLimit / m_prsChan->getGain(_PRS_05_)) +  m_prsChan->getOffset(_PRS_05_);
	}else
	{
		m_overpressureChan.prs_05 = 0;
	}

	if(m_overpressureChanEn.prs_06)
	{
		m_overpressureLimitAdc[_PRS_06_] = ((float)m_overpressureLimit / m_prsChan->getGain(_PRS_06_)) +  m_prsChan->getOffset(_PRS_06_);
	}else
	{
		m_overpressureChan.prs_06 = 0;
	}

	if(m_overpressureChanEn.prs_07)
	{
		m_overpressureLimitAdc[_PRS_07_] = ((float)m_overpressureLimit / m_prsChan->getGain(_PRS_07_)) +  m_prsChan->getOffset(_PRS_07_);
	}else
	{
		m_overpressureChan.prs_07 = 0;
	}

	if(m_overpressureChanEn.prs_08)
	{
		m_overpressureLimitAdc[_PRS_08_] = ((float)m_overpressureLimit / m_prsChan->getGain(_PRS_08_)) +  m_prsChan->getOffset(_PRS_08_);
	}else
	{
		m_overpressureChan.prs_08 = 0;
	}

}
























