#include <iostream>
#include <Windows.h>

constexpr auto SCHEDULE_TASK_NAME = L"SnapHutaoDailyNoteRefreshTask";

bool RunAsElevated(LPCWSTR exe, LPCWSTR param, int nShow = SW_SHOW)
{
    SHELLEXECUTEINFO sei;
    sei.cbSize = sizeof(sei);
    sei.fMask = SEE_MASK_NOCLOSEPROCESS;
    sei.hwnd = NULL;
    sei.lpVerb = L"runas"; // Request elevation
    sei.lpFile = exe; // Path to the executable
    sei.lpParameters = param; // Command line parameters
    sei.lpDirectory = NULL; // Current directory
    sei.nShow = nShow; // Show window
    sei.hInstApp = NULL; // Handle to the application instance
    bool ret = ShellExecuteEx(&sei);
    CloseHandle(sei.hProcess);
    return ret;
}

bool DeleteScheduledTask()
{
    auto exe = L"schtasks.exe";
    auto param = L"/delete /tn \"" + std::wstring(SCHEDULE_TASK_NAME) + L"\" /f";
    auto ret = RunAsElevated(exe, param.c_str());
    return ret;
}

int wmain(int argc, wchar_t* argv[])
{
    std::wcout << L"Snap Hutao Diagnostic Tooling - Scheduled Task Remover" << std::endl;
    std::wcout << L"Copyright (c) 2025 DGP Studio. All rights reserved." << std::endl;
    std::wcout << std::endl;

    std::wcout << L"Deleting scheduled task \"" << SCHEDULE_TASK_NAME << L"\"..." << std::endl;
    DeleteScheduledTask();
    std::wcout << L"Scheduled task \"" << SCHEDULE_TASK_NAME << L"\" deleted." << std::endl;
    std::wcout << L"Press any key to exit..." << std::endl;
    _wsystem(L"pause > nul");

    return 0;
}