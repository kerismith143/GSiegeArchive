#ifndef __GSIEGE_BOT_HEADER__
#define __GSIEGE_BOT_HEADER__

#define MS_DISABLED	0
#define MS_ENABLED	1

#define BOT_RAM_NOMASTER	0x01
#define	BOT_RAM_MASTER		0x02
#define BOT_CATAPULT		0x04
#define BOT_TREBUCHET		0x08

int BOT_EXECUTE;
DWORD g_dwThreadId;
HANDLE g_hThread;

typedef struct tagTIMERPROCPARAMS
{
	int		nType;
	DWORD	dwRange;
} TIMERPROCPARAMS, *PTIMERPROCPARAMS, *LPTIMERPROCPARAMS;
TIMERPROCPARAMS tpp;

DWORD RandOffset(DWORD dwValue, int nUseMilliseconds);
DWORD TimerProc(LPVOID lpParams);

#endif //__GSIEGE_BOT_HEADER__