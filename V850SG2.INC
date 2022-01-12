-- //---------------------------------------------------------------------------//
-- //                                                                           //
-- //                           C O M E T A                                     //
-- //                                                                           //
-- //---------------------------------------------------------------------------//
-- //
-- //  Progetto:               	BLU RUNNER REV 4 
-- //  Nome File:               V850SG2.INC
-- //  Descrizione:             Definizione EQUATES per NEC V850SG2
-- //  Versione:                $Revision: 1.1 $
-- //  Data Versione:           $Date: 2008/01/18 06:49:06 $
-- //
-- //---------------------------------------------------------------------------//
   
   
-- //--------------------------------------------------------//
-- // Definizione delle costanti
-- //--------------------------------------------------------//
   
									-- // Indirizzo flash applicativo
  
--FLASH_START		= 0x20000  
APP_START		= 0x0  
BASEREG			= 0xFFFFF000  		-- // indirizzo di inizio della tabellzza di registri


									-- // Indirizzi entry-point interrupt (non esistono
									-- // dei vettori veri e propri)

RESET 			= 0x0000			-- // Reset Interrupt RESET - Reset input - -  
NMI			= 0x0010			-- // Non-maskable Interrupt NMI0 - NMI input - -  
                          
TRAP0  			= 0x0040			-- // Exception TRAP0 - TRAP instruction - - 
TRAP1			= 0x0050			-- // Exception TRAP1 - TRAP instruction - -  
ILGOP_TRAPDB		= 0x0060			-- // Exception ILGOP/DBG0 - Illegal opcode/DBTRAP instruction
                          
INTLVI			= 0x0080			-- // Interrupt INTLVI --> Low Voltage detector 

INTP0			= 0x0090			-- // Interrupt INTP0 --> External interrupt pin input edge detection (INTP0) 
INTP1			= 0x00A0			-- // Interrupt INTP1 --> External interrupt pin input edge detection (INTP1) 
INTP2			= 0x00B0			-- // Interrupt INTP2 --> External interrupt pin input edge detection (INTP2) 
INTP3			= 0x00C0			-- // Interrupt INTP3 --> External interrupt pin input edge detection (INTP3) 
INTP4			= 0x00D0			-- // Interrupt INTP4 --> External interrupt pin input edge detection (INTP4) 
INTP5			= 0x00E0			-- // Interrupt INTP5 --> External interrupt pin input edge detection (INTP5) 
INTP6			= 0x00F0			-- // Interrupt INTP6 --> External interrupt pin input edge detection (INTP6) 
INTP7			= 0x00100			-- // Interrupt INTP7 --> External interrupt pin input edge detection (INTP7) 

                          
INTTQ0OV 		= 0x00110 	 	 	-- // Interrupt INTTQ0OV --> TMQ0 overflow
INTTQ0CC0 		= 0x00120 	 	 	-- // Interrupt INTTQ0CC0 --> TMQ0 capture 0 / compare 0 match
INTTQ0CC1 		= 0x00130 	 	 	-- // Interrupt INTTQ0CC0 --> TMQ0 capture 1 / compare 1 match
INTTQ0CC2 		= 0x00140 	 	 	-- // Interrupt INTTQ0CC2 --> TMQ0 capture 2 / compare 2 match
INTTQ0CC3 		= 0x00150 	 	 	-- // Interrupt INTTQ0CC3 --> TMQ0 capture 3 / compare 3 match

INTTP0OV		= 0X00160			-- // Interrupt INTTP0OV --> TMP0 overflow
INTTP0CC0 		= 0x00170 	 	 	-- // Interrupt INTTP0CC0 --> TMP0 capture 0 / compare 0 match
INTTP0CC1 		= 0x00180 	 	 	-- // Interrupt INTTP0CC0 --> TMP0 capture 1 / compare 1 match

INTTP1OV		= 0x00190			-- // Interrupt INTTP1OV --> TMP1 overflow
INTTP1CC0		= 0x001A0			-- // Interrupt INTTP1CC0 --> TMP1 capture 0 / compare 0 match
INTTP1CC1		= 0x001B0			-- // Interrupt INTTP1CC1 --> TMP1 capture 1 / compare 1 match

INTTP2OV		= 0x001C0			-- // Interrupt INTTP2OV --> TMP2 overflow
INTTP2CC0		= 0x001D0			-- // Interrupt INTTP2CC0 --> TMP2 capture 0 / compare 0 match
INTTP2CC1		= 0x001E0			-- // Interrupt INTTP2CC1 --> TMP2 capture 1 / compare 1 match

INTTP3OV		= 0x001F0			-- // Interrupt INTTP3OV --> TMP3 overflow
INTTP3CC0		= 0x00200			-- // Interrupt INTTP3CC0 --> TMP3 capture 0 / compare 0 match
INTTP3CC1		= 0x00210			-- // Interrupt INTTP3CC1 --> TMP3 capture 1 / compare 1 match

INTTP4OV		= 0x00220			-- // Interrupt INTTP4OV --> TMP4 overflow
INTTP4CC0		= 0x00230			-- // Interrupt INTTP4CC0 --> TMP4 capture 0 / compare 0 match
INTTP4CC1		= 0x00240			-- // Interrupt INTTP4CC1 --> TMP4 capture 1 / compare 1 match

INTTP5OV		= 0x00250			-- // Interrupt INTTP5OV --> TMP5 overflow
INTTP5CC0		= 0x00260			-- // Interrupt INTTP5CC0 --> TMP5 capture 0 / compare 0 match
INTTP5CC1		= 0x00270			-- // Interrupt INTTP5CC1 --> TMP5 capture 1 / compare 1 match

INTTM0EQ0		= 0x00280			-- // Interrupt INTTM0EQ0 --> TMM0 compare match

INTCB0R_INTIIC1		= 0x00290			-- // Interrupt INTCB0R --> CSIB0 reception completion / CSIB0 reception error / IIC1 transfert completion
INTCB0T			= 0x002A0			-- // Interrupt INTCB0T --> CSIB0 consecutive trasmission write enable

INTCB1R			= 0x002B0			-- // Interrupt INTCB1R --> CSIB1 reception completion / CSIB1 reception error 
INTCB1T			= 0x002C0			-- // Interrupt INTCB1T --> CSIB1 consecutive trasmission write enable

INTCB2R			= 0x002D0			-- // Interrupt INTCB2R --> CSIB2 reception completion / CSIB2 reception error 
INTCB2T			= 0x002E0			-- // Interrupt INTCB2T --> CSIB2 consecutive trasmission write enable

INTCB3R			= 0x002F0			-- // Interrupt INTCB3R --> CSIB3 reception completion / CSIB3 reception error 
INTCB3T			= 0x00300			-- // Interrupt INTCB3T --> CSIB3 consecutive trasmission write enable

INTUA0R_INTCB4R		= 0x00310			-- // Interrupt INTUA0R --> UART0 reception completion / CSIB3 reception completion / CSIB3 reception error 
INTUA0T_INTCB4T		= 0x00320			-- // Interrupt INTUA0T --> UART0 consecutive trasmission enable / CSIB4 consecutive trasmission write enable

INTUA1R_INTIIC2		= 0x00330			-- // Interrupt INTUA1R --> UART1 reception completion / IIC2 transfert completion
INTUA1T			= 0x00340			-- // Interrupt INTUA1T --> UART1 consecutive trasmission enable

INTUA2R_INTIIC0		= 0x00350			-- // Interrupt INTUA2R --> UART1 reception completion / IIC0 transfert completion
INTUA2T			= 0x00360			-- // Interrupt INTUA2T --> UART1 consecutive trasmission enable

INTAD			= 0X00370			-- // Interrupt INTAD --> A/D converdion completion

INTDMA0			= 0x00380			-- // Interrupt INTDMA0 --> DMA0 transert completion
INTDMA1			= 0x00390			-- // Interrupt INTDMA1 --> DMA1 transert completion
INTDMA2			= 0x003A0			-- // Interrupt INTDMA2 --> DMA2 transert completion
INTDMA3			= 0x003B0			-- // Interrupt INTDMA3 --> DMA3 transert completion

INTKR			= 0x003C0			-- // Interrupt INTKR --> Key return interrupt

INTWTI			= 0x003D0			-- // Interrupt INTWTI --> Watch timer interval
INTWT			= 0x003E0			-- // Interrupt INTWT --> Watch timer reference time

--INTC0ERR_INTERR	= 0x003F0			-- // Interrupt INTC0ERR --> AFCAN0 error / IEBus error

--INTC0WUP_INTSTA	= 0x00400			-- // Interrupt INTC0WUP --> AFCAN0 wake up / IEBus status

--INTC0REC_INTIE1	= 0x00410			-- // Interrupt INTC0REC --> AFCAN0 reception / IEBus data interrupt

--INTC0TRX_INTIE2	= 0x00420			-- // Interrupt INTC0TRX --> AFCAN0 trasmission / IEBus error / IEBus status


BUCONELLOZONO           = 0x0800

	-- // Special function registers -- SE NON SPECIFICATO è UN PUNTATORE A (volatile unsigned char*)

PDL     	= (BASEREG+0x0004)	-- (volatile unsigned short *)
PDLL    	= (BASEREG+0x0004)
PDLH    	= (BASEREG+0x0005)
PDH     	= (BASEREG+0x0006)
PCT     	= (BASEREG+0x000a)
PCM     	= (BASEREG+0x000c)
PMDL    	= (BASEREG+0x0024)	-- (volatile unsigned short *)
PMDLL   	= (BASEREG+0x0024)
PMDLH   	= (BASEREG+0x0025)
PMDH    	= (BASEREG+0x0026)
PMCT    	= (BASEREG+0x002a)
PMCM    	= (BASEREG+0x002c)
PMCDL   	= (BASEREG+0x0044)	-- (volatile unsigned short *)
PMCDLL  	= (BASEREG+0x0044)
PMCDLH  	= (BASEREG+0x0045)
PMCDH   	= (BASEREG+0x0046)
PMCCT   	= (BASEREG+0x004a)
PMCCM   	= (BASEREG+0x004c)
BPC     	= (BASEREG+0x0064)	-- (volatile unsigned short *)
BSC     	= (BASEREG+0x0066)	-- (volatile unsigned short *)
VSWC    	= (BASEREG+0x006e)
DSA0L   	= (BASEREG+0x0080)	-- (volatile unsigned short *)
DDA0L   	= (BASEREG+0x0084)	-- (volatile unsigned short *)
DDA0H   	= (BASEREG+0x0086)	-- (volatile unsigned short *)
DSA1L   	= (BASEREG+0x0088)	-- (volatile unsigned short *)
DSA1H   	= (BASEREG+0x008a)	-- (volatile unsigned short *)
DDA1L   	= (BASEREG+0x008c)	-- (volatile unsigned short *)
DDA1H   	= (BASEREG+0x008e)	-- (volatile unsigned short *)
DSA2L   	= (BASEREG+0x0090)	-- (volatile unsigned short *)
DSA2H   	= (BASEREG+0x0092)	-- (volatile unsigned short *)
DDA2L   	= (BASEREG+0x0094)	-- (volatile unsigned short *)
DDA2H   	= (BASEREG+0x0096)	-- (volatile unsigned short *)
DSA3L   	= (BASEREG+0x0098)	-- (volatile unsigned short *)
DSA3H   	= (BASEREG+0x009a)	-- (volatile unsigned short *)
DDA3L   	= (BASEREG+0x009c)	-- (volatile unsigned short *)
DDA3H   	= (BASEREG+0x009e)	-- (volatile unsigned short *)
DBC0    	= (BASEREG+0x00c0)	-- (volatile unsigned short *)
DBC1    	= (BASEREG+0x00c2)	-- (volatile unsigned short *)
DBC2    	= (BASEREG+0x00c4)	-- (volatile unsigned short *)
DBC3    	= (BASEREG+0x00c6)	-- (volatile unsigned short *)
DADC0   	= (BASEREG+0x00d0)	-- (volatile unsigned short *)
DADC1   	= (BASEREG+0x00d2)	-- (volatile unsigned short *)
DADC2   	= (BASEREG+0x00d4)	-- (volatile unsigned short *)
DADC3   	= (BASEREG+0x00d6)	-- (volatile unsigned short *)
DCHC0   	= (BASEREG+0x00e0)
DCHC1   	= (BASEREG+0x00e2)
DCHC2   	= (BASEREG+0x00e4)
DCHC3   	= (BASEREG+0x00e6)
IMR0    	= (BASEREG+0x0100)	-- (volatile unsigned short *)
IMR0L   	= (BASEREG+0x0100)
IMR0H   	= (BASEREG+0x0101)
IMR1    	= (BASEREG+0x0102)	-- (volatile unsigned short *)
IMR1L   	= (BASEREG+0x0102)
IMR1H   	= (BASEREG+0x0103)
IMR2    	= (BASEREG+0x0104)	-- (volatile unsigned short *)
IMR2L   	= (BASEREG+0x0104)
IMR2H   	= (BASEREG+0x0105)
IMR3    	= (BASEREG+0x0106)	-- (volatile unsigned short *)
IMR3L   	= (BASEREG+0x0106)
IMR3H   	= (BASEREG+0x0107)
LVIIC   	= (BASEREG+0x0110)
PIC0    	= (BASEREG+0x0112)
PIC1    	= (BASEREG+0x0114)
PIC2    	= (BASEREG+0x0116)
PIC3    	= (BASEREG+0x0118)
PIC4    	= (BASEREG+0x011a)
PIC5    	= (BASEREG+0x011c)
PIC6    	= (BASEREG+0x011e)
PIC7    	= (BASEREG+0x0120)
TQ0OVIC 	= (BASEREG+0x0122)
TQ0CCIC0	= (BASEREG+0x0124)
TQ0CCIC1	= (BASEREG+0x0126)
TQ0CCIC2	= (BASEREG+0x0128)
TQ0CCIC3	= (BASEREG+0x012a)
TP0OVIC 	= (BASEREG+0x012c)
TP0CCIC0	= (BASEREG+0x012e)
TP0CCIC1	= (BASEREG+0x0130)
TP1OVIC 	= (BASEREG+0x0132)
TP1CCIC0	= (BASEREG+0x0134)
TP1CCIC1	= (BASEREG+0x0136)
TP2OVIC 	= (BASEREG+0x0138)
TP2CCIC0	= (BASEREG+0x013a)
TP2CCIC1	= (BASEREG+0x013c)
TP3OVIC 	= (BASEREG+0x013e)
TP3CCIC0	= (BASEREG+0x0140)
TP3CCIC1	= (BASEREG+0x0142)
TP4OVIC 	= (BASEREG+0x0144)
TP4CCIC0	= (BASEREG+0x0146)
TP4CCIC1	= (BASEREG+0x0148)
TP5OVIC 	= (BASEREG+0x014a)
TP5CCIC0	= (BASEREG+0x014c)
TP5CCIC1	= (BASEREG+0x014e)
TM0EQIC0	= (BASEREG+0x0150)
CB0RIC  	= (BASEREG+0x0152)
IICIC1  	= (BASEREG+0x0152)
CB0TIC  	= (BASEREG+0x0154)
CB1RIC  	= (BASEREG+0x0156)
CB1TIC  	= (BASEREG+0x0158)
CB2RIC  	= (BASEREG+0x015a)
CB2TIC  	= (BASEREG+0x015c)
CB3RIC  	= (BASEREG+0x015e)
CB3TIC  	= (BASEREG+0x0160)
CB4RIC  	= (BASEREG+0x0162)
UA0RIC  	= (BASEREG+0x0162)
CB4TIC  	= (BASEREG+0x0164)
UA0TIC  	= (BASEREG+0x0164)
IICIC2  	= (BASEREG+0x0166)
UA1RIC  	= (BASEREG+0x0166)
UA1TIC  	= (BASEREG+0x0168)
IICIC0  	= (BASEREG+0x016a)
UA2RIC  	= (BASEREG+0x016a)
UA2TIC  	= (BASEREG+0x016c)
ADIC    	= (BASEREG+0x016e)
DMAIC0  	= (BASEREG+0x0170)
DMAIC1  	= (BASEREG+0x0172)
DMAIC2  	= (BASEREG+0x0174)
DMAIC3  	= (BASEREG+0x0176)
KRIC    	= (BASEREG+0x0178)
WTIIC   	= (BASEREG+0x017a)
WTIC    	= (BASEREG+0x017c)
ERRIC0  	= (BASEREG+0x017e)
WUPIC0  	= (BASEREG+0x0180)
RECIC0  	= (BASEREG+0x0182)
TRXIC0  	= (BASEREG+0x0184)
ISPR    	= (BASEREG+0x01fa)
PRCMD   	= (BASEREG+0x01fc)
PSC     	= (BASEREG+0x01fe)
ADA0M0  	= (BASEREG+0x0200)
ADA0M1  	= (BASEREG+0x0201)
ADA0S   	= (BASEREG+0x0202)
ADA0M2  	= (BASEREG+0x0203)
ADA0PFM 	= (BASEREG+0x0204)
ADA0PFT 	= (BASEREG+0x0205)
ADA0CR0 	= (BASEREG+0x0210)	-- (volatile unsigned short *)
ADA0CR0H	= (BASEREG+0x0211)
ADA0CR1 	= (BASEREG+0x0212)	-- (volatile unsigned short *)
ADA0CR1H	= (BASEREG+0x0213)
ADA0CR2 	= (BASEREG+0x0214)	-- (volatile unsigned short *)
ADA0CR2H	= (BASEREG+0x0215)
ADA0CR3 	= (BASEREG+0x0216)	-- (volatile unsigned short *)
ADA0CR3H	= (BASEREG+0x0217)
ADA0CR4 	= (BASEREG+0x0218)	-- (volatile unsigned short *)
ADA0CR4H	= (BASEREG+0x0219)
ADA0CR5 	= (BASEREG+0x021a)	-- (volatile unsigned short *)
ADA0CR5H	= (BASEREG+0x021b)
ADA0CR6 	= (BASEREG+0x021c)	-- (volatile unsigned short *)
ADA0CR6H	= (BASEREG+0x021d)
ADA0CR7 	= (BASEREG+0x021e)	-- (volatile unsigned short *)
ADA0CR7H	= (BASEREG+0x021f)
ADA0CR8 	= (BASEREG+0x0220)	-- (volatile unsigned short *)
ADA0CR8H	= (BASEREG+0x0221)
ADA0CR9 	= (BASEREG+0x0222)	-- (volatile unsigned short *)
ADA0CR9H	= (BASEREG+0x0223)
ADA0CR10	= (BASEREG+0x0224)	-- (volatile unsigned short *)
ADA0CR10H	= (BASEREG+0x0225)
ADA0CR11	= (BASEREG+0x0226)	-- (volatile unsigned short *)
ADA0CR11H	= (BASEREG+0x0227)
DA0CS0  	= (BASEREG+0x0280)
DA0CS1  	= (BASEREG+0x0281)
DA0M    	= (BASEREG+0x0282)
KRM     	= (BASEREG+0x0300)
SELCNT0 	= (BASEREG+0x0308)
CRCIN   	= (BASEREG+0x0310)
CRCD    	= (BASEREG+0x0312)	-- (volatile unsigned short *)
NFC     	= (BASEREG+0x0318)
PRSM1   	= (BASEREG+0x0320)
PRSCM1  	= (BASEREG+0x0321)
PRSM2   	= (BASEREG+0x0324)
PRSCM2  	= (BASEREG+0x0325)
PRSM3   	= (BASEREG+0x0328)
PRSCM3  	= (BASEREG+0x0329)
OCKS0   	= (BASEREG+0x0340)
OCKS1   	= (BASEREG+0x0344)
P0      	= (BASEREG+0x0400)
P1      	= (BASEREG+0x0402)
P3      	= (BASEREG+0x0406)	-- (volatile unsigned short *)
P3L     	= (BASEREG+0x0406)
P3H     	= (BASEREG+0x0407)
P4      	= (BASEREG+0x0408)
P5      	= (BASEREG+0x040a)
P7L     	= (BASEREG+0x040e)
P7H     	= (BASEREG+0x040f)
P9      	= (BASEREG+0x0412)	-- (volatile unsigned short *)
P9L     	= (BASEREG+0x0412)
P9H     	= (BASEREG+0x0413)
PM0     	= (BASEREG+0x0420)
PM1     	= (BASEREG+0x0422)
PM3     	= (BASEREG+0x0426)	-- (volatile unsigned short *)
PM3L    	= (BASEREG+0x0426)
PM3H    	= (BASEREG+0x0427)
PM4     	= (BASEREG+0x0428)
PM5     	= (BASEREG+0x042a)
PM7L    	= (BASEREG+0x042e)
PM7H    	= (BASEREG+0x042f)
PM9     	= (BASEREG+0x0432)	-- (volatile unsigned short *)
PM9L    	= (BASEREG+0x0432)
PM9H    	= (BASEREG+0x0433)
PMC0    	= (BASEREG+0x0440)
PMC3    	= (BASEREG+0x0446)	-- (volatile unsigned short *)
PMC3L   	= (BASEREG+0x0446)
PMC3H   	= (BASEREG+0x0447)
PMC4    	= (BASEREG+0x0448)
PMC5    	= (BASEREG+0x044a)
PMC9    	= (BASEREG+0x0452)	-- (volatile unsigned short *)
PMC9L   	= (BASEREG+0x0452)
PMC9H   	= (BASEREG+0x0453)
PFC0    	= (BASEREG+0x0460)
PFC3    	= (BASEREG+0x0466)	-- (volatile unsigned short *)
PFC3L   	= (BASEREG+0x0466)
PFC3H   	= (BASEREG+0x0467)
PFC4    	= (BASEREG+0x0468)
PFC5    	= (BASEREG+0x046a)
PFC9    	= (BASEREG+0x0472)	-- (volatile unsigned short *)
PFC9L   	= (BASEREG+0x0472)
PFC9H   	= (BASEREG+0x0473)
DWC0    	= (BASEREG+0x0484)	-- (volatile unsigned short *)
AWC     	= (BASEREG+0x0488)	-- (volatile unsigned short *)
BCC     	= (BASEREG+0x048a)	-- (volatile unsigned short *)
TQ0CTL0 	= (BASEREG+0x0540)
TQ0CTL1 	= (BASEREG+0x0541)
TQ0IOC0 	= (BASEREG+0x0542)
TQ0IOC1 	= (BASEREG+0x0543)
TQ0IOC2 	= (BASEREG+0x0544)
TQ0OPT0 	= (BASEREG+0x0545)
TQ0CCR0 	= (BASEREG+0x0546)	-- (volatile unsigned short *)
TQ0CCR1 	= (BASEREG+0x0548)	-- (volatile unsigned short *)
TQ0CCR2 	= (BASEREG+0x054a)	-- (volatile unsigned short *)
TQ0CCR3 	= (BASEREG+0x054c)	-- (volatile unsigned short *)
TQ0CNT  	= (BASEREG+0x054e)	-- (volatile unsigned short *)
TP0CTL0 	= (BASEREG+0x0590)
TP0CTL1 	= (BASEREG+0x0591)
TP0IOC0 	= (BASEREG+0x0592)
TP0IOC1 	= (BASEREG+0x0593)
TP0IOC2 	= (BASEREG+0x0594)
TP0OPT0 	= (BASEREG+0x0595)
TP0CCR0 	= (BASEREG+0x0596)	-- (volatile unsigned short *)
TP0CCR1 	= (BASEREG+0x0598)	-- (volatile unsigned short *)
TP0CNT  	= (BASEREG+0x059a)	-- (volatile unsigned short *)
TP1CTL0 	= (BASEREG+0x05a0)
TP1CTL1 	= (BASEREG+0x05a1)
TP1IOC0 	= (BASEREG+0x05a2)
TP1IOC1 	= (BASEREG+0x05a3)
TP1IOC2 	= (BASEREG+0x05a4)
TP1OPT0 	= (BASEREG+0x05a5)
TP1CCR0 	= (BASEREG+0x05a6)	-- (volatile unsigned short *)
TP1CCR1 	= (BASEREG+0x05a8)	-- (volatile unsigned short *)
TP1CNT  	= (BASEREG+0x05aa)	-- (volatile unsigned short *)
TP2CTL0 	= (BASEREG+0x05b0)
TP2CTL1 	= (BASEREG+0x05b1)
TP2IOC0 	= (BASEREG+0x05b2)
TP2IOC1 	= (BASEREG+0x05b3)
TP2IOC2 	= (BASEREG+0x05b4)
TP2OPT0 	= (BASEREG+0x05b5)
TP2CCR0 	= (BASEREG+0x05b6)	-- (volatile unsigned short *)
TP2CCR1 	= (BASEREG+0x05b8)	-- (volatile unsigned short *)
TP2CNT  	= (BASEREG+0x05ba)	-- (volatile unsigned short *)
TP3CTL0 	= (BASEREG+0x05c0)
TP3CTL1 	= (BASEREG+0x05c1)
TP3IOC0 	= (BASEREG+0x05c2)
TP3IOC1 	= (BASEREG+0x05c3)
TP3IOC2 	= (BASEREG+0x05c4)
TP3OPT0 	= (BASEREG+0x05c5)
TP3CCR0 	= (BASEREG+0x05c6)	-- (volatile unsigned short *)
TP3CCR1 	= (BASEREG+0x05c8)	-- (volatile unsigned short *)
TP3CNT  	= (BASEREG+0x05ca)	-- (volatile unsigned short *)
TP4CTL0 	= (BASEREG+0x05d0)
TP4CTL1 	= (BASEREG+0x05d1)
TP4IOC0 	= (BASEREG+0x05d2)
TP4IOC1 	= (BASEREG+0x05d3)
TP4IOC2 	= (BASEREG+0x05d4)
TP4OPT0 	= (BASEREG+0x05d5)
TP4CCR0 	= (BASEREG+0x05d6)	-- (volatile unsigned short *)
TP4CCR1 	= (BASEREG+0x05d8)	-- (volatile unsigned short *)
TP4CNT  	= (BASEREG+0x05da)	-- (volatile unsigned short *)
TP5CTL0 	= (BASEREG+0x05e0)
TP5CTL1 	= (BASEREG+0x05e1)
TP5IOC0 	= (BASEREG+0x05e2)
TP5IOC1 	= (BASEREG+0x05e3)
TP5IOC2 	= (BASEREG+0x05e4)
TP5OPT0 	= (BASEREG+0x05e5)
TP5CCR0 	= (BASEREG+0x05e6)	-- (volatile unsigned short *)
TP5CCR1 	= (BASEREG+0x05e8)	-- (volatile unsigned short *)
TP5CNT  	= (BASEREG+0x05ea)	-- (volatile unsigned short *)
WTM     	= (BASEREG+0x0680)
TM0CTL0 	= (BASEREG+0x0690)
TM0CMP0 	= (BASEREG+0x0694)	-- (volatile unsigned short *)
OSTS    	= (BASEREG+0x06c0)
PLLS    	= (BASEREG+0x06c1)
WDTM2   	= (BASEREG+0x06d0)
WDTE    	= (BASEREG+0x06d1)
RTBL0   	= (BASEREG+0x06e0)
RTBH0   	= (BASEREG+0x06e2)
RTPM0   	= (BASEREG+0x06e4)
RTPC0   	= (BASEREG+0x06e5)
PFCE3L  	= (BASEREG+0x0706)
PFCE5   	= (BASEREG+0x070a)
PFCE9   	= (BASEREG+0x0712)	-- (volatile unsigned short *)
PFCE9L  	= (BASEREG+0x0712)
PFCE9H  	= (BASEREG+0x0713)
SYS     	= (BASEREG+0x0802)
RCM     	= (BASEREG+0x080c)
DTFR0   	= (BASEREG+0x0810)
DTFR1   	= (BASEREG+0x0812)
DTFR2   	= (BASEREG+0x0814)
DTFR3   	= (BASEREG+0x0816)
PSMR    	= (BASEREG+0x0820)
CKC     	= (BASEREG+0x0822)
LOCKR   	= (BASEREG+0x0824)
PCC     	= (BASEREG+0x0828)
PLLCTL  	= (BASEREG+0x082c)
CCLS    	= (BASEREG+0x082e)
CORAD0  	= (BASEREG+0x0840)	-- (volatile unsigned long *)
CORAD0L 	= (BASEREG+0x0840)	-- (volatile unsigned short *)
CORAD0H 	= (BASEREG+0x0842)	-- (volatile unsigned short *)
CORAD1  	= (BASEREG+0x0844)	-- (volatile unsigned long *)
CORAD1L 	= (BASEREG+0x0844)	-- (volatile unsigned short *)
CORAD1H 	= (BASEREG+0x0846)	-- (volatile unsigned short *)
CORAD2  	= (BASEREG+0x0848)	-- (volatile unsigned long *)
CORAD2L 	= (BASEREG+0x0848)	-- (volatile unsigned short *)
CORAD2H 	= (BASEREG+0x084a)	-- (volatile unsigned short *)
CORAD3  	= (BASEREG+0x084c)	-- (volatile unsigned long *)
CORAD3L 	= (BASEREG+0x084c)	-- (volatile unsigned short *)
CORAD3H 	= (BASEREG+0x084e)	-- (volatile unsigned short *)
CLM     	= (BASEREG+0x0870)
CORCN   	= (BASEREG+0x0880)
RESF    	= (BASEREG+0x0888)
LVIM    	= (BASEREG+0x0890)
LVIS    	= (BASEREG+0x0891)
RAMS    	= (BASEREG+0x0892)
PRSM0   	= (BASEREG+0x08b0)
PRSCM0  	= (BASEREG+0x08b1)
OCDM    	= (BASEREG+0x09fc)
PEMU1   	= (BASEREG+0x09fe)
UA0CTL0 	= (BASEREG+0x0a00)
UA0CTL1 	= (BASEREG+0x0a01)
UA0CTL2 	= (BASEREG+0x0a02)
UA0OPT0 	= (BASEREG+0x0a03)
UA0STR  	= (BASEREG+0x0a04)
UA0RX   	= (BASEREG+0x0a06)
UA0TX   	= (BASEREG+0x0a07)
UA1CTL0 	= (BASEREG+0x0a10)
UA1CTL1 	= (BASEREG+0x0a11)
UA1CTL2 	= (BASEREG+0x0a12)
UA1OPT0 	= (BASEREG+0x0a13)
UA1STR  	= (BASEREG+0x0a14)
UA1RX   	= (BASEREG+0x0a16)
UA1TX   	= (BASEREG+0x0a17)
UA2CTL0 	= (BASEREG+0x0a20)
UA2CTL1 	= (BASEREG+0x0a21)
UA2CTL2 	= (BASEREG+0x0a22)
UA2OPT0 	= (BASEREG+0x0a23)
UA2STR  	= (BASEREG+0x0a24)
UA2RX   	= (BASEREG+0x0a26)
UA2TX   	= (BASEREG+0x0a27)
INTF0   	= (BASEREG+0x0c00)
INTF3   	= (BASEREG+0x0c06)
INTF9H  	= (BASEREG+0x0c13)
INTR0   	= (BASEREG+0x0c20)
INTR3   	= (BASEREG+0x0c26)
INTR9H  	= (BASEREG+0x0c33)
PF0     	= (BASEREG+0x0c60)
PF3     	= (BASEREG+0x0c66)	-- (volatile unsigned short *)
PF3L    	= (BASEREG+0x0c66)
PF3H    	= (BASEREG+0x0c67)
PF4     	= (BASEREG+0x0c68)
PF5     	= (BASEREG+0x0c6a)
PF9     	= (BASEREG+0x0c72)	-- (volatile unsigned short *)
PF9L    	= (BASEREG+0x0c72)
PF9H    	= (BASEREG+0x0c73)
CB0CTL0 	= (BASEREG+0x0d00)
CB0CTL1 	= (BASEREG+0x0d01)
CB0CTL2 	= (BASEREG+0x0d02)
CB0STR  	= (BASEREG+0x0d03)
CB0RX   	= (BASEREG+0x0d04)	-- (volatile unsigned short *)
CB0RXL  	= (BASEREG+0x0d04)
CB0TX   	= (BASEREG+0x0d06)	-- (volatile unsigned short *)
CB0TXL  	= (BASEREG+0x0d06)
CB1CTL0 	= (BASEREG+0x0d10)
CB1CTL1 	= (BASEREG+0x0d11)
CB1CTL2 	= (BASEREG+0x0d12)
CB1STR  	= (BASEREG+0x0d13)
CB1RX   	= (BASEREG+0x0d14)	-- (volatile unsigned short *)
CB1RXL  	= (BASEREG+0x0d14)
CB1TX   	= (BASEREG+0x0d16)	-- (volatile unsigned short *)
CB1TXL  	= (BASEREG+0x0d16)
CB2CTL0 	= (BASEREG+0x0d20)
CB2CTL1 	= (BASEREG+0x0d21)
CB2CTL2 	= (BASEREG+0x0d22)
CB2STR  	= (BASEREG+0x0d23)
CB2RX   	= (BASEREG+0x0d24)	-- (volatile unsigned short *)
CB2RXL  	= (BASEREG+0x0d24)
CB2TX   	= (BASEREG+0x0d26)	-- (volatile unsigned short *)
CB2TXL  	= (BASEREG+0x0d26)
CB3CTL0 	= (BASEREG+0x0d30)
CB3CTL1 	= (BASEREG+0x0d31)
CB3CTL2 	= (BASEREG+0x0d32)
CB3STR  	= (BASEREG+0x0d33)
CB3RX   	= (BASEREG+0x0d34)	-- (volatile unsigned short *)
CB3RXL  	= (BASEREG+0x0d34)
CB3TX   	= (BASEREG+0x0d36)	-- (volatile unsigned short *)
CB3TXL  	= (BASEREG+0x0d36)
CB4CTL0 	= (BASEREG+0x0d40)
CB4CTL1 	= (BASEREG+0x0d41)
CB4CTL2 	= (BASEREG+0x0d42)
CB4STR  	= (BASEREG+0x0d43)
CB4RX   	= (BASEREG+0x0d44)	-- (volatile unsigned short *)
CB4RXL  	= (BASEREG+0x0d44)
CB4TX   	= (BASEREG+0x0d46)	-- (volatile unsigned short *)
CB4TXL  	= (BASEREG+0x0d46)
IIC0    	= (BASEREG+0x0d80)
IICC0   	= (BASEREG+0x0d82)
SVA0    	= (BASEREG+0x0d83)
IICCL0  	= (BASEREG+0x0d84)
IICX0   	= (BASEREG+0x0d85)
IICS0   	= (BASEREG+0x0d86)
IICF0   	= (BASEREG+0x0d8a)
IIC1    	= (BASEREG+0x0d90)
IICC1   	= (BASEREG+0x0d92)
SVA1    	= (BASEREG+0x0d93)
IICCL1  	= (BASEREG+0x0d94)
IICX1   	= (BASEREG+0x0d95)
IICS1   	= (BASEREG+0x0d96)
IICF1   	= (BASEREG+0x0d9a)
IIC2    	= (BASEREG+0x0da0)
IICC2   	= (BASEREG+0x0da2)
SVA2    	= (BASEREG+0x0da3)
IICCL2  	= (BASEREG+0x0da4)
IICX2   	= (BASEREG+0x0da5)
IICS2   	= (BASEREG+0x0da6)
IICF2   	= (BASEREG+0x0daa)
EXIMC   	= (BASEREG+0x0fbe)

	



-- //--------------------------------------------------------//
-- // Definizione delle classi
-- //--------------------------------------------------------//
   
-- //--------------------------------------------------------//
-- // Definizione delle funzioni
-- //--------------------------------------------------------//


-- //--------------------------------------------------------//
-- // Definizione delle variabili
-- //--------------------------------------------------------//






-- //@@-----------------------------------------------------------------------@@//
-- //@@
-- //@@  $Source:  $
-- //@@  Note di revisione:
-- //@@
-- //@@  $Log:  $
-- //@@
-- //@@  $Author: Nicola $
-- //@@  $RCSfile: V850SG2.INC $
-- //@@  $Revision: 1.1 $
-- //@@  $State: Exp $
-- //@@  $Date: 2008/01/18 06:49:06 $
-- //@@  $Name:  $
-- //@@-----------------------------------------------------------------------@@//
-- //@@-----------------------------------------------------------------------@@//
-- //@@                                                                       @@//
-- //@@                           C O M E T A                                 @@//
-- //@@                                                                       @@//
-- //@@-----------------------------------------------------------------------@@//
  
  

