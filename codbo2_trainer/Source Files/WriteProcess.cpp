// codbo2_trainer: WriteProcess.cpp
// Gijs de Jong

// Includes
#include <windows.h>
#include "../Header Files/WriteProcess.h"

// Forward declarations
int refresh = 50;

bool disableUH = false;
bool disableUS = false;
bool disableUA = false;
bool disableNC = false;
bool disableRF = false;

DWORD oldValueUH;
DWORD oldValueUS;
DWORD oldValueUA1;
DWORD oldValueUA2;
DWORD oldValueUA3;
DWORD oldValueNC1;
DWORD oldValueNC2;
DWORD oldValueRF;
int valueHealth = 1000;
int valuePoints = 99999;
int valueAmmo1 = 999;
int valueAmmo2 = 4;
int valueRapidFire = 0;

//Following lined used for testing
//HWND hWndBO2 = FindWindow(0, L"Untitled - Notepad");
HWND hWndBO2 = FindWindow(0, L"REDACTED: Nightly [2014-06-25] - #OFFLINE MODE#");
DWORD procId;
HANDLE hProcHck;

DWORD pointerUH = 0x0217AF68;	// Pointer to health address
DWORD pointerUS = 0x02302060;	// Pointer to score address
DWORD pointerUA1 = 0x022FCE88;	// Pointer to primary weapon ammo address
DWORD pointerUA2 = 0x022FCE90;	// Pointer to secondary weapon ammo address
DWORD pointerUA3 = 0x022FCEC8;	// Pointer to grenade ammo address
DWORD pointerNC1 = 0x022FCEC4;	// Pointer to primary weapon clip address
DWORD pointerNC2 = 0x022FCECC;	// Pointer to secondary weapon clip address
DWORD pointerRF = 0x022FCAE4;	// Pointer to time to fire address

// Following functions are used to freeze value's ingame

void UnlimitedHealth() {
	while (true) {
		if (disableUH == true) {
			disableUH = false;
			break;
		}
		Sleep(refresh);
		WriteProcessMemory(hProcHck, (LPVOID)pointerUH, &valueHealth, (DWORD)sizeof(valueHealth), NULL);
	}
}

void UnlimitedPoints() {
	while (true) {
		if (disableUS == true) {
			disableUS = false;
			break;
		}
		Sleep(refresh);
		WriteProcessMemory(hProcHck, (LPVOID)pointerUS, &valuePoints, (DWORD)sizeof(valuePoints), NULL);
	}
}

void UnlimitedAmmo() {
	while (true) {
		if (disableUA == true) {
			disableUA = false;
			break;
		}
		Sleep(refresh);
		WriteProcessMemory(hProcHck, (LPVOID)pointerUA1, &valueAmmo1, (DWORD)sizeof(valueAmmo1), NULL);
		WriteProcessMemory(hProcHck, (LPVOID)pointerUA2, &valueAmmo1, (DWORD)sizeof(valueAmmo1), NULL);
		WriteProcessMemory(hProcHck, (LPVOID)pointerUA3, &valueAmmo2, (DWORD)sizeof(valueAmmo2), NULL);
	}
}

void NoClip() {
	while (true) {
		if (disableNC == true) {
			disableNC = false;
			break;
		}
		Sleep(refresh);
		WriteProcessMemory(hProcHck, (LPVOID)pointerNC1, &valueAmmo1, (DWORD)sizeof(valueAmmo1), NULL);
		WriteProcessMemory(hProcHck, (LPVOID)pointerNC2, &valueAmmo1, (DWORD)sizeof(valueAmmo1), NULL);
	}
}

void RapidFire() {
	while (true) {
		if (disableRF == true) {
			disableRF = false;
			break;
		}
		Sleep(refresh);
		WriteProcessMemory(hProcHck, (LPVOID)pointerRF, &valueRapidFire, (DWORD)sizeof(valueRapidFire), NULL);
	}
}