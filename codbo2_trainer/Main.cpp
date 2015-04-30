// codbo2_trainer: Main.cpp
// Gijs de Jong

// Includes
#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>
#include <string>
#include <tchar.h>
#include <objidl.h>
#include <gdiplus.h>
#include "FindProcess.h"
#include "Res.h"

using namespace Gdiplus;
using namespace std;
#pragma comment (lib,"Gdiplus.lib")

/*---------------------------------------------------------------------------------------------*/

// Forward declarations
static TCHAR szWindowClassIntro[] = _T("codbo2_trainer_intro");
static TCHAR szWindowClassMain[] = _T("codbo2_trainer_main");
static TCHAR szTitle[] = _T("Call of Duty Black Ops II Trainer");

HINSTANCE hInst = NULL;

LRESULT CALLBACK WndProcIntro(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProcMain(HWND, UINT, WPARAM, LPARAM);

const int INTRO_WIDTH = 850;
const int INTRO_HEIGHT = 534;

const int MAIN_WIDTH = 850;
const int MAIN_HEIGHT = 700;

bool moveWindow = false;
RECT rcWindow;
POINT pos;
POINT curPos;

bool hoverEnter = false;

bool hoverMn = false;
bool hoverCl = false;

// #000
int BtnBgInactive = 0;
// #222
int BtnBgActive = 34;

bool clickUH = false;
bool clickUS = false;
bool clickUA = false;
bool clickNC = false;
bool clickRF = false;
bool clickDA = false;

bool hoverUH = false;
bool hoverUS = false;
bool hoverUA = false;
bool hoverNC = false;
bool hoverRF = false;
bool hoverDA = false;

int valuePoints = 99999;
int valueAmmo1 = 999;
int valueAmmo2 = 4;

HWND hWndBO2 = FindWindow(0, L"Untitled - Notepad");
//HWND hWndBO2 = FindWindow(0, L"REDACTED: Nightly [2014-06-25] - #OFFLINE MODE#");
DWORD procId;
HANDLE hProcHck;

DWORD pointerUH = 0x1D7AF68;
DWORD pointerUS = 0x02302060;
//DWORD pointerUS = 0x1D7AF68;
DWORD pointerUA = 0x02302060;
DWORD pointerNC = 0x02302060;
DWORD pointerRF = 0x02302060;
DWORD pointer00 = 0x02302060;

/*---------------------------------------------------------------------------------------------*/

void onPaintIntro(HDC hdc)
{
	Graphics graphics(hdc);
	graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);
	FontFamily fontFamily(L"Calibri");

	// Draw background image for intro
	Image introBg(L"Intro_border.png");
	UINT introBgWidth = introBg.GetWidth();
	UINT introBgHeight = introBg.GetHeight();
	Rect introBgRect(0, 0, introBgWidth, introBgHeight);
	graphics.DrawImage(&introBg, introBgRect, 0, 0, introBgWidth, introBgHeight, UnitPixel);

	// Draw top bar for minimize and close button for intro
	Pen blackPen(Color(0, 68, 68, 68), 3);
	Rect rect(205 / 2, 0, 590, 25);
	graphics.DrawRectangle(&blackPen, rect);
	/*SolidBrush solidBrush(Color(255, 68, 68, 68));
	graphics.FillRectangle(&solidBrush, 205 / 2, 0, 590, 25);*/

	if (hoverMn == true) {
		// Draw minimize button for intro (active)
		Pen penMn(Color(255, BtnBgActive, BtnBgActive, BtnBgActive), 3);
		Rect rectMn(694, 0, 25, 25);
		graphics.DrawRectangle(&penMn, rectMn);
		SolidBrush solidBrushMn(Color(255, BtnBgActive, BtnBgActive, BtnBgActive));
		graphics.FillRectangle(&solidBrushMn, 694, 0, 25, 25);
	}
	else if (hoverMn == false) {
		// Draw minimize button for intro (inactive)
		Pen penMn(Color(255, BtnBgInactive, BtnBgInactive, BtnBgInactive), 3);
		Rect rectMn(694, 0, 25, 25);
		graphics.DrawRectangle(&penMn, rectMn);
		SolidBrush solidBrushMn(Color(255, BtnBgInactive, BtnBgInactive, BtnBgInactive));
		graphics.FillRectangle(&solidBrushMn, 694, 0, 25, 25);
	}

	if (hoverCl == true) {
		// Draw close button for intro (active)
		Pen penCl(Color(255, BtnBgActive, BtnBgActive, BtnBgActive), 3);
		Rect rectCl(722, 0, 25, 25);
		graphics.DrawRectangle(&penCl, rectCl);
		SolidBrush solidBrushCl(Color(255, BtnBgActive, BtnBgActive, BtnBgActive));
		graphics.FillRectangle(&solidBrushCl, 722, 0, 25, 25);
	}
	else if (hoverCl == false) {
		// Draw close button for intro (unactive)
		Pen penCl(Color(255, BtnBgInactive, BtnBgInactive, BtnBgInactive), 3);
		Rect rectCl(722, 0, 25, 25);
		graphics.DrawRectangle(&penCl, rectCl);
		SolidBrush solidBrushCl(Color(255, BtnBgInactive, BtnBgInactive, BtnBgInactive));
		graphics.FillRectangle(&solidBrushCl, 722, 0, 25, 25);
	}

	Font fontBtn(&fontFamily, 13, FontStyleBold, UnitPoint);
	SolidBrush solidBrushBtn(Color(255, 153, 153, 153));
	WCHAR Mn[] = L"_";
	WCHAR Cl[] = L"X";
	PointF pointFMn(700.0f, -2.0f);
	PointF pointFCl(728.0f, 3.0f);

	graphics.DrawString(Mn, -1, &fontBtn, pointFMn, NULL, &solidBrushBtn);
	graphics.DrawString(Cl, -1, &fontBtn, pointFCl, NULL, &solidBrushBtn);

	//RECT rect{ 340, 360, 506, 480 };
	//SolidBrush tesst(Color(255, 255, 0, 0));
	//graphics.FillRectangle(&tesst, 340, 360, 166, 120);
}

/*---------------------------------------------------------------------------------------------*/

void statusIntro(HDC hdc)
{
	Graphics graphics(hdc);
	graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);
	FontFamily fontFamily(L"Calibri");

	// Draw game status for intro
	if (status() == false) {
		static bool initialized;
		HWND hWndIntro = NULL;
		RECT rect{ 340, 360, 506, 480 };
		if (!initialized) {
			initialized = true;
			InvalidateRect(hWndIntro, &rect, FALSE);
			//RedrawWindow(hWndIntro, &rect, NULL, RDW_INVALIDATE);
		}
		WCHAR status[] = L"Not Running";
		Font fontStatus(&fontFamily, 20, FontStyleBoldItalic, UnitPoint);
		PointF pointFStatus(350.0f, 355.0f);
		// red
		SolidBrush solidBrushStatus(Color(255, 255, 0, 0));

		graphics.DrawString(status, -1, &fontStatus, pointFStatus, NULL, &solidBrushStatus);
	}
	else if (status() == true) {
		static bool initialized;
		HWND hWndIntro = NULL;
		RECT rect{ 340, 360, 506, 480 };
		if (!initialized) {
			initialized = true;
			InvalidateRect(hWndIntro, &rect, FALSE);
			//RedrawWindow(hWndIntro, &rect, NULL, RDW_INVALIDATE);
		}
		WCHAR status[] = L"Running";
		Font fontStatus(&fontFamily, 20, FontStyleBoldItalic, UnitPoint);
		PointF pointFStatus(370.0f, 355.0f);
		// green
		SolidBrush solidBrushStatus(Color(255, 0, 255, 0));

		graphics.DrawString(status, -1, &fontStatus, pointFStatus, NULL, &solidBrushStatus);

		if (hoverEnter == true) {
			WCHAR enter[] = L"Enter Trainer";
			Font fontEnter(&fontFamily, 22, FontStyleRegular, UnitPoint);
			PointF pointFEnter(340.0f, 402.0f);
			// #999
			SolidBrush solidBrushEnter(Color(255, 153, 153, 153));

			graphics.DrawString(enter, -1, &fontEnter, pointFEnter, NULL, &solidBrushEnter);
		}
		else if (hoverEnter == false) {
			WCHAR enter[] = L"Enter Trainer";
			Font fontEnter(&fontFamily, 20, FontStyleRegular, UnitPoint);
			PointF pointFEnter(348.0f, 405.0f);
			// #999
			SolidBrush solidBrushEnter(Color(255, 153, 153, 153));

			graphics.DrawString(enter, -1, &fontEnter, pointFEnter, NULL, &solidBrushEnter);
		}
	}
}

void statusIntro1(HDC hdc)
{
	Graphics graphics(hdc);
	graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);
	FontFamily fontFamily(L"Calibri");

	// Draw game status for intro
	if (status() == false) {
		static bool initialized;
		HWND hWndIntro = NULL;
		RECT rect{ 340, 360, 506, 480 };
		if (!initialized) {
			initialized = true;
			InvalidateRect(hWndIntro, &rect, FALSE);
			//RedrawWindow(hWndIntro, &rect, NULL, RDW_INVALIDATE);
		}
		WCHAR status[] = L"Not Running";
		Font fontStatus(&fontFamily, 20, FontStyleBoldItalic, UnitPoint);
		PointF pointFStatus(350.0f, 355.0f);
		// red
		SolidBrush solidBrushStatus(Color(255, 255, 0, 0));

		graphics.DrawString(status, -1, &fontStatus, pointFStatus, NULL, &solidBrushStatus);
	}
}

void statusIntro2(HDC hdc)
{
	Graphics graphics(hdc);
	graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);
	FontFamily fontFamily(L"Calibri");

	if (status() == true) {
		static bool initialized;
		HWND hWndIntro = NULL;
		RECT rect{ 340, 360, 506, 480 };
		if (!initialized) {
			initialized = true;
			InvalidateRect(hWndIntro, &rect, FALSE);
			//RedrawWindow(hWndIntro, &rect, NULL, RDW_INVALIDATE);
		}
		WCHAR status[] = L"Running";
		Font fontStatus(&fontFamily, 20, FontStyleBoldItalic, UnitPoint);
		PointF pointFStatus(370.0f, 355.0f);
		// green
		SolidBrush solidBrushStatus(Color(255, 0, 255, 0));

		graphics.DrawString(status, -1, &fontStatus, pointFStatus, NULL, &solidBrushStatus);

		if (hoverEnter == true) {
			WCHAR enter[] = L"Enter Trainer";
			Font fontEnter(&fontFamily, 22, FontStyleRegular, UnitPoint);
			PointF pointFEnter(340.0f, 402.0f);
			// #999
			SolidBrush solidBrushEnter(Color(255, 153, 153, 153));

			graphics.DrawString(enter, -1, &fontEnter, pointFEnter, NULL, &solidBrushEnter);
		}
		else if (hoverEnter == false) {
			WCHAR enter[] = L"Enter Trainer";
			Font fontEnter(&fontFamily, 20, FontStyleRegular, UnitPoint);
			PointF pointFEnter(348.0f, 405.0f);
			// #999
			SolidBrush solidBrushEnter(Color(255, 153, 153, 153));

			graphics.DrawString(enter, -1, &fontEnter, pointFEnter, NULL, &solidBrushEnter);
		}
	}
}

/*---------------------------------------------------------------------------------------------*/

void onPaintMain(HDC hdc)
{
	Graphics graphics(hdc);
	graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);
	FontFamily fontFamily(L"Calibri");

	// Draw background image for main
	Image introBg(L"Main_border_lineorange.png");
	UINT introBgWidth = introBg.GetWidth();
	UINT introBgHeight = introBg.GetHeight();
	Rect introBgRect(0, 0, introBgWidth, introBgHeight);
	graphics.DrawImage(&introBg, introBgRect, 0, 0, introBgWidth, introBgHeight, UnitPixel);

	// Draw top bar for minimize and close button for intro
	Pen blackPen(Color(0, 68, 68, 68), 3);
	Rect rect(205 / 2, 0, 645, 25);
	graphics.DrawRectangle(&blackPen, rect);
	/*SolidBrush solidBrush(Color(255, 68, 68, 68));
	graphics.FillRectangle(&solidBrush, 205 / 2, 0, 645, 25);*/

	// Draw orange line for minimize and close button
	Pen orangePen(Color(255, 237, 111, 0), 2);
	Rect line(125, 25, 600, 1);
	graphics.DrawRectangle(&orangePen, line);

	if (hoverMn == true) {
		// Draw minimize button for main (active)
		Pen penMn(Color(255, 237, 111, 0), 3);
		Rect rectMn(670, 28, 25, 25);
		graphics.DrawRectangle(&penMn, rectMn);
		SolidBrush solidBrushMn(Color(255, 237, 111, 0));
		graphics.FillRectangle(&solidBrushMn, 670, 28, 25, 25);
	}
	else if (hoverMn == false) {
		// Draw minimize button for main (inactive)
		Pen penMn(Color(255, BtnBgInactive, BtnBgInactive, BtnBgInactive), 3);
		Rect rectMn(670, 30, 25, 25);
		graphics.DrawRectangle(&penMn, rectMn);
		SolidBrush solidBrushMn(Color(255, BtnBgInactive, BtnBgInactive, BtnBgInactive));
		graphics.FillRectangle(&solidBrushMn, 670, 30, 25, 25);
	}

	if (hoverCl == true) {
		// Draw close button for main (active)
		Pen penCl(Color(255, 237, 111, 0), 3);
		Rect rectCl(698, 28, 25, 25);
		graphics.DrawRectangle(&penCl, rectCl);
		SolidBrush solidBrushCl(Color(255, 237, 111, 0));
		graphics.FillRectangle(&solidBrushCl, 698, 28, 25, 25);
	}
	else if (hoverCl == false) {
		// Draw close button for main (unactive)
		Pen penCl(Color(255, BtnBgInactive, BtnBgInactive, BtnBgInactive), 3);
		Rect rectCl(698, 30, 25, 25);
		graphics.DrawRectangle(&penCl, rectCl);
		SolidBrush solidBrushCl(Color(255, BtnBgInactive, BtnBgInactive, BtnBgInactive));
		graphics.FillRectangle(&solidBrushCl, 698, 30, 25, 25);
	}

	Font fontBtn(&fontFamily, 13, FontStyleBold, UnitPoint);
	SolidBrush solidBrushBtn(Color(255, 255, 255, 255));
	WCHAR Mn[] = L"_";
	WCHAR Cl[] = L"X";
	PointF pointFMn(676.0f, 26.0f);
	PointF pointFCl(704.0f, 31.0f);

	graphics.DrawString(Mn, -1, &fontBtn, pointFMn, NULL, &solidBrushBtn);
	graphics.DrawString(Cl, -1, &fontBtn, pointFCl, NULL, &solidBrushBtn);

	SolidBrush solidHckBrushClickActiveHoverActive(Color(255, 55, 255, 55));
	SolidBrush solidHckBrushClickActiveHoverInactive(Color(255, 0, 255, 0));

	SolidBrush solidHckBrushClickInactiveHoverActive(Color(255, 231, 76, 60));
	SolidBrush solidHckBrushClickInactiveHoverInactive(Color(255, 255, 102, 86));

	Pen hckPen(Color(255, 206, 51, 35), 5);
	Rect rectHck1(132, 310, 240, 4);
	Rect rectHck2(475, 200, 245, 110);
	Rect rectHck3(130, 360, 245, 110);
	Rect rectHck4(475, 360, 245, 110);
	Rect rectHck5(130, 520, 245, 110);
	Rect rectHck6(475, 520, 245, 110);

	Font fontHck(&fontFamily, 20, FontStyleRegular, UnitPoint);
	SolidBrush solidBrushHck(Color(255, 255, 255, 255));
	WCHAR UH[] = L"Godmode";
	WCHAR US[] = L"Unlimited Points";
	WCHAR UA[] = L"Unlimited Ammo";
	WCHAR NC[] = L"No Clip";
	WCHAR RF[] = L"Rapid Fire";
	WCHAR OO[] = L"Disable All";
	PointF pointFUHActive(190.0f, 248.0f);
	PointF pointFUHInactive(190.0f, 240.0f);
	PointF pointFUS(500.0f, 240.0f);
	PointF pointFUA(150.0f, 400.0f);
	PointF pointFNC(550.0f, 400.0f);
	PointF pointFRF(190.0f, 560.0f);
	PointF pointFDA(532.0f, 560.0f);

	graphics.DrawString(US, -1, &fontHck, pointFUS, NULL, &solidBrushHck);
	graphics.DrawString(UA, -1, &fontHck, pointFUA, NULL, &solidBrushHck);
	graphics.DrawString(NC, -1, &fontHck, pointFNC, NULL, &solidBrushHck);
	graphics.DrawString(RF, -1, &fontHck, pointFRF, NULL, &solidBrushHck);
	graphics.DrawString(OO, -1, &fontHck, pointFDA, NULL, &solidBrushHck);

	if (clickUH == true) {
		// Draw Godmode button for main (active)
		graphics.DrawRectangle(&hckPen, rectHck1);
		graphics.FillRectangle(&solidHckBrushClickActiveHoverActive, 130, 208, 245, 110);
		if (hoverUH == true) {
			graphics.DrawRectangle(&hckPen, rectHck1);
			graphics.FillRectangle(&solidHckBrushClickActiveHoverActive, 130, 208, 245, 110);
		}
		else if (hoverUH == false) {
			graphics.DrawRectangle(&hckPen, rectHck1);
			graphics.FillRectangle(&solidHckBrushClickActiveHoverInactive, 130, 208, 245, 110);
		}
		graphics.DrawString(UH, -1, &fontHck, pointFUHActive, NULL, &solidBrushHck);
	}
	else if (clickUH == false) {
		// Draw Godmode button for main (inactive)
		graphics.FillRectangle(&solidHckBrushClickInactiveHoverActive, 130, 200, 245, 110);
		graphics.DrawRectangle(&hckPen, rectHck1);
		if (hoverUH == true) {
			graphics.FillRectangle(&solidHckBrushClickInactiveHoverActive, 130, 200, 245, 110);
			graphics.DrawRectangle(&hckPen, rectHck1);
		}
		else if (hoverUH == false) {
			graphics.FillRectangle(&solidHckBrushClickInactiveHoverInactive, 130, 200, 245, 110);
			graphics.DrawRectangle(&hckPen, rectHck1);
		}
		graphics.DrawString(UH, -1, &fontHck, pointFUHInactive, NULL, &solidBrushHck);
	}

	if (clickUS == true) {
		// Draw Unlimited Points button for main (active)
		graphics.FillRectangle(&solidHckBrushClickActiveHoverActive, 475, 200, 245, 110);
		graphics.DrawRectangle(&hckPen, rectHck2);
		if (hoverUS == true) {
			graphics.FillRectangle(&solidHckBrushClickActiveHoverActive, 475, 200, 245, 110);
			graphics.DrawRectangle(&hckPen, rectHck2);
		}
		else if (hoverUS == false) {
			graphics.FillRectangle(&solidHckBrushClickActiveHoverInactive, 475, 200, 245, 110);
			graphics.DrawRectangle(&hckPen, rectHck2);
		}
	}
	else if (clickUS == false) {
		// Draw Unlimited Points button for main (inactive)
		graphics.FillRectangle(&solidHckBrushClickInactiveHoverActive, 475, 200, 245, 110);
		graphics.DrawRectangle(&hckPen, rectHck2);
		if (hoverUS == true) {
			graphics.FillRectangle(&solidHckBrushClickInactiveHoverActive, 475, 200, 245, 110);
			graphics.DrawRectangle(&hckPen, rectHck2);
		}
		else if (hoverUS == false) {
			graphics.FillRectangle(&solidHckBrushClickInactiveHoverInactive, 475, 200, 245, 110);
			graphics.DrawRectangle(&hckPen, rectHck2);
		}
	}

	if (clickUA == true) {
		// Draw Unlimited Ammo button for main (active)
		graphics.FillRectangle(&solidHckBrushClickActiveHoverActive, 130, 360, 245, 110);
		graphics.DrawRectangle(&hckPen, rectHck3);
		if (hoverUA == true) {
			graphics.FillRectangle(&solidHckBrushClickActiveHoverActive, 130, 360, 245, 110);
			graphics.DrawRectangle(&hckPen, rectHck3);
		}
		else if (hoverUA == false) {
			graphics.FillRectangle(&solidHckBrushClickActiveHoverInactive, 130, 360, 245, 110);
			graphics.DrawRectangle(&hckPen, rectHck3);
		}
	}
	else if (clickUA == false) {
		// Draw Unlimited Ammo button for main (inactive)
		graphics.FillRectangle(&solidHckBrushClickInactiveHoverActive, 130, 360, 245, 110);
		graphics.DrawRectangle(&hckPen, rectHck3);
		if (hoverUA == true) {
			graphics.FillRectangle(&solidHckBrushClickInactiveHoverActive, 130, 360, 245, 110);
			graphics.DrawRectangle(&hckPen, rectHck3);
		}
		else if (hoverUA == false) {
			graphics.FillRectangle(&solidHckBrushClickInactiveHoverInactive, 130, 360, 245, 110);
			graphics.DrawRectangle(&hckPen, rectHck3);
		}
	}

	if (clickNC == true) {
		// Draw No Clip button for main (active)
		graphics.FillRectangle(&solidHckBrushClickActiveHoverActive, 475, 360, 245, 110);
		graphics.DrawRectangle(&hckPen, rectHck4);
		if (hoverNC == true) {
			graphics.FillRectangle(&solidHckBrushClickActiveHoverActive, 475, 360, 245, 110);
			graphics.DrawRectangle(&hckPen, rectHck4);
		}
		else if (hoverNC == false) {
			graphics.FillRectangle(&solidHckBrushClickActiveHoverInactive, 475, 360, 245, 110);
			graphics.DrawRectangle(&hckPen, rectHck4);
		}
	}
	else if (clickNC == false) {
		// Draw No Clip button for main (inactive)
		graphics.FillRectangle(&solidHckBrushClickInactiveHoverActive, 475, 360, 245, 110);
		graphics.DrawRectangle(&hckPen, rectHck4);
		if (hoverNC == true) {
			graphics.FillRectangle(&solidHckBrushClickInactiveHoverActive, 475, 360, 245, 110);
			graphics.DrawRectangle(&hckPen, rectHck4);
		}
		else if (hoverNC == false) {
			graphics.FillRectangle(&solidHckBrushClickInactiveHoverInactive, 475, 360, 245, 110);
			graphics.DrawRectangle(&hckPen, rectHck4);
		}
	}

	if (clickRF == true) {
		// Draw Rapid Fire button for main (active)
		graphics.FillRectangle(&solidHckBrushClickActiveHoverActive, 130, 520, 245, 110);
		graphics.DrawRectangle(&hckPen, rectHck5);
		if (hoverRF == true) {
			graphics.FillRectangle(&solidHckBrushClickActiveHoverActive, 130, 520, 245, 110);
			graphics.DrawRectangle(&hckPen, rectHck5);
		}
		else if (hoverRF == false) {
			graphics.FillRectangle(&solidHckBrushClickActiveHoverInactive, 130, 520, 245, 110);
			graphics.DrawRectangle(&hckPen, rectHck5);
		}
	}
	else if (clickRF == false) {
		// Draw Rapid Fire button for main (inactive)
		graphics.FillRectangle(&solidHckBrushClickInactiveHoverActive, 130, 520, 245, 110);
		graphics.DrawRectangle(&hckPen, rectHck5);
		if (hoverRF == true) {
			graphics.FillRectangle(&solidHckBrushClickInactiveHoverActive, 130, 520, 245, 110);
			graphics.DrawRectangle(&hckPen, rectHck5);
		}
		else if (hoverRF == false) {
			graphics.FillRectangle(&solidHckBrushClickInactiveHoverInactive, 130, 520, 245, 110);
			graphics.DrawRectangle(&hckPen, rectHck5);
		}
	}

	if (clickDA == true) {
		// Draw Other button for main (active)
		graphics.FillRectangle(&solidHckBrushClickActiveHoverActive, 475, 520, 245, 110);
		graphics.DrawRectangle(&hckPen, rectHck6);
		if (hoverDA == true) {
			graphics.FillRectangle(&solidHckBrushClickActiveHoverActive, 475, 520, 245, 110);
			graphics.DrawRectangle(&hckPen, rectHck6);
		}
		else if (hoverDA == false) {
			graphics.FillRectangle(&solidHckBrushClickActiveHoverInactive, 475, 520, 245, 110);
			graphics.DrawRectangle(&hckPen, rectHck6);
		}
	}
	else if (clickDA == false) {
		// Draw Other button for main (inactive)
		graphics.FillRectangle(&solidHckBrushClickInactiveHoverActive, 475, 520, 245, 110);
		graphics.DrawRectangle(&hckPen, rectHck6);
		if (hoverDA == true) {
			graphics.FillRectangle(&solidHckBrushClickInactiveHoverActive, 475, 520, 245, 110);
			graphics.DrawRectangle(&hckPen, rectHck6);
		}
		else if (hoverDA == false) {
			graphics.FillRectangle(&solidHckBrushClickInactiveHoverInactive, 475, 520, 245, 110);
			graphics.DrawRectangle(&hckPen, rectHck6);
		}
	}
}

/*---------------------------------------------------------------------------------------------*/

int WINAPI WinMain(HINSTANCE hInstance,	HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// Initialize GDI+.
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	hInst = hInstance;

	WNDCLASSEX wcexIntro;
	wcexIntro.cbSize = sizeof(WNDCLASSEX);
	wcexIntro.style = CS_HREDRAW | CS_VREDRAW;
	wcexIntro.lpfnWndProc = WndProcIntro;
	wcexIntro.cbClsExtra = 0;
	wcexIntro.cbWndExtra = 0;
	wcexIntro.hInstance = hInst;
	wcexIntro.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(Icon));
	wcexIntro.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcexIntro.hbrBackground = CreateSolidBrush(RGB(5, 0, 0));
	wcexIntro.lpszMenuName = NULL;
	wcexIntro.lpszClassName = szWindowClassIntro;
	wcexIntro.hIconSm = NULL;

	WNDCLASSEX wcexMain;
	wcexMain.cbSize = sizeof(WNDCLASSEX);
	wcexMain.style = CS_HREDRAW | CS_VREDRAW;
	wcexMain.lpfnWndProc = WndProcMain;
	wcexMain.cbClsExtra = 0;
	wcexMain.cbWndExtra = 0;
	wcexMain.hInstance = hInst;
	wcexMain.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(Icon));
	wcexMain.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcexMain.hbrBackground = CreateSolidBrush(RGB(5, 0, 0));
	wcexMain.lpszMenuName = NULL;
	wcexMain.lpszClassName = szWindowClassMain;
	wcexMain.hIconSm = NULL;

	if (!RegisterClassEx(&wcexIntro) || !RegisterClassEx(&wcexMain))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Call of Duty Black Ops II Trainer"),
			NULL);

		return 1;
	}

	HWND hWndIntro = CreateWindow(
		szWindowClassIntro,
		szTitle,
		WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT,
		INTRO_WIDTH, INTRO_HEIGHT,
		NULL,
		NULL,
		hInst,
		NULL
		);

	SetWindowLong(hWndIntro, GWL_STYLE, WS_VISIBLE);
	SetWindowLong(hWndIntro, GWL_EXSTYLE, GetWindowLong(hWndIntro, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(hWndIntro, RGB(5, 0, 0), 0, LWA_COLORKEY);

	HWND hWndMain = CreateWindow(
		szWindowClassMain,
		szTitle,
		WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT,
		MAIN_WIDTH, MAIN_HEIGHT,
		NULL,
		NULL,
		hInst,
		NULL
		);

	SetWindowLong(hWndMain, GWL_STYLE, WS_VISIBLE);
	SetWindowLong(hWndMain, GWL_EXSTYLE, GetWindowLong(hWndMain, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(hWndMain, RGB(5, 0, 0), 0, LWA_COLORKEY);

	// If one of windows failes to be created
	if (!hWndIntro || !hWndMain)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Call of Duty Black Ops II Trainer"),
			NULL);

		return 1;
	}

	// Show window
	ShowWindow(hWndIntro, nCmdShow);
	UpdateWindow(hWndIntro);
	ShowWindow(hWndMain, nCmdShow);
	UpdateWindow(hWndMain);
	ShowWindow(hWndMain, SW_HIDE);

	// Main message loop
	MSG msg;
	while (GetMessage(&msg, NULL , 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	GdiplusShutdown(gdiplusToken);
	return (int)msg.wParam;	
}

/*---------------------------------------------------------------------------------------------*/

LRESULT CALLBACK WndProcIntro(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	WNDCLASSEX wcex;
	wcex.hCursor = LoadCursor(NULL, IDC_HAND);

	HWND hWndMain = FindWindow(szWindowClassMain, NULL);

	switch (message)
	{
		// WM_LBUTTONDOWN
	case WM_LBUTTONDOWN:
	{
		int iPosX = GET_X_LPARAM(lParam);
		int iPosY = GET_Y_LPARAM(lParam);

		// Pass cords when clicked on top bar
		if (iPosX > 99 && iPosX < 693 && iPosY > -1 && iPosY < 26) {
			moveWindow = true;
			SetCapture(hWnd);
			GetWindowRect(hWnd, &rcWindow);
			GetCursorPos(&pos);
			ScreenToClient(hWnd, &pos);
		}
		else {
			moveWindow = false;
		}

		// Minimize window on click
		if (iPosX > 693 && iPosX < 720 && iPosY > -1 && iPosY < 26) {
			hoverMn = false;
			ShowWindow(hWnd, SW_MINIMIZE);
		}

		// Close window on click
		if (iPosX > 721 && iPosX < 747 && iPosY > -1 && iPosY < 26) {
			hoverMn = false;
			DestroyWindow(hWnd);
		}

		// Enter trainer on click
		if (status() == true) {
			if (iPosX > 348 && iPosX < 502 && iPosY > 412 && iPosY < 432) {
				hoverEnter = false;
				ShowWindow(hWnd, SW_HIDE);
				ShowWindow(hWndMain, SW_SHOW);
			}
		}

		/*wchar_t d[20];
		wsprintf(d, _T("(%i, %i"), iPosX, iPosY);
		MessageBox(hWnd, d, _T("click"), MB_OK);*/

		break;
	}

		// WM_LBUTTONUP
	case WM_LBUTTONUP:
	{
		ReleaseCapture();
		break;
	}

		// WM_MOUSEMOVE
	case WM_MOUSEMOVE:
	{
		int iPosX = GET_X_LPARAM(lParam);
		int iPosY = GET_Y_LPARAM(lParam);

		RECT rectMn{ 693, 0, 721, 27 };
		RECT rectCl{ 721, 0, 749, 27 };

		// Move window
		if (moveWindow == true) {
			GetCursorPos(&curPos);
			if (wParam == MK_LBUTTON)
			{
				int x = curPos.x - pos.x;
				int y = curPos.y - pos.y;
				int width = rcWindow.right - rcWindow.left;
				int height = rcWindow.bottom - rcWindow.top;
				MoveWindow(hWnd, x, y, width, height, FALSE);
			}
		}

		// Minimize button: hover
		if (iPosX > 693 && iPosX < 720 && iPosY > -1 && iPosY < 26) {
			hoverMn = true;
			SetCursor(wcex.hCursor);
		}
		else {
			hoverMn = false;
		}
		static bool initializedMn;
		if (hoverMn == true) {
			if (!initializedMn) {
				initializedMn = true;
				InvalidateRect(hWnd, &rectMn, FALSE);
				//RedrawWindow(hWnd, &rectMn, NULL, RDW_INVALIDATE);
			}
		}
		else {
			if (initializedMn) {
				initializedMn = false;
				InvalidateRect(hWnd, &rectMn, FALSE);
				//RedrawWindow(hWnd, &rectMn, NULL, RDW_INVALIDATE);
			}
		}

		// Close button: hover
		if (iPosX > 721 && iPosX < 747 && iPosY > -1 && iPosY < 26) {
			hoverCl = true;
			SetCursor(wcex.hCursor);
		}
		else {
			hoverCl = false;
		}
		static bool initializedCl;
		if (hoverCl == true) {
			if (!initializedCl) {
				initializedCl = true;
				InvalidateRect(hWnd, &rectCl, FALSE);
				//RedrawWindow(hWnd, &rectCl, NULL, RDW_INVALIDATE);
			}
		}
		else {
			if (initializedCl) {
				initializedCl = false;
				InvalidateRect(hWnd, &rectCl, FALSE);
				//RedrawWindow(hWnd, &rectCl, NULL, RDW_INVALIDATE);
			}
		}

		// Enter trainer: hover
		if (status() == true) {
			RECT rect{ 340, 432, 506, 412 };
			if (iPosX > 348 && iPosX < 502 && iPosY > 412 && iPosY < 432) {
				hoverEnter = true;
				SetCursor(wcex.hCursor);
			}
			else {
				hoverEnter = false;
			}
			static bool initialized;
			if (hoverEnter == true) {
				if (!initialized) {
					initialized = true;
					InvalidateRect(hWnd, &rect, FALSE);
					//RedrawWindow(hWnd, &rect, NULL, RDW_INVALIDATE);
				}
			}
			else {
				if (initialized) {
					initialized = false;
					InvalidateRect(hWnd, &rect, FALSE);
					//RedrawWindow(hWnd, &rect, NULL, RDW_INVALIDATE);
				}
			}
		}
		break;
	}

		// WM_PAINT
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);

		RECT rect{ 340, 360, 506, 480 };
		InvalidateRect(hWnd, &rect, FALSE);

		onPaintIntro(hdc);
		statusIntro1(hdc);
		statusIntro2(hdc);

		EndPaint(hWnd, &ps);
		break;
	}

	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}

	default:
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	}

	return 0;
}

/*---------------------------------------------------------------------------------------------*/

LRESULT CALLBACK WndProcMain(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	WNDCLASSEX wcex;
	wcex.hCursor = LoadCursor(NULL, IDC_HAND);

	switch (message)
	{
	// WM_LBUTTONDOWN
	case WM_LBUTTONDOWN:
	{
		int iPosX = GET_X_LPARAM(lParam);
		int iPosY = GET_Y_LPARAM(lParam);

		RECT rectUH{ 130, 200, 375, 330 };
		RECT rectUS{ 475, 200, 720, 330 };
		RECT rectUA{ 130, 360, 375, 470 };
		RECT rectNC{ 475, 360, 720, 470 };
		RECT rectRF{ 130, 520, 375, 630 };
		RECT rectOO{ 475, 520, 720, 630 };

		// Pass cords when clicked on top bar
		if (iPosX > 99 && iPosX < 748 && iPosY > -1 && iPosY < 26) {
			moveWindow = true;
			SetCapture(hWnd);
			GetWindowRect(hWnd, &rcWindow);
			GetCursorPos(&pos);
			ScreenToClient(hWnd, &pos);
		}
		else {
			moveWindow = false;
		}

		// Minimize window on click
		if (iPosX > 669 && iPosX < 696 && iPosY > 27 && iPosY < 61) {
			hoverMn = false;
			ShowWindow(hWnd, SW_MINIMIZE);
		}

		// Close window on click
		if (iPosX > 697 && iPosX < 724 && iPosY > 27 && iPosY < 61) {
			hoverMn = false;
			DestroyWindow(hWnd);
		}

		// GODMODE
		if (clickUH == true) {
			if (iPosX > 127 && iPosX < 378 && iPosY > 197 && iPosY < 312) {
				clickUH = false;
				SetCursor(wcex.hCursor);
			}
		}
		else if (iPosX > 127 && iPosX < 378 && iPosY > 197 && iPosY < 312) {
			clickUH = true;
			SetCursor(wcex.hCursor);
		}
		static bool initializedUH;
		if (clickUH == true) {
			if (!initializedUH) {
				initializedUH = true;
				InvalidateRect(hWnd, &rectUH, FALSE);
			}
		}
		else {
			if (initializedUH) {
				initializedUH = false;
				InvalidateRect(hWnd, &rectUH, FALSE);
			}
		}

		// UNLIMITED POINTS
		if (clickUS == true) {
			if (iPosX > 472 && iPosX < 723 && iPosY > 197 && iPosY < 312) {
				clickUS = false;
				SetCursor(wcex.hCursor);
			}
		}
		else if (iPosX > 472 && iPosX < 723 && iPosY > 197 && iPosY < 312) {
			clickUS = true;
			SetCursor(wcex.hCursor);
		}
		static bool initializedUS;
		if (clickUS == true) {
			if (!initializedUS) {
				initializedUS = true;
				InvalidateRect(hWnd, &rectUS, FALSE);
			}
		}
		else {
			if (initializedUS) {
				initializedUS = false;
				InvalidateRect(hWnd, &rectUS, FALSE);
			}
		}

		// UNLIMITED AMMO
		if (clickUA == true) {
			if (iPosX > 127 && iPosX < 378 && iPosY > 357 && iPosY < 472) {
				clickUA = false;
				SetCursor(wcex.hCursor);
			}
		}
		else if (iPosX > 127 && iPosX < 378 && iPosY > 357 && iPosY < 472) {
			clickUA = true;
			SetCursor(wcex.hCursor);
		}
		static bool initializedUA;
		if (clickUA == true) {
			if (!initializedUA) {
				initializedUA = true;
				InvalidateRect(hWnd, &rectUA, FALSE);
			}
		}
		else {
			if (initializedUA) {
				initializedUA = false;
				InvalidateRect(hWnd, &rectUA, FALSE);
			}
		}

		// NO CAP
		if (clickNC == true) {
			if (iPosX > 472 && iPosX < 723 && iPosY > 357 && iPosY < 472) {
				clickNC = false;
				SetCursor(wcex.hCursor);
			}
		}
		else if (iPosX > 472 && iPosX < 723 && iPosY > 357 && iPosY < 472) {
			clickNC = true;
			SetCursor(wcex.hCursor);
		}
		static bool initializedNC;
		if (clickNC == true) {
			if (!initializedNC) {
				initializedNC = true;
				InvalidateRect(hWnd, &rectNC, FALSE);
			}
		}
		else {
			if (initializedNC) {
				initializedNC = false;
				InvalidateRect(hWnd, &rectNC, FALSE);
			}
		}

		// RAPID FIRE
		if (clickRF == true) {
			if (iPosX > 127 && iPosX < 378 && iPosY > 517 && iPosY < 632) {
				clickRF = false;
				SetCursor(wcex.hCursor);
			}
		}
		else if (iPosX > 127 && iPosX < 378 && iPosY > 517 && iPosY < 632) {
			clickRF = true;
			SetCursor(wcex.hCursor);
		}
		static bool initializedRF;
		if (clickRF == true) {
			if (!initializedRF) {
				initializedRF = true;
				InvalidateRect(hWnd, &rectRF, FALSE);
			}
		}
		else {
			if (initializedRF) {
				initializedRF = false;
				InvalidateRect(hWnd, &rectRF, FALSE);
			}
		}

		// OTHER
		if (clickDA == true) {
			if (iPosX > 472 && iPosX < 723 && iPosY > 517 && iPosY < 632) {
				clickDA = false;
				SetCursor(wcex.hCursor);
			}
		}
		else if (iPosX > 472 && iPosX < 723 && iPosY > 517 && iPosY < 632) {
			clickDA = true;
			SetCursor(wcex.hCursor);
		}
		static bool initializedOO;
		if (clickDA == true) {
			if (!initializedOO) {
				initializedOO = true;
				InvalidateRect(hWnd, &rectOO, FALSE);
			}
		}
		else {
			if (initializedOO) {
				initializedOO = false;
				InvalidateRect(hWnd, &rectOO, FALSE);
			}
		}
		break;
	}

	// WM_LBUTTONUP
	case WM_LBUTTONUP:
	{
		int iPosX = GET_X_LPARAM(lParam);
		int iPosY = GET_Y_LPARAM(lParam);

		RECT rectUH{ 130, 200, 375, 330 };
		RECT rectUS{ 475, 200, 720, 330 };
		RECT rectUA{ 130, 360, 375, 470 };
		RECT rectNC{ 475, 360, 720, 470 };
		RECT rectRF{ 130, 520, 375, 630 };
		RECT rectOO{ 475, 520, 720, 630 };

		ReleaseCapture();

		// GODMODE
		if (iPosX > 127 && iPosX < 378 && iPosY > 197 && iPosY < 312) {
			SetCursor(wcex.hCursor);
		}

		if (clickUH == true) {
			clickUH = false;
		}

		// UNLIMITED POINTS
		if (iPosX > 472 && iPosX < 723 && iPosY > 197 && iPosY < 312) {
			SetCursor(wcex.hCursor);
		}

		// UNLIMITED AMMO
		if (iPosX > 127 && iPosX < 378 && iPosY > 357 && iPosY < 472) {
			SetCursor(wcex.hCursor);
		}

		// NO CAP
		if (iPosX > 472 && iPosX < 723 && iPosY > 357 && iPosY < 472) {
			SetCursor(wcex.hCursor);
		}

		// RAPID FIRE
		if (iPosX > 127 && iPosX < 378 && iPosY > 517 && iPosY < 632) {
			SetCursor(wcex.hCursor);
		}

		// OTHER
		if (iPosX > 472 && iPosX < 723 && iPosY > 517 && iPosY < 632) {
			SetCursor(wcex.hCursor);
		}

		break;
	}

	// WM_MOUSEMOVE
	case WM_MOUSEMOVE:
	{
		int iPosX = GET_X_LPARAM(lParam);
		int iPosY = GET_Y_LPARAM(lParam);

		RECT rectMn{ 669, 27, 697, 62 };
		RECT rectCl{ 697, 27, 725, 62 };
		RECT rectUH{ 130, 200, 375, 330 };
		RECT rectUS{ 475, 200, 720, 330 };
		RECT rectUA{ 130, 360, 375, 470 };
		RECT rectNC{ 475, 360, 720, 470 };
		RECT rectRF{ 130, 520, 375, 630 };
		RECT rectOO{ 475, 520, 720, 630 };

		// Move window
		if (moveWindow == true) {
			GetCursorPos(&curPos);
			if (wParam == MK_LBUTTON)
			{
				int x = curPos.x - pos.x;
				int y = curPos.y - pos.y;
				int width = rcWindow.right - rcWindow.left;
				int height = rcWindow.bottom - rcWindow.top;
				MoveWindow(hWnd, x, y, width, height, FALSE);
			}
		}

		// Minimize button: hover
		if (iPosX > 669 && iPosX < 696 && iPosY > 27 && iPosY < 61) {
			hoverMn = true;
			SetCursor(wcex.hCursor);
		}
		else {
			hoverMn = false;
		}
		static bool initializedMn;
		if (hoverMn == true) {
			if (!initializedMn) {
				initializedMn = true;
				InvalidateRect(hWnd, &rectMn, FALSE);
				//RedrawWindow(hWnd, &rectMn, NULL, RDW_INVALIDATE);
			}
		}
		else {
			if (initializedMn) {
				initializedMn = false;
				InvalidateRect(hWnd, &rectMn, FALSE);
				//RedrawWindow(hWnd, &rectMn, NULL, RDW_INVALIDATE);
			}
		}

		// Close button: hover
		if (iPosX > 697 && iPosX < 724 && iPosY > 27 && iPosY < 61) {
			hoverCl = true;
			SetCursor(wcex.hCursor);
		}
		else {
			hoverCl = false;
		}
		static bool initializedCl;
		if (hoverCl == true) {
			if (!initializedCl) {
				initializedCl = true;
				InvalidateRect(hWnd, &rectCl, FALSE);
				//RedrawWindow(hWnd, &rectCl, NULL, RDW_INVALIDATE);
			}
		}
		else {
			if (initializedCl) {
				initializedCl = false;
				InvalidateRect(hWnd, &rectCl, FALSE);
				//RedrawWindow(hWnd, &rectCl, NULL, RDW_INVALIDATE);
			}
		}

		// GODMODE
		if (iPosX > 127 && iPosX < 378 && iPosY > 197 && iPosY < 312) {
			hoverUH = true;
			SetCursor(wcex.hCursor);
		}
		else {
			hoverUH = false;
		}
		static bool initializedUH;
		if (hoverUH == true) {
			if (!initializedUH) {
				initializedUH = true;
				InvalidateRect(hWnd, &rectUH, FALSE);
			}
		}
		else {
			if (initializedUH) {
				initializedUH = false;
				InvalidateRect(hWnd, &rectUH, FALSE);
			}
		}

		// UNLIMITED POINTS
		if (iPosX > 472 && iPosX < 723 && iPosY > 197 && iPosY < 312) {
			hoverUS = true;
			SetCursor(wcex.hCursor);
		}
		else {
			hoverUS = false;
		}
		static bool initializedUS;
		if (hoverUS == true) {
			if (!initializedUS) {
				initializedUS = true;
				InvalidateRect(hWnd, &rectUS, FALSE);
			}
		}
		else {
			if (initializedUS) {
				initializedUS = false;
				InvalidateRect(hWnd, &rectUS, FALSE);
			}
		}

		// UNLIMITED AMMO
		if (iPosX > 127 && iPosX < 378 && iPosY > 357 && iPosY < 472) {
			hoverUA = true;
			SetCursor(wcex.hCursor);
		}
		else {
			hoverUA = false;
		}
		static bool initializedUA;
		if (hoverUA == true) {
			if (!initializedUA) {
				initializedUA = true;
				InvalidateRect(hWnd, &rectUA, FALSE);
			}
		}
		else {
			if (initializedUA) {
				initializedUA = false;
				InvalidateRect(hWnd, &rectUA, FALSE);
			}
		}

		// NO CAP
		if (iPosX > 472 && iPosX < 723 && iPosY > 357 && iPosY < 472) {
			hoverNC = true;
			SetCursor(wcex.hCursor);
		}
		else {
			hoverNC = false;
		}
		static bool initializedNC;
		if (hoverNC == true) {
			if (!initializedNC) {
				initializedNC = true;
				InvalidateRect(hWnd, &rectNC, FALSE);
			}
		}
		else {
			if (initializedNC) {
				initializedNC = false;
				InvalidateRect(hWnd, &rectNC, FALSE);
			}
		}

		// RAPID FIRE
		if (iPosX > 127 && iPosX < 378 && iPosY > 517 && iPosY < 632) {
			hoverRF = true;
			SetCursor(wcex.hCursor);
		}
		else {
			hoverRF = false;
		}
		static bool initializedRF;
		if (hoverRF == true) {
			if (!initializedRF) {
				initializedRF = true;
				InvalidateRect(hWnd, &rectRF, FALSE);
			}
		}
		else {
			if (initializedRF) {
				initializedRF = false;
				InvalidateRect(hWnd, &rectRF, FALSE);
			}
		}

		// OTHER
		if (iPosX > 472 && iPosX < 723 && iPosY > 517 && iPosY < 632) {
			hoverDA = true;
			SetCursor(wcex.hCursor);
		}
		else {
			hoverDA = false;
		}
		static bool initializedOO;
		if (hoverDA == true) {
			if (!initializedOO) {
				initializedOO = true;
				InvalidateRect(hWnd, &rectOO, FALSE);
			}
		}
		else {
			if (initializedOO) {
				initializedOO = false;
				InvalidateRect(hWnd, &rectOO, FALSE);
			}
		}
		break;
	}

	// WM_PAINT
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);

		onPaintMain(hdc);

		/*static bool initialized;
		if (!initialized) {
			initialized = true;
			if (hWndBO2 == 0) {
				MessageBox(NULL,
					_T("Failed to find process!"),
					_T("Call of Duty Black Ops II Trainer"),
					NULL);

				return 1;
			}
			else {
				GetWindowThreadProcessId(hWndBO2, &procId);
				hProcHck = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);
				if (!hProcHck) {
					MessageBox(NULL,
						_T("Failed to open process!"),
						_T("Call of Duty Black Ops II Trainer"),
						NULL);

					return 1;
				}
				else {
					DWORD pointed;
					ReadProcessMemory(hProcHck, (LPCVOID)(pointerUS), &pointed, 4, NULL);
					char test[10];
					sprintf_s(test, "%d", pointed);
					MessageBox(NULL,
						(LPCWSTR)test,
						_T("Call of Duty Black Ops II Trainer"),
						NULL);

					/*int t = WriteProcessMemory(hWndBO2, (LPVOID)pointerUS, &valuePoints, (DWORD)sizeof(valuePoints), NULL);
					if (t > 0) {
					MessageBox(NULL,
					_T("Succes!"),
					_T("Call of Duty Black Ops II Trainer"),
					NULL);
					}
					else {
					MessageBox(NULL,
					_T("Not succes!"),
					_T("Call of Duty Black Ops II Trainer"),
					NULL);
					}
				}
			}

			CloseHandle(hProcHck);
		}*/

		EndPaint(hWnd, &ps);
		break;
	}

	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}

	default:
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	}

	return 0;
}