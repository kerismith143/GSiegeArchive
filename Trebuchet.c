#include "includes.h"

HWND hTrebInstructions;

int TrebuchetShowElements()
{
	ShowWindow(hTrebInstructions, g_nCmdShow);
	ShowWindow(g_hTrebSliderBar, g_nCmdShow);
	ShowWindow(g_hTrebSliderValue, g_nCmdShow);
	ShowWindow(g_hTrebStartButton, g_nCmdShow);
	ShowWindow(g_hTrebDurationBox, g_nCmdShow);

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
	SendMessage(g_hSiegeTab[0], WM_SETFONT, (WPARAM)hFont, 0); // Flame Ram
	SendMessage(g_hSiegeTab[1], WM_SETFONT, (WPARAM)hFont, 0); // Catapult
	SendMessage(g_hSiegeTab[2], WM_SETFONT, (WPARAM)hBoldFont, 0); // Trebuchet
	for ( n = 0; n < MAXTABS; n++ )
		InvalidateRect(g_hSiegeTab[n], NULL, TRUE);
}

void TrebStartBot()
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
		tpp.nType = BOT_TREBUCHET;
		tpp.dwRange = (DWORD)g_nTrebRange;

		// Start bot thread and set form elements
		g_hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&TimerProc, &tpp, 0, &g_dwThreadId); 
		if ( g_hThread != NULL )
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
	int nTpadding; // Padding between elements
	int nTIBx, nTIBy, nTIBwidth, nTIBheight; // Instruction static position
	int nTSCBx, nTSCBy, nTSCBwidth, nTSCBheight; // Scrollbar position
	int nTVRx, nTVRy, nTVRwidth, nTVRheight; // Value static position
	int nTSBx, nTSBy, nTSBwidth, nTSBheight; // Start button position
	int nTDBx, nTDBy, nTDBwidth, nTDBheight; // Duration static position

	nTpadding = 20;

	// Instruction position
	nTIBx = nTpadding;
	nTIBy = DEFBUT_HEIGHT + nTpadding;
	nTIBwidth = g_nMainClientWidth - (nTpadding * 2);
	nTIBheight = 90;

	// Scroll Bar position
	nTSCBx = nTpadding;
	nTSCBy = DEFBUT_HEIGHT + nTIBy + nTIBheight;
	nTSCBwidth = g_nMainClientWidth - (nTpadding * 7);
	nTSCBheight = (nTpadding / 2);

	// Value Readout position
	nTVRx = nTSCBx + nTSCBwidth + (nTpadding / 2);
	nTVRy = nTSCBy;
	nTVRwidth = nTpadding * 5;
	nTVRheight = nTpadding;

	// Start button position
	nTSBwidth = DEFBUT_WIDTH;
	nTSBheight = DEFBUT_HEIGHT;
	nTSBx = (g_nMainClientWidth - nTpadding) - DEFBUT_WIDTH;
	nTSBy = (g_nMainClientHeight - nTpadding) - DEFBUT_HEIGHT;

	// Duration timer position
	nTDBx = nTpadding;
	nTDBy = (g_nMainClientHeight - nTpadding) - abs(nTpadding / 2);
	nTDBwidth = g_nMainClientWidth - (nTpadding * 2) - DEFBUT_WIDTH;
	nTDBheight = nTpadding;

	// Instructions
	hTrebInstructions = CreateWindow(
		TEXT("static"), "1. Move the slider bar to the desired range (seconds to hold the fire button).\n\n" \
		                "2. Use the left and right arrows to make precision changes to the range as needed.\n\n" \
					    "3. Press Start button on this program.",
		WS_CHILD | WS_VISIBLE | SS_EDITCONTROL,
		nTIBx, nTIBy, nTIBwidth, nTIBheight,
		g_hMainWindow, NULL, g_hInstance, NULL);

	if ( hTrebInstructions == NULL )
	{
		MessageBox(NULL, "Failed to create hTrebInstructions.", "Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	SendMessage(hTrebInstructions, WM_SETFONT, (WPARAM)hFont, 0);

	// Scroll bar
	g_hTrebSliderBar = CreateWindow(
		TEXT("scrollbar"), NULL,
		WS_CHILD | WS_VISIBLE | SBS_HORZ,
		nTSCBx, nTSCBy, nTSCBwidth, nTSCBheight,
		g_hMainWindow, NULL, g_hInstance, NULL);

	if ( g_hTrebSliderBar == NULL )
	{
		MessageBox(NULL, "Failed to create g_hTrebSliderBar.", "Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	g_nTrebRange = 0;
	SetScrollRange(g_hTrebSliderBar, SB_CTL, TREBMINRNG, TREBMAXRNG, FALSE);
	SetScrollPos(g_hTrebSliderBar, SB_CTL, 0, FALSE);

	// Value readout
	g_hTrebSliderValue = CreateWindow(
		TEXT("static"), "Range: 0.00",
		WS_CHILD | WS_VISIBLE | SS_EDITCONTROL,
		nTVRx, nTVRy, nTVRwidth, nTVRheight,
		g_hMainWindow, NULL, g_hInstance, NULL);

	if ( g_hTrebSliderValue == NULL )
	{
		MessageBox(NULL, "Failed to create g_hTrebSliderValue.", "Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	SendMessage(g_hTrebSliderValue, WM_SETFONT, (WPARAM)hFont, 0);

	// Start button
	g_hTrebStartButton = CreateWindow(
		TEXT("button"), "Start",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		nTSBx, nTSBy, nTSBwidth, nTSBheight,
		g_hMainWindow, NULL, g_hInstance, NULL);

	if( g_hTrebStartButton == NULL )
	{
		MessageBox(NULL, "Failed to create g_hTrebStartButton.", "Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	SendMessage(g_hTrebStartButton, WM_SETFONT, (WPARAM)hFont, 0);

	// Duration box
	g_hTrebDurationBox = CreateWindow(
		TEXT("static"), "Runtime: 0.00s",
		WS_CHILD | WS_VISIBLE | SS_EDITCONTROL,
		nTDBx, nTDBy, nTDBwidth, nTDBheight,
		g_hMainWindow, NULL, g_hInstance, NULL);

	if ( g_hTrebDurationBox == NULL )
	{
		MessageBox(NULL, "Failed to create g_hTrebDurationBox.", "Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	SendMessage(g_hTrebDurationBox, WM_SETFONT, (WPARAM)hFont, 0);

	return 1;
}