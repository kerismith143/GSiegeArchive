#ifndef __GSIEGE_TABWINDOW_HEADER__
#define __GSIEGE_TABWINDOW_HEADER__

#define MAXTABS		3
HWND g_hSiegeTab[MAXTABS];

#define SHOWSTART	0x00
#define SHOWSTOP	0x01

void TabWindowClassBuilder();
int PaintTabs();
int ShowTab(HWND hWnd);
void SetStartStop(int nStartStop);
int GetStartStop();

#endif //__GSIEGE_TABWINDOW_HEADER__