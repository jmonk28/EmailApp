#include "framework.h"
#include "EmailApp.h"
#include "Utilities.h"

void CreateStartPage(HWND hWnd, HWND* recipient, HWND* textbox, HWND* clearButton, HWND* emailButton, HWND* sendAnother, HINSTANCE hInst) {

    // Create header text
    CreateWindowEx(
        0, L"STATIC", L"Email Application",
        WS_VISIBLE | WS_CHILD,
        10, 10, 140, 30,
        hWnd,
        (HMENU)ID_HEADER,
        hInst,
        nullptr
    );

    // Create recipient box
    *recipient = CreateWindowEx(
        WS_EX_CLIENTEDGE, L"EDIT", L"",
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
        80, 50, 200, 20,
        hWnd,
        (HMENU)ID_GENBOX,
        hInst,
        nullptr);

    // Create text box
    *textbox = CreateWindowEx(
        WS_EX_CLIENTEDGE, L"EDIT", L"",
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
        80, 80, 200, 60,
        hWnd,
        (HMENU)ID_GENBOX,
        hInst,
        nullptr);

    // Create "Send Email" button
    *clearButton = CreateWindowEx(
        0, L"BUTTON", L"Clear",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
        10, 150, 100, 30,     // x, y, width, height
        hWnd,                // parent is your main window
        (HMENU)ID_CLEAR,   // control ID
        hInst,               // global instance handle
        nullptr);

    // Create "Send Email" button
    *emailButton = CreateWindowEx(
        0, L"BUTTON", L"Send Email",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
        115, 150, 100, 30,     // x, y, width, height
        hWnd,                // parent is your main window
        (HMENU)ID_CLICKME,   // control ID
        hInst,               // global instance handle
        nullptr);

    // Create "Send Another Email" button
    *sendAnother = CreateWindowEx(
        0, L"BUTTON", L"Send Another Email",
        WS_TABSTOP | WS_CHILD | BS_OWNERDRAW,
        10, 150, 100, 30,     // x, y, width, height
        hWnd,                // parent is your main window
        (HMENU)ID_SENDANOTHER,   // control ID
        hInst,               // global instance handle
        nullptr);
}

void PaintStartButtons(LPDRAWITEMSTRUCT lpdis) {
    //Styling "Send Email" button
    if (lpdis->CtlID == ID_CLICKME) // button ID
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
}