#pragma once
#include<Windows.h>

#define ID_SEND 1001
#define ID_GENBOX 1002
#define ID_HEADER 1003
#define ID_CLEAR 1004
#define ID_SENDANOTHER 1005
#define IDC_TABCONTROL 1006

void CreateComposePage(HWND, HWND*, HWND*, HWND*, HWND*, HWND*, HINSTANCE);

void PaintStartButtons(LPDRAWITEMSTRUCT);