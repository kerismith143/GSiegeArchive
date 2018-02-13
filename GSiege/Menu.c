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
	if ( UserSettings.EnableAdv )
		CheckMenuItem(GlobalSettings.hMenu, IDM_OPT_ADV, MF_CHECKED);
	else if ( !UserSettings.EnableAdv )
		CheckMenuItem(GlobalSettings.hMenu, IDM_OPT_ADV, MF_UNCHECKED);
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
				case IDM_OPT_ADV:
				{
					if ( UserSettings.EnableAdv ) // if on
					{
						UserSettings.EnableAdv = 0; // turn it off
						SetWindowState();
						SetMenuState();
					}
					else // if off
					{
						UserSettings.EnableAdv = 1; // turn it on
						SetWindowState();
						SetMenuState();
					}
					break;
				} // case IDM_OPT_ADV:
				case IDM_OPT_HELP:
				{
					char buffer[2048];

					memset(buffer, 0, sizeof(buffer));
					_snprintf(buffer, sizeof(buffer)-1,
						"%s\nCoded by: pushbyte\n\n" \
						"1. Set whether you have Flame Ram Mastery III or greater.\n" \
						"OR\n" \
						"1. Set range (in seconds) for Catapult or Trebuchet, as desired, using the slider bar.\n" \
						"\n2. Press Start button.\n" \
						"\n\nAdvanced:\n" \
						"-----------------------" \
						"\nOffset is the maximum amount of time to wait randomly between skill usage. " \
						"This feature is suggested for bypassing some simple detections.\n(Default: 5 nanoseconds)\n" \
						"\nMinimum and Maximum values for Catapult and Trebuchet determine the valid range of values to fire within. " \
						"This permits you to tweak the range settings from default.\n(Default: Catapult Max %lu, Trebuchet Max %lu)\n" \
						"\nRecharge is the time to wait inbetween skill usage for each piece of siege.\n" \
						"(Default: Ram Mastery %lu, No Ram Mastery %lu, Catapult/Trebuchet %lu)",
						GSIEGE_VERSION_STRING, CATMAXRNG, TREBMAXRNG, RAMMASTER, RAMNOMASTER, CATTREB);
					MessageBox(NULL, buffer, "Help", MB_OK);
				}
				default:
					return 0;
			} //switch ( LOWORD(wParam) )
		default:
			return 0;
	} //switch ( msg )
	return 0;
}
