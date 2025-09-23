#pragma once
#include<Windows.h>

#define ID_SEND 1001
#define ID_GENBOX 1002
#define ID_HEADER 1003
#define ID_CLEAR 1004
#define ID_SENDANOTHER 1005
#define IDC_TABCONTROL 1006
#define ID_MARKREAD 1007

void CreateTabs(HWND, HWND*);

void CreateInboxPage(HWND, HWND*, HINSTANCE);

void CreateComposePage(HWND, HWND*, HINSTANCE);

void PaintStartButtons(LPDRAWITEMSTRUCT);

void ShowGroup(HWND*, int, BOOL);