#include "includes.h"

void SetWindowTopmost()
{
	if ( UserSettings.OnTop )
	{
		SetWindowPos(GlobalSettings.hMainWindow, HWND_TOPMOST,
			UserSettings.MainX, UserSettings.MainY,
			UserSettings.MainWidth, UserSettings.MainHeight, 0);
	}
	else
	{
		SetWindowPos(GlobalSettings.hMainWindow, HWND_NOTOPMOST,
			UserSettings.MainX, UserSettings.MainY,
			UserSettings.MainWidth, UserSettings.MainHeight, 0);
	}
}

void SetWindowState()
{
	if ( UserSettings.EnableChat )
	{
		TAB_CHAT_HEIGHT = TAB_STATIC_HEIGHT;
		ShowWindow(GlobalSettings.hChatButton, TRUE);
		ShowWindow(GlobalSettings.hChatWindow, TRUE);
		UpdateWindow(GlobalSettings.hChatButton);
		UpdateWindow(GlobalSettings.hChatWindow);
		UserSettings.MainHeight = (250 + GetSystemMetrics(SM_CYMENU)) + TAB_CHAT_HEIGHT;
		InvalidateRect(GlobalSettings.hMainWindow, NULL, FALSE);
	}
	else
	{
		TAB_CHAT_HEIGHT = 0;
		ShowWindow(GlobalSettings.hChatButton, FALSE);
		ShowWindow(GlobalSettings.hChatWindow, FALSE);
		UpdateWindow(GlobalSettings.hChatButton);
		UpdateWindow(GlobalSettings.hChatWindow);
		UserSettings.MainHeight = 250 + GetSystemMetrics(SM_CYMENU);
		InvalidateRect(GlobalSettings.hMainWindow, NULL, FALSE);
	}
	SetWindowTopmost();
}

// Main window procedure to control the program
LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch ( msg )
	{
		// Process click messages
		case WM_COMMAND:
		{
			ShowTab((HWND)lParam); // Set tab states
			FlameRamCheckState((HWND)lParam); // Check processing for Flame Ram tab and elements
			CatapultCheckState((HWND)lParam); // Check processing for Catapult tab and elements
			TrebuchetCheckState((HWND)lParam); // Check processing for Trbuchet tab and elements
			MenuCheckState(hWnd, msg, wParam, lParam);
			if ( ((HWND)lParam) == GlobalSettings.hChatButton )
				SendChat();
			break;
		}
		// Process Scrollbar messages
		case WM_HSCROLL:
		{
			int nValue;
			char buffer[32];

			// Set global values for Scrollbar
			if ( ((HWND)lParam) == g_hCatSliderBar )
				nValue = UserSettings.CatRange;
			else if ( ((HWND)lParam) == g_hTrebSliderBar )
				nValue = UserSettings.TrebRange;
			else
				break;

			// Process Scrollbar message types
			switch ( LOWORD(wParam) )
			{
				case SB_PAGELEFT:
				case SB_LINELEFT:
					if ( (nValue - 1) >= 0 ) nValue--; // Decrease by 1 unit
					break;

				case SB_PAGERIGHT:
				case SB_LINERIGHT:
					if ( ((HWND)lParam) == g_hCatSliderBar )
					{
						if ( (nValue + 1) <= CATMAXRNG ) nValue++; // Increase by 1 unit
					}
					else if ( ((HWND)lParam) == g_hTrebSliderBar )
					{
						if ( (nValue + 1) <= TREBMAXRNG ) nValue++; // Increase by 1 unit
					}
					break;

				case SB_LEFT:
				case SB_RIGHT:
				case SB_THUMBPOSITION:
				case SB_THUMBTRACK:
					nValue = HIWORD(wParam); // Set nValue to selected value
					break;

				default:
					break;
			}
			// Set Scrollbar selector to nValue
			SetScrollPos((HWND)lParam, SB_CTL, nValue, TRUE);

			// Set range readout for Catapult Scrollbar
			if ( ((HWND)lParam) == g_hCatSliderBar )
			{
				UserSettings.CatRange = nValue;

				memset(buffer, 0, sizeof(buffer));
				// Calculate range to the 10ths
				_snprintf(buffer, sizeof(buffer)-1, "Range: %.2f", (((float)UserSettings.CatRange / (float)CATMAXRNG) * 100));
				SendMessage(g_hCatSliderValue, WM_SETTEXT, 0, (LPARAM)buffer);
				// Set Catapult state values
				CatapultCheckState((HWND)lParam);
			}
			// Set range readout for Trebuchet Scrollbar
			else if ( ((HWND)lParam) == g_hTrebSliderBar )
			{
				UserSettings.TrebRange = nValue;

				memset(buffer, 0, sizeof(buffer));
				// Calculate range to the 10ths
				_snprintf(buffer, sizeof(buffer)-1, "Range: %.2f", (((float)UserSettings.TrebRange / (float)TREBMAXRNG) * 100));
				SendMessage(g_hTrebSliderValue, WM_SETTEXT, 0, (LPARAM)buffer);
				// Set Trebuchet state values
				TrebuchetCheckState((HWND)lParam);
			}

			break;
		}
		// Build main window and relocate to last known position
		case WM_CREATE:
		{
			WINDOWPLACEMENT wp;
			HICON hMainIcon;

			// Load main program icon for window
			hMainIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MAINICON));
			SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hMainIcon);

			// Restore last known coordinates
			memset(&wp, 0, sizeof(wp));
			wp.length = sizeof(WINDOWPLACEMENT);
			GetWindowPlacement(hWnd, &wp);
			UserSettings.MainX = wp.rcNormalPosition.left;
			UserSettings.MainY = wp.rcNormalPosition.top;

			break;
		} //WM_CREATE
		case WM_SIZE:
		{
			SetWindowState();
			break;
		}
		// Get window placement information
		case WM_MOVE:
		{
			WINDOWPLACEMENT wp;

			wp.length = sizeof(WINDOWPLACEMENT);
			GetWindowPlacement(hWnd, &wp);
			UserSettings.MainX = wp.rcNormalPosition.left;
			UserSettings.MainY = wp.rcNormalPosition.top;
			break;
		}
		// Cleanup threads and windows, and save settings
		case WM_CLOSE:
			SaveSettings();
			if ( GlobalSettings.dwThreadId )
				TerminateThread(GlobalSettings.hThread, 0);
			DestroyWindow(hWnd);
			break;
		// Cleanup threads and windows, and save settings
		case WM_DESTROY:
			SaveSettings();
			if ( GlobalSettings.dwThreadId )
				TerminateThread(GlobalSettings.hThread, 0);
			PostQuitMessage(0);
			break;
		// Pass all other messages
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

// Builds the GSiegeWindowClass main class and registers it
HWND MainWindowClassBuilder()
{
	WNDCLASSEX wc;
	RECT rc;

	// Load user settings
	LoadSettings();
	UserSettings.MainWidth = 300;
	if ( UserSettings.EnableChat )
	{
		TAB_CHAT_HEIGHT = TAB_STATIC_HEIGHT;
		UserSettings.MainHeight = (250 + GetSystemMetrics(SM_CYMENU)) + TAB_CHAT_HEIGHT;
	}
	else
	{
		TAB_CHAT_HEIGHT = 0;
		UserSettings.MainHeight = 250 + GetSystemMetrics(SM_CYMENU);
	}

	// Define class properties and register.
	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.style			= 0;
	wc.lpfnWndProc		= MainWindowProc;
    wc.cbClsExtra		= 0;
    wc.cbWndExtra		= 0;
    wc.hInstance		= GlobalSettings.hInstance;
    wc.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground	= (HBRUSH)(COLOR_WINDOW);
    wc.lpszMenuName		= GSIEGE_MENU_NAME;
    wc.lpszClassName	= GSIEGE_MAIN_CLASS;
    wc.hIconSm			= LoadIcon(NULL, IDI_APPLICATION);

	if( !RegisterClassEx(&wc) )
	{
		MessageBox(NULL, "Failed to register g_szMainWindow class.", "Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	// Build main window.
	GlobalSettings.hMainWindow = CreateWindowEx(
		WS_EX_WINDOWEDGE,
		GSIEGE_MAIN_CLASS,
		GSIEGE_VERSION_STRING,
		WS_OVERLAPPEDWINDOW,
		(UserSettings.MainX ? UserSettings.MainX : CW_USEDEFAULT),
		(UserSettings.MainY ? UserSettings.MainY : CW_USEDEFAULT),
		UserSettings.MainWidth, UserSettings.MainHeight,
		NULL, NULL, GlobalSettings.hInstance, NULL);

	if( GlobalSettings.hMainWindow == NULL )
	{
		MessageBox(NULL, "Failed to create g_hMainWindow.", "Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	// Load menu items
	GlobalSettings.hMenu = GetMenu(GlobalSettings.hMainWindow);

	// Set window position in z-order
	SetWindowState();
	SetMenuState();

	memset(&rc, 0, sizeof(rc));
	GetClientRect(GlobalSettings.hMainWindow, &rc);
	UserSettings.ClientWidth = rc.right - rc.left;
	UserSettings.ClientHeight = rc.bottom - rc.top;

	// Draw main window.
	ShowWindow(GlobalSettings.hMainWindow, GlobalSettings.nCmdShow);
	UpdateWindow(GlobalSettings.hMainWindow);
	return GlobalSettings.hMainWindow;
}
