// codbo2_trainer: FindProcess.cpp
// Gijs de Jong

// Includes and namespaces
#include <windows.h>
#include <tchar.h>
#include <tlhelp32.h>
#include <iostream>

using namespace std;

// Forward declarations

DWORD FindProcessId(const wstring& processName);
HWND hWndIntro;

// Callback function used to find intro window
BOOL CALLBACK EnumWindowsProcMy(HWND hWnd, LPARAM lParam)
{
	DWORD processId;
	GetWindowThreadProcessId(hWnd, &processId);
	if (processId == lParam)
	{
		hWndIntro = hWnd;
		return FALSE;
	}
	return TRUE;
}

// Boolean which returns status of process
bool status()
{
	static bool initialized;
	RECT rect{ 340, 360, 506, 480 };
	LPARAM ID = GetCurrentProcessId();
	EnumWindows(EnumWindowsProcMy, ID);
	if (FindProcessId(L"t6zm.exe")) {
		if (!initialized) {
			initialized = true;
			InvalidateRect(hWndIntro, &rect, FALSE);
			return true;
		}
	}
	else {
		if (initialized) {
			initialized = false;
			InvalidateRect(hWndIntro, &rect, FALSE);
			return false;
		}
	}
}

// Function used to find ID of process
DWORD FindProcessId(const wstring& processName)
{
	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof(processInfo);

	HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (processesSnapshot == INVALID_HANDLE_VALUE)
		return 0;

	Process32First(processesSnapshot, &processInfo);
	if (!processName.compare(processInfo.szExeFile))
	{
		CloseHandle(processesSnapshot);
		return processInfo.th32ProcessID;
	}

	while (Process32Next(processesSnapshot, &processInfo))
	{
		if (!processName.compare(processInfo.szExeFile))
		{
			CloseHandle(processesSnapshot);
			return processInfo.th32ProcessID;
		}
	}

	CloseHandle(processesSnapshot);
	return 0;
}