#ifndef __GSIEGE_TREBUCHET_HEADER__
#define __GSIEGE_TREBUCHET_HEADER__

#define TREBMINRNG	0
#define TREBMAXRNG	780

HWND g_hTrebStartButton;
HWND g_hTrebDurationBox;
HWND g_hTrebSliderBar;
HWND g_hTrebSliderValue;
int g_nTrebRange;

int TrebuchetFormBuilder();
int TrebuchetShowElements();
int TrebuchetHideElements();
void TrebuchetToggleElements();
void TrebuchetCheckState(HWND hWnd);

#endif //__GSIEGE_TREBUCHET_HEADER__