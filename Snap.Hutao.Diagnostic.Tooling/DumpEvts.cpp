#include "DumpEvts.h"

constexpr auto ARRAY_SIZE = 10;

constexpr auto QUERY = 
L"<QueryList>"     
L"  <Query Path='Application'>"     
L"    <Select Path='Application'>*[System[Provider[@Name='.NET Runtime']]]</Select>"     
L"  </Query>"     
L"</QueryList>";

void DumpHutaoDotNetEvents()
{
	EVT_HANDLE hResults = NULL;

	hResults = EvtQuery(NULL, NULL, QUERY, EvtQueryChannelPath | EvtQueryTolerateQueryErrors);
	if (NULL == hResults)
	{
		goto cleanup;
	}

	if (ERROR_SUCCESS == PrintQueryStatuses(hResults))
	{
		std::vector<std::string> results;
		PrintResults(hResults, &results);

		for (auto event = results.rbegin(); event != results.rend(); ++event)
		{
			AppendToOutputFile(*event);
		}

		wprintf(L"Events has been saved to %hs\n\n", GetOutputFileName().c_str());
	}

cleanup:

	if (hResults)
		EvtClose(hResults);

}

DWORD PrintQueryStatuses(EVT_HANDLE hResults)
{
	DWORD status = ERROR_SUCCESS;
	PEVT_VARIANT pPaths = NULL;
	PEVT_VARIANT pStatuses = NULL;

	if (status = GetQueryStatusProperty(EvtQueryNames, hResults, pPaths))
		goto cleanup;

	if (status = GetQueryStatusProperty(EvtQueryStatuses, hResults, pStatuses))
		goto cleanup;

	for (DWORD i = 0; i < pPaths->Count; i++)
	{
		status += pStatuses->UInt32Arr[i];
	}

cleanup:

	if (pPaths)
		free(pPaths);

	if (pStatuses)
		free(pStatuses);

	return status;
}

DWORD GetQueryStatusProperty(EVT_QUERY_PROPERTY_ID Id, EVT_HANDLE hResults, PEVT_VARIANT& pProperty)
{
	DWORD status = ERROR_SUCCESS;
	DWORD dwBufferSize = 0;
	DWORD dwBufferUsed = 0;

	if (!EvtGetQueryInfo(hResults, Id, dwBufferSize, pProperty, &dwBufferUsed))
	{
		status = GetLastError();
		if (ERROR_INSUFFICIENT_BUFFER == status)
		{
			dwBufferSize = dwBufferUsed;
			pProperty = (PEVT_VARIANT)malloc(dwBufferSize);
			if (pProperty)
			{
				EvtGetQueryInfo(hResults, Id, dwBufferSize, pProperty, &dwBufferUsed);
			}
			else
			{
				wprintf(L"realloc failed\n");
				status = ERROR_OUTOFMEMORY;
				goto cleanup;
			}
		}

		if (ERROR_SUCCESS != (status = GetLastError()))
		{
			wprintf(L"EvtGetQueryInfo failed with %d\n", GetLastError());
			goto cleanup;
		}
	}

cleanup:

	return status;
}

DWORD PrintResults(EVT_HANDLE hResults, std::vector<std::string>* resultsPtr)
{
	DWORD status = ERROR_SUCCESS;
	EVT_HANDLE hEvents[ARRAY_SIZE];
	DWORD dwReturned = 0;

	while (true)
	{
		if (!EvtNext(hResults, ARRAY_SIZE, hEvents, INFINITE, 0, &dwReturned))
		{
			if (ERROR_NO_MORE_ITEMS != (status = GetLastError()))
			{
				wprintf(L"EvtNext failed with %lu\n", status);
			}

			goto cleanup;
		}

		for (DWORD i = 0; i < dwReturned; i++)
		{
			std::string result;
			if (ERROR_SUCCESS == (status = PrintEvent(hEvents[i], &result)))
			{
				resultsPtr->push_back(result);
				EvtClose(hEvents[i]);
				hEvents[i] = NULL;
			}
			else
			{
				goto cleanup;
			}
		}
	}

cleanup:

	for (DWORD i = 0; i < dwReturned; i++)
	{
		if (NULL != hEvents[i])
			EvtClose(hEvents[i]);
	}

	return status;
}

DWORD PrintEvent(EVT_HANDLE hEvent, std::string* resultPtr)
{
	DWORD status = ERROR_SUCCESS;
	DWORD dwBufferSize = 0;
	DWORD dwBufferUsed = 0;
	DWORD dwPropertyCount = 0;
	LPWSTR pRenderedContent = NULL;

	if (!EvtRender(NULL, hEvent, EvtRenderEventXml, dwBufferSize, pRenderedContent, &dwBufferUsed, &dwPropertyCount))
	{
		if (ERROR_INSUFFICIENT_BUFFER == (status = GetLastError()))
		{
			dwBufferSize = dwBufferUsed;
			pRenderedContent = (LPWSTR)malloc(dwBufferSize);
			if (pRenderedContent)
			{
				EvtRender(NULL, hEvent, EvtRenderEventXml, dwBufferSize, pRenderedContent, &dwBufferUsed, &dwPropertyCount);
			}
			else
			{
				wprintf(L"malloc failed\n");
				status = ERROR_OUTOFMEMORY;
				goto cleanup;
			}
		}

		if (ERROR_SUCCESS != (status = GetLastError()))
		{
			wprintf(L"EvtRender failed with %d\n", GetLastError());
			goto cleanup;
		}
	}

	*resultPtr = RenderEvent(pRenderedContent);

cleanup:

	if (pRenderedContent)
		free(pRenderedContent);

	return status;
}

std::string RenderEvent(LPWSTR event)
{
	std::string rendered = WCharToString(event);
	try
	{
		std::istringstream iss(rendered);

		boost::property_tree::ptree pt;
		boost::property_tree::read_xml(iss, pt);

		std::string time = pt.get<std::string>("Event.System.TimeCreated.<xmlattr>.SystemTime");
		std::string data = pt.get<std::string>("Event.EventData.Data");

		if (data.find("Snap.Hutao") == std::string::npos)
		{
			return "";
		}

		return "Time: " + time + "\nEventData: " + data + "\n\n";
	}
	catch (const std::exception& e)
	{
		wprintf(L"ERROR reading %hs", e.what());
		return "";
	}
}