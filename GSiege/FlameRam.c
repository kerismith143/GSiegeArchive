#include "includes.h"

HWND hRamInstructions;
HWND hRamMasteryCheckbox;

int FlameRamHideElements()
{
	ShowWindow(g_hRamDurationBox, SW_HIDE);
	ShowWindow(g_hRamStartButton, SW_HIDE);
	ShowWindow(hRamMasteryCheckbox, SW_HIDE);
	ShowWindow(hRamInstructions, SW_HIDE);
	return 1;
}

int FlameRamShowElements()
{
	ShowWindow(g_hRamDurationBox, GlobalSettings.nCmdShow);
	ShowWindow(g_hRamStartButton, GlobalSettings.nCmdShow);
	ShowWindow(hRamMasteryCheckbox, GlobalSettings.nCmdShow);
	ShowWindow(hRamInstructions, GlobalSettings.nCmdShow);
	UpdateWindow(g_hRamDurationBox);
	UpdateWindow(g_hRamStartButton);
	UpdateWindow(hRamMasteryCheckbox);
	UpdateWindow(hRamInstructions);
	return GlobalSettings.nCmdShow;
}

void FlameRamToggleElements()
{
	int n;

	// Hide
	CatapultHideElements();
	TrebuchetHideElements();

	// Show
	FlameRamShowElements();

	// Set bold font and repaint
	SendMessage(GlobalSettings.hSiegeTab[0], WM_SETFONT, (WPARAM)GlobalSettings.hBoldFont, 0); // Flame Ram
	SendMessage(GlobalSettings.hSiegeTab[1], WM_SETFONT, (WPARAM)GlobalSettings.hFont, 0); // Catapult
	SendMessage(GlobalSettings.hSiegeTab[2], WM_SETFONT, (WPARAM)GlobalSettings.hFont, 0); // Trebuchet
	for ( n = 0; n < MAXTABS; n++ )
		InvalidateRect(GlobalSettings.hSiegeTab[n], NULL, TRUE);
}

void FlameRamSetMastery()
{
	int nState;

	nState = SendMessage(hRamMasteryCheckbox, BM_GETCHECK, 0, 0);
	switch ( nState )
	{
		case BST_UNCHECKED:
			UserSettings.RamMastery = 0;
			break;
		case BST_CHECKED:
			UserSettings.RamMastery = 1;
			break;
		default:
			UserSettings.RamMastery = 0;
			SendMessage(hRamMasteryCheckbox, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
			break;
	}
}

void FlameRamStartBot()
{
	if ( GlobalSettings.BOT_EXECUTE )
	{
		DWORD dwDuration;

		// Set bot execution control variable
		GlobalSettings.BOT_EXECUTE = 0;

		memset(&tpp, 0, sizeof(tpp));

		// Stop bot thread and set form elements
		if ( GlobalSettings.dwThreadId )
		{
			GetExitCodeThread(GlobalSettings.hThread, &dwDuration);
			GlobalSettings.dwThreadId = 0;
		}
	}
	else if ( GetStartStop() )
	{
		// Set bot execution control variable
		GlobalSettings.BOT_EXECUTE = 1;

		// Build params struct
		if ( UserSettings.RamMastery )
			tpp.nType = BOT_RAM_MASTER;
		else
			tpp.nType = BOT_RAM_NOMASTER;
		tpp.dwRange = 0;

		// Start bot thread and set form elements
		GlobalSettings.hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&TimerProc, &tpp, 0, &GlobalSettings.dwThreadId); 
		if ( GlobalSettings.hThread != NULL )
			SetStartStop(SHOWSTOP);
	}
}

void FlameRamCheckState(HWND hWnd)
{
	if ( hWnd == hRamMasteryCheckbox )
		FlameRamSetMastery();
	else if ( hWnd == g_hRamStartButton )
		FlameRamStartBot();
}

int FlameRamFormBuilder()
{
	int nFRpadding; // Padding between elements
	int nFRIBx, nFRIBy, nFRIBwidth, nFRIBheight; // Information static position
	int nFRCBx, nFRCBy, nFRCBwidth, nFRCBheight; // Checkbox button position
	int nFRSBx, nFRSBwidth, nFRSBheight; // Start button position
	int nFRDBx, nFRDBy, nFRDBwidth, nFRDBheight; // Duration static position

	nFRpadding = 20;

	// Instruction position
	nFRIBx = nFRpadding;
	nFRIBy = DEFBUT_HEIGHT + nFRpadding;
	nFRIBwidth = UserSettings.ClientWidth - (nFRpadding * 2);
	nFRIBheight = 70;

	// Mastery checkbox position
	nFRCBx = nFRpadding;
	nFRCBy = DEFBUT_HEIGHT + nFRIBy + nFRIBheight;
	nFRCBwidth = UserSettings.ClientWidth - (nFRpadding * 2);
	nFRCBheight = nFRpadding;

	// Start button position
	nFRSBwidth = DEFBUT_WIDTH;
	nFRSBheight = DEFBUT_HEIGHT;
	nFRSBx = (UserSettings.ClientWidth - nFRpadding) - DEFBUT_WIDTH;
	g_nSBy = (UserSettings.ClientHeight - nFRpadding) - DEFBUT_HEIGHT - TAB_CHAT_HEIGHT;

	// Duration timer position
	nFRDBx = nFRpadding;
	nFRDBy = (UserSettings.ClientHeight - nFRpadding) - abs(nFRpadding / 2) - TAB_CHAT_HEIGHT;
	nFRDBwidth = UserSettings.ClientWidth - (nFRpadding * 2) - DEFBUT_WIDTH;
	nFRDBheight = nFRpadding;

	// Instructions
	hRamInstructions = CreateWindow(
		TEXT("static"), "1. Select whether you have Flame Ram Mastery III or higher.\n\n" \
		                "2. Select your target in-game.\n\n" \
					    "3. Press Start button on this program.",
		WS_CHILD | WS_VISIBLE | SS_EDITCONTROL,
		nFRIBx, nFRIBy, nFRIBwidth, nFRIBheight,
		GlobalSettings.hMainWindow, NULL, GlobalSettings.hInstance, NULL);

	if ( hRamInstructions == NULL )
	{
		MessageBox(NULL, "Failed to create hRamInstructions.", "Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	SendMessage(hRamInstructions, WM_SETFONT, (WPARAM)GlobalSettings.hFont, 0);

	// Mastery checkbox
	hRamMasteryCheckbox = CreateWindow(
		TEXT("button"), "Flame Ram Mastery",
		WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
		nFRCBx, nFRCBy, nFRCBwidth, nFRCBheight,
		GlobalSettings.hMainWindow, NULL, GlobalSettings.hInstance, NULL);

	if( hRamMasteryCheckbox == NULL )
	{
		MessageBox(NULL, "Failed to create hRamMasteryCheckbox.", "Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	SendMessage(hRamMasteryCheckbox, WM_SETFONT, (WPARAM)GlobalSettings.hFont, 0);
	if ( UserSettings.RamMastery == 1 )
		SendMessage(hRamMasteryCheckbox, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);

	// Start button
	g_hRamStartButton = CreateWindow(
		TEXT("button"), "Start",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		nFRSBx, g_nSBy, nFRSBwidth, nFRSBheight,
		GlobalSettings.hMainWindow, NULL, GlobalSettings.hInstance, NULL);

	if( g_hRamStartButton == NULL )
	{
		MessageBox(NULL, "Failed to create g_hRamStartButton.", "Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	SendMessage(g_hRamStartButton, WM_SETFONT, (WPARAM)GlobalSettings.hFont, 0);

	// Duration box
	g_hRamDurationBox = CreateWindow(
		TEXT("static"), "Runtime: 0.00s",
		WS_CHILD | WS_VISIBLE | SS_EDITCONTROL,
		nFRDBx, nFRDBy, nFRDBwidth, nFRDBheight,
		GlobalSettings.hMainWindow, NULL, GlobalSettings.hInstance, NULL);

	if ( g_hRamDurationBox == NULL )
	{
		MessageBox(NULL, "Failed to create g_hRamDurationBox.", "Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	SendMessage(g_hRamDurationBox, WM_SETFONT, (WPARAM)GlobalSettings.hFont, 0);

	return 1;
}