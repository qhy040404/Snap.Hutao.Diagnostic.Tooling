#define _CRT_SECURE_NO_WARNINGS

#include "utils.h"

std::string WCharToString(const WCHAR* wChar) {
	return WCharToString(std::wstring(wChar));
}

std::string WCharToString(const std::wstring& wstr)
{
	std::string result;
	int len = WideCharToMultiByte(65001, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
	char* buffer = new char[len + 1];
	WideCharToMultiByte(65001, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);
	buffer[len] = '\0';
	result.append(buffer);
	delete[] buffer;
	return result;
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