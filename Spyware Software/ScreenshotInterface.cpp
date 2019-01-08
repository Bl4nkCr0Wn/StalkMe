#include "ScreenshotInterface.h"

ScreenshotInterface::ScreenshotInterface()
{
}

ScreenshotInterface::~ScreenshotInterface()
{
}

/* Copies current screen and returns it
Input: None
Output: HBITMAP
*/
HBITMAP ScreenshotInterface::TakeShot()
{
	RECT rc;
	// Get a handle to the desktop window
	HWND hWnd2 = GetDesktopWindow();
	// Get the size of screen to the variable rc
	GetWindowRect(hWnd2, &rc);
	HDC hScreenDC = GetDC(NULL);
	HDC hMemoryDC = CreateCompatibleDC(hScreenDC);
	// copy screen to bitmap
	HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, rc.right, rc.bottom);

	SelectObject(hMemoryDC, hBitmap);

	BOOL    bRet = BitBlt(hMemoryDC, 0, 0, rc.right, rc.bottom, hScreenDC, 0, 0, SRCCOPY);

	// release memory
	DeleteDC(hMemoryDC);
	DeleteDC(hScreenDC);
	////////////////////DeleteObject(hBitmap);

	//this->CopyScreenToClipboard();
	return hBitmap;
}

void ScreenshotInterface::CopyScreenToClipboard()
{
	RECT rc;
	// Get a handle to the desktop window
	HWND hWnd2 = GetDesktopWindow();
	// Get the size of screen to the variable rc
	GetWindowRect(hWnd2, &rc);
	HDC hScreenDC = GetDC(NULL);
	HDC hMemoryDC = CreateCompatibleDC(hScreenDC);
	// copy screen to bitmap
	HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, rc.right, rc.bottom);

	SelectObject(hMemoryDC, hBitmap);

	BOOL    bRet = BitBlt(hMemoryDC, 0, 0, rc.right, rc.bottom, hScreenDC, 0, 0, SRCCOPY);

	// save bitmap to clipboard
	OpenClipboard(NULL);
	EmptyClipboard();
	SetClipboardData(CF_BITMAP, hBitmap);
	CloseClipboard();
	
	// release memory
	DeleteDC(hMemoryDC);
	DeleteDC(hScreenDC);
	DeleteObject(hBitmap);
}
