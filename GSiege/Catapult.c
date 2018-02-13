#include "includes.h"

HWND hCatInstructions;

int CatapultShowElements()
{
	ShowWindow(hCatInstructions, GlobalSettings.nCmdShow);
	ShowWindow(g_hCatSliderBar, GlobalSettings.nCmdShow);
	ShowWindow(g_hCatSliderValue, GlobalSettings.nCmdShow);
	ShowWindow(g_hCatStartButton, GlobalSettings.nCmdShow);
	ShowWindow(g_hCatDurationBox, GlobalSettings.nCmdShow);

	UpdateWindow(hCatInstructions);
	UpdateWindow(g_hCatSliderBar);
	UpdateWindow(g_hCatSliderValue);
	UpdateWindow(g_hCatStartButton);
	UpdateWindow(g_hCatDurationBox);
	return 1;
}

int CatapultHideElements()
{
	ShowWindow(hCatInstructions, SW_HIDE);
	ShowWindow(g_hCatSliderBar, SW_HIDE);
	ShowWindow(g_hCatSliderValue, SW_HIDE);
	ShowWindow(g_hCatStartButton, SW_HIDE);
	ShowWindow(g_hCatDurationBox, SW_HIDE);
	return 1;
}

void CatapultToggleElements()
{
	int n;

	// Hide
	FlameRamHideElements();
	TrebuchetHideElements();

	// Show
	CatapultShowElements();

	// Set bold font and repaint
	SendMessage(GlobalSettings.hSiegeTab[0], WM_SETFONT, (WPARAM)GlobalSettings.hFont, 0); // Flame Ram
	SendMessage(GlobalSettings.hSiegeTab[1], WM_SETFONT, (WPARAM)GlobalSettings.hBoldFont, 0); // Catapult
	SendMessage(GlobalSettings.hSiegeTab[2], WM_SETFONT, (WPARAM)GlobalSettings.hFont, 0); // Trebuchet
	for ( n = 0; n < MAXTABS; n++ )
		InvalidateRect(GlobalSettings.hSiegeTab[n], NULL, TRUE);
}

void CatStartBot()
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
		tpp.nType = BOT_CATAPULT;
		tpp.dwRange = (DWORD)UserSettings.CatRange;

		// Start bot thread and set form elements
		GlobalSettings.hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&TimerProc, &tpp, 0, &GlobalSettings.dwThreadId); 
		if ( GlobalSettings.hThread != NULL )
			SetStartStop(SHOWSTOP);
	}
}

void CatapultCheckState(HWND hWnd)
{
	if ( hWnd == g_hCatStartButton )
		CatStartBot();
}

int CatapultFormBuilder()
{
	char buffer[32];

	int nCpadding; // Padding between elements
	int nCIBx, nCIBy, nCIBwidth, nCIBheight; // Information static position
	int nCSCBx, nCSCBy, nCSCBwidth, nCSCBheight; // Scrollbar position
	int nCVRx, nCVRy, nCVRwidth, nCVRheight; // Value static position
	int nCSBx, nCSBwidth, nCSBheight; // Start button position
	int nCDBx, nCDBy, nCDBwidth, nCDBheight; // Duration static position

	nCpadding = 20;

	// Instruction position
	nCIBx = nCpadding;
	nCIBy = DEFBUT_HEIGHT + nCpadding;
	nCIBwidth = UserSettings.ClientWidth - (nCpadding * 2);
	nCIBheight = 90;

	// Scroll Bar position
	nCSCBx = nCpadding;
	nCSCBy = DEFBUT_HEIGHT + nCIBy + nCIBheight;
	nCSCBwidth = UserSettings.ClientWidth - (nCpadding * 7);
	nCSCBheight = (nCpadding / 2);

	// Value Readout position
	nCVRx = nCSCBx + nCSCBwidth + (nCpadding / 2);
	nCVRy = nCSCBy;
	nCVRwidth = nCpadding * 5;
	nCVRheight = nCpadding;

	// Start button position
	nCSBwidth = DEFBUT_WIDTH;
	nCSBheight = DEFBUT_HEIGHT;
	nCSBx = (UserSettings.ClientWidth - nCpadding) - DEFBUT_WIDTH;
	g_nSBy = (UserSettings.ClientHeight - nCpadding) - DEFBUT_HEIGHT - TAB_CHAT_HEIGHT;

	// Duration timer position
	nCDBx = nCpadding;
	nCDBy = (UserSettings.ClientHeight - nCpadding) - abs(nCpadding / 2) - TAB_CHAT_HEIGHT;
	nCDBwidth = UserSettings.ClientWidth - (nCpadding * 2) - DEFBUT_WIDTH;
	nCDBheight = nCpadding;

	// Instructions
	hCatInstructions = CreateWindow(
		TEXT("static"), "1. Move the slider bar to the desired range (seconds to hold the fire button).\n\n" \
		                "2. Use the left and right arrows to make precision changes to the range as needed.\n\n" \
					    "3. Press Start button on this program.",
		WS_CHILD | WS_VISIBLE | SS_EDITCONTROL,
		nCIBx, nCIBy, nCIBwidth, nCIBheight,
		GlobalSettings.hMainWindow, NULL, GlobalSettings.hInstance, NULL);

	if ( hCatInstructions == NULL )
	{
		MessageBox(NULL, "Failed to create hCatInstructions.", "Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	SendMessage(hCatInstructions, WM_SETFONT, (WPARAM)GlobalSettings.hFont, 0);

	// Scroll bar
	g_hCatSliderBar = CreateWindow(
		TEXT("scrollbar"), NULL,
		WS_CHILD | WS_VISIBLE | SBS_HORZ,
		nCSCBx, nCSCBy, nCSCBwidth, nCSCBheight,
		GlobalSettings.hMainWindow, NULL, GlobalSettings.hInstance, NULL);

	if ( g_hCatSliderBar == NULL )
	{
		MessageBox(NULL, "Failed to create g_hCatSliderBar.", "Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	SetScrollRange(g_hCatSliderBar, SB_CTL, CATMINRNG, CATMAXRNG, FALSE);
	SetScrollPos(g_hCatSliderBar, SB_CTL, UserSettings.CatRange, FALSE);

	// Value readout
	g_hCatSliderValue = CreateWindow(
		TEXT("static"), "Range: 0.00",
		WS_CHILD | WS_VISIBLE | SS_EDITCONTROL,
		nCVRx, nCVRy, nCVRwidth, nCVRheight,
		GlobalSettings.hMainWindow, NULL, GlobalSettings.hInstance, NULL);

	if ( g_hCatSliderValue == NULL )
	{
		MessageBox(NULL, "Failed to create g_hCatSliderValue.", "Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	memset(buffer, 0, sizeof(buffer));
	_snprintf(buffer, sizeof(buffer)-1, "Range: %.2f", (((float)UserSettings.CatRange / (float)CATMAXRNG) * 100));
	SendMessage(g_hCatSliderValue, WM_SETFONT, (WPARAM)GlobalSettings.hFont, 0);
	SendMessage(g_hCatSliderValue, WM_SETTEXT, 0, (LPARAM)buffer);

	// Start button
	g_hCatStartButton = CreateWindow(
		TEXT("button"), "Start",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		nCSBx, g_nSBy, nCSBwidth, nCSBheight,
		GlobalSettings.hMainWindow, NULL, GlobalSettings.hInstance, NULL);

	if( g_hCatStartButton == NULL )
	{
		MessageBox(NULL, "Failed to create g_hCatStartButton.", "Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	SendMessage(g_hCatStartButton, WM_SETFONT, (WPARAM)GlobalSettings.hFont, 0);

	// Duration box
	g_hCatDurationBox = CreateWindow(
		TEXT("static"), "Runtime: 0.00s",
		WS_CHILD | WS_VISIBLE | SS_EDITCONTROL,
		nCDBx, nCDBy, nCDBwidth, nCDBheight,
		GlobalSettings.hMainWindow, NULL, GlobalSettings.hInstance, NULL);

	if ( g_hCatDurationBox == NULL )
	{
		MessageBox(NULL, "Failed to create g_hCatDurationBox.", "Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	SendMessage(g_hCatDurationBox, WM_SETFONT, (WPARAM)GlobalSettings.hFont, 0);

	return 1;
}