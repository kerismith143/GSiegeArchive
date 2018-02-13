#ifndef __GSIEGE_GLOBALS_H__
#define __GSIEGE_GLOBALS_H__

// Guild Wars 2 main window class name
#define FINDCLASS	"ArenaNet_Dx_Window_Class"
#define GSIEGE_MAJOR_VERSION	1
#define GSIEGE_MINOR_VERSION	4
#define GSIEGE_VERSION_STRING	"GSiege v1.4"

#define SLEEPMIN	50

#define RAMMASTER	3500 // Wait time + offset for Flame Ram with Mastery III or higher
#define RAMNOMASTER	5500 // Wait time + offset for Flame Ram with Mastery II or lower
#define CATTREB		5500 // Wait time + offset for Catapult and Trebuchet

// Global Flame Ram handles

// Trebuchet global window handles

// Global Catapult tab handles

int g_nSBy;

#define MAXTABS			3 // Maximum number of tabs to display
#define GSIEGE_DAT		"GSiege.dat" // Name of settings file
#define MAXTEXT			100 // Maximum number of characters for chat with NULL terminator
#define MAXRNG			4
#define RANDOFFSET		5
#define MAINWIDTH		300
#define MAINHEIGHT		210

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
	HWND hRandomEdit;

	HWND hRamStartButton;
	HWND hRamDurationBox;
	HWND hRamRecharge;
	HWND hRamMasteryCheckbox;

	HWND hTrebStartButton;
	HWND hTrebDurationBox;
	HWND hTrebSliderBar;
	HWND hTrebSliderValue;
	HWND hTrebMinEdit;
	HWND hTrebMaxEdit;
	HWND hTrebRecharge;

	HWND hCatStartButton;
	HWND hCatDurationBox;
	HWND hCatSliderBar;
	HWND hCatSliderValue;
	HWND hCatMinEdit;
	HWND hCatMaxEdit;
	HWND hCatRecharge;

	HWND hOffset;
	HWND hOffsetStatic;

	HWND SelectedTab;
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
	int RamMastRecharge;
	int RamNoMastRecharge;
	int CatRange;
	int CatMin;
	int CatMax;
	int CatRecharge;
	int TrebRange;
	int TrebMin;
	int TrebMax;
	int TrebRecharge;
	int Offset;
	unsigned char OnTop;
	unsigned char EnableChat;
	unsigned char EnableAdv;
} USERSETTINGS, *PUSERSETTINGS, *LPUSERSETTINGS;
#pragma pack(pop)
USERSETTINGS UserSettings;


// Structure to pass to thread initialisation
typedef struct tagTIMERPROCPARAMS
{
	int		nType; // Type of thread to initialise
} TIMERPROCPARAMS, *PTIMERPROCPARAMS, *LPTIMERPROCPARAMS;
TIMERPROCPARAMS tpp; // Global thread initialisation type

DWORD RandOffset(DWORD dwValue, int nUseMilliseconds); // Vary dwValue by 100-500 milliseconds randomly, or 1 to 5 seconds randomly
void SaveSettings();
void LoadSettings();
void ModifyEdits(HWND hWnd);

#endif //__GSIEGE_GLOBALS_H__