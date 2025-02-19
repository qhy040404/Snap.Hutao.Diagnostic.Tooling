#pragma once

#include "GlobalInclude.h"

#include <atlstr.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <fstream>
#include <sddl.h>
#include <vector>
#include <winevt.h>

#include "File.h"
#include "utils.h"

#pragma comment(lib, "wevtapi.lib")

void DumpHutaoEvents();

DWORD PrintQueryStatuses(EVT_HANDLE hResults);
DWORD GetQueryStatusProperty(EVT_QUERY_PROPERTY_ID Id, EVT_HANDLE hResults, PEVT_VARIANT& pProperty);
DWORD PrintResults(EVT_HANDLE hResults, std::vector<std::string>* resultsPtr);
DWORD PrintEvent(EVT_HANDLE hEvent, std::string* resultPtr);
std::string RenderEvent(LPWSTR event);