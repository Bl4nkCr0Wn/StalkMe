#pragma once
#include <Windows.h>

class ScreenshotInterface
{
public:
	ScreenshotInterface();
	~ScreenshotInterface();
	HBITMAP TakeShot();

private:
	void CopyScreenToClipboard();

};