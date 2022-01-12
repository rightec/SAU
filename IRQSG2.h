/**
@file		IrqSG2.h
@brief		Interrupts reallocation routines 
					
@author		
@date		18/01/2011
@version	01.00
*/


#ifndef _IRQSG2_H_
#define _IRQSG2_H_

#include "global.h"

/**
IRQ error star number
*/
#define IRQ_BASE_ERR		BASEMOD_IRQSG2			
/**
IRQ initialization already done. Used in DEBUG mode only.
*/
#define IRQ_INIT_DONE		0x55AA

/**
Interrupt Errors
*/
enum IRQErrors{ IRQ_ERR_NONE,
				IRQ_ERR_INV_VECT=IRQ_BASE_ERR,	/**< Invalide vector*/
				IRQ_ERR_INV_FUN,		/**< Invalide IRQ function*/
				IRQ_ERR_ILL_VECT,		/**< Vector already used*/
				IRQ_ERR_INTSPUR			/**< Spurius Interupt (not handled by user)*/
				};

typedef enum IRQErrors IRQErrors;

/**
Interrpt vector priority
*/
enum IRQPriority{					
		IRQ_PRIO0,						/**< Highest priority*/
		IRQ_PRIO1,
		IRQ_PRIO2,
		IRQ_PRIO3,
		IRQ_PRIO4,
		IRQ_PRIO5,
		IRQ_PRIO6,
		IRQ_PRIO7 						/**< Lowest priority*/
		};

typedef enum IRQPriority IRQPriority;

/**
Available interrupt vector
*/
enum IRQVectors{						
		IRQ_VEC_RESET,//0
		IRQ_VEC_NMI,
		
		IRQ_VEC_TRAP0,//2
		IRQ_VEC_TRAP1,
		IRQ_VEC_ILGOP_TRAPDB,
		
		IRQ_VEC_INTLVI,//5
		
		IRQ_VEC_INTP0,	//6
		IRQ_VEC_INTP1,
		IRQ_VEC_INTP2,
		IRQ_VEC_INTP3, 
		IRQ_VEC_INTP4, 
		IRQ_VEC_INTP5, 
		IRQ_VEC_INTP6, 
		IRQ_VEC_INTP7, 
		
		IRQ_VEC_INTTQ0OV, //14
		IRQ_VEC_INTTQ0CC0, 
		IRQ_VEC_INTTQ0CC1, 
		IRQ_VEC_INTTQ0CC2,			
		IRQ_VEC_INTTQ0CC3,  
		
		IRQ_VEC_INTTP0OV,  //19  		
		IRQ_VEC_INTTP0CC0,    		
		IRQ_VEC_INTTP0CC1, 
		
		IRQ_VEC_INTTP1OV,  //22  		
		IRQ_VEC_INTTP1CC0,    		
		IRQ_VEC_INTTP1CC1,  
		
		IRQ_VEC_INTTP2OV,  //25  		
		IRQ_VEC_INTTP2CC0,    		
		IRQ_VEC_INTTP2CC1,  
		
		IRQ_VEC_INTTP3OV,  //28  		
		IRQ_VEC_INTTP3CC0,    		
		IRQ_VEC_INTTP3CC1,  
		
		IRQ_VEC_INTTP4OV, //31			
		IRQ_VEC_INTTP4CC0,			
		IRQ_VEC_INTTP4CC1,		
		
		IRQ_VEC_INTTP5OV,//34			
		IRQ_VEC_INTTP5CC0,			
		IRQ_VEC_INTTP5CC1,
		
		IRQ_VEC_INTTM0EQ0,//37
		
		IRQ_VEC_INTCB0R_INTIIC1,  
		IRQ_VEC_INTCB0T,	
		
		IRQ_VEC_INTCB1R,  	
		IRQ_VEC_INTCB1T, 
		
		IRQ_VEC_INTCB2R,   
		IRQ_VEC_INTCB2T, 	
		
		IRQ_VEC_INTCB3R,   	
		IRQ_VEC_INTCB3T, 
		
		IRQ_VEC_INTUA0R_INTCB4R,
		IRQ_VEC_INTUA0T_INTCB4T,

		IRQ_VEC_INTUA1R_INTIIC2,
		IRQ_VEC_INTUA1T,

		IRQ_VEC_INTUA2R_INTIIC0,
		IRQ_VEC_INTUA2T,

		IRQ_VEC_INTAD,

		IRQ_VEC_INTDMA0,	
		IRQ_VEC_INTDMA1, 
		IRQ_VEC_INTDMA2,  	
		IRQ_VEC_INTDMA3, 

		IRQ_VEC_INTKR,

		IRQ_VEC_INTWTI,
		IRQ_VEC_INTWT,

		//IRQ_VEC_INTC0ERR_INTERR,

		//IRQ_VEC_INTC0WUP_INTSTA,

		//IRQ_VEC_INTC0REC_INTIE1,

		//IRQ_VEC_INTC0TRX_INTIE2,

		IRQ_VEC_MAX,				// numero max di vettori
		};

typedef enum IRQVectors IRQVectors;



#ifdef __cplusplus
extern "C" {
#endif


void irq_init(void);

bool irq_setVect(IRQVectors reqVect, void (*pfIrqHandle)(int vect) );

bool irq_resetVect(IRQVectors reqVect);

void irq_chkErrors(void);


										// Questa volta le funzioni vengono definite con 
										// attributo "interrupt" e quindi i registri non
										// vengono + salvati dalle routines di salto 
										// nel modulo "startup.s"
void irq_RESET(void)				__attribute__ ((interrupt));
void irq_NMI(void)                  __attribute__ ((interrupt));

void irq_TRAP0(void)                __attribute__ ((interrupt));
void irq_TRAP1(void)                __attribute__ ((interrupt));
void irq_ILGOP_TRAPDB(void)         __attribute__ ((interrupt));

void irq_INTLVI(void)               __attribute__ ((interrupt));

void irq_INTP0(void)	            __attribute__ ((interrupt));
void irq_INTP1(void)        	    __attribute__ ((interrupt));
void irq_INTP2(void)       	        __attribute__ ((interrupt));
void irq_INTP3(void)      			__attribute__ ((interrupt));
void irq_INTP4(void)   			    __attribute__ ((interrupt));
void irq_INTP5(void)   			    __attribute__ ((interrupt));
void irq_INTP6(void) 			    __attribute__ ((interrupt));
void irq_INTP7(void) 			    __attribute__ ((interrupt));

void irq_INTTQ0OV(void)      		__attribute__ ((interrupt));
void irq_INTTQ0CC0(void)     		__attribute__ ((interrupt));
void irq_INTTQ0CC1(void)      		__attribute__ ((interrupt));
void irq_INTTQ0CC2(void)			__attribute__ ((interrupt));
void irq_INTTQ0CC3(void)			__attribute__ ((interrupt));

void irq_INTTP0OV(void)    		    __attribute__ ((interrupt));
void irq_INTTP0CC0(void)    		__attribute__ ((interrupt));
void irq_INTTP0CC1(void)    		__attribute__ ((interrupt));

void irq_INTTP1OV(void)    		    __attribute__ ((interrupt));
void irq_INTTP1CC0(void)    		__attribute__ ((interrupt));
void irq_INTTP1CC1(void)    		__attribute__ ((interrupt));

void irq_INTTP2OV(void)    		    __attribute__ ((interrupt));
void irq_INTTP2CC0(void)    		__attribute__ ((interrupt));
void irq_INTTP2CC1(void)    		__attribute__ ((interrupt));

void irq_INTTP3OV(void)    		    __attribute__ ((interrupt));
void irq_INTTP3CC0(void)    		__attribute__ ((interrupt));
void irq_INTTP3CC1(void)    		__attribute__ ((interrupt));

void irq_INTTP4OV(void)    		    __attribute__ ((interrupt));
void irq_INTTP4CC0(void)    		__attribute__ ((interrupt));
void irq_INTTP4CC1(void)    		__attribute__ ((interrupt));

void irq_INTTP5OV(void)    		    __attribute__ ((interrupt));
void irq_INTTP5CC0(void)    		__attribute__ ((interrupt));
void irq_INTTP5CC1(void)    		__attribute__ ((interrupt));

void irq_INTTM0EQ0(void)    		__attribute__ ((interrupt));

void irq_INTCB0R_INTIIC1(void)      __attribute__ ((interrupt));
void irq_INTCB0T(void)    		    __attribute__ ((interrupt));

void irq_INTCB1R(void)    		    __attribute__ ((interrupt));
void irq_INTCB1T(void)    		    __attribute__ ((interrupt));

void irq_INTCB2R(void)    		    __attribute__ ((interrupt));
void irq_INTCB2T(void)    		    __attribute__ ((interrupt));

void irq_INTCB3R(void)    		    __attribute__ ((interrupt));
void irq_INTCB3T(void)    		    __attribute__ ((interrupt));

void irq_INTUA0R_INTCB4R(void)    		    __attribute__ ((interrupt));
void irq_INTUA0T_INTCB4T(void)    		    __attribute__ ((interrupt));

void irq_INTUA1R_INTIIC2(void)    		    __attribute__ ((interrupt));
void irq_INTUA1T(void)    		    __attribute__ ((interrupt));

void irq_INTUA2R_INTIIC0(void)    		    __attribute__ ((interrupt));
void irq_INTUA2T(void)    		    __attribute__ ((interrupt));

void irq_INTAD(void)    		    __attribute__ ((interrupt));

void irq_INTDMA0(void)    		    __attribute__ ((interrupt));
void irq_INTDMA1(void)    		    __attribute__ ((interrupt));
void irq_INTDMA2(void)    		    __attribute__ ((interrupt));
void irq_INTDMA3(void)    		    __attribute__ ((interrupt));

void irq_INTKR(void)    		    __attribute__ ((interrupt));

void irq_INTWTI(void)    		    __attribute__ ((interrupt));
void irq_INTWT(void)    		    __attribute__ ((interrupt));

#ifdef __cplusplus
}
#endif


//--------------------------------------------------------//
// Definizione delle variabili
//--------------------------------------------------------//


#endif

//@@-----------------------------------------------------------------------@@//
//@@
//@@  $Source:  $
//@@  Note di revisione:
//@@
//@@  $Log:  $
//@@
//@@  $Author: junior $
//@@  $RCSfile: IRQSG2.H,v $
//@@  $Revision: 1.1 $
//@@  $State: Exp $
//@@  $Date: 2001/09/07 06:49:06 $
//@@  $Name:  $
//@@-----------------------------------------------------------------------@@//
//@@-----------------------------------------------------------------------@@//
//@@                                                                       @@//
//@@                            C O M E T A                                @@//
//@@                                                                       @@//
//@@-----------------------------------------------------------------------@@//
