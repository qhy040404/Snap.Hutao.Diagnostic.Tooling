#include "SystemInfo.h"

void CheckWindowsVersion()
{
	const LPCTSTR MAJOR_KEY = _T("CurrentMajorVersionNumber");
	const LPCTSTR MINOR_KEY = _T("CurrentMinorVersionNumber");
	const LPCTSTR BUILD_KEY = _T("CurrentBuildNumber");
	const LPCTSTR REVISION_KEY = _T("UBR");

	HKEY hKey;
	LPCTSTR path = _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion");

	if (ERROR_SUCCESS != ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, path, 0, KEY_READ, &hKey))
	{
		wprintf(L"Unable to open reg");
		return;
	}

	std::ostringstream buffer;

	buffer << "Windows version: ";

	int dwValue;
	char dwValue2[256];
	DWORD dwSzType = REG_DWORD;
	DWORD dwSzType2 = REG_SZ;
	DWORD dwSize = sizeof(dwValue);
	DWORD dwSize2 = sizeof(dwValue2);

	if (ERROR_SUCCESS != ::RegQueryValueEx(hKey, MAJOR_KEY, 0, &dwSzType, (LPBYTE)&dwValue, &dwSize))
	{
		wprintf(L"Unable to read major version");
	}

	buffer << dwValue << ".";

	if (ERROR_SUCCESS != ::RegQueryValueEx(hKey, MINOR_KEY, 0, &dwSzType, (LPBYTE)&dwValue, &dwSize))
	{
		wprintf(L"Unable to read minor version");
	}

	buffer << dwValue << ".";

	if (ERROR_SUCCESS != ::RegQueryValueEx(hKey, BUILD_KEY, 0, &dwSzType2, (LPBYTE)&dwValue2, &dwSize2))
	{
		wprintf(L"Unable to read build number");
	}

	buffer << FormatReg(dwValue2) << ".";

	if (ERROR_SUCCESS != ::RegQueryValueEx(hKey, REVISION_KEY, 0, &dwSzType, (LPBYTE)&dwValue, &dwSize))
	{
		wprintf(L"Unable to read revision number");
	}

	buffer << dwValue << std::endl;

	std::string result = buffer.str();
	AppendToOutputFile(result);
	printf(result.c_str());
}