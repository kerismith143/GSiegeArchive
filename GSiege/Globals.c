#include "includes.h"

// Load position and setting information from disk
void LoadSettings()
{
	FILE *fp;

	// Search for saved settings information
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

	if ( nMilliseconds ) nMultiplier = 100; // MS_ENABLED
	srand((unsigned int)time(NULL));
	dwNewValue = (((rand() % 5)+1) * nMultiplier) + dwOrigValue; // Variance by 5 units
	return dwNewValue;
}