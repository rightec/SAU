/**
@file		IrqSG2.CPP
@brief		Interrupts reallocation routines 
					
@author		
@date		18/01/2011
@version	01.00
*/

#include "Irqsg2.h"

/**
Spurius ISR to link to the interrupt funtion table when no Interrupt service routinr was linked by the user.
@param vect interrupt vector.
@see pfIrqHandTab
*/
static void irq_spurHandle(int vect);


#ifdef __SDATA__							// se utilizza il segmento sdata ....
															// puntatori alle funzioni che gestiscono gli ints.
static void (*pfIrqHandTab[IRQ_VEC_MAX])(int vect) 	__attribute__ ((section (".sdata")));		

static IRQErrors irqError 							__attribute__ ((section (".sdata")));
static word spurVect = 0xFFFF;
															// flag tabella inizializzata
static word irqTabInit 								__attribute__ ((section (".sdata")));		
															// vettore entrato prima dell'iniz.
static word bastVect 								__attribute__ ((section (".sdata")));
#else
/**
Interrupt interrupt function table.

Array thay points to the Interrupt Service Routines
*/
static void (*pfIrqHandTab[IRQ_VEC_MAX])(int vect);

/**
Errors occurred.
**/
static IRQErrors irqError;
/**
Vector that calls the spurious ISR, irq_spurHandle
*/
static word spurVect = 0xFFFF;
static word irqTabInit;
/**
Used in debug mode.
*/
static word bastVect;
#endif

/**
Initialization IRQ module
*/
void irq_init(void)
{
int i;

	for(i=0; i<IRQ_VEC_MAX; i++)						// precarica i vettori
		pfIrqHandTab[i] = irq_spurHandle;

	spurVect = 0xFFFF;									// precarica il registro
	bastVect = 0xFFFF;									// precarica il registro vettori bastardi
	irqTabInit = 0x55AA;								// flag tabella inizializzata
}

/**
Sets the ISR for the desire interrupt vector.

@param reqVect desire interrupt vector
@param pfIrqHandle pointer to the Interrupt Service Routine (IRS)
@return true if the allocation va succesfully, false othewise
*/
bool irq_setVect(IRQVectors reqVect, void (*pfIrqHandle)(int vect) )
{
IRQErrors localError = IRQ_ERR_NONE;

	if( reqVect >= IRQ_VEC_MAX )
		localError = IRQ_ERR_INV_VECT;					// è un bell'errorone !!

	if( pfIrqHandle == NULL )
		localError = IRQ_ERR_INV_FUN;					// è un bell'errorone !!


	if( pfIrqHandTab[reqVect] != irq_spurHandle )		// se è già stato impostato
		localError = IRQ_ERR_ILL_VECT;					// non lo posso sovrascrivere

	if( localError )
	{
		irqError = localError;							// cosi' non rischio di perderlo
#ifdef __DEBUG__										// se esplicitamente richiesto
		error_trace("IRQSG2", "irq_setVect", irqError);	// segnala l'errore
#endif
		return(False);
	}

	pfIrqHandTab[reqVect] = pfIrqHandle;				// imposta la funzione richiesta

	return(True);										// tutto OK
}

/**
Reset the ISR linked with the interrupt vector reqVect. Assign to the specif inverrupt vector the spurius ISR, irq_spurHandle
@param reqVect interrupt vector to reset
@return true if the reqVect is valide vector, false otherwise.
*/
bool irq_resetVect(IRQVectors reqVect)
{
	if( reqVect >= IRQ_VEC_MAX )
	{
		irqError = IRQ_ERR_INV_VECT;				// è un bell'errorone !!

#ifdef __DEBUG__									// se esplicitamente richiesto
		error_trace("IRQSG2", "irq_resetVect", irqError);	// segnala l'errore
#endif
		return(False);
	}

	pfIrqHandTab[reqVect] = irq_spurHandle;			// imposta l'handler di default
	return(True);									// tutto OK
}

/**
Checks if errors occured.
*/
void irq_chkErrors(void)
{
	if( irqError == IRQ_ERR_INTSPUR || bastVect != 0xFFFF )
	{
//		error_trace("IRQSG2", "irq_chkErros", irqError);	// segnala l'errore
//		cprintf("irqError = %ld, irqVector = %u, bastVect = %u\n\r", irqError, spurVect, bastVect);	// interrupt spurio agganciato
		irqError = IRQ_ERR_NONE;
		spurVect = 0xFFFF;
	}
}

static void irq_spurHandle(int vect)
{
	irqError = IRQ_ERR_INTSPUR;				// interrupt spurio agganciato
	spurVect = vect;						// e questo è il faccione che lo ha generato
}


/////////////////////////////////////////////////////////////////////
//
// Gestori selettivi dei vari interrupts vaganti
//
// Note:
//  1) se si necessita di una esecuzione + veloce e consigliabile
// 	   intercettare l'irq in assembler nel file "startup.s"
//
/////////////////////////////////////////////////////////////////////

void irq_RESET(void)
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_RESET](IRQ_VEC_RESET);		// Reset input
	else
		bastVect = IRQ_VEC_RESET;
#else
		pfIrqHandTab[IRQ_VEC_RESET](IRQ_VEC_RESET);		// Reset input
#endif
}


void irq_NMI(void)
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_NMI](IRQ_VEC_NMI);			// NMI Input pin
	else
		bastVect = IRQ_VEC_NMI;
#else
	pfIrqHandTab[IRQ_VEC_NMI](IRQ_VEC_NMI);				// NMI Input pin
#endif
}

void irq_TRAP0(void)
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_TRAP0](IRQ_VEC_TRAP0);		// TRAP instruction
	else
		bastVect = IRQ_VEC_TRAP0;
#else
	pfIrqHandTab[IRQ_VEC_TRAP0](IRQ_VEC_TRAP0);			// TRAP instruction
#endif
}

void irq_TRAP1(void)
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_TRAP1](IRQ_VEC_TRAP1);		// TRAP instruction
	else
		bastVect = IRQ_VEC_TRAP1;
#else
	pfIrqHandTab[IRQ_VEC_TRAP1](IRQ_VEC_TRAP1);			// TRAP instruction
#endif
}

void irq_ILGOP_TRAPDB(void)
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_ILGOP_TRAPDB](IRQ_VEC_ILGOP_TRAPDB);		// Illegal opcode
	else
		bastVect = IRQ_VEC_ILGOP_TRAPDB;
#else
	pfIrqHandTab[IRQ_VEC_ILGOP_TRAPDB](IRQ_VEC_ILGOP_TRAPDB);			// Illegal opcode
#endif
}


void irq_INTLVI(void)              
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTLVI](IRQ_VEC_INTLVI);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTLVI;
#else
	pfIrqHandTab[IRQ_VEC_INTLVI](IRQ_VEC_INTLVI);			// Illegal opcode
#endif
}

void irq_INTP0(void)	            
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTP0](IRQ_VEC_INTP0);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTP0;
#else
	pfIrqHandTab[IRQ_VEC_INTP0](IRQ_VEC_INTP0);			// Illegal opcode
#endif
}

void irq_INTP1(void)              
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTP1](IRQ_VEC_INTP1);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTP1;
#else
	pfIrqHandTab[IRQ_VEC_INTP1](IRQ_VEC_INTP1);			// Illegal opcode
#endif
}

void irq_INTP2(void)              
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTP2](IRQ_VEC_INTP2);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTP2;
#else
	pfIrqHandTab[IRQ_VEC_INTP2](IRQ_VEC_INTP2);			// Illegal opcode
#endif
}


void irq_INTP3(void)      
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTP3](IRQ_VEC_INTP3);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTP3;
#else
	pfIrqHandTab[IRQ_VEC_INTP3](IRQ_VEC_INTP3);			// Illegal opcode
#endif
}

void irq_INTP4(void)      
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTP4](IRQ_VEC_INTP4);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTP4;
#else
	pfIrqHandTab[IRQ_VEC_INTP4](IRQ_VEC_INTP4);			// Illegal opcode
#endif
}


void irq_INTP5(void)      
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTP5](IRQ_VEC_INTP5);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTP5;
#else
	pfIrqHandTab[IRQ_VEC_INTP5](IRQ_VEC_INTP5);			// Illegal opcode
#endif
}

void irq_INTP6(void)      
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTP6](IRQ_VEC_INTP6);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTP6;
#else
	pfIrqHandTab[IRQ_VEC_INTP6](IRQ_VEC_INTP6);			// Illegal opcode
#endif
}

void irq_INTP7(void)      
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTP7](IRQ_VEC_INTP7);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTP7;
#else
	pfIrqHandTab[IRQ_VEC_INTP7](IRQ_VEC_INTP7);			// Illegal opcode
#endif
}

void irq_INTTQ0OV(void)      
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTTQ0OV](IRQ_VEC_INTTQ0OV);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTTQ0OV;
#else
	pfIrqHandTab[IRQ_VEC_INTTQ0OV](IRQ_VEC_INTTQ0OV);			// Illegal opcode
#endif
}

void irq_INTTQ0CC0(void)      
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTTQ0CC0](IRQ_VEC_INTTQ0CC0);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTTQ0CC0;
#else
	pfIrqHandTab[IRQ_VEC_INTTQ0CC0](IRQ_VEC_INTTQ0CC0);			// Illegal opcode
#endif
}

void irq_INTTQ0CC1(void)      
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTTQ0CC1](IRQ_VEC_INTTQ0CC1);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTTQ0CC1;
#else
	pfIrqHandTab[IRQ_VEC_INTTQ0CC1](IRQ_VEC_INTTQ0CC1);			// Illegal opcode
#endif
}


void irq_INTTQ0CC2(void)			    
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTTQ0CC2](IRQ_VEC_INTTQ0CC2);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTTQ0CC2;
#else
	pfIrqHandTab[IRQ_VEC_INTTQ0CC2](IRQ_VEC_INTTQ0CC2);			// Illegal opcode
#endif
}

void irq_INTTQ0CC3(void)    		    
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTTQ0CC3](IRQ_VEC_INTTQ0CC3);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTTQ0CC3;
#else
	pfIrqHandTab[IRQ_VEC_INTTQ0CC3](IRQ_VEC_INTTQ0CC3);			// Illegal opcode
#endif
}

void irq_INTTP0OV(void)    		    
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTTP0OV](IRQ_VEC_INTTP0OV);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTTP0OV;
#else
	pfIrqHandTab[IRQ_VEC_INTTP0OV](IRQ_VEC_INTTP0OV);			// Illegal opcode
#endif
}

void irq_INTTP0CC0(void)    		    
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTTP0CC0](IRQ_VEC_INTTP0CC0);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTTP0CC0;
#else
	pfIrqHandTab[IRQ_VEC_INTTP0CC0](IRQ_VEC_INTTP0CC0);			// Illegal opcode
#endif
}

void irq_INTTP0CC1(void)    		    
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTTP0CC1](IRQ_VEC_INTTP0CC1);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTTP0CC1;
#else
	pfIrqHandTab[IRQ_VEC_INTTP0CC1](IRQ_VEC_INTTP0CC1);			// Illegal opcode
#endif
}

void irq_INTTP1OV(void)    		    
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTTP1OV](IRQ_VEC_INTTP1OV);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTTP1OV;
#else
	pfIrqHandTab[IRQ_VEC_INTTP1OV](IRQ_VEC_INTTP1OV);			// Illegal opcode
#endif
}

void irq_INTTP1CC0(void)    		    
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTTP1CC0](IRQ_VEC_INTTP1CC0);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTTP1CC0;
#else
	pfIrqHandTab[IRQ_VEC_INTTP1CC0](IRQ_VEC_INTTP1CC0);			// Illegal opcode
#endif
}

void irq_INTTP1CC1(void)    		    
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTTP1CC1](IRQ_VEC_INTTP1CC1);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTTP1CC1;
#else
	pfIrqHandTab[IRQ_VEC_INTTP1CC1](IRQ_VEC_INTTP1CC1);			// Illegal opcode
#endif
}

void irq_INTTP2OV(void)    		    
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTTP2OV](IRQ_VEC_INTTP2OV);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTTP2OV;
#else
	pfIrqHandTab[IRQ_VEC_INTTP2OV](IRQ_VEC_INTTP2OV);			// Illegal opcode
#endif
}

void irq_INTTP2CC0(void)    		    
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTTP2CC0](IRQ_VEC_INTTP2CC0);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTTP2CC0;
#else
	pfIrqHandTab[IRQ_VEC_INTTP2CC0](IRQ_VEC_INTTP2CC0);			// Illegal opcode
#endif
}

void irq_INTTP2CC1(void)    		    
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTTP2CC1](IRQ_VEC_INTTP2CC1);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTTP2CC1;
#else
	pfIrqHandTab[IRQ_VEC_INTTP2CC1](IRQ_VEC_INTTP2CC1);			// Illegal opcode
#endif
}

void irq_INTTP3OV(void)    		    
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTTP3OV](IRQ_VEC_INTTP3OV);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTTP3OV;
#else
	pfIrqHandTab[IRQ_VEC_INTTP3OV](IRQ_VEC_INTTP3OV);			// Illegal opcode
#endif
}

void irq_INTTP3CC0(void)    		    
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTTP3CC0](IRQ_VEC_INTTP3CC0);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTTP3CC0;
#else
	pfIrqHandTab[IRQ_VEC_INTTP3CC0](IRQ_VEC_INTTP3CC0);			// Illegal opcode
#endif
}

void irq_INTTP3CC1(void)    		    
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTTP3CC1](IRQ_VEC_INTTP3CC1);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTTP3CC1;
#else
	pfIrqHandTab[IRQ_VEC_INTTP3CC1](IRQ_VEC_INTTP3CC1);			// Illegal opcode
#endif
}

void irq_INTTP4OV(void)    		    
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTTP4OV](IRQ_VEC_INTTP4OV);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTTP4OV;
#else
	pfIrqHandTab[IRQ_VEC_INTTP4OV](IRQ_VEC_INTTP4OV);			// Illegal opcode
#endif
}

void irq_INTTP4CC0(void)    		    
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTTP4CC0](IRQ_VEC_INTTP4CC0);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTTP4CC0;
#else
	pfIrqHandTab[IRQ_VEC_INTTP4CC0](IRQ_VEC_INTTP4CC0);			// Illegal opcode
#endif
}

void irq_INTTP4CC1(void)    		    
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTTP4CC1](IRQ_VEC_INTTP4CC1);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTTP4CC1;
#else
	pfIrqHandTab[IRQ_VEC_INTTP4CC1](IRQ_VEC_INTTP4CC1);			// Illegal opcode
#endif
}

void irq_INTTP5OV(void)    		    
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTTP5OV](IRQ_VEC_INTTP5OV);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTTP5OV;
#else
	pfIrqHandTab[IRQ_VEC_INTTP5OV](IRQ_VEC_INTTP5OV);			// Illegal opcode
#endif
}

void irq_INTTP5CC0(void)    		    
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTTP5CC0](IRQ_VEC_INTTP5CC0);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTTP5CC0;
#else
	pfIrqHandTab[IRQ_VEC_INTTP5CC0](IRQ_VEC_INTTP5CC0);			// Illegal opcode
#endif
}

void irq_INTTP5CC1(void)    		    
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTTP5CC1](IRQ_VEC_INTTP5CC1);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTTP5CC1;
#else
	pfIrqHandTab[IRQ_VEC_INTTP5CC1](IRQ_VEC_INTTP5CC1);			// Illegal opcode
#endif
}

void irq_INTTM0EQ0(void)    		    
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTTM0EQ0](IRQ_VEC_INTTM0EQ0);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTTM0EQ0;
#else
	pfIrqHandTab[IRQ_VEC_INTTM0EQ0](IRQ_VEC_INTTM0EQ0);			// Illegal opcode
#endif
}

void irq_INTCB0R_INTIIC1(void)    		    
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTCB0R_INTIIC1](IRQ_VEC_INTCB0R_INTIIC1);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTCB0R_INTIIC1;
#else
	pfIrqHandTab[IRQ_VEC_INTCB0R_INTIIC1](IRQ_VEC_INTCB0R_INTIIC1);			// Illegal opcode
#endif
}

void irq_INTCB0T(void)    		    
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTCB0T](IRQ_VEC_INTCB0T);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTCB0T;
#else
	pfIrqHandTab[IRQ_VEC_INTCB0T](IRQ_VEC_INTCB0T);			// Illegal opcode
#endif
}

void irq_INTCB1R(void)    		    
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTCB1R](IRQ_VEC_INTCB1R);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTCB1R;
#else
	pfIrqHandTab[IRQ_VEC_INTCB1R](IRQ_VEC_INTCB1R);			// Illegal opcode
#endif
}

void irq_INTCB1T(void)    		    
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTCB1T](IRQ_VEC_INTCB1T);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTCB1T;
#else
	pfIrqHandTab[IRQ_VEC_INTCB1T](IRQ_VEC_INTCB1T);			// Illegal opcode
#endif
}

void irq_INTCB2R(void)    		    
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTCB2R](IRQ_VEC_INTCB2R);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTCB2R;
#else
	pfIrqHandTab[IRQ_VEC_INTCB2R](IRQ_VEC_INTCB2R);			// Illegal opcode
#endif
}

void irq_INTCB2T(void)    		    
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTCB2T](IRQ_VEC_INTCB2T);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTCB2T;
#else
	pfIrqHandTab[IRQ_VEC_INTCB2T](IRQ_VEC_INTCB2T);			// Illegal opcode
#endif
}

void irq_INTCB3R(void)    		    
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTCB3R](IRQ_VEC_INTCB3R);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTCB3R;
#else
	pfIrqHandTab[IRQ_VEC_INTCB3R](IRQ_VEC_INTCB3R);			// Illegal opcode
#endif
}

void irq_INTCB3T(void)    		    
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTCB3T](IRQ_VEC_INTCB3T);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTCB3T;
#else
	pfIrqHandTab[IRQ_VEC_INTCB3T](IRQ_VEC_INTCB3T);			// Illegal opcode
#endif
}

/* esportato nel main per ottimizzare xfr seriale 
void irq_INTUA0R_INTCB4R(void)    		    
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTUA0R_INTCB4R](IRQ_VEC_INTUA0R_INTCB4R);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTUA0R_INTCB4R;
#else
	pfIrqHandTab[IRQ_VEC_INTUA0R_INTCB4R](IRQ_VEC_INTUA0R_INTCB4R);			// Illegal opcode
#endif
}
*/

/* esportato nel main per ottimizzare xfr seriale 
void irq_INTUA0T_INTCB4T(void)    		    
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTUA0T_INTCB4T](IRQ_VEC_INTUA0T_INTCB4T);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTUA0T_INTCB4T;
#else
	pfIrqHandTab[IRQ_VEC_INTUA0T_INTCB4T](IRQ_VEC_INTUA0T_INTCB4T);			// Illegal opcode
#endif
}
*/

void irq_INTUA1R_INTIIC2(void)    		    
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTUA1R_INTIIC2](IRQ_VEC_INTUA1R_INTIIC2);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTUA1R_INTIIC2;
#else
	pfIrqHandTab[IRQ_VEC_INTUA1R_INTIIC2](IRQ_VEC_INTUA1R_INTIIC2);			// Illegal opcode
#endif
}

void irq_INTUA1T(void)    		    
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTUA1T](IRQ_VEC_INTUA1T);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTUA1T;
#else
	pfIrqHandTab[IRQ_VEC_INTUA1T](IRQ_VEC_INTUA1T);			// Illegal opcode
#endif
}

void irq_INTUA2R_INTIIC0(void)    		    
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTUA2R_INTIIC0](IRQ_VEC_INTUA2R_INTIIC0);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTUA2R_INTIIC0;
#else
	pfIrqHandTab[IRQ_VEC_INTUA2R_INTIIC0](IRQ_VEC_INTUA2R_INTIIC0);			// Illegal opcode
#endif
}

void irq_INTUA2T(void)    		    
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTUA2T](IRQ_VEC_INTUA2T);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTUA2T;
#else
	pfIrqHandTab[IRQ_VEC_INTUA2T](IRQ_VEC_INTUA2T);			// Illegal opcode
#endif
}

void irq_INTAD(void)			    
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTAD](IRQ_VEC_INTAD);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTAD;
#else
	pfIrqHandTab[IRQ_VEC_INTAD](IRQ_VEC_INTAD);			// Illegal opcode
#endif
}

void irq_INTDMA0(void)			    
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTDMA0](IRQ_VEC_INTDMA0);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTDMA0;
#else
	pfIrqHandTab[IRQ_VEC_INTDMA0](IRQ_VEC_INTDMA0);			// Illegal opcode
#endif

}

void irq_INTDMA1(void)			    
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTDMA1](IRQ_VEC_INTDMA1);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTDMA1;
#else
	pfIrqHandTab[IRQ_VEC_INTDMA1](IRQ_VEC_INTDMA1);			// Illegal opcode
#endif
}

void irq_INTDMA2(void)			    
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTDMA2](IRQ_VEC_INTDMA2);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTDMA2;
#else
	pfIrqHandTab[IRQ_VEC_INTDMA2](IRQ_VEC_INTDMA2);			// Illegal opcode
#endif
}


void irq_INTDMA3(void)
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTDMA3](IRQ_VEC_INTDMA3);	// DMA0 End xfr Interrupt
	else
		bastVect = IRQ_VEC_INTDMA3;
#else
	pfIrqHandTab[IRQ_VEC_INTDMA3](IRQ_VEC_INTDMA3);		// DMA0 End xfr Interrupt
#endif
}

void irq_INTKR(void)       
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTKR](IRQ_VEC_INTKR);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTKR;
#else
	pfIrqHandTab[IRQ_VEC_INTKR](IRQ_VEC_INTKR);			// Illegal opcode
#endif
}

void irq_INTWTI(void)        
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTWTI](IRQ_VEC_INTWTI);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTWTI;
#else
	pfIrqHandTab[IRQ_VEC_INTWTI](IRQ_VEC_INTWTI);			// Illegal opcode
#endif
}

void irq_INTWT(void)	        
{
#ifdef __DEBUG__										// se esplicitamente richiesto
	if( irqTabInit == IRQ_INIT_DONE )
		pfIrqHandTab[IRQ_VEC_INTWT](IRQ_VEC_INTWT);		// Illegal opcode
	else
		bastVect = IRQ_VEC_INTWT;
#else
	pfIrqHandTab[IRQ_VEC_INTWT](IRQ_VEC_INTWT);			// Illegal opcode
#endif
}


//@@-----------------------------------------------------------------------@@//
//@@
//@@  $Source:  $
//@@  Note di revisione:
//@@
//@@  $Revision: 1.2 $
//@@  $Date: 2002/03/22 00:00:00 $
//@@  			Il vettore "pfIrqHandTab" viene istanziato per primo perchè
//@@			il compilatore non allinea ai 4 bytes le variabili statiche
//@@			globali e quindi se prima del vettore dichiaro una varibile
//@@ 			di tipo "word" il vettore viene istanziato ad un indirizzo
//@@			allineato ai 2bytes anzichè ai 4bytes con conseguente
//@@			disallineamento degli elementi della struttura del vettore.
//@@
//@@  			Vengono eliminati tutti gli "asm(EI)" perchè ora nel modulo
//@@			"startup" le interruzioni vengono abilitate prima del
//@@			salvataggio completo dei registri.
//@@
//@@  $Revision: 1.3 $
//@@  $Date: 2004/05/27 00:00:00 $
//@@  			Vengono accorciati i nomi delle costanti di definizione 
//@@			dei vettori di interrupt xchè erroneamente erano stati 
//@@			inclusi i flag di interrupt-pending
//@@
//@@  $Log:  $
//@@
//@@  $Author: junior $
//@@  $RCSfile: IRQSG2.CPP,v $
//@@  $Revision: 1.3 $
//@@  $State: Exp $
//@@  $Date: 2002/05/21 00:00:00 $
//@@  $Name:  $
//@@-----------------------------------------------------------------------@@//
//@@-----------------------------------------------------------------------@@//
//@@                                                                       @@//
//@@                            C O M E T A                                @@//
//@@                                                                       @@//
//@@-----------------------------------------------------------------------@@//


