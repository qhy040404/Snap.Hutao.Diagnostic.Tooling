#include "DumpEvts.h"
#include "GetID.h"

int main()
{
	wprintf(L"Snap Hutao Diagnosis Tools\n");
	wprintf(L"Copyright (c) DGP Studio. All rights reserved.\n");
	wprintf(L"\n");

	wprintf(L"Calculating Unique ID...\n\n");
	GetHutaoUniqueID();

	wprintf(L"Dumping .NET Runtime Events...\n\n");
	DumpHutaoDotNetEvents();

	system("pause");
}