// codbo2_trainer: WriteProcess.h
// Gijs de Jong

// Declare all variables and function
extern int refresh;

extern bool disableUH;
extern bool disableUS;
extern bool disableUA;
extern bool disableNC;
extern bool disableRF;

extern DWORD oldValueUH;
extern DWORD oldValueUS;
extern DWORD oldValueUA1;
extern DWORD oldValueUA2;
extern DWORD oldValueUA3;
extern DWORD oldValueNC1;
extern DWORD oldValueNC2;
extern DWORD oldValueRF;
extern int valueHealth;
extern int valuePoints;
extern int valueAmmo1;
extern int valueAmmo2;
extern int valueRapidFire;

extern HWND hWndBO2;
extern DWORD procId;
extern HANDLE hProcHck;

extern DWORD pointerUH;
extern DWORD pointerUS;
extern DWORD pointerUA1;
extern DWORD pointerUA2;
extern DWORD pointerUA3;
extern DWORD pointerNC1;
extern DWORD pointerNC2;
extern DWORD pointerRF;

void UnlimitedHealth();
void UnlimitedPoints();
void UnlimitedAmmo();
void NoClip();
void RapidFire();