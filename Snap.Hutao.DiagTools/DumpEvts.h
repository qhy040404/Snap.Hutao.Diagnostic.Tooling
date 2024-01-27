#include "GlobalInclude.h"

#include <sddl.h>
#include <winevt.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <fstream>
#include <atlstr.h>

#include "File.h"
#include "utils.h"

#pragma comment(lib, "wevtapi.lib")

void DumpHutaoDotNetEvents();

DWORD PrintQueryStatuses(EVT_HANDLE hResults);
DWORD GetQueryStatusProperty(EVT_QUERY_PROPERTY_ID Id, EVT_HANDLE hResults, PEVT_VARIANT& pProperty);
DWORD PrintResults(EVT_HANDLE hResults);
DWORD PrintEvent(EVT_HANDLE hEvent);
std::string RenderEvent(LPWSTR event);