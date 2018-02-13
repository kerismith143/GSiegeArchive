#include "includes.h"

char *lpszTabNames[] = {
	"Flame Ram",
	"Catapult",
	"Trebuchet"
};

void SetStartStop(int nStartStop)
{
	char buffer[32];

	if ( nStartStop == SHOWSTOP )
		strcpy(buffer, "Stop");
	else
		strcpy(buffer, "Start");
	SendMessage(g_hRamStartButton, WM_SETTEXT, 0, (LPARAM)buffer);
	SendMessage(g_hCatStartButton, WM_SETTEXT, 0, (LPARAM)buffer);
	SendMessage(g_hTrebStartButton, WM_SETTEXT, 0, (LPARAM)buffer);
}

int GetStartStop()
{
	char buffer[32];

	memset(buffer, 0, sizeof(buffer));
	SendMessage(g_hRamStartButton, WM_GETTEXT, 0, (LPARAM)buffer);
	if ( !strcmp(buffer, "Stop") )
		return 0;
	return 1;
}

int ShowTab(HWND hWnd)
{
	// Toggle relevant tabs
	if ( hWnd == g_hSiegeTab[0] ) // Flame Ram tab
		FlameRamToggleElements();
	else if ( hWnd == g_hSiegeTab[1] ) // Catapult tab
		CatapultToggleElements();
	else if ( hWnd == g_hSiegeTab[2] ) // Trebuchet tab
		TrebuchetToggleElements();
	else
		return 0;
	return 1;
}

HWND TabButtonBuilder(int nButtonIndex, int nMaxButtons, char *szText)
{
	HWND hWnd;
	int nButtonX, nButtonY, nButtonHeight, nButtonWidth;

	// Set button element parameters
	nButtonX = abs(g_nMainClientWidth / nMaxButtons) * nButtonIndex;
	nButtonY = 0;
	nButtonWidth = abs(g_nMainClientWidth / nMaxButtons);
	nButtonHeight = DEFBUT_HEIGHT;

	// Build new tab
	hWnd = CreateWindow(
		TEXT("button"), szText,
		WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
		nButtonX, nButtonY, nButtonWidth, nButtonHeight,
		g_hMainWindow, NULL, g_hInstance, NULL);

	if ( hWnd == NULL )
	{
		MessageBox(NULL, "Failed to create tab array.", "Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	SendMessage(hWnd, WM_SETFONT, (WPARAM)hFont, 0);
	
	return hWnd;
}

void TabWindowClassBuilder()
{
	int n;

	// Build tab buttons.
	for ( n = 0; n < MAXTABS; n++ )
		g_hSiegeTab[n] = TabButtonBuilder(n, MAXTABS, lpszTabNames[n]);
}