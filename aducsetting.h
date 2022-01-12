#ifndef ADUCSETTING_H
#define ADUCSETTING_H

#include "Global.h"
#include "ADuC_types.h"

#define __ADUC_MAX_SAMPLING_FRQ__       2000
#define __ADuC_MAX_SAMPLES_PER_PACKET   126

class ADuCsetting
{
public:
    enum settingConsistency
    {
        sc_noError = 0,
        sc_pADC_noChannelSelect,
        sc_aADC_noChannelSelect,    
        sc_aADC_DiffMode_ADC4_ADC5_pVref2_Error,
        sc_aADC_SingleEnded_ADC4_ADC5_pVref2_Error,
        sc_aADC_DiffMode_ADC4_ADC5_aVref2_Error,
        sc_aADC_SingleEnded_ADC4_ADC5_aVref2_Error,
        sc_pADC_aADC_samplingTheSameChannel,
        sc_pADC_protocol_zeroSamplePerPacket,
        sc_aADC_protocol_zeroSamplePerPacket,
        sc_ADC_mustHaveSameSamplesPerPacket,
        sc_ADC_tooMuchSamplesPerPacket,
        sc_ADC_oddSamplesPerPacket,
        sc_ADC_protocol_CanNotSend_P_A_SamplesTogether,
        sc_DAC_modeVSregADC0DAT_error,
        sc_numOfConsitency
    };

    ADuCsetting();
    ~ADuCsetting();

    //------------------- GET SETTINGS ---------------------
    //- ADC
    bool  get_ADC_P_isEnable(){ return m_ADC_P_enable;}
    int  get_ADC_frq(){return m_ADC_frq;}
    word  get_ADC_reg_ADCFLT(){return m_ADC_reg_ADCFLT;}

    //- PRIMARY ADC
    type_ADC_P_VREF  get_ADC_P_VREF(){return m_ADC_P_VREF;}
    int get_ADC_P_VREF_mV(){ return m_ADC_P_Vref_mV ;}
    type_ADC_P_PGAgain  get_ADC_P_PGAgain(){return m_ADC_P_PGAgain;}
    type_ADC_P_Mode  get_ADC_P_Mode(){return m_ADC_P_Mode;}
    type_ADC_P_Coding   get_ADC_P_Coding(){return m_ADC_P_Coding;}
    int  get_ADC_P_DifferentialChanEnabled(int i){  if (i >= ADuC_ADC_P_NUM_DIFFERENTIAL_CHAN) return -1;
                                                    return m_ADC_P_DifferentialChanEnabled[i];
                                                 }
    int *pADC_P_DifferentialChanEnabled(){return m_ADC_P_DifferentialChanEnabled;}
    int  get_ADC_P_SingleEndedChanEnabled(int i){   if (i >= ADuC_ADC_P_NUM_SINGLE_ENDED_CHAN) return -1;
                                                    return m_ADC_P_SingleEndedChanEnabled[i];}
    int *pADC_P_SingleEndedChanEnabled(){return m_ADC_P_SingleEndedChanEnabled;}
    int  get_ADC_P_ChanEnabled(int i){ return m_ADC_P_ChanEnabled[i];}
    int  get_ADC_P_NumChanEnabled(){ return m_ADC_P_NumChanEnabled;}
    int  get_ADC_P_NumChanEnabled_index(){ return m_ADC_P_NumChanEnabled_index;}

    //- AUXILIARY ADC
    bool  get_ADC_A_isEnable(){return m_ADC_A_enable;}
    type_ADC_A_VREF  get_ADC_A_VREF(){return m_ADC_A_VREF;}
    int get_ADC_A_VREF_mV(){ return m_ADC_A_Vref_mV ;}
    type_ADC_A_PGAgain get_ADC_A_PGAgain(){return m_ADC_A_PGAgain;}
    type_ADC_A_Mode  get_ADC_A_Mode(){return m_ADC_A_Mode;}
    type_ADC_A_Coding  get_ADC_A_Coding(){return m_ADC_A_Coding;}
    int  get_ADC_A_DifferentialChanEnabled(int i){   if (i >= ADuC_ADC_A_NUM_DIFFERENTIAL_CHAN) return -1;
                                                     return m_ADC_A_DifferentialChanEnabled[i];}
    int *pADC_A_DifferentialChanEnabled(){return m_ADC_A_DifferentialChanEnabled;}
    int  get_ADC_A_SingleEndedChanEnabled(int i){   if (i >= ADuC_ADC_A_NUM_SINGLE_ENDED_CHAN) return -1;
                                                    return m_ADC_A_SingleEndedChanEnabled[i];}
    int *pADC_A_SingleEndedChanEnabled(){return m_ADC_A_SingleEndedChanEnabled;}
    int  get_ADC_A_ChanEnabled(int i){ return m_ADC_A_ChanEnabled[i];}
    int  get_ADC_A_NumChanEnabled(){ return m_ADC_A_NumChanEnabled;}
    int  get_ADC_A_NumChanEnabled_index(){ return m_ADC_A_NumChanEnabled_index;}

    //- ADC Protocol
    type_ADC_P_NumBitToSend get_ADC_P_NumBitToSend(){ return m_ADC_P_NumBitToSend;}
    int get_ADC_P_NumBytesToSend();
    type_ADC_A_NumBitToSend get_ADC_A_NumBitToSend(){ return m_ADC_A_NumBitToSend;}
    int get_ADC_A_NumBytesToSend();
    int get_ADC_P_NumSamplesPerPacket(){ return m_ADC_P_NumSamplesPerPacket;}
    int get_ADC_A_NumSamplesPerPacket(){ return m_ADC_A_NumSamplesPerPacket;}
    bool get_ADC_SendSamplesWithinSamePacket(){ return m_ADC_SendSamplesWithinSamePacket;}
    int get_ADC_P_byteToSend(){ return m_ADC_P_byteToSend; }
    int get_ADC_A_byteToSend(){ return m_ADC_A_byteToSend; }

    // ADC REG --
    word get_ADC_reg_ADCSTA(){ return m_ADC_reg_ADCSTA;}
    byte get_ADC_reg_ADCMSKI(){ return m_ADC_reg_ADCMSKI;}
    byte get_ADC_reg_ADCMDE(){ return m_ADC_reg_ADCMDE;}
    word get_ADC_reg_ADC0CON(){ return m_ADC_reg_ADC0CON;}
    word get_ADC_reg_ADC1CON(){ return m_ADC_reg_ADC1CON;}
    byte get_ADC_reg_ADCCFG(){ return m_ADC_reg_ADCCFG;}
    word get_ADC_reg_ADC0OF(){ return m_ADC_reg_ADC0OF;}
    word get_ADC_reg_ADC0GN(){ return m_ADC_reg_ADC0GN;}
    word get_ADC_reg_ADC1OF(){ return m_ADC_reg_ADC1OF;}
    word get_ADC_reg_ADC1GN(){ return m_ADC_reg_ADC1GN;}
    word get_ADC_reg_ADC0RCR(){ return m_ADC_reg_ADC0RCR;}
    word get_ADC_reg_ADC0RCV(){ return m_ADC_reg_ADC0RCV;}
    word get_ADC_reg_ADC0TH(){ return m_ADC_reg_ADC0TH;}
    word get_ADC_reg_ADC0THC(){ return m_ADC_reg_ADC0THC;}
    word get_ADC_reg_ADC0THV(){ return m_ADC_reg_ADC0THV;}
    word get_ADC_reg_ADC0ACC(){ return m_ADC_reg_ADC0ACC;}
    word get_ADC_reg_ADC0ATH(){ return m_ADC_reg_ADC0ATH;}

    //- IEXC
    bool  get_IEXC0_isEnable(){return m_IEXC0_enable;}
    bool  get_IEXC1_isEnable(){return m_IEXC1_enable;}
    type_IEXC_Current get_IEXC_Current(){return m_IEXC_Current;}
    type_IEXC0_DIR  get_IEXC0_DIR(){return m_IEXC0_DIR;}
    type_IEXC1_DIR  get_IEXC1_DIR(){return m_IEXC1_DIR;}
    byte get_IEXC_reg_IEXCON(){ return m_IEXC_reg_IEXCON;}

    //- DAC
    bool  get_DAC_isEnable(){return m_DAC_enable;}
    word  get_DAC_DAT(){return m_DAC_DAT;}
    type_DAC_Range  get_DAC_range(){return m_DAC_range;}
    int get_DAC_VREF_mV(){ return m_DAC_Vref_mV ;}
    type_DAC_mode  get_DAC_mode(){return m_DAC_mode;}
    type_DAC_interpolationRate  get_DAC_interpolationRate(){return m_DAC_interpolationRate;}
    bool  get_DAC_OpampMode(){return m_DAC_OpampMode;}
    bool  get_DAC_Bypass(){return m_DAC_Bypass;}
    word  get_DAC_reg_DACCON(){ return m_DAC_reg_DACCON;}
    dword get_DAC_reg_DAC0DAT(){ return m_DAC_reg_DAC0DAT;}

    //- PWM
    bool  get_PWM0_isEnable(){return m_PWM0_enable;}
    bool  get_PWM1_isEnable(){return m_PWM1_enable;}
    long  get_PWM_frq(){return m_PWM_frq;}
    double  get_PWM0_duty(){return m_PWM0_duty;}
    double  get_PWM1_duty(){return m_PWM1_duty;}
    word  get_PWM_reg_PWMCON(){return m_PWM_reg_PWMCON;}
    word  get_PWM_reg_PWM0COM0(){return m_PWM_reg_PWM0COM0;}
    word  get_PWM_reg_PWM0COM1(){return m_PWM_reg_PWM0COM1;}
    word  get_PWM_reg_PWM0COM2(){return m_PWM_reg_PWM0COM2;}
    word  get_PWM_reg_PWM0LEN(){return m_PWM_reg_PWM0LEN;}

    //------------------- SET SETTINGS ---------------------
    //- ADC
    void  set_ADC_P_enable(bool __enable){ m_ADC_P_enable = __enable;}
    void  set_ADC_frq(int __frq){ if(__frq < 4) __frq = 4; if(__frq > __ADUC_MAX_SAMPLING_FRQ__) __frq = __ADUC_MAX_SAMPLING_FRQ__; m_ADC_frq = __frq;}
    void  set_ADC_reg_ADCFLT(word __ADCFLT){ m_ADC_reg_ADCFLT = __ADCFLT;}

    //- PRIMARY ADC
    void  set_ADC_P_VREF(type_ADC_P_VREF __VREF){ m_ADC_P_VREF = __VREF;}
    void  set_ADC_P_VREF_mV(int __mV){ m_ADC_P_Vref_mV = __mV;}
    void  set_ADC_P_PGAgain(type_ADC_P_PGAgain __PGAgain){ m_ADC_P_PGAgain = __PGAgain;}
    void  set_ADC_P_Mode(type_ADC_P_Mode __mode){ m_ADC_P_Mode = __mode;}
    void  set_ADC_P_Coding(type_ADC_P_Coding __coding){ m_ADC_P_Coding = __coding;}
    int   set_ADC_P_DifferentialChanEnabled(int i, int __enable){  if (i >= ADuC_ADC_P_NUM_DIFFERENTIAL_CHAN) return -1;
                                                    m_ADC_P_DifferentialChanEnabled[i] = __enable; return __enable;
                                                 }
    int  set_ADC_P_SingleEndedChanEnabled(int i, int __enable){   if (i >= ADuC_ADC_P_NUM_SINGLE_ENDED_CHAN) return -1;
                                                    m_ADC_P_SingleEndedChanEnabled[i] = __enable; return __enable;}
    //- AUXILIARY ADC
    void  set_ADC_A_enable(bool __enable){ m_ADC_A_enable = __enable;}
    void  set_ADC_A_VREF(type_ADC_A_VREF __VREF){ m_ADC_A_VREF = __VREF;}
    void  set_ADC_A_VREF_mV(int __mV){ m_ADC_A_Vref_mV = __mV;}
    void set_ADC_A_PGAgain(type_ADC_A_PGAgain __PGAgain){ m_ADC_A_PGAgain = __PGAgain;}
    void  set_ADC_A_Mode(type_ADC_A_Mode __mode){ m_ADC_A_Mode = __mode;}
    void  set_ADC_A_Coding(type_ADC_A_Coding __coding){ m_ADC_A_Coding = __coding;}
    int  set_ADC_A_DifferentialChanEnabled(int i, int __enable){   if (i >= ADuC_ADC_A_NUM_DIFFERENTIAL_CHAN) return -1;
                                                     m_ADC_A_DifferentialChanEnabled[i] = __enable; return __enable;}
    int  set_ADC_A_SingleEndedChanEnabled(int i, int __enable){   if (i >= ADuC_ADC_A_NUM_SINGLE_ENDED_CHAN) return -1;
                                                    m_ADC_A_SingleEndedChanEnabled[i] = __enable; return __enable;}

    //- ADC Protocol
    void  set_ADC_P_NumBitToSend(type_ADC_P_NumBitToSend __numBitToSend){ m_ADC_P_NumBitToSend = __numBitToSend;}
    void  set_ADC_A_NumBitToSend(type_ADC_A_NumBitToSend __numBitToSend){ m_ADC_A_NumBitToSend = __numBitToSend;}
    void  set_ADC_P_NumSamplesPerPacket(int __numSamplesPerPacket){ m_ADC_P_NumSamplesPerPacket = __numSamplesPerPacket;}
    void  set_ADC_A_NumSamplesPerPacket(int __numSamplesPerPacket){ m_ADC_A_NumSamplesPerPacket = __numSamplesPerPacket;}
    void  set_ADC_SendSamplesWithinSamePacket(bool __sendSamplesWithinSamePacket){ m_ADC_SendSamplesWithinSamePacket = __sendSamplesWithinSamePacket;}

    //- ADC REG
    void set_ADC_reg_ADCSTA(word __reg){ m_ADC_reg_ADCSTA = __reg;}
    void set_ADC_reg_ADCMSKI(byte __reg){  m_ADC_reg_ADCMSKI = __reg;}
    void set_ADC_reg_ADCMDE(byte __reg){ m_ADC_reg_ADCMDE = __reg;}
    void set_ADC_reg_ADC0CON(word __reg){ m_ADC_reg_ADC0CON = __reg;}
    void set_ADC_reg_ADC1CON(word __reg){ m_ADC_reg_ADC1CON = __reg;}
    void set_ADC_reg_ADCCFG(byte __reg){  m_ADC_reg_ADCCFG = __reg;}
    void set_ADC_reg_ADC0OF(word __reg){  m_ADC_reg_ADC0OF = __reg;}
    void set_ADC_reg_ADC0GN(word __reg){  m_ADC_reg_ADC0GN = __reg;}
    void set_ADC_reg_ADC1OF(word __reg){  m_ADC_reg_ADC1OF = __reg;}
    void set_ADC_reg_ADC1GN(word __reg){  m_ADC_reg_ADC1GN = __reg;}
    void set_ADC_reg_ADC0RCR(word __reg){  m_ADC_reg_ADC0RCR = __reg;}
    void set_ADC_reg_ADC0RCV(word __reg){  m_ADC_reg_ADC0RCV = __reg;}
    void set_ADC_reg_ADC0TH(word __reg){  m_ADC_reg_ADC0TH = __reg;}
    void set_ADC_reg_ADC0THC(word __reg){  m_ADC_reg_ADC0THC = __reg;}
    void set_ADC_reg_ADC0THV(word __reg){  m_ADC_reg_ADC0THV = __reg;}
    void set_ADC_reg_ADC0ACC(word __reg){  m_ADC_reg_ADC0ACC = __reg;}
    void set_ADC_reg_ADC0ATH(word __reg){  m_ADC_reg_ADC0ATH = __reg;}

    //- IEXC
    void  set_IEXC0_enable(bool __enable){ m_IEXC0_enable = __enable;}
    void  set_IEXC1_enable(bool __enable){ m_IEXC1_enable = __enable;}
    void  set_IEXC_Current(type_IEXC_Current __current){ m_IEXC_Current = __current;}
    void  set_IEXC0_DIR(type_IEXC0_DIR __dir){ m_IEXC0_DIR = __dir;}
    void  set_IEXC1_DIR(type_IEXC1_DIR __dir){ m_IEXC1_DIR = __dir;}
    void set_IEXC_reg_IEXCON(byte __reg){ m_IEXC_reg_IEXCON = __reg;}

    //- DAC
    void  set_DAC_enable(bool __enable){ m_DAC_enable = __enable;}
    void  set_DAC_DAT(word __DAT){ m_DAC_DAT = __DAT;}
    void  set_DAC_range(type_DAC_Range __range){ m_DAC_range = __range;}
    void  set_DAC_VREF_mV(int __mV){ m_DAC_Vref_mV = __mV;}
    void  set_DAC_mode(type_DAC_mode __mode){ m_DAC_mode = __mode;}
    void  set_DAC_interpolationRate(type_DAC_interpolationRate __intRate){ m_DAC_interpolationRate = __intRate;}
    void  set_DAC_OpampMode(bool __enable){ m_DAC_OpampMode = __enable;}
    void  set_DAC_Bypass(bool __enable){ m_DAC_Bypass = __enable;}
    void  set_DAC_reg_DACCON(word __reg){ m_DAC_reg_DACCON = __reg;}
    void  set_DAC_reg_DAC0DAT(dword __reg){ m_DAC_reg_DAC0DAT = __reg;}

    //- PWM
    void  set_PWM0_enable(bool __enable){ m_PWM0_enable = __enable;}
    void  set_PWM1_enable(bool __enable){ m_PWM1_enable = __enable;}
    void  set_PWM_frq(long __frq){ m_PWM_frq = __frq;}
    void  set_PWM0_duty(double __duty){ if (__duty > 100.0) __duty = 100.0; if (__duty < 0.0) __duty = 0.0; m_PWM0_duty = __duty;}
    void  set_PWM1_duty(double __duty){ if (__duty > 100.0) __duty = 100.0; if (__duty < 0.0) __duty = 0.0; m_PWM1_duty = __duty;}
    void  set_PWM_reg_PWMCON(word __PWMCON){ m_PWM_reg_PWMCON = __PWMCON;}
    void  set_PWM_reg_PWM0COM0(word __PWM0COM0){ m_PWM_reg_PWM0COM0 = __PWM0COM0;}
    void  set_PWM_reg_PWM0COM1(word __PWM0COM1){ m_PWM_reg_PWM0COM1 = __PWM0COM1;}
    void  set_PWM_reg_PWM0COM2(word __PWM0COM2){ m_PWM_reg_PWM0COM2 = __PWM0COM2;}
    void  set_PWM_reg_PWM0LEN(word __PWM0LEN){ m_PWM_reg_PWM0LEN = __PWM0LEN;}
    //-----------------------
    settingConsistency checkSettingConsistency();
    settingConsistency checkSettingConsistency_ADC();
    settingConsistency checkSettingConsistency_IEXC();
    settingConsistency checkSettingConsistency_DAC();
    settingConsistency checkSettingConsistency_PWM();
protected:
    void Init_ADC();
    void Init_IEXC();
    void Init_DAC();
    void Init_PWM();

private:
    bool m_ADC_P_enable;
    int m_ADC_frq;
    word m_ADC_reg_ADCFLT;
    type_ADC_P_VREF m_ADC_P_VREF;
    int m_ADC_P_Vref_mV;
    type_ADC_P_PGAgain m_ADC_P_PGAgain;
    type_ADC_P_Mode m_ADC_P_Mode;
    type_ADC_P_Coding m_ADC_P_Coding;
    int m_ADC_P_DifferentialChanEnabled[ADuC_ADC_P_NUM_DIFFERENTIAL_CHAN];
    int m_ADC_P_SingleEndedChanEnabled[ADuC_ADC_P_NUM_SINGLE_ENDED_CHAN];
    int m_ADC_P_ChanEnabled[ADuC_ADC_P_NUM_SINGLE_ENDED_CHAN];
    int m_ADC_P_NumChanEnabled;
    int m_ADC_P_NumChanEnabled_index;

    bool m_ADC_A_enable;
    type_ADC_A_VREF m_ADC_A_VREF;
    int m_ADC_A_Vref_mV;
    type_ADC_A_PGAgain m_ADC_A_PGAgain;
    type_ADC_A_Mode m_ADC_A_Mode;
    type_ADC_A_Coding m_ADC_A_Coding;
    int m_ADC_A_DifferentialChanEnabled[ADuC_ADC_A_NUM_DIFFERENTIAL_CHAN];
    int m_ADC_A_SingleEndedChanEnabled[ADuC_ADC_A_NUM_SINGLE_ENDED_CHAN];
    int m_ADC_A_ChanEnabled[ADuC_ADC_A_NUM_SINGLE_ENDED_CHAN];
    int m_ADC_A_NumChanEnabled;
    int m_ADC_A_NumChanEnabled_index;

    type_ADC_P_NumBitToSend m_ADC_P_NumBitToSend;
    type_ADC_A_NumBitToSend m_ADC_A_NumBitToSend;
    int m_ADC_P_NumSamplesPerPacket;
    int m_ADC_A_NumSamplesPerPacket;
    bool m_ADC_SendSamplesWithinSamePacket;
    int m_ADC_P_byteToSend;
    int m_ADC_A_byteToSend;

    word m_ADC_reg_ADCSTA;
    byte m_ADC_reg_ADCMSKI;
    byte m_ADC_reg_ADCMDE;
    word m_ADC_reg_ADC0CON;
    word m_ADC_reg_ADC1CON;
    byte m_ADC_reg_ADCCFG;
    word m_ADC_reg_ADC0OF;
    word m_ADC_reg_ADC0GN;
    word m_ADC_reg_ADC1OF;
    word m_ADC_reg_ADC1GN;
    word m_ADC_reg_ADC0RCR;
    word m_ADC_reg_ADC0RCV;
    word m_ADC_reg_ADC0TH;
    word m_ADC_reg_ADC0THC;
    word m_ADC_reg_ADC0THV;
    word m_ADC_reg_ADC0ACC;
    word m_ADC_reg_ADC0ATH;

    bool m_IEXC0_enable;
    bool m_IEXC1_enable;
    type_IEXC_Current m_IEXC_Current;
    type_IEXC0_DIR m_IEXC0_DIR;
    type_IEXC1_DIR m_IEXC1_DIR;
    byte m_IEXC_reg_IEXCON;

    bool m_DAC_enable;
    word m_DAC_DAT;
    type_DAC_Range m_DAC_range;
    int m_DAC_Vref_mV;
    type_DAC_mode m_DAC_mode;
    type_DAC_interpolationRate m_DAC_interpolationRate;
    bool m_DAC_OpampMode;
    bool m_DAC_Bypass;
    word m_DAC_reg_DACCON;
    dword m_DAC_reg_DAC0DAT;

    bool m_PWM0_enable;
    bool m_PWM1_enable;
    long m_PWM_frq;
    double m_PWM0_duty;
    double m_PWM1_duty;
    word m_PWM_reg_PWMCON;
    word m_PWM_reg_PWM0COM0;
    word m_PWM_reg_PWM0COM1;
    word m_PWM_reg_PWM0COM2;
    word m_PWM_reg_PWM0LEN;
};

#endif // ADUCSETTING_H
