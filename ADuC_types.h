#ifndef _ADUC_TYPES_H_
#define _ADUC_TYPES_H_

enum type_ADC_P_DIFF_CHAN
{
    ADuC_ADC_P_DIFF_CHAN_ADC0_ADC1 = 0,
    ADuC_ADC_P_DIFF_CHAN_ADC2_ADC3,
    ADuC_ADC_P_NUM_DIFFERENTIAL_CHAN
};

enum type_ADC_P_SINGLE_ENDED_CHAN
{
    ADuC_ADC_P_SINGLE_ENDED_CHAN_ADC0_ADC5 = 0,
    ADuC_ADC_P_SINGLE_ENDED_CHAN_ADC1_ADC5,
    ADuC_ADC_P_SINGLE_ENDED_CHAN_ADC2_ADC5,
    ADuC_ADC_P_SINGLE_ENDED_CHAN_ADC3_ADC5,
    ADuC_ADC_P_NUM_SINGLE_ENDED_CHAN
};

enum type_ADC_A_DIFF_CHAN
{
    ADuC_ADC_A_DIFF_CHAN_ADC2_ADC3 = 0,
    ADuC_ADC_A_DIFF_CHAN_ADC4_ADC5,
    ADuC_ADC_A_DIFF_CHAN_ADC6_ADC7,
    ADuC_ADC_A_DIFF_CHAN_ADC8_ADC9,
    ADuC_ADC_A_NUM_DIFFERENTIAL_CHAN
};

enum type_ADC_A_SINGLE_ENDED_CHAN
{
    ADuC_ADC_A_SINGLE_ENDED_CHAN_ADC2_ADC5 = 0,
    ADuC_ADC_A_SINGLE_ENDED_CHAN_ADC3_ADC5,
    ADuC_ADC_A_SINGLE_ENDED_CHAN_ADC4_ADC5,
    ADuC_ADC_A_SINGLE_ENDED_CHAN_ADC6_ADC5,
    ADuC_ADC_A_SINGLE_ENDED_CHAN_ADC7_ADC5,
    ADuC_ADC_A_SINGLE_ENDED_CHAN_ADC8_ADC5,
    ADuC_ADC_A_SINGLE_ENDED_CHAN_ADC9_ADC5,
    ADuC_ADC_A_NUM_SINGLE_ENDED_CHAN
};

/**
Available reference for the primary ADC
*/
enum type_ADC_P_VREF
{
    pADC_Vref_InternalRef = 0,	/**< internal reference selected (1.2V)*/
    pADC_Vref_ExternalRef_Vref1,	/**< external reference inputs (VREF+, VREF-) selected. Set the HIGHEXTREF0 bit if the reference voltage exceeds 1.3 V*/
    pADC_Vref_ExternalRef_Vref1_HIGH,
    pADC_Vref_ExternalRef_Vref2,	/**< auxiliary external reference inputs (ADC4/EXT_REF2IN+, ADC5/EXT_REF2IN-) selected. Set the HIGHEXTREF0 bit if the reference voltage exceeds 1.3 V.*/
    pADC_Vref_ExternalRef_Vref2_HIGH,
    pADC_Vref_AVDD_AGND, 		/**< (AVDD, AGND) divide-by-two selected*/
    pADC_VrefNumReferences
};

/**
Available gain for the primary ADC PGA
*/
enum  type_ADC_P_PGAgain
{
    pADC_PGA_Gain_1 = 0,
    pADC_PGA_Gain_2,
    pADC_PGA_Gain_4,
    pADC_PGA_Gain_8,
    pADC_PGA_Gain_16,
    pADC_PGA_Gain_32,
    pADC_PGA_Gain_64,
    pADC_PGA_Gain_128,
    pADC_PGA_Gain_256,
    pADC_PGA_Gain_512,
    pADC_PGA_Gain_NumOfGains
};

enum type_ADC_P_Mode
{
    pADC_mode_differential = 0,
    pADC_mode_singleEnded,
    pADC_mode_numOfMode
};

enum type_ADC_P_Coding
{
    pADC_coding_unipolar = 0,
    pADC_coding_bipolar,
    pADC_coding_numOfCoding
};



/**
Available reference for the auxiliary ADC
*/
enum type_ADC_A_VREF
{
    aADC_Vref_InternalRef = 0,	/**< internal reference selected (1.2V)*/
    aADC_Vref_ExternalRef_Vref1,	/**< external reference inputs (VREF+, VREF-) selected. Set the HIGHEXTREF1 bit if the reference voltage exceeds 1.3 V*/
    aADC_Vref_ExternalRef_Vref1_HIGH,
    aADC_Vref_ExternalRef_Vref2,	/**< auxiliary external reference inputs (ADC4/EXT_REF2IN+, ADC5/EXT_REF2IN-) selected. Set the HIGHEXTREF1 bit if the reference voltage exceeds 1.3 V.*/
    aADC_Vref_ExternalRef_Vref2_HIGH,
    aADC_Vref_AVDD_AGND, 		/**< (AVDD, AGND) divide-by-two selected. If this configuration is selected, the HIGHEXTREF1 bit is set automatically.*/
    aADC_Vref_AVDD_ADC3,		/**< (AVDD, ADC3). ADC3 can be used as the negative input terminal for the reference source.*/
    aADC_Vref_NumReferences
};

/**
Available gain for the auxiliary ADC - digital gain
*/
enum  type_ADC_A_PGAgain
{
    aADC_PGA_Gain_1 = 0,
    aADC_PGA_Gain_2,
    aADC_PGA_Gain_4,
    aADC_PGA_Gain_8,
    aADC_PGA_Gain_NumOfGains
};

enum type_ADC_A_Mode
{
    aADC_mode_differential = 0,
    aADC_mode_singleEnded,
    aADC_mode_numOfMode
};

enum type_ADC_A_Coding
{
    aADC_coding_unipolar = 0,
    aADC_coding_bipolar,
    aADC_coding_numOfCoding
};


enum type_ADC_P_NumBitToSend
{
    pADC_bitToSend_8bit = 0,
    pADC_bitToSend_16bit,
    pADC_bitToSend_24bit
};

enum type_ADC_A_NumBitToSend
{
    aADC_bitToSend_8bit = 0,
    aADC_bitToSend_16bit,
    aADC_bitToSend_24bit
};

enum type_IEXC_Current
{
    IEXC_0uA = 0,
    IEXC_200uA,
    IEXC_400uA,
    IEXC_600uA,
    IEXC_800uA,
    IEXC_1000uA,
    IEXC_numOfCurrent
};

enum type_IEXC0_DIR
{
    IEXC0_DIR_toIEXC0_pin = 0,
    IEXC0_DIR_toIEXC1_pin,
    IEXC0_DIR_numOfDir
};

enum type_IEXC1_DIR
{
    IEXC1_DIR_toIEXC1_pin = 0,
    IEXC1_DIR_toIEXC0_pin,
    IEXC1_DIR_numOfDir
};

enum type_DAC_Range
{
    DAC_range_0V_AVDD = 0,
    DAC_range_ADC5_ADC4,
    DAC_range_VREF_EXT,
    DAC_range_VREF_INT,
    DAC_range_numOfRange
};

enum type_DAC_mode
{
    DAC_mode_normal_12bit = 0,
    DAC_mode_interpolation_16bit,
    DAC_mode_numOfMode
};

enum type_DAC_interpolationRate
{
    DAC_interpolationRate_UCLK_16 = 0,
    DAC_interpolationRate_UCLK_32,
    DAC_interpolationRate_numOfIntRate
};


#endif

