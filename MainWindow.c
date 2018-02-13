#include "includes.h"

void LoadSettings()
{
	FILE *fp;
	unsigned char buffer[4096];
	unsigned char *p;

	if ( (fp = fopen("GSiege.dat", "rb")) == NULL )
		return;
	fread(buffer, sizeof(buffer[0]), sizeof(buffer), fp);
	fclose(fp);

	p = buffer;
	memcpy(&g_nMainX, p, sizeof(g_nMainX));
	
	p += sizeof(g_nMainX);
	memcpy(&g_nMainY, p, sizeof(g_nMainY));

	p += sizeof(g_nMainY);
	memcpy(&g_nRamMastery, p, sizeof(g_nRamMastery));
}

void SaveSettings()
{
	FILE *fp;
	unsigned char buffer[4096];
	unsigned char *p;
	unsigned int count;

	count = 0;

	p = buffer;
	memcpy(p, &g_nMainX, sizeof(g_nMainX));
	count += sizeof(g_nMainX);
	
	p += sizeof(g_nMainX);
	memcpy(p, &g_nMainY, sizeof(g_nMainY));
	count += sizeof(g_nMainY);

	p += sizeof(g_nMainY);
	memcpy(p, &g_nRamMastery, sizeof(g_nRamMastery));
	count += sizeof(g_nRamMastery);

	if ( (fp = fopen("GSiege.dat", "wb")) != NULL )
	{
		fwrite(buffer, sizeof(buffer[0]), count, fp);
		fclose(fp);
	}
}

LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch ( msg )
	{
		case WM_COMMAND:
		{
			ShowTab((HWND)lParam);
			FlameRamCheckState((HWND)lParam);
			CatapultCheckState((HWND)lParam);
			TrebuchetCheckState((HWND)lParam);
			break;
		}
		case WM_HSCROLL:
		{
			int nValue;
			char buffer[32];

			if ( ((HWND)lParam) == g_hCatSliderBar )
				nValue = g_nCatRange;
			else if ( ((HWND)lParam) == g_hTrebSliderBar )
				nValue = g_nTrebRange;
			else
				break;


			switch ( LOWORD(wParam) )
			{
				case SB_PAGELEFT:
				case SB_LINELEFT:
					nValue--;
					break;

				case SB_PAGERIGHT:
				case SB_LINERIGHT:
					nValue++;
					break;

				case SB_LEFT:
				case SB_RIGHT:
				case SB_THUMBPOSITION:
				case SB_THUMBTRACK:
					nValue = HIWORD(wParam);
					break;

				default:
					break;
			}
			SetScrollPos((HWND)lParam, SB_CTL, nValue, TRUE);

			if ( ((HWND)lParam) == g_hCatSliderBar )
			{
				g_nCatRange = nValue;

				memset(buffer, 0, sizeof(buffer));
				_snprintf(buffer, sizeof(buffer)-1, "Range: %.2f", (((float)g_nCatRange / (float)CATMAXRNG) * 100));
				SendMessage(g_hCatSliderValue, WM_SETTEXT, 0, (LPARAM)buffer);

				CatapultCheckState((HWND)lParam);
			}
			else if ( ((HWND)lParam) == g_hTrebSliderBar )
			{
				g_nTrebRange = nValue;

				memset(buffer, 0, sizeof(buffer));
				_snprintf(buffer, sizeof(buffer)-1, "Range: %.2f", (((float)g_nTrebRange / (float)TREBMAXRNG) * 100));
				SendMessage(g_hTrebSliderValue, WM_SETTEXT, 0, (LPARAM)buffer);

				TrebuchetCheckState((HWND)lParam);
			}

			break;
		}
		case WM_CREATE:
		{
			WINDOWPLACEMENT wp;
			HICON hMainIcon;

			hMainIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MAINICON));
			SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hMainIcon);
			wp.length = sizeof(WINDOWPLACEMENT);
			GetWindowPlacement(hWnd, &wp);
			g_nMainX = wp.rcNormalPosition.left;
			g_nMainY = wp.rcNormalPosition.top;
			break;
		}
		case WM_SIZE:
		{
			WINDOWPLACEMENT wp;

			wp.length = sizeof(WINDOWPLACEMENT);
			GetWindowPlacement(hWnd, &wp);
			g_nMainX = wp.rcNormalPosition.left;
			g_nMainY = wp.rcNormalPosition.top;
			SetWindowPos(hWnd, HWND_TOP, g_nMainX, g_nMainY, g_nMainWidth, g_nMainHeight, SWP_NOMOVE);
			break;
		}
		case WM_MOVE:
		{
			WINDOWPLACEMENT wp;

			wp.length = sizeof(WINDOWPLACEMENT);
			GetWindowPlacement(hWnd, &wp);
			g_nMainX = wp.rcNormalPosition.left;
			g_nMainY = wp.rcNormalPosition.top;
			break;
		}
		case WM_CLOSE:
			SaveSettings();
			if ( g_dwThreadId )
				ExitThread(g_dwThreadId);
			DestroyWindow(hWnd);
			break;
		case WM_DESTROY:
			SaveSettings();
			if ( g_dwThreadId )
				ExitThread(g_dwThreadId);
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

HWND MainWindowClassBuilder()
{
	WNDCLASSEX wc;

	// Initialise global parameters.
	memset(g_szMainWindow, 0, sizeof(g_szMainWindow));
	memset(g_szMainTitle, 0, sizeof(g_szMainTitle));
	strncpy(g_szMainWindow, "GSiegeWindowClass", sizeof(g_szMainWindow)-1);
	strncpy(g_szMainTitle, "GSiege v1.0", sizeof(g_szMainTitle)-1);
	g_nMainWidth = 300;
	g_nMainHeight = 250;
	g_nMainX = 0;
	g_nMainY = 0;

	// Load user settings
	LoadSettings();

	// Define class properties and register.
	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.style			= 0;
	wc.lpfnWndProc		= MainWindowProc;
    wc.cbClsExtra		= 0;
    wc.cbWndExtra		= 0;
    wc.hInstance		= g_hInstance;
    wc.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground	= (HBRUSH)(COLOR_WINDOW);
    wc.lpszMenuName		= NULL;
    wc.lpszClassName	= g_szMainWindow;
    wc.hIconSm			= LoadIcon(NULL, IDI_APPLICATION);

	if( !RegisterClassEx(&wc) )
	{
		MessageBox(NULL, "Failed to register g_szMainWindow class.", "Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	// Build main window.
	g_hMainWindow = CreateWindowEx(
		WS_EX_WINDOWEDGE | WS_EX_TOPMOST,
		g_szMainWindow,
		g_szMainTitle,
		WS_OVERLAPPEDWINDOW,
		(g_nMainX ? g_nMainX : CW_USEDEFAULT),
		(g_nMainY ? g_nMainY : CW_USEDEFAULT),
		g_nMainWidth, g_nMainHeight,
		NULL, NULL, g_hInstance, NULL);

	if( g_hMainWindow == NULL )
	{
		MessageBox(NULL, "Failed to create g_hMainWindow.", "Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	// Draw main window.
	ShowWindow(g_hMainWindow, g_nCmdShow);
	UpdateWindow(g_hMainWindow);
	return g_hMainWindow;
}
