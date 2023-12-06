#define _CRT_SECURE_NO_WARNINGS

#include "utils.h"

std::string WCharToString(const WCHAR* wChar) {
	std::wstring orig = std::wstring(wChar);
	std::string result;
	int len = WideCharToMultiByte(65001, 0, orig.c_str(), orig.size(), NULL, 0, NULL, NULL);
	char* buffer = new char[len + 1];
	WideCharToMultiByte(65001, 0, orig.c_str(), orig.size(), buffer, len, NULL, NULL);
	buffer[len] = '\0';
	result.append(buffer);
	delete[] buffer;
	return result;
}