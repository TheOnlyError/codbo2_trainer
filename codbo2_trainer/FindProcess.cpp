// codbo2_trainer: FindProcess.cpp
// Gijs de Jong

// Includes
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>

using namespace std;

DWORD FindProcessId(const wstring& processName);

bool status()
{
	RECT rect{ 340, 432, 506, 352 };

	if (FindProcessId(L"notepad.exe"))
	{
		return true;
	}
	else {
		return false;
	}

	return false;
}

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