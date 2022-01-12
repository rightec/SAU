/**
@file		swtim.h
@brief		Classes to manage software timers.

@author		
@date		18/01/2011
@version	01.0
*/


//--------------------------------------------------------//
// Definizione delle costanti
//--------------------------------------------------------//


//--------------------------------------------------------//
// Definizione dei tipi
//--------------------------------------------------------//
#ifndef _SWTIM_H_
#define _SWTIM_H_

#include "global.h"

/**
Elapsed time.
*/
struct MilliSecTime{                
    dword   elapsed;                /**< elapsed time in msec*/
    word	usec;					/**< micro sec. elapsed within the msec.*/
};


//typedef struct MilliSecTime MilliSecTime;

//--------------------------------------------------------//
// Definizione delle classi 
//--------------------------------------------------------//


/**
Software Timer base class.
It is the time base for all the others timers.
*/
class Timer{

public:

	Timer(void);

	/**
	Increment the elapsed time
	@param msec elapsed time increment in msec
	@param usec micro sec. elapsed within the current msec.
	*/
	void clockTick(dword msec, word usec) { sysTimer.elapsed += msec, sysTimer.usec = usec; }

	/**
	@return the elapsed time in sec.
	*/
    double getTime(void) { return( (double) sysTimer.elapsed / 1000.0 ); }

	/**
	@return the elapsed time in msec.
	*/
    dword getMsec(void) { return( sysTimer.elapsed ); }

private:
	MilliSecTime sysTimer;    // orologio interno (system clock)
};

/**
Global time. Time Base for all the timers.
*/
extern Timer globalTimer;


/**
Incremental Timer
*/
class IncTimer
{

public:
	/**
	Class constructor
	*/
    IncTimer(void) { startTime = latchTime = globalTimer.getMsec(), running=latched=False; }

	/**
	Starts the timer.
	*/
	void Start(void) { running = True, latched=False; }

	/**
	Stops the timer
	*/
	void Stop(void) { running = False; }

	/**
	latches the msec elapsed
	*/
    void Latch(void) { latchTime = globalTimer.getMsec(), latched=True; }

	/**
	resets the timer
	*/
    void Reset(void) { startTime = globalTimer.getMsec(), running=True, latched=False; }

	/**
	@return true if the timer is running, false otherwise.
	*/
    bool getStatus(void) { return( running ); }

    double getTime(void);
    dword getMsec(void);

private:
    bool    running;
    bool    latched;
    dword   startTime;
    dword   latchTime;
};


/**
Decrement Timer
*/
class DecTimer{

public:
	/**
	Class constructor
	*/
	DecTimer(void) { matchTime = globalTimer.getMsec(); }

	/**
	Preset the counting time and start the timer.
	@param msec counting time in msec.
	*/
	void Preset(dword msec)   { startTime = globalTimer.getMsec(), matchTime = startTime + msec, running = true; };

    bool Match(void);

	/**
	Stops the timer.
	*/
	void Stop(void) { running = false; };

	/**
	Starts the timer.
	*/
	void Start(){running = true;};

	/**
	@return true if the timer is running, false otherwise.
	*/
    bool getStatus(void) { return( running ); };

private:

    volatile dword   startTime;
    volatile dword   matchTime;
    volatile bool    running;
};

//--------------------------------------------------------//
// Definizione delle funzioni
//--------------------------------------------------------//


//--------------------------------------------------------//
// Definizione delle variabili
//--------------------------------------------------------//


#ifndef __SWTIM__
extern Timer globalTimer;
#endif
#endif
