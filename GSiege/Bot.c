#include "includes.h"

#define SKPROCESS	   3 // Amount of buffer entries to process for SendKeysQue()

// Procedure to control Flame Ram bot functionality
DWORD BotRam()
{
	HWND hGuildWars;
	unsigned int n = 3;
	DWORD dwDuration = 0;
	DWORD dwSleep;
	unsigned int k;

	while ( GlobalSettings.BOT_EXECUTE )
	{
		// Locate and set Guild Wars 2 as active
		hGuildWars = FindWindow(FINDCLASS, NULL);
		if ( hGuildWars != NULL )
		{
			// Use Flame Blast(2) skill every 4th tick
			if ( n >= 3 )
			{
				n = 0; // Reset tick count
				dwDuration += SendKeys(hGuildWars, '2', RandOffset(0, MS_ENABLED));
			}

			// Pause between skills
			dwSleep = RandOffset(UserSettings.Offset, MS_ENABLED);
			dwDuration += dwSleep;
			Sleep(dwSleep);

			// Use Ram(1) skill
			dwDuration += SendKeys(hGuildWars, '1', RandOffset(0, MS_ENABLED));

			// Sleep one iteration and increment tick count
			dwSleep = RandOffset((UserSettings.RamMastery ? UserSettings.RamMastRecharge : UserSettings.RamNoMastRecharge), MS_ENABLED);
			dwDuration += dwSleep;
			n++;
			// Send SKPROCESS lines of text from SendQue
			for ( k = 0; k < SKPROCESS; k++ )
			{
				int nRet;
				if ( (nRet= SendKeysQue()) >= 0 )
				{
					dwDuration += nRet;
					dwSleep -= nRet;
					if ( dwSleep > 0xFFFFFF ) // rolled over check
						dwSleep = 0;
				}
			}
			// Wait for iteration
			Sleep(dwSleep);
		}
	}
	return dwDuration;
}

// Procedure to control Catapult and Trebuchet bot functionality
DWORD BotCT(int nType)
{
	HWND hGuildWars;
	unsigned int n = 0;
	DWORD dwDuration = 0;
	DWORD dwSleep;
	DWORD dwRange;
	unsigned int k;

	while ( GlobalSettings.BOT_EXECUTE )
	{
		// Locate and set Guild Wars 2 as active
		hGuildWars = FindWindow(FINDCLASS, NULL);
		if ( hGuildWars != NULL )
		{
			// Set range of siege
			if ( nType == BOT_TREBUCHET )
				dwRange = (((DWORD)UserSettings.TrebRange) * 10);
			else if ( nType == BOT_CATAPULT )
				dwRange = (((DWORD)UserSettings.CatRange) * 10);
			else
			{
				GlobalSettings.BOT_EXECUTE = 0;
				return dwDuration;
			}

			// Press Fire Boulder(2)/Fire(2) skill
			dwDuration += SendKeys(hGuildWars, '2', RandOffset(dwRange, MS_ENABLED));

			// Sleep one iteration and increment tick count
			if ( nType == BOT_TREBUCHET )
				dwSleep = RandOffset(UserSettings.TrebRecharge, MS_ENABLED);
			else if ( nType == BOT_CATAPULT )
				dwSleep = RandOffset(UserSettings.CatRecharge, MS_ENABLED);
			dwDuration += dwSleep;
			n++;

			// Send SKPROCESS lines of text from SendQue
			for ( k = 0; k < SKPROCESS; k++ )
			{
				int nRet;
				if ( (nRet= SendKeysQue()) >= 0 )
				{
					dwDuration += nRet;
					dwSleep -= nRet;
					if ( dwSleep > 0xFFFFFF ) // rolled over check
						dwSleep = 0;
				}
			}

			// Wait for iteration
			Sleep(dwSleep);
		}
	}
	return dwDuration;
}

// Thread procedure for controlling the bot
DWORD TimerProc(LPVOID lpParams)
{
	LPTIMERPROCPARAMS ptpp = (LPTIMERPROCPARAMS)lpParams;
	DWORD dwDuration;
	float fDuration;
	char buffer[32];

	memset(buffer, 0, sizeof(buffer));

	// Execute procedure for Flame Ram with Mastery II or less
	if ( ptpp->nType == BOT_RAM )
	{
		dwDuration = BotRam();
		fDuration = (((float)dwDuration) / 1000);
		_snprintf(buffer, sizeof(buffer)-1, "Run Time: %.2fs", fDuration);
		SendMessage(GlobalSettings.hRamDurationBox, WM_SETTEXT, 0, (LPARAM)buffer);
		SetStartStop(SHOWSTART);
		return dwDuration;
	}

	// Execute procedure for Catapult and Trebuchet
	else if ( (ptpp->nType == BOT_CATAPULT) || (ptpp->nType == BOT_TREBUCHET) )
	{
		dwDuration = BotCT(ptpp->nType);
		fDuration = (((float)dwDuration) / 1000);
		_snprintf(buffer, sizeof(buffer)-1, "Run Time: %.2fs", fDuration);
		if ( ptpp->nType == BOT_CATAPULT )
			SendMessage(GlobalSettings.hCatDurationBox, WM_SETTEXT,0,(LPARAM)buffer);
		else
			SendMessage(GlobalSettings.hTrebDurationBox, WM_SETTEXT,0,(LPARAM)buffer);
		SetStartStop(SHOWSTART);
		return dwDuration;
	}
	return 0;
}