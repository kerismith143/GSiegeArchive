#include "includes.h"

void ModifyEdits(HWND hWnd)
{
	DWORD dwOffsetLength;
	char *buffer;
	char buffer2[32];

	dwOffsetLength = SendMessage(hWnd, WM_GETTEXTLENGTH, 0, 0);
	buffer = (char *)malloc(dwOffsetLength + 1);
	SendMessage(hWnd, WM_GETTEXT, (WPARAM)(dwOffsetLength + 1), (LPARAM)buffer);

	if ( hWnd == GlobalSettings.hOffset )
		UserSettings.Offset = atoi(buffer);
	else if ( hWnd == GlobalSettings.hCatMinEdit )
		UserSettings.CatMin = atoi(buffer);
	else if ( hWnd == GlobalSettings.hCatMaxEdit )
		UserSettings.CatMax = atoi(buffer);
	else if ( hWnd == GlobalSettings.hTrebMinEdit )
		UserSettings.TrebMin = atoi(buffer);
	else if ( hWnd == GlobalSettings.hTrebMaxEdit )
		UserSettings.TrebMax = atoi(buffer);
	else if ( hWnd == GlobalSettings.hCatRecharge )
		UserSettings.CatRecharge = atoi(buffer);
	else if ( hWnd == GlobalSettings.hTrebRecharge )
		UserSettings.TrebRecharge = atoi(buffer);
	else if ( hWnd == GlobalSettings.hRamRecharge )
	{
		if ( UserSettings.RamMastery )
			UserSettings.RamMastRecharge = atoi(buffer);
		else
			UserSettings.RamNoMastRecharge = atoi(buffer);
	}

	SetScrollRange(GlobalSettings.hCatSliderBar, SB_CTL, UserSettings.CatMin, UserSettings.CatMax, FALSE);
	if ( UserSettings.CatRange < UserSettings.CatMin )
		UserSettings.CatRange = UserSettings.CatMin;
	else if ( UserSettings.CatRange > UserSettings.CatMax )
		UserSettings.CatRange = UserSettings.CatMax;
	SetScrollPos(GlobalSettings.hCatSliderBar, SB_CTL, UserSettings.CatRange, TRUE);
	memset(buffer2, 0, sizeof(buffer2));
	_snprintf(buffer2, sizeof(buffer2)-1, "Range: %.2f", (((float)(UserSettings.CatRange - UserSettings.CatMin) / (float)(UserSettings.CatMax - UserSettings.CatMin)) * 100) );
	SendMessage(GlobalSettings.hCatSliderValue, WM_SETTEXT, 0, (LPARAM)buffer2);

	SetScrollRange(GlobalSettings.hTrebSliderBar, SB_CTL, UserSettings.TrebMin, UserSettings.TrebMax, FALSE);
	if ( UserSettings.TrebRange < UserSettings.TrebMin )
		UserSettings.TrebRange = UserSettings.TrebMin;
	else if ( UserSettings.TrebRange > UserSettings.TrebMax )
		UserSettings.TrebRange = UserSettings.TrebMax;
	SetScrollPos(GlobalSettings.hTrebSliderBar, SB_CTL, UserSettings.TrebRange, TRUE);
	memset(buffer2, 0, sizeof(buffer2));
	_snprintf(buffer2, sizeof(buffer2)-1, "Range: %.2f", (((float)(UserSettings.TrebRange - UserSettings.TrebMin) / (float)(UserSettings.TrebMax - UserSettings.TrebMin)) * 100) );
	SendMessage(GlobalSettings.hTrebSliderValue, WM_SETTEXT, 0, (LPARAM)buffer2);

	free(buffer);
}

// Load position and setting information from disk
void LoadSettings()
{
	FILE *fp;

	// Search for saved settings information
	memset(&UserSettings, 0, sizeof(UserSettings));
	UserSettings.CatMax = CATMAXRNG;
	UserSettings.TrebMax = TREBMAXRNG;
	UserSettings.Offset = RANDOFFSET;
	UserSettings.RamNoMastRecharge = RAMNOMASTER;
	UserSettings.RamMastRecharge = RAMMASTER;
	UserSettings.CatRecharge = CATTREB;
	UserSettings.TrebRecharge = CATTREB;
	UserSettings.MajorVersion = GSIEGE_MAJOR_VERSION;
	UserSettings.MinorVersion = GSIEGE_MINOR_VERSION;

	if ( (fp = fopen(GSIEGE_DAT, "rb")) == NULL )
		return;
	fread(&UserSettings, sizeof(UserSettings), 1, fp);
	fclose(fp);
	if ( (UserSettings.MajorVersion != GSIEGE_MAJOR_VERSION) &&
		 (UserSettings.MinorVersion != GSIEGE_MINOR_VERSION) )
	{
		memset(&UserSettings, 0, sizeof(UserSettings));
		UserSettings.MajorVersion = GSIEGE_MAJOR_VERSION;
		UserSettings.MinorVersion = GSIEGE_MINOR_VERSION;
		remove(GSIEGE_DAT);
	}
}

// Save position and setting information to disk
void SaveSettings()
{
	FILE *fp;

	// Save settings information to disk
	if ( (fp = fopen(GSIEGE_DAT, "wb")) != NULL )
	{
		fwrite(&UserSettings, sizeof(UserSettings), 1, fp);
		fclose(fp);
	}
}

// Generate a randomly incremented number based on the original value
DWORD RandOffset(DWORD dwOrigValue, int nMilliseconds)
{
	DWORD dwNewValue;
	int nMultiplier = 1; // MS_DISABLED

	if ( nMilliseconds ) nMultiplier = 100;
	srand((unsigned int)time(NULL));
	dwNewValue = dwOrigValue;
	if ( UserSettings.Offset )
	{
		dwNewValue += (((rand() % UserSettings.Offset)) * nMultiplier); // Variance by 5 units
	}
	return dwNewValue;
}