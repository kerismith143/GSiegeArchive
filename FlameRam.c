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
	ShowWindow(g_hRamDurationBox, g_nCmdShow);
	ShowWindow(g_hRamStartButton, g_nCmdShow);
	ShowWindow(hRamMasteryCheckbox, g_nCmdShow);
	ShowWindow(hRamInstructions, g_nCmdShow);
	UpdateWindow(g_hRamDurationBox);
	UpdateWindow(g_hRamStartButton);
	UpdateWindow(hRamMasteryCheckbox);
	UpdateWindow(hRamInstructions);
	return g_nCmdShow;
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
	SendMessage(g_hSiegeTab[0], WM_SETFONT, (WPARAM)hBoldFont, 0); // Flame Ram
	SendMessage(g_hSiegeTab[1], WM_SETFONT, (WPARAM)hFont, 0); // Catapult
	SendMessage(g_hSiegeTab[2], WM_SETFONT, (WPARAM)hFont, 0); // Trebuchet
	for ( n = 0; n < MAXTABS; n++ )
		InvalidateRect(g_hSiegeTab[n], NULL, TRUE);
}

void FlameRamSetMastery()
{
	int nState;

	nState = SendMessage(hRamMasteryCheckbox, BM_GETCHECK, 0, 0);
	switch ( nState )
	{
		case BST_UNCHECKED:
			g_nRamMastery = 0;
			break;
		case BST_CHECKED:
			g_nRamMastery = 1;
			break;
		default:
			g_nRamMastery = 0;
			SendMessage(hRamMasteryCheckbox, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
			break;
	}
}

void FlameRamStartBot()
{
	if ( BOT_EXECUTE )
	{
		DWORD dwDuration;

		// Set bot execution control variable
		BOT_EXECUTE = 0;

		memset(&tpp, 0, sizeof(tpp));

		// Stop bot thread and set form elements
		if ( g_dwThreadId )
		{
			GetExitCodeThread(g_hThread, &dwDuration);
			g_dwThreadId = 0;
		}
	}
	else if ( GetStartStop() )
	{
		// Set bot execution control variable
		BOT_EXECUTE = 1;

		// Build params struct
		if ( g_nRamMastery )
			tpp.nType = BOT_RAM_MASTER;
		else
			tpp.nType = BOT_RAM_NOMASTER;
		tpp.dwRange = 0;

		// Start bot thread and set form elements
		g_hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&TimerProc, &tpp, 0, &g_dwThreadId); 
		if ( g_hThread != NULL )
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
	int nFRSBx, nFRSBy, nFRSBwidth, nFRSBheight; // Start button position
	int nFRDBx, nFRDBy, nFRDBwidth, nFRDBheight; // Duration static position

	nFRpadding = 20;

	// Instruction position
	nFRIBx = nFRpadding;
	nFRIBy = DEFBUT_HEIGHT + nFRpadding;
	nFRIBwidth = g_nMainClientWidth - (nFRpadding * 2);
	nFRIBheight = 70;

	// Mastery checkbox position
	nFRCBx = nFRpadding;
	nFRCBy = DEFBUT_HEIGHT + nFRIBy + nFRIBheight;
	nFRCBwidth = g_nMainClientWidth - (nFRpadding * 2);
	nFRCBheight = nFRpadding;

	// Start button position
	nFRSBwidth = DEFBUT_WIDTH;
	nFRSBheight = DEFBUT_HEIGHT;
	nFRSBx = (g_nMainClientWidth - nFRpadding) - DEFBUT_WIDTH;
	nFRSBy = (g_nMainClientHeight - nFRpadding) - DEFBUT_HEIGHT;

	// Duration timer position
	nFRDBx = nFRpadding;
	nFRDBy = (g_nMainClientHeight - nFRpadding) - abs(nFRpadding / 2);
	nFRDBwidth = g_nMainClientWidth - (nFRpadding * 2) - DEFBUT_WIDTH;
	nFRDBheight = nFRpadding;

	// Instructions
	hRamInstructions = CreateWindow(
		TEXT("static"), "1. Select whether you have Flame Ram Mastery III or higher.\n\n" \
		                "2. Select your target in-game.\n\n" \
					    "3. Press Start button on this program.",
		WS_CHILD | WS_VISIBLE | SS_EDITCONTROL,
		nFRIBx, nFRIBy, nFRIBwidth, nFRIBheight,
		g_hMainWindow, NULL, g_hInstance, NULL);

	if ( hRamInstructions == NULL )
	{
		MessageBox(NULL, "Failed to create hRamInstructions.", "Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	SendMessage(hRamInstructions, WM_SETFONT, (WPARAM)hFont, 0);

	// Mastery checkbox
	hRamMasteryCheckbox = CreateWindow(
		TEXT("button"), "Flame Ram Mastery",
		WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
		nFRCBx, nFRCBy, nFRCBwidth, nFRCBheight,
		g_hMainWindow, NULL, g_hInstance, NULL);

	if( hRamMasteryCheckbox == NULL )
	{
		MessageBox(NULL, "Failed to create hRamMasteryCheckbox.", "Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	SendMessage(hRamMasteryCheckbox, WM_SETFONT, (WPARAM)hFont, 0);
	if ( g_nRamMastery == 1 )
		SendMessage(hRamMasteryCheckbox, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);

	// Start button
	g_hRamStartButton = CreateWindow(
		TEXT("button"), "Start",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		nFRSBx, nFRSBy, nFRSBwidth, nFRSBheight,
		g_hMainWindow, NULL, g_hInstance, NULL);

	if( g_hRamStartButton == NULL )
	{
		MessageBox(NULL, "Failed to create g_hRamStartButton.", "Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	SendMessage(g_hRamStartButton, WM_SETFONT, (WPARAM)hFont, 0);

	// Duration box
	g_hRamDurationBox = CreateWindow(
		TEXT("static"), "Runtime: 0.00s",
		WS_CHILD | WS_VISIBLE | SS_EDITCONTROL,
		nFRDBx, nFRDBy, nFRDBwidth, nFRDBheight,
		g_hMainWindow, NULL, g_hInstance, NULL);

	if ( g_hRamDurationBox == NULL )
	{
		MessageBox(NULL, "Failed to create g_hRamDurationBox.", "Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	SendMessage(g_hRamDurationBox, WM_SETFONT, (WPARAM)hFont, 0);

	return 1;
}