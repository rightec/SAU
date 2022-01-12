/**
@file		swtim.cpp
@brief		Classes to manage software timers.

@author		
@date		18/01/2011
@version	01.0
*/


#ifdef __SWTIM__
#error __SWTIM__ already defined !!
#else
#define __SWTIM__
#endif

#include "swtim.h"

#ifdef __SDATA__							// se utilizza il segmento sdata ....
Timer globalTimer		__attribute__ ((section (".sdata")));
#else
Timer globalTimer;
#endif



/**
Class constructor.
*/
Timer::Timer()
{
    sysTimer.elapsed = 0;
    sysTimer.usec = 0;
}

/**
	@return the elapsed time since the timer has been latched in sec. 
	If the timer hasn't been latched return the time elapsed since the timer has been started.
*/
double IncTimer::getTime(void)
{
dword ltime;
double dtime;

	if( latched )
		ltime = latchTime;
	else
		if( running )
			ltime = globalTimer.getMsec();
		else
			ltime = startTime;

	if( ltime >= startTime )     // se il tempo e` andato avanti
		ltime -= startTime;
	else
        ltime += 0x00000000 - startTime;    // considera l'overflow

	dtime = ltime;
    dtime /= 1000.0;

	return( dtime );
}

/**
	@return the elapsed time since the timer has been latched in msec. 
	If the timer hasn't been latched return the time elapsed since the timer has been started.
*/
dword IncTimer::getMsec(void)
{
dword ltime;

	if( latched )
		ltime = latchTime;
	else
		if( running )
			ltime = globalTimer.getMsec();
		else
			ltime = startTime;

	if( ltime >= startTime )     // se il tempo e` andato avanti
		ltime -= startTime;
	else
        ltime += 0x00000000 - startTime;    // considera l'overflow

	return( ltime );
}

/**
@return true if the presetted counting time is elapsed and the timer is running, false otherwise.
*/	
bool DecTimer::Match(void)
{
dword ltime = globalTimer.getMsec();

    if( !running )
        return( False );

    if( matchTime >= startTime )        // se non c'e` overflow di sorta
    {
		if(ltime < startTime || ltime >= matchTime)     // basta il confronto diretto
			return( True );
		else
			return( False );
	}
	else
	{
		if(ltime < startTime && ltime >= matchTime)     // doppio confronto
            return( True );
        else
            return( False );
    }

}




















