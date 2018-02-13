#include "includes.h"

HWND hCatInstructions;

int CatapultShowElements()
{
	ShowWindow(hCatInstructions, g_nCmdShow);
	ShowWindow(g_hCatSliderBar, g_nCmdShow);
	ShowWindow(g_hCatSliderValue, g_nCmdShow);
	ShowWindow(g_hCatStartButton, g_nCmdShow);
	ShowWindow(g_hCatDurationBox, g_nCmdShow);

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
	SendMessage(g_hSiegeTab[0], WM_SETFONT, (WPARAM)hFont, 0); // Flame Ram
	SendMessage(g_hSiegeTab[1], WM_SETFONT, (WPARAM)hBoldFont, 0); // Catapult
	SendMessage(g_hSiegeTab[2], WM_SETFONT, (WPARAM)hFont, 0); // Trebuchet
	for ( n = 0; n < MAXTABS; n++ )
		InvalidateRect(g_hSiegeTab[n], NULL, TRUE);
}

void CatStartBot()
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
		tpp.nType = BOT_CATAPULT;
		tpp.dwRange = (DWORD)g_nCatRange;

		// Start bot thread and set form elements
		g_hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&TimerProc, &tpp, 0, &g_dwThreadId); 
		if ( g_hThread != NULL )
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
	int nCpadding; // Padding between elements
	int nCIBx, nCIBy, nCIBwidth, nCIBheight; // Information static position
	int nCSCBx, nCSCBy, nCSCBwidth, nCSCBheight; // Scrollbar position
	int nCVRx, nCVRy, nCVRwidth, nCVRheight; // Value static position
	int nCSBx, nCSBy, nCSBwidth, nCSBheight; // Start button position
	int nCDBx, nCDBy, nCDBwidth, nCDBheight; // Duration static position

	nCpadding = 20;

	// Instruction position
	nCIBx = nCpadding;
	nCIBy = DEFBUT_HEIGHT + nCpadding;
	nCIBwidth = g_nMainClientWidth - (nCpadding * 2);
	nCIBheight = 90;

	// Scroll Bar position
	nCSCBx = nCpadding;
	nCSCBy = DEFBUT_HEIGHT + nCIBy + nCIBheight;
	nCSCBwidth = g_nMainClientWidth - (nCpadding * 7);
	nCSCBheight = (nCpadding / 2);

	// Value Readout position
	nCVRx = nCSCBx + nCSCBwidth + (nCpadding / 2);
	nCVRy = nCSCBy;
	nCVRwidth = nCpadding * 5;
	nCVRheight = nCpadding;

	// Start button position
	nCSBwidth = DEFBUT_WIDTH;
	nCSBheight = DEFBUT_HEIGHT;
	nCSBx = (g_nMainClientWidth - nCpadding) - DEFBUT_WIDTH;
	nCSBy = (g_nMainClientHeight - nCpadding) - DEFBUT_HEIGHT;

	// Duration timer position
	nCDBx = nCpadding;
	nCDBy = (g_nMainClientHeight - nCpadding) - abs(nCpadding / 2);
	nCDBwidth = g_nMainClientWidth - (nCpadding * 2) - DEFBUT_WIDTH;
	nCDBheight = nCpadding;

	// Instructions
	hCatInstructions = CreateWindow(
		TEXT("static"), "1. Move the slider bar to the desired range (seconds to hold the fire button).\n\n" \
		                "2. Use the left and right arrows to make precision changes to the range as needed.\n\n" \
					    "3. Press Start button on this program.",
		WS_CHILD | WS_VISIBLE | SS_EDITCONTROL,
		nCIBx, nCIBy, nCIBwidth, nCIBheight,
		g_hMainWindow, NULL, g_hInstance, NULL);

	if ( hCatInstructions == NULL )
	{
		MessageBox(NULL, "Failed to create hCatInstructions.", "Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	SendMessage(hCatInstructions, WM_SETFONT, (WPARAM)hFont, 0);

	// Scroll bar
	g_hCatSliderBar = CreateWindow(
		TEXT("scrollbar"), NULL,
		WS_CHILD | WS_VISIBLE | SBS_HORZ,
		nCSCBx, nCSCBy, nCSCBwidth, nCSCBheight,
		g_hMainWindow, NULL, g_hInstance, NULL);

	if ( g_hCatSliderBar == NULL )
	{
		MessageBox(NULL, "Failed to create g_hCatSliderBar.", "Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	g_nCatRange = 0;
	SetScrollRange(g_hCatSliderBar, SB_CTL, CATMINRNG, CATMAXRNG, FALSE);
	SetScrollPos(g_hCatSliderBar, SB_CTL, 0, FALSE);

	// Value readout
	g_hCatSliderValue = CreateWindow(
		TEXT("static"), "Range: 0.00",
		WS_CHILD | WS_VISIBLE | SS_EDITCONTROL,
		nCVRx, nCVRy, nCVRwidth, nCVRheight,
		g_hMainWindow, NULL, g_hInstance, NULL);

	if ( g_hCatSliderValue == NULL )
	{
		MessageBox(NULL, "Failed to create g_hCatSliderValue.", "Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	SendMessage(g_hCatSliderValue, WM_SETFONT, (WPARAM)hFont, 0);

	// Start button
	g_hCatStartButton = CreateWindow(
		TEXT("button"), "Start",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		nCSBx, nCSBy, nCSBwidth, nCSBheight,
		g_hMainWindow, NULL, g_hInstance, NULL);

	if( g_hCatStartButton == NULL )
	{
		MessageBox(NULL, "Failed to create g_hCatStartButton.", "Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	SendMessage(g_hCatStartButton, WM_SETFONT, (WPARAM)hFont, 0);

	// Duration box
	g_hCatDurationBox = CreateWindow(
		TEXT("static"), "Runtime: 0.00s",
		WS_CHILD | WS_VISIBLE | SS_EDITCONTROL,
		nCDBx, nCDBy, nCDBwidth, nCDBheight,
		g_hMainWindow, NULL, g_hInstance, NULL);

	if ( g_hCatDurationBox == NULL )
	{
		MessageBox(NULL, "Failed to create g_hCatDurationBox.", "Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	SendMessage(g_hCatDurationBox, WM_SETFONT, (WPARAM)hFont, 0);

	return 1;
}