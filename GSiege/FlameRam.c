#include "includes.h"

HWND hRechargeStatic;

int FlameRamHideElements()
{
	ShowWindow(GlobalSettings.hRamDurationBox, SW_HIDE);
	ShowWindow(GlobalSettings.hRamStartButton, SW_HIDE);
	ShowWindow(GlobalSettings.hRamMasteryCheckbox, SW_HIDE);
	ShowWindow(GlobalSettings.hRamRecharge, SW_HIDE);
	ShowWindow(hRechargeStatic, SW_HIDE);
	InvalidateRect(GlobalSettings.hMainWindow, NULL, FALSE);
	return 1;
}

int FlameRamShowElements()
{
	if ( UserSettings.EnableAdv ) ShowWindow(GlobalSettings.hRamRecharge, GlobalSettings.nCmdShow);
	else ShowWindow(GlobalSettings.hRamRecharge, SW_HIDE);
	if ( UserSettings.EnableAdv ) ShowWindow(hRechargeStatic, GlobalSettings.nCmdShow);
	else ShowWindow(hRechargeStatic, SW_HIDE);
	UpdateWindow(GlobalSettings.hRamRecharge);
	UpdateWindow(hRechargeStatic);

	ShowWindow(GlobalSettings.hRamDurationBox, GlobalSettings.nCmdShow);
	UpdateWindow(GlobalSettings.hRamDurationBox);

	ShowWindow(GlobalSettings.hRamStartButton, GlobalSettings.nCmdShow);
	UpdateWindow(GlobalSettings.hRamStartButton);

	ShowWindow(GlobalSettings.hRamMasteryCheckbox, GlobalSettings.nCmdShow);
	UpdateWindow(GlobalSettings.hRamMasteryCheckbox);

	InvalidateRect(GlobalSettings.hMainWindow, NULL, FALSE);
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
	GlobalSettings.SelectedTab = GlobalSettings.hSiegeTab[0];
	for ( n = 0; n < MAXTABS; n++ )
		InvalidateRect(GlobalSettings.hSiegeTab[n], NULL, TRUE);
}

void FlameRamSetMastery()
{
	int nState;
	char buffer[32];

	memset(buffer, 0, sizeof(buffer));
	nState = SendMessage(GlobalSettings.hRamMasteryCheckbox, BM_GETCHECK, 0, 0);
	switch ( nState )
	{
		case BST_UNCHECKED:
			UserSettings.RamMastery = 0;
			_snprintf(buffer, sizeof(buffer)-1, "%lu", UserSettings.RamNoMastRecharge);
			SendMessage(GlobalSettings.hRamRecharge, WM_SETTEXT, 0, (LPARAM)buffer);
			break;
		case BST_CHECKED:
			UserSettings.RamMastery = 1;
			_snprintf(buffer, sizeof(buffer)-1, "%lu", UserSettings.RamMastRecharge);
			SendMessage(GlobalSettings.hRamRecharge, WM_SETTEXT, 0, (LPARAM)buffer);
			break;
		default:
			UserSettings.RamMastery = 0;
			_snprintf(buffer, sizeof(buffer)-1, "%lu", UserSettings.RamNoMastRecharge);
			SendMessage(GlobalSettings.hRamRecharge, WM_SETTEXT, 0, (LPARAM)buffer);
			SendMessage(GlobalSettings.hRamMasteryCheckbox, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
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

		tpp.nType = BOT_RAM;

		// Start bot thread and set form elements
		GlobalSettings.hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&TimerProc, &tpp, 0, &GlobalSettings.dwThreadId); 
		if ( GlobalSettings.hThread != NULL )
			SetStartStop(SHOWSTOP);
	}
}

void FlameRamCheckState(HWND hWnd)
{
	if ( hWnd == GlobalSettings.hRamMasteryCheckbox )
		FlameRamSetMastery();
	else if ( hWnd == GlobalSettings.hRamStartButton )
		FlameRamStartBot();
}

int FlameRamFormBuilder()
{
	char buffer[32];
	int nPadding; // Padding between elements
	int nMasteryCheckY, nMasteryCheckWidth, nMasteryCheckHeight; // Checkbox button position
	int nRechargeY, nRechargeWidth, nRechargeHeight, nOffsetY, nOffsetHeight;
	int nRechargeStaticX, nRechargeStaticY, nRechargeStaticWidth;
	int nFRSBx, nFRSBwidth, nFRSBheight; // Start button position
	int nFRDBx, nFRDBy, nFRDBwidth, nFRDBheight; // Duration static position

	nPadding = 20;

	// Mastery checkbox position
	nMasteryCheckY = DEFBUT_HEIGHT + nPadding;
	nMasteryCheckWidth = UserSettings.ClientWidth - (nPadding * 2);
	nMasteryCheckHeight = nPadding;

	nOffsetY = (nPadding / 2) + nMasteryCheckY + (TAB_CHAT_HEIGHT ? TAB_CHAT_HEIGHT : TAB_STATIC_HEIGHT);
	nOffsetHeight = (nPadding / 2);

	// Recharge Edit
	nRechargeY = (nPadding / 2) + nOffsetY + nOffsetHeight;
	nRechargeWidth = nPadding *2;
	nRechargeHeight = TAB_CHAT_HEIGHT ? TAB_CHAT_HEIGHT : TAB_STATIC_HEIGHT;
	nRechargeStaticX = nPadding + nRechargeWidth + 5;
	nRechargeStaticY = nRechargeY + 2;
	nRechargeStaticWidth = 60;

	// Start button position
	nFRSBwidth = DEFBUT_WIDTH;
	nFRSBheight = DEFBUT_HEIGHT;
	nFRSBx = (UserSettings.ClientWidth - nPadding) - DEFBUT_WIDTH;
	g_nSBy = (UserSettings.ClientHeight - nPadding) - DEFBUT_HEIGHT - TAB_CHAT_HEIGHT;

	// Duration timer position
	nFRDBx = nPadding;
	nFRDBy = (UserSettings.ClientHeight - nPadding) - abs(nPadding / 2) - TAB_CHAT_HEIGHT;
	nFRDBwidth = UserSettings.ClientWidth - (nPadding * 2) - DEFBUT_WIDTH;
	nFRDBheight = nPadding;

	// Mastery checkbox
	GlobalSettings.hRamMasteryCheckbox = CreateWindow(
		TEXT("button"), "Flame Ram Mastery",
		WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
		nPadding, nMasteryCheckY, nMasteryCheckWidth, nMasteryCheckHeight,
		GlobalSettings.hMainWindow, NULL, GlobalSettings.hInstance, NULL);
	SendMessage(GlobalSettings.hRamMasteryCheckbox, WM_SETFONT, (WPARAM)GlobalSettings.hFont, 0);
	if ( UserSettings.RamMastery == 1 )
		SendMessage(GlobalSettings.hRamMasteryCheckbox, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);

	// Recharge edit box
	memset(buffer, 0, sizeof(buffer));
	_snprintf(buffer, sizeof(buffer)-1, "%lu",
		(UserSettings.RamMastery ? UserSettings.RamMastRecharge : UserSettings.RamNoMastRecharge));
	GlobalSettings.hRamRecharge = CreateWindow(
		TEXT("edit"), NULL,
		WS_CHILDWINDOW | ES_RIGHT | WS_BORDER | ES_AUTOHSCROLL,
		nPadding, nRechargeY, nRechargeWidth, nRechargeHeight,
		GlobalSettings.hMainWindow, NULL, GlobalSettings.hInstance, NULL);
	hRechargeStatic = CreateWindow(
		TEXT("static"), "Recharge",
		WS_CHILD | SS_EDITCONTROL,
		nRechargeStaticX, nRechargeStaticY, nRechargeStaticWidth, nRechargeHeight,
		GlobalSettings.hMainWindow, NULL, GlobalSettings.hInstance, NULL);
	SendMessage(GlobalSettings.hRamRecharge, EM_SETLIMITTEXT, (WPARAM)MAXRNG, 0);
	SendMessage(GlobalSettings.hRamRecharge, WM_SETFONT, (WPARAM)GlobalSettings.hFont, 0);
	SendMessage(GlobalSettings.hRamRecharge, WM_SETTEXT, 0, (LPARAM)buffer);
	SendMessage(hRechargeStatic, WM_SETFONT, (WPARAM)GlobalSettings.hFont, 0);

	// Start button
	GlobalSettings.hRamStartButton = CreateWindow(
		TEXT("button"), "Start",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		nFRSBx, g_nSBy, nFRSBwidth, nFRSBheight,
		GlobalSettings.hMainWindow, NULL, GlobalSettings.hInstance, NULL);
	SendMessage(GlobalSettings.hRamStartButton, WM_SETFONT, (WPARAM)GlobalSettings.hFont, 0);

	// Duration box
	GlobalSettings.hRamDurationBox = CreateWindow(
		TEXT("static"), "Runtime: 0.00s",
		WS_CHILD | WS_VISIBLE | SS_EDITCONTROL,
		nFRDBx, nFRDBy, nFRDBwidth, nFRDBheight,
		GlobalSettings.hMainWindow, NULL, GlobalSettings.hInstance, NULL);
	SendMessage(GlobalSettings.hRamDurationBox, WM_SETFONT, (WPARAM)GlobalSettings.hFont, 0);

	return 1;
}