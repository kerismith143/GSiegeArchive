#include "includes.h"

HWND hMinStatic, hMaxStatic, hOffsetStatic, hRechargeStatic;

int TrebuchetShowElements()
{
	if ( UserSettings.EnableAdv ) ShowWindow(GlobalSettings.hTrebMinEdit, GlobalSettings.nCmdShow);
	else ShowWindow(GlobalSettings.hTrebMinEdit, SW_HIDE);
	if ( UserSettings.EnableAdv ) ShowWindow(hMinStatic, GlobalSettings.nCmdShow);
	else ShowWindow(hMinStatic, SW_HIDE);
	UpdateWindow(GlobalSettings.hTrebMinEdit);
	UpdateWindow(hMinStatic);

	if ( UserSettings.EnableAdv ) ShowWindow(GlobalSettings.hTrebMaxEdit, GlobalSettings.nCmdShow);
	else ShowWindow(GlobalSettings.hTrebMaxEdit, SW_HIDE);
	if ( UserSettings.EnableAdv ) ShowWindow(hMaxStatic, GlobalSettings.nCmdShow);
	else ShowWindow(hMaxStatic, SW_HIDE);
	UpdateWindow(GlobalSettings.hTrebMaxEdit);
	UpdateWindow(hMaxStatic);

	if ( UserSettings.EnableAdv ) ShowWindow(GlobalSettings.hTrebRecharge, GlobalSettings.nCmdShow);
	else ShowWindow(GlobalSettings.hTrebRecharge, SW_HIDE);
	if ( UserSettings.EnableAdv ) ShowWindow(hRechargeStatic, GlobalSettings.nCmdShow);
	else ShowWindow(hRechargeStatic, SW_HIDE);
	UpdateWindow(GlobalSettings.hTrebRecharge);
	UpdateWindow(hRechargeStatic);

	ShowWindow(GlobalSettings.hTrebSliderBar, GlobalSettings.nCmdShow);
	UpdateWindow(GlobalSettings.hTrebSliderBar);

	ShowWindow(GlobalSettings.hTrebSliderValue, GlobalSettings.nCmdShow);
	UpdateWindow(GlobalSettings.hTrebSliderValue);

	ShowWindow(GlobalSettings.hTrebStartButton, GlobalSettings.nCmdShow);
	UpdateWindow(GlobalSettings.hTrebStartButton);

	ShowWindow(GlobalSettings.hTrebDurationBox, GlobalSettings.nCmdShow);
	UpdateWindow(GlobalSettings.hTrebDurationBox);

	InvalidateRect(GlobalSettings.hMainWindow, NULL, FALSE);
	return 1;
}

int TrebuchetHideElements()
{
	ShowWindow(GlobalSettings.hTrebMinEdit, SW_HIDE);
	ShowWindow(hMinStatic, SW_HIDE);
	ShowWindow(GlobalSettings.hTrebMaxEdit, SW_HIDE);
	ShowWindow(hMaxStatic, SW_HIDE);
	ShowWindow(GlobalSettings.hTrebRecharge, SW_HIDE);
	ShowWindow(hRechargeStatic, SW_HIDE);
	ShowWindow(GlobalSettings.hTrebSliderBar, SW_HIDE);
	ShowWindow(GlobalSettings.hTrebSliderValue, SW_HIDE);
	ShowWindow(GlobalSettings.hTrebStartButton, SW_HIDE);
	ShowWindow(GlobalSettings.hTrebDurationBox, SW_HIDE);
	InvalidateRect(GlobalSettings.hMainWindow, NULL, FALSE);
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
	GlobalSettings.SelectedTab = GlobalSettings.hSiegeTab[2];
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
	if ( hWnd == GlobalSettings.hTrebStartButton )
		TrebStartBot();
}

int TrebuchetFormBuilder()
{
	char buffer[32];

	int nPadding; // Padding between elements
	int nRangeY, nRangeWidth, nRangeHeight, nRangeStaticY, nRangeStaticWidth; // Range edit and static
	int nRechargeY, nRechargeWidth, nRechargeHeight, nRechargeStaticX, nRechargeStaticY, nRechargeStaticWidth;
	int nMinX, nSMinX; // Minimum range edit box
	int nMaxX, nSMaxX; // Maximum range edit box
	int nTSCBx, nTSCBy, nTSCBwidth, nTSCBheight; // Scrollbar position
	int nTVRx, nTVRy, nTVRwidth, nTVRheight; // Value static position
	int nTSBx, nTSBwidth, nTSBheight; // Start button position
	int nTDBx, nTDBy, nTDBwidth, nTDBheight; // Duration static position

	nPadding = 20;

	nRangeY = DEFBUT_HEIGHT + nPadding;
	nRangeWidth = nPadding * 2;
	nRangeHeight = TAB_CHAT_HEIGHT ? TAB_CHAT_HEIGHT : TAB_STATIC_HEIGHT;
	nRangeStaticY = nRangeY + 2;
	nRangeStaticWidth = 60;

	nMinX = nPadding;
	nSMinX = nMinX + nRangeWidth + 5;

	nMaxX = nSMinX + nRangeStaticWidth + nPadding;
	nSMaxX = nMaxX + nRangeWidth + 5;

	// Scroll Bar position
	nTSCBx = nPadding;
	nTSCBy = (int)(nPadding / 2) + nRangeY + nRangeHeight;
	nTSCBwidth = UserSettings.ClientWidth - (nPadding * 7);
	nTSCBheight = (nPadding / 2);

	// Recharge Edit
	nRechargeY = (nPadding / 2) + nTSCBy + nTSCBheight;
	nRechargeWidth = nPadding *2;
	nRechargeHeight = TAB_CHAT_HEIGHT ? TAB_CHAT_HEIGHT : TAB_STATIC_HEIGHT;
	nRechargeStaticX = nPadding + nRechargeWidth + 5;
	nRechargeStaticY = nRechargeY + 2;
	nRechargeStaticWidth = 60;

	// Value Readout position
	nTVRx = nTSCBx + nTSCBwidth + (nPadding / 2);
	nTVRy = nTSCBy;
	nTVRwidth = nPadding * 5;
	nTVRheight = nPadding;

	// Start button position
	nTSBwidth = DEFBUT_WIDTH;
	nTSBheight = DEFBUT_HEIGHT;
	nTSBx = (UserSettings.ClientWidth - nPadding) - DEFBUT_WIDTH;
	g_nSBy = (UserSettings.ClientHeight - nPadding) - DEFBUT_HEIGHT - TAB_CHAT_HEIGHT;

	// Duration timer position
	nTDBx = nPadding;
	nTDBy = (UserSettings.ClientHeight - nPadding) - abs(nPadding / 2) - TAB_CHAT_HEIGHT;
	nTDBwidth = UserSettings.ClientWidth - (nPadding * 2) - DEFBUT_WIDTH;
	nTDBheight = nPadding;

	// Minimum range edit box
	memset(buffer, 0, sizeof(buffer));
	_snprintf(buffer, sizeof(buffer)-1, "%lu", UserSettings.TrebMin);
	GlobalSettings.hTrebMinEdit = CreateWindow(
		TEXT("edit"), NULL,
		WS_CHILDWINDOW | WS_VISIBLE | ES_RIGHT | WS_BORDER | ES_AUTOHSCROLL,
		nMinX, nRangeY, nRangeWidth, nRangeHeight,
		GlobalSettings.hMainWindow, NULL, GlobalSettings.hInstance, NULL);
	hMinStatic = CreateWindow(
		TEXT("static"), "Minimum",
		WS_CHILD | SS_EDITCONTROL,
		nSMinX, nRangeStaticY, nRangeStaticWidth, nRangeHeight,
		GlobalSettings.hMainWindow, NULL, GlobalSettings.hInstance, NULL);
	SendMessage(GlobalSettings.hTrebMinEdit, EM_SETLIMITTEXT, (WPARAM)MAXRNG, 0);
	SendMessage(GlobalSettings.hTrebMinEdit, WM_SETFONT, (WPARAM)GlobalSettings.hFont, 0);
	SendMessage(GlobalSettings.hTrebMinEdit, WM_SETTEXT, 0, (LPARAM)buffer);
	SendMessage(hMinStatic, WM_SETFONT, (WPARAM)GlobalSettings.hFont, 0);

	// Maximum range edit box
	memset(buffer, 0, sizeof(buffer));
	_snprintf(buffer, sizeof(buffer)-1, "%lu", UserSettings.TrebMax);
	GlobalSettings.hTrebMaxEdit = CreateWindow(
		TEXT("edit"), NULL,
		WS_CHILDWINDOW | ES_RIGHT | WS_BORDER | ES_AUTOHSCROLL,
		nMaxX, nRangeY, nRangeWidth, nRangeHeight,
		GlobalSettings.hMainWindow, NULL, GlobalSettings.hInstance, NULL);
	hMaxStatic = CreateWindow(
		TEXT("static"), "Maximum",
		WS_CHILD | SS_EDITCONTROL,
		nSMaxX, nRangeStaticY, nRangeStaticWidth, nRangeHeight,
		GlobalSettings.hMainWindow, NULL, GlobalSettings.hInstance, NULL);
	SendMessage(GlobalSettings.hTrebMaxEdit, EM_SETLIMITTEXT, (WPARAM)MAXRNG, 0);
	SendMessage(GlobalSettings.hTrebMaxEdit, WM_SETFONT, (WPARAM)GlobalSettings.hFont, 0);
	SendMessage(GlobalSettings.hTrebMaxEdit, WM_SETTEXT, 0, (LPARAM)buffer);
	SendMessage(hMaxStatic, WM_SETFONT, (WPARAM)GlobalSettings.hFont, 0);

	// Scroll bar
	GlobalSettings.hTrebSliderBar = CreateWindow(
		TEXT("scrollbar"), NULL,
		WS_CHILD | WS_VISIBLE | SBS_HORZ,
		nTSCBx, nTSCBy, nTSCBwidth, nTSCBheight,
		GlobalSettings.hMainWindow, NULL, GlobalSettings.hInstance, NULL);
	SetScrollRange(GlobalSettings.hTrebSliderBar, SB_CTL, UserSettings.TrebMin, UserSettings.TrebMax, FALSE);
	SetScrollPos(GlobalSettings.hTrebSliderBar, SB_CTL, UserSettings.TrebRange, FALSE);

	// Value readout
	GlobalSettings.hTrebSliderValue = CreateWindow(
		TEXT("static"), "Range: 0.00",
		WS_CHILD | WS_VISIBLE | SS_EDITCONTROL,
		nTVRx, nTVRy, nTVRwidth, nTVRheight,
		GlobalSettings.hMainWindow, NULL, GlobalSettings.hInstance, NULL);
	memset(buffer, 0, sizeof(buffer));
	_snprintf(buffer, sizeof(buffer)-1, "Range: %.2f", (((float)(UserSettings.TrebRange - UserSettings.TrebMin) / (float)(UserSettings.TrebMax - UserSettings.TrebMin)) * 100) );
	SendMessage(GlobalSettings.hTrebSliderValue, WM_SETFONT, (WPARAM)GlobalSettings.hFont, 0);
	SendMessage(GlobalSettings.hTrebSliderValue, WM_SETTEXT, 0, (LPARAM)buffer);

	// Recharge edit box
	memset(buffer, 0, sizeof(buffer));
	_snprintf(buffer, sizeof(buffer)-1, "%lu", UserSettings.TrebRecharge);
	GlobalSettings.hTrebRecharge = CreateWindow(
		TEXT("edit"), NULL,
		WS_CHILDWINDOW | ES_RIGHT | WS_BORDER | ES_AUTOHSCROLL,
		nPadding, nRechargeY, nRechargeWidth, nRechargeHeight,
		GlobalSettings.hMainWindow, NULL, GlobalSettings.hInstance, NULL);
	hRechargeStatic = CreateWindow(
		TEXT("static"), "Recharge",
		WS_CHILD | SS_EDITCONTROL,
		nRechargeStaticX, nRechargeStaticY, nRechargeStaticWidth, nRangeHeight,
		GlobalSettings.hMainWindow, NULL, GlobalSettings.hInstance, NULL);
	SendMessage(GlobalSettings.hTrebRecharge, EM_SETLIMITTEXT, (WPARAM)MAXRNG, 0);
	SendMessage(GlobalSettings.hTrebRecharge, WM_SETFONT, (WPARAM)GlobalSettings.hFont, 0);
	SendMessage(GlobalSettings.hTrebRecharge, WM_SETTEXT, 0, (LPARAM)buffer);
	SendMessage(hRechargeStatic, WM_SETFONT, (WPARAM)GlobalSettings.hFont, 0);

	// Start button
	GlobalSettings.hTrebStartButton = CreateWindow(
		TEXT("button"), "Start",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		nTSBx, g_nSBy, nTSBwidth, nTSBheight,
		GlobalSettings.hMainWindow, NULL, GlobalSettings.hInstance, NULL);
	SendMessage(GlobalSettings.hTrebStartButton, WM_SETFONT, (WPARAM)GlobalSettings.hFont, 0);

	// Duration box
	GlobalSettings.hTrebDurationBox = CreateWindow(
		TEXT("static"), "Runtime: 0.00s",
		WS_CHILD | WS_VISIBLE | SS_EDITCONTROL,
		nTDBx, nTDBy, nTDBwidth, nTDBheight,
		GlobalSettings.hMainWindow, NULL, GlobalSettings.hInstance, NULL);
	SendMessage(GlobalSettings.hTrebDurationBox, WM_SETFONT, (WPARAM)GlobalSettings.hFont, 0);

	return 1;
}