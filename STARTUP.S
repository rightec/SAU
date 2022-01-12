-- //-------------------------------------------------------------------------//
-- //                                                                         //
-- //                         C O M E T A                                     //
-- //                                                                         //
-- //-------------------------------------------------------------------------//
-- //
-- //  Progetto:                CONFERENCE
-- //  Nome File:               STARTCG3.S
-- //  Descrizione:             Modulo di startup per Nec SG2
-- //						    in realtà è soprattutto un modulo di
-- //							gestione dei salti per routines interrupt
-- //  Supporto Fisico:         BR rev4
-- //  Versione:                $Revision: 1.7 $
-- //  Data Versione:           $Date: 2008/01/21 12:43:17 $
-- //  Descrizione Versione:    Stesura iniziale
-- //
-- //-------------------------------------------------------------------------//

		.include 	"v850SG2.inc"

-- -----------------------------------------------------
-- Funzioni globali esterne a questo modulo
-- -----------------------------------------------------
			.extern  _irq_NMI
			
			.extern  _irq_TRAP0
			.extern  _irq_TRAP1
			.extern  _irq_ILGOP_TRAPDB
			
			.extern  _irq_INTLVI
			
			.extern  _irq_INTP0
			.extern  _irq_INTP1
			.extern  _irq_INTP2
			.extern  _irq_INTP3
			.extern  _irq_INTP4
			.extern  _irq_INTP5
			.extern  _irq_INTP6
			.extern  _irq_INTP7
			
			.extern  _irq_INTTQ0OV	
			.extern  _irq_INTTQ0CC0
			.extern  _irq_INTTQ0CC1
			.extern  _irq_INTTQ0CC2
			.extern  _irq_INTTQ0CC3
			
			.extern  _irq_INTTP0OV	
			.extern  _irq_INTTP0CC0
			.extern  _irq_INTTP0CC1
			
			.extern  _irq_INTTP1OV	
			.extern  _irq_INTTP1CC0
			.extern  _irq_INTTP1CC1
			
			.extern  _irq_INTTP2OV	
			.extern  _irq_INTTP2CC0
			.extern  _irq_INTTP2CC1
			
			.extern  _irq_INTTP3OV	
			.extern  _irq_INTTP3CC0
			.extern  _irq_INTTP3CC1
			
			.extern  _irq_INTTP4OV	
			.extern  _irq_INTTP4CC0
			.extern  _irq_INTTP4CC1
			
			.extern  _irq_INTTP5OV	
			.extern  _irq_INTTP5CC0
			.extern  _irq_INTTP5CC1
			
			.extern  _irq_INTTM0EQ0
				
			.extern  _irq_INTCB0R_INTIIC1
			.extern  _irq_INTCB0T
			   
			.extern  _irq_INTCB1R  
			.extern  _irq_INTCB1T
			   
			.extern  _irq_INTCB2R   
			.extern  _irq_INTCB2T
			
			.extern  _irq_INTCB3R   
			.extern  _irq_INTCB3T
			   
			.extern  _irq_INTUA0R_INTCB4R  
			.extern  _irq_INTUA0T_INTCB4T
				
			.extern  _irq_INTUA1R_INTIIC2	
			.extern  _irq_INTUA1T
				
			.extern  _irq_INTUA2R_INTIIC0
			.extern  _irq_INTUA2T
				
			.extern  _irq_INTAD
				
			.extern  _irq_INTDMA0
			.extern  _irq_INTDMA1	
			.extern  _irq_INTDMA2	
			.extern  _irq_INTDMA3
			
			.extern  _irq_INTKR	
			
			.extern  _irq_INTWTI	
			.extern  _irq_INTWT
	


-- ----------------------------------------------------
-- Variabili globali utili a tutti i moduli
-- -----------------------------------------------------

-- -----------------------------------------------------
-- Funzioni globali utili a tutti i moduli
-- -----------------------------------------------------

-- -----------------------------------------------------
-- Variabili globali utili a tutti i moduli
-- -----------------------------------------------------
		.global	__system_stack
--		.global	__user_stack
	
--		.section	.user_stack
--__user_stack:		.space	1000			-- 8k di stack utente

		.section	.stack
		
__system_stack:		.space 	1024*8			-- 4K bytes di stack supervisore



-- -----------------------------------------------------
-- Funzioni statiche utili solo a questo modulo
-- -----------------------------------------------------

-- -----------------------------------------------------
-- Costanti statiche utili solo a questo modulo
-- -----------------------------------------------------

-- -----------------------------------------------------
-- Variabili statiche utili solo a questo modulo
-- -----------------------------------------------------



-------------------------------------------------------
-- Inizio codice segmento "INTRPT"
-------------------------------------------------------

			.global _irqJump
			.global _saveRegs
			
-------------------------------------------------------
-- Inizio codice vero e proprio
-------------------------------------------------------

			.section	.text
		
			.org RESET
_irqJump:

			jr		_cpu_initial				-- entry point RESET
			
chkSum:	.long	0							-- buchetto per il chksum applicativo
												-- stipato nella flash interna
			.long	(_intflash_end)				-- indirizzo di fine dell'app nella flash interna

			.org 	NMI								-- entry point NMI
			jr 		_irq_NMI

			.org  	TRAP0
			jr 		_irq_TRAP0

			.org  	TRAP1
			jr 		_irq_TRAP1

			.org  	ILGOP_TRAPDB
			jr 		_irq_ILGOP_TRAPDB
			
			.org  	INTLVI
			jr 		_irq_INTLVI

			.org  INTP0
			jr 		_irq_INTP0

			.org  INTP1 
			jr 		_irq_INTP1

			.org  INTP2 
			jr 		_irq_INTP2

			.org  INTP3 
			jr 		_irq_INTP3

			.org  INTP4 
			jr 		_irq_INTP4

			.org  INTP5 
			jr 		_irq_INTP5

			.org  INTP6 
			jr 		_irq_INTP6

			.org  INTP7 
			jr 		_irq_INTP7

			.org  INTTQ0OV
			jr 		_irq_INTTQ0OV

			.org  INTTQ0CC0
			jr 		_irq_INTTQ0CC0

			.org  INTTQ0CC1
			jr 		_irq_INTTQ0CC1

			.org  INTTQ0CC2
			jr 		_irq_INTTQ0CC2
			
			.org  INTTQ0CC3
			jr 		_irq_INTTQ0CC3

			.org  INTTP0OV	
			jr		_irq_INTTP0OV	

			.org  INTTP0CC0
			jr		_irq_INTTP0CC0
			
			.org  INTTP0CC1
			jr		_irq_INTTP0CC1
			
			.org  INTTP1OV	
			jr		_irq_INTTP1OV	
			
			.org  INTTP1CC0
			jr		_irq_INTTP1CC0
			
			.org  INTTP1CC1
			jr		_irq_INTTP1CC1
			
			.org  INTTP2OV	
			jr		_irq_INTTP2OV	
			
			.org  INTTP2CC0
			jr		_irq_INTTP2CC0
			
			.org  INTTP2CC1
			jr		_irq_INTTP2CC1
			
			.org  INTTP3OV	
			jr		_irq_INTTP2OV	
			
			.org  INTTP3CC0
			jr		_irq_INTTP3CC0
			
			.org  INTTP3CC1
			jr		_irq_INTTP3CC1
			
			.org  INTTP4OV	
			jr		_irq_INTTP4OV	
			
			.org  INTTP4CC0
			jr		_irq_INTTP4CC0
			
			.org  INTTP4CC1
			jr		_irq_INTTP4CC1
			
			.org  INTTP5OV	
			jr		_irq_INTTP5OV	
			
			.org  INTTP5CC0
			jr		_irq_INTTP5CC0
			
			.org  INTTP5CC1
			jr		_irq_INTTP5CC1
			
			.org  INTTM0EQ0
			jr		_irq_INTTM0EQ0	
			
			.org  INTCB0R_INTIIC1
			jr		_irq_INTCB0R_INTIIC1
			
			.org  INTCB0T  
			jr		_irq_INTCB0T  
			
			.org  INTCB1R  
			jr		_irq_INTCB1R   
			
			.org  INTCB1T   
			jr		_irq_INTCB1T  
			
			.org  INTCB2R  
			jr		_irq_INTCB2R   
			
			.org  INTCB2T   
			jr		_irq_INTCB2T 
			
			.org  INTCB3R  
			jr		_irq_INTCB3R   
			
			.org  INTCB3T   
			jr		_irq_INTCB3T   
			
			.org  INTUA0R_INTCB4R  
			jr		_irq_INTUA0R_INTCB4R 
			
			.org  INTUA0T_INTCB4T  
			jr		_irq_INTUA0T_INTCB4T   
			
			.org  INTUA1R_INTIIC2   
			jr		_irq_INTUA1R_INTIIC2   
			
			.org  INTUA1T
			jr		_irq_INTUA1T	
			
			.org  INTUA2R_INTIIC0	
			jr		_irq_INTUA2R_INTIIC0	
			
			.org  INTUA2T	
			jr		_irq_INTUA2T	
			
			.org  INTAD
			jr		_irq_INTAD	
			
			.org  INTDMA0	
			jr		_irq_INTDMA0	
			
			.org  INTDMA1	
			jr		_irq_INTDMA1	
			
			.org  INTDMA2	
			jr		_irq_INTDMA2
			
			.org  INTDMA3	
			jr		_irq_INTDMA3	
			
			.org  INTKR	
			jr		_irq_INTKR	
			
			.org  INTWTI	
			jr		_irq_INTWTI	
			
			.org  INTWT	
			jr		_irq_INTWT
			
	
	

-----------------------------------------------------------------------------
-- Seguono le tabelline di riepilogo dei segmenti di codice e dati 
-- sparpagliati nella memoria esterna ed loro crc di controllo
-----------------------------------------------------------------------------
			.org  0x600

CrcTab:		.long	0x11223344					-- magic number
			.long	2							-- tabelliamo un numero di 4 segmenti
			.long	0							-- chksum globale dell applicativo
			.long	_sAppRevision
			.long	_sAppName


--crc_fpga:	.long	0							-- crc del segmento
--			.long	(__fpgacode_start)			-- indirizzo di inizio del segmento
--			.long	(__fpgacode_end)			-- indirizzo di fine del segmento

crc_message:
			.long	0							-- 
			.long	(__message_start)			-- indirizzo di inizio del segmento
			.long	(__message_end)				-- indirizzo di fine del segmento

crc_rodata:	
			.long	0							-- 
			.long	(__rodata_start)			-- indirizzo di inizio del segmento
			.long	(__datarom_end)				-- indirizzo di fine del segmento

crc_spare:	
			.long	__stext							-- 
			.long	__etext							-- indirizzo di inizio del segmento
			.long	__rodata_start		
			.long	__datarom_end		
			.long	__message_start		
			.long	__message_end		
			.long	__ramcode_start 
			.long 	__ramcode_end
			.long	__data_start							-- indirizzo di fine del segmento
			.long	_end
			.long	__base_small_heap
			.long	__end_small_heap
			.long   __end_stack	
			.long   __xdata_start
			.long   __xdata_end
			.long	__base_heap
			.long	__end_heap
			.long   0		--__disk_start
			.long   0		--__disk_end
			.org  0x700

-----------------------------------------------------------------------------
-- Seguono le routines di libreria di prologo ed epilogo delle routines
-- di risposta agli interrupt modificate per consentire il nesting dei
-- diversi livelli di priorità disponibili
-- per gentile concessione della GNU
--
-----------------------------------------------------------------------------
#ifdef	L_save_interrupt
	.text
	.align	2
	.globl	__save_interrupt
	.type	__save_interrupt,@function
	/* Save registers r1, r4 on stack and load up with expected values */
	/* Note, 12 bytes of stack have already been allocated. */
	/* Called via:	jalr __save_interrupt,r10 */

__save_interrupt:
	st.w	ep,0[sp]
	st.w	gp,4[sp]
	st.w	r1,8[sp]
	movhi	hi(__ep),r0,ep
	movea	lo(__ep),ep,ep
	movhi	hi(__gp),r0,gp
	movea	lo(__gp),gp,gp
	jmp	[r10]
	.size	__save_interrupt,.-__save_interrupt

	/* Restore saved registers, deallocate stack and return from the interrupt */
	/* Called via:	jr __return_interrupt */
	.align	2
	.globl	__return_interrupt
	.type	__return_interrupt,@function
__return_interrupt:
	ld.w	0[sp],ep
	ld.w	4[sp],gp
	ld.w	8[sp],r1
	ld.w	12[sp],r10

	addi	16,sp,sp
	reti
	.size	__return_interrupt,.-__return_interrupt
#endif /* L_save_interrupt */

#ifdef L_save_all_interrupt
	.text
	.align	2
	.globl	__save_all_interrupt
	.type	__save_all_interrupt,@function
	/* Save all registers except for those saved in __save_interrupt */
	/* allocate enough stack for all of the registers & 16 bytes of space */
	/* Called via:	jalr __save_all_interrupt,r10 */
__save_all_interrupt:
	addi	-128,sp,sp

	stsr	1,r1				-- salva i registri di copia del PC di ritorno e 
	st.w	r1,120[sp]			-- PSW di ritorno
	stsr	0,r1
	st.w	r1,124[sp]

	mov	ep,r1
	mov	sp,ep
	
	ei							-- fast enable int. a registri non completamente
								-- salvati (int. + veloce)

	sst.w	r31,116[ep]
	sst.w	r2,112[ep]
	sst.w	gp,108[ep]
	sst.w	r6,104[ep]
	sst.w	r7,100[ep]
	sst.w	r8,96[ep]
	sst.w	r9,92[ep]
	sst.w	r11,88[ep]
	sst.w	r12,84[ep]
	sst.w	r13,80[ep]
	sst.w	r14,76[ep]
	sst.w	r15,72[ep]
	sst.w	r16,68[ep]
	sst.w	r17,64[ep]
	sst.w	r18,60[ep]
	sst.w	r19,56[ep]
	sst.w	r20,52[ep]
	sst.w	r21,48[ep]
	sst.w	r22,44[ep]
	sst.w	r23,40[ep]
	sst.w	r24,36[ep]
	sst.w	r25,32[ep]
	sst.w	r26,28[ep]
	sst.w	r27,24[ep]
	sst.w	r28,20[ep]
	sst.w	r29,16[ep]

	addi	-16,sp,sp			-- generiamo un ulteriore frame di stack che le 
								-- funzioni chiamate si aspettano di avere a disposizione
								-- ma che il gestore di interrupt non contempla

	mov	r1,ep
	jmp	[r10]

	.size	__save_all_interrupt,.-__save_all_interrupt

	.globl	__restore_all_interrupt
	.type	__restore_all_interrupt,@function
	/* Restore all registers saved in __save_all_interrupt */
	/* & deallocate the stack space */
	/* Called via:	jalr __restore_all_interrupt,r10 */
__restore_all_interrupt:

	addi	16,sp,sp			-- recuperiamo l'ulteriore frame di stack che le 
								-- funzioni chiamate si aspettano di avere a disposizione
								-- ma che il gestore di interrupt non contempla

	mov	ep,r1
	mov	sp,ep

	sld.w	116[ep],r31
	sld.w	112[ep],r2
	sld.w	108[ep],gp
	sld.w	104[ep],r6
	sld.w	100[ep],r7
	sld.w	96[ep],r8
	sld.w	92[ep],r9
	sld.w	88[ep],r11
	sld.w	84[ep],r12
	sld.w	80[ep],r13
	sld.w	76[ep],r14
	sld.w	72[ep],r15
	sld.w	68[ep],r16
	sld.w	64[ep],r17
	sld.w	60[ep],r18
	sld.w	56[ep],r19
	sld.w	52[ep],r20
	sld.w	48[ep],r21
	sld.w	44[ep],r22
	sld.w	40[ep],r23
	sld.w	36[ep],r24
	sld.w	32[ep],r25
	sld.w	28[ep],r26
	sld.w	24[ep],r27
	sld.w	20[ep],r28
	sld.w	16[ep],r29

	di							-- disabilitiamo per sicurezza

	mov	r1,ep

	ld.w	124[sp],r1			-- recupera i registri speciali
	ldsr	r1,0
	ld.w	120[sp],r1
	ldsr	r1,1

	addi	128,sp,sp
	jmp	[r10]
	.size	__restore_all_interrupt,.-__restore_all_interrupt
#endif /* L_save_all_interrupt */


-----------------------------------------------------------------------------
-- Segue l'inizializzazione dei vari registri del microbo
-- per gentile concessione della NEC
--
-----------------------------------------------------------------------------
	.global	_cpu_initial
	
_cpu_initial:
		nop
		nop
		nop

		
		jr		_start					-- entry point C-Runtime init

-----------------------------------------------------------------------------



