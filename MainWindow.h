#ifndef __GSIEGE_MAINWINDOW_HEADER__
#define __GSIEGE_MAINWINDOW_HEADER__

char g_szMainWindow[32];
char g_szMainTitle[32];
HWND g_hMainWindow;

int g_nMainX;
int g_nMainY;
int g_nMainWidth;
int g_nMainHeight;
int g_nMainClientWidth;
int g_nMainClientHeight;

HINSTANCE g_hInstance;
HINSTANCE g_hPrevInstance;
int g_nCmdShow;

HFONT hFont;
HFONT hBoldFont;

#define DEFBUT_WIDTH	70
#define DEFBUT_HEIGHT	20

HWND MainWindowClassBuilder();

#endif //__GSIEGE_MAINWINDOW_HEADER__