#include <cstdio>
#include <iostream>
#include <time.h>
#include <unistd.h>
#include "kbhit.h"
#include "CLog.h"


#define ESC						( 27 )


int main()
{
	bool				bRet = false;
	CLog				cLog;
	unsigned long		i = 0;

	printf("-----[ CLog Demo ]-----\n");
	printf(" [Enter] key : Demo End\n");
	while (1)
	{
		cLog.Output(CLog::LOG_OUTPUT_ERROR, "%09lu - 0123456789ABCDEF", i);
		cLog.Output(CLog::LOG_OUTPUT_ERROR, "%09lu - ABCDEFGHIJK", i);
		cLog.Output(CLog::LOG_OUTPUT_ERROR, "%09lu - LMNOPQRSTUV", i);
		if (kbhit())
		{
			break;
		}
		usleep(1000 * 100);
		i++;
	}

	return 0;
}
