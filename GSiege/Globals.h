#ifndef __GSIEGE_GLOBALS_H__
#define __GSIEGE_GLOBALS_H__

// Guild Wars 2 main window class name
#define FINDCLASS	"ArenaNet_Dx_Window_Class"

#define SLEEPMIN	50

// Global Flame Ram handles
HWND g_hRamStartButton;
HWND g_hRamDurationBox;

// Trebuchet global window handles
HWND g_hTrebStartButton;
HWND g_hTrebDurationBox;
HWND g_hTrebSliderBar;
HWND g_hTrebSliderValue;

// Global Catapult tab handles
HWND g_hCatStartButton;
HWND g_hCatDurationBox;
HWND g_hCatSliderBar;
HWND g_hCatSliderValue;

int g_nSBy;

#define MAXTABS			3 // Maximum number of tabs to display
#define GSIEGE_DAT		"GSiege.dat" // Name of settings file
#define MAXTEXT			100 // Maximum number of characters for chat with NULL terminator

typedef struct tagGLOBALSETTINGS
{
	HINSTANCE hInstance;
	HINSTANCE hPrevInstance;
	int nCmdShow;
	HFONT hFont;
	HFONT hBoldFont;
	HWND hMainWindow;
	HWND hSiegeTab[MAXTABS];
	unsigned char BOT_EXECUTE;
	DWORD dwThreadId;
	HANDLE hThread;
	char szMenuName;
	HMENU hMenu;
	HWND hChatWindow;
	HWND hChatButton;
	HACCEL AccelTable;
} GLOBALSETTINGS, *PGLOBALSETTINGS, *LPGLOBALSETTINGS;
GLOBALSETTINGS GlobalSettings;

#pragma pack(push)
#pragma pack(1)
typedef struct tagUSERSETTINGS
{
	unsigned char MajorVersion;
	unsigned char MinorVersion;
	int MainX;
	int MainY;
	int MainWidth;
	int MainHeight;
	int ClientWidth;
	int ClientHeight;
	unsigned char RamMastery;
	int CatRange;
	int TrebRange;
	unsigned char OnTop;
	unsigned char EnableChat;
} USERSETTINGS, *PUSERSETTINGS, *LPUSERSETTINGS;
#pragma pack(pop)
USERSETTINGS UserSettings;


// Structure to pass to thread initialisation
typedef struct tagTIMERPROCPARAMS
{
	int		nType; // Type of thread to initialise
	DWORD	dwRange; // Range of Catapult and Trebuchet where applicable
} TIMERPROCPARAMS, *PTIMERPROCPARAMS, *LPTIMERPROCPARAMS;
TIMERPROCPARAMS tpp; // Global thread initialisation type

DWORD RandOffset(DWORD dwValue, int nUseMilliseconds); // Vary dwValue by 100-500 milliseconds randomly, or 1 to 5 seconds randomly
void SaveSettings();
void LoadSettings();

#endif //__GSIEGE_GLOBALS_H__