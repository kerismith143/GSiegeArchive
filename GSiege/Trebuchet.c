#include "includes.h"

HWND hTrebInstructions;

int TrebuchetShowElements()
{
	ShowWindow(hTrebInstructions, GlobalSettings.nCmdShow);
	ShowWindow(g_hTrebSliderBar, GlobalSettings.nCmdShow);
	ShowWindow(g_hTrebSliderValue, GlobalSettings.nCmdShow);
	ShowWindow(g_hTrebStartButton, GlobalSettings.nCmdShow);
	ShowWindow(g_hTrebDurationBox, GlobalSettings.nCmdShow);

	UpdateWindow(hTrebInstructions);
	UpdateWindow(g_hTrebSliderBar);
	UpdateWindow(g_hTrebSliderValue);
	UpdateWindow(g_hTrebStartButton);
	UpdateWindow(g_hTrebDurationBox);
	return 1;
}

int TrebuchetHideElements()
{
	ShowWindow(hTrebInstructions, SW_HIDE);
	ShowWindow(g_hTrebSliderBar, SW_HIDE);
	ShowWindow(g_hTrebSliderValue, SW_HIDE);
	ShowWindow(g_hTrebStartButton, SW_HIDE);
	ShowWindow(g_hTrebDurationBox, SW_HIDE);
	return 1;
}

void TrebuchetToggleElements()
{
	int n;

	// Hide
	FlameRamHideElements();
	CatapultHideElements();

	// Show
	TrebuchetShowElements();
	
	// Set bold font and repaint
	SendMessage(GlobalSettings.hSiegeTab[0], WM_SETFONT, (WPARAM)GlobalSettings.hFont, 0); // Flame Ram
	SendMessage(GlobalSettings.hSiegeTab[1], WM_SETFONT, (WPARAM)GlobalSettings.hFont, 0); // Catapult
	SendMessage(GlobalSettings.hSiegeTab[2], WM_SETFONT, (WPARAM)GlobalSettings.hBoldFont, 0); // Trebuchet
	for ( n = 0; n < MAXTABS; n++ )
		InvalidateRect(GlobalSettings.hSiegeTab[n], NULL, TRUE);
}

void TrebStartBot()
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
		tpp.nType = BOT_TREBUCHET;
		tpp.dwRange = (DWORD)UserSettings.TrebRange;

		// Start bot thread and set form elements
		GlobalSettings.hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&TimerProc, &tpp, 0, &GlobalSettings.dwThreadId); 
		if ( GlobalSettings.hThread != NULL )
			SetStartStop(SHOWSTOP);
	}
}

void TrebuchetCheckState(HWND hWnd)
{
	if ( hWnd == g_hTrebStartButton )
		TrebStartBot();
}

int TrebuchetFormBuilder()
{
	char buffer[32];

	int nTpadding; // Padding between elements
	int nTIBx, nTIBy, nTIBwidth, nTIBheight; // Instruction static position
	int nTSCBx, nTSCBy, nTSCBwidth, nTSCBheight; // Scrollbar position
	int nTVRx, nTVRy, nTVRwidth, nTVRheight; // Value static position
	int nTSBx, nTSBwidth, nTSBheight; // Start button position
	int nTDBx, nTDBy, nTDBwidth, nTDBheight; // Duration static position

	nTpadding = 20;

	// Instruction position
	nTIBx = nTpadding;
	nTIBy = DEFBUT_HEIGHT + nTpadding;
	nTIBwidth = UserSettings.ClientWidth - (nTpadding * 2);
	nTIBheight = 90;

	// Scroll Bar position
	nTSCBx = nTpadding;
	nTSCBy = DEFBUT_HEIGHT + nTIBy + nTIBheight;
	nTSCBwidth = UserSettings.ClientWidth - (nTpadding * 7);
	nTSCBheight = (nTpadding / 2);

	// Value Readout position
	nTVRx = nTSCBx + nTSCBwidth + (nTpadding / 2);
	nTVRy = nTSCBy;
	nTVRwidth = nTpadding * 5;
	nTVRheight = nTpadding;

	// Start button position
	nTSBwidth = DEFBUT_WIDTH;
	nTSBheight = DEFBUT_HEIGHT;
	nTSBx = (UserSettings.ClientWidth - nTpadding) - DEFBUT_WIDTH;
	g_nSBy = (UserSettings.ClientHeight - nTpadding) - DEFBUT_HEIGHT - TAB_CHAT_HEIGHT;

	// Duration timer position
	nTDBx = nTpadding;
	nTDBy = (UserSettings.ClientHeight - nTpadding) - abs(nTpadding / 2) - TAB_CHAT_HEIGHT;
	nTDBwidth = UserSettings.ClientWidth - (nTpadding * 2) - DEFBUT_WIDTH;
	nTDBheight = nTpadding;

	// Instructions
	hTrebInstructions = CreateWindow(
		TEXT("static"), "1. Move the slider bar to the desired range (seconds to hold the fire button).\n\n" \
		                "2. Use the left and right arrows to make precision changes to the range as needed.\n\n" \
					    "3. Press Start button on this program.",
		WS_CHILD | WS_VISIBLE | SS_EDITCONTROL,
		nTIBx, nTIBy, nTIBwidth, nTIBheight,
		GlobalSettings.hMainWindow, NULL, GlobalSettings.hInstance, NULL);

	if ( hTrebInstructions == NULL )
	{
		MessageBox(NULL, "Failed to create hTrebInstructions.", "Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	SendMessage(hTrebInstructions, WM_SETFONT, (WPARAM)GlobalSettings.hFont, 0);

	// Scroll bar
	g_hTrebSliderBar = CreateWindow(
		TEXT("scrollbar"), NULL,
		WS_CHILD | WS_VISIBLE | SBS_HORZ,
		nTSCBx, nTSCBy, nTSCBwidth, nTSCBheight,
		GlobalSettings.hMainWindow, NULL, GlobalSettings.hInstance, NULL);

	if ( g_hTrebSliderBar == NULL )
	{
		MessageBox(NULL, "Failed to create g_hTrebSliderBar.", "Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	SetScrollRange(g_hTrebSliderBar, SB_CTL, TREBMINRNG, TREBMAXRNG, FALSE);
	SetScrollPos(g_hTrebSliderBar, SB_CTL, UserSettings.TrebRange, FALSE);

	// Value readout
	g_hTrebSliderValue = CreateWindow(
		TEXT("static"), "Range: 0.00",
		WS_CHILD | WS_VISIBLE | SS_EDITCONTROL,
		nTVRx, nTVRy, nTVRwidth, nTVRheight,
		GlobalSettings.hMainWindow, NULL, GlobalSettings.hInstance, NULL);

	if ( g_hTrebSliderValue == NULL )
	{
		MessageBox(NULL, "Failed to create g_hTrebSliderValue.", "Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	memset(buffer, 0, sizeof(buffer));
	_snprintf(buffer, sizeof(buffer)-1, "Range: %.2f", (((float)UserSettings.TrebRange / (float)TREBMAXRNG) * 100));
	SendMessage(g_hTrebSliderValue, WM_SETFONT, (WPARAM)GlobalSettings.hFont, 0);
	SendMessage(g_hTrebSliderValue, WM_SETTEXT, 0, (LPARAM)buffer);

	// Start button
	g_hTrebStartButton = CreateWindow(
		TEXT("button"), "Start",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		nTSBx, g_nSBy, nTSBwidth, nTSBheight,
		GlobalSettings.hMainWindow, NULL, GlobalSettings.hInstance, NULL);

	if( g_hTrebStartButton == NULL )
	{
		MessageBox(NULL, "Failed to create g_hTrebStartButton.", "Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	SendMessage(g_hTrebStartButton, WM_SETFONT, (WPARAM)GlobalSettings.hFont, 0);

	// Duration box
	g_hTrebDurationBox = CreateWindow(
		TEXT("static"), "Runtime: 0.00s",
		WS_CHILD | WS_VISIBLE | SS_EDITCONTROL,
		nTDBx, nTDBy, nTDBwidth, nTDBheight,
		GlobalSettings.hMainWindow, NULL, GlobalSettings.hInstance, NULL);

	if ( g_hTrebDurationBox == NULL )
	{
		MessageBox(NULL, "Failed to create g_hTrebDurationBox.", "Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	SendMessage(g_hTrebDurationBox, WM_SETFONT, (WPARAM)GlobalSettings.hFont, 0);

	return 1;
}