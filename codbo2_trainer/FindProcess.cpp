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

	bool isRunning = false;
	
	if (FindProcessId(L"notepad.exe"))
	{
		isRunning = true;
	}

	if (isRunning == false)
	{
		return false;
	}
	else {
		return true;
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