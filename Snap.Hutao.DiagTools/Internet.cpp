#include "Internet.h"

HINTERNET hSession = NULL,
hConnect = NULL,
hRequest = NULL;
BOOL bResult = FALSE;

static void __cleanup()
{
	if (hRequest)
		WinHttpCloseHandle(hRequest);

	if (hConnect)
		WinHttpCloseHandle(hConnect);

	if (hSession)
		WinHttpCloseHandle(hSession);
}

void PrintPublicIp()
{
	hSession = WinHttpOpen(L"Snap.Hutao.DiagTools/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);

	if (hSession)
		hConnect = WinHttpConnect(hSession, L"api.snapgenshin.com", INTERNET_DEFAULT_HTTPS_PORT, 0);

	if (hConnect)
		hRequest = WinHttpOpenRequest(hConnect, L"GET", L"/ip", NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);

	if (hRequest)
		bResult = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);

	if (bResult)
		bResult = WinHttpReceiveResponse(hRequest, NULL);

	DWORD dwSize = 0;
	DWORD bytesRead = 0;
	LPSTR pszOutBuffer;

	if (bResult)
	{
		do
		{
			dwSize = 0;
			if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
				printf("Error %u in WinHttpQueryDataAvailable.\n", GetLastError());

			pszOutBuffer = new char[dwSize + 1];
			if (!pszOutBuffer)
			{
				printf("Out of memory\n");
				dwSize = 0;
			}
			else
			{
				ZeroMemory(pszOutBuffer, dwSize + 1);

				if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwSize, &bytesRead))
					printf("Error %u in WinHttpReadData.\n", GetLastError());
				else
				{
					try
					{
						std::istringstream iss(pszOutBuffer);

						boost::property_tree::ptree pt;
						boost::property_tree::read_json(iss, pt);

						std::string ip = pt.get<std::string>("data.ip");

						std::string result = "Public IP: " + ip + "\n";

						AppendToOutputFile(result);
						printf(result.c_str());
					}
					catch (const std::exception&)
					{
					}
				}

				delete[] pszOutBuffer;
			}
		} while (dwSize > 0);
	}

	__cleanup();
}

void CheckHutaoEndpoints()
{

}