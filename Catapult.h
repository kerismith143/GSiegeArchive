#ifndef __GSIEGE_CATAPULT_HEADER__
#define __GSIEGE_CATAPULT_HEADER__

#define CATMINRNG	0
#define CATMAXRNG	380

HWND g_hCatStartButton;
HWND g_hCatDurationBox;
HWND g_hCatSliderBar;
HWND g_hCatSliderValue;
int g_nCatRange;

int CatapultFormBuilder();
int CatapultShowElements();
int CatapultHideElements();
void CatapultToggleElements();
void CatapultCheckState(HWND hWnd);

#endif //__GSIEGE_CATAPULT_HEADER__