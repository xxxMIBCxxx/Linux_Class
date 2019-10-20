#include <cstdio>
#include "CThread.h"
#include <time.h>
#include "kbhit.h"



#define ESC						( 27 )


int main()
{
	srand((unsigned)time(NULL));
	timespec		tTimeSpec;
	tTimeSpec.tv_sec = 1;
	tTimeSpec.tv_nsec = 0;


	CThread		A("A");
	CThread		B("B");
	CThread		C("C");
	CThread		D("D");
	CThread		E("E");
	CThread		F("F");

	printf("-----[ CThread Demo ]-----\n");
	printf(" [Enter] key : Demo End\n");

	A.Start();
	B.Start();
	C.Start();
	D.Start();
	E.Start();
	F.Start();

	while (1)
	{
		if(kbhit())
		{
			break;
		}
		nanosleep(&tTimeSpec, NULL);

	}

    return 0;
}