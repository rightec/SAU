/**
@file		Channels.h
@brief		Classes to manage harwdare channels.

			Specifically here there are all the classes that manage the hw channels, so:
			- Impedance
			- Pressure
			- pH ISFET
			- Vol
			- Flw
			- EMG
			- Generic
			- Power Voltage 
			- Board's Current consumption

@author		Nicola Molinazzi
@date		18/01/2011
@version	01.10

@section history Revisions

From 01.00 to 01.10
- added general hardware channel
- modified the manage of the channel type propriety (m_chanType) with the HwChannel and within the HwChannelManager
- added the method inline bool popAdcData (T *adc, int num);

*/



#ifndef __CHANNELS_H__
#define __CHANNELS_H__

#include "global.h" 
#include "graph.h"
/**
Acd bits resolution.
*/
typedef enum
{
	_8_BITS_RESOLUTION_ = 0, /**< 8 bits resolution - byte */
	_10_BITS_RESOLUTION_,	 /**< 8 bits resolution - word */
	_12_BITS_RESOLUTION_,	 /**< 8 bits resolution - word */
	_16_BITS_RESOLUTION_,    /**< 8 bits resolution - word */
	_24_BITS_RESOLUTION_,	 /**< 8 bits resolution - dword */
	_32_BITS_RESOLUTION_,	 /**< 8 bits resolution - dword */
		
}bitResolution;

/**
Analog channels type.
*/
typedef enum
{
	_HW_CHAN_NO_TYPE_ = 0,			/**< None */
	_HW_CHAN_IMP_TYPE_,				/**< Impedance */
	_HW_CHAN_PRS_TYPE_,				/**< Pressure */
	_HW_CHAN_pHi_TYPE_,				/**< pH ISFET */
	_HW_CHAN_VINF_TYPE_,				/**< Infused volume */
	_HW_CHAN_VV_TYPE_,				/**< Voided volume */
	_HW_CHAN_EMG_TYPE_,				/**< Electromyography */
	_HW_CHAN_pH_TYPE_,				/**< pH glass or antimony */
	_HW_CHAN_UEC_TYPE_,				/**< UEC */
	_HW_CHAN_POWER_VOLTAGE_TYPE_,	/**< power voltage */
	_HW_CHAN_POWER_CURRENT_TYPE_,	/**< current consumption */
	_HW_CHAN_GENERIC_TYPE_,			/**< Generic Hardware channel*/
	
}HwChannelType;

/**
Hardware channels errors.
*/
typedef enum
{
	_HW_CHAN_NO_ERROR_ = 0,			/**< None */
	_HW_CHAN_BUFFER_OVER_FLOW_,		/**< buffer overflow */
}HwChanKindOfError;

template <class T>
class AdcRingBuffer
{
	public:
		AdcRingBuffer(int __size) { alloc(__size);	}
		AdcRingBuffer()
			{
				// + Crea il buffer circolare e lo inizializza
				m_pBuf = NULL;
				m_size = 0; 			// buffer size annullato
				zeroIndex();
			}
		
		~AdcRingBuffer()
			{
				// + Distrugge il buffer circolare
				
				if( m_pBuf )
				{
					destroy();
				}
			}

		int resize(int __newsize)
			{
				if( m_pBuf )
				{
					destroy();
				}

				alloc(__newsize);

				return m_pBuf != NULL;
			}// ridimensiona il buffer di lavoro
			
		int read(T *__pBuf, int __count)
			{
				int delta = 0;
				//int bufused = used();

					if( m_pBuf == NULL )
						return 0;
					
					//if( bufused == 0 )
						//return 0;
					
					//if( bufused < __count )					// se tutto non ci sta...
						//return 0;//__count = bufused;					// ritorniamo quello che c'è

					if( m_idxGet + __count <= m_size )		// se ci sta in un sol fiato
					{
						memcpy(__pBuf, &m_pBuf[m_idxGet], __count * sizeof(T));	// copia tutto

						m_idxGet += __count;	// avanti l'indice di copia

						if( m_idxGet >= m_size ) 
						{
							m_idxGetRound++;
							m_idxGet = 0;
						}
					}
					else
					{
						delta = m_size - m_idxGet;					// rimanenti nel fondo del sacco

						__count -= delta;

						memcpy(__pBuf, &m_pBuf[m_idxGet], delta * sizeof(T));	// copia il primo pezzo
					
						m_idxGetRound++;
						m_idxGet = 0;
						
						memcpy( &__pBuf[delta], m_pBuf,__count * sizeof(T));	// copia il resto

						m_idxGet += __count;	// avanti l'indice di copia
					}

					return delta + __count;
				}

		int read_isr(T *__pBuf, int __count)
			{
				int delta = 0;
				//int bufused = used_isr();

					if( m_pBuf == NULL )
						return 0;
					
					//if( bufused == 0 )
						//return 0;
					
					//if( bufused < __count )					// se tutto non ci sta...
						//return 0;//__count = bufused;					// ritorniamo quello che c'è

					if( m_idxGet + __count <= m_size )		// se ci sta in un sol fiato
					{
						memcpy(__pBuf, &m_pBuf[m_idxGet], __count * sizeof(T));	// copia tutto

						m_idxGet += __count;	// avanti l'indice di copia

						if( m_idxGet >= m_size ) 
						{
							m_idxGetRound++;
							m_idxGet = 0;
						}
					}
					else
					{
						delta = m_size - m_idxGet;					// rimanenti nel fondo del sacco

						__count -= delta;

						memcpy(__pBuf, &m_pBuf[m_idxGet], delta * sizeof(T));	// copia il primo pezzo

						m_idxGetRound++;
						m_idxGet = 0;
						
						memcpy( &__pBuf[delta], m_pBuf,__count * sizeof(T));	// copia il resto

						m_idxGet += __count;	// avanti l'indice di copia
					}

					return delta + __count;
				}

				
		int write(const T *__pBuf, int __count)
				{
					int delta = 0;
					//int bufused = used();

					//if( m_pBuf == NULL )
						//return 0;

					//if( m_size - bufused < __count )		// se tutto non ci sta...
						//return 0;	

					if( m_idxMem + __count <= m_size )		// se ci sta in un sol fiato
					{
						memcpy( &m_pBuf[m_idxMem], __pBuf,__count * sizeof(T));	// copia tutto

						m_idxMem += __count;	// avanti l'indice di copia

						if( m_idxMem >= m_size ) 
						{
							m_idxMemRound++;
							m_idxMem = 0;
						}
					}
					else
					{
						delta = m_size - m_idxMem;					// rimanenti nel fondo del sacco

						__count -= delta;

						memcpy(&m_pBuf[m_idxMem], __pBuf, delta * sizeof(T));	// copia il primo pezzo

						m_idxMemRound++;
						m_idxMem = 0;		// va a capo
						
						memcpy( &m_pBuf[m_idxMem], &__pBuf[delta],__count * sizeof(T));	// copia il resto

						m_idxMem += __count;	// avanti l'indice di copia
					}
					return __count + delta;
				}

		int write_isr(const T *__pBuf, int __count)
				{
					int delta = 0;
					//int bufused = used_isr();

					//if( m_pBuf == NULL )
						//return 0;

					//if( m_size - bufused < __count )		// se tutto non ci sta...
						//return 0;	

					if( m_idxMem + __count <= m_size )		// se ci sta in un sol fiato
					{
						memcpy( &m_pBuf[m_idxMem], __pBuf,__count * sizeof(T));	// copia tutto

						m_idxMem += __count;	// avanti l'indice di copia

						if( m_idxMem >= m_size ) 
						{
							m_idxMemRound++;
							m_idxMem = 0;
						}
					}
					else
					{
						delta = m_size - m_idxMem;					// rimanenti nel fondo del sacco

						__count -= delta;

						memcpy(&m_pBuf[m_idxMem], __pBuf, delta * sizeof(T));	// copia il primo pezzo

						m_idxMemRound++;
						m_idxMem = 0;		// va a capo
						
						memcpy( &m_pBuf[m_idxMem], &__pBuf[delta],__count * sizeof(T));	// copia il resto

						m_idxMem += __count;	// avanti l'indice di copia
					}
					return __count + delta;
				}
		
		inline T get() { 	register T c = m_pBuf[m_idxGet++]; 
							if(m_idxGet >= m_size) 
							{
								m_idxGetRound++;	//incrementare prima m_idxGetRound per evitare errori di overrun a interrupt attivati
								m_idxGet = 0;
							}
							return c;
						}
		
		inline int put(T __c) { m_pBuf[m_idxMem++] = __c; 
									if(m_idxMem >= m_size) 
								{
									m_idxMemRound++;
									m_idxMem = 0; 
								}
								return __c;
							  }

					// routines dedicate per il transfer da routines irq
		inline T get_isr() { register T c = m_pBuf[m_idxGet++];
									if(m_idxGet >= m_size) 
									{
										m_idxGetRound++;
										m_idxGet = 0;
									}
									return c; 
								}
					
		inline int put_isr(T __c) { m_pBuf[m_idxMem++] = __c; 
										if(m_idxMem >= m_size)
										{
											m_idxMemRound++;
											m_idxMem = 0;
										}
										return __c; }

		int used()
			{
				int bufused;
				asm("di");
				register int idxMem = m_idxMem;
				register int idxMemRound = m_idxMemRound;

				register int idxGet = m_idxGet;
				register int idxGetRound = m_idxGetRound;
				asm("ei");	
				
				if( idxMem >= idxGet )	// questo è facile
				{
					if(idxGetRound == idxMemRound)
					{
						bufused = idxMem - idxGet;
					}else
					{
						bufused = m_size;
					}
				}
				else
				{
					bufused = m_size - idxGet;
					bufused += idxMem;
				}
				return( bufused );
			}

		int used_isr()
			{
				int bufused;
				int idxMem = m_idxMem;
				int idxMemRound = m_idxMemRound;

				int idxGet = m_idxGet;
				int idxGetRound = m_idxGetRound;	
				
				if( idxMem >= idxGet )	// questo è facile
				{
					if(idxGetRound == idxMemRound)
					{
						bufused = idxMem - idxGet;
					}else
					{
						bufused = m_size;
					}
				}
				else
				{
					bufused = m_size - idxGet;
					bufused += idxMem;
				}
				return( bufused );
			}
		
		int size() { return m_size; }
		bool valid() { return m_pBuf != NULL; }
		int isempty() { return (m_idxMem == m_idxGet) && (m_idxGetRound == m_idxMemRound); }
		int isempty_isr() { return (m_idxMem == m_idxGet) && (m_idxGetRound == m_idxMemRound); }

		int isfull() { return (used() >= m_size); }
		int isfull_isr() { return (used_isr() >= m_size); }

		int isvalid() { return m_pBuf != NULL; }
		void clear() { asm("di"); m_count = m_idxMem = m_idxGet = m_idxMemRound = m_idxGetRound =0; asm("ei"); }
		void clear_isr() { m_count = m_idxMem = m_idxGet = m_idxMemRound = m_idxGetRound =0;  }

	protected:
		void alloc(int __size)
			{
				m_pBuf = new T[__size];		// buffer dati allocato in dinamica
			
				if( m_pBuf != NULL )
					m_size = __size;		// buffer size paddato 
				else
					m_size = 0; 			// buffer size annullato
			
				zeroIndex();
			}
		
		void destroy()
			{
				delete [] m_pBuf;
				m_pBuf = NULL;
				m_size = 0;
				zeroIndex();
			}
		
		void zeroIndex()
			{
				m_count = 0;				// counter per ? profiling ? boh 
				m_idxMem = 0;				// indice dentro
				m_idxGet = 0;				// indice fuori	
				m_idxGetRound = 0;
				m_idxMemRound = 0;
			}

	private:
		T 	*m_pBuf;		// buffer dati allocato in dinamica
		int 	 m_size;		// buffer size paddato 
		int	 	 m_count;		// count locale per test
		int		 m_idxMem;		// indice dentro
		int		 m_idxGet;		// indice fuori
		int m_idxMemRound;
	    int m_idxGetRound;
};


/**
Generic hardware channel implementation class.

It implements a general hardware channels with its main proprieties, as:
	- sample frequency
	- bit resolution
	- gain and offset
	- samples' buffer
	- ....
	
HwChannel is a template class were T is the of data sampled (byte, word or dword) 
and capacity the dimension of the samples's buffer. 

@param T type of data sampled (byte, word or dword). 
@param capacity dimension of the samples' buffer.
*/
template <class T, int capacity>
class HwChannel
{
	public:
		/**
		Class constructor. 
		@param bits adc bits resolution.
		*/
		HwChannel(bitResolution bits = _16_BITS_RESOLUTION_): m_fifoAdcData(capacity)
		{
		    m_fifoAdcData.clear();
			m_bitsResolution = bits;
			m_offset = 0;
			m_offsetToZero = 0;
			m_gain = 1.0;
			m_sampleFrq = 100;
			m_isZeroable = false;
			m_isEnabled = true;
			m_adcVoltageReference_mV = 3300.0;
			m_hwChanError = _HW_CHAN_NO_ERROR_;
			m_lastAdcSampled = 0;
			m_errorsOccurence = 0;
			
			switch (m_bitsResolution)
			{
				case _8_BITS_RESOLUTION_:
					m_adcResolution = 256.0;
					break;
				case _10_BITS_RESOLUTION_:
					m_adcResolution = 1024.0;
					break;
				case _12_BITS_RESOLUTION_:
					m_adcResolution = 4096.0;
					break;
				case _16_BITS_RESOLUTION_:	
					m_adcResolution = 65536.0;
					break;
				case _24_BITS_RESOLUTION_:	
					m_adcResolution = 16777216.0;
					break;
				case _32_BITS_RESOLUTION_:
					m_adcResolution = 4294967296.0;
					break;
			}
			m_chanType = _HW_CHAN_NO_TYPE_;
		};
		/**
		Class descructor.
		*/
		~HwChannel(){};

		/**
		@return the channel's sample frequency
		*/
		dword getSampleFrq(){ return m_sampleFrq;};
		
		/**
		Sets the channel sample frequency.
		@param frq channel's sample frequency.
		*/
		void setSampleFrq(dword frq){ m_sampleFrq = frq;};

		/**
		@return if the channel is zeroable
		*/
		bool getIsZeroable(){ return m_isZeroable;};
		/**
		Sets if the channel is zeroable accordint to z.
		@param z true set the channel as zeroable, false as NOT zeroable.
		@return the channel's sample frequency.
		*/
		void setIsZeroable(bool z){ m_isZeroable = z;};

		/**
		@return the channel's offset
		*/
		long getOffset(){ return m_offset;};
		/**
		Sets the channel's offset.
		@param offset channel's offset
		*/
		void setOffset(long offset){ m_offset = offset;};

		/**
		@return the channel's gain
		*/
		double getGain(){ return m_gain;};
		/**
		Sets the channel's gain.
		@param gain channel's gain
		*/
		void setGain(double gain){ m_gain = gain;};

		/**
		@return channel's bits resolution
		*/
		bitResolution getBitResolution(){return m_bitsResolution;};

		/**
		@return the number of samples buffered.
		*/
		inline int getNumDataBuffered(){return (m_fifoAdcData.used());};
		inline int getNumDataBuffered_isr(){return (m_fifoAdcData.used_isr());};
		/**
		@return true if the samples buffer is empty, false otherwise.
		*/
		inline bool isSampleBufferEmpty(){return m_fifoAdcData.isempty();};
		inline bool isSampleBufferEmpty_isr(){return m_fifoAdcData.isempty_isr();};
		/**
		@return true if the samples buffer is full, false otherwise.
		*/
		bool isSampleBufferFull(){return m_fifoAdcData.isfull();};
		bool isSampleBufferFull_isr(){return m_fifoAdcData.isfull_isr();};
		/**
		@param adc is the last data sampled - adc value.
		@return always 1.
		*/
		int getLastAdcDataSampled(T &adc){adc = m_lastAdcSampled; return 1;};		

		/**
		@param adc is the last data sampled - physical value.
		@return always 1.
		*/
		int getLastPhyDataSampled(double &data)
		{
			T tAdc;
			int i;
			i = getLastAdcDataSampled(tAdc);
			data = physicalValue(tAdc);
			return i;
		};

		/**
		Push a sample into the samples' buffer.
		@param adc value to push - adc value.
		@return 1 if the sample was correctly pushed, -1 otherwise. A sample can be not pushed for 2 reason or the channel 
				is disabled or the buffer is full (in this last case the error _HW_CHAN_BUFFER_OVER_FLOW_ is raised)
		@see isEnabled
		@see HwChanKindOfError
		*/
		inline int pushAdcData(T adc)
		{
			if (!m_fifoAdcData.isfull())
			{
				m_lastAdcSampled = adc;
				m_fifoAdcData.put(adc);
				return 1;
			}
			m_hwChanError = _HW_CHAN_BUFFER_OVER_FLOW_;
			m_errorsOccurence++;
		
			return -1;
		};

		inline int pushAdcData_isr(T adc)
		{
			if (!m_fifoAdcData.isfull_isr())
			{
				m_lastAdcSampled = adc;
				m_fifoAdcData.put_isr(adc);
				return 1;
			}
			m_hwChanError = _HW_CHAN_BUFFER_OVER_FLOW_;
			m_errorsOccurence++;
			
			return -1;
		};
		
		inline int pushAdcData(T *adc, int __count)
		{
			m_lastAdcSampled = adc[__count-1];
			if(m_fifoAdcData.write(adc, __count) == 0)
			{
				m_hwChanError = _HW_CHAN_BUFFER_OVER_FLOW_;
				m_errorsOccurence++;
				return -1;
			}
			return __count;
		
		};

		inline int pushAdcData_isr(T *adc, int __count)
		{
			m_lastAdcSampled = adc[__count-1];
			if(m_fifoAdcData.write(adc, __count) == 0)
			{
				m_hwChanError = _HW_CHAN_BUFFER_OVER_FLOW_;
				m_errorsOccurence++;
				return -1;
			}
			return __count;
		
		};
		
		/**
		Pop a sample from the samples' buffer.
		@param adc value popped - adc value.
		@return 1 if the sample was correctly popped, -1 if the buffer is empty.
		*/
		inline int popAdcData(T &adc)
		{
			adc = m_fifoAdcData.get();
			return 1;
		}
		
		inline int popAdcData_isr(T &adc)
		{
			adc = m_fifoAdcData.get_isr();
			return 1;
		}

		inline int popAdcData(T *adc, int __count)
		{
			return m_fifoAdcData.read(adc, __count);
		}

		inline int popAdcData_isr(T *adc, int __count)
		{
			return m_fifoAdcData.read_isr(adc, __count);
		}
		
		/**
		Clear the sample's buffer.
		*/
		void clearAdcFifo()
		{
			m_fifoAdcData.clear();
			m_hwChanError = _HW_CHAN_NO_ERROR_;
		};

		void clearAdcFifo_isr()
		{
			m_fifoAdcData.clear_isr();
			m_hwChanError = _HW_CHAN_NO_ERROR_;
		};
		
		void clearError()
		{
			m_hwChanError = _HW_CHAN_NO_ERROR_;
		}
		
		/**
		Returns the physical value of the channel according to gain, offset and offsetToZero.
		@param adc sample value - adc value
		@return physical value
		@see getGain
		@see setGain
		@see setOffset
		@see getOffset
		@see zero
		*/
		virtual double physicalValue(T adc){return (((long)adc - m_offset - m_offsetToZero)*m_gain);};
		
		/**
		channel zeroing.
		@return 1 if the channels is zeroable, -1 otherwise.
		@see setIsZeroable
		@see getIsZeroable
		*/
		int zero()
		{
			if (m_isZeroable)
			{
				m_offsetToZero = (long)m_lastAdcSampled - m_offset;
				return 1;
			}
			return -1;
		};

		/**
		Resets the zero value the channel.
		*/
		void rstZero(){m_offsetToZero = 0;};

			/**
		@return true if the channel is enable, false otherwise
		*/
		inline bool isEnabled(){ return m_isEnabled;};
		
		/**
		Enabled the channel according to e
		@param e true enable the channel, false to disable.
		*/
		void setEnabled(bool e){ m_isEnabled = true;};
		
		/**
		@return the type of channel
		@see HwChannelType
		*/
		HwChannelType getChannelType(){ return m_chanType;};

		/**
		@return adc resolution according to the bit resolution (i.e. if bit resolution is _8_BITS_RESOLUTION_ this funtion returns 256).
		@see bitResolution
		*/
		double getAdcResolution(){ return m_adcResolution;};

		/**
		@return the value of the adc voltage reference.
		*/
		double getVoltageRefence_mV(){return m_adcVoltageReference_mV;};
		/**
		Sets the value of the ad voltage reference
		@param mV value in mV of the voltage reference.
		*/
		void setVoltageRefernce_mV(double mV){ m_adcVoltageReference_mV = mV;};

		/**
		Returns the value in mV of the voltage sampled by the ADC.
		@param adc sampled value - adc value.
		@return mV sampled.
		*/
		double adcTOmV(T adc){return (double) (m_adcVoltageReference_mV / m_adcResolution)*(double) adc;};

		/**
		@return the error occured.
		@see HwChanKindOfError
		*/
		HwChanKindOfError kindOfError(){return m_hwChanError;};
		/**
		@return the number of error occurences.  
		*/
		dword getNumErrorOccurrence(){ return m_errorsOccurence;};	

		/**
		set the type of channel
		@type kind of channels.
		@see HwChannelType
		*/
		void setChannelType(HwChannelType type){ m_chanType = type;};
		
	protected:
		HwChannelType m_chanType;
		
	private:
		dword m_sampleFrq;
		long m_offset;
		long m_offsetToZero;
		double m_gain;
		double m_adcResolution;
		double m_adcVoltageReference_mV;
		bool m_isZeroable;
		bitResolution m_bitsResolution;
		AdcRingBuffer <T> m_fifoAdcData;
		T m_lastAdcSampled;
		bool m_isEnabled;
		HwChanKindOfError m_hwChanError;
		dword m_errorsOccurence;
};


/**
Impedance hardware channel implementation class.

@param T type of data sampled (byte, word or dword). 
@param capacity dimension of the samples' buffer.
*/
template <class T, int capacity>
class ImpHwChannel: public HwChannel<T, capacity>
{
	public:
		/**
		Class constructor. 
		@param bits adc bits resolution.
		*/
		ImpHwChannel(bitResolution bits = _16_BITS_RESOLUTION_):
			HwChannel<T, capacity>(bits)
		{
			HwChannel<T, capacity>::m_chanType = _HW_CHAN_IMP_TYPE_;
			HwChannel<T, capacity>::setSampleFrq(50);
			HwChannel<T, capacity>::setIsZeroable(false);
			HwChannel<T, capacity>::setOffset(0);
			HwChannel<T, capacity>::setGain(7.043);
			
		};
		/**
		Class desctuctor.
		*/
		~ImpHwChannel(){};

		/**
		Returns the physical value of the channel.
		@param adc sample value - adc value
		@return physical value
		*/
		double physicalValue(T adc)
		{
			double Ves;
			Ves = HwChannel<T, capacity>::adcTOmV(adc) / HwChannel<T, capacity>::getGain();
			return (double)(Ves*20000.0/(1100.0-Ves) - 100.0);
		};
	protected:
		
	private:	
		
};

/**
Pressure hardware channel implementation class.

@param T type of data sampled (byte, word or dword). 
@param capacity dimension of the samples' buffer.
*/
template <class T, int capacity>
class PrsHwChannel: public HwChannel<T, capacity> 
{
	public:
		/**
		Class constructor. 
		@param bits adc bits resolution.
		*/
		PrsHwChannel(bitResolution bits = _16_BITS_RESOLUTION_):
			HwChannel<T, capacity>(bits)
		{
			HwChannel<T, capacity>::m_chanType = _HW_CHAN_PRS_TYPE_;
			HwChannel<T, capacity>::setSampleFrq(100);
			HwChannel<T, capacity>::setIsZeroable(false);
			HwChannel<T, capacity>::setOffset(0);
			HwChannel<T, capacity>::setGain(1);
		};
			/**
			Class descructor.
			*/
		~PrsHwChannel(){};

	protected:
		
	private:	
		
};

/**
Generic hardware channel implementation class.

@param T type of data sampled (byte, word or dword). 
@param capacity dimension of the samples' buffer.
*/
template <class T, int capacity>
class GenericHwChannel: public HwChannel<T, capacity> 
{
	public:
		/**
		Class constructor. 
		@param type kind of channel @see HwChannelType
		@param bits adc bits resolution.
		*/
		GenericHwChannel(HwChannelType type = _HW_CHAN_GENERIC_TYPE_, bitResolution bits = _16_BITS_RESOLUTION_):
			HwChannel<T, capacity>(bits)
		{
			HwChannel<T, capacity>::m_chanType = type;
			HwChannel<T, capacity>::setSampleFrq(100);
			HwChannel<T, capacity>::setIsZeroable(false);
			HwChannel<T, capacity>::setOffset(0);
			HwChannel<T, capacity>::setGain(1);
		};
			/**
			Class descructor.
			*/
		GenericHwChannel(){};

	protected:
		
	private:	
		
};


/**
pH ISFET hardware channel implementation class.

@param T type of data sampled (byte, word or dword). 
@param capacity dimension of the samples' buffer.
*/
template <class T, int capacity>
class pHiHwChannel: public HwChannel<T, capacity> 
{
	public:
		/**
		Class constructor. 
		@param bits adc bits resolution.
		*/
		pHiHwChannel(bitResolution bits = _16_BITS_RESOLUTION_):
			HwChannel<T, capacity>(bits)
		{
			HwChannel<T, capacity>::m_chanType = _HW_CHAN_pHi_TYPE_;
			HwChannel<T, capacity>::setSampleFrq(100);
			HwChannel<T, capacity>::setIsZeroable(false);
			HwChannel<T, capacity>::setOffset(0);
			HwChannel<T, capacity>::setGain(1);
			m_adcTOmVoffset = 0;
			m_adcTOmVgain = 1.0;
		};
		/**
		Class destructor
		*/
		~pHiHwChannel(){};

		/**
		Returns the pH probe's mV value.
		@param adc sample value - adc value
		@return the pH probe's mV value
		@see setAdcTOmVoffset
		@see setAdcTOmVgain
		*/
		double pHprobe_mV(T adc){return (double)(adc - m_adcTOmVoffset) * m_adcTOmVgain;};

		/**
		Sets the offset to calculate the probe's mV value
		@param offset value depending on the pH's read out circuit
		*/
		void setAdcTOmVoffset(long offset){ m_adcTOmVoffset = offset;};
		/**
		@return the offset to calculate the pH probe's mV
		*/
		long getAdcTOmVoffset(){ return m_adcTOmVoffset;};

		/**
		Sets the gain to calculate the probe's mV value
		@param gain value depending on the pH's read out circuit
		*/
		void setAdcTOmVgain(double gain){ m_adcTOmVgain = gain;};
		/**
		@return the gain to calculate the pH probe's mV
		*/
		double getAdcTOmVgain(){ return m_adcTOmVgain;};
		
	protected:
		
	private:
		long m_adcTOmVoffset;
		double m_adcTOmVgain;
};

/**
Power voltage hardware channel implementation class.

@param T type of data sampled (byte, word or dword). 
@param capacity dimension of the samples' buffer.
*/
template <class T, int capacity>
class VoltageHwChannel: public HwChannel<T, capacity> 
{
	public:
		/**
		Class constructor. 
		@param bits adc bits resolution.
		*/
		VoltageHwChannel(bitResolution bits = _10_BITS_RESOLUTION_):
			HwChannel<T, capacity>(bits)
		{
			HwChannel<T, capacity>::m_chanType = _HW_CHAN_POWER_VOLTAGE_TYPE_;
			HwChannel<T, capacity>::setSampleFrq(1);
			HwChannel<T, capacity>::setIsZeroable(false);
			HwChannel<T, capacity>::setOffset(0);
			HwChannel<T, capacity>::setGain(1);
		};
		/**
		Class destructor. 
		*/
		~VoltageHwChannel(){};

	protected:
		
	private:	
		
};

/**
Current consumption hardware channel implementation class.

@param T type of data sampled (byte, word or dword). 
@param capacity dimension of the samples' buffer.
*/
template <class T, int capacity>
class CurrentHwChannel: public HwChannel<T, capacity> 
{
	public:
		/**
		Class constructor. 
		@param bits adc bits resolution.
		*/
		CurrentHwChannel(bitResolution bits = _10_BITS_RESOLUTION_):
			HwChannel<T, capacity>(bits)
		{
			HwChannel<T, capacity>::m_chanType = _HW_CHAN_POWER_CURRENT_TYPE_;
			HwChannel<T, capacity>::setSampleFrq(1);
			HwChannel<T, capacity>::setIsZeroable(false);
			HwChannel<T, capacity>::setOffset(0);
			HwChannel<T, capacity>::setGain(1);
		};
		/**
		Class destructor. 
		*/
		~CurrentHwChannel(){};

	protected:
		
	private:	
		
};

/**
Generic hardware channels manager class.

It implements a manager for generic hardware channels. 
This class does not allocate any  hardware channels so it can NOT be allocated any istances of it. 
It's used as base class for  more specific implementation.


HwChanManager is a template class were T is the of data sampled (byte, word or dword) 
and capacity the dimension of the samples's buffer. 

@param T type of data sampled (byte, word or dword). 
@param capacity dimension of the samples' buffer.
@see HwChannel
*/
template <class T, int capacity>
class HwChanManager
{
	public:
		/**
		Class constructor. 
		@param numChan number of hardware channels
		@param bits adc bits resolution.
		@param frq channels sample frequency.
		*/
		HwChanManager(int numChan, bitResolution bits = _16_BITS_RESOLUTION_, dword frq = 50)
		{
			m_hwChan = NULL;
			//m_hwChan = new HwChannel<T, capacity> *[numChan];
			m_numHwChan = numChan;
			m_numEnabledHwChan = 0;
			m_sampleFrq = frq;
			m_bitsResolution = bits;
			m_adcResolution = 1.0;
			m_voltageRefernce_mV = 3300;
			m_chanType = _HW_CHAN_NO_TYPE_;
		};
		/**
		Class destructor. 
		*/
		~HwChanManager(){};

		/**
		@return the channels' sample frequency 
		*/
		dword getSampleFrq(){ return m_sampleFrq;};

		/**
		@return the channels' bit resolution.
		@see bitResolution
		*/
		bitResolution getBitResolution(){ return m_bitsResolution;};

		/**
		@return adc resolution according to the bit resolution (i.e. if bit resolution is _8_BITS_RESOLUTION_ this funtion returns 256).
		@see bitResolution
		*/
		double getAdcResolution(){ return m_adcResolution;};

		/**
		@return the type of channels - pure virtual function.
		@see HwChannelType
		*/
		virtual HwChannelType getChannelType(){return _HW_CHAN_NO_TYPE_;};

		/**
		@return the value in mV of the adc voltage reference.
		*/
		double getVoltageRefence_mV(){ return m_voltageRefernce_mV;};

		/**
		@return the number of channels
		*/
		int getNumHwChan(){ return m_numHwChan;};

		/**
		Sets the i-th channel as zeroalble according to z
		@param i channel number
		@param true set the i-th channel as zeroable, false as NOT zeroable.
		*/
		void setIsZeroable(int i, bool z)
		{
			if (i < 0 || i >= m_numHwChan)
			{
				return;
			}
			m_hwChan[i]->setIsZeroable(z);
		};
		
		/**
		Returns if the i-th channel is zeroable
		@param i channel number
		@return true if the i-th channel is zeroable, false otherwise.
		*/
		bool getIsZeroable(int i)
		{
			if (i < 0 || i >= m_numHwChan)
			{
				return false;
			}
			return m_hwChan[i]->getIsZeroable();
		};
		
		/**
		Returns the i-th channel's offset
		@param i channel number
		@return i-th offset
		*/
		long getOffset(int i)
		{
			if (i < 0 || i >= m_numHwChan)
			{
				return 0;
			}
			return m_hwChan[i]->getOffset();
		};
		
		/**
		Sets the offset of the i-th channel
		@param i channel number
		@param offset i-th channel offset
		*/
		void setOffset(int i, long offset)
		{
			if (i < 0 || i >= m_numHwChan)
			{
				return;
			}
			m_hwChan[i]->setOffset(offset);
		};

		/**
		Returns the i-th channel's gain
		@param i channel number
		@return i-th gain
		*/
		double getGain(int i)
		{
			if (i < 0 || i >= m_numHwChan)
			{
				return 1.0;
			}
			return m_hwChan[i]->getGain();
		};

		/**
		Sets the gain of the i-th channel
		@param i channel number
		@param offset i-th channel offset
		*/
		void setGain(int i, double gain)
		{
			if (i < 0 || i >= m_numHwChan)
			{
				return;
			}
			m_hwChan[i]->setGain(gain);
		};

		/**
		Returns the i-th channel's physical value
		@param i channel number
		@param adc sample value - adc value
		@return i-th channel physical value
		*/
		double physicalValue(int i, T adc)
		{
			if (i < 0 || i >= m_numHwChan)
			{
				return 0.0;
			}
			return m_hwChan[i]->physicalValue(adc);
		};
		
		/**
		Returns the value in mV of the voltage sampled by the ADC.
		@param i channel number
		@param adc sampled value - adc value.
		@return mV sampled.
		*/
		double adcTOmV(int i, T adc)
        {
            if (i < 0 || i >= m_numHwChan)
            {
                return 0.0;
            }
            return (double) m_hwChan[i]->adcTOmV(adc);
        }

		/**
		i-th channel zeroing.
		@return 1 if the channels is zeroable, -1 otherwise.
		@param i channel number
		@see setIsZeroable
		@see getIsZeroable
		*/
		int zero(int i)
		{
			if (i < 0 || i >= m_numHwChan)
			{
				return -1;
			}
			return m_hwChan[i]->zero();
		}

		/**
		Resets the zero value of the i-th channel
		*/
		void rstZero(int i)
		{
			if (i < 0 || i >= m_numHwChan)
			{
				return;
			}
			m_hwChan[i]->rstZero();
		}
		
		/**
		@return true if the i-th channel is enable, false otherwise
		@see setEnabled
		*/
		bool isEnabled(int i)
		{
			if (i < 0 || i >= m_numHwChan)
			{
				return false;
			}
			return m_hwChan[i]->isEnabled();
		}
		
		/**
		Sets the i-th channel enabled accordint to e value
		@param i channel number
		@param e if true se the i-th channel enabled, disabled otherwise.
		@see isEnabled
		*/
		void setEnabled(int i, bool e)
		{
			if (i < 0 || i >= m_numHwChan)
			{
				return;
			}
			m_hwChan[i]->setEnabled(e);
		}

		/**
		Sets the ADC voltage reference
		@param mV adc voltage reference value in mV 
		*/
		void setVoltageRefernce_mV(double mV)
		{
			int i;
			for (i = 0; i < m_numHwChan; i++)
			{
				m_hwChan[i]->setVoltageRefernce_mV(mV);
			}		
		}

		/**
		Push a sample into the i-th samples' buffer.
		@param i channel number
		@param adc value to push - adc value.
		@return 1 if the sample was correctly pushed, -1 otherwise. A sample can be not pushed for 2 reason or the channel 
				is disabled or the buffer is full (in this last case the error _HW_CHAN_BUFFER_OVER_FLOW_ is raised)
		@see isEnabled
		@see HwChanKindOfError
		*/
		inline int pushAdcDataToChan(int i, T adc)
		{
			if (i < 0 || i >= m_numHwChan)
			{
				return -1;
			}
			return m_hwChan[i]->pushAdcData(adc);
		}

		inline int pushAdcDataToChan_isr(int i, T adc)
		{
			if (i < 0 || i >= m_numHwChan)
			{
				return -1;
			}
			return m_hwChan[i]->pushAdcData_isr(adc);
		}
		
		/**
		Returns the last value sampled from channel i.
		@param i channel number
		@param adc is the last data sampled - adc value.
		@return 1 if i is comprised between 0 and m_numHwChan, -1 otherwise
		*/
		int getLastAdcDataSampledByChan(int i, T &adc)
		{
			if (i < 0 || i >= m_numHwChan)
			{
				return -1;
			}
			return m_hwChan[i]->getLastAdcDataSampled(adc);
		}
		/**
		Returns the last physical value sampled from channel i.
		@param i channel number
		@param adc is the last data sampled - physical value.
		@return 1 if i is comprised between 0 and numChan, -1 otherwise
		*/
		int getLastPhyDataSampledByChan(int i, double &data)
		{
			if (i < 0 || i >= m_numHwChan)
			{
				return -1;
			}
			return m_hwChan[i]->getLastPhyDataSampled(data);
		}

		/**
		Pop samples from the samples' buffers if at least one sample is present in each buffers.
		@param adc pointer to and array of data whose dimension are m_numHwChan.
		@return true if at least num samplea are present in each buffer, false otherwise.
		*/
		inline bool popAdcData (T *adc)
		{
			int i;
			bool readValues = true;
			
			for(i = 0; i < m_numHwChan; i++)
			{
				if (m_hwChan[i]->isSampleBufferEmpty())
				{
					readValues = false;
					break;
				}
			}
			
			if (readValues)
			{
				for(i = 0; i < m_numHwChan; i++)
				{
					m_hwChan[i]->popAdcData(adc[i]);
				}
			}
			
			return readValues;
		}

		inline bool popAdcData_isr (T *adc)
		{
			int i;
			bool readValues = true;
			
			for(i = 0; i < m_numHwChan; i++)
			{
				if (m_hwChan[i]->isSampleBufferEmpty_isr())
				{
					readValues = false;
					break;
				}
			}
			
			if (readValues)
			{
				for(i = 0; i < m_numHwChan; i++)
				{
					m_hwChan[i]->popAdcData_isr(adc[i]);
				}
			}
			
			return readValues;
		}

		inline bool popAdcDataChan (int i, T &adc)
		{
			if ( i < m_numHwChan)
			{
				if (!m_hwChan[i]->isSampleBufferEmpty())
				{
					m_hwChan[i]->popAdcData(adc);
					return true;			
				}
			}
			return false;
		}

		inline bool popAdcDataChan_isr (int i, T &adc)
		{
			if ( i < m_numHwChan)
			{
				if (!m_hwChan[i]->isSampleBufferEmpty())
				{
					m_hwChan[i]->popAdcData_isr(adc);
					return true;			
				}
			}
			return false;
		}
		/**
		Pops samples from the samples' buffers if at least n samples are present in each buffers.

		The adc array contains num times the samples.
		@param adc pointer to and array of data whose dimension are m_numHwChan.
		@param num number of samples that have to present in each buffer
		@return true if at least one sample is present in each buffer, false otherwise.
		*/
		inline bool popAdcData (T *adc, int __count)
		{
			int i;
			bool readValues = true;
			
			for(i = 0; i < m_numHwChan; i++)
			{
				if (m_hwChan[i]->getNumDataBuffered() < __count)
				{
					readValues = false;
					break;
				}
			}
			if (readValues)
			{
				for(i = 0; i < m_numHwChan; i++)
				{	
					m_hwChan[i]->popAdcData(&adc[__count*i], __count);	
				}
			}
			
			
			return readValues;
		};

		inline bool popAdcData_isr (T *adc, int __count)
		{
			int i;
			bool readValues = true;
			
			for(i = 0; i < m_numHwChan; i++)
			{
				if (m_hwChan[i]->getNumDataBuffered_isr() < __count)
				{
					readValues = false;
					break;
				}
			}
			if (readValues)
			{
				for(i = 0; i < m_numHwChan; i++)
				{	
					m_hwChan[i]->popAdcData_isr(&adc[__count*i], __count);	
				}
			}
			
			return readValues;
		}
		
		inline bool popAdcDataChan (int i, T *adc, int __count)
		{
			if( i < m_numHwChan)
			{
				if (m_hwChan[i]->getNumDataBuffered() >=__count)
				{
					m_hwChan[i]->popAdcData(adc, __count);	
					return true;
				}
			}
			return false;	
		}

		inline bool popAdcDataChan_isr (int i, T *adc, int __count)
		{
			if( i < m_numHwChan)
			{
				if (m_hwChan[i]->getNumDataBuffered_isr() >=__count)
				{
					m_hwChan[i]->popAdcData_isr(adc, __count);	
					return true;
				}
			}
			return false;	
		}
		
		/**
		Clears all the samples's buffers.
		*/
		void clearFifo()
		{
			int i;
			
			for (i = 0; i < m_numHwChan; i++)
			{
				m_hwChan[i]->clearAdcFifo();
			}
		
		}

		void clearFifo_isr()
		{
			int i;
			
			for (i = 0; i < m_numHwChan; i++)
			{
				m_hwChan[i]->clearAdcFifo_isr();
			}
		
		}
		
		void clearError()
		{
			int i;
			for (i = 0; i < m_numHwChan; i++)
				{
					m_hwChan[i]->clearAdcFifo();
				}
		}
		
		/**
		Returns the number of samples present in the i-th buffer
		@param i channel number
		@return the number of samples buffered from the i-th channel.
		*/
		int getNumDataFifoed(int i){return m_hwChan[i]->getNumDataBuffered();};
		int getNumDataFifoed_isr(int i){return m_hwChan[i]->getNumDataBuffered_isr();};
		/**
		Returns if the i-th bufffer is empty
		@param i channel number
		@return true is the i-th buffer is empty, false othewise
		*/
		bool isFifoEmpty(int i){return m_hwChan[i]->isSampleBufferEmpty();};
		bool isFifoEmpty_isr(int i){return m_hwChan[i]->isSampleBufferEmpty_isr();};
		/**
		Returns if the i-th bufffer is full
		@param i channel number
		@return true is the i-th buffer is full, false othewise
		*/
		bool isFifoFull(int i){return m_hwChan[i]->isSampleBufferFull();};		
		bool isFifoFull_isr(int i){return m_hwChan[i]->isSampleBufferFull_isr();};
		/**
		Returns the error of the hardware channels
		@param errorType pointer to and array of data whose dimension are m_numHwChan. This array contains the error for each channel
		@return true if an error occured at least in one channel, false if no errors occurred in any channel.
		@see HwChanKindOfError
		*/
		bool HwChanManagerError(HwChanKindOfError *errorType)
		{
			int i;
			bool retVal = false;
			for (i = 0; i < m_numHwChan; i++)
			{
				errorType[i] = m_hwChan[i]->kindOfError();
				if (errorType[i] != _HW_CHAN_NO_ERROR_)
				{
					retVal = true;
				}
			}
			return retVal;
		};
		/**
		Returns the error of the hardware channels
		@return true if an error occured at least in one channel, false if no errors occurred in any channel.
		*/
		bool HwChanManagerError()
		{
			int i;
			HwChanKindOfError errorType;
			for (i = 0; i < m_numHwChan; i++)
			{
				errorType = m_hwChan[i]->kindOfError();
				if (errorType != _HW_CHAN_NO_ERROR_)
				{
					return true;
				}
			}
			return false;
		};

		/**
		Returns the number of errors' occurrences in the i-th channel
		@param i channel number
		@return number of errors' occurrences in the i-th channel
		*/
		dword getNumErrorOccurrence(int i)
		{
			if (i < 0 || i >= m_numHwChan)
			{
				return -1;
			}
			return m_hwChan[i]->getNumErrorOccurrence();
		};	
		
	protected:
		HwChannelType m_chanType;
		HwChannel<T, capacity> **m_hwChan;
		
		/**
		Attribute than holds the number of channels.
		*/
		word m_numHwChan;
		word m_numEnabledHwChan;
		double m_adcResolution;
		double m_voltageRefernce_mV;
		
	private:	
		dword m_sampleFrq;
		bitResolution m_bitsResolution;
		
};


/**
Impedance hardware channels manager class.

@param T type of data sampled (byte, word or dword). 
@param capacity dimension of the samples' buffer.
@see HwChannel
*/
template <class T, int capacity>
class ImpHwChanManager: public HwChanManager<T, capacity> 
{
	public:
		/**
		Class constructor. 
		@param numChan number of hardware channels
		@param bits adc bits resolution.
		@param frq channels sample frequency.
		*/
		ImpHwChanManager<T, capacity>(int numChan, bitResolution bits = _16_BITS_RESOLUTION_, dword frq = 50):
			HwChanManager<T, capacity>(numChan, bits, frq)
		{
			int i;
			if (numChan < 1)
				numChan = 1;
			(HwChanManager<T, capacity>::m_hwChan) = (HwChannel<T, capacity>**)new ImpHwChannel<T, capacity> *[numChan];
			for (i = 0; i < numChan; i++)
			{
				HwChanManager<T, capacity>::m_hwChan[i] = (HwChannel<T, capacity>*)new ImpHwChannel<T, capacity>(bits);
				HwChanManager<T, capacity>::m_hwChan[i]->setSampleFrq(frq);
				HwChanManager<T, capacity>::m_hwChan[i]->setVoltageRefernce_mV(HwChanManager<T, capacity>::m_voltageRefernce_mV);
				HwChanManager<T, capacity>::m_adcResolution = HwChanManager<T, capacity>::m_hwChan[i]->getAdcResolution();
			}
		};
		/**
		Class destructor. 
		*/	
		~ImpHwChanManager(){
			int i;
			if ( HwChanManager<T, capacity>::m_hwChan)
			{
				for (i = 0; i < HwChanManager<T, capacity>::m_numHwChan; i++)
				{
					delete HwChanManager<T, capacity>::m_hwChan[i];
				}
				delete HwChanManager<T, capacity>::m_hwChan;
				HwChanManager<T, capacity>::m_hwChan = NULL;
			}
		};

		/**
		@return the type of channels.
		@see HwChannelType
		*/
		HwChannelType getChannelType(){return ((ImpHwChannel<T, capacity>*)(HwChanManager<T, capacity>::m_hwChan[0]))->getChannelType();};
		
	protected:

	private:
		
};


/**
Pressure hardware channels manager class.

@param T type of data sampled (byte, word or dword). 
@param capacity dimension of the samples' buffer.
@see HwChannel
*/
template <class T, int capacity>
class PrsHwChanManager: public HwChanManager<T, capacity> 
{
	public:
		/**
		Class constructor. 
		@param numChan number of hardware channels
		@param bits adc bits resolution.
		@param frq channels sample frequency.
		*/
		PrsHwChanManager(int numChan, bitResolution bits = _16_BITS_RESOLUTION_, dword frq = 100):
			HwChanManager<T, capacity>(numChan, bits, frq)
		{
			int i;
			if (numChan < 1)
				numChan = 1;
			HwChanManager<T, capacity>::m_hwChan = (HwChannel<T, capacity>**)new PrsHwChannel<T, capacity> *[numChan];
			for (i = 0; i < numChan; i++)
			{
				HwChanManager<T, capacity>::m_hwChan[i] = (HwChannel<T, capacity>*)new PrsHwChannel<T, capacity>(bits);
				HwChanManager<T, capacity>::m_hwChan[i]->setSampleFrq(frq);
				HwChanManager<T, capacity>::m_hwChan[i]->setVoltageRefernce_mV(HwChanManager<T, capacity>::m_voltageRefernce_mV);
				HwChanManager<T, capacity>::m_adcResolution = HwChanManager<T, capacity>::m_hwChan[i]->getAdcResolution();
			}
		};
		/**
		Class destructor. 
		*/	
		~PrsHwChanManager()
		{
			int i;
			if ( HwChanManager<T, capacity>::m_hwChan)
			{
				for (i = 0; i < HwChanManager<T, capacity>::m_numHwChan; i++)
				{
					delete HwChanManager<T, capacity>::m_hwChan[i];
				}
				delete HwChanManager<T, capacity>::m_hwChan;
				HwChanManager<T, capacity>::m_hwChan = NULL;
			}
		};

		/**
		@return the type of channels.
		@see HwChannelType
		*/
		HwChannelType getChannelType(){return ((PrsHwChannel<T, capacity>*)(HwChanManager<T, capacity>::m_hwChan[0]))->getChannelType();};
			
	protected:

	private:
		
};

/**
Generic hardware channels manager class.

@param T type of data sampled (byte, word or dword). 
@param capacity dimension of the samples' buffer.
@see HwChannel
*/
template <class T, int capacity>
class GenericHwChanManager: public HwChanManager<T, capacity> 
{
	public:
		/**
		Class constructor. 
		@param type kind of channel @see HwChannelType
		@param numChan number of hardware channels
		@param bits adc bits resolution.
		@param frq channels sample frequency.
		*/
		GenericHwChanManager(int numChan, HwChannelType type, bitResolution bits = _16_BITS_RESOLUTION_, dword frq = 100):
			HwChanManager<T, capacity>(numChan, bits, frq)
		{
			int i;
			if (numChan < 1)
				numChan = 1;
			HwChanManager<T, capacity>::m_hwChan = (HwChannel<T, capacity>**)new GenericHwChannel<T, capacity> *[numChan];
			for (i = 0; i < numChan; i++)
			{
				HwChanManager<T, capacity>::m_hwChan[i] = (HwChannel<T, capacity>*)new GenericHwChannel<T, capacity>(type, bits);
				HwChanManager<T, capacity>::m_hwChan[i]->setSampleFrq(frq);
				HwChanManager<T, capacity>::m_hwChan[i]->setVoltageRefernce_mV(HwChanManager<T, capacity>::m_voltageRefernce_mV);
				HwChanManager<T, capacity>::m_adcResolution = HwChanManager<T, capacity>::m_hwChan[i]->getAdcResolution();
			}
		};
		/**
		Class destructor. 
		*/	
		~GenericHwChanManager()
		{
			int i;
			if ( HwChanManager<T, capacity>::m_hwChan)
			{
				for (i = 0; i < HwChanManager<T, capacity>::m_numHwChan; i++)
				{
					delete HwChanManager<T, capacity>::m_hwChan[i];
				}
				delete HwChanManager<T, capacity>::m_hwChan;
				HwChanManager<T, capacity>::m_hwChan = NULL;
			}
		};

		/**
		sets the type of the i-th channels
		@param i channel number
		@param type kind of channel @see HwChannelType
		*/
		void setChanType(int i, HwChannelType type)
		{
			if(i >= 0 || i < HwChanManager<T, capacity>::m_numHwChan)
			{
				((GenericHwChannel<T, capacity>*)(HwChanManager<T, capacity>::m_hwChan[0]))->setChannelType(type);
			}
		}

		/**
		@return the type of channels. if i less or equal than 0 or i bigger than m_numHwChan, return HwChannelType::_HW_CHAN_NO_TYPE_
		@see HwChannelType
		*/
		HwChannelType getChannelType(int i)
		{
			if(i >= 0 || i < HwChanManager<T, capacity>::m_numHwChan)
			{
				return ((GenericHwChannel<T, capacity>*)(HwChanManager<T, capacity>::m_hwChan[0]))->getChannelType();
			}
			return _HW_CHAN_NO_TYPE_;
		}
	protected:

	private:
		
};


/**
pH ISFET hardware channels manager class.

@param T type of data sampled (byte, word or dword). 
@param capacity dimension of the samples' buffer.
@see HwChannel
*/
template <class T, int capacity>
class pHiHwChanManager: public HwChanManager<T, capacity> 
{
	public:
		/**
		Class constructor. 
		@param numChan number of hardware channels
		@param bits adc bits resolution.
		@param frq channels sample frequency.
		*/
		pHiHwChanManager(int numChan, bitResolution bits = _16_BITS_RESOLUTION_, dword frq = 100):
			HwChanManager<T, capacity>(numChan, bits, frq)
		{
			int i;
			if (numChan < 1)
				numChan = 1;
			HwChanManager<T, capacity>::m_hwChan = (HwChannel<T, capacity>**)new pHiHwChannel<T, capacity> *[numChan];
			for (i = 0; i < numChan; i++)
			{
				HwChanManager<T, capacity>::m_hwChan[i] = (HwChannel<T, capacity>*)new pHiHwChannel<T, capacity>(bits);
				HwChanManager<T, capacity>::m_hwChan[i]->setSampleFrq(frq);
				HwChanManager<T, capacity>::m_hwChan[i]->setVoltageRefernce_mV(HwChanManager<T, capacity>::m_voltageRefernce_mV);
				HwChanManager<T, capacity>::m_adcResolution = HwChanManager<T, capacity>::m_hwChan[i]->getAdcResolution();
			}
		};
		/**
		Class destructor. 
		*/	
		~pHiHwChanManager()
		{
			int i;
			if ( HwChanManager<T, capacity>::m_hwChan)
			{
				for (i = 0; i < HwChanManager<T, capacity>::m_numHwChan; i++)
				{
					delete HwChanManager<T, capacity>::m_hwChan[i];
				}
				delete HwChanManager<T, capacity>::m_hwChan;
				HwChanManager<T, capacity>::m_hwChan = NULL;
			}
		};
		/**
		Sets the offset to calculate the i-th probe's mV value
		@param i channel number
		@param offset value depending on the pH's read out circuit
		*/
		void setAdcTOmVoffset(int i, long offset)
        {
            if(i < HwChanManager<T, capacity>::getNumHwChan() && i >= 0)
            {
                ((pHiHwChannel<T, capacity>*)(HwChanManager<T, capacity>::m_hwChan[i]))->setAdcTOmVoffset(offset);
            }
           
        };
		/**
		@param i channel number
		@return the offset to calculate the i-th pH probe's mV
		*/
        long getAdcTOmVoffset(int i)
        {
            if(i < HwChanManager<T, capacity>::getNumHwChan() && i >= 0)
            {
                return ((pHiHwChannel<T, capacity>*)(HwChanManager<T, capacity>::m_hwChan[i]))->getAdcTOmVoffset();
            }
            return 0;
        };
		/**
		Sets the gain to calculate the i-th probe's mV value
		@param i channel number
		@param gain value depending on the pH's read out circuit
		*/
        void setAdcTOmVgain(int i, double gain)
        {
            if(i < HwChanManager<T, capacity>::getNumHwChan() && i >= 0)
            {
                ((pHiHwChannel<T, capacity>*)(HwChanManager<T, capacity>::m_hwChan[i]))->setAdcTOmVgain(gain);
            }    
        };
		/**
		@param i channel number
		@return the gain to calculate the i-th pH probe's mV
		*/
        double getAdcTOmVgain(int i)
        {
            if(i < HwChanManager<T, capacity>::getNumHwChan() && i >= 0)
            {
                return ((pHiHwChannel<T, capacity>*)(HwChanManager<T, capacity>::m_hwChan[i]))->getAdcTOmVgain();
            }
            return 1.0;
        };
        /**
		@return the i-th pH probe's mV value
		@param i channel number
		@param adc sample value - adc value
		@see setAdcTOmVoffset
		@see setAdcTOmVgain
		*/
        double pHprobe_mV(int i, T adc)
        {
            if(i < HwChanManager<T, capacity>::getNumHwChan() && i >= 0)
            {
                return ((pHiHwChannel<T, capacity>*)(HwChanManager<T, capacity>::m_hwChan[i]))->pHprobe_mV(adc);
            }
            return 0.0;
        }

		/**
		@return the type of channels.
		@see HwChannelType
		*/
		HwChannelType getChannelType(){return ((pHiHwChannel<T, capacity>*)(HwChanManager<T, capacity>::m_hwChan[0]))->getChannelType();};
		
	protected:

	private:
		
};

/**
Power Voltage hardware channels manager class.

@param T type of data sampled (byte, word or dword). 
@param capacity dimension of the samples' buffer.
@see HwChannel
*/
template <class T, int capacity>
class VoltageHwChanManager: public HwChanManager<T, capacity> 
{
	public:
		/**
		Class constructor. 
		@param numChan number of hardware channels
		@param bits adc bits resolution.
		@param frq channels sample frequency.
		*/
		VoltageHwChanManager(int numChan, bitResolution bits = _10_BITS_RESOLUTION_, dword frq = 1):
			HwChanManager<T, capacity>(numChan, bits, frq)
		{
			int i;
			if (numChan < 1)
				numChan = 1;
			HwChanManager<T, capacity>::m_hwChan = (HwChannel<T, capacity>**)new VoltageHwChannel<T, capacity> *[numChan];
			for (i = 0; i < numChan; i++)
			{
				HwChanManager<T, capacity>::m_hwChan[i] = (HwChannel<T, capacity>*)new VoltageHwChannel<T, capacity>(bits);
				HwChanManager<T, capacity>::m_hwChan[i]->setSampleFrq(frq);
				HwChanManager<T, capacity>::m_hwChan[i]->setVoltageRefernce_mV(HwChanManager<T, capacity>::m_voltageRefernce_mV);
				HwChanManager<T, capacity>::m_adcResolution = HwChanManager<T, capacity>::m_hwChan[i]->getAdcResolution();
			}
		};
		/**
		Class destructor. 
		*/	
		~VoltageHwChanManager()
		{
			int i;
			if ( HwChanManager<T, capacity>::m_hwChan)
			{
				for (i = 0; i < HwChanManager<T, capacity>::m_numHwChan; i++)
				{
					delete HwChanManager<T, capacity>::m_hwChan[i];
				}
				delete HwChanManager<T, capacity>::m_hwChan;
				HwChanManager<T, capacity>::m_hwChan = NULL;
			}
		};
		
		/**
		@return the type of channels.
		@see HwChannelType
		*/
		HwChannelType getChannelType(){return ((VoltageHwChannel<T, capacity>*)(HwChanManager<T, capacity>::m_hwChan[0]))->getChannelType();};
	protected:

	private:
		
};

/**
Current consumption hardware channels manager class.

@param T type of data sampled (byte, word or dword). 
@param capacity dimension of the samples' buffer.
@see HwChannel
*/
template <class T, int capacity>
class CurrentHwChanManager: public HwChanManager<T, capacity> 
{
	public:
		/**
		Class constructor. 
		@param numChan number of hardware channels
		@param bits adc bits resolution.
		@param frq channels sample frequency.
		*/
		CurrentHwChanManager(int numChan, bitResolution bits = _10_BITS_RESOLUTION_, dword frq = 1):
			HwChanManager<T, capacity>(numChan, bits, frq)
		{
			int i;
			if (numChan < 1)
				numChan = 1;
			HwChanManager<T, capacity>::m_hwChan = (HwChannel<T, capacity>**)new CurrentHwChannel<T, capacity> *[numChan];
			for (i = 0; i < numChan; i++)
			{
				HwChanManager<T, capacity>::m_hwChan[i] = (HwChannel<T, capacity>*)new CurrentHwChannel<T, capacity>(bits);
				HwChanManager<T, capacity>::m_hwChan[i]->setSampleFrq(frq);
				HwChanManager<T, capacity>::m_hwChan[i]->setVoltageRefernce_mV(HwChanManager<T, capacity>::m_voltageRefernce_mV);
				HwChanManager<T, capacity>::m_adcResolution = HwChanManager<T, capacity>::m_hwChan[i]->getAdcResolution();
			}
		};
		/**
		Class destructor. 
		*/	
		~CurrentHwChanManager()
		{
			int i;
			if ( HwChanManager<T, capacity>::m_hwChan)
			{
				for (i = 0; i < HwChanManager<T, capacity>::m_numHwChan; i++)
				{
					delete HwChanManager<T, capacity>::m_hwChan[i];
				}
				delete HwChanManager<T, capacity>::m_hwChan;
				HwChanManager<T, capacity>::m_hwChan = NULL;
			}
		};

		/**
		@return the type of channels.
		@see HwChannelType
		*/
		HwChannelType getChannelType(){return ((CurrentHwChannel<T, capacity>*)(HwChanManager<T, capacity>::m_hwChan[0]))->getChannelType();};
		
	protected:

	private:
		
};



/**
@see GenericHwChanManager
*/
extern GenericHwChanManager <long, _WEIGHT_BUFFER_LENGTH_> *weightChan;
/**
@see PrsHwChanManager
*/
extern PrsHwChanManager <short, _PRS_BUFFER_LENGTH_> *prsChan;

/**
This can be any kind of channels. 
SAU7000 supports UEC channels and EMGZ
@see GenericHwChanManager
*/
//extern GenericHwChanManager <word, _GENERIC_BUFFER_LENGTH_> *genericChan;

/**
High Resolution EMG. 
@see GenericHwChanManager
*/
//extern GenericHwChanManager <word, _HR_EMG_BUFFER_LENGTH_> *hrEmgChan;



#endif
























































