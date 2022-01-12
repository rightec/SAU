/**
@file		Timer_NEC.h
@brief		timer classes

@author		Nicola Molinazzi
@date		10/10/2011
@version	01.0

*/



#include "Timer_NEC.h"

Timer_NEC::Timer_NEC(volatile NEC_timer_Generic_t *_pReg):TimerAbstract()
{
	m_pReg = _pReg;
}

bool Timer_NEC::setTxnEST(TxnEST est, bool forceModify)
{
	if (m_TxnEST != est || forceModify)
	{
		m_TxnEST = est;
		m_pReg->CTL1 &= ~0x40;
		switch(est)
		{
			case TxnEST_noSwTrigCntr:
				break;
			case TxnEST_swTrigCntr:
				m_pReg->CTL1 |= 0x40;
				break;
		}
			
		return true;
	}
	return false;
}


bool Timer_NEC::setTxnEEE(TxnEEE eee, bool forceModify)
{
	if (m_TxnEEE != eee || forceModify)
	{
		m_TxnEEE = eee;
		m_pReg->CTL1 &= ~0x20;
		switch(eee)
		{
			case TxnEEE_noExtClk:
				break;
			case TxnEEE_extClk:
				m_pReg->CTL1 |= 0x20;
				break;
		}
			
		return true;
	}
	return false;
}

bool Timer_NEC::setTimerMode(TimerMode tm, bool forceModify)
{
	if (m_timerMode!= tm || forceModify)
	{
		m_timerMode = tm;
		m_pReg->CTL1 &= ~0x07;
		switch(tm)
		{
			case tm_IntervalTimerMode:
				break;
			case tm_ExtEventCountMode:
				m_pReg->CTL1 |= 0x01;
				break;
			case tm_extTrigPulseOutputMode:
				m_pReg->CTL1 |= 0x02;
				break;
			case tm_OneShotPulseOutputMode:
				m_pReg->CTL1 |= 0x03;
				break;
			case tm_pwmOutputMode:
				m_pReg->CTL1 |= 0x04;
				break;
			case tm_FreeRunningTimerMode:
				m_pReg->CTL1 |= 0x06;
				break;
			case tm_pulseWidthMeasurementMode:
				m_pReg->CTL1 |= 0x06;
				break;
		}
			
		return true;
	}
	return false;
}

bool Timer_NEC::setTxnOL1(TxnOL1 OL1, bool forceModify)
{
	if (m_TxnOL1 != OL1 || forceModify)
	{
		m_TxnOL1 = OL1;
		m_pReg->IOC0 &= ~0x08;
		switch(OL1)
		{
			case TOPn1_startHigh:
				break;
			case TOPn1_startLow:
				m_pReg->IOC0 |= 0x08;
				break;
		}
			
		return true;
	}
	return false;
}

bool Timer_NEC::setTxnOE1(TxnOE1 OE1, bool forceModify)
{
	if (m_TxnOE1 != OE1 || forceModify)
	{
		m_TxnOE1 = OE1;
		m_pReg->IOC0 &= ~0x04;
		switch(OE1)
		{
			case TxnOE1_outputDisable:
				break;
			case TxnOE1_outputEnable:
				m_pReg->IOC0 |= 0x04;
				break;
		}
			
		return true;
	}
	return false;
}

bool Timer_NEC::setTxnOL0(TxnOL0 OL0, bool forceModify)
{
	if (m_TxnOL0 != OL0 || forceModify)
	{
		m_TxnOL0 = OL0;
		m_pReg->IOC0 &= ~0x02;
		switch(OL0)
		{
			case TOPn0_startHigh:
				break;
			case TOPn0_startLow:
				m_pReg->IOC0 |= 0x02;
				break;
		}
			
		return true;
	}
	return false;
}

bool Timer_NEC::setTxnOE0(TxnOE0 OE0, bool forceModify)
{
	if (m_TxnOE0 != OE0 || forceModify)
	{
		m_TxnOE0 = OE0;
		m_pReg->IOC0 &= ~0x01;
		switch(OE0)
		{
			case TxnOE0_outputDisable:
				break;
			case TxnOE0_outputEnable:
				m_pReg->IOC0 |= 0x01;
				break;
		}
			
		return true;
	}
	return false;
}

bool Timer_NEC::setValidEdgeCaptureTrigTIxn1(ValidEdgeCaptureTrigTIxn1 ve, bool forceModify)
{
	if (m_ValidEdgeCaptureTrigTIxn1 != ve || forceModify)
	{
		m_ValidEdgeCaptureTrigTIxn1 = ve;
		m_pReg->IOC1 &= ~0x0C;
		switch(ve)
		{
			case vectTIxn1_noEdge:
				
				break;
			case vectTIxn1_risingEdge:
				m_pReg->IOC1 |= 0x04;
				break;
			case vectTIxn1_fallingEdge:
				m_pReg->IOC1 |= 0x08;
				break;
			case vectTIxn1_bothEdges:
				m_pReg->IOC1 |= 0x0C;
				break;
		}
			
		return true;
	}
	return false;
}

bool Timer_NEC::setValidEdgeCaptureTrigTIxn0(ValidEdgeCaptureTrigTIxn0 ve, bool forceModify)
{
	if (m_ValidEdgeCaptureTrigTIxn0 != ve || forceModify)
	{
		m_ValidEdgeCaptureTrigTIxn0 = ve;
		m_pReg->IOC1 &= ~0x03;
		switch(ve)
		{
			case vectTIPn0_noEdge:
				
				break;
			case vectTIPn0_risingEdge:
				m_pReg->IOC1 |= 0x01;
				break;
			case vectTIPn0_fallingEdge:
				m_pReg->IOC1 |= 0x02;
				break;
			case vectTIPn0_bothEdges:
				m_pReg->IOC1 |= 0x03;
				break;
		}
			
		return true;
	}
	return false;
}

bool Timer_NEC::setValidEdgeExtCountTIxn1(ValidEdgeExtCountTIxn1 ve, bool forceModify)
{
	if (m_ValidEdgeExtCountTIxn1 != ve || forceModify)
	{
		m_ValidEdgeExtCountTIxn1 = ve;
		m_pReg->IOC2 &= ~0x0C;
		switch(ve)
		{
			case vectTIxn1_noEdge:
				
				break;
			case veecTIxn1_risingEdge:
				m_pReg->IOC2 |= 0x04;
				break;
			case veecTIxn1_fallingEdge:
				m_pReg->IOC2 |= 0x08;
				break;
			case veecTIxn1_bothEdges:
				m_pReg->IOC2 |= 0x0C;
				break;
		}
			
		return true;
	}
	return false;
}

bool Timer_NEC::setValidEdgeExtCountTIxn0(ValidEdgeExtCountTIxn0 ve, bool forceModify)
{
	if (m_ValidEdgeExtCountTIxn0 != ve || forceModify)
	{
		m_ValidEdgeExtCountTIxn0 = ve;
		m_pReg->IOC2 &= ~0x03;
		switch(ve)
		{
			case vectTIPn0_noEdge:
				
				break;
			case veecTIxn0_risingEdge:
				m_pReg->IOC2 |= 0x01;
				break;
			case veecTIxn0_fallingEdge:
				m_pReg->IOC2 |= 0x02;
				break;
			case veecTIxn0_bothEdges:
				m_pReg->IOC2 |= 0x03;
				break;
		}
			
		return true;
	}
	return false;
}

bool Timer_NEC::setTxnCCR1_selection(TxnCCR1_selection sel, bool forceModify)
{
	if (m_TxnCCR1_selection != sel || forceModify)
	{
		m_TxnCCR1_selection = sel;
		m_pReg->OPT0 &= ~0x20;
		switch(sel)
		{
			case TxnCCR1_sel_compareRegister:
				break;
			case TxnCCR1_sel_captureRegister:
				m_pReg->OPT0 |= 0x20;
				break;
		}
			
		return true;
	}
	return false;
}

bool Timer_NEC::setTxnCCR0_selection(TxnCCR0_selection sel, bool forceModify)
{
	if (m_TxnCCR0_selection != sel || forceModify)
	{
		m_TxnCCR0_selection = sel;
		m_pReg->OPT0 &= ~0x10;
		switch(sel)
		{
			case TxnCCR0_sel_compareRegister:
				break;
			case TxnCCR0_sel_captureRegister:
				m_pReg->OPT0 |= 0x10;
				break;
		}
			
		return true;
	}
	return false;
}

word Timer_NEC::getCCR(CCRn ccrn)
{
	switch(ccrn)
	{
		case _CCR0:
			return getCCR0();
		case _CCR1:
			return getCCR1();
		case _CCR2:
			return getCCR2();
		case _CCR3:
			return getCCR3();
	}
}

void Timer_NEC::setCCR(CCRn ccrn, word cc)
{
	switch(ccrn)
	{
		case _CCR0:
			setCCR0(cc);
			break;
		case _CCR1:
			setCCR1(cc);
			break;
		case _CCR2:
			setCCR2(cc);
			break;
		case _CCR3:
			setCCR3(cc);
			break;
	}
}




Timer_NEC_P::Timer_NEC_P(volatile NEC_timerP_t *_pReg, volatile NEC_timerP_Int_t *_pIntReg): Timer_NEC((volatile NEC_timer_Generic_t *)_pReg)
{
	m_pReg = NULL;
	m_pIntReg = NULL;
	
	if((_pReg != NULL) && (_pIntReg != NULL))
	{
		m_pReg = _pReg;
		m_pIntReg = _pIntReg;

		setBaseFrequency(20e+6);
	}
}

void Timer_NEC_P::setPeriod(double T, CCRn ccrn)
{
	Timer_NEC_P::setPeriod(T, ccrn);
	setFrequency(getFrequency(), ccrn);
}

bool Timer_NEC_P::setCCR0(word ccr, bool forceModify, bool recalulateFrq)
{
	if(ccr != m_pReg->CCR0 || forceModify)
	{
		m_pReg->CCR0 = ccr;
		return true;
	}
	return false;
}

bool Timer_NEC_P::setCCR1(word ccr, bool forceModify, bool recalulateFrq)
{
	if(ccr != m_pReg->CCR1 || forceModify)
	{
		m_pReg->CCR1 = ccr;
		return true;
	}
	return false;
}


void Timer_NEC_P::setOVIC_interrupt(bool en, byte priority)
{
	if (priority > 0x07)
		priority = 0x07;
	if(en)
	{
		m_pIntReg->OVIC |= priority;
		m_pIntReg->OVIC &= ~0x40;
		
	}else
	{
		m_pIntReg->OVIC |= 0x40;
	}
}

void Timer_NEC_P::setCCIC0_interrupt(bool en, byte priority)
{
	if (priority > 0x07)
		priority = 0x07;

	if(en)
	{
		m_pIntReg->CCIC0 |= priority;
		m_pIntReg->CCIC0 &= ~0x40;
		
	}else
	{
		m_pIntReg->CCIC0 |= 0x40;
	}
}

void Timer_NEC_P::setCCIC1_interrupt(bool en, byte priority)
{
	if (priority > 0x07)
		priority = 0x07;

	if(en)
	{
		m_pIntReg->CCIC1 |= priority;
		m_pIntReg->CCIC1 &= ~0x40;
		
	}else
	{
		m_pIntReg->CCIC1 |= 0x40;
	}
}



Timer_NEC_P024::Timer_NEC_P024(volatile NEC_timerP_t *_pReg, volatile NEC_timerP_Int_t *_pIntReg)
	: Timer_NEC_P(_pReg, _pIntReg)
{
	setBaseFrequency(20e+6);
}

void Timer_NEC_P024::setEnable(bool en)
{
	Timer_NEC_P::setEnable(en);
	if(en)
		m_pReg->CTL0 |= 0x80;
	else
		m_pReg->CTL0 &= ~0x80;
}

void Timer_NEC_P024::setFrequency(double frq, CCRn ccrn)
{
	Timer_NEC_P::setFrequency(frq, ccrn);
	set_clk(ccrn);
}

double Timer_NEC_P024::calculateFrequency( CCRn ccrn)
{
	double frq = (getBaseFrequency() / (prescalerMultiplyFactor(getPrescaler()) * (getCCR(ccrn) + 1)));
	Timer_NEC::setFrequency(frq, ccrn);
	return frq;
}

bool Timer_NEC_P024::setPrescaler(Prescalers p, bool forceModify)
{	
	if(m_prescaler != p || forceModify)
	{
		m_prescaler = p;
		m_pReg->CTL0 &= ~0x7F;
		switch(p)
		{
			case fxx_1:
				break;
			case fxx_2:
				m_pReg->CTL0 |= 0x01;
				break;
			case fxx_4:
				m_pReg->CTL0 |= 0x02;
				break;
			case fxx_8:
				m_pReg->CTL0 |= 0x03;
				break;
			case fxx_16:
				m_pReg->CTL0 |= 0x04;
				break;
			case fxx_32:
				m_pReg->CTL0 |= 0x05;
				break;
			case fxx_64:
				m_pReg->CTL0 |= 0x06;
				break;
			case fxx_128:
				m_pReg->CTL0 |= 0x07;
				break;
		}
	}
}

bool Timer_NEC_P024::setCCR0(word ccr, bool forceModify, bool recalulateFrq)
{
	bool retVal = Timer_NEC_P::setCCR0(ccr, forceModify, recalulateFrq);
	
	if (recalulateFrq)
	{
		double frq;
		frq = calculateFrequency(_CCR0);
		Timer_NEC::setFrequency(frq, _CCR0);
	}
	
	return retVal;
}

bool Timer_NEC_P024::setCCR1(word ccr, bool forceModify, bool recalulateFrq)
{
	bool retVal = Timer_NEC_P::setCCR1(ccr, forceModify, recalulateFrq);
	
	if (recalulateFrq)
	{
		double frq;
		frq = calculateFrequency(_CCR1);
		Timer_NEC::setFrequency(frq, _CCR1);
	}
	
	return retVal;
}

int Timer_NEC_P024::set_clk(CCRn ccrn)
{
	int i;
	unsigned long s;
	word prs[max_prescalers];
	word cnt[max_prescalers];
	int k;
	int num_prs_cnt_to_check ;
	unsigned long product_prs_cnt[max_prescalers];
	unsigned long  max_product;
	int max_k;
	bool enable;

	

	for(k = 0; k < max_prescalers; k++)
	{
		product_prs_cnt[k] = 0;
		cnt[k] = 0;
		prs[k] = 0;
	}
	
	i = fxx_1;
	k = 0;
	num_prs_cnt_to_check = 0;
	
	while (i < max_prescalers)
	{
		frqEdges((Prescalers)i);
		if ( (getFrequency() <= m_maxFrq) && (getFrequency() >= m_minFrq))
		{			
			s = (long)( m_maxFrq / getFrequency());
			cnt[k] = s;
			prs[k] = i;
			product_prs_cnt[k] = cnt[k] * (unsigned long)prescalerMultiplyFactor((Prescalers)i);
			k++;
		}
		i++;
	}

	num_prs_cnt_to_check = k;
	if (num_prs_cnt_to_check == 0)
	{		
		setPrescaler(fxx_128);
		setCCR(ccrn, 0xFFFF);
		
		if (enable)
			setEnable(true);
		
		return -1;
	}
	
	max_product = product_prs_cnt[0];
	max_k = 0;
	for (k = 1 ; k < num_prs_cnt_to_check; k++)
	{
		if (product_prs_cnt[k] > max_product)
		{
			max_product = product_prs_cnt[k];
			max_k = k;
		}
	}
	s = cnt[max_k];

	enable = isEnable();
	setEnable(false);
	
	setPrescaler((Prescalers) prs[max_k]);
	
	s--;
	setCCR(ccrn, (word)s);
	
	if (enable)
		setEnable(true);
	
	return 1;
}



int Timer_NEC_P024::frqEdges(Prescalers p)
{
	if (p >= max_prescalers)
	{
		m_prescaler = max_prescalers;
		return -1;
	}
	
	m_maxFrq = (getBaseFrequency() / (double)prescalerMultiplyFactor(p));
	m_minFrq = m_maxFrq / 65536.0;
	
	return 0;
}

int Timer_NEC_P024::prescalerMultiplyFactor(Prescalers p)
{
	switch (p)
	{
		case fxx_1:
			return 1; 
			break;
		case fxx_2:
			return 2; 
			break;
		case fxx_4:
			return 4; 
			break;
		case fxx_8:
			return 8;
			break;
		case fxx_16:
			return 16;
			break;
		case fxx_32:
			return 32;
			break;
		case fxx_64:
			return 64;
			break;
		case fxx_128:
			return 128;
			break;
	}
	return 1;
}





Timer_NEC_P135::Timer_NEC_P135(volatile NEC_timerP_t *_pReg, volatile NEC_timerP_Int_t *_pIntReg)
	: Timer_NEC_P(_pReg, _pIntReg)
{
	setBaseFrequency(20e+6);
}

void Timer_NEC_P135::setEnable(bool en)
{
	Timer_NEC_P::setEnable(en);
	if(en)
		m_pReg->CTL0 |= 0x80;
	else
		m_pReg->CTL0 &= ~0x80;
}

void Timer_NEC_P135::setFrequency(double frq, CCRn ccrn)
{
	Timer_NEC_P::setFrequency(frq, ccrn);
	set_clk(ccrn);
}

double Timer_NEC_P135::calculateFrequency( CCRn ccrn)
{
	double frq = (getBaseFrequency() / (prescalerMultiplyFactor(getPrescaler()) * (getCCR(ccrn) + 1)));
	Timer_NEC::setFrequency(frq, ccrn);
	return frq;
}

bool Timer_NEC_P135::setPrescaler(Prescalers p, bool forceModify)
{	
	if(m_prescaler != p || forceModify)
	{
		m_prescaler = p;
		m_pReg->CTL0 &= ~0x7F;
		switch(p)
		{
			case fxx_1:
				break;
			case fxx_2:
				m_pReg->CTL0 |= 0x01;
				break;
			case fxx_4:
				m_pReg->CTL0 |= 0x02;
				break;
			case fxx_8:
				m_pReg->CTL0 |= 0x03;
				break;
			case fxx_16:
				m_pReg->CTL0 |= 0x04;
				break;
			case fxx_32:
				m_pReg->CTL0 |= 0x05;
				break;
			case fxx_256:
				m_pReg->CTL0 |= 0x06;
				break;
			case fxx_512:
				m_pReg->CTL0 |= 0x07;
				break;
		}
	}
}

bool Timer_NEC_P135::setCCR0(word ccr, bool forceModify, bool recalulateFrq)
{
	bool retVal = Timer_NEC_P::setCCR0(ccr, forceModify, recalulateFrq);
	
	if (recalulateFrq)
	{
		double frq;
		frq = calculateFrequency(_CCR0);
		Timer_NEC::setFrequency(frq, _CCR0);
	}
	
	return retVal;
}

bool Timer_NEC_P135::setCCR1(word ccr, bool forceModify, bool recalulateFrq)
{
	bool retVal = Timer_NEC_P::setCCR1(ccr, forceModify, recalulateFrq);
	
	if (recalulateFrq)
	{
		double frq;
		frq = calculateFrequency(_CCR1);
		Timer_NEC::setFrequency(frq, _CCR1);
	}
	
	return retVal;
}

int Timer_NEC_P135::set_clk(CCRn ccrn)
{
	int i;
	unsigned long s;
	word prs[max_prescalers];
	word cnt[max_prescalers];
	int k;
	int num_prs_cnt_to_check ;
	unsigned long product_prs_cnt[max_prescalers];
	unsigned long  max_product;
	int max_k;
	bool enable;

	for(k = 0; k < max_prescalers; k++)
	{
		product_prs_cnt[k] = 0;
		cnt[k] = 0;
		prs[k] = 0;
	}
	
	i = fxx_1;
	k = 0;
	num_prs_cnt_to_check = 0;
	
	while (i < max_prescalers)
	{
		frqEdges((Prescalers)i);
		if ( (getFrequency() <= m_maxFrq) && (getFrequency() >= m_minFrq))
		{			
			s = (long)( m_maxFrq / getFrequency());
			cnt[k] = s;
			prs[k] = i;
			product_prs_cnt[k] = cnt[k] * (unsigned long)prescalerMultiplyFactor((Prescalers)i);
			k++;
		}
		i++;
	}

	num_prs_cnt_to_check = k;
	if (num_prs_cnt_to_check == 0)
	{		
		setPrescaler(fxx_512);
		setCCR(ccrn, 0xFFFF);
		
		if (enable)
			setEnable(true);
		
		return -1;
	}
	
	max_product = product_prs_cnt[0];
	max_k = 0;
	for (k = 1 ; k < num_prs_cnt_to_check; k++)
	{
		if (product_prs_cnt[k] > max_product)
		{
			max_product = product_prs_cnt[k];
			max_k = k;
		}
	}
	s = cnt[max_k];

	enable = isEnable();
	setEnable(false);
	
	setPrescaler((Prescalers) prs[max_k]);
	
	s--;
	setCCR(ccrn, (word)s);
	
	if (enable)
		setEnable(true);
	
	return 1;
}



int Timer_NEC_P135::frqEdges(Prescalers p)
{
	if (p >= max_prescalers)
	{
		m_prescaler = max_prescalers;
		return -1;
	}
	
	m_maxFrq = (getBaseFrequency() / (double)prescalerMultiplyFactor(p));
	m_minFrq = m_maxFrq / 65536.0;
	
	return 0;
}

int Timer_NEC_P135::prescalerMultiplyFactor(Prescalers p)
{
	switch (p)
	{
		case fxx_1:
			return 1; 
			break;
		case fxx_2:
			return 2; 
			break;
		case fxx_4:
			return 4; 
			break;
		case fxx_8:
			return 8;
			break;
		case fxx_16:
			return 16;
			break;
		case fxx_32:
			return 32;
			break;
		case fxx_256:
			return 256;
			break;
		case fxx_512:
			return 128;
			break;
	}
	return 1;
}


Timer_NEC_Q::Timer_NEC_Q(volatile NEC_timerQ_t *_pReg, volatile NEC_timerQ_Int_t *_pIntReg): Timer_NEC((volatile NEC_timer_Generic_t *)_pReg)
{
	m_pReg = NULL;
	m_pIntReg = NULL;
	
	if((_pReg != NULL) && (_pIntReg != NULL))
	{
		m_pReg = _pReg;
		m_pIntReg = _pIntReg;

		setBaseFrequency(20e+6);
	}
}

void Timer_NEC_Q::setPeriod(double T, CCRn ccrn)
{
	Timer_NEC::setPeriod(T, ccrn);
	setFrequency(getFrequency(), ccrn);
}

void Timer_NEC_Q::setEnable(bool en)
{
	Timer_NEC::setEnable(en);
	if(en)
		m_pReg->CTL0 |= 0x80;
	else
		m_pReg->CTL0 &= ~0x80;
}

void Timer_NEC_Q::setFrequency(double frq, CCRn ccrn)
{
	Timer_NEC::setFrequency(frq, ccrn);
	set_clk(ccrn);
}

double Timer_NEC_Q::calculateFrequency( CCRn ccrn)
{
	double frq = (getBaseFrequency() / (prescalerMultiplyFactor(getPrescaler()) * (getCCR(ccrn) + 1)));
	Timer_NEC::setFrequency(frq, ccrn);
	return frq;
}

	
bool Timer_NEC_Q::setTxnOL2(TxnOL2 OL2, bool forceModify)
{
	if (m_TxnOL2 != OL2 || forceModify)
	{
		m_TxnOL2 = OL2;
		m_pReg->IOC0 &= ~0x20;
		switch(OL2)
		{
			case TOPn2_startHigh:
				break;
			case TOPn2_startLow:
				m_pReg->IOC0 |= 0x20;
				break;
		}
			
		return true;
	}
	return false;
}

bool Timer_NEC_Q::setTxnOE2(TxnOE2 OE2, bool forceModify)
{
	if (m_TxnOE2 != OE2 || forceModify)
	{
		m_TxnOE2 = OE2;
		m_pReg->IOC0 &= ~0x01;
		switch(OE2)
		{
			case TxnOE1_outputDisable:
				break;
			case TxnOE1_outputEnable:
				m_pReg->IOC0 |= 0x01;
				break;
		}
			
		return true;
	}
	return false;
}

bool Timer_NEC_Q::setTxnOL3(TxnOL3 OL3, bool forceModify)
{
	if (m_TxnOL3 != OL3 || forceModify)
	{
		m_TxnOL3 = OL3;
		m_pReg->IOC0 &= ~0x80;
		switch(OL3)
		{
			case TOPn3_startHigh:
				break;
			case TOPn3_startLow:
				m_pReg->IOC0 |= 0x80;
				break;
		}
			
		return true;
	}
	return false;
}

bool Timer_NEC_Q::setTxnOE3(TxnOE3 OE3, bool forceModify)
{
	if (m_TxnOE3 != OE3 || forceModify)
	{
		m_TxnOE3 = OE3;
		m_pReg->IOC0 &= ~0x40;
		switch(OE3)
		{
			case TxnOE3_outputDisable:
				break;
			case TxnOE3_outputEnable:
				m_pReg->IOC0 |= 0x40;
				break;
		}
			
		return true;
	}
	return false;
}

bool Timer_NEC_Q::setValidEdgeCaptureTrigTIxn2(ValidEdgeCaptureTrigTIxn2 ve, bool forceModify)
{
	if (m_ValidEdgeCaptureTrigTIxn2 != ve || forceModify)
	{
		m_ValidEdgeCaptureTrigTIxn2 = ve;
		m_pReg->IOC1 &= ~0x30;
		switch(ve)
		{
			case vectTIxn1_noEdge:
				
				break;
			case vectTIxn1_risingEdge:
				m_pReg->IOC1 |= 0x10;
				break;
			case vectTIxn1_fallingEdge:
				m_pReg->IOC1 |= 0x20;
				break;
			case vectTIxn1_bothEdges:
				m_pReg->IOC1 |= 0x30;
				break;
		}
			
		return true;
	}
	return false;
}

bool Timer_NEC_Q::setValidEdgeCaptureTrigTIxn3(ValidEdgeCaptureTrigTIxn3 ve, bool forceModify)
{
	if (m_ValidEdgeCaptureTrigTIxn3 != ve || forceModify)
	{
		m_ValidEdgeCaptureTrigTIxn3 = ve;
		m_pReg->IOC1 &= ~0xC0;
		switch(ve)
		{
			case vectTIxn1_noEdge:
				
				break;
			case vectTIxn1_risingEdge:
				m_pReg->IOC1 |= 0x40;
				break;
			case vectTIxn1_fallingEdge:
				m_pReg->IOC1 |= 0x80;
				break;
			case vectTIxn1_bothEdges:
				m_pReg->IOC1 |= 0xC0;
				break;
		}
			
		return true;
	}
	return false;
}

bool Timer_NEC_Q::setTxnCCR2_selection(TxnCCR2_selection sel, bool forceModify)
{
	if (m_TxnCCR2_selection != sel || forceModify)
	{
		m_TxnCCR2_selection = sel;
		m_pReg->OPT0 &= ~0x40;
		switch(sel)
		{
			case TxnCCR2_sel_compareRegister:
				break;
			case TxnCCR2_sel_captureRegister:
				m_pReg->OPT0 |= 0x40;
				break;
		}
			
		return true;
	}
	return false;
}

bool Timer_NEC_Q::setTxnCCR3_selection(TxnCCR3_selection sel, bool forceModify)
{
	if (m_TxnCCR3_selection != sel || forceModify)
	{
		m_TxnCCR3_selection = sel;
		m_pReg->OPT0 &= ~0x80;
		switch(sel)
		{
			case TxnCCR3_sel_compareRegister:
				break;
			case TxnCCR3_sel_captureRegister:
				m_pReg->OPT0 |= 0x80;
				break;
		}
			
		return true;
	}
	return false;
}

bool Timer_NEC_Q::setPrescaler(Prescalers p, bool forceModify, bool recalulateFrq)
{	
	if(m_prescaler != p || forceModify)
	{
		m_prescaler = p;
		m_pReg->CTL0 &= ~0x7F;
		switch(p)
		{
			case fxx_1:
				break;
			case fxx_2:
				m_pReg->CTL0 |= 0x01;
				break;
			case fxx_4:
				m_pReg->CTL0 |= 0x02;
				break;
			case fxx_8:
				m_pReg->CTL0 |= 0x03;
				break;
			case fxx_16:
				m_pReg->CTL0 |= 0x04;
				break;
			case fxx_32:
				m_pReg->CTL0 |= 0x05;
				break;
			case fxx_64:
				m_pReg->CTL0 |= 0x06;
				break;
			case fxx_128:
				m_pReg->CTL0 |= 0x07;
				break;
		}
	}
	if (recalulateFrq)
	{
		double frq;
		frq = getBaseFrequency() /(prescalerMultiplyFactor(m_prescaler) * (m_pReg->CCR0 + 1));
		TimerAbstract::setFrequency(frq);
	}
}

bool Timer_NEC_Q::setCCR0(word ccr, bool forceModify, bool recalulateFrq)
{
	bool retVal = false;
	
	if(ccr != m_pReg->CCR0 || forceModify)
	{
		m_pReg->CCR0 = ccr;
		retVal =  true;
	}
	
	if (recalulateFrq)
	{
		double frq;
		frq = calculateFrequency(_CCR0);
		Timer_NEC::setFrequency(frq, _CCR0);
	}
	
	return retVal;
}

bool Timer_NEC_Q::setCCR1(word ccr, bool forceModify, bool recalulateFrq)
{
	bool retVal = false;
	
	if(ccr != m_pReg->CCR1 || forceModify)
	{
		m_pReg->CCR1 = ccr;
		retVal =  true;
	}
	
	if (recalulateFrq)
	{
		double frq;
		frq = calculateFrequency(_CCR1);
		Timer_NEC::setFrequency(frq,_CCR1);
	}
	
	return retVal;
}

bool Timer_NEC_Q::setCCR2(word ccr, bool forceModify, bool recalulateFrq)
{
	bool retVal = false;
	
	if(ccr != m_pReg->CCR2 || forceModify)
	{
		m_pReg->CCR2 = ccr;
		retVal =  true;
	}
	
	if (recalulateFrq)
	{
		double frq;
		frq = calculateFrequency(_CCR2);
		Timer_NEC::setFrequency(frq, _CCR2);
	}
	
	return retVal;
}

bool Timer_NEC_Q::setCCR3(word ccr, bool forceModify, bool recalulateFrq)
{
	bool retVal = false;
	
	if(ccr != m_pReg->CCR3 || forceModify)
	{
		m_pReg->CCR3 = ccr;
		retVal =  true;
	}
	
	if (recalulateFrq)
	{
		double frq;
		frq = calculateFrequency(_CCR3);
		Timer_NEC::setFrequency(frq, _CCR3);
	}
	
	return retVal;
}

void Timer_NEC_Q::setOVIC_interrupt(bool en, byte priority)
{
	if (priority > 0x07)
		priority = 0x07;
	if(en)
	{
		m_pIntReg->OVIC |= priority;
		m_pIntReg->OVIC &= ~0x40;
		
	}else
	{
		m_pIntReg->OVIC |= 0x40;
	}
}

void Timer_NEC_Q::setCCIC0_interrupt(bool en, byte priority)
{
	if (priority > 0x07)
		priority = 0x07;

	if(en)
	{
		m_pIntReg->CCIC0 |= priority;
		m_pIntReg->CCIC0 &= ~0x40;
		
	}else
	{
		m_pIntReg->CCIC0 |= 0x40;
	}
}

void Timer_NEC_Q::setCCIC1_interrupt(bool en, byte priority)
{
	if (priority > 0x07)
		priority = 0x07;

	if(en)
	{
		m_pIntReg->CCIC1 |= priority;
		m_pIntReg->CCIC1 &= ~0x40;
		
	}else
	{
		m_pIntReg->CCIC1 |= 0x40;
	}
}

void Timer_NEC_Q::setCCIC2_interrupt(bool en, byte priority)
{
	if (priority > 0x07)
		priority = 0x07;

	if(en)
	{
		m_pIntReg->CCIC2 |= priority;
		m_pIntReg->CCIC2 &= ~0x40;
		
	}else
	{
		m_pIntReg->CCIC2 |= 0x40;
	}
}

void Timer_NEC_Q::setCCIC3_interrupt(bool en, byte priority)
{
	if (priority > 0x07)
		priority = 0x07;

	if(en)
	{
		m_pIntReg->CCIC3 |= priority;
		m_pIntReg->CCIC3 &= ~0x40;
		
	}else
	{
		m_pIntReg->CCIC3 |= 0x40;
	}
}


int Timer_NEC_Q::set_clk(CCRn ccrn)
{
	int i;
	unsigned long s;
	word prs[max_prescalers];
	word cnt[max_prescalers];
	int k;
	int num_prs_cnt_to_check ;
	unsigned long product_prs_cnt[max_prescalers];
	unsigned long  max_product;
	int max_k;
	bool enable;

	for(k = 0; k < max_prescalers; k++)
	{
		product_prs_cnt[k] = 0;
		cnt[k] = 0;
		prs[k] = 0;
	}
	
	i = fxx_1;
	k = 0;
	num_prs_cnt_to_check = 0;
	
	while (i < max_prescalers)
	{
		if(getTimerMode() != tm_pulseWidthMeasurementMode)
		{
			frqEdges((Prescalers)i);
			if ( (getFrequency() <= m_maxFrq) && (getFrequency() >= m_minFrq))
			{			
				s = (long)( m_maxFrq / getFrequency());
				cnt[k] = s;
				prs[k] = i;
				product_prs_cnt[k] = cnt[k] * (unsigned long)prescalerMultiplyFactor((Prescalers)i);
				k++;
			}
		}else
		{
			m_maxFrq = (getBaseFrequency() / (double)prescalerMultiplyFactor((Prescalers)i));
			if ( (getFrequency() <= m_maxFrq))
			{			
				s = 1;//(long)( m_maxFrq / getFrequency());
				cnt[k] = s;
				prs[k] = i;
				product_prs_cnt[k] = cnt[k] *( prescalerMultiplyFactor( (Prescalers)((int)max_prescalers - 1)) / (unsigned long)prescalerMultiplyFactor((Prescalers)i));
				//product_prs_cnt[k] = cnt[k] *( (unsigned long)prescalerMultiplyFactor((Prescalers)i));
				k++;
			}	
		}
		i++;
	}

	num_prs_cnt_to_check = k;
	if (num_prs_cnt_to_check == 0)
	{		
		setPrescaler(fxx_128);
		setCCR(ccrn, 0xFFFF);
		
		if (enable)
			setEnable(true);
		
		return -1;
	}
	
	max_product = product_prs_cnt[0];
	max_k = 0;
	for (k = 1 ; k < num_prs_cnt_to_check; k++)
	{
		if (product_prs_cnt[k] > max_product)
		{
			max_product = product_prs_cnt[k];
			max_k = k;
		}
	}
	s = cnt[max_k];

	enable = isEnable();
	setEnable(false);
	
	setPrescaler((Prescalers) prs[max_k]);
	
	s--;
	setCCR(ccrn, (word)s);
	
	if (enable)
		setEnable(true);
	
	return 1;
}



int Timer_NEC_Q::frqEdges(Prescalers p)
{
	if (p >= max_prescalers)
	{
		m_prescaler = max_prescalers;
		return -1;
	}
	
	m_maxFrq = (getBaseFrequency() / (double)prescalerMultiplyFactor(p));
	m_minFrq = m_maxFrq / 65536.0;
	
	return 0;
}

int Timer_NEC_Q::prescalerMultiplyFactor(Prescalers p)
{
	switch (p)
	{
		case fxx_1:
			return 1; 
			break;
		case fxx_2:
			return 2; 
			break;
		case fxx_4:
			return 4; 
			break;
		case fxx_8:
			return 8;
			break;
		case fxx_16:
			return 16;
			break;
		case fxx_32:
			return 32;
			break;
		case fxx_64:
			return 64;
			break;
		case fxx_128:
			return 128;
			break;
	}
	return 1;
}









