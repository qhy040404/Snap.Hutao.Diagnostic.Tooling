#include "GetID.h"

std::string FormatReg(char* value);

void GetHutaoUniqueID()
{
	TCHAR username[UNLEN + 1];
	DWORD username_len = UNLEN + 1;
	if (!GetUserName(username, &username_len))
	{
		wprintf(L"Unable to get username");
		return;
	}

	HKEY hKey;
	LPCTSTR path = _T("SOFTWARE\\Microsoft\\Cryptography");

	if (ERROR_SUCCESS != ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, path, 0, KEY_READ, &hKey))
	{
		wprintf(L"Unable to open reg");
		return;
	}

	char dwValue[256];
	DWORD dwSzType = REG_SZ;
	DWORD dwSize = sizeof(dwValue);

	if (ERROR_SUCCESS != ::RegQueryValueEx(hKey, _T("MachineGuid"), 0, &dwSzType, (LPBYTE)&dwValue, &dwSize))
	{
		wprintf(L"Unable to read reg");
	}
	::RegCloseKey(hKey);

	std::string machineGuid = FormatReg(dwValue);

	std::string orig = WCharToString(username) + machineGuid;

	std::string ret = MD5(orig).toStr();
	std::transform(ret.begin(), ret.end(), ret.begin(), ::toupper);

	AppendToOutputFile("Your machine id: " + ret + "\n\n");
	wprintf(L"Your machine id: %hs\n\n", ret.c_str());
}

std::string FormatReg(char* value)
{
	std::string ret;
	BOOL isEnd = FALSE;
	for (int i = 0; i < 256; i++)
	{
		char currentChar = value[i];
		if (currentChar == '\0')
		{
			if (isEnd)
			{
				ret += "\0";
				break;
			}
			isEnd = TRUE;
		}
		else
		{
			ret += currentChar;
			isEnd = FALSE;
		}
	}
	return ret;
}
