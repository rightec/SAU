
/**
@file		V850SG2.h
@brief		NEC V850 microcontroller device NEC - V850ES/JG2 - uPD70F3719GC
                                                                    
Declarations of I/O Registers                                                                                         
                                                              
Copyright (C) NEC Corporation 2003                                  
This file was created from device file DF3281Y.800      [V1.10]    
by DeFiX V1.07b                                                     
                                                              
This file is only intended as a sample supplement to NEC tools.     
Feel free to adapt it to your own needs.                           
This File is provided 'as is' without warranty of any kind.       
Neither NEC nor their sales representatives can be held liable     
of any inconvenience or problem caused by its contents.        
	
@author		
@date		18/01/2011
@version	01.0

*/

#ifndef __uPD70F3281Y_H
#define __uPD70F3281Y_H

struct bitf {
	unsigned char bit00:1;
	unsigned char bit01:1;
	unsigned char bit02:1;
	unsigned char bit03:1;
	unsigned char bit04:1;
	unsigned char bit05:1;
	unsigned char bit06:1;
	unsigned char bit07:1;
	unsigned char bit08:1;
	unsigned char bit09:1;
	unsigned char bit10:1;
	unsigned char bit11:1;
	unsigned char bit12:1;
	unsigned char bit13:1;
	unsigned char bit14:1;
	unsigned char bit15:1;
};


struct bitcharf {								// bitfields declaration
	unsigned char bit00:1;
	unsigned char bit01:1;
	unsigned char bit02:1;
	unsigned char bit03:1;
	unsigned char bit04:1;
	unsigned char bit05:1;
	unsigned char bit06:1;
	unsigned char bit07:1;
};


/* I/O register */

#define	PDL     	*((volatile unsigned short *)0xfffff004)
#define	PDLL    	*((volatile unsigned char  *)0xfffff004)
#define	PDLH    	*((volatile unsigned char  *)0xfffff005)
#define	PDH     	*((volatile unsigned char  *)0xfffff006)
#define	PCT     	*((volatile unsigned char  *)0xfffff00a)
#define	PCM     	*((volatile unsigned char  *)0xfffff00c)
#define	PMDL    	*((volatile unsigned short *)0xfffff024)
#define	PMDLL   	*((volatile unsigned char  *)0xfffff024)
#define	PMDLH   	*((volatile unsigned char  *)0xfffff025)
#define	PMDH    	*((volatile unsigned char  *)0xfffff026)
#define	PMCT    	*((volatile unsigned char  *)0xfffff02a)
#define	PMCM    	*((volatile unsigned char  *)0xfffff02c)
#define	PMCDL   	*((volatile unsigned short *)0xfffff044)
#define	PMCDLL  	*((volatile unsigned char  *)0xfffff044)
#define	PMCDLH  	*((volatile unsigned char  *)0xfffff045)
#define	PMCDH   	*((volatile unsigned char  *)0xfffff046)
#define	PMCCT   	*((volatile unsigned char  *)0xfffff04a)
#define	PMCCM   	*((volatile unsigned char  *)0xfffff04c)
#define	BPC     	*((volatile unsigned short *)0xfffff064)
#define	BSC     	*((volatile unsigned short *)0xfffff066)
#define	VSWC    	*((volatile unsigned char  *)0xfffff06e)
#define	DSA0L   	*((volatile unsigned short *)0xfffff080)
#define	DSA0H   	*((volatile unsigned short *)0xfffff082)
#define	DDA0L   	*((volatile unsigned short *)0xfffff084)
#define	DDA0H   	*((volatile unsigned short *)0xfffff086)
#define	DSA1L   	*((volatile unsigned short *)0xfffff088)
#define	DSA1H   	*((volatile unsigned short *)0xfffff08a)
#define	DDA1L   	*((volatile unsigned short *)0xfffff08c)
#define	DDA1H   	*((volatile unsigned short *)0xfffff08e)
#define	DSA2L   	*((volatile unsigned short *)0xfffff090)
#define	DSA2H   	*((volatile unsigned short *)0xfffff092)
#define	DDA2L   	*((volatile unsigned short *)0xfffff094)
#define	DDA2H   	*((volatile unsigned short *)0xfffff096)
#define	DSA3L   	*((volatile unsigned short *)0xfffff098)
#define	DSA3H   	*((volatile unsigned short *)0xfffff09a)
#define	DDA3L   	*((volatile unsigned short *)0xfffff09c)
#define	DDA3H   	*((volatile unsigned short *)0xfffff09e)
#define	DBC0    	*((volatile unsigned short *)0xfffff0c0)
#define	DBC1    	*((volatile unsigned short *)0xfffff0c2)
#define	DBC2    	*((volatile unsigned short *)0xfffff0c4)
#define	DBC3    	*((volatile unsigned short *)0xfffff0c6)
#define	DADC0   	*((volatile unsigned short *)0xfffff0d0)
#define	DADC1   	*((volatile unsigned short *)0xfffff0d2)
#define	DADC2   	*((volatile unsigned short *)0xfffff0d4)
#define	DADC3   	*((volatile unsigned short *)0xfffff0d6)
#define	DCHC0   	*((volatile unsigned char  *)0xfffff0e0)
#define	DCHC1   	*((volatile unsigned char  *)0xfffff0e2)
#define	DCHC2   	*((volatile unsigned char  *)0xfffff0e4)
#define	DCHC3   	*((volatile unsigned char  *)0xfffff0e6)
#define	IMR0    	*((volatile unsigned short *)0xfffff100)
#define	IMR0L   	*((volatile unsigned char  *)0xfffff100)
#define	IMR0H   	*((volatile unsigned char  *)0xfffff101)
#define	IMR1    	*((volatile unsigned short *)0xfffff102)
#define	IMR1L   	*((volatile unsigned char  *)0xfffff102)
#define	IMR1H   	*((volatile unsigned char  *)0xfffff103)
#define	IMR2    	*((volatile unsigned short *)0xfffff104)
#define	IMR2L   	*((volatile unsigned char  *)0xfffff104)
#define	IMR2H   	*((volatile unsigned char  *)0xfffff105)
#define	IMR3    	*((volatile unsigned short *)0xfffff106)
#define	IMR3L   	*((volatile unsigned char  *)0xfffff106)
#define	IMR3H   	*((volatile unsigned char  *)0xfffff107)
#define	LVIIC   	*((volatile unsigned char  *)0xfffff110)
#define	PIC0    	*((volatile unsigned char  *)0xfffff112)
#define	PIC1    	*((volatile unsigned char  *)0xfffff114)
#define	PIC2    	*((volatile unsigned char  *)0xfffff116)
#define	PIC3    	*((volatile unsigned char  *)0xfffff118)
#define	PIC4    	*((volatile unsigned char  *)0xfffff11a)
#define	PIC5    	*((volatile unsigned char  *)0xfffff11c)
#define	PIC6    	*((volatile unsigned char  *)0xfffff11e)
#define	PIC7    	*((volatile unsigned char  *)0xfffff120)
#define	TQ0OVIC 	*((volatile unsigned char  *)0xfffff122)
#define	TQ0CCIC0	*((volatile unsigned char  *)0xfffff124)
#define	TQ0CCIC1	*((volatile unsigned char  *)0xfffff126)
#define	TQ0CCIC2	*((volatile unsigned char  *)0xfffff128)
#define	TQ0CCIC3	*((volatile unsigned char  *)0xfffff12a)
#define	TP0OVIC 	*((volatile unsigned char  *)0xfffff12c)
#define	TP0CCIC0	*((volatile unsigned char  *)0xfffff12e)
#define	TP0CCIC1	*((volatile unsigned char  *)0xfffff130)
#define	TP1OVIC 	*((volatile unsigned char  *)0xfffff132)
#define	TP1CCIC0	*((volatile unsigned char  *)0xfffff134)
#define	TP1CCIC1	*((volatile unsigned char  *)0xfffff136)
#define	TP2OVIC 	*((volatile unsigned char  *)0xfffff138)
#define	TP2CCIC0	*((volatile unsigned char  *)0xfffff13a)
#define	TP2CCIC1	*((volatile unsigned char  *)0xfffff13c)
#define	TP3OVIC 	*((volatile unsigned char  *)0xfffff13e)
#define	TP3CCIC0	*((volatile unsigned char  *)0xfffff140)
#define	TP3CCIC1	*((volatile unsigned char  *)0xfffff142)
#define	TP4OVIC 	*((volatile unsigned char  *)0xfffff144)
#define	TP4CCIC0	*((volatile unsigned char  *)0xfffff146)
#define	TP4CCIC1	*((volatile unsigned char  *)0xfffff148)
#define	TP5OVIC 	*((volatile unsigned char  *)0xfffff14a)
#define	TP5CCIC0	*((volatile unsigned char  *)0xfffff14c)
#define	TP5CCIC1	*((volatile unsigned char  *)0xfffff14e)
#define	TM0EQIC0	*((volatile unsigned char  *)0xfffff150)
#define	CB0RIC  	*((volatile unsigned char  *)0xfffff152)
#define	IICIC1  	*((volatile unsigned char  *)0xfffff152)
#define	CB0TIC  	*((volatile unsigned char  *)0xfffff154)
#define	CB1RIC  	*((volatile unsigned char  *)0xfffff156)
#define	CB1TIC  	*((volatile unsigned char  *)0xfffff158)
#define	CB2RIC  	*((volatile unsigned char  *)0xfffff15a)
#define	CB2TIC  	*((volatile unsigned char  *)0xfffff15c)
#define	CB3RIC  	*((volatile unsigned char  *)0xfffff15e)
#define	CB3TIC  	*((volatile unsigned char  *)0xfffff160)
#define	CB4RIC  	*((volatile unsigned char  *)0xfffff162)
#define	UA0RIC  	*((volatile unsigned char  *)0xfffff162)
#define	CB4TIC  	*((volatile unsigned char  *)0xfffff164)
#define	UA0TIC  	*((volatile unsigned char  *)0xfffff164)
#define	IICIC2  	*((volatile unsigned char  *)0xfffff166)
#define	UA1RIC  	*((volatile unsigned char  *)0xfffff166)
#define	UA1TIC  	*((volatile unsigned char  *)0xfffff168)
#define	IICIC0  	*((volatile unsigned char  *)0xfffff16a)
#define	UA2RIC  	*((volatile unsigned char  *)0xfffff16a)
#define	UA2TIC  	*((volatile unsigned char  *)0xfffff16c)
#define	ADIC    	*((volatile unsigned char  *)0xfffff16e)
#define	DMAIC0  	*((volatile unsigned char  *)0xfffff170)
#define	DMAIC1  	*((volatile unsigned char  *)0xfffff172)
#define	DMAIC2  	*((volatile unsigned char  *)0xfffff174)
#define	DMAIC3  	*((volatile unsigned char  *)0xfffff176)
#define	KRIC    	*((volatile unsigned char  *)0xfffff178)
#define	WTIIC   	*((volatile unsigned char  *)0xfffff17a)
#define	WTIC    	*((volatile unsigned char  *)0xfffff17c)
#define	ERRIC0  	*((volatile unsigned char  *)0xfffff17e)
#define	WUPIC0  	*((volatile unsigned char  *)0xfffff180)
#define	RECIC0  	*((volatile unsigned char  *)0xfffff182)
#define	TRXIC0  	*((volatile unsigned char  *)0xfffff184)
#define	ISPR    	*((volatile unsigned char  *)0xfffff1fa)
#define	PRCMD   	*((volatile unsigned char  *)0xfffff1fc)
#define	PSC     	*((volatile unsigned char  *)0xfffff1fe)
#define	ADA0M0  	*((volatile unsigned char  *)0xfffff200)
#define	ADA0M1  	*((volatile unsigned char  *)0xfffff201)
#define	ADA0S   	*((volatile unsigned char  *)0xfffff202)
#define	ADA0M2  	*((volatile unsigned char  *)0xfffff203)
#define	ADA0PFM 	*((volatile unsigned char  *)0xfffff204)
#define	ADA0PFT 	*((volatile unsigned char  *)0xfffff205)
#define	ADA0CR0 	*((volatile unsigned short *)0xfffff210)
#define	ADA0CR0H	*((volatile unsigned char  *)0xfffff211)
#define	ADA0CR1 	*((volatile unsigned short *)0xfffff212)
#define	ADA0CR1H	*((volatile unsigned char  *)0xfffff213)
#define	ADA0CR2 	*((volatile unsigned short *)0xfffff214)
#define	ADA0CR2H	*((volatile unsigned char  *)0xfffff215)
#define	ADA0CR3 	*((volatile unsigned short *)0xfffff216)
#define	ADA0CR3H	*((volatile unsigned char  *)0xfffff217)
#define	ADA0CR4 	*((volatile unsigned short *)0xfffff218)
#define	ADA0CR4H	*((volatile unsigned char  *)0xfffff219)
#define	ADA0CR5 	*((volatile unsigned short *)0xfffff21a)
#define	ADA0CR5H	*((volatile unsigned char  *)0xfffff21b)
#define	ADA0CR6 	*((volatile unsigned short *)0xfffff21c)
#define	ADA0CR6H	*((volatile unsigned char  *)0xfffff21d)
#define	ADA0CR7 	*((volatile unsigned short *)0xfffff21e)
#define	ADA0CR7H	*((volatile unsigned char  *)0xfffff21f)
#define	ADA0CR8 	*((volatile unsigned short *)0xfffff220)
#define	ADA0CR8H	*((volatile unsigned char  *)0xfffff221)
#define	ADA0CR9 	*((volatile unsigned short *)0xfffff222)
#define	ADA0CR9H	*((volatile unsigned char  *)0xfffff223)
#define	ADA0CR10	*((volatile unsigned short *)0xfffff224)
#define	ADA0CR10H	*((volatile unsigned char  *)0xfffff225)
#define	ADA0CR11	*((volatile unsigned short *)0xfffff226)
#define	ADA0CR11H	*((volatile unsigned char  *)0xfffff227)
#define	DA0CS0  	*((volatile unsigned char  *)0xfffff280)
#define	DA0CS1  	*((volatile unsigned char  *)0xfffff281)
#define	DA0M    	*((volatile unsigned char  *)0xfffff282)
#define	KRM     	*((volatile unsigned char  *)0xfffff300)
#define	SELCNT0 	*((volatile unsigned char  *)0xfffff308)
#define	CRCIN   	*((volatile unsigned char  *)0xfffff310)
#define	CRCD    	*((volatile unsigned short *)0xfffff312)
#define	NFC     	*((volatile unsigned char  *)0xfffff318)
#define	PRSM1   	*((volatile unsigned char  *)0xfffff320)
#define	PRSCM1  	*((volatile unsigned char  *)0xfffff321)
#define	PRSM2   	*((volatile unsigned char  *)0xfffff324)
#define	PRSCM2  	*((volatile unsigned char  *)0xfffff325)
#define	PRSM3   	*((volatile unsigned char  *)0xfffff328)
#define	PRSCM3  	*((volatile unsigned char  *)0xfffff329)
#define	OCKS0   	*((volatile unsigned char  *)0xfffff340)
#define	OCKS1   	*((volatile unsigned char  *)0xfffff344)
#define	P0      	*((volatile unsigned char  *)0xfffff400)
#define	P1      	*((volatile unsigned char  *)0xfffff402)
#define	P3      	*((volatile unsigned short *)0xfffff406)
#define	P3L     	*((volatile unsigned char  *)0xfffff406)
#define	P3H     	*((volatile unsigned char  *)0xfffff407)
#define	P4      	*((volatile unsigned char  *)0xfffff408)
#define	P5      	*((volatile unsigned char  *)0xfffff40a)
#define	P7L     	*((volatile unsigned char  *)0xfffff40e)
#define	P7H     	*((volatile unsigned char  *)0xfffff40f)
#define	P9      	*((volatile unsigned short *)0xfffff412)
#define	P9L     	*((volatile unsigned char  *)0xfffff412)
#define	P9H     	*((volatile unsigned char  *)0xfffff413)
#define	PM0     	*((volatile unsigned char  *)0xfffff420)
#define	PM1     	*((volatile unsigned char  *)0xfffff422)
#define	PM3     	*((volatile unsigned short *)0xfffff426)
#define	PM3L    	*((volatile unsigned char  *)0xfffff426)
#define	PM3H    	*((volatile unsigned char  *)0xfffff427)
#define	PM4     	*((volatile unsigned char  *)0xfffff428)
#define	PM5     	*((volatile unsigned char  *)0xfffff42a)
#define	PM7L    	*((volatile unsigned char  *)0xfffff42e)
#define	PM7H    	*((volatile unsigned char  *)0xfffff42f)
#define	PM9     	*((volatile unsigned short *)0xfffff432)
#define	PM9L    	*((volatile unsigned char  *)0xfffff432)
#define	PM9H    	*((volatile unsigned char  *)0xfffff433)
#define	PMC0    	*((volatile unsigned char  *)0xfffff440)
#define	PMC3    	*((volatile unsigned short *)0xfffff446)
#define	PMC3L   	*((volatile unsigned char  *)0xfffff446)
#define	PMC3H   	*((volatile unsigned char  *)0xfffff447)
#define	PMC4    	*((volatile unsigned char  *)0xfffff448)
#define	PMC5    	*((volatile unsigned char  *)0xfffff44a)
#define	PMC9    	*((volatile unsigned short *)0xfffff452)
#define	PMC9L   	*((volatile unsigned char  *)0xfffff452)
#define	PMC9H   	*((volatile unsigned char  *)0xfffff453)
#define	PFC0    	*((volatile unsigned char  *)0xfffff460)
#define	PFC3    	*((volatile unsigned short *)0xfffff466)
#define	PFC3L   	*((volatile unsigned char  *)0xfffff466)
#define	PFC3H   	*((volatile unsigned char  *)0xfffff467)
#define	PFC4    	*((volatile unsigned char  *)0xfffff468)
#define	PFC5    	*((volatile unsigned char  *)0xfffff46a)
#define	PFC9    	*((volatile unsigned short *)0xfffff472)
#define	PFC9L   	*((volatile unsigned char  *)0xfffff472)
#define	PFC9H   	*((volatile unsigned char  *)0xfffff473)
#define	DWC0    	*((volatile unsigned short *)0xfffff484)
#define	AWC     	*((volatile unsigned short *)0xfffff488)
#define	BCC     	*((volatile unsigned short *)0xfffff48a)
#define	TQ0CTL0 	*((volatile unsigned char  *)0xfffff540)
#define	TQ0CTL1 	*((volatile unsigned char  *)0xfffff541)
#define	TQ0IOC0 	*((volatile unsigned char  *)0xfffff542)
#define	TQ0IOC1 	*((volatile unsigned char  *)0xfffff543)
#define	TQ0IOC2 	*((volatile unsigned char  *)0xfffff544)
#define	TQ0OPT0 	*((volatile unsigned char  *)0xfffff545)
#define	TQ0CCR0 	*((volatile unsigned short *)0xfffff546)
#define	TQ0CCR1 	*((volatile unsigned short *)0xfffff548)
#define	TQ0CCR2 	*((volatile unsigned short *)0xfffff54a)
#define	TQ0CCR3 	*((volatile unsigned short *)0xfffff54c)
#define	TQ0CNT  	*((volatile unsigned short *)0xfffff54e)
#define	TP0CTL0 	*((volatile unsigned char  *)0xfffff590)
#define	TP0CTL1 	*((volatile unsigned char  *)0xfffff591)
#define	TP0IOC0 	*((volatile unsigned char  *)0xfffff592)
#define	TP0IOC1 	*((volatile unsigned char  *)0xfffff593)
#define	TP0IOC2 	*((volatile unsigned char  *)0xfffff594)
#define	TP0OPT0 	*((volatile unsigned char  *)0xfffff595)
#define	TP0CCR0 	*((volatile unsigned short *)0xfffff596)
#define	TP0CCR1 	*((volatile unsigned short *)0xfffff598)
#define	TP0CNT  	*((volatile unsigned short *)0xfffff59a)
#define	TP1CTL0 	*((volatile unsigned char  *)0xfffff5a0)
#define	TP1CTL1 	*((volatile unsigned char  *)0xfffff5a1)
#define	TP1IOC0 	*((volatile unsigned char  *)0xfffff5a2)
#define	TP1IOC1 	*((volatile unsigned char  *)0xfffff5a3)
#define	TP1IOC2 	*((volatile unsigned char  *)0xfffff5a4)
#define	TP1OPT0 	*((volatile unsigned char  *)0xfffff5a5)
#define	TP1CCR0 	*((volatile unsigned short *)0xfffff5a6)
#define	TP1CCR1 	*((volatile unsigned short *)0xfffff5a8)
#define	TP1CNT  	*((volatile unsigned short *)0xfffff5aa)
#define	TP2CTL0 	*((volatile unsigned char  *)0xfffff5b0)
#define	TP2CTL1 	*((volatile unsigned char  *)0xfffff5b1)
#define	TP2IOC0 	*((volatile unsigned char  *)0xfffff5b2)
#define	TP2IOC1 	*((volatile unsigned char  *)0xfffff5b3)
#define	TP2IOC2 	*((volatile unsigned char  *)0xfffff5b4)
#define	TP2OPT0 	*((volatile unsigned char  *)0xfffff5b5)
#define	TP2CCR0 	*((volatile unsigned short *)0xfffff5b6)
#define	TP2CCR1 	*((volatile unsigned short *)0xfffff5b8)
#define	TP2CNT  	*((volatile unsigned short *)0xfffff5ba)
#define	TP3CTL0 	*((volatile unsigned char  *)0xfffff5c0)
#define	TP3CTL1 	*((volatile unsigned char  *)0xfffff5c1)
#define	TP3IOC0 	*((volatile unsigned char  *)0xfffff5c2)
#define	TP3IOC1 	*((volatile unsigned char  *)0xfffff5c3)
#define	TP3IOC2 	*((volatile unsigned char  *)0xfffff5c4)
#define	TP3OPT0 	*((volatile unsigned char  *)0xfffff5c5)
#define	TP3CCR0 	*((volatile unsigned short *)0xfffff5c6)
#define	TP3CCR1 	*((volatile unsigned short *)0xfffff5c8)
#define	TP3CNT  	*((volatile unsigned short *)0xfffff5ca)
#define	TP4CTL0 	*((volatile unsigned char  *)0xfffff5d0)
#define	TP4CTL1 	*((volatile unsigned char  *)0xfffff5d1)
#define	TP4IOC0 	*((volatile unsigned char  *)0xfffff5d2)
#define	TP4IOC1 	*((volatile unsigned char  *)0xfffff5d3)
#define	TP4IOC2 	*((volatile unsigned char  *)0xfffff5d4)
#define	TP4OPT0 	*((volatile unsigned char  *)0xfffff5d5)
#define	TP4CCR0 	*((volatile unsigned short *)0xfffff5d6)
#define	TP4CCR1 	*((volatile unsigned short *)0xfffff5d8)
#define	TP4CNT  	*((volatile unsigned short *)0xfffff5da)
#define	TP5CTL0 	*((volatile unsigned char  *)0xfffff5e0)
#define	TP5CTL1 	*((volatile unsigned char  *)0xfffff5e1)
#define	TP5IOC0 	*((volatile unsigned char  *)0xfffff5e2)
#define	TP5IOC1 	*((volatile unsigned char  *)0xfffff5e3)
#define	TP5IOC2 	*((volatile unsigned char  *)0xfffff5e4)
#define	TP5OPT0 	*((volatile unsigned char  *)0xfffff5e5)
#define	TP5CCR0 	*((volatile unsigned short *)0xfffff5e6)
#define	TP5CCR1 	*((volatile unsigned short *)0xfffff5e8)
#define	TP5CNT  	*((volatile unsigned short *)0xfffff5ea)
#define	WTM     	*((volatile unsigned char  *)0xfffff680)
#define	TM0CTL0 	*((volatile unsigned char  *)0xfffff690)
#define	TM0CMP0 	*((volatile unsigned short *)0xfffff694)
#define	OSTS    	*((volatile unsigned char  *)0xfffff6c0)
#define	PLLS    	*((volatile unsigned char  *)0xfffff6c1)
#define	WDTM2   	*((volatile unsigned char  *)0xfffff6d0)
#define	WDTE    	*((volatile unsigned char  *)0xfffff6d1)
#define	RTBL0   	*((volatile unsigned char  *)0xfffff6e0)
#define	RTBH0   	*((volatile unsigned char  *)0xfffff6e2)
#define	RTPM0   	*((volatile unsigned char  *)0xfffff6e4)
#define	RTPC0   	*((volatile unsigned char  *)0xfffff6e5)
#define	PFCE3L  	*((volatile unsigned char  *)0xfffff706)
#define	PFCE5   	*((volatile unsigned char  *)0xfffff70a)
#define	PFCE9   	*((volatile unsigned short *)0xfffff712)
#define	PFCE9L  	*((volatile unsigned char  *)0xfffff712)
#define	PFCE9H  	*((volatile unsigned char  *)0xfffff713)
#define	SYS     	*((volatile unsigned char  *)0xfffff802)
#define	RCM     	*((volatile unsigned char  *)0xfffff80c)
#define	DTFR0   	*((volatile unsigned char  *)0xfffff810)
#define	DTFR1   	*((volatile unsigned char  *)0xfffff812)
#define	DTFR2   	*((volatile unsigned char  *)0xfffff814)
#define	DTFR3   	*((volatile unsigned char  *)0xfffff816)
#define	PSMR    	*((volatile unsigned char  *)0xfffff820)
#define	CKC     	*((volatile unsigned char  *)0xfffff822)
#define	LOCKR   	*((volatile unsigned char  *)0xfffff824)
#define	PCC     	*((volatile unsigned char  *)0xfffff828)
#define	PLLCTL  	*((volatile unsigned char  *)0xfffff82c)
#define	CCLS    	*((volatile unsigned char  *)0xfffff82e)
#define	CORAD0  	*((volatile unsigned long  *)0xfffff840)
#define	CORAD0L 	*((volatile unsigned short *)0xfffff840)
#define	CORAD0H 	*((volatile unsigned short *)0xfffff842)
#define	CORAD1  	*((volatile unsigned long  *)0xfffff844)
#define	CORAD1L 	*((volatile unsigned short *)0xfffff844)
#define	CORAD1H 	*((volatile unsigned short *)0xfffff846)
#define	CORAD2  	*((volatile unsigned long  *)0xfffff848)
#define	CORAD2L 	*((volatile unsigned short *)0xfffff848)
#define	CORAD2H 	*((volatile unsigned short *)0xfffff84a)
#define	CORAD3  	*((volatile unsigned long  *)0xfffff84c)
#define	CORAD3L 	*((volatile unsigned short *)0xfffff84c)
#define	CORAD3H 	*((volatile unsigned short *)0xfffff84e)
#define	CLM     	*((volatile unsigned char  *)0xfffff870)
#define	CORCN   	*((volatile unsigned char  *)0xfffff880)
#define	RESF    	*((volatile unsigned char  *)0xfffff888)
#define	LVIM    	*((volatile unsigned char  *)0xfffff890)
#define	LVIS    	*((volatile unsigned char  *)0xfffff891)
#define	RAMS    	*((volatile unsigned char  *)0xfffff892)
#define	PRSM0   	*((volatile unsigned char  *)0xfffff8b0)
#define	PRSCM0  	*((volatile unsigned char  *)0xfffff8b1)
#define	OCDM    	*((volatile unsigned char  *)0xfffff9fc)
#define	PEMU1   	*((volatile unsigned char  *)0xfffff9fe)
#define	UA0CTL0 	*((volatile unsigned char  *)0xfffffa00)
#define	UA0CTL1 	*((volatile unsigned char  *)0xfffffa01)
#define	UA0CTL2 	*((volatile unsigned char  *)0xfffffa02)
#define	UA0OPT0 	*((volatile unsigned char  *)0xfffffa03)
#define	UA0STR  	*((volatile unsigned char  *)0xfffffa04)
#define	UA0RX   	*((volatile unsigned char  *)0xfffffa06)
#define	UA0TX   	*((volatile unsigned char  *)0xfffffa07)
#define	UA1CTL0 	*((volatile unsigned char  *)0xfffffa10)
#define	UA1CTL1 	*((volatile unsigned char  *)0xfffffa11)
#define	UA1CTL2 	*((volatile unsigned char  *)0xfffffa12)
#define	UA1OPT0 	*((volatile unsigned char  *)0xfffffa13)
#define	UA1STR  	*((volatile unsigned char  *)0xfffffa14)
#define	UA1RX   	*((volatile unsigned char  *)0xfffffa16)
#define	UA1TX   	*((volatile unsigned char  *)0xfffffa17)
#define	UA2CTL0 	*((volatile unsigned char  *)0xfffffa20)
#define	UA2CTL1 	*((volatile unsigned char  *)0xfffffa21)
#define	UA2CTL2 	*((volatile unsigned char  *)0xfffffa22)
#define	UA2OPT0 	*((volatile unsigned char  *)0xfffffa23)
#define	UA2STR  	*((volatile unsigned char  *)0xfffffa24)
#define	UA2RX   	*((volatile unsigned char  *)0xfffffa26)
#define	UA2TX   	*((volatile unsigned char  *)0xfffffa27)
#define	INTF0   	*((volatile unsigned char  *)0xfffffc00)
#define	INTF3   	*((volatile unsigned char  *)0xfffffc06)
#define	INTF9H  	*((volatile unsigned char  *)0xfffffc13)
#define	INTR0   	*((volatile unsigned char  *)0xfffffc20)
#define	INTR3   	*((volatile unsigned char  *)0xfffffc26)
#define	INTR9H  	*((volatile unsigned char  *)0xfffffc33)
#define	PF0     	*((volatile unsigned char  *)0xfffffc60)
#define	PF3     	*((volatile unsigned short *)0xfffffc66)
#define	PF3L    	*((volatile unsigned char  *)0xfffffc66)
#define	PF3H    	*((volatile unsigned char  *)0xfffffc67)
#define	PF4     	*((volatile unsigned char  *)0xfffffc68)
#define	PF5     	*((volatile unsigned char  *)0xfffffc6a)
#define	PF9     	*((volatile unsigned short *)0xfffffc72)
#define	PF9L    	*((volatile unsigned char  *)0xfffffc72)
#define	PF9H    	*((volatile unsigned char  *)0xfffffc73)
#define	CB0CTL0 	*((volatile unsigned char  *)0xfffffd00)
#define	CB0CTL1 	*((volatile unsigned char  *)0xfffffd01)
#define	CB0CTL2 	*((volatile unsigned char  *)0xfffffd02)
#define	CB0STR  	*((volatile unsigned char  *)0xfffffd03)
#define	CB0RX   	*((volatile unsigned short *)0xfffffd04)
#define	CB0RXL  	*((volatile unsigned char  *)0xfffffd04)
#define	CB0TX   	*((volatile unsigned short *)0xfffffd06)
#define	CB0TXL  	*((volatile unsigned char  *)0xfffffd06)
#define	CB1CTL0 	*((volatile unsigned char  *)0xfffffd10)
#define	CB1CTL1 	*((volatile unsigned char  *)0xfffffd11)
#define	CB1CTL2 	*((volatile unsigned char  *)0xfffffd12)
#define	CB1STR  	*((volatile unsigned char  *)0xfffffd13)
#define	CB1RX   	*((volatile unsigned short *)0xfffffd14)
#define	CB1RXL  	*((volatile unsigned char  *)0xfffffd14)
#define	CB1TX   	*((volatile unsigned short *)0xfffffd16)
#define	CB1TXL  	*((volatile unsigned char  *)0xfffffd16)
#define	CB2CTL0 	*((volatile unsigned char  *)0xfffffd20)
#define	CB2CTL1 	*((volatile unsigned char  *)0xfffffd21)
#define	CB2CTL2 	*((volatile unsigned char  *)0xfffffd22)
#define	CB2STR  	*((volatile unsigned char  *)0xfffffd23)
#define	CB2RX   	*((volatile unsigned short *)0xfffffd24)
#define	CB2RXL  	*((volatile unsigned char  *)0xfffffd24)
#define	CB2TX   	*((volatile unsigned short *)0xfffffd26)
#define	CB2TXL  	*((volatile unsigned char  *)0xfffffd26)
#define	CB3CTL0 	*((volatile unsigned char  *)0xfffffd30)
#define	CB3CTL1 	*((volatile unsigned char  *)0xfffffd31)
#define	CB3CTL2 	*((volatile unsigned char  *)0xfffffd32)
#define	CB3STR  	*((volatile unsigned char  *)0xfffffd33)
#define	CB3RX   	*((volatile unsigned short *)0xfffffd34)
#define	CB3RXL  	*((volatile unsigned char  *)0xfffffd34)
#define	CB3TX   	*((volatile unsigned short *)0xfffffd36)
#define	CB3TXL  	*((volatile unsigned char  *)0xfffffd36)
#define	CB4CTL0 	*((volatile unsigned char  *)0xfffffd40)
#define	CB4CTL1 	*((volatile unsigned char  *)0xfffffd41)
#define	CB4CTL2 	*((volatile unsigned char  *)0xfffffd42)
#define	CB4STR  	*((volatile unsigned char  *)0xfffffd43)
#define	CB4RX   	*((volatile unsigned short *)0xfffffd44)
#define	CB4RXL  	*((volatile unsigned char  *)0xfffffd44)
#define	CB4TX   	*((volatile unsigned short *)0xfffffd46)
#define	CB4TXL  	*((volatile unsigned char  *)0xfffffd46)
#define	IIC0    	*((volatile unsigned char  *)0xfffffd80)
#define	IICC0   	*((volatile unsigned char  *)0xfffffd82)
#define	SVA0    	*((volatile unsigned char  *)0xfffffd83)
#define	IICCL0  	*((volatile unsigned char  *)0xfffffd84)
#define	IICX0   	*((volatile unsigned char  *)0xfffffd85)
#define	IICS0   	*((volatile unsigned char  *)0xfffffd86)
#define	IICF0   	*((volatile unsigned char  *)0xfffffd8a)
#define	IIC1    	*((volatile unsigned char  *)0xfffffd90)
#define	IICC1   	*((volatile unsigned char  *)0xfffffd92)
#define	SVA1    	*((volatile unsigned char  *)0xfffffd93)
#define	IICCL1  	*((volatile unsigned char  *)0xfffffd94)
#define	IICX1   	*((volatile unsigned char  *)0xfffffd95)
#define	IICS1   	*((volatile unsigned char  *)0xfffffd96)
#define	IICF1   	*((volatile unsigned char  *)0xfffffd9a)
#define	IIC2    	*((volatile unsigned char  *)0xfffffda0)
#define	IICC2   	*((volatile unsigned char  *)0xfffffda2)
#define	SVA2    	*((volatile unsigned char  *)0xfffffda3)
#define	IICCL2  	*((volatile unsigned char  *)0xfffffda4)
#define	IICX2   	*((volatile unsigned char  *)0xfffffda5)
#define	IICS2   	*((volatile unsigned char  *)0xfffffda6)
#define	IICF2   	*((volatile unsigned char  *)0xfffffdaa)
#define	EXIMC   	*((volatile unsigned char  *)0xffffffbe)


/* I/O register bit */

#define	_E00     	((volatile struct bitf *)0xfffff0e0)->bit00
#define	_STG0    	((volatile struct bitf *)0xfffff0e0)->bit01
#define	_INIT0   	((volatile struct bitf *)0xfffff0e0)->bit02
#define	_TC0     	((volatile struct bitf *)0xfffff0e0)->bit07

#define	_E11     	((volatile struct bitf *)0xfffff0e2)->bit00
#define	_STG1    	((volatile struct bitf *)0xfffff0e2)->bit01
#define	_INIT1   	((volatile struct bitf *)0xfffff0e2)->bit02
#define	_TC1     	((volatile struct bitf *)0xfffff0e2)->bit07

#define	_E22     	((volatile struct bitf *)0xfffff0e4)->bit00
#define	_STG2    	((volatile struct bitf *)0xfffff0e4)->bit01
#define	_INIT2   	((volatile struct bitf *)0xfffff0e4)->bit02
#define	_TC2     	((volatile struct bitf *)0xfffff0e4)->bit07

#define	_E33     	((volatile struct bitf *)0xfffff0e6)->bit00
#define	_STG3    	((volatile struct bitf *)0xfffff0e6)->bit01
#define	_INIT3   	((volatile struct bitf *)0xfffff0e6)->bit02
#define	_TC3     	((volatile struct bitf *)0xfffff0e6)->bit07

#define	_LVIMK   	((volatile struct bitf *)0xfffff110)->bit06
#define	_LVIIF   	((volatile struct bitf *)0xfffff110)->bit07

#define	_PMK0    	((volatile struct bitf *)0xfffff112)->bit06
#define	_PIF0    	((volatile struct bitf *)0xfffff112)->bit07

#define	_PMK1    	((volatile struct bitf *)0xfffff114)->bit06
#define	_PIF1    	((volatile struct bitf *)0xfffff114)->bit07

#define	_PMK2    	((volatile struct bitf *)0xfffff116)->bit06
#define	_PIF2    	((volatile struct bitf *)0xfffff116)->bit07

#define	_PMK3    	((volatile struct bitf *)0xfffff118)->bit06
#define	_PIF3    	((volatile struct bitf *)0xfffff118)->bit07

#define	_PMK4    	((volatile struct bitf *)0xfffff11a)->bit06
#define	_PIF4    	((volatile struct bitf *)0xfffff11a)->bit07

#define	_PMK5    	((volatile struct bitf *)0xfffff11c)->bit06
#define	_PIF5    	((volatile struct bitf *)0xfffff11c)->bit07

#define	_PMK6    	((volatile struct bitf *)0xfffff11e)->bit06
#define	_PIF6    	((volatile struct bitf *)0xfffff11e)->bit07

#define	_PMK7    	((volatile struct bitf *)0xfffff120)->bit06
#define	_PIF7    	((volatile struct bitf *)0xfffff120)->bit07

#define	_TQ0OVMK 	((volatile struct bitf *)0xfffff122)->bit06
#define	_TQ0OVIF 	((volatile struct bitf *)0xfffff122)->bit07

#define	_TQ0CCMK0	((volatile struct bitf *)0xfffff124)->bit06
#define	_TQ0CCIF0	((volatile struct bitf *)0xfffff124)->bit07

#define	_TQ0CCMK1	((volatile struct bitf *)0xfffff126)->bit06
#define	_TQ0CCIF1	((volatile struct bitf *)0xfffff126)->bit07

#define	_TQ0CCMK2	((volatile struct bitf *)0xfffff128)->bit06
#define	_TQ0CCIF2	((volatile struct bitf *)0xfffff128)->bit07

#define	_TQ0CCMK3	((volatile struct bitf *)0xfffff12a)->bit06
#define	_TQ0CCIF3	((volatile struct bitf *)0xfffff12a)->bit07

#define	_TP0OVMK 	((volatile struct bitf *)0xfffff12c)->bit06
#define	_TP0OVIF 	((volatile struct bitf *)0xfffff12c)->bit07

#define	_TP0CCMK0	((volatile struct bitf *)0xfffff12e)->bit06
#define	_TP0CCIF0	((volatile struct bitf *)0xfffff12e)->bit07

#define	_TP0CCMK1	((volatile struct bitf *)0xfffff130)->bit06
#define	_TP0CCIF1	((volatile struct bitf *)0xfffff130)->bit07

#define	_TP1OVMK 	((volatile struct bitf *)0xfffff132)->bit06
#define	_TP1OVIF 	((volatile struct bitf *)0xfffff132)->bit07

#define	_TP1CCMK0	((volatile struct bitf *)0xfffff134)->bit06
#define	_TP1CCIF0	((volatile struct bitf *)0xfffff134)->bit07

#define	_TP1CCMK1	((volatile struct bitf *)0xfffff136)->bit06
#define	_TP1CCIF1	((volatile struct bitf *)0xfffff136)->bit07

#define	_TP2OVMK 	((volatile struct bitf *)0xfffff138)->bit06
#define	_TP2OVIF 	((volatile struct bitf *)0xfffff138)->bit07

#define	_TP2CCMK0	((volatile struct bitf *)0xfffff13a)->bit06
#define	_TP2CCIF0	((volatile struct bitf *)0xfffff13a)->bit07

#define	_TP2CCMK1	((volatile struct bitf *)0xfffff13c)->bit06
#define	_TP2CCIF1	((volatile struct bitf *)0xfffff13c)->bit07

#define	_TP3OVMK 	((volatile struct bitf *)0xfffff13e)->bit06
#define	_TP3OVIF 	((volatile struct bitf *)0xfffff13e)->bit07

#define	_TP3CCMK0	((volatile struct bitf *)0xfffff140)->bit06
#define	_TP3CCIF0	((volatile struct bitf *)0xfffff140)->bit07

#define	_TP3CCMK1	((volatile struct bitf *)0xfffff142)->bit06
#define	_TP3CCIF1	((volatile struct bitf *)0xfffff142)->bit07

#define	_TP4OVMK 	((volatile struct bitf *)0xfffff144)->bit06
#define	_TP4OVIF 	((volatile struct bitf *)0xfffff144)->bit07

#define	_TP4CCMK0	((volatile struct bitf *)0xfffff146)->bit06
#define	_TP4CCIF0	((volatile struct bitf *)0xfffff146)->bit07

#define	_TP4CCMK1	((volatile struct bitf *)0xfffff148)->bit06
#define	_TP4CCIF1	((volatile struct bitf *)0xfffff148)->bit07

#define	_TP5OVMK 	((volatile struct bitf *)0xfffff14a)->bit06
#define	_TP5OVIF 	((volatile struct bitf *)0xfffff14a)->bit07

#define	_TP5CCMK0	((volatile struct bitf *)0xfffff14c)->bit06
#define	_TP5CCIF0	((volatile struct bitf *)0xfffff14c)->bit07

#define	_TP5CCMK1	((volatile struct bitf *)0xfffff14e)->bit06
#define	_TP5CCIF1	((volatile struct bitf *)0xfffff14e)->bit07

#define	_TM0EQMK0	((volatile struct bitf *)0xfffff150)->bit06
#define	_TM0EQIF0	((volatile struct bitf *)0xfffff150)->bit07

#define	_CB0RMK  	((volatile struct bitf *)0xfffff152)->bit06
#define	_CB0RIF  	((volatile struct bitf *)0xfffff152)->bit07
#define	_CB0RMK  	((volatile struct bitf *)0xfffff152)->bit06
#define	_CB0RIF  	((volatile struct bitf *)0xfffff152)->bit07

#define	_CB0TMK  	((volatile struct bitf *)0xfffff154)->bit06
#define	_CB0TIF  	((volatile struct bitf *)0xfffff154)->bit07

#define	_CB1RMK  	((volatile struct bitf *)0xfffff156)->bit06
#define	_CB1RIF  	((volatile struct bitf *)0xfffff156)->bit07

#define	_CB1TMK  	((volatile struct bitf *)0xfffff158)->bit06
#define	_CB1TIF  	((volatile struct bitf *)0xfffff158)->bit07

#define	_CB2RMK  	((volatile struct bitf *)0xfffff15a)->bit06
#define	_CB2RIF  	((volatile struct bitf *)0xfffff15a)->bit07

#define	_CB2TMK  	((volatile struct bitf *)0xfffff15c)->bit06
#define	_CB2TIF  	((volatile struct bitf *)0xfffff15c)->bit07

#define	_CB3RMK  	((volatile struct bitf *)0xfffff15e)->bit06
#define	_CB3RIF  	((volatile struct bitf *)0xfffff15e)->bit07

#define	_CB3TMK  	((volatile struct bitf *)0xfffff160)->bit06
#define	_CB3TIF  	((volatile struct bitf *)0xfffff160)->bit07

#define	_CB4RMK  	((volatile struct bitf *)0xfffff162)->bit06
#define	_CB4RIF  	((volatile struct bitf *)0xfffff162)->bit07

#define	_CB4RMK  	((volatile struct bitf *)0xfffff162)->bit06
#define	_CB4RIF  	((volatile struct bitf *)0xfffff162)->bit07

#define	_CB4TMK  	((volatile struct bitf *)0xfffff164)->bit06
#define	_CB4TIF  	((volatile struct bitf *)0xfffff164)->bit07
#define	_CB4TMK  	((volatile struct bitf *)0xfffff164)->bit06
#define	_CB4TIF  	((volatile struct bitf *)0xfffff164)->bit07

#define	_IICMK2  	((volatile struct bitf *)0xfffff166)->bit06
#define	_IICIF2  	((volatile struct bitf *)0xfffff166)->bit07
#define	_IICMK2  	((volatile struct bitf *)0xfffff166)->bit06
#define	_IICIF2  	((volatile struct bitf *)0xfffff166)->bit07

#define	_UA1TMK  	((volatile struct bitf *)0xfffff168)->bit06
#define	_UA1TIF  	((volatile struct bitf *)0xfffff168)->bit07

#define	_IICMK0  	((volatile struct bitf *)0xfffff16a)->bit06
#define	_IICIF0  	((volatile struct bitf *)0xfffff16a)->bit07
#define	_IICMK0  	((volatile struct bitf *)0xfffff16a)->bit06
#define	_IICIF0  	((volatile struct bitf *)0xfffff16a)->bit07

#define	_UA2TMK  	((volatile struct bitf *)0xfffff16c)->bit06
#define	_UA2TIF  	((volatile struct bitf *)0xfffff16c)->bit07

#define	_ADMK    	((volatile struct bitf *)0xfffff16e)->bit06
#define	_ADIF    	((volatile struct bitf *)0xfffff16e)->bit07

#define	_DMAMK0  	((volatile struct bitf *)0xfffff170)->bit06
#define	_DMAIF0  	((volatile struct bitf *)0xfffff170)->bit07

#define	_DMAMK1  	((volatile struct bitf *)0xfffff172)->bit06
#define	_DMAIF1  	((volatile struct bitf *)0xfffff172)->bit07

#define	_DMAMK2  	((volatile struct bitf *)0xfffff174)->bit06
#define	_DMAIF2  	((volatile struct bitf *)0xfffff174)->bit07

#define	_DMAMK3  	((volatile struct bitf *)0xfffff176)->bit06
#define	_DMAIF3  	((volatile struct bitf *)0xfffff176)->bit07

#define	_KRMK    	((volatile struct bitf *)0xfffff178)->bit06
#define	_KRIF    	((volatile struct bitf *)0xfffff178)->bit07

#define	_WTIMK   	((volatile struct bitf *)0xfffff17a)->bit06
#define	_WTIIF   	((volatile struct bitf *)0xfffff17a)->bit07

#define	_WTMK    	((volatile struct bitf *)0xfffff17c)->bit06
#define	_WTIF    	((volatile struct bitf *)0xfffff17c)->bit07

#define	_ERRMK0  	((volatile struct bitf *)0xfffff17e)->bit06
#define	_ERRIF0  	((volatile struct bitf *)0xfffff17e)->bit07

#define	_WUPMK0  	((volatile struct bitf *)0xfffff180)->bit06
#define	_WUPIF0  	((volatile struct bitf *)0xfffff180)->bit07

#define	_RECMK0  	((volatile struct bitf *)0xfffff182)->bit06
#define	_RECIF0  	((volatile struct bitf *)0xfffff182)->bit07

#define	_TRXMK0  	((volatile struct bitf *)0xfffff184)->bit06
#define	_TRXIF0  	((volatile struct bitf *)0xfffff184)->bit07

#define	_ISPR0   	((volatile struct bitf *)0xfffff1fa)->bit00
#define	_ISPR1   	((volatile struct bitf *)0xfffff1fa)->bit01
#define	_ISPR2   	((volatile struct bitf *)0xfffff1fa)->bit02
#define	_ISPR3   	((volatile struct bitf *)0xfffff1fa)->bit03
#define	_ISPR4   	((volatile struct bitf *)0xfffff1fa)->bit04
#define	_ISPR5   	((volatile struct bitf *)0xfffff1fa)->bit05
#define	_ISPR6   	((volatile struct bitf *)0xfffff1fa)->bit06
#define	_ISPR7   	((volatile struct bitf *)0xfffff1fa)->bit07

#define	_STP     	((volatile struct bitf *)0xfffff1fe)->bit01
#define	_INTM    	((volatile struct bitf *)0xfffff1fe)->bit04
#define	_NMI0M   	((volatile struct bitf *)0xfffff1fe)->bit05
#define	_NMI1M   	((volatile struct bitf *)0xfffff1fe)->bit06

#define	_ADA0EF  	((volatile struct bitf *)&ADA0M0)->bit00
#define	_ADA0TMD 	((volatile struct bitf *)&ADA0M0)->bit01
#define	_ADA0ETS0  	((volatile struct bitf *)&ADA0M0)->bit02
#define	_ADA0ETS1  	((volatile struct bitf *)&ADA0M0)->bit03
#define	_ADA0MD0 	((volatile struct bitf *)&ADA0M0)->bit04
#define	_ADA0MD1  	((volatile struct bitf *)&ADA0M0)->bit05
#define	_ADA0CE  	((volatile struct bitf *)&ADA0M0)->bit07

#define	_ADA0PFE 	((volatile struct bitf *)0xfffff204)->bit07

#define	_DA0CE0  	((volatile struct bitf *)0xfffff282)->bit04
#define	_DA0CE1  	((volatile struct bitf *)0xfffff282)->bit05

#define	_ISEL0   	((volatile struct bitf *)0xfffff308)->bit00
#define	_ISEL1   	((volatile struct bitf *)0xfffff308)->bit01
#define	_ISEL3   	((volatile struct bitf *)0xfffff308)->bit03
#define	_ISEL4   	((volatile struct bitf *)0xfffff308)->bit04
#define	_ISEL6   	((volatile struct bitf *)0xfffff308)->bit06

#define	_BGCE1   	((volatile struct bitf *)0xfffff320)->bit04

#define	_BGCE2   	((volatile struct bitf *)0xfffff324)->bit04

#define	_BGCE3   	((volatile struct bitf *)0xfffff328)->bit04

#define	_TQ0CKS0   	((volatile struct bitf *)&TQ0CTL0)->bit00
#define	_TQ0CKS1   	((volatile struct bitf *)&TQ0CTL0)->bit01
#define	_TQ0CKS2   	((volatile struct bitf *)&TQ0CTL0)->bit02
#define	_TQ0CE   	((volatile struct bitf *)&TQ0CTL0)->bit07

#define	_TQ0MD0 	((volatile struct bitf *)&TQ0CTL1)->bit00
#define	_TQ0MD1   	((volatile struct bitf *)&TQ0CTL1)->bit01
#define	_TQ0MD2  	((volatile struct bitf *)&TQ0CTL1)->bit02
#define	_TQ0EEE  	((volatile struct bitf *)&TQ0CTL1)->bit05
#define	_TQ0EST  	((volatile struct bitf *)&TQ0CTL1)->bit06

#define	_TQ0OE0  	((volatile struct bitf *)0xfffff542)->bit00
#define	_TQ0OE1  	((volatile struct bitf *)0xfffff542)->bit02
#define	_TQ0OE2  	((volatile struct bitf *)0xfffff542)->bit04
#define	_TQ0OE3  	((volatile struct bitf *)0xfffff542)->bit06

#define	_TQ0OVF  	((volatile struct bitf *)0xfffff545)->bit00

#define _TP0CKS0	((volatile struct bitf *)&TP0CTL0)->bit00
#define _TP0CKS1	((volatile struct bitf *)&TP0CTL0)->bit01
#define _TP0CKS2	((volatile struct bitf *)&TP0CTL0)->bit02
#define	_TP0CE   	((volatile struct bitf *)&TP0CTL0)->bit07

#define	_TP0MD0 	((volatile struct bitf *)&TP0CTL1)->bit00
#define	_TP0MD1   	((volatile struct bitf *)&TP0CTL1)->bit01
#define	_TP0MD2  	((volatile struct bitf *)&TP0CTL1)->bit02
#define	_TP0EEE  	((volatile struct bitf *)&TP0CTL1)->bit05
#define	_TP0EST  	((volatile struct bitf *)&TP0CTL1)->bit06

#define _TP1CKS0	((volatile struct bitf *)&TP1CTL0)->bit00
#define _TP1CKS1	((volatile struct bitf *)&TP1CTL0)->bit01
#define _TP1CKS2	((volatile struct bitf *)&TP1CTL0)->bit02

#define _TP3CKS0	((volatile struct bitf *)&TP3CTL0)->bit00
#define _TP3CKS1	((volatile struct bitf *)&TP3CTL0)->bit01
#define _TP3CKS2	((volatile struct bitf *)&TP3CTL0)->bit02

//#define	_TP1CE   	((volatile struct bitf *)&TP1CTL0)->bit07

#define	_TP1MD0 	((volatile struct bitf *)&TP1CTL1)->bit00
#define	_TP1MD1   	((volatile struct bitf *)&TP1CTL1)->bit01
#define	_TP1MD2  	((volatile struct bitf *)&TP1CTL1)->bit02
#define	_TP1EEE  	((volatile struct bitf *)&TP1CTL1)->bit05
#define	_TP1EST  	((volatile struct bitf *)&TP1CTL1)->bit06

#define	_TP3MD0 	((volatile struct bitf *)&TP3CTL1)->bit00
#define	_TP3MD1   	((volatile struct bitf *)&TP3CTL1)->bit01
#define	_TP3MD2  	((volatile struct bitf *)&TP3CTL1)->bit02
#define	_TP3EEE  	((volatile struct bitf *)&TP3CTL1)->bit05
#define	_TP3EST  	((volatile struct bitf *)&TP3CTL1)->bit06


#define _TP4CKS0	((volatile struct bitf *)&TP4CTL0)->bit00
#define _TP4CKS1	((volatile struct bitf *)&TP4CTL0)->bit01
#define _TP4CKS2	((volatile struct bitf *)&TP4CTL0)->bit02

#define	_TP4MD0 	((volatile struct bitf *)&TP4CTL1)->bit00
#define	_TP4MD1   	((volatile struct bitf *)&TP4CTL1)->bit01
#define	_TP4MD2  	((volatile struct bitf *)&TP4CTL1)->bit02

#define	_TP4OL0  	((volatile struct bitf *)&TP4IOC0)->bit01
#define	_TP4OL1  	((volatile struct bitf *)&TP4IOC0)->bit03

#define _TP5CKS0	((volatile struct bitf *)&TP5CTL0)->bit00
#define _TP5CKS1	((volatile struct bitf *)&TP5CTL0)->bit01
#define _TP5CKS2	((volatile struct bitf *)&TP5CTL0)->bit02

#define	_TP5MD0 	((volatile struct bitf *)&TP5CTL1)->bit00
#define	_TP5MD1   	((volatile struct bitf *)&TP5CTL1)->bit01
#define	_TP5MD2  	((volatile struct bitf *)&TP5CTL1)->bit02

#define	_TP5OL0  	((volatile struct bitf *)&TP5IOC0)->bit01
#define	_TP5OL1  	((volatile struct bitf *)&TP5IOC0)->bit03


#define	_TP0OL0  	((volatile struct bitf *)&TP0IOC0)->bit01
#define	_TP0OL1  	((volatile struct bitf *)&TP0IOC0)->bit03


#define	_TP0OE0  	((volatile struct bitf *)0xfffff592)->bit00
#define	_TP0OE1  	((volatile struct bitf *)0xfffff592)->bit02

#define	_TP0OVF  	((volatile struct bitf *)0xfffff595)->bit00

#define	_TP1CE   	((volatile struct bitf *)0xfffff5a0)->bit07

//#define	_TP1EEE  	((volatile struct bitf *)0xfffff5a1)->bit05
//#define	_TP1EST  	((volatile struct bitf *)0xfffff5a1)->bit06

#define	_TP1OE0  	((volatile struct bitf *)0xfffff5a2)->bit00
#define	_TP1OE1  	((volatile struct bitf *)0xfffff5a2)->bit02

#define	_TP1OVF  	((volatile struct bitf *)0xfffff5a5)->bit00

#define	_TP2CE   	((volatile struct bitf *)0xfffff5b0)->bit07

#define	_TP2EEE  	((volatile struct bitf *)0xfffff5b1)->bit05
#define	_TP2EST  	((volatile struct bitf *)0xfffff5b1)->bit06

#define	_TP2OE0  	((volatile struct bitf *)0xfffff5b2)->bit00
#define	_TP2OE1  	((volatile struct bitf *)0xfffff5b2)->bit02

#define	_TP2OVF  	((volatile struct bitf *)0xfffff5b5)->bit00

#define	_TP3CE   	((volatile struct bitf *)0xfffff5c0)->bit07

//#define	_TP3EEE  	((volatile struct bitf *)0xfffff5c1)->bit05
//#define	_TP3EST  	((volatile struct bitf *)0xfffff5c1)->bit06

#define	_TP3OE0  	((volatile struct bitf *)0xfffff5c2)->bit00
#define	_TP3OE1  	((volatile struct bitf *)0xfffff5c2)->bit02

#define	_TP3OVF  	((volatile struct bitf *)0xfffff5c5)->bit00

#define	_TP4CE   	((volatile struct bitf *)0xfffff5d0)->bit07

#define	_TP4EEE  	((volatile struct bitf *)0xfffff5d1)->bit05
#define	_TP4EST  	((volatile struct bitf *)0xfffff5d1)->bit06

#define	_TP4OE0  	((volatile struct bitf *)0xfffff5d2)->bit00
#define	_TP4OE1  	((volatile struct bitf *)0xfffff5d2)->bit02

#define	_TP4OVF  	((volatile struct bitf *)0xfffff5d5)->bit00

#define	_TP5CE   	((volatile struct bitf *)0xfffff5e0)->bit07

#define	_TP5EEE  	((volatile struct bitf *)0xfffff5e1)->bit05
#define	_TP5EST  	((volatile struct bitf *)0xfffff5e1)->bit06

#define	_TP5OE0  	((volatile struct bitf *)0xfffff5e2)->bit00
#define	_TP5OE1  	((volatile struct bitf *)0xfffff5e2)->bit02

#define	_TP5OVF  	((volatile struct bitf *)0xfffff5e5)->bit00

#define	_WTM0    	((volatile struct bitf *)0xfffff680)->bit00
#define	_WTM1    	((volatile struct bitf *)0xfffff680)->bit01

#define	_TM0CE   	((volatile struct bitf *)0xfffff690)->bit07

#define	_RTPOE0  	((volatile struct bitf *)0xfffff6e5)->bit07

#define	_PRERR   	((volatile struct bitf *)0xfffff802)->bit00

#define	_RSTOP   	((volatile struct bitf *)0xfffff80c)->bit00

#define	_DF0     	((volatile struct bitf *)0xfffff810)->bit07

#define	_DF1     	((volatile struct bitf *)0xfffff812)->bit07

#define	_DF2     	((volatile struct bitf *)0xfffff814)->bit07

#define	_DF3     	((volatile struct bitf *)0xfffff816)->bit07

#define	_PSM0    	((volatile struct bitf *)0xfffff820)->bit00
#define	_PSM1    	((volatile struct bitf *)0xfffff820)->bit01

#define	_LOCK    	((volatile struct bitf *)0xfffff824)->bit00

#define	_CK3     	((volatile struct bitf *)0xfffff828)->bit03
#define	_CLS     	((volatile struct bitf *)0xfffff828)->bit04
#define	_MCK     	((volatile struct bitf *)0xfffff828)->bit06

#define	_PLLON   	((volatile struct bitf *)0xfffff82c)->bit00
#define	_SELPLL  	((volatile struct bitf *)0xfffff82c)->bit01

#define	_CLME    	((volatile struct bitf *)0xfffff870)->bit00

#define	_LVIF    	((volatile struct bitf *)0xfffff890)->bit00
#define	_LVIMD   	((volatile struct bitf *)0xfffff890)->bit01
#define	_LVION   	((volatile struct bitf *)0xfffff890)->bit07

#define	_RAMF    	((volatile struct bitf *)0xfffff892)->bit00

#define	_BGCE0   	((volatile struct bitf *)0xfffff8b0)->bit04

#define	_OCDM0   	((volatile struct bitf *)0xfffff9fc)->bit00

#define	_UA0DIR  	((volatile struct bitf *)0xfffffa00)->bit04
#define	_UA0RXE  	((volatile struct bitf *)0xfffffa00)->bit05
#define	_UA0TXE  	((volatile struct bitf *)0xfffffa00)->bit06
#define	_UA0PWR  	((volatile struct bitf *)0xfffffa00)->bit07

#define	_UA0SRF  	((volatile struct bitf *)0xfffffa03)->bit07

#define	_UA0OVE  	((volatile struct bitf *)0xfffffa04)->bit00
#define	_UA0FE   	((volatile struct bitf *)0xfffffa04)->bit01
#define	_UA0PE   	((volatile struct bitf *)0xfffffa04)->bit02
#define	_UA0TSF  	((volatile struct bitf *)0xfffffa04)->bit07

#define	_UA1DIR  	((volatile struct bitf *)0xfffffa10)->bit04
#define	_UA1RXE  	((volatile struct bitf *)0xfffffa10)->bit05
#define	_UA1TXE  	((volatile struct bitf *)0xfffffa10)->bit06
#define	_UA1PWR  	((volatile struct bitf *)0xfffffa10)->bit07

#define	_UA1SRF  	((volatile struct bitf *)0xfffffa13)->bit07

#define	_UA1OVE  	((volatile struct bitf *)0xfffffa14)->bit00
#define	_UA1FE   	((volatile struct bitf *)0xfffffa14)->bit01
#define	_UA1PE   	((volatile struct bitf *)0xfffffa14)->bit02
#define	_UA1TSF  	((volatile struct bitf *)0xfffffa14)->bit07

#define	_UA2DIR  	((volatile struct bitf *)0xfffffa20)->bit04
#define	_UA2RXE  	((volatile struct bitf *)0xfffffa20)->bit05
#define	_UA2TXE  	((volatile struct bitf *)0xfffffa20)->bit06
#define	_UA2PWR  	((volatile struct bitf *)0xfffffa20)->bit07

#define	_UA2SRF  	((volatile struct bitf *)0xfffffa23)->bit07

#define	_UA2OVE  	((volatile struct bitf *)0xfffffa24)->bit00
#define	_UA2FE   	((volatile struct bitf *)0xfffffa24)->bit01
#define	_UA2PE   	((volatile struct bitf *)0xfffffa24)->bit02
#define	_UA2TSF  	((volatile struct bitf *)0xfffffa24)->bit07

#define	_CB0SCE  	((volatile struct bitf *)0xfffffd00)->bit00
#define	_CB0DIR  	((volatile struct bitf *)0xfffffd00)->bit04
#define	_CB0RXE  	((volatile struct bitf *)0xfffffd00)->bit05
#define	_CB0TXE  	((volatile struct bitf *)0xfffffd00)->bit06
#define	_CB0PWR  	((volatile struct bitf *)0xfffffd00)->bit07

#define	_CB0OVE  	((volatile struct bitf *)0xfffffd03)->bit00
#define	_CB0TSF  	((volatile struct bitf *)0xfffffd03)->bit07

#define	_CB1SCE  	((volatile struct bitf *)0xfffffd10)->bit00
#define	_CB1DIR  	((volatile struct bitf *)0xfffffd10)->bit04
#define	_CB1RXE  	((volatile struct bitf *)0xfffffd10)->bit05
#define	_CB1TXE  	((volatile struct bitf *)0xfffffd10)->bit06
#define	_CB1PWR  	((volatile struct bitf *)0xfffffd10)->bit07

#define	_CB1OVE  	((volatile struct bitf *)0xfffffd13)->bit00
#define	_CB1TSF  	((volatile struct bitf *)0xfffffd13)->bit07

#define	_CB2SCE  	((volatile struct bitf *)0xfffffd20)->bit00
#define	_CB2DIR  	((volatile struct bitf *)0xfffffd20)->bit04
#define	_CB2RXE  	((volatile struct bitf *)0xfffffd20)->bit05
#define	_CB2TXE  	((volatile struct bitf *)0xfffffd20)->bit06
#define	_CB2PWR  	((volatile struct bitf *)0xfffffd20)->bit07

#define	_CB2OVE  	((volatile struct bitf *)0xfffffd23)->bit00
#define	_CB2TSF  	((volatile struct bitf *)0xfffffd23)->bit07

#define	_CB3SCE  	((volatile struct bitf *)0xfffffd30)->bit00
#define	_CB3DIR  	((volatile struct bitf *)0xfffffd30)->bit04
#define	_CB3RXE  	((volatile struct bitf *)0xfffffd30)->bit05
#define	_CB3TXE  	((volatile struct bitf *)0xfffffd30)->bit06
#define	_CB3PWR  	((volatile struct bitf *)0xfffffd30)->bit07

#define	_CB3OVE  	((volatile struct bitf *)0xfffffd33)->bit00
#define	_CB3TSF  	((volatile struct bitf *)0xfffffd33)->bit07

#define	_CB4SCE  	((volatile struct bitf *)0xfffffd40)->bit00
#define	_CB4DIR  	((volatile struct bitf *)0xfffffd40)->bit04
#define	_CB4RXE  	((volatile struct bitf *)0xfffffd40)->bit05
#define	_CB4TXE  	((volatile struct bitf *)0xfffffd40)->bit06
#define	_CB4PWR  	((volatile struct bitf *)0xfffffd40)->bit07

#define	_CB4OVE  	((volatile struct bitf *)0xfffffd43)->bit00
#define	_CB4TSF  	((volatile struct bitf *)0xfffffd43)->bit07

#define	_SPT0    	((volatile struct bitf *)0xfffffd82)->bit00
#define	_STT0    	((volatile struct bitf *)0xfffffd82)->bit01
#define	_ACKE0   	((volatile struct bitf *)0xfffffd82)->bit02
#define	_WTIM0   	((volatile struct bitf *)0xfffffd82)->bit03
#define	_SPIE0   	((volatile struct bitf *)0xfffffd82)->bit04
#define	_WREL0   	((volatile struct bitf *)0xfffffd82)->bit05
#define	_LREL0   	((volatile struct bitf *)0xfffffd82)->bit06
#define	_IICE0   	((volatile struct bitf *)0xfffffd82)->bit07

#define	_DAD0    	((volatile struct bitf *)0xfffffd84)->bit04
#define	_CLD0    	((volatile struct bitf *)0xfffffd84)->bit05

#define	_CLX0    	((volatile struct bitf *)0xfffffd85)->bit00

#define	_SPD0    	((volatile struct bitf *)0xfffffd86)->bit00
#define	_STD0    	((volatile struct bitf *)0xfffffd86)->bit01
#define	_ACKD0   	((volatile struct bitf *)0xfffffd86)->bit02
#define	_TRC0    	((volatile struct bitf *)0xfffffd86)->bit03
#define	_COI0    	((volatile struct bitf *)0xfffffd86)->bit04
#define	_EXC0    	((volatile struct bitf *)0xfffffd86)->bit05
#define	_ALD0    	((volatile struct bitf *)0xfffffd86)->bit06
#define	_MSTS0   	((volatile struct bitf *)0xfffffd86)->bit07

#define	_IICRSV0 	((volatile struct bitf *)0xfffffd8a)->bit00
#define	_STCEN0  	((volatile struct bitf *)0xfffffd8a)->bit01
#define	_IICBSY0 	((volatile struct bitf *)0xfffffd8a)->bit06
#define	_STCF0   	((volatile struct bitf *)0xfffffd8a)->bit07

#define	_SPT1    	((volatile struct bitf *)0xfffffd92)->bit00
#define	_STT1    	((volatile struct bitf *)0xfffffd92)->bit01
#define	_ACKE1   	((volatile struct bitf *)0xfffffd92)->bit02
#define	_WTIM1   	((volatile struct bitf *)0xfffffd92)->bit03
#define	_SPIE1   	((volatile struct bitf *)0xfffffd92)->bit04
#define	_WREL1   	((volatile struct bitf *)0xfffffd92)->bit05
#define	_LREL1   	((volatile struct bitf *)0xfffffd92)->bit06
#define	_IICE1   	((volatile struct bitf *)0xfffffd92)->bit07

#define	_DAD1    	((volatile struct bitf *)0xfffffd94)->bit04
#define	_CLD1    	((volatile struct bitf *)0xfffffd94)->bit05

#define	_CLX1    	((volatile struct bitf *)0xfffffd95)->bit00

#define	_SPD1    	((volatile struct bitf *)0xfffffd96)->bit00
#define	_STD1    	((volatile struct bitf *)0xfffffd96)->bit01
#define	_ACKD1   	((volatile struct bitf *)0xfffffd96)->bit02
#define	_TRC1    	((volatile struct bitf *)0xfffffd96)->bit03
#define	_COI1    	((volatile struct bitf *)0xfffffd96)->bit04
#define	_EXC1    	((volatile struct bitf *)0xfffffd96)->bit05
#define	_ALD1    	((volatile struct bitf *)0xfffffd96)->bit06
#define	_MSTS1   	((volatile struct bitf *)0xfffffd96)->bit07

#define	_IICRSV1 	((volatile struct bitf *)0xfffffd9a)->bit00
#define	_STCEN1  	((volatile struct bitf *)0xfffffd9a)->bit01
#define	_IICBSY1 	((volatile struct bitf *)0xfffffd9a)->bit06
#define	_STCF1   	((volatile struct bitf *)0xfffffd9a)->bit07

#define	_SPT2    	((volatile struct bitf *)0xfffffda2)->bit00
#define	_STT2    	((volatile struct bitf *)0xfffffda2)->bit01
#define	_ACKE2   	((volatile struct bitf *)0xfffffda2)->bit02
#define	_WTIM2   	((volatile struct bitf *)0xfffffda2)->bit03
#define	_SPIE2   	((volatile struct bitf *)0xfffffda2)->bit04
#define	_WREL2   	((volatile struct bitf *)0xfffffda2)->bit05
#define	_LREL2   	((volatile struct bitf *)0xfffffda2)->bit06
#define	_IICE2   	((volatile struct bitf *)0xfffffda2)->bit07

#define	_DAD2    	((volatile struct bitf *)0xfffffda4)->bit04
#define	_CLD2    	((volatile struct bitf *)0xfffffda4)->bit05

#define	_CLX2    	((volatile struct bitf *)0xfffffda5)->bit00

#define	_SPD2    	((volatile struct bitf *)0xfffffda6)->bit00
#define	_STD2    	((volatile struct bitf *)0xfffffda6)->bit01
#define	_ACKD2   	((volatile struct bitf *)0xfffffda6)->bit02
#define	_TRC2    	((volatile struct bitf *)0xfffffda6)->bit03
#define	_COI2    	((volatile struct bitf *)0xfffffda6)->bit04
#define	_EXC2    	((volatile struct bitf *)0xfffffda6)->bit05
#define	_ALD2    	((volatile struct bitf *)0xfffffda6)->bit06
#define	_MSTS2   	((volatile struct bitf *)0xfffffda6)->bit07

#define	_IICRSV2 	((volatile struct bitf *)0xfffffdaa)->bit00
#define	_STCEN2  	((volatile struct bitf *)0xfffffdaa)->bit01
#define	_IICBSY2 	((volatile struct bitf *)0xfffffdaa)->bit06
#define	_STCF2   	((volatile struct bitf *)0xfffffdaa)->bit07

#define	_INTF02    	((volatile struct bitf *)0xfffffc00)->bit02
#define	_INTF03    	((volatile struct bitf *)0xfffffc00)->bit03
#define	_INTF04    	((volatile struct bitf *)0xfffffc00)->bit04
#define	_INTF05    	((volatile struct bitf *)0xfffffc00)->bit05
#define	_INTF06    	((volatile struct bitf *)0xfffffc00)->bit06

#define	_INTR02    	((volatile struct bitf *)0xfffffc20)->bit02
#define	_INTR03    	((volatile struct bitf *)0xfffffc20)->bit03
#define	_INTR04    	((volatile struct bitf *)0xfffffc20)->bit04
#define	_INTR05    	((volatile struct bitf *)0xfffffc20)->bit05
#define	_INTR06    	((volatile struct bitf *)0xfffffc20)->bit06

#define	_INTF915    ((volatile struct bitf *)INTF9H)->bit07
#define	_INTR915    ((volatile struct bitf *)INTR9H)->bit07


//-------------------- PORT ------------------------------------
//-- PORT 0 --
#define	_P02   		((volatile struct bitf *)&P0)->bit02
#define	_P03   		((volatile struct bitf *)&P0)->bit03
#define	_P04   		((volatile struct bitf *)&P0)->bit04
#define	_P05   		((volatile struct bitf *)&P0)->bit05
#define	_P06   		((volatile struct bitf *)&P0)->bit06

#define	_PM02   	((volatile struct bitf *)&PM0)->bit02
#define	_PM03   	((volatile struct bitf *)&PM0)->bit03
#define	_PM04   	((volatile struct bitf *)&PM0)->bit04
#define	_PM05   	((volatile struct bitf *)&PM0)->bit05
#define	_PM06   	((volatile struct bitf *)&PM0)->bit06

#define	_PF02   	((volatile struct bitf *)&PF0)->bit02
#define	_PF03   	((volatile struct bitf *)&PF0)->bit03
#define	_PF04   	((volatile struct bitf *)&PF0)->bit04
#define	_PF05   	((volatile struct bitf *)&PF0)->bit05
#define	_PF06   	((volatile struct bitf *)&PF0)->bit06

#define	_PMC02   	((volatile struct bitf *)&PMC0)->bit02
#define	_PMC03   	((volatile struct bitf *)&PMC0)->bit03
#define	_PMC04   	((volatile struct bitf *)&PMC0)->bit04
#define	_PMC05   	((volatile struct bitf *)&PMC0)->bit05
#define	_PMC06   	((volatile struct bitf *)&PMC0)->bit06


//-- PORT 1 --
#define	_P10   		((volatile struct bitf *)&P1)->bit00
#define	_P11   		((volatile struct bitf *)&P1)->bit01

#define	_PM10   	((volatile struct bitf *)&PM1)->bit00
#define	_PM11   	((volatile struct bitf *)&PM1)->bit01

//-- PORT 3 --
#define	_P30   		((volatile struct bitf *)&P3L)->bit00
#define	_P31   		((volatile struct bitf *)&P3L)->bit01
#define	_P32   		((volatile struct bitf *)&P3L)->bit02
#define	_P33   		((volatile struct bitf *)&P3L)->bit03
#define	_P34   		((volatile struct bitf *)&P3L)->bit04
#define	_P35   		((volatile struct bitf *)&P3L)->bit05
#define	_P36   		((volatile struct bitf *)&P3L)->bit06
#define	_P37   		((volatile struct bitf *)&P3L)->bit07
#define	_P38   		((volatile struct bitf *)&P3H)->bit00
#define	_P39   		((volatile struct bitf *)&P3H)->bit01

#define	_PM30   	((volatile struct bitf *)&PM3L)->bit00
#define	_PM31   	((volatile struct bitf *)&PM3L)->bit01
#define	_PM32   	((volatile struct bitf *)&PM3L)->bit02
#define	_PM33   	((volatile struct bitf *)&PM3L)->bit03
#define	_PM34   	((volatile struct bitf *)&PM3L)->bit04
#define	_PM35   	((volatile struct bitf *)&PM3L)->bit05
#define	_PM36   	((volatile struct bitf *)&PM3L)->bit06
#define	_PM37   	((volatile struct bitf *)&PM3L)->bit07
#define	_PM38   	((volatile struct bitf *)&PM3H)->bit00
#define	_PM39   	((volatile struct bitf *)&PM3H)->bit01

#define	_PMC30   	((volatile struct bitf *)&PMC3L)->bit00
#define	_PMC31   	((volatile struct bitf *)&PMC3L)->bit01
#define	_PMC32   	((volatile struct bitf *)&PMC3L)->bit02
#define	_PMC33   	((volatile struct bitf *)&PMC3L)->bit03
#define	_PMC34   	((volatile struct bitf *)&PMC3L)->bit04
#define	_PMC35   	((volatile struct bitf *)&PMC3L)->bit05
#define	_PMC36   	((volatile struct bitf *)&PMC3L)->bit06
#define	_PMC37   	((volatile struct bitf *)&PMC3L)->bit07
#define	_PMC38   	((volatile struct bitf *)&PMC3H)->bit00
#define	_PMC39   	((volatile struct bitf *)&PMC3H)->bit01

#define	_PFC30   	((volatile struct bitf *)&PFC3L)->bit00
#define	_PFC31   	((volatile struct bitf *)&PFC3L)->bit01
#define	_PFC32   	((volatile struct bitf *)&PFC3L)->bit02
#define	_PFC33   	((volatile struct bitf *)&PFC3L)->bit03
#define	_PFC34   	((volatile struct bitf *)&PFC3L)->bit04
#define	_PFC35   	((volatile struct bitf *)&PFC3L)->bit05
#define	_PFC36   	((volatile struct bitf *)&PFC3L)->bit06
#define	_PFC37   	((volatile struct bitf *)&PFC3L)->bit07
#define	_PFC38   	((volatile struct bitf *)&PFC3H)->bit00
#define	_PFC39   	((volatile struct bitf *)&PFC3H)->bit01

#define	_PFCE32   	((volatile struct bitf *)&PFCE3L)->bit02

#define	_PF30   	((volatile struct bitf *)&PF3L)->bit00
#define	_PF31   	((volatile struct bitf *)&PF3L)->bit01
#define	_PF32   	((volatile struct bitf *)&PF3L)->bit02
#define	_PF33   	((volatile struct bitf *)&PF3L)->bit03
#define	_PF34   	((volatile struct bitf *)&PF3L)->bit04
#define	_PF35   	((volatile struct bitf *)&PF3L)->bit05
#define	_PF36   	((volatile struct bitf *)&PF3L)->bit06
#define	_PF37   	((volatile struct bitf *)&PF3L)->bit07
#define	_PF38   	((volatile struct bitf *)&PF3H)->bit00
#define	_PF39   	((volatile struct bitf *)&PF3H)->bit01

//-- PORT 4 --
#define	_P40   		((volatile struct bitf *)&P4)->bit00
#define	_P41   		((volatile struct bitf *)&P4)->bit01
#define	_P42   		((volatile struct bitf *)&P4)->bit02

#define	_PM40   	((volatile struct bitf *)&PM4)->bit00
#define	_PM41   	((volatile struct bitf *)&PM4)->bit01
#define	_PM42   	((volatile struct bitf *)&PM4)->bit02

#define	_PMC40   	((volatile struct bitf *)&PMC4)->bit00
#define	_PMC41   	((volatile struct bitf *)&PMC4)->bit01
#define	_PMC42   	((volatile struct bitf *)&PMC4)->bit02

#define	_PFC40   	((volatile struct bitf *)&PFC4)->bit00
#define	_PFC41   	((volatile struct bitf *)&PFC4)->bit01
#define	_PFC42   	((volatile struct bitf *)&PFC4)->bit02

#define	_PF40   	((volatile struct bitf *)&PF4)->bit00
#define	_PF41   	((volatile struct bitf *)&PF4)->bit01
#define	_PF42   	((volatile struct bitf *)&PF4)->bit02

//-- PORT 5 --
#define	_P50   		((volatile struct bitf *)&P5)->bit00
#define	_P51   		((volatile struct bitf *)&P5)->bit01
#define	_P52   		((volatile struct bitf *)&P5)->bit02
#define	_P53   		((volatile struct bitf *)&P5)->bit03
#define	_P54   		((volatile struct bitf *)&P5)->bit04
#define	_P55   		((volatile struct bitf *)&P5)->bit05

#define	_PM50   	((volatile struct bitf *)&PM5)->bit00
#define	_PM51   	((volatile struct bitf *)&PM5)->bit01
#define	_PM52   	((volatile struct bitf *)&PM5)->bit02
#define	_PM53   	((volatile struct bitf *)&PM5)->bit03
#define	_PM54   	((volatile struct bitf *)&PM5)->bit04
#define	_PM55   	((volatile struct bitf *)&PM5)->bit05

#define	_PMC50   	((volatile struct bitf *)&PMC5)->bit00
#define	_PMC51   	((volatile struct bitf *)&PMC5)->bit01
#define	_PMC52   	((volatile struct bitf *)&PMC5)->bit02
#define	_PMC53   	((volatile struct bitf *)&PMC5)->bit03
#define	_PMC54   	((volatile struct bitf *)&PMC5)->bit04
#define	_PMC55   	((volatile struct bitf *)&PMC5)->bit05

#define	_PFC50   	((volatile struct bitf *)&PFC5)->bit00
#define	_PFC51   	((volatile struct bitf *)&PFC5)->bit01
#define	_PFC52   	((volatile struct bitf *)&PFC5)->bit02
#define	_PFC53   	((volatile struct bitf *)&PFC5)->bit03
#define	_PFC54   	((volatile struct bitf *)&PFC5)->bit04
#define	_PFC55   	((volatile struct bitf *)&PFC5)->bit05

#define	_PFCE50   	((volatile struct bitf *)&PFCE5)->bit00
#define	_PFCE51   	((volatile struct bitf *)&PFCE5)->bit01
#define	_PFCE52   	((volatile struct bitf *)&PFCE5)->bit02
#define	_PFCE53   	((volatile struct bitf *)&PFCE5)->bit03
#define	_PFCE54   	((volatile struct bitf *)&PFCE5)->bit04
#define	_PFCE55   	((volatile struct bitf *)&PFCE5)->bit05

#define	_PF50   	((volatile struct bitf *)&PF5)->bit00
#define	_PF51   	((volatile struct bitf *)&PF5)->bit01
#define	_PF52   	((volatile struct bitf *)&PF5)->bit02
#define	_PF53   	((volatile struct bitf *)&PF5)->bit03
#define	_PF54   	((volatile struct bitf *)&PF5)->bit04
#define	_PF55   	((volatile struct bitf *)&PF5)->bit05

//-- PORT 7 --
#define	_P70   		((volatile struct bitf *)&P7L)->bit00
#define	_P71   		((volatile struct bitf *)&P7L)->bit01
#define	_P72   		((volatile struct bitf *)&P7L)->bit02
#define	_P73   		((volatile struct bitf *)&P7L)->bit03
#define	_P74   		((volatile struct bitf *)&P7L)->bit04
#define	_P75   		((volatile struct bitf *)&P7L)->bit05
#define	_P76   		((volatile struct bitf *)&P7L)->bit06
#define	_P77   		((volatile struct bitf *)&P7L)->bit07

#define	_PM70   	((volatile struct bitf *)&PM7L)->bit00
#define	_PM71   	((volatile struct bitf *)&PM7L)->bit01
#define	_PM72   	((volatile struct bitf *)&PM7L)->bit02
#define	_PM73   	((volatile struct bitf *)&PM7L)->bit03
#define	_PM74   	((volatile struct bitf *)&PM7L)->bit04
#define	_PM75   	((volatile struct bitf *)&PM7L)->bit05
#define	_PM76   	((volatile struct bitf *)&PM7L)->bit06
#define	_PM77   	((volatile struct bitf *)&PM7L)->bit07

#define	_P78   		((volatile struct bitf *)&P7H)->bit00
#define	_P79   		((volatile struct bitf *)&P7H)->bit01
#define	_P710   	((volatile struct bitf *)&P7H)->bit02
#define	_P711   	((volatile struct bitf *)&P7H)->bit03

#define	_PM78   	((volatile struct bitf *)&PM7H)->bit00
#define	_PM79   	((volatile struct bitf *)&PM7H)->bit01
#define	_PM710   	((volatile struct bitf *)&PM7H)->bit02
#define	_PM711   	((volatile struct bitf *)&PM7H)->bit03


//-- PORT 9 --
#define	_P90   		((volatile struct bitf *)&P9)->bit00
#define	_P91   		((volatile struct bitf *)&P9)->bit01
#define	_P92   		((volatile struct bitf *)&P9)->bit02
#define	_P93   		((volatile struct bitf *)&P9)->bit03
#define	_P94   		((volatile struct bitf *)&P9)->bit04
#define	_P95   		((volatile struct bitf *)&P9)->bit05
#define	_P96   		((volatile struct bitf *)&P9)->bit06
#define	_P97   		((volatile struct bitf *)&P9)->bit07
#define	_P98   		((volatile struct bitf *)&P9)->bit08
#define	_P99   		((volatile struct bitf *)&P9)->bit09
#define	_P910   	((volatile struct bitf *)&P9)->bit10
#define	_P911   	((volatile struct bitf *)&P9)->bit11
#define	_P912   	((volatile struct bitf *)&P9)->bit12
#define	_P913  		((volatile struct bitf *)&P9)->bit13
#define	_P914   	((volatile struct bitf *)&P9)->bit14
#define	_P915   	((volatile struct bitf *)&P9)->bit15

#define	_PM90   		((volatile struct bitf *)&PM9)->bit00
#define	_PM91   		((volatile struct bitf *)&PM9)->bit01
#define	_PM92   		((volatile struct bitf *)&PM9)->bit02
#define	_PM93   		((volatile struct bitf *)&PM9)->bit03
#define	_PM94   		((volatile struct bitf *)&PM9)->bit04
#define	_PM95   		((volatile struct bitf *)&PM9)->bit05
#define	_PM96   		((volatile struct bitf *)&PM9)->bit06
#define	_PM97   		((volatile struct bitf *)&PM9)->bit07
#define	_PM98   		((volatile struct bitf *)&PM9)->bit08
#define	_PM99   		((volatile struct bitf *)&PM9)->bit09
#define	_PM910   		((volatile struct bitf *)&PM9)->bit10
#define	_PM911   		((volatile struct bitf *)&PM9)->bit11
#define	_PM912   		((volatile struct bitf *)&PM9)->bit12
#define	_PM913  		((volatile struct bitf *)&PM9)->bit13
#define	_PM914   		((volatile struct bitf *)&PM9)->bit14
#define	_PM915   		((volatile struct bitf *)&PM9)->bit15

#define	_PMC90   		((volatile struct bitf *)&PMC9)->bit00
#define	_PMC91   		((volatile struct bitf *)&PMC9)->bit01
#define	_PMC92   		((volatile struct bitf *)&PMC9)->bit02
#define	_PMC93   		((volatile struct bitf *)&PMC9)->bit03
#define	_PMC94   		((volatile struct bitf *)&PMC9)->bit04
#define	_PMC95   		((volatile struct bitf *)&PMC9)->bit05
#define	_PMC96   		((volatile struct bitf *)&PMC9)->bit06
#define	_PMC97   		((volatile struct bitf *)&PMC9)->bit07
#define	_PMC98   		((volatile struct bitf *)&PMC9)->bit08
#define	_PMC99   		((volatile struct bitf *)&PMC9)->bit09
#define	_PMC910   		((volatile struct bitf *)&PMC9)->bit10
#define	_PMC911   		((volatile struct bitf *)&PMC9)->bit11
#define	_PMC912   		((volatile struct bitf *)&PMC9)->bit12
#define	_PMC913  		((volatile struct bitf *)&PMC9)->bit13
#define	_PMC914   		((volatile struct bitf *)&PMC9)->bit14
#define	_PMC915   		((volatile struct bitf *)&PMC9)->bit15

#define	_PFC90   		((volatile struct bitf *)&PFC9)->bit00
#define	_PFC91   		((volatile struct bitf *)&PFC9)->bit01
#define	_PFC92   		((volatile struct bitf *)&PFC9)->bit02
#define	_PFC93   		((volatile struct bitf *)&PFC9)->bit03
#define	_PFC94   		((volatile struct bitf *)&PFC9)->bit04
#define	_PFC95   		((volatile struct bitf *)&PFC9)->bit05
#define	_PFC96   		((volatile struct bitf *)&PFC9)->bit06
#define	_PFC97   		((volatile struct bitf *)&PFC9)->bit07
#define	_PFC98   		((volatile struct bitf *)&PFC9)->bit08
#define	_PFC99   		((volatile struct bitf *)&PFC9)->bit09
#define	_PFC910   		((volatile struct bitf *)&PFC9)->bit10
#define	_PFC911   		((volatile struct bitf *)&PFC9)->bit11
#define	_PFC912   		((volatile struct bitf *)&PFC9)->bit12
#define	_PFC913  		((volatile struct bitf *)&PFC9)->bit13
#define	_PFC914   		((volatile struct bitf *)&PFC9)->bit14
#define	_PFC915   		((volatile struct bitf *)&PFC9)->bit15

#define	_PFCE90   		((volatile struct bitf *)&PFCE9)->bit00
#define	_PFCE91   		((volatile struct bitf *)&PFCE9)->bit01
#define	_PFCE92   		((volatile struct bitf *)&PFCE9)->bit02
#define	_PFCE93   		((volatile struct bitf *)&PFCE9)->bit03
#define	_PFCE94   		((volatile struct bitf *)&PFCE9)->bit04
#define	_PFCE95   		((volatile struct bitf *)&PFCE9)->bit05
#define	_PFCE96   		((volatile struct bitf *)&PFCE9)->bit06
#define	_PFCE97   		((volatile struct bitf *)&PFCE9)->bit07
#define	_PFCE98   		((volatile struct bitf *)&PFCE9)->bit08
#define	_PFCE99   		((volatile struct bitf *)&PFCE9)->bit09
#define	_PFCE910   		((volatile struct bitf *)&PFCE9)->bit10
#define	_PFCE911   		((volatile struct bitf *)&PFCE9)->bit11
#define	_PFCE912   		((volatile struct bitf *)&PFCE9)->bit12
#define	_PFCE913  		((volatile struct bitf *)&PFCE9)->bit13
#define	_PFCE914   		((volatile struct bitf *)&PFCE9)->bit14
#define	_PFCE915   		((volatile struct bitf *)&PFCE9)->bit15

#define	_PF90   		((volatile struct bitf *)&PF9)->bit00
#define	_PF91   		((volatile struct bitf *)&PF9)->bit01
#define	_PF92   		((volatile struct bitf *)&PF9)->bit02
#define	_PF93   		((volatile struct bitf *)&PF9)->bit03
#define	_PF94   		((volatile struct bitf *)&PF9)->bit04
#define	_PF95   		((volatile struct bitf *)&PF9)->bit05
#define	_PF96   		((volatile struct bitf *)&PF9)->bit06
#define	_PF97   		((volatile struct bitf *)&PF9)->bit07
#define	_PF98   		((volatile struct bitf *)&PF9)->bit08
#define	_PF99   		((volatile struct bitf *)&PF9)->bit09
#define	_PF910   		((volatile struct bitf *)&PF9)->bit10
#define	_PF911   		((volatile struct bitf *)&PF9)->bit11
#define	_PF912   		((volatile struct bitf *)&PF9)->bit12
#define	_PF913  		((volatile struct bitf *)&PF9)->bit13
#define	_PF914   		((volatile struct bitf *)&PF9)->bit14
#define	_PF915   		((volatile struct bitf *)&PF9)->bit15

//-- PORT CM --
#define	_PCM0   		((volatile struct bitf *)&PCM)->bit00
#define	_PCM1   		((volatile struct bitf *)&PCM)->bit01
#define	_PCM2   		((volatile struct bitf *)&PCM)->bit02
#define	_PCM3   		((volatile struct bitf *)&PCM)->bit03

#define	_PMCM0   		((volatile struct bitf *)&PMCM)->bit00
#define	_PMCM1   		((volatile struct bitf *)&PMCM)->bit01
#define	_PMCM2   		((volatile struct bitf *)&PMCM)->bit02
#define	_PMCM3   		((volatile struct bitf *)&PMCM)->bit03

#define	_PMCCM0   		((volatile struct bitf *)&PMCCM)->bit00
#define	_PMCCM1   		((volatile struct bitf *)&PMCCM)->bit01
#define	_PMCCM2   		((volatile struct bitf *)&PMCCM)->bit02
#define	_PMCCM3   		((volatile struct bitf *)&PMCCM)->bit03

//-- PORT CT --
#define	_PCT0   		((volatile struct bitf *)&PCT)->bit00
#define	_PCT1   		((volatile struct bitf *)&PCT)->bit01
#define	_PCT4   		((volatile struct bitf *)&PCM)->bit04
#define	_PCT6   		((volatile struct bitf *)&PCT)->bit06

#define	_PMCT0   		((volatile struct bitf *)&PMCT)->bit00
#define	_PMCT1   		((volatile struct bitf *)&PMCT)->bit01
#define	_PMCT4   		((volatile struct bitf *)&PMCT)->bit04
#define	_PMCT6   		((volatile struct bitf *)&PMCT)->bit06

#define	_PMCCT0   		((volatile struct bitf *)&PMCCT)->bit00
#define	_PMCCT1   		((volatile struct bitf *)&PMCCT)->bit01
#define	_PMCCT4   		((volatile struct bitf *)&PMCCT)->bit04
#define	_PMCCT6   		((volatile struct bitf *)&PMCCT)->bit06

//-- PORT DH --
#define	_PDH0   		((volatile struct bitf *)&PDH)->bit00
#define	_PDH1   		((volatile struct bitf *)&PDH)->bit01
#define	_PDH2   		((volatile struct bitf *)&PDH)->bit02
#define	_PDH3   		((volatile struct bitf *)&PDH)->bit03
#define	_PDH4   		((volatile struct bitf *)&PDH)->bit04
#define	_PDH5   		((volatile struct bitf *)&PDH)->bit05

#define	_PMDH0   		((volatile struct bitf *)&PMDH)->bit00
#define	_PMDH1   		((volatile struct bitf *)&PMDH)->bit01
#define	_PMDH2   		((volatile struct bitf *)&PMDH)->bit02
#define	_PMDH3   		((volatile struct bitf *)&PMDH)->bit03
#define	_PMDH4   		((volatile struct bitf *)&PMDH)->bit04
#define	_PMDH5   		((volatile struct bitf *)&PMDH)->bit05

#define	_PMCDH0   		((volatile struct bitf *)&PMCDH)->bit00
#define	_PMCDH1   		((volatile struct bitf *)&PMCDH)->bit01
#define	_PMCDH2   		((volatile struct bitf *)&PMCDH)->bit02
#define	_PMCDH3   		((volatile struct bitf *)&PMCDH)->bit03
#define	_PMCDH4   		((volatile struct bitf *)&PMCDH)->bit04
#define	_PMCDH5   		((volatile struct bitf *)&PMCDH)->bit05

//-- PORT DL --
#define	_PDL0   		((volatile struct bitf *)&PDL)->bit00
#define	_PDL1   		((volatile struct bitf *)&PDL)->bit01
#define	_PDL2   		((volatile struct bitf *)&PDL)->bit02
#define	_PDL3   		((volatile struct bitf *)&PDL)->bit03
#define	_PDL4   		((volatile struct bitf *)&PDL)->bit04
#define	_PDL5   		((volatile struct bitf *)&PDL)->bit05
#define	_PDL6   		((volatile struct bitf *)&PDL)->bit06
#define	_PDL7   		((volatile struct bitf *)&PDL)->bit07
#define	_PDL8   		((volatile struct bitf *)&PDL)->bit08
#define	_PDL9   		((volatile struct bitf *)&PDL)->bit09
#define	_PDL10   		((volatile struct bitf *)&PDL)->bit10
#define	_PDL11   		((volatile struct bitf *)&PDL)->bit11
#define	_PDL12   		((volatile struct bitf *)&PDL)->bit12
#define	_PDL13   		((volatile struct bitf *)&PDL)->bit13
#define	_PDL14   		((volatile struct bitf *)&PDL)->bit14
#define	_PDL15   		((volatile struct bitf *)&PDL)->bit15

#define	_PMDL0   		((volatile struct bitf *)&PMDL)->bit00
#define	_PMDL1   		((volatile struct bitf *)&PMDL)->bit01
#define	_PMDL2   		((volatile struct bitf *)&PMDL)->bit02
#define	_PMDL3   		((volatile struct bitf *)&PMDL)->bit03
#define	_PMDL4   		((volatile struct bitf *)&PMDL)->bit04
#define	_PMDL5   		((volatile struct bitf *)&PMDL)->bit05
#define	_PMDL6   		((volatile struct bitf *)&PMDL)->bit06
#define	_PMDL7   		((volatile struct bitf *)&PMDL)->bit07
#define	_PMDL8   		((volatile struct bitf *)&PMDL)->bit08
#define	_PMDL9   		((volatile struct bitf *)&PMDL)->bit09
#define	_PMDL10   		((volatile struct bitf *)&PMDL)->bit10
#define	_PMDL11   		((volatile struct bitf *)&PMDL)->bit11
#define	_PMDL12   		((volatile struct bitf *)&PMDL)->bit12
#define	_PMDL13   		((volatile struct bitf *)&PMDL)->bit13
#define	_PMDL14   		((volatile struct bitf *)&PMDL)->bit14
#define	_PMDL15   		((volatile struct bitf *)&PMDL)->bit15

#define	_PMCDL0   		((volatile struct bitf *)&PMCDL)->bit00
#define	_PMCDL1   		((volatile struct bitf *)&PMCDL)->bit01
#define	_PMCDL2   		((volatile struct bitf *)&PMCDL)->bit02
#define	_PMCDL3   		((volatile struct bitf *)&PMCDL)->bit03
#define	_PMCDL4   		((volatile struct bitf *)&PMCDL)->bit04
#define	_PMCDL5   		((volatile struct bitf *)&PMCDL)->bit05
#define	_PMCDL6   		((volatile struct bitf *)&PMCDL)->bit06
#define	_PMCDL7   		((volatile struct bitf *)&PMCDL)->bit07
#define	_PMCDL8   		((volatile struct bitf *)&PMCDL)->bit08
#define	_PMCDL9   		((volatile struct bitf *)&PMCDL)->bit09
#define	_PMCDL10   		((volatile struct bitf *)&PMCDL)->bit10
#define	_PMCDL11   		((volatile struct bitf *)&PMCDL)->bit11
#define	_PMCDL12   		((volatile struct bitf *)&PMCDL)->bit12
#define	_PMCDL13   		((volatile struct bitf *)&PMCDL)->bit13
#define	_PMCDL14   		((volatile struct bitf *)&PMCDL)->bit14
#define	_PMCDL15   		((volatile struct bitf *)&PMCDL)->bit15

#endif /* __uPD70F3281Y_H */

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




//@@-----------------------------------------------------------------------@@//
//@@
//@@  $Source:  $
//@@  Note di revisione:
//@@
//@@  $Revision: 1.2 $
//@@  $Date: 2004/04/04 00:00:00 $
//@@  - Aggiunte alcune definizioni "volatile" per i pin di interfaccia
//@@  con l'eeprom e altri dispositivi
//@@
//@@  $Log:  $
//@@
//@@  $Author: junior $
//@@  $RCSfile: V850SG2.H,v $
//@@  $Revision: 1.4 $
//@@  $State: Exp $
//@@  $Date: 2002/05/13 00:00:00 $
//@@  $Name:  $
//@@-----------------------------------------------------------------------@@//
//@@-----------------------------------------------------------------------@@//
//@@                                                                       @@//
//@@                           C O M E T A                                 @@//
//@@                                                                       @@//
//@@-----------------------------------------------------------------------@@//



