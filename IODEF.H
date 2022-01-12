/**
@file		Iodef.h
@brief		Board's pins definitions
					
@author		
@date		18/01/2011
@version	01.00
*/

#ifndef _IODEF_H_
#define _IODEF_H_
//--------------------------------------------------------//
// Definizione delle costanti
// Nota:
// 		InXXXX		-> bit su maschera ingressi
//		OutXXXX		-> bit su maschera uscite
//		PinXXXX		-> bit su periferica pin ingresso/uscita
//--------------------------------------------------------//

/*
extern IOVAR INPLDKEY;

#define OutLED0     IOBUFPLD.bits.B0
#define OutLED1     IOBUFPLD.bits.B1
#define OutLED2     IOBUFPLD.bits.B2
#define OutLED3     IOBUFPLD.bits.B3
#define OutPLC0     IOBUFPLD.bits.B4
#define OutPLC1     IOBUFPLD.bits.B5
#define OutEEPWP    IOBUFPLD.bits.B6
#define OutEEPCS    IOBUFPLD.bits.B7
*/

/* Selezione dispositivi di in-out e gestione moemoria
*/

#ifdef __SIMUL__
#define BASEREG	0xFFFFF000					// nuovi indirizzi per simulatore in fondo alla mem.
#else
#define BASEREG 0xFFFFF000					// indirizzi reali registri macchina
#endif

/**
Hardware revision 0
*/
#define __HW_REV_0__	0
#define __HW_REV_1__	1

#define __FPGA_REV_0__	0
#define __FPGA_REV_1__	1

/**
Current Hardware revision
*/
#define __HARDWARE_REV__ 	__HW_REV_0__
#define __FPGA_REV__ 		__FPGA_REV_1__

/**
Crystal frequency
*/
#define CRYSTAL_FREQ	20000000.0				/
/**
Sub crystal frequancy
*/
#define CRYST_SUBFRQ	32768.0			

								// costanti di base dei vari moduli
#define BASEMOD_SIOSG2	100		// prima i moduli HW
#define BASEMOD_DMASG2	200		// modulo dma
#define BASEMOD_IRQSG2	300		// modulo irq & vectors
#define BASEMOD_WTNMSG2	400		// Watch Timer
#define BASEMOD_WDTMSG2	500		// Watch Dog Timer
#define BASEMOD_TIMMSG2	600		// modulo timers
#define BASEMOD_SERIAL_ADC	700		// modulo ad conveters

#define BASEMOD_COMBUF	1100	// poi i moduli SW
#define BASEMOD_CSI		1200
#define BASEMOD_I2C		1300
#define BASEMOD_UART	1400
#define BASEMOD_ALLOC	1500
#define BASEMOD_MMCARD	1600
#define BASEMOD_FILSYS	1700
#define BASEMOD_CHRSET	1800
#define BASEMOD_RTCDRV	1900
#define BASEMOD_NVRDRV	2000
#define BASEMOD_DATAFLASH	2100
//#define BASEMOD_EEPDRV	2100

/**
Definition for the dynamic memory allocation
*/
#define ALLOC_ALIGNMENT		4

#define DPY_DRV 	DRV_ST7565_SPI			// definizione display da utilizzare
#define	RTC_DRV 							// definizione rtc da utilizzare
#define	NVR_DRV 							// definizione nvr da utilizzare
#define EEP_DRV                 			// ram ferromagnetica

						
/**
All 8 pins set as input
*/
#define DDIR_INPUT		0xFF	
/**
All 8 pins set as output
*/
#define DDIR_OUTPUT		0x00			

/**
Pin set as output 
*/
#define PDIR_OUTPUT		0
/**
Pin set as input 
*/
#define PDIR_INPUT		1

//-------------------------------------------------------
//-------------- Hardware Revision 0 --------------------
//-------------------------------------------------------

#if (__HARDWARE_REV__ == __HW_REV_0__)
//----------------------------------------------------------------------------------------------------------
//--- uC's pins defines ---------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
#define	PinLED_LIFE		_P10				// pin LED Life: a questo pin è stato collegato il led di Life
												    // che segnala il corretto funzionamento dell'app - NON è sul tastierino.. 
#define ioPinLED_LIFE	_PM10


#define LED_LIFE_ON				1
#define LED_LIFE_OFF			0

#define BT_MODULE_PRESENT			0
#define BT_MODULE_NOT_PRESENT		1

/** LCD defines */
#define PinLCD_RST				_P11
#define ioPinLCD_RST			_PM11

#define PinLCD_CS1				_P02
#define ioPinLCD_CS1			_PM02

#define PinLCD_A0				_P03
#define ioPinLCD_A0				_PM03

#define PinLCD_WR				_P04
#define ioPinLCD_WR				_PM04

#define PinLCD_RD				_P05
#define ioPinLCD_RD				_PM05

#define Pin_EN_LCD_LED			_P33
#define ioPin_EN_LCD_LED		_PM33

/** EEPROM defines */
#define	PinCSeeprom				_P06			// pin load: ingresso che seleziona se 
#define	ioPinCSeeprom			_PM06

/** Power enable Pin */
#define PinEN_POWER				_P32
#define ioPinEN_POWER			_PM32

/** Peristaltic Pump (M1) defines */
#define PinDIR_M1				_P34
#define ioPinDIR_M1				_PM34

#define PinSTEP_M1				_P35
#define ioPinSTEP_M1			_PM35

#define PinEN_M1				_P36
#define ioPinEN_M1				_PM36

#define PinSPI_M1_CS			_P52
#define ioPinSPI_M1_CS			_PM52

#define PinHALL1_M1				_P95
#define ioPinHALL1_M1			_PM95

#define PinHALL2_M1				_P94
#define ioPinHALL2_M1			_PM94


/** Puller (M2) defines */
#define PinEN_M2				_P37
#define ioPinEN_M2				_PM37

#define PinDIR_M2				_P51
#define ioPinDIR_M2				_PM51

#define PinSTEP_M2				_P96
#define ioPinSTEP_M2			_PM96

#define PinHALL1_M2				_P93
#define ioPinHALL1_M2			_PM93

#define PinHALL2_M2				_P92
#define ioPinHALL2_M2			_PM92

/** Remote control (IR) Pin */
#define PinIR_RX				_P50
#define ioPinIR_RX				_PM50

/** Buzzer */
#define PinBUZZER 				_P914
#define ioPinBUZZER				_PM914

/** SPI FPGA*/
#define PinCS_SPI_FPGA			_P913
#define ioPinCS_SPI_FPGA		_PM913

/** FPGA programming pin*/
#define PinFPGA_PROG			_PCM3
#define ioPinFPGA_PROG			_PMCM3

/** FPGA done pin*/
#define PinFPGA_DONE			_PCM2
#define ioPinFPGA_DONE			_PMCM2

#endif



//-------------------------------------------------------
//------------------- ADDRESS_DECODER ------------------
//-------------------------------------------------------
//- Memory Map:
//  	DEVICE			--		START ADDERESS (HEX)	--		END ADDRESS (HEX)
//		ram				--		  	0x100000        	--   		0x17FFFF
//	    jtag itf		--    		0x200100	      	--   		
//	    gen_reg			--    		0x200200	      	--   		
//
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

#if (__FPGA_REV__ == __FPGA_REV_1__)
/* Indirizzi intervento su CSXX
*/
#define CS0			0x00100000 		// 001F.FFFFH (1 MB-Block0+Block4)
#define	CS1			0x00200000		// 003F.FFFFH (2 MB-Block1)
#define	CS2			0x00400000		// 00BF.FFFFH (4 MB-Block1)
#define	CS3			0x00800000		// 00FF.FFFFH (8 MB-Block1)

#define	CS_FASTRAM	 	((byte *) CS0)
#define	CS_PLD 	     	((volatile byte *) CS1)


// mappa periferiche nella pld
#define	PLD_JTAG_ITF 	((volatile jtag_bridge_t *)(CS_PLD+0x0100))
#define	PLD_GEN_REG		((volatile ctl_regs_t *)	(CS_PLD+0x0200))
#define	PLD_UART_PC		((volatile uart_fpga_t *)	(CS_PLD+0x1000))
#define	PLD_UART_BLUE	((volatile uart_fpga_t *)	(CS_PLD+0x1100))
#define	PLD_UART_BUBBLE	((volatile uart_fpga_t *)	(CS_PLD+0x1200))
#define	PLD_UART_EMG	((volatile uart_fpga_t *)	(CS_PLD+0x1300))
#define	PLD_UART_ADUC0	((volatile uart_fpga_t *)	(CS_PLD+0x1400))
#define	PLD_UART_ADUC1	((volatile uart_fpga_t *)	(CS_PLD+0x1500))
#define	PLD_UART_ADUC2	((volatile uart_fpga_t *)	(CS_PLD+0x1600))
#define	PLD_UART_ADUC3	((volatile uart_fpga_t *)	(CS_PLD+0x1700))
#define	PLD_UART_ADUC4	((volatile uart_fpga_t *)	(CS_PLD+0x1800))
#define	PLD_UART_ADUC5	((volatile uart_fpga_t *)	(CS_PLD+0x1900))
#define	PLD_UART_ADUC6	((volatile uart_fpga_t *)	(CS_PLD+0x1A00))




// definizione del moduletto jtag_bridge nella pld
struct jtag_bridge_t{
	byte	JTAG_AND;			// registro maschera AND forzatura linee jtag verso mcu
	byte	JTAG_OR;			// registro maschera OR forzatura linee jtag
	byte	jtag_adapt_status;	// stato attuale segnali programmatore
	byte	jtag_mcu_status;	// stato attuale segnali microbi
	word	marker_f4fa;		// marker per periferica jtag: 0xF4FA
};
	

// definizione del moduletto ctl_regs nella pld
struct ctl_regs_t{
	byte	BUBBLE;
	byte	HALL;
	byte	PUMP;
	byte    WTH;
	byte	USBPW;
	byte 	IDGEN1;
	byte	IDGEN2;
	byte 	KEY;
	byte 	LED;
	byte	LED_ro;				// valore leds, read-only
	word	bitstream;			// versione bit stream fpga
	word	marker_f4f0;		// marker per periferica reg: 0xF4F0
};


	// mappiamo globali i singoli bits xchè la macro reg è singola
			//-- Bubble detect pins --

// segnali rw
#define PinTXE_BUBBLE		((struct bitcharf *)&PLD_GEN_REG->BUBBLE)->bit00
#define PinCMD_PINZA		((struct bitcharf *)&PLD_GEN_REG->BUBBLE)->bit01

// segnali ro
#define PinCMP_BUBBLE		((const struct bitcharf *)&PLD_GEN_REG->BUBBLE)->bit06
#define PinRDY_BUBBLE		((const struct bitcharf *)&PLD_GEN_REG->BUBBLE)->bit07

			//----- Stepper motor sensors ? ---

#define FpHALL1_M1		((const struct bitcharf *)&PLD_GEN_REG->HALL)->bit04
#define FpHALL2_M1		((const struct bitcharf *)&PLD_GEN_REG->HALL)->bit05
#define FpHALL1_M2		((const struct bitcharf *)&PLD_GEN_REG->HALL)->bit06
#define FpHALL2_M2		((const struct bitcharf *)&PLD_GEN_REG->HALL)->bit07


			//----- Motore pompa

#define PinSTALL_M1		((const struct bitcharf *)&PLD_GEN_REG->PUMP)->bit00

			//----- Motore estrattore ---

#define PinWTH_FCL		((const struct bitcharf *)&PLD_GEN_REG->WTH)->bit00
#define PinWTH_FCH		((const struct bitcharf *)&PLD_GEN_REG->WTH)->bit01

#define PinEN_PWR		((const struct bitcharf *)&PLD_GEN_REG->USBPW)->bit00	// input pin for pwr-latch rele' 
#define PinPLUG_BLUE	((const struct bitcharf *)&PLD_GEN_REG->USBPW)->bit04	// input signal presenza bluetooth
#define PinUSB_PWR		((const struct bitcharf *)&PLD_GEN_REG->USBPW)->bit07	// usb power (bit 7) + pwr-en (bit0)


			//---- display key/led ---
#define PinKEY				((const struct bitcharf *)&PLD_GEN_REG->KEY)->bit00	

#define PinLED_LIFE_PLD		((volatile struct bitcharf *)&PLD_GEN_REG->LED)->bit00
#define PinLED1				((volatile struct bitcharf *)&PLD_GEN_REG->LED)->bit01
#define PinLED2				((volatile struct bitcharf *)&PLD_GEN_REG->LED)->bit02


//--- id generic channels ---
#define ID_GEN1 (PLD_GEN_REG->IDGEN1)
#define ID_GEN2 (PLD_GEN_REG->IDGEN2)

//---- fpga bitstream version
#define FPGA_BITSTREAM_VERSION		(PLD_GEN_REG->bitstream)

#endif

//--------------------------------------------------------//
// Definizione dei tipi
//--------------------------------------------------------//

//--------------------------------------------------------//
// Definizione delle classi
//--------------------------------------------------------//

//--------------------------------------------------------//
// Definizione delle funzioni
//--------------------------------------------------------//

//--------------------------------------------------------//
// Definizione delle variabili
//--------------------------------------------------------//

//--------------------------------------------------------//
// Inclusione altri files .....
//--------------------------------------------------------//

#endif
//@@-----------------------------------------------------------------------@@//
//@@
//@@  $Source:  $
//@@  Note di revisione:
//@@
//@@  $Revision: 1.2 $
//@@  -
//@@
//@@
//@@
//@@
//@@  $Log:  $
//@@
//@@  $Author: junior $
//@@  $RCSfile: IODEF.H,v $
//@@  $Revision: 1.1 $
//@@  $State: Exp $
//@@  $Date: 2002/05/20 00:00:00 $
//@@  $Name:  $
//@@-----------------------------------------------------------------------@@//
//@@-----------------------------------------------------------------------@@//
//@@                                                                       @@//
//@@                            C O M E T A                                @@//
//@@                                                                       @@//
//@@-----------------------------------------------------------------------@@//

