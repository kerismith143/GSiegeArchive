#include "includes.h"

HWND hMinStatic, hMaxStatic, hOffsetStatic, hRechargeStatic;

int CatapultShowElements()
{
	if ( UserSettings.EnableAdv ) ShowWindow(GlobalSettings.hCatMinEdit, GlobalSettings.nCmdShow);
	else ShowWindow(GlobalSettings.hCatMinEdit, SW_HIDE);
	if ( UserSettings.EnableAdv ) ShowWindow(hMinStatic, GlobalSettings.nCmdShow);
	else ShowWindow(hMinStatic, SW_HIDE);
	UpdateWindow(GlobalSettings.hCatMinEdit);
	UpdateWindow(hMinStatic);

	if ( UserSettings.EnableAdv ) ShowWindow(GlobalSettings.hCatMaxEdit, GlobalSettings.nCmdShow);
	else ShowWindow(GlobalSettings.hCatMaxEdit, SW_HIDE);
	if ( UserSettings.EnableAdv ) ShowWindow(hMaxStatic, GlobalSettings.nCmdShow);
	else ShowWindow(hMaxStatic, SW_HIDE);
	UpdateWindow(GlobalSettings.hCatMaxEdit);
	UpdateWindow(hMaxStatic);

	if ( UserSettings.EnableAdv ) ShowWindow(GlobalSettings.hCatRecharge, GlobalSettings.nCmdShow);
	else ShowWindow(GlobalSettings.hCatRecharge, SW_HIDE);
	if ( UserSettings.EnableAdv ) ShowWindow(hRechargeStatic, GlobalSettings.nCmdShow);
	else ShowWindow(hRechargeStatic, SW_HIDE);
	UpdateWindow(GlobalSettings.hCatRecharge);
	UpdateWindow(hRechargeStatic);

	ShowWindow(GlobalSettings.hCatSliderBar, GlobalSettings.nCmdShow);
	UpdateWindow(GlobalSettings.hCatSliderBar);

	ShowWindow(GlobalSettings.hCatSliderValue, GlobalSettings.nCmdShow);
	UpdateWindow(GlobalSettings.hCatSliderValue);

	ShowWindow(GlobalSettings.hCatStartButton, GlobalSettings.nCmdShow);
	UpdateWindow(GlobalSettings.hCatStartButton);

	ShowWindow(GlobalSettings.hCatDurationBox, GlobalSettings.nCmdShow);
	UpdateWindow(GlobalSettings.hCatDurationBox);

	InvalidateRect(GlobalSettings.hMainWindow, NULL, FALSE);
	return 1;
}

int CatapultHideElements()
{
	ShowWindow(GlobalSettings.hCatMinEdit, SW_HIDE);
	ShowWindow(hMinStatic, SW_HIDE);
	ShowWindow(GlobalSettings.hCatMaxEdit, SW_HIDE);
	ShowWindow(hMaxStatic, SW_HIDE);
	ShowWindow(GlobalSettings.hCatRecharge, SW_HIDE);
	ShowWindow(hRechargeStatic, SW_HIDE);
	ShowWindow(GlobalSettings.hCatSliderBar, SW_HIDE);
	ShowWindow(GlobalSettings.hCatSliderValue, SW_HIDE);
	ShowWindow(GlobalSettings.hCatStartButton, SW_HIDE);
	ShowWindow(GlobalSettings.hCatDurationBox, SW_HIDE);
	InvalidateRect(GlobalSettings.hMainWindow, NULL, FALSE);
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
	GlobalSettings.SelectedTab = GlobalSettings.hSiegeTab[1];
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
	if ( hWnd == GlobalSettings.hCatStartButton )
		CatStartBot();
}

int CatapultFormBuilder()
{
	char buffer[32];

	int nPadding; // Padding between elements
	//int nCIBx, nCIBy, nCIBwidth, nCIBheight; // Information static position
	int nRangeY, nRangeWidth, nRangeHeight, nRangeStaticY, nRangeStaticWidth; // Range edit and static
	int nRechargeY, nRechargeWidth, nRechargeHeight, nRechargeStaticX, nRechargeStaticY, nRechargeStaticWidth;
	int nMinX, nSMinX; // Minimum range edit box
	int nMaxX, nSMaxX; // Maximum range edit box
	int nCSCBx, nCSCBy, nCSCBwidth, nCSCBheight; // Scrollbar position
	int nCVRx, nCVRy, nCVRwidth, nCVRheight; // Value static position
	int nCSBx, nCSBwidth, nCSBheight; // Start button position
	int nCDBx, nCDBy, nCDBwidth, nCDBheight; // Duration static position

	nPadding = 20;

	// Range Edits
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
	nCSCBx = nPadding;
	nCSCBy = (nPadding / 2) + nRangeY + nRangeHeight;
	nCSCBwidth = UserSettings.ClientWidth - (nPadding * 7);
	nCSCBheight = (nPadding / 2);

	// Recharge Edit
	nRechargeY = (nPadding / 2) + nCSCBy + nCSCBheight;
	nRechargeWidth = nPadding *2;
	nRechargeHeight = TAB_CHAT_HEIGHT ? TAB_CHAT_HEIGHT : TAB_STATIC_HEIGHT;
	nRechargeStaticX = nPadding + nRechargeWidth + 5;
	nRechargeStaticY = nRechargeY + 2;
	nRechargeStaticWidth = 60;

	// Value Readout position
	nCVRx = nCSCBx + nCSCBwidth + (nPadding / 2);
	nCVRy = nCSCBy;
	nCVRwidth = nPadding * 5;
	nCVRheight = nPadding;

	// Start button position
	nCSBwidth = DEFBUT_WIDTH;
	nCSBheight = DEFBUT_HEIGHT;
	nCSBx = (UserSettings.ClientWidth - nPadding) - DEFBUT_WIDTH;
	g_nSBy = (UserSettings.ClientHeight - nPadding) - DEFBUT_HEIGHT - TAB_CHAT_HEIGHT;

	// Duration timer position
	nCDBx = nPadding;
	nCDBy = (UserSettings.ClientHeight - nPadding) - abs(nPadding / 2) - TAB_CHAT_HEIGHT;
	nCDBwidth = UserSettings.ClientWidth - (nPadding * 2) - DEFBUT_WIDTH;
	nCDBheight = nPadding;

	// Minimum range edit box
	memset(buffer, 0, sizeof(buffer));
	_snprintf(buffer, sizeof(buffer)-1, "%lu", UserSettings.CatMin);
	GlobalSettings.hCatMinEdit = CreateWindow(
		TEXT("edit"), NULL,
		WS_CHILDWINDOW | ES_RIGHT | WS_BORDER | ES_AUTOHSCROLL,
		nMinX, nRangeY, nRangeWidth, nRangeHeight,
		GlobalSettings.hMainWindow, NULL, GlobalSettings.hInstance, NULL);
	hMinStatic = CreateWindow(
		TEXT("static"), "Minimum",
		WS_CHILD | SS_EDITCONTROL,
		nSMinX, nRangeStaticY, nRangeStaticWidth, nRangeHeight,
		GlobalSettings.hMainWindow, NULL, GlobalSettings.hInstance, NULL);
	SendMessage(GlobalSettings.hCatMinEdit, EM_SETLIMITTEXT, (WPARAM)MAXRNG, 0);
	SendMessage(GlobalSettings.hCatMinEdit, WM_SETFONT, (WPARAM)GlobalSettings.hFont, 0);
	SendMessage(GlobalSettings.hCatMinEdit, WM_SETTEXT, 0, (LPARAM)buffer);
	SendMessage(hMinStatic, WM_SETFONT, (WPARAM)GlobalSettings.hFont, 0);

	// Maximum range edit box
	memset(buffer, 0, sizeof(buffer));
	_snprintf(buffer, sizeof(buffer)-1, "%lu", UserSettings.CatMax);
	GlobalSettings.hCatMaxEdit = CreateWindow(
		TEXT("edit"), NULL,
		WS_CHILDWINDOW | WS_VISIBLE | ES_RIGHT | WS_BORDER | ES_AUTOHSCROLL,
		nMaxX, nRangeY, nRangeWidth, nRangeHeight,
		GlobalSettings.hMainWindow, NULL, GlobalSettings.hInstance, NULL);
	hMaxStatic = CreateWindow(
		TEXT("static"), "Maximum",
		WS_CHILD | SS_EDITCONTROL,
		nSMaxX, nRangeStaticY, nRangeStaticWidth, nRangeHeight,
		GlobalSettings.hMainWindow, NULL, GlobalSettings.hInstance, NULL);
	SendMessage(GlobalSettings.hCatMaxEdit, EM_SETLIMITTEXT, (WPARAM)MAXRNG, 0);
	SendMessage(GlobalSettings.hCatMaxEdit, WM_SETFONT, (WPARAM)GlobalSettings.hFont, 0);
	SendMessage(GlobalSettings.hCatMaxEdit, WM_SETTEXT, 0, (LPARAM)buffer);
	SendMessage(hMaxStatic, WM_SETFONT, (WPARAM)GlobalSettings.hFont, 0);

	// Scroll bar
	GlobalSettings.hCatSliderBar = CreateWindow(
		TEXT("scrollbar"), NULL,
		WS_CHILD | WS_VISIBLE | SBS_HORZ,
		nCSCBx, nCSCBy, nCSCBwidth, nCSCBheight,
		GlobalSettings.hMainWindow, NULL, GlobalSettings.hInstance, NULL);
	SetScrollRange(GlobalSettings.hCatSliderBar, SB_CTL, UserSettings.CatMin, UserSettings.CatMax, FALSE);
	SetScrollPos(GlobalSettings.hCatSliderBar, SB_CTL, UserSettings.CatRange, FALSE);

	// Value readout
	GlobalSettings.hCatSliderValue = CreateWindow(
		TEXT("static"), "Range: 0.00",
		WS_CHILD | WS_VISIBLE | SS_EDITCONTROL,
		nCVRx, nCVRy, nCVRwidth, nCVRheight,
		GlobalSettings.hMainWindow, NULL, GlobalSettings.hInstance, NULL);
	memset(buffer, 0, sizeof(buffer));
	_snprintf(buffer, sizeof(buffer)-1, "Range: %.2f", (((float)(UserSettings.CatRange - UserSettings.CatMin) / (float)(UserSettings.CatMax - UserSettings.CatMin)) * 100) );
	SendMessage(GlobalSettings.hCatSliderValue, WM_SETFONT, (WPARAM)GlobalSettings.hFont, 0);
	SendMessage(GlobalSettings.hCatSliderValue, WM_SETTEXT, 0, (LPARAM)buffer);

	// Recharge edit box
	memset(buffer, 0, sizeof(buffer));
	_snprintf(buffer, sizeof(buffer)-1, "%lu", UserSettings.CatRecharge);
	GlobalSettings.hCatRecharge = CreateWindow(
		TEXT("edit"), NULL,
		WS_CHILDWINDOW | ES_RIGHT | WS_BORDER | ES_AUTOHSCROLL,
		nPadding, nRechargeY, nRechargeWidth, nRechargeHeight,
		GlobalSettings.hMainWindow, NULL, GlobalSettings.hInstance, NULL);
	hRechargeStatic = CreateWindow(
		TEXT("static"), "Recharge",
		WS_CHILD | SS_EDITCONTROL,
		nRechargeStaticX, nRechargeStaticY, nRechargeStaticWidth, nRangeHeight,
		GlobalSettings.hMainWindow, NULL, GlobalSettings.hInstance, NULL);
	SendMessage(GlobalSettings.hCatRecharge, EM_SETLIMITTEXT, (WPARAM)MAXRNG, 0);
	SendMessage(GlobalSettings.hCatRecharge, WM_SETFONT, (WPARAM)GlobalSettings.hFont, 0);
	SendMessage(GlobalSettings.hCatRecharge, WM_SETTEXT, 0, (LPARAM)buffer);
	SendMessage(hRechargeStatic, WM_SETFONT, (WPARAM)GlobalSettings.hFont, 0);

	// Start button
	GlobalSettings.hCatStartButton = CreateWindow(
		TEXT("button"), "Start",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		nCSBx, g_nSBy, nCSBwidth, nCSBheight,
		GlobalSettings.hMainWindow, NULL, GlobalSettings.hInstance, NULL);
	SendMessage(GlobalSettings.hCatStartButton, WM_SETFONT, (WPARAM)GlobalSettings.hFont, 0);

	// Duration box
	GlobalSettings.hCatDurationBox = CreateWindow(
		TEXT("static"), "Runtime: 0.00s",
		WS_CHILD | WS_VISIBLE | SS_EDITCONTROL,
		nCDBx, nCDBy, nCDBwidth, nCDBheight,
		GlobalSettings.hMainWindow, NULL, GlobalSettings.hInstance, NULL);
	SendMessage(GlobalSettings.hCatDurationBox, WM_SETFONT, (WPARAM)GlobalSettings.hFont, 0);

	return 1;
}