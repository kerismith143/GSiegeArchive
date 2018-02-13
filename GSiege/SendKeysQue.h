#ifndef __SENDKEYSQUE_H__
#define __SENDKEYSQUE_H__

#define SKCHARMAX		99

#define SK_SUCCESS		 1
#define SKE_UNKNOWN		 0
#define SKE_MUSTINIT	-1
#define SKE_VALLIMIT	-2
#define SKE_NOTFOUND	-3

struct send_que
{
	HWND hWnd;
	char Text[SKCHARMAX+1];
	struct send_que *NextEntry;
};

struct send_que *lpSendQueHead;
struct send_que *lpSendQueCurrent;

struct send_que *SKQueAddElement(HWND hWnd, char *text);
int SKQueRemoveElement(HWND hWnd, char *text);
int SKQueRemoveFirstElement();
int SKQueRemoveLastElement();

int SendKeysQueAdd(HWND hWnd, char *text);
DWORD SendKeysQue();
void SKQueInit();

#endif //__SENDKEYSQUE_H__
