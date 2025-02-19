#pragma once

#include "GlobalInclude.h"

#include <fstream>

std::string GetOutputFileName();
void InitializeOutputFile();
void AppendToOutputFile(std::string content);