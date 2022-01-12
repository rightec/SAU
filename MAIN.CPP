/**
@file		Main.cpp
@brief		SAU-7000 Main file
					
@author		
@date		18/01/2011
@version	01.00
*/


#include "main.h"

/**
Application firmware name. Max length 32 bytes
*/
const char sAppName[]     = "SAU-7000";
/**
Application firmware Revision. Max length 16 bytes
*/
const char sAppRevision[] = "00.01.29";
/**
Board's hardware revision.
*/
const char sHwVersion[] = "01.00.00";
/**
Application firmware's revision date.
*/
const char sAppDate[]     = "23/11/2015";

/**
FPGA's bitstream which has to be loaded into the fpga
*/
const word fpgaWantedBitstream = 0x0006;

/**
sAppName length
*/
const int sAppNameLength = (sizeof(sAppName)/sizeof(char) -1);
/**
sAppRevision length
*/
const int sAppRevisionLength = (sizeof(sAppRevision)/sizeof(char) -1);
/**
sAppName length
*/
const int sHwVersionLength = (sizeof(sHwVersion)/sizeof(char) -1);
/**
sAppDate length
*/
const int sAppDateLength = (sizeof(sAppDate)/sizeof(char) -1);


/**
@see CMenuList
*/
CMenuList *pageMenu;

/**
@see CMenuLogo
*/
CMenuLogo *pLogo;

/**
@see CMenuMain
*/
CMenuMain *pMain;

/**
@see ADuC status page*/ 
CMenuADuCstatus *pADuCstatus;

CMenuMessages *pageMessages;

CMenuSwitchOff *pageSwitchOff;

CMenuInfo *pageInfo;

CMenuADuCfrqCal *pageADuCfrqCal;

/**
Display setting
*/
DisplaySetting displaySetting;



/**
@see GenericHwChanManager
*/
GenericHwChanManager <long, _WEIGHT_BUFFER_LENGTH_> *weightChan;
/**
@see PrsHwChanManager
*/
PrsHwChanManager <short, _PRS_BUFFER_LENGTH_> *prsChan;


/**
High Resolution EMG. 
@see GenericHwChanManager
*/
//GenericHwChanManager <word, _HR_EMG_BUFFER_LENGTH_> *hrEmgChan;

/**
High Resolution EMG electrodes IMP. 
@see GenericHwChanManager
*/
//GenericHwChanManager <word, _HR_EIMP_BUFFER_LENGTH_> *hrEmgImpChan;


/**
@see VoltageHwChanManager
*/
//VoltageHwChanManager <word, _PWR_VOLTAGE_BUFFER_LENGTH_> *powerVoltageChan;
/**
@see CurrentHwChanManager
*/
//CurrentHwChanManager <word, _PWR_CURRENT_BUFFER_LENGTH_> *powerCurrentChan;

/**
@see SauPcProtocol
*/
SauPcProtocol *sauPcProtocol;

/**
@see ADuC_WEIGHT_protocol
*/
ADuC_WEIGHT_protocol *VinfProtocol;

/**
@see ADuC_WEIGHT_protocol
*/
ADuC_WEIGHT_protocol *VVProtocol;

/**
@see ADuC_PRS_protocol
*/
ADuC_PRS_protocol *prsProtocol_12;

/**
@see ADuC_PRS_protocol
*/
ADuC_PRS_protocol *prsProtocol_34;

/**
@see ADuC_PRS_protocol
*/
ADuC_PRS_protocol *prsProtocol_56;

/**
@see ADuC_PRS_protocol
*/
ADuC_PRS_protocol *prsProtocol_78;

/**
@see ADuC_GEN_protocol
*/
ADuC_GEN_protocol *genProtocol;

/**
@see 
*/
SauAndEmgzProtocol *emgProtocol;

iWRAP_BLUEGIGA_SPP *bluetoothProtocol;


/**
@see protocolADuCfwUpgrade
*/
ADuCfwUpgrade_protocol *ADuCfwUpgradeProtocol;

/**
@see Keyboard
*/
Keyboard *myKeyboard;


ADuCUpgradeManger *ADuCUpgradeMng;

/**
@see SAU7000_Manager
*/
SAU7000_Manager *sau7000;




/** uart del microbo
*/
UartNEC *pUART0;
UartNEC *pUART1;
UartNEC *pUART2;

/** uart in pld
*/
UartFPGA *pUART_PC;
UartFPGA *pUART_BLUE;
UartFPGA *pUART_BUBBLE;
UartFPGA *pUART_EMG;
UartFPGA *pUART_ADuC0;
UartFPGA *pUART_ADuC1;
UartFPGA *pUART_ADuC2;
UartFPGA *pUART_ADuC3;
UartFPGA *pUART_ADuC4;
UartFPGA *pUART_ADuC5;
UartFPGA *pUART_ADuC6;
UartFPGA *pUART_ADuC[ADuC_NUM];

UartFPGA *pUART_ADuCupgrade;


Timer_NEC_P024 *timerP2;
NECpwmTimer *pwmTimerP2;
Timer_NEC_P024 *timerP4;
NECpwmTimer *pwmTimerP4;

WthMenfis	*wth;

Timer_NEC_Q *timerQ;
NECpwMeasurentTimer *pwMeasureTimer;
RC5_Standard_IR_protocol *IRprotocol;
IR_Remote	*remoteControl;

SPI_Poller_NEC *spi2;
TMC262_StepperDriver *TMC262_driver;
Timer_NEC_P135 *timerP1;
Timer_NEC_P135 *timerP3;

NECpwmTimer *pwmTimerP1;
WtpMenfis  *wtp;

Adc_NEC_SAU7000 *adcNec;

int powerSamplePeriod;

volatile int milliSec;

gainOffset_t g_pGen1ChanGainOffset[MAX_GEN_GAIN_OFFSET];
gainOffset_t g_pGen2ChanGainOffset[MAX_GEN_GAIN_OFFSET];


//		ce_uart_pc_o     			0x201000
//		ce_uart_blue_o   			0x201100
//		ce_uart_bubble_o 			0x201200
//		ce_uart_emg_o    			0x201300
//		ce_uart_aduc0_o  			0x201400
//		ce_uart_aduc1_o  			0x201500
//		ce_uart_aduc2_o  			0x201600
//		ce_uart_aduc3_o  			0x201700
//		ce_uart_aduc4_o  			0x201800
//		ce_uart_aduc5_o  			0x201900
//		ce_uart_aduc6_o  			0x201A00

/**
global variable to keep the cpu in stop mode
*/
volatile word stop_cpu;


void FlashLed(void);
void hwsw_init(void);
void pin_init(void);
void fpga_pin_init(void);

void KeyboardInit();
void HwChannels_init();
void UartProcol_init();
void page_init();
void wth_Init();
void wtp_Init();
void IR_remoteInit();
void adc_uC_init();
void fpga_config();

static void IrqHandTM0(int vector);

/**
TMQ0 Interrupt Service Routine (ISR).

Hardware timer uses as time base for impedance sampling.
@param vector interrupt vector number.
*/
static void IrqHandINTTP2CC0(int vector);
static void IrqHandINTTP3CC0(int vector);
static void IrqHandINTTP4CC0(int vector);

static void IrqHandINTTQ0CC1(int vector);
static void IrqHandINTTQ0OV(int vector);

static void IrqHandADC(int vector);

static void SetupTimers(void);
void breakpoint(void);

byte Asciihex_to_num (char *pc);
void Num_to_Asciihex (word n, char *pc);

int GrCpuMemTest(word *ram_start, word *ram_end);


void clearAllBuffers();

long timeout;
long msec_test;

/**
Used to jump at the loader start address
*/
void (*fp_m)(void);

int __wait(int a_msTimeDelay)
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
	return 1;
}

void fpga_config()
{
	char str[40];
	int upgrade_result;

	if (FPGA_BITSTREAM_VERSION != fpgaWantedBitstream)
	{
		
		sprintf(str, "LOADED:%d WANTED:%d",FPGA_BITSTREAM_VERSION, fpgaWantedBitstream);
		setMessage(0, "-------------------------");
		setMessage(1, str);
		setMessage(2, "Upgrading fpga..");
		setMessage(3, "-------------------------");
		setMessage(4, "Do NOT switch off the");
		setMessage(5, "power supply in any case.");
		setMessage(6, "The upgrade process can");
		setMessage(7, "take up to 2 minutes!");
		setMessage(8, "-------------------------");
		setMessage(9, "-------------------------");

		display_update();
		__wait(5000);
		
		upgrade_result = SSPIE_main();
		
		display_update();
		__wait(3000);
		
		if (upgrade_result > 0)
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
		}else
		{
			setMessage(0, "-------------------------");
			setMessage(1, "-------------------------");
			setMessage(2, "-------------------------");
			setMessage(3, "-------------------------");
			setMessage(4, "------   UPGRADE   ------");
			setMessage(5, "------   FAILED!!  ------");
			setMessage(6, "-------------------------");
			setMessage(7, "-------------------------");
			setMessage(8, "-------------------------");
			setMessage(9, "-------------------------");		
			display_update();
			while(1);
		}

		//---- Toggling the PROGRAMN pin to transfer the Flash data to SRAM ---
		PinFPGA_PROG = 0;
		ioPinFPGA_PROG = 0;
		__wait(200);
		PinFPGA_PROG = 1;
		__wait(200);
		ioPinFPGA_PROG = 1;
		//---------------------------------------------------------------------

		__wait(5000);
		while (1)
		{
			PinEN_POWER = 1;
			PinEN_POWER = 1;
		}

		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		fp_m = (void (*)()) LOADER_FLASH_START_ADDRESS;				// inizio dell'applicone
	    (*fp_m)();	
	}
}


int	main()
{
DecTimer wait;

	hwsw_init();
	SetupTimers();
	page_init();

	milliSec = 0;
	asm	( "ei" );	// start a tutte le	interruzioni

	wait.Preset(200);
	timeout = 100000;

	//ADuCUpgradeMng->hwReset();

	while(1)
	{

		if( timeout )
			timeout--;
		else
			break;
		
		if( wait.Match() )
			break;

	}
	wait.Stop();


	//---- FPGA ---
	asm("di");
	fpga_config();
	asm	( "ei" );
	
	//---------------------------------------------------
	goBuzzer(LONG_LIKE);

	wait.Preset(2000);
	
	PinLED_LIFE = LED_LIFE_OFF;

	VinfProtocol->TurnOn();
	VVProtocol->TurnOn();
	prsProtocol_12->TurnOn();
	prsProtocol_34->TurnOn();
	prsProtocol_56->TurnOn();
	prsProtocol_78->TurnOn();
	genProtocol->TurnOn();
	emgProtocol->TurnOn();

	bluetoothProtocol->TurnOn();
	sauPcProtocol->TurnOn();
	
	while( 1 )
	{
		FlashLed();

		buzzerManager();

		if( sau7000 )
			sau7000->manager();

		pageMenu->Entry();


		#ifdef __APP_IN_RAM__
			breakpoint();
		#endif

	}

}


/* This function will generate a breakpoint exception.  It is used at the
   beginning of a program to sync up with a debugger and can be used
   otherwise as a quick means to stop program execution and "break" into
   the debugger. */

#define BREAKPOINT() asm volatile ("	trap 1");

void breakpoint(void)
{
	if( timeout < 10000 )
	{
		timeout++;
	}
	else
	{
		timeout = 0;

	}

		// purtoppo con il nuovo gdb non parte il segnale di break
		// e non si ferma l'esecuzione, pork !!
		// commo mai ?
	if( _CB4RIF )		// UA0RIF
	{
	//if( !PinKEY || timeout == 0 )
		BREAKPOINT();
	}
}

/**
Flash board's led
*/
void FlashLed(void)
{
static long	blink;

	blink++;

	if(	blink == 40 )
	{
		PinLED_LIFE = !PinLED_LIFE;
	}

	if(	blink == 80 )
	{
		PinLED_LIFE = !PinLED_LIFE;
	}

	if( blink == 120 )
	{
		PinLED_LIFE = !PinLED_LIFE;
	}					

	if( blink == 160 )
	{
		PinLED_LIFE = !PinLED_LIFE;
		blink = 0;
	}

	
}

/**
Hardware and software initialization
*/
void hwsw_init(void)
{
byte displayValid = 0;

	asm	( "di" );			// stop	a tutte	le interruzioni

	pin_init();				// init	pins. locali
	fpga_pin_init();
	mem_init();
	irq_init();
	error_init();            
	sio_init_ch0();
	sio_init_ch1();
	sio_init_ch2();
	buzzer_init();
	KeyboardInit();
	display_init(0,false);
	init_eeprom_spi();

	ee_spi_read(EE_SPI_ADD_VALID_DISPLAY_SETTING_LOADED, &displayValid, 1);
	
	if(displayValid == EE_SPI_VALID_DISPLAY_SETTING_LOADED)
	{
		ee_spi_read(EE_SPI_ADD_DISPLAY_SETTING_BACKLIGHT, &(displaySetting.backlight), 1);
		ee_spi_read(EE_SPI_ADD_DISPLAY_SETTING_BRIGTHNESS, &(displaySetting.brightness), 1);
		ee_spi_read(EE_SPI_ADD_DISPLAY_SETTING_CONTRAST, &(displaySetting.contrast), 1);
	}
	else
	{
		displaySetting.backlight = ON;
		displaySetting.brightness = 100;
		displaySetting.contrast = 22;
		displaySetting.writeSettingToEEPROM = false;
	}
	
	display_brightness(displaySetting.brightness);
	display_contrast(displaySetting.contrast);

	charset_init();							// init set di caratteri
	
	charset_register(&pc8x16CharSet);
	charset_register(&xm8x13bCharSet);
	charset_register(&pc6x8CharSet);
	charset_register(&xm6x10CharSet);
	charset_register(&xm5x6CharSet);

	HwChannels_init();
	
	wth_Init();
	wtp_Init();
	IR_remoteInit();
	
	UartProcol_init();
	adc_uC_init();
	
	ADuCUpgradeMng = new ADuCUpgradeManger(ADuCfwUpgradeProtocol);
	ADuCUpgradeMng->setUpgradeFile((byte*)ADuC_Source, (unsigned int)ADuC_Source_size);
	
	sau7000 = new SAU7000_Manager(myKeyboard, sauPcProtocol);
	
	sau7000->setWeightChan(weightChan);
	sau7000->setPrsChan(prsChan);
	sau7000->setGenChanGainAndOffset(g_pGen1ChanGainOffset, g_pGen2ChanGainOffset);

	sau7000->setAdcNec(adcNec);
	
	sau7000->setVinfProtocol(VinfProtocol);
	sau7000->setVVProtocol(VVProtocol);
	sau7000->setP12Protocol(prsProtocol_12);
	sau7000->setP34Protocol(prsProtocol_34);
	sau7000->setP56Protocol(prsProtocol_56);
	sau7000->setP78Protocol(prsProtocol_78);
	sau7000->setGenProtocol(genProtocol);
	
	sau7000->setEmgProtocol(emgProtocol);

	sau7000->setADuCUpgradeManger(ADuCUpgradeMng);

	sau7000->setBluetoothProtocol(bluetoothProtocol);
	sau7000->setWth(wth);

	sau7000->setWtp(wtp);

	sau7000->setIR_remote(remoteControl);
}

/**
Harware channels initialization
*/
void HwChannels_init()
{
	int i;
	//--- Weight Channels ---
	weightChan = new GenericHwChanManager <long, _WEIGHT_BUFFER_LENGTH_> (_NUM_OF_WEIGHT_CHAN_, _HW_CHAN_VINF_TYPE_, _24_BITS_RESOLUTION_, 10);
	for (i = 0; i < _NUM_OF_WEIGHT_CHAN_; i++)
	{
		weightChan->setEnabled(i, true);
	}
	weightChan->setChanType(_VINF_CHAN_, _HW_CHAN_VINF_TYPE_);
	weightChan->setChanType(_VV_CHAN_, _HW_CHAN_VV_TYPE_);

	//--- Prs Channels ---
	prsChan = new PrsHwChanManager <short, _PRS_BUFFER_LENGTH_> (_NUM_OF_PRS_CHAN_, _16_BITS_RESOLUTION_, 100);
	for (i = 0; i < _NUM_OF_PRS_CHAN_; i++)
	{
		prsChan->setEnabled(i, true);
	}
	
	//--- Generic Channels ---
	/*genericChan = new GenericHwChanManager <word, _GENERIC_BUFFER_LENGTH_> (_NUM_OF_GENERIC_CHAN_ , _HW_CHAN_GENERIC_TYPE_, _16_BITS_RESOLUTION_, 1000);
	for (i = 0; i < _NUM_OF_GENERIC_CHAN_; i++)
	{
		genericChan->setEnabled(i, true);
	}
*/
	//--- EMG Channel ---
	/*
	hrEmgChan = new GenericHwChanManager <word, _HR_EMG_BUFFER_LENGTH_> (_MAX_HR_EMG_CHAN_ , _HW_CHAN_EMG_TYPE_, _16_BITS_RESOLUTION_, 6000);
	for (i = 0; i < _MAX_HR_EMG_CHAN_; i++)
	{
		hrEmgChan->setEnabled(i, true);
	}

	hrEmgImpChan = new GenericHwChanManager <word, _HR_EIMP_BUFFER_LENGTH_> (_MAX_HR_EIMP_CHAN_ , _HW_CHAN_IMP_TYPE_, _10_BITS_RESOLUTION_, 1);
	for (i = 0; i < _MAX_HR_EIMP_CHAN_; i++)
	{
		hrEmgImpChan->setEnabled(i, true);
	}
	*/
	/*//--- power Voltage channel ---
	powerVoltageChan = new VoltageHwChanManager <word, _PWR_VOLTAGE_BUFFER_LENGTH_> (_NUM_VOLTAGE_MEASURE_, _10_BITS_RESOLUTION_, 1);
	powerVoltageChan->setVoltageRefernce_mV(_INTERNAL_ADC_REF_mV_);
	for(i = 0; i < _NUM_VOLTAGE_MEASURE_; i++)
	{
		powerVoltageChan->setIsZeroable(i, false);
		powerVoltageChan->setOffset(i, 0);
		powerVoltageChan->setGain(i, (powerVoltageChan->getVoltageRefence_mV()/powerVoltageChan->getAdcResolution()) * (5.0/3.0));
		powerVoltageChan->setEnabled(i, true);
	}	
	//--- power Current channel  ---
	powerCurrentChan = new CurrentHwChanManager <word, _PWR_CURRENT_BUFFER_LENGTH_> (1, _10_BITS_RESOLUTION_, 1);
	powerCurrentChan->setVoltageRefernce_mV(_INTERNAL_ADC_REF_mV_);
	powerCurrentChan->setIsZeroable(0, false);
	powerCurrentChan->setOffset(0, 0);
	powerCurrentChan->setGain(0, (powerCurrentChan->getVoltageRefence_mV()/powerCurrentChan->getAdcResolution()) * (20.0/604.0)/0.12);
	powerCurrentChan->setEnabled(0, true);*/
}

/**
Keyboard initialization
*/
void KeyboardInit()
{
	int i;
	myKeyboard = new Keyboard(KEY_NUM_OF_KEYS);
	
	i = 0;
	myKeyboard->setKeyPin(i, NULL, 0);
	myKeyboard->setKeyName(i, NO_KEY);
	i++;

	myKeyboard->setKeyPin(i, &PLD_GEN_REG->KEY, 0);
	myKeyboard->setKeyName(i, KEY_ON_OFF);
	i++;

	for( i = NO_KEY + 1 ; i < KEY_NUM_OF_KEYS; i++)
	{
		myKeyboard->setEnable(i, true, 200);
		myKeyboard->setBeepEnable(i, true);
		myKeyboard->setCountinuousReading(true, 200);
	}

}

/**
Uart communication protocol initialization
*/
void UartProcol_init()
{
		// prima inizializza un po' tutte le uart

		/*

						// uart0 -> usata per debug tramite rom-monitor, non toccare
	pUART0 = new UartNEC((volatile uart_nec_t *) &UA0CTL0, UartDeviceAbstract::ParityNone, 	
									UartDeviceAbstract::Frame8Bits, 
										UartDeviceAbstract::Stop1Bits, 
										UartDeviceAbstract::BR38400,
										250, 		// rx buffer size
										250) ;		// tx buffer size
		

						// uart1 -> usata per comunicazione con modulo zigbee o bluetooth
	pUART1 = new UartNEC((volatile uart_nec_t *) &UA1CTL0, UartDeviceAbstract::ParityNone, 	
									UartDeviceAbstract::Frame8Bits, 
										UartDeviceAbstract::Stop1Bits, 
										UartDeviceAbstract::BR38400,
										250, 		// rx buffer size
										250) ;		// tx buffer size

						// uart1 -> usata per comunicazione con modulo estrattore esterno
	pUART2 = new UartNEC((volatile uart_nec_t *) &UA2CTL0, UartDeviceAbstract::ParityNone, 	
									UartDeviceAbstract::Frame8Bits, 
										UartDeviceAbstract::Stop1Bits, 
										UartDeviceAbstract::BR38400,
										250, 		// rx buffer size
										250) ;		// tx buffer size
*/

	pUART_PC = new UartFPGA(PLD_UART_PC, UartDeviceAbstract::ParityNone, 	
										UartDeviceAbstract::Frame8Bits, 
										UartDeviceAbstract::Stop1Bits, 
										UartDeviceAbstract::BR460800,
										UartDeviceAbstract::hwFlwCtl_CtsRts,
										1000, 		// rx buffer size
										40000) ;		// tx buffer size

	pUART_BLUE = new UartFPGA(PLD_UART_BLUE, UartDeviceAbstract::ParityNone, 	
										UartDeviceAbstract::Frame8Bits, 
										UartDeviceAbstract::Stop1Bits, 
										UartDeviceAbstract::BR460800,
										UartDeviceAbstract::hwFlwCtl_CtsRts,
										1000, 		// rx buffer size
										40000) ;	// tx buffer size
										
	pUART_ADuC0 = new UartFPGA(PLD_UART_ADUC0, UartDeviceAbstract::ParityNone, 	
										UartDeviceAbstract::Frame8Bits, 
										UartDeviceAbstract::Stop1Bits, 
										UartDeviceAbstract::BR57600,
										UartDeviceAbstract::hwFlwCtl_None,
										2000, 		// rx buffer size
										300) ;		// tx buffer size -- if smaller upgrade can not be performed


	pUART_ADuC1 = new UartFPGA(PLD_UART_ADUC1, UartDeviceAbstract::ParityNone, 	
										UartDeviceAbstract::Frame8Bits, 
										UartDeviceAbstract::Stop1Bits, 
										UartDeviceAbstract::BR57600,
										UartDeviceAbstract::hwFlwCtl_None,
										2000, 		// rx buffer size
										300) ;		// tx buffer size


	pUART_ADuC2 = new UartFPGA(PLD_UART_ADUC2, UartDeviceAbstract::ParityNone, 	
										UartDeviceAbstract::Frame8Bits, 
										UartDeviceAbstract::Stop1Bits, 
										UartDeviceAbstract::BR57600,
										UartDeviceAbstract::hwFlwCtl_None,
										5000, 		// rx buffer size
										300) ;		// tx buffer size

	pUART_ADuC3 = new UartFPGA(PLD_UART_ADUC3, UartDeviceAbstract::ParityNone, 	
										UartDeviceAbstract::Frame8Bits, 
										UartDeviceAbstract::Stop1Bits, 
										UartDeviceAbstract::BR57600,
										UartDeviceAbstract::hwFlwCtl_None,
										2000, 		// rx buffer size
										300) ;		// tx buffer size

	pUART_ADuC4 = new UartFPGA(PLD_UART_ADUC4, UartDeviceAbstract::ParityNone, 	
										UartDeviceAbstract::Frame8Bits, 
										UartDeviceAbstract::Stop1Bits, 
										UartDeviceAbstract::BR57600,
										UartDeviceAbstract::hwFlwCtl_None,
										2000, 		// rx buffer size
										300) ;		// tx buffer size

	pUART_ADuC5 = new UartFPGA(PLD_UART_ADUC5, UartDeviceAbstract::ParityNone, 	
										UartDeviceAbstract::Frame8Bits, 
										UartDeviceAbstract::Stop1Bits, 
										UartDeviceAbstract::BR57600,
										UartDeviceAbstract::hwFlwCtl_None,
										2000, 		// rx buffer size
										300) ;		// tx buffer size

	pUART_ADuC6 = new UartFPGA(PLD_UART_ADUC6, UartDeviceAbstract::ParityNone, 	
										UartDeviceAbstract::Frame8Bits, 
										UartDeviceAbstract::Stop1Bits, 
										UartDeviceAbstract::BR57600,
										UartDeviceAbstract::hwFlwCtl_None,
										2000, 		// rx buffer size
										300) ;		// tx buffer size


	pUART_EMG = new UartFPGA(PLD_UART_EMG, UartDeviceAbstract::ParityNone, 	
										UartDeviceAbstract::Frame8Bits, 
										UartDeviceAbstract::Stop1Bits, 
										UartDeviceAbstract::BR230400,
										UartDeviceAbstract::hwFlwCtl_CtsRts,
										10000, 		// rx buffer size
										300) ;		// tx buffer size

										
	pUART_ADuC[ADuC_VINF] = pUART_ADuC0;
	pUART_ADuC[ADuC_VV] = pUART_ADuC1;
	/*pUART_ADuC[ADuC_P12] = pUART_ADuC5;
	pUART_ADuC[ADuC_P34] = pUART_ADuC6;
	pUART_ADuC[ADuC_P56] = pUART_ADuC3;
	pUART_ADuC[ADuC_P78] = pUART_ADuC4;
	*/
	pUART_ADuC[ADuC_P12] = pUART_ADuC3;
	pUART_ADuC[ADuC_P34] = pUART_ADuC4;
	pUART_ADuC[ADuC_P56] = pUART_ADuC5;
	pUART_ADuC[ADuC_P78] = pUART_ADuC6;
	
	pUART_ADuC[ADuC_GEN] = pUART_ADuC2;
	pUART_ADuCupgrade = NULL;
	
	sauPcProtocol = new SauPcProtocol(pUART_PC);
	sauPcProtocol->setWeightChan(weightChan);
	sauPcProtocol->setPrsChan(prsChan);
	sauPcProtocol->setGenChanGainAndOffset(g_pGen1ChanGainOffset, g_pGen2ChanGainOffset);
	
	VinfProtocol = NULL;
	VVProtocol = NULL;
	prsProtocol_12 = NULL;
	prsProtocol_34 = NULL;
	prsProtocol_56 = NULL;
	prsProtocol_78 = NULL;
	genProtocol = NULL;
	emgProtocol = NULL;
	
	VinfProtocol = new ADuC_WEIGHT_protocol(pUART_ADuC[ADuC_VINF]);
	VinfProtocol->TurnOff();
	VinfProtocol->setWeightChan(weightChan, _VINF_CHAN_);
	VinfProtocol->registerClearAllBuffFunct(&clearAllBuffers);
	
	VVProtocol = new ADuC_WEIGHT_protocol(pUART_ADuC[ADuC_VV]);
	VVProtocol->TurnOff();
	VVProtocol->setWeightChan(weightChan, _VV_CHAN_);
	VVProtocol->registerClearAllBuffFunct(&clearAllBuffers);
	
	prsProtocol_12 = new ADuC_PRS_protocol(pUART_ADuC[ADuC_P12]);
	prsProtocol_12->TurnOff();
	prsProtocol_12->setPrsChan(prsChan, _PRS_01_, _PRS_02_);
	prsProtocol_12->registerClearAllBuffFunct(&clearAllBuffers);
	
	prsProtocol_34 = new ADuC_PRS_protocol(pUART_ADuC[ADuC_P34]);
	prsProtocol_34->TurnOff();
	prsProtocol_34->setPrsChan(prsChan, _PRS_03_, _PRS_04_);
	prsProtocol_34->registerClearAllBuffFunct(&clearAllBuffers);
	
	prsProtocol_56 = new ADuC_PRS_protocol(pUART_ADuC[ADuC_P56]);
	prsProtocol_56->TurnOff();
	prsProtocol_56->setPrsChan(prsChan, _PRS_05_, _PRS_06_);
	prsProtocol_56->registerClearAllBuffFunct(&clearAllBuffers);
	
	prsProtocol_78 = new ADuC_PRS_protocol(pUART_ADuC[ADuC_P78]);
	prsProtocol_78->TurnOff();
	prsProtocol_78->setPrsChan(prsChan, _PRS_07_, _PRS_08_);
	prsProtocol_78->registerClearAllBuffFunct(&clearAllBuffers);
	

	genProtocol = new ADuC_GEN_protocol(pUART_ADuC[ADuC_GEN]);
	genProtocol->TurnOff();
	genProtocol->registerClearAllBuffFunct(&clearAllBuffers);
	
	ADuCfwUpgradeProtocol = new ADuCfwUpgrade_protocol(pUART_ADuCupgrade);
	
	emgProtocol = new SauAndEmgzProtocol(pUART_EMG, 8000);
	emgProtocol->TurnOff();
	emgProtocol->registerClearAllBuffFunct(&clearAllBuffers);

	bluetoothProtocol = new iWRAP_BLUEGIGA_SPP(pUART_BLUE, 1000);

	emgProtocol->setProtocolPc(sauPcProtocol);
	genProtocol->setProtocolPc(sauPcProtocol);
	//bluetoothProtocol->setBypassMode(true, pUART_PC);
	//sauPcProtocol->setBypassMode(true, pUART_BLUE);

	__wait(10);
	
	PLD_JTAG_ITF->JTAG_OR = 0x02;	// su tutto meno reset 

	__wait(300);

	PLD_JTAG_ITF->JTAG_OR = 0x03;	// su tutto e pure il reset 

	__wait(100);
}

/**
Video page initialization
*/
void page_init()
{
	pageMenu = new CMenuList();
	pLogo = new CMenuLogo();
	pMain = new CMenuMain();
	pADuCstatus = new CMenuADuCstatus();
	pageMessages = new CMenuMessages();
	pageSwitchOff = new CMenuSwitchOff();
	pageInfo = new CMenuInfo();
	pageADuCfrqCal = new CMenuADuCfrqCal();
	
	#ifdef __FORCE_UPGRADE_ADuC__
		pageMenu->ChangePage(pADuCstatus);
	#else
		pageMenu->ChangePage(pLogo);
	#endif
}

void wth_Init()
{
	timerP2 = new Timer_NEC_P024((volatile NEC_timerP_t*) &TP2CTL0, (volatile NEC_timerP_Int_t*) &TP2OVIC);
	pwmTimerP2 = new NECpwmTimer(timerP2);
	pwmTimerP2->setOutput_01(Timer_NEC::TxnOE1_outputEnable, Timer_NEC::TOPn1_startLow);
	pwmTimerP2->setPwmFrq(1000);
	pwmTimerP2->setPwmTonDuty(50);
	pwmTimerP2->setEnable(false);

	//irq_setVect(IRQ_VEC_INTTP2CC0, IrqHandINTTP2CC0);

	//----- TP4 ---
	timerP4 = new Timer_NEC_P024((volatile NEC_timerP_t*) &TP4CTL0, (volatile NEC_timerP_Int_t*) &TP4OVIC);
	
	pwmTimerP4 = new NECpwmTimer(timerP4);
	pwmTimerP4->setPwmFrq(1000);
	pwmTimerP4->setPwmTonDuty(50);
	pwmTimerP4->setEnable(false);
	
	irq_setVect(IRQ_VEC_INTTP4CC0, IrqHandINTTP4CC0);
	
	
	wth = new WthMenfis(pwmTimerP2, pwmTimerP4);
	
	//--- port settings ---
	_PMC96 = 1;
	_PFCE96 = 1;
	_PFC96 = 1;

	
}

void wtp_Init()
{
	spi2 = new SPI_Poller_NEC((volatile spi_nec_t*)&CB2CTL0);

	TMC262_driver = new TMC262_StepperDriver(spi2);

	timerP1 = new Timer_NEC_P135((volatile NEC_timerP_t*) &TP1CTL0, (volatile NEC_timerP_Int_t*) &TP1OVIC);
	pwmTimerP1 = new NECpwmTimer(timerP1);
	pwmTimerP1->setOutput_01(Timer_NEC::TxnOE1_outputEnable, Timer_NEC::TOPn1_startLow);
	_PMC35 = 1;
	_PFC35 = 1;

	timerP3 = new Timer_NEC_P135((volatile NEC_timerP_t*) &TP3CTL0, (volatile NEC_timerP_Int_t*) &TP3OVIC);
	irq_setVect(IRQ_VEC_INTTP3CC0,IrqHandINTTP3CC0);
	timerP3->setCCIC0_interrupt(true, 0x06);
	
	wtp = new WtpMenfis(pwmTimerP1, TMC262_driver, timerP3);

}


void IR_remoteInit()
{
	int i;

	timerQ = new Timer_NEC_Q((volatile NEC_timerQ_t*) &TQ0CTL0, (volatile NEC_timerQ_Int_t*) &TQ0OVIC);
	timerQ->setOVIC_interrupt(true, 4);//TQ0OVIC = 0x04;
	timerQ->setCCIC1_interrupt(true, 4);//TQ0CCIC1 = 0x04;
	irq_setVect(IRQ_VEC_INTTQ0CC1, IrqHandINTTQ0CC1);
	irq_setVect(IRQ_VEC_INTTQ0OV, IrqHandINTTQ0OV);

	timerQ->setValidEdgeCaptureTrigTIxn1(Timer_NEC::vectTIxn1_bothEdges, true);
	pwMeasureTimer = new NECpwMeasurentTimer(timerQ, Timer_NEC::_CCR1);
	pwMeasureTimer->setTimeBase(1, pwMeasurementAbstract::tu_usec);

	//--- port setting ---
	ioPinIR_RX = 1;
	asm("nop");
	asm("nop");
	asm("nop");
	pwMeasureTimer->setMeasurementState((pwMeasurementAbstract::MeasurementState)PinIR_RX);
		
	_PMC50 = 1;
	_PFCE50 = 0;
	_PFC50 = 1;
	
	pwMeasureTimer->setEnable(true);

	IRprotocol = new RC5_Standard_IR_protocol(pwMeasureTimer);

	remoteControl = new IR_Remote(IRk_numOfKeys, IRprotocol);

	for( i = 0 ; i < IRk_numOfKeys; i++)
	{
		remoteControl->setEnable(i, true);
		remoteControl->setBeepEnable(i, true);
		remoteControl->setCountinuousReading(i, false);
	}
	remoteControl->setKeyName(0, IRk_1);
	remoteControl->setKeyName(1, IRk_2);
	remoteControl->setKeyName(2, IRk_3);
	remoteControl->setKeyName(3, IRk_4);
	remoteControl->setKeyName(4, IRk_5);
	remoteControl->setKeyName(5, IRk_6);
	remoteControl->setKeyName(6, IRk_7);
	remoteControl->setKeyName(7, IRk_8);
	remoteControl->setKeyName(8, IRk_9);	

	remoteControl->setKeyName(9, IRk_10);
	remoteControl->setKeyName(10, IRk_11);
	remoteControl->setKeyName(11, IRk_12);
	remoteControl->setKeyName(12, IRk_13);
	remoteControl->setKeyName(13, IRk_14);
	remoteControl->setKeyName(14, IRk_15);
	remoteControl->setKeyName(15, IRk_16);
	remoteControl->setKeyName(16, IRk_17);
	remoteControl->setKeyName(17, IRk_18);
	remoteControl->setKeyName(18, IRk_19);

	remoteControl->setKeyName(19, IRk_20);
	remoteControl->setKeyName(20, IRk_21);
	remoteControl->setKeyName(21, IRk_22);
	remoteControl->setKeyName(22, IRk_23);
	remoteControl->setKeyName(23, IRk_24);
	remoteControl->setKeyName(24, IRk_25);
	remoteControl->setKeyName(25, IRk_26);
	remoteControl->setKeyName(26, IRk_27);
	remoteControl->setKeyName(27, IRk_28);
	remoteControl->setKeyName(28, IRk_29);

	remoteControl->setKeyName(29, IRk_30);
	remoteControl->setKeyName(30, IRk_31);
	remoteControl->setKeyName(31, IRk_32);
	remoteControl->setKeyName(32, IRk_33);
	remoteControl->setKeyName(33, IRk_34);
	remoteControl->setKeyName(34, IRk_35);
	remoteControl->setKeyName(35, IRk_36);
	remoteControl->setKeyName(36, IRk_37);
	remoteControl->setKeyName(37, IRk_38);
	remoteControl->setKeyName(38, IRk_39);

	remoteControl->setKeyName(39, IRk_40);
	remoteControl->setKeyName(40, IRk_41);
	remoteControl->setKeyName(41, IRk_42);
	remoteControl->setKeyName(42, IRk_43);

	remoteControl->setKeyName(43, IRk_44);
	remoteControl->setKeyName(44, IRk_45);
	remoteControl->setKeyName(45, IRk_46);
	remoteControl->setKeyName(46, IRk_47);
	remoteControl->setKeyName(47, IRk_48);
	
	remoteControl->setCountinuousReading(1, true, 400);
}

void adc_uC_init()
{
	int i;
	
	adcNec = new Adc_NEC_SAU7000((volatile adc_nec_t *)&ADA0M0, (volatile adcConv_nec_t*)&ADA0CR0, (volatile adc_nec_interrupt_t*)&ADIC);

	irq_setVect(IRQ_VEC_INTAD, IrqHandADC);
	adcNec->setADIC_interrupt(true, 7);
	
	powerSamplePeriod = 100;

	adcNec->setEnable(true);
}

/**
uC pin initialization.
*/
void pin_init(void)
{
	PinLED_LIFE = LED_LIFE_OFF;
	ioPinLED_LIFE = 0;

	//-- LCD defines --
	PinLCD_RST	= 0;
	ioPinLCD_RST = 0;

	PinLCD_CS1 = 1;
	ioPinLCD_CS1 = 0;

	PinLCD_A0 = 0;
	ioPinLCD_A0 = 0;

	PinLCD_WR	= 1;
	ioPinLCD_WR	= 0;

	PinLCD_RD	= 1;
	ioPinLCD_RD	= 0;

	Pin_EN_LCD_LED = 1;
	ioPin_EN_LCD_LED = 0;

	//-- EEPROM defines --
	PinCSeeprom = 1; 
	ioPinCSeeprom = 0;

	//-- Power enable Pin --
	PinEN_POWER	= 0;
	ioPinEN_POWER = 0;
	
	//-- Peristaltic Pump (M1) defines 
	PinEN_M1 = 0;
	ioPinEN_M1 = 0;
	
	PinDIR_M1	= 0;
	ioPinDIR_M1 = 0;
	
	PinSTEP_M1 = 0;
	ioPinSTEP_M1 = 0;

	PinSPI_M1_CS = 1;
	ioPinSPI_M1_CS = 0;

	PinHALL1_M1 = 0;
	ioPinHALL1_M1 = 1;

	PinHALL2_M1 = 0;				
	ioPinHALL2_M1 = 1;


	//-- Puller (M2) defines --
	PinEN_M2 = 0;
	ioPinEN_M2 = 0;

	PinDIR_M2 = 0;
	ioPinDIR_M2	= 0;

 	PinSTEP_M2 = 0;
 	ioPinSTEP_M2 = 0;

 	PinHALL1_M2	= 0;
 	ioPinHALL1_M2 = 1;

 	PinHALL2_M2	= 0;
 	ioPinHALL2_M2 = 1;

	//-- Remote control (IR) Pin --
	PinIR_RX = 0;
	ioPinIR_RX = 1;

	//-- Buzzer --
	PinBUZZER = 0;
	ioPinBUZZER = 1;

	//-- SPI FPGA --
	PinCS_SPI_FPGA = 1;
	ioPinCS_SPI_FPGA = 0;

	//-- FPGA programming pin --
	PinFPGA_PROG = 0;
	ioPinFPGA_PROG = 1;

	//-- FPGA done pin --
	PinFPGA_DONE = 0;
	ioPinFPGA_DONE = 1;
}

/**
fpga pin initialization.
*/
void fpga_pin_init(void)
{
		//////////////////////////////////////////////////////////////
					// inizializza l'interfaccina per il JTAG
	//PLD_JTAG_ITF->JTAG_OR = 0x00; 	// giu' reset e ntrst per microbi aduc

	//__wait(10);
	PLD_JTAG_ITF->JTAG_OR = 0x02;	// su tutto meno reset
	
	//////////////////////////////////////////////////////////////
	// inizializza i registri in pld

					
	PinLED_LIFE_PLD	= 1;
	PinLED1	= 1;
	PinLED2	= 1;

	PinTXE_BUBBLE = 1; //Disable bubble air test

}

/**
Timer initialization
*/
void SetupTimers(void)
{
	/*
	TM0 -> time base 
	TQ0 -> remote control
	TP0 -> LCD backlight
	TP1 -> wtp
	TP2 -> wth
	TP3->  wtp acceleration ramp
	TP4 -> wth step count timer
	TP5 -> buzzer
	*/
	//---------- TMM0 --------
	irq_setVect(IRQ_VEC_INTTM0EQ0, IrqHandTM0);


	//-- Counter frequency	--> fc=fxx/4
	//-- Bits in TM0CTL0 register
	_TM0CE = 0;
	TM0CTL0 = 0x02;

	TM0CMP0 = 4999; // Frequenza di interrupt --> fi =	fc / (TM0CCR0 + 1)

	TM0EQIC0 = 0x05; // Interrupt abilitato con priorità 5

	_TM0CE = 1; // TMP0 start

	//----- TP4 ---
	/*timerP4 = new Timer_NEC_P024((volatile NEC_timerP_t*) &TP4CTL0, (volatile NEC_timerP_Int_t*) &TP4OVIC);
	timerP4->setTimerMode(Timer_NEC_P::tm_IntervalTimerMode, true);
	timerP4->setFrequency(1500 ,Timer_NEC::_CCR0);
	timerP4->setCCIC0_interrupt(true, 0x01);
	irq_setVect(IRQ_VEC_INTTP4CC0, IrqHandINTTP4CC0);
	timerP4->setEnable(true);*/
}

void clearAllBuffers()
{
	weightChan->clearFifo_isr();
	prsChan->clearFifo_isr();
}

void irq_INTUA0R_INTCB4R(void)
{
byte c;
byte e;

	c = UA0RX;
	e = UA0STR;

	if( pUART0 != NULL )
	{

			// framing error o overrun error ?
			// il data register non contiene dati nuovi 
		if( (e & (UART_STR_OVE | UART_STR_FE)) != 0 )
		{
			if( (e & UART_STR_OVE) != 0 )
				pUART0->SetReceptionError(UartDeviceAbstract::OverrunError);

			if( (e & UART_STR_FE) != 0 )
				pUART0->SetReceptionError(UartDeviceAbstract::FrameError);
		}
		else
		{
			if( (e & UART_STR_PE) != 0 )
				pUART0->SetReceptionError(UartDeviceAbstract::ParityError);
			else
				pUART0->isr_rxPut(c);	// non controlla lo sforamento ..(overflow)
		}

		
	}

	UA0STR &= 0xf8;		// reset errors

}

void irq_INTUA0T_INTCB4T(void)
{
	if( pUART0 != NULL )
	{
		if( pUART0->isr_txAvail() )
			UA0TX = pUART0->isr_txGet();
		else
			UA0TIC |= 0x40;		// stop interruzioni
	}
}

/**
ISR used as time base for the uC sofware time. Interrupt time 1 msec.
*/
void IrqHandTM0(int vector)
{
	if(sau7000->bubbleDetectionEn())
	{
		if((PinCMP_BUBBLE == 0) && !sau7000->bubbleAirTestRunning())
		{
			sau7000->airDetected();
			sau7000->stopWtp();
			sau7000->stopWth();
			PinCMD_PINZA = 1;  // close
		}
	}
	//PinLED_LIFE = 1;
	
	// tiene pollate le porte serial
	
	//if( pUART_PC ) 
		pUART_PC->HwTransfer();

	//if( pUART_BLUE )
		pUART_BLUE->HwTransfer();
		
	//if( pUART_ADuC0 )	
		pUART_ADuC0->HwTransfer();

	//if( pUART_ADuC1 )	
		pUART_ADuC1->HwTransfer();

	//if( pUART_ADuC2 )	
		pUART_ADuC2->HwTransfer();

	//if( pUART_ADuC3 )	
		pUART_ADuC3->HwTransfer();

	//if( pUART_ADuC4 )	
		pUART_ADuC4->HwTransfer();
	
	//if( pUART_ADuC5 )	
		pUART_ADuC5->HwTransfer();

	//if( pUART_ADuC6 )	
		pUART_ADuC6->HwTransfer();

	//if( pUART_EMG )	
		pUART_EMG->HwTransfer();


	globalTimer.clockTick(1, 0);	// avanti il contatore di riferimento per i	timer sw
	
	milliSec++;
	if (milliSec >= powerSamplePeriod)
	{
		milliSec = 0;
		adcNec->setEnable(true);
	}
	//PinLED_LIFE = 0;
}

static void IrqHandINTTP4CC0(int vector)
{
	if (pwmTimerP4 != NULL)
	{
		if (pwmTimerP4->getPulseCount() < pwmTimerP4->getNumPulse())
		{
			pwmTimerP4->newPulse();
		}else //if (pwmTimerP4->getPulseCount() == pwmTimerP4->getNumPulse())
		{
			pwmTimerP4->pulseCountingEnded_ISR();
			pwmTimerP4->setEnable(false, true);
			pwmTimerP2->setEnable(false, true);
			wth->setEnable(false);
		}
	}
}

static void IrqHandINTTP2CC0(int vector)
{
	/*if (pwmTimerP2 != NULL)
	{
		if (pwmTimerP2->getPulseCount() < pwmTimerP2->getNumPulse())
		{
			pwmTimerP2->newPulse();
		}else //if (pwmTimerP2->getPulseCount() == pwmTimerP2->getNumPulse())
		{
			pwmTimerP2->pulseCountingEnded_ISR();
			pwmTimerP2->setEnable(false, true);
		}
	}*/
}

static void IrqHandINTTP3CC0(int vector)
{
	wtp->accelerationRamp();
}

static void IrqHandINTTQ0CC1(int vector)
{
	dword count;
	dword overflow;

	count = TQ0CCR1;
	overflow = pwMeasureTimer->getTimerOverFlow();
	pwMeasureTimer->rstTimerOverflow();
	
	pwMeasureTimer->setMeasurementState((pwMeasurementAbstract::MeasurementState)!PinIR_RX);
		
	if (pwMeasureTimer->whatMeasuring() == pwMeasurementAbstract::measurement_high)
	{
		pwMeasureTimer->pushCountHigh(count);
		pwMeasureTimer->pushCountHigh(overflow);
		pwMeasureTimer->setMeasurementState(pwMeasurementAbstract::measurement_low);
	}else
	{
		pwMeasureTimer->pushCountLow(count);
		pwMeasureTimer->pushCountLow(overflow);
		pwMeasureTimer->setMeasurementState(pwMeasurementAbstract::measurement_high);
	}
	
}

static void IrqHandINTTQ0OV(int vector)
{
	dword overflow = 1;
	
	if(pwMeasureTimer->getTimerOverFlow() < overflow)
	{
		pwMeasureTimer->newTimerOverFlow();
	}

	if (pwMeasureTimer->getTimerOverFlow() >= overflow)
	{
		
		pwMeasureTimer->setMeasurementState((pwMeasurementAbstract::MeasurementState)PinIR_RX);
		
		if (pwMeasureTimer->whatMeasuring() == pwMeasurementAbstract::measurement_high)
		{
			pwMeasureTimer->pushCountHigh(0);
			pwMeasureTimer->pushCountHigh(overflow);
			
		}else
		{
			pwMeasureTimer->pushCountLow(0);
			pwMeasureTimer->pushCountLow(overflow);
		}
		pwMeasureTimer->rstTimerOverflow();
	}
	TQ0OPT0 &= ~0x01;
}
	

void IrqHandADC(int vector)
{
	unsigned short adc0, adc1, adc2, adc3, adc4, adc5, adc6, adc7, adc8, adc9, adc10, adc11;
	int i;
	adc0 = (ADA0CR0 >>6);
	adc1 = (ADA0CR1 >>6);
	adc2 = (ADA0CR2 >>6);
	adc3 = (ADA0CR3 >>6);
	adc4 = (ADA0CR4 >>6);
	adc5 = (ADA0CR5 >>6);
	adc6 = (ADA0CR6 >>6);
	adc7 = (ADA0CR7 >>6);
	adc8 = (ADA0CR8 >>6);
	adc9 = (ADA0CR9 >>6);
	adc10 = (ADA0CR10 >>6);
	adc11 = (ADA0CR11 >>6);
	
	adcNec->pushVoltageAdcDataToChan(_24VDC_, adc5);
	adcNec->pushVoltageAdcDataToChan(_12VDC_, adc1);
	adcNec->pushVoltageAdcDataToChan(_5VDC_DIGITAL_, adc1);
	adcNec->pushVoltageAdcDataToChan(_5VDC_ANALOG_, adc7);
	adcNec->pushVoltageAdcDataToChan(_5VDC_BUBBLE_, adc0);
	adcNec->pushVoltageAdcDataToChan(_V_2_5_, adc6);
	adcNec->pushVoltageAdcDataToChan(_V_1_2_, adc2);
	adcNec->pushVoltageAdcDataToChan(_VCC_VINF_, adc8);
	adcNec->pushVoltageAdcDataToChan(_VCC_VV_, adc9);
	adcNec->pushVoltageAdcDataToChan(_VCC_PRS14_, adc4);
	adcNec->pushVoltageAdcDataToChan(_VCC_PRS58_, adc3);

	
	adcNec->pushCurrentAdcDataToChan(0, adc10);
	adcNec->setEnable(false);
	
	/*	
	pwrVoltageAverage[_5VDC_BUBBLE_] += adc0;
	pwrVoltageAverage[_5VDC_DIGITAL_] += adc1;
	pwrVoltageAverage[_V_1_2_] += adc2;
	pwrVoltageAverage[_VCC_PRS58_] += adc3;
	pwrVoltageAverage[_VCC_PRS14_] += adc4;
	pwrVoltageAverage[_24VDC_] += adc5;
	pwrVoltageAverage[_V_2_5_] += adc6;
	pwrVoltageAverage[_5VDC_ANALOG_] += adc7;
	pwrVoltageAverage[_VCC_VINF_] += adc8;
	pwrVoltageAverage[_VCC_VV_] += adc9;
	pwrCurrentAverage += adc10;
	pwrVoltageAverage[_12VDC_] += adc11;
		*/
}


byte Asciihex_to_num (char *pc)           // Converte carattere asci esadecimale in numero
{
   if(*pc>0x2F && *pc<0x3A)               // Se carattere asci è un numero
   {
      return(*pc-0x30);                   // Toglie offset e lo restituisce
   }
   else
   {
      if(*pc>0x40 && *pc<0x47)            // Se è inoltre un carattere esadecimale
      {
         return(*pc-0x37);                // Toglie offset e lo restituisce
      }
      else
      {
         return(0x00);                    // Se nessuno dei casi , torna 0
      }
   }
}

void Num_to_Asciihex (word n, char *pc)
{
	if(n > 999)
	{
		pc[0]=0x30; pc[1]=0x30; pc[2]=0x30;
		return;
	}

	pc[0] = (n / 100) + 0x30;
	n 	  = (n % 100)       ;
	pc[1] = (n / 10)  + 0x30;
	pc[2] = (n % 10)  + 0x30;
}

/**
RAM memory test function
@param ram_start ram start address
@param ram_end ram end address
@return always 0
*/
int GrCpuMemTest(word *ram_start, word *ram_end)
{

	//////////////////////////////////////////////////////////////////////////
	// test ram-esterna
	//////////////////////////////////////////////////////////////////////////
	asm( "test_mem: 																					");
	asm( "                                                                                      		");
	asm( "			di							-- disable interrupts                            		");
	asm( "                                                                                      		");
//	asm( "---------------------------------------------------                                   		");
//	asm( "-- Inizio test ram esterna                                                          			");
//	asm( "--                                                                                    		");
	asm( "test_xram:                                                                            		");
	asm( "																								");
//	asm( "			movhi	hi(0x200000),r0,r6	-- r6 -> ram begin                             			");
//	asm( "			movhi	hi(0x220000),r0,r7	-- r7 -> ram end                             			");
//	asm( "			movea	lo(0x200000),r6,r6	-- r6 -> ram begin                             			");
//	asm( "			movea	lo(0x220000),r7,r7	-- r7 -> ram end                               			");
//	asm( "			movea	lo(0x27FFFF),r7,r7	-- r7 -> ram end                               			");

	asm( "			mov		r6,r8				-- r8 -> (copy-of) ram begin                           	");
	asm( "			mov		-1,r9				-- r9 -> filler				                          	");

//	asm( "---------------------------------------------------                                   		");
//	asm( "-- Primo step: fill della memoria con FFFF (bus a 16bit)                                      ");
	asm( "xram_fill:		                                                                            ");
	asm( "			st.h	r9,0[r6]			-- filla...     										");
	asm( "			add		2,r6				-- avanti il prossimo 				  					");
	asm( "			cmp		r7,r6				-- fine fillo ???				     					");
	asm( "			bl		xram_fill			-- se no salta 											");

//	asm( "---------------------------------------------------                                   		");
//	asm( "-- Secondo step: clear della memoria bit a bit                                    			");
	asm( "         	mov		1,r9                -- r9 ->                                                ");
	asm( "         	movhi	1,r0,r12			-- r12 -> fine test                                     ");
	asm( "          mov     r8,r6				-- r6 -> xram begin                                     ");
	asm( "          mov     r0,r10				-- r10 -> buffer                                        ");

	asm( "xram_clrbit:		                                                                            ");
	asm( "			ld.h	0[r6],r10			-- piglia...     										");
	asm( "			tst		r9,r10				-- test nuovamenteo col bit di test 					");
	asm( "			be		xram_clrbit_err		-- se non è alto segnala l'errore 						");

	asm( "										-- azzera il bit pure in memoria						");
	asm( "			xor		r9,r10				-- maschera col bit di test 							");
	asm( "			st.h	r10,0[r6]			-- riscrive...     										");

	asm( "			shl		1,r9				-- shifta il bit di test...     						");
	asm( "			tst		r12,r9				-- testa la fine del test ...     						");
	asm( "			bne		xram_clrbit			-- se non è finito salta  								");

	asm( "			add		2,r6				-- avanti il prossimo 				  					");
	asm( "         	mov		1,r9                -- r9 -> test bit                                       ");
	asm( "			cmp		r7,r6				-- fine test ???				     					");
	asm( "			bl		xram_clrbit			-- se no salta 											");

	asm( "xram_clrbit_ok:		                -- tutto ok: test passato                               ");
	asm( "			br		test_mem3			-- passa avanti											");

	asm( "xram_clrbit_err:		                                                                        ");
	asm( "         	mov		1,r10                                                                       ");
	asm( "         	jmp		[r31]		                                                                ");


//	asm( "---------------------------------------------------                                   		");
//	asm( "-- Terzo step: set della memoria bit a bit                                    			    ");
	asm( "test_mem3: 																					");
	asm( "         	mov		1,r9                -- r9 ->                                                ");
	asm( "         	movhi	1,r0,r12			-- r12 -> fine test                                     ");
	asm( "          mov     r8,r6				-- r6 -> xram begin                                     ");
	asm( "          mov     r0,r10				-- r10 -> buffer                                        ");

	asm( "xram_setbit:		                                                                            ");
	asm( "			ld.h	0[r6],r10			-- piglia...     										");
	asm( "			tst		r9,r10				-- test nuovamenteo col bit di test 					");
	asm( "			bne		xram_setbit_err		-- se è alto segnala l'errore 							");

	asm( "										-- setta il bit pure in memoria							");
	asm( "			or		r9,r10				-- maschera col bit di test 							");
	asm( "			st.h	r10,0[r6]			-- riscrive...     										");

	asm( "			shl		1,r9				-- shifta il bit di test...     						");
	asm( "			tst		r12,r9				-- testa la fine del test ...     						");
	asm( "			bne		xram_setbit			-- se non è finito salta  								");

	asm( "			add		2,r6				-- avanti il prossimo 				  					");
	asm( "         	mov		1,r9                -- r9 -> test bit                                       ");
	asm( "			cmp		r7,r6				-- fine test ???				     					");
	asm( "			bl		xram_setbit			-- se no salta 											");

	asm( "xram_setbit_ok:		                -- tutto ok: test passato                               ");
	asm( "			br		test_mem4			-- passa avanti											");

	asm( "xram_setbit_err:		                                                                        ");
	asm( "         	mov		2,r10                                                                       ");
	asm( "         	jmp		[r31]		                                                                ");


//	asm( "---------------------------------------------------                                   		");
//	asm( "-- Quarto step: clr della memoria bit a bit                                    			    ");
	asm( "test_mem4: 																					");
	asm( "         	movea	0x8000,r0,r9        -- r9 -> test bit                                       ");
	asm( "         	mov		0,r12				-- r12 -> fine test                                     ");
	asm( "          mov     r8,r6				-- r6 -> xram begin                                     ");
	asm( "          mov     r0,r10				-- r10 -> buffer                                        ");

	asm( "xram_mskbit:		                                                                            ");
	asm( "			ld.h	0[r6],r10			-- piglia...     										");
	asm( "			tst		r9,r10				-- test nuovamenteo col bit di test 					");
	asm( "			be		xram_mskbit_err		-- se non è alto segnala l'errore 						");

	asm( "										-- azzera il bit pure in memoria						");
	asm( "			xor		r9,r10				-- maschera col bit di test 							");
	asm( "			st.h	r10,0[r6]			-- riscrive...     										");

	asm( "			shr		1,r9				-- shifta il bit di test...     						");
	asm( "			tst		r12,r9				-- testa la fine del test ...     						");
	asm( "			bne		xram_mskbit			-- se non è finito salta  								");

	asm( "			add		2,r6				-- avanti il prossimo 				  					");
	asm( "         	mov		1,r9                -- r9 -> test bit                                       ");
	asm( "			cmp		r7,r6				-- fine test ???				     					");
	asm( "			bl		xram_mskbit			-- se no salta 											");

	asm( "xram_mskbit_ok:		                -- tutto ok: test passato                               ");
	asm( "			br		test_mem5			-- passa avanti											");

	asm( "xram_mskbit_err:		                                                                        ");
	asm( "         	mov		3,r10                                                                       ");
	asm( "         	jmp		[r31]		                                                                ");


//	asm( "---------------------------------------------------                                   		");
//	asm( "-- Quinto step: test address bus				                                  			    ");
	asm( "test_mem5: 																					");
	asm( "         	movea	0x1188,r0,r9        -- r9 -> test bit                                       ");
	asm( "         	movea	0x8811,r0,r12		-- r12 -> test adder                                    ");
	asm( "          mov     r8,r6				-- r6 -> xram begin                                     ");
	asm( "          mov   	r0,r10				-- r10 -> counter                                       ");

	asm( "xram_tstbus:		                                                                            ");
	asm( "			st.h	r9,0[r6]			-- filla...     										");

	asm( "			add		3,r10				-- avanti il prossimo count			  					");
	asm( "			add		r12,r9				-- avanti il prossimo count			  					");
	asm( "			add		2,r6				-- avanti il prossimo address		  					");
	asm( "			add		r10,r9				-- avanti il prossimo count			  					");
	asm( "			cmp		r7,r6				-- fine fillo ???				     					");
	asm( "			bl		xram_tstbus			-- se no salta 											");

	asm( "         	movea	0x1188,r0,r9        -- r9 -> test bit                                       ");
	asm( "         	movea	0x8811,r0,r12		-- r12 -> fine test                                     ");
	asm( "          mov     r8,r6				-- r6 -> xram begin                                     ");
	asm( "          mov   	r0,r10				-- r10 -> counter                                       ");
	asm( "          mov   	r0,r11				-- r11 -> buffer                                       	");

	asm( "xram_verbus:		                                                                            ");
	asm( "			andi	lo(-1),r9,r9		-- tosa un attimo r9 									");
	asm( "			ld.h	0[r6],r11			-- piglia il valore prec...								");
	asm( "			andi	lo(-1),r11,r11		-- tosa un attimo r11 									");
	asm( "			cmp		r9,r11				-- è uguale alla serie sviluppata prima 				");
	asm( "			bne		xram_tstbus_err		-- se no salta (siamo in buca)							");

	asm( "			add		3,r10				-- avanti il prossimo count			  					");
	asm( "			add		r12,r9				-- avanti il prossimo count			  					");
	asm( "			add		2,r6				-- avanti il prossimo address		  					");
	asm( "			add		r10,r9				-- avanti il prossimo count			  					");
	asm( "			cmp		r7,r6				-- fine fillo ???				     					");
	asm( "			bl		xram_verbus			-- se no salta 											");

	asm( "xram_tstbus_ok:		                -- tutto ok: test passato                               ");
	asm( "         	mov		r0,r10											                            ");
	asm( "         	jmp		[r31]		                                                                ");

	asm( "xram_tstbus_err:		                -- buca !!: qualcosa non va nel bus.                    ");
	asm( "         	mov		4,r10                                                                       ");
	asm( "         	jmp		[r31]		                                                                ");


	//////////////////////////////////////////////////////////////////////////
	// test ram-interna
	//////////////////////////////////////////////////////////////////////////

	return(0);
}





