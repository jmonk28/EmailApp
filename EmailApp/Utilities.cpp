#include "framework.h"
#include "EmailApp.h"
#include "Utilities.h"
#include <commctrl.h>

void CreateTabs(HWND hWnd, HWND* tabs) {
    *tabs = CreateWindowEx(
        0, WC_TABCONTROL, L"",
        WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE,
        10, 10, 400, 200,
        hWnd, (HMENU)IDC_TABCONTROL,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL
    );

    TCITEM tie{};
    tie.mask = TCIF_TEXT;

    tie.pszText = (LPWSTR)L"Inbox";
    TabCtrl_InsertItem(*tabs, 0, &tie);

    tie.pszText = (LPWSTR)L"Compose";
    TabCtrl_InsertItem(*tabs, 1, &tie);
}

void CreateInboxPage(HWND hWnd, HWND* inboxControls, HINSTANCE hInst) {

   // Create inbox header
   inboxControls[0] = CreateWindowEx(
        0, L"STATIC", L"Inbox",
        WS_VISIBLE | WS_CHILD,
        15, 40, 140, 30,
        hWnd,
        (HMENU)ID_HEADER,
        hInst,
        nullptr
    );

   // Create "Mark as Read" button
   inboxControls[1] = CreateWindowEx(
       0, L"BUTTON", L"Mark as Read",
       WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
       15, 70, 100, 30,     // x, y, width, height
       hWnd,                
       (HMENU)ID_MARKREAD,   
       hInst,               
       nullptr);

}

void CreateComposePage(HWND hWnd, HWND* composeControls, HINSTANCE hInst) {

    // Create recipient box
    composeControls[0] = CreateWindowEx(
        WS_EX_CLIENTEDGE, L"EDIT", L"",
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
        80, 60, 200, 20,
        hWnd,
        (HMENU)ID_GENBOX,
        hInst,
        nullptr);

    // Create text box
    composeControls[1] = CreateWindowEx(
        WS_EX_CLIENTEDGE, L"EDIT", L"",
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
        80, 90, 200, 60,
        hWnd,
        (HMENU)ID_GENBOX,
        hInst,
        nullptr);

    // Create "Send Email" button
    composeControls[2] = CreateWindowEx(
        0, L"BUTTON", L"Clear",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
        10, 160, 100, 30,     // x, y, width, height
        hWnd,                // parent is your main window
        (HMENU)ID_CLEAR,   // control ID
        hInst,               // global instance handle
        nullptr);

    // Create "Send Email" button
    composeControls[3] = CreateWindowEx(
        0, L"BUTTON", L"Send Email",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
        115, 160, 100, 30,     // x, y, width, height
        hWnd,                // parent is your main window
        (HMENU)ID_SEND,   // control ID
        hInst,               // global instance handle
        nullptr);

    // Create "Send Another Email" button
    composeControls[4] = CreateWindowEx(
        0, L"BUTTON", L"Send Another Email",
        WS_TABSTOP | WS_CHILD | BS_OWNERDRAW,
        10, 160, 100, 30,     // x, y, width, height
        hWnd,                // parent is your main window
        (HMENU)ID_SENDANOTHER,   // control ID
        hInst,               // global instance handle
        nullptr);
}

void PaintStartButtons(LPDRAWITEMSTRUCT lpdis) {
    //Styling "Send Email" button
    if (lpdis->CtlID == ID_SEND) // button ID
    {
        // Choose colors
        COLORREF bgColor = (lpdis->itemState & ODS_SELECTED) ? RGB(150, 200, 255) : RGB(200, 200, 255);
        COLORREF textColor = RGB(0, 0, 128);

        // Fill background
        HBRUSH hBrush = CreateSolidBrush(bgColor);
        FillRect(lpdis->hDC, &lpdis->rcItem, hBrush);
        DeleteObject(hBrush);

        // Draw text centered
        SetBkMode(lpdis->hDC, TRANSPARENT);
        SetTextColor(lpdis->hDC, textColor);

        DrawText(lpdis->hDC, L"Send Email", -1, (LPRECT)&lpdis->rcItem,
            DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }
    //Styling "Clear" button
    if (lpdis->CtlID == ID_CLEAR)
    {
        // Choose colors
        COLORREF bgColor = (lpdis->itemState & ODS_SELECTED) ? RGB(150, 200, 255) : RGB(200, 200, 255);
        COLORREF textColor = RGB(0, 0, 128);

        // Fill background
        HBRUSH hBrush = CreateSolidBrush(bgColor);
        FillRect(lpdis->hDC, &lpdis->rcItem, hBrush);
        DeleteObject(hBrush);

        // Draw text centered
        SetBkMode(lpdis->hDC, TRANSPARENT);
        SetTextColor(lpdis->hDC, textColor);

        DrawText(lpdis->hDC, L"Clear", -1, (LPRECT)&lpdis->rcItem,
            DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }
    //Styling "Send Another Email" button
    if (lpdis->CtlID == ID_SENDANOTHER)
    {
        // Choose colors
        COLORREF bgColor = (lpdis->itemState & ODS_SELECTED) ? RGB(150, 200, 255) : RGB(200, 200, 255);
        COLORREF textColor = RGB(0, 0, 128);

        // Fill background
        HBRUSH hBrush = CreateSolidBrush(bgColor);
        FillRect(lpdis->hDC, &lpdis->rcItem, hBrush);
        DeleteObject(hBrush);

        // Draw text centered
        SetBkMode(lpdis->hDC, TRANSPARENT);
        SetTextColor(lpdis->hDC, textColor);

        DrawText(lpdis->hDC, L"Send Another", -1, (LPRECT)&lpdis->rcItem,
            DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }
    //Styling "Mark as Read" button
    if (lpdis->CtlID == ID_MARKREAD) // button ID
    {
        // Choose colors
        COLORREF bgColor = (lpdis->itemState & ODS_SELECTED) ? RGB(150, 200, 255) : RGB(200, 200, 255);
        COLORREF textColor = RGB(0, 0, 128);

        // Fill background
        HBRUSH hBrush = CreateSolidBrush(bgColor);
        FillRect(lpdis->hDC, &lpdis->rcItem, hBrush);
        DeleteObject(hBrush);

        // Draw text centered
        SetBkMode(lpdis->hDC, TRANSPARENT);
        SetTextColor(lpdis->hDC, textColor);

        DrawText(lpdis->hDC, L"Mark as Read", -1, (LPRECT)&lpdis->rcItem,
            DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }
}

// Helper to show/hide a group
void ShowGroup(HWND* controls, int count, BOOL show) {
    for (int i = 0; i < count; i++) {
        ShowWindow(controls[i], show ? SW_SHOW : SW_HIDE);
    }
}