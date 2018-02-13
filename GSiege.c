#include "includes.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	RECT rc;

	// Initialise global variables
	g_hInstance = hInstance;
	g_hPrevInstance = hPrevInstance;
	g_nCmdShow = nCmdShow;
	g_dwThreadId = 0;

	// Build main window class and set font
	MainWindowClassBuilder();
	hFont = CreateFont(11,6,0,0,0,0,0,0,0,0,0,0,0,TEXT("Tahoma"));
	hBoldFont = CreateFont(11,6,0,0,FW_BOLD,0,0,0,0,0,0,0,0,TEXT("Tahoma"));

	// Check and set global window dimensions
	memset(&rc, 0, sizeof(rc));
	GetWindowRect(g_hMainWindow, &rc);
	g_nMainX = rc.left;
	g_nMainY = rc.top;
	g_nMainWidth = rc.right - rc.left;
	g_nMainHeight = rc.bottom - rc.top;

	// Check and set global window client dimensions
	memset(&rc, 0, sizeof(rc));
	GetClientRect(g_hMainWindow, &rc);
	g_nMainClientWidth = rc.right - rc.left;
	g_nMainClientHeight = rc.bottom - rc.top;

	// Build child windows.
	TabWindowClassBuilder();
	FlameRamFormBuilder();
	CatapultFormBuilder();
	TrebuchetFormBuilder();
	FlameRamToggleElements();


	// Start main procedure loop
	while ( GetMessage(&msg, NULL, 0, 0) > 0 )
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// Cleanup before exit
	DeleteObject(hFont);
	DeleteObject(hBoldFont);

	return msg.wParam;
}
