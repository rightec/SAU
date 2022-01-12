/************************************************************************
* Lattice Semiconductor Corp. Copyright 2008
*
* Utility functions
*
************************************************************************/
#include "SSPIE_util.h"
#include "graph.h"

void init_CS(CSU *cs, short int width, short int chunkSize){
	cs->csWidth = width;
	cs->csChunkSize = chunkSize;
	cs->csValue = 0;
}

unsigned int getCheckSum(CSU *cs){
	unsigned int mask = 0xFFFFFFFF;

	mask >>= (32 - cs->csWidth);
	return (cs->csValue & mask);
}

void putChunk(CSU *cs, unsigned int chunk){
	unsigned int mask = 0xFFFFFFFF;

	mask >>= (32 - cs->csChunkSize);
	cs->csValue += (chunk & mask);
}

void print_out_string(const char *stringOut)
{
	static long i = 0;
	if(stringOut)
	{
		setMessage(i, stringOut);
		
		i++;
		if (i > 9 )
		{
			i = 0;
		}
		
		setMessage(i, " ");	
		display_update();
	}

}

void setMessage(int line, const char* str)
{
	char clr[27];
	char message[27];
	long i;
	
	if ((line < 0) || (line > 9))
		return;

	for (i = 0; i < 25; i++)
		clr[i] = ' ';

	clr[25] = '\0';
	
	setcolor(BLACK);
	charset_select("xm5x6");

	if(strlen(str) > 25)
	{
		strncpy(message, str, 25);
	}else
	{
		strcpy(message, str);
	}

	message[25] = '\0';

	gputsxy(0, line * 6, clr);
	gputsxy(0, line * 6, message);
}
