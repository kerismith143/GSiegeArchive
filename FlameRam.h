#ifndef __GSIEGE_FLAMERAM_HEADER__
#define __GSIEGE_FLAMERAM_HEADER__

unsigned char g_nRamMastery;
HWND g_hRamStartButton;
HWND g_hRamDurationBox;

int FlameRamFormBuilder();
int FlameRamHideElements();
int FlameRamShowElements();
void FlameRamToggleElements();
void FlameRamCheckState(HWND hWnd);

#endif //__GSIEGE_FLAMERAM_HEADER__