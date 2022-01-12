/**
@file		QueueUart.cpp
@brief		Classes to manage the microntroller UART.

@author		Nicola
@date		18/01/2011
@version	01.1
*/


#include "UartAbstract.h"

//
// Definizione della classe UartRingBuffer
//



/**
Class constructor
@param size size of the buffer in byte
*/
UartRingBuffer::UartRingBuffer(int __size)
{
	// + Crea il buffer circolare e lo inizializza
	alloc(__size);
}


/**
Class constructor
@param none, default static constructor
*/
UartRingBuffer::UartRingBuffer()
{
	// + Crea il buffer circolare e lo inizializza
	m_pBuf = NULL;
	m_size = 0;				// buffer size annullato
	zeroIndex();
}



/**
Class desctructor
*/
UartRingBuffer::~UartRingBuffer()
{
	// + Distrugge il buffer circolare
	
	if( m_pBuf )
	{
		destroy();
	}
}


// ridimensiona il buffer di lavoro
int UartRingBuffer::resize(int __newsize)
{
	if( m_pBuf )
	{
		destroy();
	}

	alloc(__newsize);

	return m_pBuf != NULL;
}

void UartRingBuffer::alloc(int __size)
{
	m_pBuf = new char[__size];		// buffer dati allocato in dinamica

	if( m_pBuf != NULL )
		m_size = __size;		// buffer size paddato 
	else
		m_size = 0;				// buffer size annullato

	zeroIndex();
}

void UartRingBuffer::destroy()
{
	delete [] m_pBuf;
	m_pBuf = NULL;
	m_size = 0;
	zeroIndex();
}

void UartRingBuffer::zeroIndex()
{
	m_count = 0;				// counter per ? profiling ? boh 
	m_idxMem = 0;				// indice dentro
	m_idxGet = 0;				// indice fuori	
	m_idxGetRound = 0;
	m_idxMemRound = 0;
}


int UartRingBuffer::read(char *__pBuf, int __count)
{
int delta = 0;
int bufused = used();

	if( m_pBuf == NULL )
		return 0;
	
	//if( bufused == 0 )
		//return 0;
	
	if( bufused < __count )					// se tutto non ci sta...
		return 0; //__count = bufused;					// ritorniamo quello che c'è

	if( (m_idxGet + __count) <= m_size )		// se ci sta in un sol fiato
	{
		memcpy(__pBuf, m_pBuf + m_idxGet, __count);	// copia tutto

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

		memcpy(__pBuf, m_pBuf + m_idxGet, delta);	// copia il primo pezzo

		m_idxGetRound++;
		m_idxGet = 0;
		
		memcpy( __pBuf + delta, m_pBuf,__count);	// copia il resto

		m_idxGet += __count;	// avanti l'indice di copia
	}

	return delta + __count;

}

int UartRingBuffer::read_isr(char *__pBuf, int __count)
{
int delta = 0;
int bufused = used_isr();

	if( m_pBuf == NULL )
		return 0;
	
	//if( bufused == 0 )
		//return 0;
	
	if( bufused < __count )					// se tutto non ci sta...
		return 0; //__count = bufused;					// ritorniamo quello che c'è

	if( (m_idxGet + __count) <= m_size )		// se ci sta in un sol fiato
	{
		memcpy(__pBuf, m_pBuf + m_idxGet, __count);	// copia tutto

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

		memcpy(__pBuf, m_pBuf + m_idxGet, delta);	// copia il primo pezzo

		m_idxGetRound++;
		m_idxGet = 0;
		
		memcpy( __pBuf + delta, m_pBuf,__count);	// copia il resto

		m_idxGet += __count;	// avanti l'indice di copia
	}

	return delta + __count;

}

int UartRingBuffer::write(const char *__pBuf, int __count)
{
int delta = 0;
int bufused = used();

	if( m_pBuf == NULL )
		return 0;
	
	if( m_size - bufused < __count )		// se tutto non ci sta...
		__count = m_size - bufused;		// ci limitiamo a riempirlo

	if( (m_idxMem + __count) <= m_size )		// se ci sta in un sol fiato
	{
		memcpy( m_pBuf + m_idxMem, __pBuf,__count);	// copia tutto

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

		memcpy(m_pBuf + m_idxMem, __pBuf, delta);	// copia il primo pezzo

		m_idxMemRound++;
		m_idxMem = 0;		// va a capo

		memcpy( m_pBuf, __pBuf + delta,__count);	// copia il resto

		m_idxMem += __count;	// avanti l'indice di copia
	}
	return __count + delta;

}

int UartRingBuffer::write_isr(const char *__pBuf, int __count)
{
int delta = 0;
int bufused = used_isr();

	if( m_pBuf == NULL )
		return 0;
	
	if( m_size - bufused < __count )		// se tutto non ci sta...
		return 0;				// ci limitiamo a riempirlo

	if( (m_idxMem + __count) <= m_size )		// se ci sta in un sol fiato
	{
		memcpy( m_pBuf + m_idxMem, __pBuf,__count);	// copia tutto

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

		memcpy(m_pBuf + m_idxMem, __pBuf, delta);	// copia il primo pezzo

		m_idxMemRound++;
		m_idxMem = 0;		// va a capo
		
		memcpy( m_pBuf, __pBuf + delta,__count);	// copia il resto

		m_idxMem += __count;	// avanti l'indice di copia
	}
	return __count + delta;

}


int UartRingBuffer::puts( char *s )
{
int len = 0;

	if( m_pBuf == NULL )
		return 0;

	while( *s != 0 )
		putc(*s++), len++;

	return len;
}

int UartRingBuffer::puts_isr( char *s )
{
int len = 0;

	if( m_pBuf == NULL )
		return 0;

	while( *s != 0 )
		putc_isr(*s++), len++;

	return len;
}


int UartRingBuffer::used()
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

int UartRingBuffer::used_isr()
{
int bufused;

register int idxMem = m_idxMem;
register int idxMemRound = m_idxMemRound;

register int idxGet = m_idxGet;
register int idxGetRound = m_idxGetRound;
	
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




//
// Definizione della classe UartDeviceAbstract
//


/**
Class constructor
@param device UART  device
*/
UartDeviceAbstract::UartDeviceAbstract(int __rxbuf_size, int __txbuf_size) : 
									 m_txbuf(__txbuf_size), m_rxbuf(__rxbuf_size)
{
	// + Seleziona l'insieme di registri corretto in relazione alla UART da usare
	// + Abilita la funzionalità UART sui piedini di I/O corrispondenti
	// + Abilita l'interrupt di trasmissione con livello di priorità 7

	m_LastError = NoErrorUART;

	m_Parity = ParityNone;		// settaggi 
	m_FrameBits = Frame8Bits;
	m_StopBits = Stop1Bits;
	m_BaudRate = BR115200;
	m_hwFlwCnt = hwFlwCtl_None;
	
	m_RunningTransmission = 0;
	m_TransmitBufferOverflow = 0;
	m_ReceptionBufferOverflow = 0;

}

/**
Class desctructor
*/
UartDeviceAbstract::~UartDeviceAbstract()
{
	TurnOff();

}


/**
Set the Parity of the UART protocol. 
@param parity king of parity
@param forcemodify if true the property value is overwritten also if equal to the previous one.
*/
bool UartDeviceAbstract::SetParity(UARTParity parity, bool forcemodify)
{

	if(parity != m_Parity || forcemodify) 
	{
		m_Parity = parity;
		return true;
	}
	return true;

}

/**
Set the number of bits per frame. 
@param framebits number of bit per frame
@param forcemodify if true the property value is overwritten also if equal to the previous one.
*/
bool UartDeviceAbstract::SetFrameBits(UARTFrameBits framebits, bool forcemodify)
{

	if(framebits != m_FrameBits || forcemodify) 
	{
		m_FrameBits = framebits;
		return true;
	}
	return false;
}

/**
Set the number of stop bits. 
@param stopbits number of stop bits
@param forcemodify if true the property value is overwritten also if equal to the previous one.
*/
bool UartDeviceAbstract::SetStopBits(UARTStopBits stopbits, bool forcemodify)
{

	if(stopbits != m_StopBits || forcemodify) 
	{
		m_StopBits = stopbits;
		return true;
	}
	return false;
}


/**
Set the buad rate. 
@param baudrate
@param forcemodify if true the property value is overwritten also if equal to the previous one.
*/
bool UartDeviceAbstract::SetBaudRate(UARTBaudRate baudrate, bool forcemodify)
{

	if(baudrate != m_BaudRate || forcemodify) 
	{
		m_BaudRate = baudrate;
		return true;
	}
	return false;
}


/**
Set the flow control. 
@param flwCtl
@param forcemodify if true the property value is overwritten also if equal to the previous one.
*/
bool UartDeviceAbstract::SetHwFlwCtl(UARThwFlwCnt flwCtl, bool forcemodify)
{
	if(flwCtl != m_hwFlwCnt|| forcemodify) 
	{
		m_hwFlwCnt = flwCtl;
		return true;
	}
	return false;
}

/**
Transmits a data.

- Locks the transmission disabling the interrupts.
- if the fifo is empty the data is immediately transmitted
- if the fifo is not empry the dato is pushed into the fifo.
- if the fifo is full sets the transmitt buffer overflow error
*/
void UartDeviceAbstract::TransmitData(byte data)
{
	
	if( !m_txbuf.isfull() )
	{
//		asm("di");
		m_txbuf.putc(data);
//		asm("ei");
	}
	else
	{
		m_TransmitBufferOverflow = true;
	}

}

/**
Transmits count data from data. 
*/
void UartDeviceAbstract::TransmitData(const byte *pdata, int count)
{
int written;
/*
	for(written=0; written<count; written++)
		m_txbuf.putc(*pdata++);
*/


	// + Trasmette una sequenza di dati di lunghezza count
	written = m_txbuf.write((const char *) pdata, count);

	if( written != count )
		m_TransmitBufferOverflow = true;

}

/**
Transmits the pdata string. 
*/
void UartDeviceAbstract::TransmitString(const char *pdata)
{

	// + Trasmette una stringa terminata con zero
	TransmitData( (const byte *) pdata, strlen(pdata));
}


/**
Clear the transmission fifo
*/
void UartDeviceAbstract::ClearTransmissionBuffer()
{
	// + Svuota la coda di ricezione	
	m_txbuf.clear();
	m_RunningTransmission = false;
}


/**
if the reception fifo is not empty pops a data from the fifo.
@param data data read.
@return true if the fifo is not empty, false otherwise
*/
bool UartDeviceAbstract::ReceiveData(char &data)
{
	// + Se la coda di ricezione non è vuota, estrae un elemento e lo mette in data
	// + Se la coda di ricezione è vuota, ritorna false
	
	bool dataIsReceived = false;

	if( !m_rxbuf.isempty() )
	{
		data = m_rxbuf.getc();
		dataIsReceived = true;
	}
	
	return dataIsReceived;
}

/**
Tries to read count elements from the reception fifo. 
@param pdata array of the read bytes
@param count number of unsigned bytes
@return true if count elements were read false otherwise. Anyway count contains the number of bytes read.
*/
bool UartDeviceAbstract::ReceiveData(byte *pdata, int &count)
{
	// + Tenta di estrarre count elementi dalla coda di ricezione
	// + Se non esistono count elementi nella coda di ricezione, ritorna false
	// + In ogni caso, count contiene in uscita il numero di elementi estratti dalla coda di ricezione


	bool dataIsReceived = false;

	//ReceiveLock();
	int counter = m_rxbuf.read( (char *) pdata, count);

	if( counter == count )			// come richiesto ?
		dataIsReceived = true;		// ok
	
	count = counter;				// copia per i posteri
	
	return dataIsReceived;	
}



/**
Creal the reception fifo.
*/
void UartDeviceAbstract::ClearReceptionBuffer()
{
	// + Svuota la coda di ricezione
	asm("di");
	m_rxbuf.clear();
	asm("ei");
}

void UartDeviceAbstract::usec_delay(int __req_delay)
{
	for(int i = 0; i< __req_delay; i++)
		asm("nop");

}

/**
Clears the transmission and reception fifo

void UartDeviceAbstract::resetFifo()
{
	ClearReceptionBuffer();
	ClearTransmissionBuffer();

}

*/

