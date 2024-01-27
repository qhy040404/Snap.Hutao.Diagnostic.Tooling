#include "GlobalInclude.h"

#include <winhttp.h>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include "utils.h"
#include "File.h"

#pragma comment(lib, "winhttp.lib")

void PrintPublicIp();
void CheckHutaoEndpoints();