/**
@file		Timer_NEC.h
@brief		timer classes

@author		Nicola Molinazzi
@date		10/10/2011
@version	01.0

*/


#ifndef __TIMER_NEC_H__
#define __TIMER_NEC_H__


#include "TimerAbstract.h"

struct NEC_timer_Generic_t
{
	byte CTL0;
	byte CTL1;
	byte IOC0;
	byte IOC1;
	byte IOC2;
	byte OPT0;
};

struct NEC_timerP_t
{
	byte CTL0;
	byte CTL1;
	byte IOC0;
	byte IOC1;
	byte IOC2;
	byte OPT0;
	word CCR0;
	word CCR1;
	word CNT;
};

struct NEC_timerP_Int_t
{
	byte OVIC;
	byte FREE_00;
	byte CCIC0;
	byte FREE_01;
	byte CCIC1;
};

struct NEC_timerQ_t
{
	byte CTL0;
	byte CTL1;
	byte IOC0;
	byte IOC1;
	byte IOC2;
	byte OPT0;
	word CCR0;
	word CCR1;
	word CCR2;
	word CCR3;
	word CNT;
};

struct NEC_timerQ_Int_t
{
	byte OVIC;
	byte FREE_00;
	byte CCIC0;
	byte FREE_01;
	byte CCIC1;
	byte FREE_02;
	byte CCIC2;
	byte FREE_03;
	byte CCIC3;
};


class Timer_NEC: public TimerAbstract
{
	public:
		enum TxnEST
		{
			TxnEST_noSwTrigCntr =0,
			TxnEST_swTrigCntr,
		};
		
		enum TxnEEE
		{
			TxnEEE_noExtClk = 0,
			TxnEEE_extClk,
		};

		enum TimerMode
		{
			tm_IntervalTimerMode = 0,
			tm_ExtEventCountMode,
			tm_extTrigPulseOutputMode,
			tm_OneShotPulseOutputMode,
			tm_pwmOutputMode,
			tm_FreeRunningTimerMode,
			tm_pulseWidthMeasurementMode,
		};

		enum TxnOL3
		{
			TOPn3_startHigh = 0,
			TOPn3_startLow,
		};

		enum TxnOE3
		{
			TxnOE3_outputDisable = 0,
			TxnOE3_outputEnable,
		};
		
		enum TxnOL2
		{
			TOPn2_startHigh = 0,
			TOPn2_startLow,
		};

		enum TxnOE2
		{
			TxnOE2_outputDisable = 0,
			TxnOE2_outputEnable,
		};
		
		enum TxnOL1
		{
			TOPn1_startHigh = 0,
			TOPn1_startLow,
		};

		enum TxnOE1
		{
			TxnOE1_outputDisable = 0,
			TxnOE1_outputEnable,
		};

		enum TxnOL0
		{
			TOPn0_startHigh = 0,
			TOPn0_startLow,
		};

		enum TxnOE0
		{
			TxnOE0_outputDisable = 0,
			TxnOE0_outputEnable,
		};

		enum ValidEdgeCaptureTrigTIxn3
		{
			vectTIxn3_noEdge = 0,
			vectTIxn3_risingEdge,
			vectTIxn3_fallingEdge,
			vectTIxn3_bothEdges,
		};
		
		enum ValidEdgeCaptureTrigTIxn2
		{
			vectTIxn2_noEdge = 0,
			vectTIxn2_risingEdge,
			vectTIxn2_fallingEdge,
			vectTIxn2_bothEdges,
		};
		
		enum ValidEdgeCaptureTrigTIxn1
		{
			vectTIxn1_noEdge = 0,
			vectTIxn1_risingEdge,
			vectTIxn1_fallingEdge,
			vectTIxn1_bothEdges,
		};

		enum ValidEdgeCaptureTrigTIxn0
		{
			vectTIPn0_noEdge = 0,
			vectTIPn0_risingEdge,
			vectTIPn0_fallingEdge,
			vectTIPn0_bothEdges,
		};

		enum ValidEdgeExtCountTIxn1
		{
			veecTIxn1_noEdge = 0,
			veecTIxn1_risingEdge,
			veecTIxn1_fallingEdge,
			veecTIxn1_bothEdges,
		};

		enum ValidEdgeExtCountTIxn0
		{
			veecTIxn0_noEdge = 0,
			veecTIxn0_risingEdge,
			veecTIxn0_fallingEdge,
			veecTIxn0_bothEdges,
		};

		enum TxnCCR3_selection
		{
			TxnCCR3_sel_compareRegister = 0,
			TxnCCR3_sel_captureRegister,	
		};

		enum TxnCCR2_selection
		{
			TxnCCR2_sel_compareRegister = 0,
			TxnCCR2_sel_captureRegister,	
		};
		
		enum TxnCCR1_selection
		{
			TxnCCR1_sel_compareRegister = 0,
			TxnCCR1_sel_captureRegister,	
		};

		enum TxnCCR0_selection
		{
			TxnCCR0_sel_compareRegister = 0,
			TxnCCR0_sel_captureRegister,	
		};

		enum CCRn
		{
			_CCR0 = 0,
			_CCR1,
			_CCR2,
			_CCR3,
		};
		
		Timer_NEC(volatile NEC_timer_Generic_t *_pReg);
		~Timer_NEC(){};

		virtual void setEnable(bool en){TimerAbstract::setEnable(en);}
		virtual bool isEnable(){return TimerAbstract::isEnable();}

		virtual void setBaseFrequency(double fxx){TimerAbstract::setBaseFrequency(fxx);} 
		virtual double getBaseFrequency(){return TimerAbstract::getBaseFrequency();}
		
		virtual void setFrequency(double frq, CCRn ccrn){TimerAbstract::setFrequency(frq);}
		virtual double getFrequency(){return TimerAbstract::getFrequency();}

		virtual double calculateFrequency( CCRn ccr){return 1;}
		
		virtual void setPeriod(double T, CCRn ccrn){TimerAbstract::setPeriod(T);}
		virtual double getPeriod(){return TimerAbstract::getPeriod();}
	
		virtual bool setCCR0(word ccr, bool forceModify = false, bool recalulateFrq = false){return false;};
		virtual word getCCR0(){return 0;}
		
		virtual bool setCCR1(word ccr, bool forceModify = false,  bool recalulateFrq = false){return false;}
		virtual word getCCR1(){return 0;}

		virtual bool setCCR2(word ccr, bool forceModify = false, bool recalulateFrq = false){return false;};
		virtual word getCCR2(){return 0;}
		
		virtual bool setCCR3(word ccr, bool forceModify = false,  bool recalulateFrq = false){return false;}
		virtual word getCCR3(){return 0;}

		virtual word getCCR(CCRn ccrn);
		virtual void setCCR(CCRn ccrn, word cc);
		
		virtual word getCNT(){return 0;}
		
		virtual void setOVIC_interrupt(bool en, byte priority){}
		virtual void setCCIC0_interrupt(bool en, byte priority){}
		virtual void setCCIC1_interrupt(bool en, byte priority){}

		virtual TxnEST getTxnEST(){return m_TxnEST;}
		virtual bool setTxnEST(TxnEST est, bool forceModify = false);
		
		virtual TxnEEE	getTxnEEE(){return m_TxnEEE;}
		virtual bool setTxnEEE(TxnEEE eee, bool forceModify = false);
		
		virtual TimerMode getTimerMode(){return m_timerMode;}
		virtual bool setTimerMode(TimerMode tm, bool forceModify = false);
		
		virtual TxnOL1 getTxnOL1(){return m_TxnOL1;}
		virtual bool setTxnOL1(TxnOL1 OL1, bool forceModify = false);
		
		virtual TxnOE1 getTxnOE1(){return m_TxnOE1;}
		virtual bool setTxnOE1(TxnOE1 OE1, bool forceModify = false);
		
		virtual TxnOL0 getTxnOL0(){return m_TxnOL0;}
		virtual bool setTxnOL0(TxnOL0 OL0, bool forceModify = false);
		
		virtual TxnOE0 getTxnOE0(){return m_TxnOE0;}
		virtual bool setTxnOE0(TxnOE0 OE0, bool forceModify = false);
		
		virtual ValidEdgeCaptureTrigTIxn1 getValidEdgeCaptureTrigTIxn1(){return m_ValidEdgeCaptureTrigTIxn1;}
		virtual bool setValidEdgeCaptureTrigTIxn1(ValidEdgeCaptureTrigTIxn1 ve, bool forceModify = false);
		
		virtual ValidEdgeCaptureTrigTIxn0 getValidEdgeCaptureTrigTIxn0(){return m_ValidEdgeCaptureTrigTIxn0;}
		bool setValidEdgeCaptureTrigTIxn0(ValidEdgeCaptureTrigTIxn0 ve, bool forceModify = false);
		
		ValidEdgeExtCountTIxn1 getValidEdgeExtCountTIxn1(){return m_ValidEdgeExtCountTIxn1;}
		virtual bool setValidEdgeExtCountTIxn1(ValidEdgeExtCountTIxn1 ve, bool forceModify = false);
		
		ValidEdgeExtCountTIxn0 getValidEdgeExtCountTIxn0(){return m_ValidEdgeExtCountTIxn0;}
		virtual bool setValidEdgeExtCountTIxn0(ValidEdgeExtCountTIxn0 ve, bool forceModify = false);
		
		TxnCCR1_selection getTxnCCR1_selection(){return m_TxnCCR1_selection;}
		virtual bool setTxnCCR1_selection(TxnCCR1_selection sel, bool forceModify = false);
		
		TxnCCR0_selection getTxnCCR0_selection(){return m_TxnCCR0_selection;}
		virtual bool setTxnCCR0_selection(TxnCCR0_selection sel, bool forceModify = false);
		
	protected:
		TxnEST m_TxnEST;
		TxnEEE	m_TxnEEE;
		TimerMode m_timerMode;
		TxnOL1 m_TxnOL1;
		TxnOE1 m_TxnOE1;
		TxnOL0 m_TxnOL0;
		TxnOE0 m_TxnOE0;
		ValidEdgeCaptureTrigTIxn1 m_ValidEdgeCaptureTrigTIxn1;
		ValidEdgeCaptureTrigTIxn0 m_ValidEdgeCaptureTrigTIxn0;
		ValidEdgeExtCountTIxn1 m_ValidEdgeExtCountTIxn1;
		ValidEdgeExtCountTIxn0 m_ValidEdgeExtCountTIxn0;
		TxnCCR1_selection m_TxnCCR1_selection;
		TxnCCR0_selection m_TxnCCR0_selection;
		
	private:
		volatile NEC_timer_Generic_t * m_pReg;
};

class Timer_NEC_P: public Timer_NEC
{
	public:
		Timer_NEC_P(volatile NEC_timerP_t *_pReg, volatile NEC_timerP_Int_t *_pIntReg);
		~Timer_NEC_P(){};

		virtual void setEnable(bool en){Timer_NEC::setEnable(en);}
		
		virtual void setFrequency(double frq, CCRn ccrn){Timer_NEC::setFrequency(frq, ccrn);}
		virtual void setPeriod(double T, CCRn ccrn);

		virtual double calculateFrequency( CCRn ccrn){return Timer_NEC::calculateFrequency(ccrn);}
		
		virtual bool setCCR0(word ccr, bool forceModify = false, bool recalulateFrq = false);
		virtual word getCCR0(){return m_pReg->CCR0;}

		virtual bool setCCR1(word ccr, bool forceModify = false, bool recalulateFrq = false);
		virtual word getCCR1(){return m_pReg->CCR1;}

		virtual word getCNT(){return m_pReg->CNT;}

		virtual void setOVIC_interrupt(bool en, byte priority);
		virtual void setCCIC0_interrupt(bool en, byte priority);
		virtual void setCCIC1_interrupt(bool en, byte priority);

		inline bool OVIC_interruptFlag(){return ((m_pIntReg->OVIC & 0x80) == 0x80);}
		inline bool CCIC0_interruptFlag(){return ((m_pIntReg->CCIC0 & 0x80) == 0x80);}
		inline bool CCIC1_interruptFlag(){return ((m_pIntReg->CCIC1 & 0x80) == 0x80);}

		inline void rstOVIC_interruptFlag(){ m_pIntReg->OVIC &= ~0x80;}
		inline void rstCCIC0_interruptFlag(){ m_pIntReg->CCIC0 &= ~0x80;}
		inline void rstCCIC1_interruptFlag(){ m_pIntReg->CCIC1 &= ~0x80;}
		
	protected:
		volatile NEC_timerP_t *m_pReg;
		volatile NEC_timerP_Int_t *m_pIntReg;

	private:
		
};

class Timer_NEC_P024: public Timer_NEC_P
{
public:
	
	enum Prescalers
	{	
		fxx_1 = 0,
		fxx_2,
		fxx_4,
		fxx_8,
		fxx_16,
		fxx_32,
		fxx_64,
		fxx_128,
		max_prescalers,
	};

	Timer_NEC_P024(volatile NEC_timerP_t *_pReg, volatile NEC_timerP_Int_t *_pIntReg);
	~Timer_NEC_P024(){};

	virtual void setEnable(bool en);
	
	virtual void setFrequency(double frq, CCRn ccrn);

	virtual double calculateFrequency( CCRn ccrn);
	
	virtual bool setPrescaler(Prescalers p, bool forceModif = false);
	virtual Prescalers getPrescaler(){return m_prescaler;}

	virtual bool setCCR0(word ccr, bool forceModify = false, bool recalulateFrq = false);
	virtual bool setCCR1(word ccr, bool forceModify = false, bool recalulateFrq = false);
		
protected:
	inline int frqEdges(Prescalers p);
	inline int prescalerMultiplyFactor(Prescalers p);
	inline int set_clk(CCRn ccrn);
	
private:
	Prescalers m_prescaler;

	double m_maxFrq;
	double m_minFrq;
	
};


class Timer_NEC_P135: public Timer_NEC_P
{
public:
	
	enum Prescalers
	{	
		fxx_1 = 0,
		fxx_2,
		fxx_4,
		fxx_8,
		fxx_16,
		fxx_32,
		fxx_256,
		fxx_512,
		max_prescalers,
	};

	Timer_NEC_P135(volatile NEC_timerP_t *_pReg, volatile NEC_timerP_Int_t *_pIntReg);
	~Timer_NEC_P135(){};

	virtual void setEnable(bool en);
	
	virtual void setFrequency(double frq, CCRn ccrn);

	virtual double calculateFrequency( CCRn ccrn);
		
	virtual bool setPrescaler(Prescalers p, bool forceModif = false);
	virtual Prescalers getPrescaler(){return m_prescaler;}

	virtual bool setCCR0(word ccr, bool forceModify = false, bool recalulateFrq = false);
	virtual bool setCCR1(word ccr, bool forceModify = false, bool recalulateFrq = false);
	
protected:
	inline int frqEdges(Prescalers p);
	inline int prescalerMultiplyFactor(Prescalers p);
	inline int set_clk(CCRn ccrn);
	
private:
	Prescalers m_prescaler;

	double m_maxFrq;
	double m_minFrq;
	
};


class Timer_NEC_Q: public Timer_NEC
{
public:
	enum Prescalers
	{	
		fxx_1 = 0,
		fxx_2,
		fxx_4,
		fxx_8,
		fxx_16,
		fxx_32,
		fxx_64,
		fxx_128,
		max_prescalers,
	};
	
	Timer_NEC_Q(volatile NEC_timerQ_t *_pReg, volatile NEC_timerQ_Int_t *_pIntReg);
	~Timer_NEC_Q(){};

	virtual void setEnable(bool en);
	
	virtual void setFrequency(double frq, CCRn ccrn);
	virtual void setPeriod(double T, CCRn ccrn);

	virtual double calculateFrequency( CCRn ccrn);
	
	virtual bool setPrescaler(Prescalers p, bool forceModif = false, bool recalulateFrq = false);
	virtual Prescalers getPrescaler(){return m_prescaler;}

	virtual bool setCCR0(word ccr, bool forceModify = false, bool recalulateFrq = false);
	virtual word getCCR0(){return m_pReg->CCR0;}

	virtual bool setCCR1(word ccr, bool forceModify = false, bool recalulateFrq = false);
	virtual word getCCR1(){return m_pReg->CCR1;}

	virtual bool setCCR2(word ccr, bool forceModify = false, bool recalulateFrq = false);
	virtual word getCCR2(){return m_pReg->CCR2;}

	virtual bool setCCR3(word ccr, bool forceModify = false, bool recalulateFrq = false);
	virtual word getCCR3(){return m_pReg->CCR3;}
	
	virtual word getCNT(){return m_pReg->CNT;}
		
	TxnOL2 getTxnOL2(){return m_TxnOL2;}
	bool setTxnOL2(TxnOL2 OL2, bool forceModify = false);
	
	TxnOE2 getTxnOE2(){return m_TxnOE2;}
	bool setTxnOE2(TxnOE2 OE2, bool forceModify = false);

	TxnOL3 getTxnOL3(){return m_TxnOL3;}
	bool setTxnOL3(TxnOL3 OL3, bool forceModify = false);
	
	TxnOE3 getTxnOE3(){return m_TxnOE3;}
	bool setTxnOE3(TxnOE3 OE3, bool forceModify = false);
		
	virtual void setOVIC_interrupt(bool en, byte priority);
	virtual void setCCIC0_interrupt(bool en, byte priority);
	virtual void setCCIC1_interrupt(bool en, byte priority);
	void setCCIC2_interrupt(bool en, byte priority);
	void setCCIC3_interrupt(bool en, byte priority);

	ValidEdgeCaptureTrigTIxn2 getValidEdgeCaptureTrigTIxn2(){return m_ValidEdgeCaptureTrigTIxn2;}
	bool setValidEdgeCaptureTrigTIxn2(ValidEdgeCaptureTrigTIxn2 ve, bool forceModify = false);

	ValidEdgeCaptureTrigTIxn3 getValidEdgeCaptureTrigTIxn3(){return m_ValidEdgeCaptureTrigTIxn3;}
	bool setValidEdgeCaptureTrigTIxn3(ValidEdgeCaptureTrigTIxn3 ve, bool forceModify = false);

	TxnCCR2_selection getTxnCCR2_selection(){return m_TxnCCR2_selection;}
	virtual bool setTxnCCR2_selection(TxnCCR2_selection sel, bool forceModify = false);
	
	TxnCCR3_selection getTxnCCR3_selection(){return m_TxnCCR3_selection;}
	virtual bool setTxnCCR3_selection(TxnCCR3_selection sel, bool forceModify = false);
		
	inline bool OVIC_interruptFlag(){return ((m_pIntReg->OVIC & 0x80) == 0x80);}
	inline bool CCIC0_interruptFlag(){return ((m_pIntReg->CCIC0 & 0x80) == 0x80);}
	inline bool CCIC1_interruptFlag(){return ((m_pIntReg->CCIC1 & 0x80) == 0x80);}
	inline bool CCIC2_interruptFlag(){return ((m_pIntReg->CCIC2 & 0x80) == 0x80);}
	inline bool CCIC3_interruptFlag(){return ((m_pIntReg->CCIC3 & 0x80) == 0x80);}

	inline void rstOVIC_interruptFlag(){ m_pIntReg->OVIC &= ~0x80;}
	inline void rstCCIC0_interruptFlag(){ m_pIntReg->CCIC0 &= ~0x80;}
	inline void rstCCIC1_interruptFlag(){ m_pIntReg->CCIC1 &= ~0x80;}
	inline void rstCCIC2_interruptFlag(){ m_pIntReg->CCIC2 &= ~0x80;}
	inline void rstCCIC3_interruptFlag(){ m_pIntReg->CCIC3 &= ~0x80;}
	
protected:
	volatile NEC_timerQ_t *m_pReg;
	volatile NEC_timerQ_Int_t *m_pIntReg;

	inline int frqEdges(Prescalers p);
	inline int prescalerMultiplyFactor(Prescalers p);
	inline int set_clk(CCRn ccrn);
	
private:
	TxnOL2 m_TxnOL2;
	TxnOE2 m_TxnOE2;

	TxnOL3 m_TxnOL3;
	TxnOE3 m_TxnOE3;
	
	ValidEdgeCaptureTrigTIxn2 m_ValidEdgeCaptureTrigTIxn2;
	ValidEdgeCaptureTrigTIxn3 m_ValidEdgeCaptureTrigTIxn3;

	TxnCCR2_selection m_TxnCCR2_selection;
	TxnCCR3_selection m_TxnCCR3_selection;
	
	Prescalers m_prescaler;

	double m_maxFrq;
	double m_minFrq;	
};
















#endif

