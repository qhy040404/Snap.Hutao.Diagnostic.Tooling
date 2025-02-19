#pragma once

#include "GlobalInclude.h"

#include <vector>

std::string WCharToString(const WCHAR* wChar);
std::string WCharToString(const std::wstring& wstr);
std::string FormatReg(char* value);

bool contains(const std::vector<std::string>& vec, const std::string& str);