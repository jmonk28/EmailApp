#pragma once
#include<Windows.h>

#define ID_CLICKME 1001
#define ID_GENBOX 1002
#define ID_HEADER 1003
#define ID_CLEAR 1004
#define ID_SENDANOTHER 1005

void CreateStartPage(HWND, HWND*, HWND*, HWND*, HWND*, HWND*, HINSTANCE);

void PaintStartButtons(LPDRAWITEMSTRUCT);