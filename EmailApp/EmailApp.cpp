// EmailApp.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "EmailApp.h"
#include "Utilities.h"
#include "EmailFunctionality.h"
#include <string>
#include <commctrl.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HWND composeControls[5];
HWND inboxControls[2];
HWND composeTab;
HWND tabs;
bool showTextBox = true;
bool showSendAnother = false;
std::string accessToken;
std::string refreshToken;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Poop(HWND, UINT, WPARAM, LPARAM);
void                CreateStartPage(HWND);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_EMAILAPP, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_EMAILAPP));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_EMAILAPP));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = CreateSolidBrush(RGB(180, 220, 255));
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_EMAILAPP);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {

    case WM_CREATE:
        {
            CreateTabs(hWnd, &tabs);
            CreateInboxPage(hWnd, inboxControls, hInst);
            CreateComposePage(hWnd, composeControls, hInst);
            //Start with compose page showing, inbox page hidden
            ShowGroup(inboxControls, 2, FALSE);
        }

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case IDM_POOP:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_POOPBOX), hWnd, Poop);
                break;
            case ID_SEND:
                showTextBox = false;
                ShowWindow(composeControls[0], SW_HIDE);
                ShowWindow(composeControls[1], SW_HIDE);
                ShowWindow(composeControls[2], SW_HIDE);
                ShowWindow(composeControls[3], SW_HIDE);
                ShowWindow(composeControls[4], SW_NORMAL);
                InvalidateRect(hWnd, NULL, TRUE);
                break;
            case ID_CLEAR:
                SetWindowText(composeControls[0], L"");
                SetWindowText(composeControls[1], L"");
                break;
            case ID_SENDANOTHER:
                showTextBox = true;
                ShowWindow(composeControls[0], SW_NORMAL);
                ShowWindow(composeControls[1], SW_NORMAL);
                ShowWindow(composeControls[2], SW_NORMAL);
                ShowWindow(composeControls[3], SW_NORMAL);
                ShowWindow(composeControls[4], SW_HIDE);
                SetWindowText(composeControls[0], L"");
                SetWindowText(composeControls[1], L"");
                InvalidateRect(hWnd, NULL, TRUE);
                break;
            case ID_MARKREAD:
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            if (!showTextBox) {
                TextOut(hdc, 10, 50, L"Email sent!", 12);
            }
            else {
                TextOut(hdc, 10, 50, L"To:", 4);
                TextOut(hdc, 10, 80, L"Message:", 9);
            }
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DRAWITEM:
        {
            LPDRAWITEMSTRUCT lpdis = (LPDRAWITEMSTRUCT)lParam;
            PaintStartButtons(lpdis);
            return TRUE;
        }
    break;
    case WM_NOTIFY:
        {
            LPNMHDR pnmh = (LPNMHDR)lParam;
            if (pnmh->idFrom == IDC_TABCONTROL && pnmh->code == TCN_SELCHANGE) {
                int iPage = TabCtrl_GetCurSel(pnmh->hwndFrom);

                // Toggle groups based on selected tab
                ShowGroup(inboxControls, 2, iPage == 0);
                ShowGroup(composeControls, 4, iPage == 1);
            }
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

// Message handler for poop box.
INT_PTR CALLBACK Poop(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDPOOPAGAIN) {
            DialogBox(hInst, MAKEINTRESOURCE(IDD_POOPBOX), hDlg, Poop);
        }
        break;
    }
    return (INT_PTR)FALSE;
}

