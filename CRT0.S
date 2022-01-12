


# NEC V850 / V850E startup code

		.section .text
		.global	_start

_start:
		di									-- non fa mai male

		sub		r0, r0
		movea   255,r0,	r20
		mov     r0,r21
		ori     65535,r0,r21
		movhi   hi(_end_stack),r0,sp
		movea   lo(_end_stack),sp,sp
		
--		movhi   hi(__stack),r0,sp
--		movea   lo(__stack),sp,sp

--		movhi   hi(__system_stack),r0,sp
--		movea   lo(__system_stack),sp,sp

--		movhi   hi(__system_stack),r0,r2		-- in teoria solo r2 dovrebbe essere lo stack di sistema
--		movea   lo(__system_stack),sp,r2

		movhi   hi(__ep),r0,ep
		movea   lo(__ep),ep,ep
		movhi   hi(__gp),r0,gp
		movea   lo(__gp),gp,gp

-- #### Initialization of main memory ####
											-- spazzola l'area bss
		movhi   hi(_edata),	r0,	r6
		movea   lo(_edata),	r6,	r6
		movhi   hi(_end),	r0,	r7
		movea   lo(_end),	r7,	r7

.L0:
		st.b    r0,		0[r6]
		addi    1,		r6,	r6
		cmp     r7,		r6
		bl      .L0

-- ### Mettiamo a zero tutta la heap ---

		movhi   hi(__base_small_heap),	r0,	r6
		movea   lo(__base_small_heap),	r6,	r6
		movhi   hi(_end_small_heap),	r0,	r7
		movea   lo(_end_small_heap),	r7,	r7

.L1:
		st.b    r0,		0[r6]
		addi    1,		r6,	r6
		cmp     r7,		r6
		bl      .L1

		movhi   hi(__base_heap),	r0,	r6
		movea   lo(__base_heap),	r6,	r6
		movhi   hi(_end_heap),	r0,	r7
		movea   lo(_end_heap),	r7,	r7

.L2:
		st.b    r0,		0[r6]
		addi    1,		r6,	r6
		cmp     r7,		r6
		bl      .L2

		jarl    ___main,	r31				-- chiama il c++ startup
		addi    -12,		sp,	sp


		mov     0,	r6						-- argc
		mov     0,	r7						-- argv
		mov     0,	r8						-- envp
		jarl    _main,	r31					-- finalmente chiama la main function

		mov     r10,r6
		jarl    _start,	r31


--		.section .stack

--_stack:	.long 	1


