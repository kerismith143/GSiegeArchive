#ifndef __SENDKEYS_H__
#define __SENDKEYS_H__

// VK_RETURN 0x0D

DWORD SendKeys(HWND hWnd, char ch, DWORD pause);
DWORD SendKeysEx(HWND hWnd, char *text, size_t len, DWORD pause);

#endif //__SENDKEYS_H__