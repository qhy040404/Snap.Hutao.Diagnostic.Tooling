#include "DumpEvts.h"
#include "GetID.h"
#include "SystemInfo.h"

int main()
{
	InitializeOutputFile();

	wprintf(L"Snap Hutao Diagnosis Tools\n");
	wprintf(L"Copyright (c) 2024 DGP Studio. All rights reserved.\n");
	printf("\n");

	CheckWindowsVersion();

	printf("\n----------\n\n");

	wprintf(L"Calculating Unique ID...\n\n");
	GetHutaoUniqueID();

	wprintf(L"Dumping .NET Runtime Events...\n\n");
	DumpHutaoDotNetEvents();

	system("pause");
}