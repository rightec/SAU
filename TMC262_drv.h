/**
@file		TMC262_drv.h
@brief		Class to manage the stepper driver TM262

@author		Nicola Molinazzi
@date		28/09/2011
@version	01.00
*/


#ifndef __TMC262_DRV_H__
#define __TMC262_DRV_H__


#include "global.h"
#include "SPI_Abstract.h"
#include "graph.h"
#include "sound.h"

class TMC262_StepperDriver
{
public:

	//--- DRVCTRL ----
	enum Polarity
	{
		polarity_0,
		polarity_1,
	};

	enum MicroStepResolution
	{
		uSr_256 = 0,
		uSr_128,
		uSr_64,
		uSr_32,
		uSr_16,
		uSr_8,
		uSr_4,
		uSr_2,
		uSr_fullStep,
	};

	//--- CHOPCONF ---
	enum CHOPCONF_BlankTime
	{
		bt_16_clk = 0,
		bt_24_clk,
		bt_36_clk,
		bt_54_clk,
	};

	enum CHOPCONF_ChopperMode
	{
		cm_standartMode = 0,
		cm_ConstToff,
	};

	enum CHOPCONF_RandomToffTime
	{
		rndtf_fixed = 0,
		rndtf_random,
	};

	enum CHOPCONF_HysteresisDecInterval
	{
		hdec_16_clk = 0,
		hdec_32_clk,
		hdec_48_clk,
		hdec_64_clk,
	};

	enum CHOPCONF_HysteresisLowInterval
	{
		hlow_m3 = 0,
		hlow_m2,
		hlow_m1,
		hlow_0,
		hlow_1,
		hlow_2,
		hlow_3,
		hlow_4,
		hlow_5,
		hlow_6,
		hlow_7,
		hlow_8,
		hlow_9,
		hlow_10,
		hlow_11,
		hlow_12,
	};

	enum CHOPCONF_HysteresisStartValue
	{
		hsv_hysteresisLow_plush_1 = 0,
		hsv_hysteresisLow_plush_2,
		hsv_hysteresisLow_plush_3,
		hsv_hysteresisLow_plush_4,
		hsv_hysteresisLow_plush_5,
		hsv_hysteresisLow_plush_6,
		hsv_hysteresisLow_plush_7,
		hsv_hysteresisLow_plush_8,
	};

	enum CHOPCONF_CurrentComparator
	{
		ccmp_enable = 0,
		ccmp_disbale,
	};

	typedef byte CHOPCONF_SineWaveOffset;

	typedef byte CHOPCONF_FastDecayTimeSetting;

	enum CHOPCONF_DrvEnableAndToff
	{
		drv_disable = 0,
		drv_T0ff_01,
		drv_T0ff_02,
		drv_T0ff_03,
		drv_T0ff_04,
		drv_T0ff_05,
		drv_T0ff_06,
		drv_T0ff_07,
		drv_T0ff_08,
		drv_T0ff_09,
		drv_T0ff_10,
		drv_T0ff_11,
		drv_T0ff_12,
		drv_T0ff_13,
		drv_T0ff_14,
		drv_T0ff_15,
	};
	
	//---- SAMRTEN ---
	enum SMARTEN_MinimumCurrent
	{
		mc_half = 0,
		mc_quarter,	
	};

	enum SMARTEN_CurrentDownStepSpeed
	{
		cds_32 = 0,
		cds_8,
		cds_2,
		cds_1,
	} ;

	enum SMARTEN_StallGuardHysteresis
	{
		sgh_0 = 0,
		sgh_1,
		sgh_2,
		sgh_3,
		sgh_4,
		sgh_5,
		sgh_6,
		sgh_7,
		sgh_8,
		sgh_9,
		sgh_10,
		sgh_11,
		sgh_12,
		sgh_13,
		sgh_14,
		sgh_15,
	};

	enum SMARTEN_CurrentUpStepWidth
	{
		cusw_1 = 0,
		cusw_2,
		cusw_4,
		cusw_8,
	};

	enum SMARTEN_StallGuardMinimum
	{
		sgm_controlOff = 0,
		sgm_1,
		sgm_2,
		sgm_3,
		sgm_4,
		sgm_5,
		sgm_6,
		sgm_7,
		sgm_8,
		sgm_9,
		sgm_10,
		sgm_11,
		sgm_12,
		sgm_13,
		sgm_14,
		sgm_15,
	};

	//----- SGCSCONF ----
	enum SGCSCONF_StallGuardFilterEnable
	{
		sgfe_standardMode = 0,
		sgfe_filterMode,
	};
	
	typedef char SGCSCONF_StallGuardThreasholdValue;

	typedef byte SGCSCONF_CurrentScale;

	//----- DRVCONF ----
	enum DRVCONF_SlopeControlHighSide
	{
		sch_min = 0,
		sch_minPlusTc,
		sch_medPlusTc,
		sch_max,
	};

	enum DRVCONF_SlopeControlLowSide
	{
		scl_min = 1,
		scl_med,
		scl_max,
	};

	enum DRVCONF_ShortToGndProtection
	{
		stgndp_enable = 0,
		stgndp_disable,
	};

	enum DRVCONF_ShortToGndDetectionTimer
	{
		stgndt_3_2us = 0,
		stgndt_1_6us,
		stgndt_1_2us,
		stgndt_0_8us,
	};

	enum DRVCONF_StepDirInput
	{
		sdi_enableStepAndDirectionMode = 0,
		sdi_enableSpiMode,
	};

	enum DRVCONF_SenseResistorVoltage
	{
		srv_fullScaleIs_305mV = 0,
		srv_fullScaleIs_165mV,
	};

	enum DRVCONF_ValueToRead
	{
		rv_microstepPosition = 0,
		rv_stallGuardLevel,
		rv_stallGuardAndCurrentLevel,
	};

	//--- read Values
	enum readBackValue
	{
		rb_false = 0,
		rb_true,
		rb_unknown = 0xFFFF,
	};
	
	TMC262_StepperDriver(SPI_Abstract *__spi);
	~TMC262_StepperDriver(){};

	void manager();
	
	void setEnable(bool en);
	bool isEnable(){return m_enable;}
	
	//--- DRVCTRL ----
	bool getReg_DRVCTRL_wasUpdated(){return m_reg_DRVCTRL_updated;}
	
	void setPolarityA(Polarity p){m_polarityA = p; m_reg_DRVCTRL_updated = false;}
	Polarity getPolarityA(){return m_polarityA;} 

	void setPolarityB(Polarity p){m_polarityB = p; m_reg_DRVCTRL_updated = false;}
	Polarity getPolarityB(){return m_polarityB;} 

	void setCurrentA(byte current){ m_currentA = current; m_reg_DRVCTRL_updated = false;}
	byte getCurrentA(){return m_currentA;}

	void setCurrentB(byte current){ m_currentB = current; m_reg_DRVCTRL_updated = false;}
	byte getCurrentB(){return m_currentB;}

	void setStepInterpolationEnable(bool en){m_stepInterpolationEnable = en; m_reg_DRVCTRL_updated = false;}
	bool stepInterpolationIsEnable(){return m_stepInterpolationEnable;}

	void setDoubleEdgePulsesEnable(bool en){m_doubleEdgeStepPulsesEnable = en; m_reg_DRVCTRL_updated = false;}
	bool doubleEdgePulsesIsEnable(){return m_doubleEdgeStepPulsesEnable;}

	void setMicroStepResolution(MicroStepResolution uStep){m_uStepResolution = uStep; m_reg_DRVCTRL_updated = false;};
	MicroStepResolution getMicroStepResolution(){return m_uStepResolution;}

	//--- CHOPCONF ---
	bool getReg_CHOPCONF_wasUpdated(){return m_reg_CHOPCONF_updated;}

	void setChopperBlankTime(CHOPCONF_BlankTime time){m_chopConfBlankTime = time; m_reg_CHOPCONF_updated = false;}
	CHOPCONF_BlankTime getChopperBlankTime(){return m_chopConfBlankTime;}

	CHOPCONF_ChopperMode getChopperMode(){return m_chopConfChopperMode;}

	void setChopperRandomToffTime(CHOPCONF_RandomToffTime rnd){m_chopConfRandomToffTime = rnd; m_reg_CHOPCONF_updated = false;};
	CHOPCONF_RandomToffTime getChopperRandomToffTime(){return m_chopConfRandomToffTime;}
	
	void setChopperNormalMode(CHOPCONF_HysteresisDecInterval hystDecInt, CHOPCONF_HysteresisLowInterval hystLowInt, CHOPCONF_HysteresisStartValue hystStartVal)
		{
			m_chopConfChopperMode = cm_standartMode;
			m_chopConfHystDecInt = hystDecInt;
			m_chopConfLowHystInt = hystLowInt;
			m_chopConfHystStartVal = hystStartVal;
			m_reg_CHOPCONF_updated = false;
		}

	CHOPCONF_HysteresisDecInterval getChopperHysteresisDecrementInterval(){ return m_chopConfHystDecInt;}

	CHOPCONF_HysteresisLowInterval getChopperHysteresisLowInterval(){ return m_chopConfLowHystInt;}

	CHOPCONF_HysteresisStartValue getHysteresisStartValue(){ return m_chopConfHystStartVal;}

	void setChopperConstantToffMode(CHOPCONF_SineWaveOffset sineWaveoffset,	CHOPCONF_FastDecayTimeSetting fastDecTimeSetting, CHOPCONF_CurrentComparator currentComparator)
		{
			m_chopConfChopperMode = cm_ConstToff;
			m_chopConfCurrentComparator = currentComparator;
			if(sineWaveoffset > 15)
				sineWaveoffset = 15;
			m_chopConfSineWaveOffset = sineWaveoffset;
			if(fastDecTimeSetting > 15)
				fastDecTimeSetting = 15;
			m_chopConfFastDecTimeSetting = fastDecTimeSetting;
			m_reg_CHOPCONF_updated = false;
		}

	CHOPCONF_CurrentComparator getCurrentComparator(){ return m_chopConfCurrentComparator;};
	

	CHOPCONF_SineWaveOffset getChopperSineWaveOffset(){ return m_chopConfSineWaveOffset;}

	CHOPCONF_FastDecayTimeSetting getChopperFastDecayTimeSetting(){return m_chopConfFastDecTimeSetting;}

	void setChopperDrvEnableAndToff(CHOPCONF_DrvEnableAndToff Toff){m_chopConfDrvEnableAndToff = Toff; m_reg_CHOPCONF_updated = false;}
	CHOPCONF_DrvEnableAndToff getChopperDrvEnableAndToff(){ return m_chopConfDrvEnableAndToff;}


	//---- SAMRTEN ---
	bool getReg_SMARTEN_wasUpdated(){return m_reg_SMARTEN_updated;}

	void setSmartEnMinimumCurrent(SMARTEN_MinimumCurrent minCurr){ m_smartEnMiminumCurrent = minCurr; m_reg_SMARTEN_updated = false;}
	SMARTEN_MinimumCurrent getSmartEnMinimumCurrent(){ return m_smartEnMiminumCurrent;}

	void setSmartEnCurrentDownStepSpeed(SMARTEN_CurrentDownStepSpeed cdss){ m_smartEnCurrentDownStepSpeed = cdss; m_reg_SMARTEN_updated = false;}
	SMARTEN_CurrentDownStepSpeed getSmartEnCurrentDownStepSpeed(){ return m_smartEnCurrentDownStepSpeed;}

	void setSmartEnStallGuardHysteresis(SMARTEN_StallGuardHysteresis sgh){ m_smartEnStallGuardHysteresis = sgh; m_reg_SMARTEN_updated = false;}
	SMARTEN_StallGuardHysteresis getSmartEnStallGuardHysteresis(){ return m_smartEnStallGuardHysteresis;}

	void setSmartEnCurrentUpStepWidth(SMARTEN_CurrentUpStepWidth suw){ m_smartEnCurrentUpStepWidth = suw;  m_reg_SMARTEN_updated = false;}
	SMARTEN_CurrentUpStepWidth getSmartEnCurrentUpStepWidth(){ return m_smartEnCurrentUpStepWidth;}

	void setSamrtEnStallGuardMinimum(SMARTEN_StallGuardMinimum enSg){ m_smartEnStallGuardMinimum = enSg; m_reg_SMARTEN_updated = false;}
	SMARTEN_StallGuardMinimum getSamrtEnStallGuardMinimum(){ return m_smartEnStallGuardMinimum;}

	//----- SGCSCONF ----
	bool getReg_SGCSCONF_wasUpdated(){return m_reg_SGCFCONF_updated;}

	void setSGCSConfStallGuardFilterEnable(SGCSCONF_StallGuardFilterEnable sgfe) {m_sgcsConfStallGuardFilterEn = sgfe; m_reg_SGCFCONF_updated = false;}
	SGCSCONF_StallGuardFilterEnable getSGCSConfStallGuardFilterEnable() {return m_sgcsConfStallGuardFilterEn;}

	void setSGCSConfStallGuardThreasholdValue(SGCSCONF_StallGuardThreasholdValue sgtv)
		{ 
			if(sgtv > 63) 
				sgtv = 63;
			
			if(sgtv < -64)
				sgtv = -64;
			
			m_sgcsConfStallThresholdVal = sgtv; 
			m_reg_SGCFCONF_updated = false;

		}
	SGCSCONF_StallGuardThreasholdValue getSGCSConfStallGuardThreasholdValue(){ return m_sgcsConfStallThresholdVal;}

	void setSGCSConfCurrentScale(SGCSCONF_CurrentScale cs)
		{ 
			if(cs > 31)
				cs = 31;
			m_sgcsConfCurrentScale = cs;
			m_reg_SGCFCONF_updated = false;
		}
	SGCSCONF_CurrentScale getSGCSConfCurrentScale(){ return m_sgcsConfCurrentScale;}

	//----- DRVCONF ----
	bool getReg_DRVCONF_wasUpdated(){return m_reg_DRVCONF_updated;}
	
	void setDrvConfSlopeControlHighSide(DRVCONF_SlopeControlHighSide schs){ m_drvConfSplopeControlHighSide = schs; m_reg_DRVCONF_updated = false;}
	DRVCONF_SlopeControlHighSide getDrvConfSlopeControlHighSide(){ return m_drvConfSplopeControlHighSide;}

	void setDrvConfSlopeControlLowSide(DRVCONF_SlopeControlLowSide scls){ m_drvConfSlopeControlLowSide = scls; m_reg_DRVCONF_updated = false;}
	DRVCONF_SlopeControlLowSide getDrvConfSlopeControlLowSide(){ return m_drvConfSlopeControlLowSide;}

	void setDrvConfShortToGndProtection(DRVCONF_ShortToGndProtection gndEn){ m_drvConfShortToGndProtection = gndEn; m_reg_DRVCONF_updated = false;}
	DRVCONF_ShortToGndProtection getDrvConfShortToGndProtection(){ return m_drvConfShortToGndProtection;}

	void setDrvConfShortToGndDetectionTimer(DRVCONF_ShortToGndDetectionTimer gndTime){ m_drvConfShortToGndDetectionTimer = gndTime; m_reg_DRVCONF_updated = false;}
	DRVCONF_ShortToGndDetectionTimer getDrvConfShortToGndDetectionTimer(){ return m_drvConfShortToGndDetectionTimer;}

	void setDrvConfStepDirInput (DRVCONF_StepDirInput dir){ m_drvConfStepDirInput = dir; m_reg_DRVCONF_updated = false;}
	DRVCONF_StepDirInput getDrvConfStepDirInput (){ return m_drvConfStepDirInput;}

	void setDrvConfSenseResistorVoltage (DRVCONF_SenseResistorVoltage srv){ m_drvConfSenseResistorVoltage = srv; m_reg_DRVCONF_updated = false;}
	DRVCONF_SenseResistorVoltage getDrvConfSenseResistorVoltage (){ return m_drvConfSenseResistorVoltage;}

	void	setDrvConfValueToRead (DRVCONF_ValueToRead rv){ m_drvConfValueToRead = rv; m_reg_DRVCONF_updated = false;}
	DRVCONF_ValueToRead	getDrvConfValueToRead (){ return m_drvConfValueToRead;}

	//--- Read Value ---
	
	word getMicroStepPosition(){ return m_microStepPosition;}
	word getStallGuardResult(){ return m_stallGuardResult;}
	word getCurrentControlScaling(){ return m_currentControlScaling;}

	readBackValue getStandStillStep(){return m_standStillStep;}

	readBackValue getOpenLoopIndicatorMotorA(){ return m_openLoopIndicatorMotorA;}
	readBackValue getOpenLoopIndicatorMotorB(){ return m_openLoopIndicatorMotorB;}

	readBackValue getShortToGndDetectedInMotorA(){ return m_shortToGndDetectedOnMotorA;}
	readBackValue getShortToGndDetectedInMotorB(){ return m_shortToGndDetectedOnMotorB;}

	readBackValue getOverTempertaturePreWarning(){ return m_overTemperaturePreWarning;}
	readBackValue getOverTemperature(){return m_overTemperature;}

	readBackValue getStallGuard(){ return m_stallGuardStatus;}

	//-----------------------------------
	dword write_DRVCONF();
	dword write_DRVCTRL();
	dword write_CHOPCONF();
	dword write_SMARTEN();
	dword write_SGSFCONF();

	dword read_DRVSTARTUS(DRVCONF_ValueToRead val);
	
protected:
	 dword transmitData(dword reg);
	 dword decodeDRVSTATUS(dword reg);

	 void Init();
private:
	SPI_Abstract * m_spi;

	bool m_enable;
	
	//--- DRVCTRL ----
	bool m_reg_DRVCTRL_updated;
	
	Polarity m_polarityA;
	Polarity m_polarityB;

	byte m_currentA;
	byte m_currentB;

	bool m_stepInterpolationEnable;

	bool m_doubleEdgeStepPulsesEnable;

	MicroStepResolution m_uStepResolution;

	//--- CHOPCONF ---
	bool m_reg_CHOPCONF_updated;

	CHOPCONF_BlankTime m_chopConfBlankTime;

	CHOPCONF_ChopperMode m_chopConfChopperMode;
	
	CHOPCONF_RandomToffTime m_chopConfRandomToffTime;

	CHOPCONF_HysteresisDecInterval m_chopConfHystDecInt;

	CHOPCONF_HysteresisLowInterval m_chopConfLowHystInt;

	CHOPCONF_HysteresisStartValue m_chopConfHystStartVal;

	CHOPCONF_CurrentComparator m_chopConfCurrentComparator;
	
	CHOPCONF_SineWaveOffset m_chopConfSineWaveOffset;

	CHOPCONF_FastDecayTimeSetting m_chopConfFastDecTimeSetting;

	CHOPCONF_DrvEnableAndToff m_chopConfDrvEnableAndToff;

	//---- SAMRTEN ---
	bool m_reg_SMARTEN_updated;
	
	SMARTEN_MinimumCurrent m_smartEnMiminumCurrent;

	SMARTEN_CurrentDownStepSpeed m_smartEnCurrentDownStepSpeed;

	SMARTEN_StallGuardHysteresis m_smartEnStallGuardHysteresis;

	SMARTEN_CurrentUpStepWidth m_smartEnCurrentUpStepWidth;

	SMARTEN_StallGuardMinimum m_smartEnStallGuardMinimum;

	//----- SGCSCONF ----
	bool m_reg_SGCFCONF_updated;

	SGCSCONF_StallGuardFilterEnable m_sgcsConfStallGuardFilterEn;
	
	SGCSCONF_StallGuardThreasholdValue m_sgcsConfStallThresholdVal;

	SGCSCONF_CurrentScale m_sgcsConfCurrentScale;

	//----- DRVCONF ----
	bool m_reg_DRVCONF_updated;

	DRVCONF_SlopeControlHighSide m_drvConfSplopeControlHighSide;

	DRVCONF_SlopeControlLowSide m_drvConfSlopeControlLowSide;

	DRVCONF_ShortToGndProtection m_drvConfShortToGndProtection;

	DRVCONF_ShortToGndDetectionTimer m_drvConfShortToGndDetectionTimer;

	DRVCONF_StepDirInput m_drvConfStepDirInput;

	DRVCONF_SenseResistorVoltage m_drvConfSenseResistorVoltage;

	DRVCONF_ValueToRead	m_drvConfValueToRead;

	//-- read values --
	word m_microStepPosition;
	word m_stallGuardResult;
	word m_currentControlScaling;

	readBackValue m_standStillStep;

	readBackValue m_openLoopIndicatorMotorA;
	readBackValue m_openLoopIndicatorMotorB;

	readBackValue m_shortToGndDetectedOnMotorA;
	readBackValue m_shortToGndDetectedOnMotorB;

	readBackValue m_overTemperaturePreWarning;
	readBackValue m_overTemperature;

	readBackValue m_stallGuardStatus;
	
};















#endif



















































