#include "aducsetting.h"

ADuCsetting::ADuCsetting()
{
    Init_ADC();
    Init_IEXC();
    Init_DAC();
    Init_PWM();
}

ADuCsetting::~ADuCsetting()
{

}

void ADuCsetting::Init_ADC()
{
    int i;

    m_ADC_P_enable = false;
    m_ADC_frq = 4;
    m_ADC_reg_ADCFLT = 0;
    m_ADC_P_VREF = pADC_Vref_InternalRef;
    m_ADC_P_Vref_mV = 1200;
    m_ADC_P_PGAgain = pADC_PGA_Gain_1;
    m_ADC_P_Mode = pADC_mode_differential;
    m_ADC_P_Coding = pADC_coding_unipolar;

    for (i = 0; i < ADuC_ADC_P_NUM_DIFFERENTIAL_CHAN; i++)
    {
        m_ADC_P_DifferentialChanEnabled[i] = 0;
    }
    for (i = 0; i < ADuC_ADC_P_NUM_SINGLE_ENDED_CHAN; i++)
    {
        m_ADC_P_SingleEndedChanEnabled[i] = 0;
    }

    m_ADC_A_enable = false;
    m_ADC_A_VREF = aADC_Vref_InternalRef;
    m_ADC_A_Vref_mV = 1200;
    m_ADC_A_PGAgain = aADC_PGA_Gain_1;
    m_ADC_A_Mode = aADC_mode_differential;
    m_ADC_A_Coding = aADC_coding_unipolar;
    for (i = 0; i < ADuC_ADC_A_NUM_DIFFERENTIAL_CHAN; i++)
    {
        m_ADC_A_DifferentialChanEnabled[i] = 0;
    }
    for (i = 0; i < ADuC_ADC_A_NUM_SINGLE_ENDED_CHAN; i++)
    {
        m_ADC_A_SingleEndedChanEnabled[i] = 0;
    }

    m_ADC_P_NumBitToSend = pADC_bitToSend_8bit;
    m_ADC_A_NumBitToSend = aADC_bitToSend_8bit;
    m_ADC_P_NumSamplesPerPacket = 0;
    m_ADC_A_NumSamplesPerPacket = 0;
    m_ADC_SendSamplesWithinSamePacket = false;

    m_ADC_P_NumChanEnabled = 0;
    m_ADC_P_NumChanEnabled_index = 0;
    for (i = 0; i < ADuC_ADC_P_NUM_SINGLE_ENDED_CHAN; i++)
    {
        m_ADC_P_ChanEnabled[i] = 0;
    }
    m_ADC_A_NumChanEnabled = 0;
    m_ADC_A_NumChanEnabled_index = 0;
    for (i = 0; i < ADuC_ADC_A_NUM_SINGLE_ENDED_CHAN; i++)
    {
        m_ADC_A_ChanEnabled[i] = 0;
    }

}
void ADuCsetting::Init_IEXC()
{
    m_IEXC0_enable = false;
    m_IEXC1_enable = false;
    m_IEXC_Current = IEXC_0uA;
    m_IEXC0_DIR = IEXC0_DIR_toIEXC0_pin;
    m_IEXC1_DIR = IEXC1_DIR_toIEXC1_pin;
}

void ADuCsetting::Init_DAC()
{
    m_DAC_enable = false;
    m_DAC_reg_DAC0DAT = 0;
    m_DAC_range = DAC_range_0V_AVDD;
    m_DAC_Vref_mV = 2500;
    m_DAC_mode = DAC_mode_normal_12bit;
    m_DAC_interpolationRate = DAC_interpolationRate_UCLK_16;
    m_DAC_OpampMode = 0;
    m_DAC_Bypass = 0;
}

void ADuCsetting::Init_PWM()
{
    m_PWM0_enable = false;
    m_PWM1_enable = false;
    m_PWM_frq = 0;
    m_PWM0_duty = 0.0;
    m_PWM1_duty = 0.0;
    m_PWM_reg_PWMCON = 0;
    m_PWM_reg_PWM0COM0 = 0;
    m_PWM_reg_PWM0COM1 = 0;
    m_PWM_reg_PWM0COM2 = 0;
    m_PWM_reg_PWM0LEN = 0;
}

ADuCsetting::settingConsistency ADuCsetting::checkSettingConsistency_ADC()
{
    int i;
    bool consistent = false;

    m_ADC_P_byteToSend = 0;
    m_ADC_A_byteToSend = 0;

    m_ADC_P_NumChanEnabled = 0;
    for (i = 0; i < ADuC_ADC_P_NUM_SINGLE_ENDED_CHAN; i++)
    {
        m_ADC_P_ChanEnabled[i] = 0;
    }
    m_ADC_A_NumChanEnabled = 0;
    for (i = 0; i < ADuC_ADC_A_NUM_SINGLE_ENDED_CHAN; i++)
    {
        m_ADC_A_ChanEnabled[i] = 0;
    }

    if (m_ADC_P_enable)
    {
        //--- ADC Primary: verify channels selection ---
        consistent = false;
        switch (m_ADC_P_Mode)
        {
            case pADC_mode_differential:
                for (i = 0; i < ADuC_ADC_P_NUM_DIFFERENTIAL_CHAN; i++)
                {
                    if (m_ADC_P_DifferentialChanEnabled[i] != 0)
                    {
                        consistent = true;
                        m_ADC_P_ChanEnabled[m_ADC_P_NumChanEnabled] = i;
                        m_ADC_P_NumChanEnabled++;
                    }
                }
                for (i = 0; i < ADuC_ADC_P_NUM_SINGLE_ENDED_CHAN; i++)
                {
                    m_ADC_P_SingleEndedChanEnabled[i] = 0;
                }
                break;
            case pADC_mode_singleEnded:
                for (i = 0; i < ADuC_ADC_P_NUM_SINGLE_ENDED_CHAN; i++)
                {
                    if (m_ADC_P_SingleEndedChanEnabled[i] != 0)
                    {
                        consistent = true;
                        m_ADC_P_ChanEnabled[m_ADC_P_NumChanEnabled] = i;
                        m_ADC_P_NumChanEnabled++;
                    }
                }
                for (i = 0; i < ADuC_ADC_P_NUM_DIFFERENTIAL_CHAN; i++)
                {
                    m_ADC_P_DifferentialChanEnabled[i] = 0;
                }
                break;
        }
        if (!consistent)
        {
            return sc_pADC_noChannelSelect;
        }
        if (m_ADC_P_NumSamplesPerPacket == 0)
        {
            return sc_pADC_protocol_zeroSamplePerPacket;
        }
        if(!m_ADC_A_enable && m_ADC_SendSamplesWithinSamePacket)
        {
            return sc_ADC_protocol_CanNotSend_P_A_SamplesTogether;
        }
    }

    if (m_ADC_A_enable)
    {
        //--- ADC Auxiliary: verify channels selection ---
        consistent = false;
        switch (m_ADC_A_Mode)
        {
            case aADC_mode_differential:
                for (i = 0; i < ADuC_ADC_A_NUM_DIFFERENTIAL_CHAN; i++)
                {
                    if (m_ADC_A_DifferentialChanEnabled[i] != 0)
                    {
                        consistent = true;
                        m_ADC_A_ChanEnabled[m_ADC_A_NumChanEnabled] = i;
                        m_ADC_A_NumChanEnabled++;
                    }
                }
                for (i = 0; i < ADuC_ADC_A_NUM_SINGLE_ENDED_CHAN; i++)
                {
                    m_ADC_A_SingleEndedChanEnabled[i] = 0;
                }
                break;
            case aADC_mode_singleEnded:
                for (i = 0; i < ADuC_ADC_A_NUM_SINGLE_ENDED_CHAN; i++)
                {
                    if (m_ADC_A_SingleEndedChanEnabled[i] != 0)
                    {
                        consistent = true;
                        m_ADC_A_ChanEnabled[m_ADC_A_NumChanEnabled] = i;
                        m_ADC_A_NumChanEnabled++;
                    }
                }
                for (i = 0; i < ADuC_ADC_A_NUM_DIFFERENTIAL_CHAN; i++)
                {
                    m_ADC_A_DifferentialChanEnabled[i] = 0;
                }
                break;
        }
        if (!consistent)
        {
            return sc_aADC_noChannelSelect;
        }
        if (m_ADC_A_NumSamplesPerPacket == 0)
        {
            return sc_aADC_protocol_zeroSamplePerPacket;
        }
        if(!m_ADC_P_enable && m_ADC_SendSamplesWithinSamePacket)
        {
            return sc_ADC_protocol_CanNotSend_P_A_SamplesTogether;
        }
        //--- ADC Auxiliary - primary: verify channels selection vs Reference selection ---
        switch (m_ADC_P_VREF)
        {
            case pADC_Vref_InternalRef :
                break;
            case pADC_Vref_ExternalRef_Vref1:
                break;
            case pADC_Vref_ExternalRef_Vref1_HIGH:
                break;
            case pADC_Vref_ExternalRef_Vref2:
            case pADC_Vref_ExternalRef_Vref2_HIGH:
                if (m_ADC_A_DifferentialChanEnabled[ADuC_ADC_A_DIFF_CHAN_ADC4_ADC5] != 0)
                    return sc_aADC_DiffMode_ADC4_ADC5_pVref2_Error;

                if (m_ADC_A_SingleEndedChanEnabled[ADuC_ADC_A_SINGLE_ENDED_CHAN_ADC4_ADC5] != 0)
                    return sc_aADC_SingleEnded_ADC4_ADC5_pVref2_Error;
                break;
            case pADC_Vref_AVDD_AGND:
                break;
        }

        switch (m_ADC_A_VREF)
        {
            case aADC_Vref_InternalRef :
                break;
            case aADC_Vref_ExternalRef_Vref1:
                break;
            case aADC_Vref_ExternalRef_Vref1_HIGH:
                break;
            case aADC_Vref_ExternalRef_Vref2:
            case aADC_Vref_ExternalRef_Vref2_HIGH:
                if (m_ADC_A_DifferentialChanEnabled[ADuC_ADC_A_DIFF_CHAN_ADC4_ADC5] != 0)
                    return sc_aADC_DiffMode_ADC4_ADC5_aVref2_Error;

                if (m_ADC_A_SingleEndedChanEnabled[ADuC_ADC_A_SINGLE_ENDED_CHAN_ADC4_ADC5] != 0)
                    return sc_aADC_SingleEnded_ADC4_ADC5_aVref2_Error;
                break;
            case pADC_Vref_AVDD_AGND:
                break;

        }

         //--- ADC Auxiliary - primary: verify primary channels selection vs auxiliary channels selection ---
        if (m_ADC_P_enable)
        {
            if(m_ADC_P_DifferentialChanEnabled[ADuC_ADC_P_DIFF_CHAN_ADC2_ADC3] != 0 ||
               m_ADC_P_SingleEndedChanEnabled[ADuC_ADC_P_SINGLE_ENDED_CHAN_ADC2_ADC5] != 0)
            {
                if (m_ADC_A_DifferentialChanEnabled[ADuC_ADC_A_DIFF_CHAN_ADC2_ADC3] != 0 ||
                    m_ADC_A_SingleEndedChanEnabled[ADuC_ADC_A_SINGLE_ENDED_CHAN_ADC2_ADC5] != 0 ||
                    m_ADC_A_SingleEndedChanEnabled[ADuC_ADC_A_SINGLE_ENDED_CHAN_ADC3_ADC5] != 0 )
                {
                    return sc_pADC_aADC_samplingTheSameChannel;
                }
            }

            if(m_ADC_P_SingleEndedChanEnabled[ADuC_ADC_P_SINGLE_ENDED_CHAN_ADC3_ADC5] != 0)
            {
                if (m_ADC_A_DifferentialChanEnabled[ADuC_ADC_A_DIFF_CHAN_ADC2_ADC3] != 0 ||
                    m_ADC_A_SingleEndedChanEnabled[ADuC_ADC_A_SINGLE_ENDED_CHAN_ADC3_ADC5] != 0)
                {
                    return sc_pADC_aADC_samplingTheSameChannel;
                }
            }

            if (m_ADC_SendSamplesWithinSamePacket)
            {
                if (m_ADC_P_NumSamplesPerPacket != m_ADC_A_NumSamplesPerPacket)
                    return sc_ADC_mustHaveSameSamplesPerPacket;
            }
        }
    }

    //---- Verify maximum samples per packets
    if (m_ADC_SendSamplesWithinSamePacket)
    {
        if(( (m_ADC_P_NumSamplesPerPacket * get_ADC_P_NumBytesToSend()) + (m_ADC_A_NumSamplesPerPacket * get_ADC_P_NumBytesToSend()) ) > __ADuC_MAX_SAMPLES_PER_PACKET)
            return sc_ADC_tooMuchSamplesPerPacket;
    }else
    {
        if((m_ADC_P_NumSamplesPerPacket * get_ADC_P_NumBytesToSend() ) > __ADuC_MAX_SAMPLES_PER_PACKET)
            return sc_ADC_tooMuchSamplesPerPacket;
        if(( m_ADC_A_NumSamplesPerPacket * get_ADC_P_NumBytesToSend()) > __ADuC_MAX_SAMPLES_PER_PACKET)
            return sc_ADC_tooMuchSamplesPerPacket;
    }
    if(m_ADC_P_enable && m_ADC_P_NumChanEnabled > 0)
    {
        if((m_ADC_P_NumSamplesPerPacket % m_ADC_P_NumChanEnabled) != 0)
            return sc_ADC_oddSamplesPerPacket;
    }
    if(m_ADC_A_enable && m_ADC_A_NumChanEnabled > 0)
    {
        if((m_ADC_A_NumSamplesPerPacket % m_ADC_A_NumChanEnabled) != 0)
        return sc_ADC_oddSamplesPerPacket;
    }

    m_ADC_P_byteToSend = get_ADC_P_NumSamplesPerPacket() * get_ADC_P_NumBytesToSend();
    m_ADC_A_byteToSend = get_ADC_A_NumSamplesPerPacket() * get_ADC_A_NumBytesToSend();

    return sc_noError;
}

ADuCsetting::settingConsistency ADuCsetting::checkSettingConsistency_IEXC()
{
    return sc_noError;
}

ADuCsetting::settingConsistency ADuCsetting::checkSettingConsistency_DAC()
{
    if (m_DAC_enable)
    {
        //--- Mode VS ADC0DAT value
        switch(m_DAC_mode)
        {
            case DAC_mode_normal_12bit:
                if(m_DAC_reg_DAC0DAT > 4095)
                {
                    return sc_DAC_modeVSregADC0DAT_error;
                }
                break;
            case DAC_mode_interpolation_16bit:
                if(m_DAC_reg_DAC0DAT > 65535)
                {
                    return sc_DAC_modeVSregADC0DAT_error;
                }
                break;
        }

    }
    return sc_noError;
}

ADuCsetting::settingConsistency ADuCsetting::checkSettingConsistency_PWM()
{
    return sc_noError;
}

ADuCsetting::settingConsistency ADuCsetting::checkSettingConsistency()
{
    settingConsistency consistency = sc_noError;

    consistency = checkSettingConsistency_ADC();
    if(consistency != sc_noError)
            return consistency;

    consistency = checkSettingConsistency_IEXC();
    if(consistency != sc_noError)
            return consistency;

    consistency = checkSettingConsistency_DAC();
    if(consistency != sc_noError)
            return consistency;

    consistency = checkSettingConsistency_PWM();
    if(consistency != sc_noError)
            return consistency;

    return sc_noError;
}

int ADuCsetting::get_ADC_P_NumBytesToSend()
{
    switch(m_ADC_P_NumBitToSend)
    {
        case pADC_bitToSend_8bit:
            return 1;
        case pADC_bitToSend_16bit:
            return 2;
        case pADC_bitToSend_24bit:
            return 3;
    }
}

int ADuCsetting::get_ADC_A_NumBytesToSend()
{
    switch(m_ADC_A_NumBitToSend)
    {
        case aADC_bitToSend_8bit:
            return 1;
        case aADC_bitToSend_16bit:
            return 2;
        case aADC_bitToSend_24bit:
            return 3;
    }
}




















