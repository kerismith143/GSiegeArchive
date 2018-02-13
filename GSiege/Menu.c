#include "includes.h"

void SetMenuState()
{
	if ( UserSettings.OnTop )
		CheckMenuItem(GlobalSettings.hMenu, IDM_OPT_ONTOP, MF_CHECKED);
	else if ( !UserSettings.OnTop )
		CheckMenuItem(GlobalSettings.hMenu, IDM_OPT_ONTOP, MF_UNCHECKED);
	if ( UserSettings.EnableChat )
		CheckMenuItem(GlobalSettings.hMenu, IDM_OPT_CHAT, MF_CHECKED);
	else if ( !UserSettings.EnableChat )
		CheckMenuItem(GlobalSettings.hMenu, IDM_OPT_CHAT, MF_UNCHECKED);
}

int MenuCheckState(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch ( msg )
	{
		case WM_COMMAND:
			switch ( LOWORD(wParam) )
			{
				case IDM_OPT_ONTOP:
				{
					if ( UserSettings.OnTop ) // if on
					{
						UserSettings.OnTop = 0; // turn it off
						SetWindowState();
						SetMenuState();
					}
					else // if off
					{
						UserSettings.OnTop = 1; // turn it on
						SetWindowState();
						SetMenuState();
					}
					break;
				} //case IDM_OPT_ONTOP:
				case IDM_OPT_CHAT:
				{
					if ( UserSettings.EnableChat ) // if on
					{
						UserSettings.EnableChat = 0; // turn it off
						SetWindowState();
						SetMenuState();
					}
					else // if off
					{
						UserSettings.EnableChat = 1; // turn it on
						SetWindowState();
						SetMenuState();
					}
					break;
				} // case IDM_OPT_CHAT:
				default:
					return 0;
			} //switch ( LOWORD(wParam) )
		default:
			return 0;
	} //switch ( msg )
	return 0;
}
