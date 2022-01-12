/**
@file		Main.h
@brief		HRC-7000 Main file
					
@author		
@date		18/01/2011
@version	01.00
*/
   
#ifndef _MAIN_H_
#define _MAIN_H_

#include "global.h"
#include "swtim.h"
#include "alloc.h"
#include "new.h"
#include "strclass.h"

#include "sioSG2.h"
#include "dpydrv.h"
#include "chrset.h"
#include "userfont.h"

#include "Adc_SAU7000.h"

#include "appmenu.h"

#include "sound.h"
#include "keyboard.h"
#include "UartAbstract.h"
#include "UartNEC.h"
#include "UartFPGA.h"
#include "channels.h"
#include "protocolSAUandPC.h"
#include "protocol_uC.h"
#include "protocolSauAndEmgz.h"
#include "protocolADuCfwUpgrade.h"
#include "ADuC_UpgradeManager.h"
#include "iWRAP_BLUEGIGA_SPP.h"

#include "EEPROM_SPI.h"

#include "SAU7000_Manager.h"

#include "timer_NEC.h"
#include "pwmAbstract.h"
#include "pwm_NEC.h"

#include "wth.h"
#include "IR_Toshiba_protocol.h"
#include "IR_RC5_Standard_protocol.h"
#include "IR_remote.h"
#include "SPI_Abstract.h"
#include "SPI_NEC.h"
#include "TMC262_drv.h"
#include "wtp.h"
#include "SSPIE_main.h"
#include "sspie_util.h"



//--------------------------------------------------------//
// Definizione delle variabili
//--------------------------------------------------------//

extern const char sAppRevision[];
extern const char sAppDate[];
extern const char sAppName[];
extern const char sHwVersion[];
extern const word fpgaWantedBitstream;

extern const int sAppNameLength;
extern const int sAppRevisionLength;
extern const int sAppDateLength;
extern const int sHwVersionLength;

extern DisplaySetting displaySetting;
 
extern UartFPGA *pUART_PC;
extern UartFPGA *pUART_BLUE;
extern UartFPGA *pUART_EMG;
extern UartFPGA *pUART_ADuC[ADuC_NUM];

#endif

































