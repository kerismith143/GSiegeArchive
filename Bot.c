#include "includes.h"

#define FINDCLASS	"ArenaNet_Dx_Window_Class"

#define RAMMASTER	3500
#define RAMNOMASTER	5500
#define CATTREB		5500

#define VKSC_UP		0x01
#define VK_0		0x30
#define VK_1		0x31
#define VK_2		0x32
#define VK_3		0x33
#define VK_4		0x34
#define VK_5		0x35
#define VK_6		0x36
#define VK_7		0x37
#define VK_8		0x38
#define VK_9		0x39

DWORD RandOffset(DWORD dwOrigValue, int nMilliseconds)
{
	DWORD dwNewValue;
	int nMultiplier = 1;

	if ( nMilliseconds ) nMultiplier = 100;
	srand((unsigned int)time(NULL));
	dwNewValue = (((rand() % 5)+1) * nMultiplier) + dwOrigValue;
	return dwNewValue;
}

UINT32 CreateLParam(USHORT nRepeatCount, CHAR vkScanCode, BOOL bDownBefore, BOOL bState)
{
	UINT32 nValue = 0;

	nValue = nRepeatCount | (vkScanCode << 16);
	if ( bDownBefore ) nValue |= (VKSC_UP << 30);;
	if ( bState ) nValue |= (VKSC_UP << 31);
	return nValue;
}

DWORD SendKeys(HWND hWnd, char ch, DWORD pause)
{
	short vkChar;
	DWORD dwDuration = 0;

	vkChar = LOBYTE(VkKeyScan(ch));
	PostMessage(hWnd, WM_KEYDOWN, (WPARAM)vkChar,
		(LPARAM)CreateLParam(0, MapVirtualKey(vkChar, MAPVK_VK_TO_VSC), FALSE, FALSE));
	dwDuration = RandOffset(pause, MS_ENABLED);
	Sleep(dwDuration);
	PostMessage(hWnd, WM_KEYUP, (WPARAM)vkChar,
		(LPARAM)CreateLParam(1, MapVirtualKey(vkChar, MAPVK_VK_TO_VSC), TRUE, TRUE));

	return dwDuration;
}

DWORD BotRam(DWORD dwIteration)
{
	HWND hGuildWars;
	unsigned int n = 3;
	DWORD dwDuration = 0;
	DWORD dwSleep;

	while ( BOT_EXECUTE )
	{
		// Locate and set Guild Wars 2 as active
		hGuildWars = FindWindow(FINDCLASS, NULL);
		if ( hGuildWars != NULL )
		{
			// Use Flame Blast(2) skill every 4th tick
			if ( n >= 3 )
			{
				n = 0; // Reset tick count
				dwDuration += SendKeys(hGuildWars, VK_2, 0);
			}

			// Pause between skills
			dwSleep = RandOffset(100, MS_ENABLED);
			dwDuration += dwSleep;
			Sleep(dwSleep);

			// Use Ram(1) skill
			dwDuration += SendKeys(hGuildWars, VK_1, 0);

			// Sleep one iteration and increment tick count
			dwSleep = RandOffset(dwIteration, MS_ENABLED);
			dwDuration += dwSleep;
			n++;
			Sleep(dwSleep);
		}
	}
	return dwDuration;
}

DWORD BotCT(int nType)
{
	HWND hGuildWars;
	unsigned int n = 0;
	DWORD dwDuration = 0;
	DWORD dwSleep;
	DWORD dwRange;

	while ( BOT_EXECUTE )
	{
		// Locate and set Guild Wars 2 as active
		hGuildWars = FindWindow(FINDCLASS, NULL);
		if ( hGuildWars != NULL )
		{
			// Set range of siege
			if ( nType == BOT_TREBUCHET )
				dwRange = (((DWORD)g_nTrebRange) * 10);
			else if ( nType == BOT_CATAPULT )
				dwRange = (((DWORD)g_nCatRange) * 10);
			else
			{
				BOT_EXECUTE = 0;
				return dwDuration;
			}

			// Press Fire Boulder(2)/Fire(2) skill
			dwDuration += SendKeys(hGuildWars, VK_2, dwRange);

			// Sleep one iteration and increment tick count
			dwSleep = RandOffset(CATTREB, MS_ENABLED);
			dwDuration += dwSleep;
			n++;
			Sleep(dwSleep);
		}
	}
	return dwDuration;
}

DWORD TimerProc(LPVOID lpParams)
{
	LPTIMERPROCPARAMS ptpp = (LPTIMERPROCPARAMS)lpParams;
	DWORD dwDuration;
	float fDuration;
	char buffer[32];

	memset(buffer, 0, sizeof(buffer));

	if ( ptpp->nType == BOT_RAM_NOMASTER )
	{
		dwDuration = BotRam(RAMNOMASTER);
		fDuration = (((float)dwDuration) / 1000);
		_snprintf(buffer, sizeof(buffer)-1, "Run Time: %.2fs", fDuration);
		SendMessage(g_hRamDurationBox, WM_SETTEXT,0,(LPARAM)buffer);
		SetStartStop(SHOWSTART);
		return dwDuration;
	}
	else if ( ptpp->nType == BOT_RAM_MASTER )
	{
		dwDuration = BotRam(RAMMASTER);
		fDuration = (((float)dwDuration) / 1000);
		_snprintf(buffer, sizeof(buffer)-1, "Run Time: %.2fs", fDuration);
		SendMessage(g_hRamDurationBox, WM_SETTEXT,0,(LPARAM)buffer);
		SetStartStop(SHOWSTART);
		return dwDuration;
	}
	else if ( (ptpp->nType == BOT_CATAPULT) || (ptpp->nType == BOT_TREBUCHET) )
	{
		dwDuration = BotCT(ptpp->nType);
		fDuration = (((float)dwDuration) / 1000);
		_snprintf(buffer, sizeof(buffer)-1, "Run Time: %.2fs", fDuration);
		if ( ptpp->nType == BOT_CATAPULT )
			SendMessage(g_hCatDurationBox, WM_SETTEXT,0,(LPARAM)buffer);
		else
			SendMessage(g_hTrebDurationBox, WM_SETTEXT,0,(LPARAM)buffer);
		SetStartStop(SHOWSTART);
		return dwDuration;
	}
	return 0;
}