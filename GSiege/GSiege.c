#include "includes.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;

	timeBeginPeriod(SLEEPMIN);
	SKQueInit();

	memset(&GlobalSettings, 0, sizeof(GlobalSettings));
	memset(&UserSettings, 0, sizeof(UserSettings));

	UserSettings.MajorVersion = GSIEGE_MAJOR_VERSION;
	UserSettings.MinorVersion = GSIEGE_MINOR_VERSION;

	// Initialise global variables
	GlobalSettings.hInstance = hInstance;
	GlobalSettings.hPrevInstance = hPrevInstance;
	GlobalSettings.nCmdShow = nCmdShow;
	GlobalSettings.dwThreadId = 0;

	// Set font
	GlobalSettings.hFont = CreateFont(11,6,0,0,0,0,0,0,0,0,0,0,0,TEXT("Tahoma"));
	GlobalSettings.hBoldFont = CreateFont(11,6,0,0,FW_BOLD,0,0,0,0,0,0,0,0,TEXT("Tahoma"));

	// Build windows
	MainWindowClassBuilder();
	TabWindowClassBuilder();
	FlameRamFormBuilder();
	CatapultFormBuilder();
	TrebuchetFormBuilder();
	ChatWindowClassBuilder();
	FlameRamToggleElements();

	// Start main procedure loop
	while ( GetMessage(&msg, NULL, 0, 0) > 0 )
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// Cleanup before exit
	if ( GlobalSettings.dwThreadId )
		TerminateThread(GlobalSettings.hThread, 0);
	DeleteObject(GlobalSettings.hFont);
	DeleteObject(GlobalSettings.hBoldFont);

	timeEndPeriod(SLEEPMIN);

	return msg.wParam;
}
