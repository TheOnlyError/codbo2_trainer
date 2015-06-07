// codbo2_trainer: Main.cpp
// Gijs de Jong

// Includes and namespaces
#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>
#include <string>
#include <tchar.h>
#include <thread>
#include <objidl.h>
#include <gdiplus.h>
#include <tlhelp32.h>
#include "../Header Files/Res.h"
#include "../Header Files/FindProcess.h"
#include "../Header Files/WriteProcess.h"

using namespace Gdiplus;
using namespace std;
#pragma comment (lib,"Gdiplus.lib")

/*---------------------------------------------------------------------------------------------*/
// Forward declarations

static TCHAR szWindowClassIntro[] = _T("codbo2_trainer_intro");
static TCHAR szWindowClassMain[] = _T("codbo2_trainer_main");
static TCHAR szWindowClassSettings[] = _T("codbo2_trainer_settings");
static TCHAR szTitle[] = _T("Call of Duty Black Ops II Trainer");

HINSTANCE hInst = NULL;

LRESULT CALLBACK WndProcIntro(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProcMain(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProcSettings(HWND, UINT, WPARAM, LPARAM);

const int INTRO_WIDTH = 850;
const int INTRO_HEIGHT = 534;

const int MAIN_WIDTH = 850;
const int MAIN_HEIGHT = 700;

const int SETTINGS_WIDTH = 425;
const int SETTINGS_HEIGHT = 350;

bool moveWindow = false;
RECT rcWindow;
POINT pos;
POINT curPos;

bool hoverEnter = false;

bool hoverSe = false;
bool hoverMn = false;
bool hoverCl = false;

// #000
int BtnBgInactive = 0;
// #222
int BtnBgActive = 34;

bool downUH = false;
bool downUS = false;
bool downUA = false;
bool downNC = false;
bool downRF = false;
bool downDA = false;

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

bool advanced = true;
bool friendly = false;

/*---------------------------------------------------------------------------------------------*/
// Following function is used to paint everything on intro window

void onPaintIntro(HDC hdc)
{
	Graphics graphics(hdc);
	graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);
	FontFamily fontFamily(L"Calibri");

	// Draw background image for intro
	Image introBg(L"Resource Files/Intro_border.png");
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
}

/*---------------------------------------------------------------------------------------------*/
// Following function is used to paint display status of process on intro window

void statusIntro1(HDC hdc)
{
	Graphics graphics(hdc);
	graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);
	FontFamily fontFamily(L"Calibri");

	// Draw game status for intro
	if (status() == false) {
		WCHAR status[] = L"Not Running";
		Font fontStatus(&fontFamily, 20, FontStyleBoldItalic, UnitPoint);
		PointF pointFStatus(350.0f, 355.0f);
		// red
		SolidBrush solidBrushStatus(Color(255, 255, 0, 0));

		graphics.DrawString(status, -1, &fontStatus, pointFStatus, NULL, &solidBrushStatus);
	}
	else if (status() == true) {
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

void statusIntro2(HDC hdc)
{
	Graphics graphics(hdc);
	graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);
	FontFamily fontFamily(L"Calibri");

	WCHAR statusFalse[] = L"False Running";
	Font fontStatusFalse(&fontFamily, 20, FontStyleBoldItalic, UnitPoint);
	PointF pointFStatusFalse(350.0f, 355.0f);
	// red
	SolidBrush solidBrushStatusFalse(Color(255, 255, 0, 0));

	// Draw game status for intro
	while (status() == false) {
		Sleep(100);
		graphics.DrawString(statusFalse, -1, &fontStatusFalse, pointFStatusFalse, NULL, &solidBrushStatusFalse);
	}
}

void statusIntro3(HDC hdc)
{
	Graphics graphics(hdc);
	graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);
	FontFamily fontFamily(L"Calibri");

	WCHAR statusTrue[] = L"Running";
	Font fontstatusTrue(&fontFamily, 20, FontStyleBoldItalic, UnitPoint);
	PointF pointFstatusTrue(370.0f, 355.0f);
	// green
	SolidBrush solidBrushstatusTrue(Color(255, 0, 255, 0));

	// Draw game status for intro
	while (status() == true) {
		Sleep(100);
		graphics.DrawString(statusTrue, -1, &fontstatusTrue, pointFstatusTrue, NULL, &solidBrushstatusTrue);
	}
}

void statusIntro4(HDC hdc)
{
	Graphics graphics(hdc);
	graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);
	FontFamily fontFamily(L"Calibri");

	// Draw game status for intro
	while (true) {
		if (status() == false) {
			WCHAR status[] = L"Not Running";
			Font fontStatus(&fontFamily, 20, FontStyleBoldItalic, UnitPoint);
			PointF pointFStatus(350.0f, 355.0f);
			// red
			SolidBrush solidBrushStatus(Color(255, 255, 0, 0));

			graphics.DrawString(status, -1, &fontStatus, pointFStatus, NULL, &solidBrushStatus);
		}
		else if (status() == true) {
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
}

/*---------------------------------------------------------------------------------------------*/
// Following function is used to paint everything on main window

void onPaintMain(HDC hdc)
{
	Graphics graphics(hdc);
	graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);
	FontFamily fontFamily(L"Calibri");

	// Draw background image for main
	Image mainBg(L"Resource Files/Main_orange.png");
	UINT mainBgWidth = mainBg.GetWidth();
	UINT mainBgHeight = mainBg.GetHeight();
	Rect mainBgRect(0, 0, mainBgWidth, mainBgHeight);
	graphics.DrawImage(&mainBg, mainBgRect, 0, 0, mainBgWidth, mainBgHeight, UnitPixel);

	// Draw top bar for settings, minimize and close button for intro
	Pen blackPen(Color(0, 68, 68, 68), 3);
	Rect rect(205 / 2, 0, 645, 25);
	graphics.DrawRectangle(&blackPen, rect);
	/*SolidBrush solidBrush(Color(255, 68, 68, 68));
	graphics.FillRectangle(&solidBrush, 205 / 2, 0, 645, 25);*/

	// Draw orange line for settings, minimize and close button
	Pen orangePen(Color(255, 237, 111, 0), 2);
	Rect line(125, 25, 600, 1);
	graphics.DrawRectangle(&orangePen, line);

	if (hoverSe == true) {
		// Draw settings button for main (active)
		Pen penSe(Color(255, 237, 111, 0), 3);
		Rect rectSe(125, 28, 25, 25);
		graphics.DrawRectangle(&penSe, rectSe);
		SolidBrush solidBrushSe(Color(255, 237, 111, 0));
		graphics.FillRectangle(&solidBrushSe, 125, 28, 25, 25);
	}
	else if (hoverSe == false) {
		// Draw settings button for main (unactive)
		Pen penSe(Color(255, BtnBgInactive, BtnBgInactive, BtnBgInactive), 3);
		Rect rectSe(125, 30, 25, 25);
		graphics.DrawRectangle(&penSe, rectSe);
		SolidBrush solidBrushSe(Color(255, BtnBgInactive, BtnBgInactive, BtnBgInactive));
		graphics.FillRectangle(&solidBrushSe, 125, 30, 25, 25);
	}

	// Draw settings icon image for main
	Image introSe(L"Resource Files/Settings_icon.png");
	UINT introSeWidth = introSe.GetWidth();
	UINT introSeHeight = introSe.GetHeight();
	Rect introSeRect(129, 31, introSeWidth, introSeHeight);
	graphics.DrawImage(&introSe, introSeRect, 0, 0, introSeWidth, introSeHeight, UnitPixel);

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

	SolidBrush solidHckBrushClickActiveHoverActive(Color(255, 72, 230, 139));
	SolidBrush solidHckBrushClickActiveHoverInactive(Color(255, 46, 204, 113));

	SolidBrush solidHckBrushClickInactiveHoverActive(Color(255, 255, 102, 86));
	SolidBrush solidHckBrushClickInactiveHoverInactive(Color(255, 231, 76, 60));

	Pen hckPenActive(Color(255, 21, 179, 88), 4);
	Pen hckPenInactive(Color(255, 206, 51, 35), 4);
	Rect rectHck1(132, 312, 241, 3);
	Rect rectHck2(477, 312, 241, 3);
	Rect rectHck3(132, 472, 241, 3);
	Rect rectHck4(477, 472, 241, 3);
	Rect rectHck5(132, 632, 241, 3);
	Rect rectHck6(477, 632, 241, 3);

	Font fontHck(&fontFamily, 20, FontStyleRegular, UnitPoint);
	SolidBrush solidBrushHck(Color(255, 255, 255, 255));
	WCHAR UH[] = L"Godmode";
	WCHAR US[] = L"Unlimited Points";
	WCHAR UA[] = L"Unlimited Ammo";
	WCHAR NC[] = L"No Clip";
	WCHAR RF[] = L"Rapid Fire";
	WCHAR DA[] = L"Disable All";

	PointF pointFUHActive(190.0f, 244.0f);
	PointF pointFUHInactive(190.0f, 240.0f);
	PointF pointFUSActive(500.0f, 244.0f);
	PointF pointFUSInactive(500.0f, 240.0f);
	PointF pointFUAActive(150.0f, 404.0f);
	PointF pointFUAInactive(150.0f, 400.0f);
	PointF pointFNCActive(550.0f, 404.0f);
	PointF pointFNCInactive(550.0f, 400.0f);
	PointF pointFRFActive(190.0f, 564.0f);
	PointF pointFRFInactive(190.0f, 560.0f);
	PointF pointFDAActive(532.0f, 564.0f);
	PointF pointFDAInactive(532.0f, 560.0f);

	// GODMODE
	if (downUH == true) {
		if (clickUH == true) {
			graphics.DrawRectangle(&hckPenActive, rectHck1);
			graphics.FillRectangle(&solidHckBrushClickActiveHoverActive, 130, 206, 245, 110);
		}
		else if (clickUH == false) {
			graphics.DrawRectangle(&hckPenInactive, rectHck1);
			graphics.FillRectangle(&solidHckBrushClickInactiveHoverActive, 130, 206, 245, 110);
		}
		graphics.DrawString(UH, -1, &fontHck, pointFUHActive, NULL, &solidBrushHck);
	}
	else if (downUH == false) {
		if (clickUH == true) {
			if (hoverUH == true) {
				graphics.FillRectangle(&solidHckBrushClickActiveHoverActive, 130, 200, 245, 110);
			}
			else if (hoverUH == false) {
				graphics.FillRectangle(&solidHckBrushClickActiveHoverInactive, 130, 200, 245, 110);
			}
			graphics.DrawRectangle(&hckPenActive, rectHck1);
		}
		else if (clickUH == false) {
			if (hoverUH == true) {
				graphics.FillRectangle(&solidHckBrushClickInactiveHoverActive, 130, 200, 245, 110);
			}
			else if (hoverUH == false) {
				graphics.FillRectangle(&solidHckBrushClickInactiveHoverInactive, 130, 200, 245, 110);
			}
			graphics.DrawRectangle(&hckPenInactive, rectHck1);
		}
		graphics.DrawString(UH, -1, &fontHck, pointFUHInactive, NULL, &solidBrushHck);
	}

	// UNLIMITED POINTS
	if (downUS == true) {
		if (clickUS == true) {
			graphics.DrawRectangle(&hckPenActive, rectHck1);
			graphics.FillRectangle(&solidHckBrushClickActiveHoverActive, 475, 206, 245, 110);
		}
		else if (clickUS == false) {
			graphics.DrawRectangle(&hckPenInactive, rectHck1);
			graphics.FillRectangle(&solidHckBrushClickInactiveHoverActive, 475, 206, 245, 110);
		}
		graphics.DrawString(US, -1, &fontHck, pointFUSActive, NULL, &solidBrushHck);
	}
	else if (downUS == false) {
		if (clickUS == true) {
			if (hoverUS == true) {
				graphics.FillRectangle(&solidHckBrushClickActiveHoverActive, 475, 200, 245, 110);
			}
			else if (hoverUS == false) {
				graphics.FillRectangle(&solidHckBrushClickActiveHoverInactive, 475, 200, 245, 110);
			}
			graphics.DrawRectangle(&hckPenActive, rectHck2);
		}
		else if (clickUS == false) {
			if (hoverUS == true) {
				graphics.FillRectangle(&solidHckBrushClickInactiveHoverActive, 475, 200, 245, 110);
			}
			else if (hoverUS == false) {
				graphics.FillRectangle(&solidHckBrushClickInactiveHoverInactive, 475, 200, 245, 110);
			}
			graphics.DrawRectangle(&hckPenInactive, rectHck2);
		}
		graphics.DrawString(US, -1, &fontHck, pointFUSInactive, NULL, &solidBrushHck);
	}

	// UNLIMITED AMMO
	if (downUA == true) {
		if (clickUA == true) {
			graphics.DrawRectangle(&hckPenActive, rectHck3);
			graphics.FillRectangle(&solidHckBrushClickActiveHoverActive, 130, 366, 245, 110);
		}
		else if (clickUA == false) {
			graphics.DrawRectangle(&hckPenInactive, rectHck3);
			graphics.FillRectangle(&solidHckBrushClickInactiveHoverActive, 130, 366, 245, 110);
		}
		graphics.DrawString(UA, -1, &fontHck, pointFUAActive, NULL, &solidBrushHck);
	}
	else if (downUA == false) {
		if (clickUA == true) {
			if (hoverUA == true) {
				graphics.FillRectangle(&solidHckBrushClickActiveHoverActive, 130, 360, 245, 110);
			}
			else if (hoverUA == false) {
				graphics.FillRectangle(&solidHckBrushClickActiveHoverInactive, 130, 360, 245, 110);
			}
			graphics.DrawRectangle(&hckPenActive, rectHck3);
		}
		else if (clickUA == false) {
			if (hoverUA == true) {
				graphics.FillRectangle(&solidHckBrushClickInactiveHoverActive, 130, 360, 245, 110);
			}
			else if (hoverUA == false) {
				graphics.FillRectangle(&solidHckBrushClickInactiveHoverInactive, 130, 360, 245, 110);
			}
			graphics.DrawRectangle(&hckPenInactive, rectHck3);
		}
		graphics.DrawString(UA, -1, &fontHck, pointFUAInactive, NULL, &solidBrushHck);
	}

	// NO CLIP
	if (downNC == true) {
		if (clickNC == true) {
			graphics.DrawRectangle(&hckPenActive, rectHck4);
			graphics.FillRectangle(&solidHckBrushClickActiveHoverActive, 475, 366, 245, 110);
		}
		else if (clickNC == false) {
			graphics.DrawRectangle(&hckPenInactive, rectHck4);
			graphics.FillRectangle(&solidHckBrushClickInactiveHoverActive, 475, 366, 245, 110);
		}
		graphics.DrawString(NC, -1, &fontHck, pointFNCActive, NULL, &solidBrushHck);
	}
	else if (downNC == false) {
		if (clickNC == true) {
			if (hoverNC == true) {
				graphics.FillRectangle(&solidHckBrushClickActiveHoverActive, 475, 360, 245, 110);
			}
			else if (hoverNC == false) {
				graphics.FillRectangle(&solidHckBrushClickActiveHoverInactive, 475, 360, 245, 110);
			}
			graphics.DrawRectangle(&hckPenActive, rectHck4);
		}
		else if (clickNC == false) {
			if (hoverNC == true) {
				graphics.FillRectangle(&solidHckBrushClickInactiveHoverActive, 475, 360, 245, 110);
			}
			else if (hoverNC == false) {
				graphics.FillRectangle(&solidHckBrushClickInactiveHoverInactive, 475, 360, 245, 110);
			}
			graphics.DrawRectangle(&hckPenInactive, rectHck4);
		}
		graphics.DrawString(NC, -1, &fontHck, pointFNCInactive, NULL, &solidBrushHck);
	}

	// RAPID FIRE
	if (downRF == true) {
		if (clickRF == true) {
			graphics.DrawRectangle(&hckPenActive, rectHck5);
			graphics.FillRectangle(&solidHckBrushClickActiveHoverActive, 130, 526, 245, 110);
		}
		else if (clickRF == false) {
			graphics.DrawRectangle(&hckPenInactive, rectHck5);
			graphics.FillRectangle(&solidHckBrushClickInactiveHoverActive, 130, 526, 245, 110);
		}
		graphics.DrawString(RF, -1, &fontHck, pointFRFActive, NULL, &solidBrushHck);
	}
	else if (downRF == false) {
		if (clickRF == true) {
			if (hoverRF == true) {
				graphics.FillRectangle(&solidHckBrushClickActiveHoverActive, 130, 520, 245, 110);
			}
			else if (hoverRF == false) {
				graphics.FillRectangle(&solidHckBrushClickActiveHoverInactive, 130, 520, 245, 110);
			}
			graphics.DrawRectangle(&hckPenActive, rectHck5);
		}
		else if (clickRF == false) {
			if (hoverRF == true) {
				graphics.FillRectangle(&solidHckBrushClickInactiveHoverActive, 130, 520, 245, 110);
			}
			else if (hoverRF == false) {
				graphics.FillRectangle(&solidHckBrushClickInactiveHoverInactive, 130, 520, 245, 110);
			}
			graphics.DrawRectangle(&hckPenInactive, rectHck5);
		}
		graphics.DrawString(RF, -1, &fontHck, pointFRFInactive, NULL, &solidBrushHck);
	}

	// DISABLE ALL
	if (downDA == true) {
		if (clickDA == true) {
			graphics.DrawRectangle(&hckPenActive, rectHck6);
			graphics.FillRectangle(&solidHckBrushClickActiveHoverActive, 475, 526, 245, 110);
		}
		graphics.DrawString(DA, -1, &fontHck, pointFDAActive, NULL, &solidBrushHck);
	}
	else if (downDA == false) {
		if (clickDA == false) {
			if (hoverDA == true) {
				graphics.FillRectangle(&solidHckBrushClickInactiveHoverActive, 475, 520, 245, 110);
			}
			else if (hoverDA == false) {
				graphics.FillRectangle(&solidHckBrushClickInactiveHoverInactive, 475, 520, 245, 110);
			}
			graphics.DrawRectangle(&hckPenInactive, rectHck6);
		}
		graphics.DrawString(DA, -1, &fontHck, pointFDAInactive, NULL, &solidBrushHck);
	}
}

/*---------------------------------------------------------------------------------------------*/
// Following function is used to paint everything on settings window

void onPaintSettings(HDC hdc)
{
	Graphics graphics(hdc);
	graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);
	FontFamily fontFamily(L"Calibri");

	// Draw background image for main
	Image settingsBg(L"Resource Files/Settings_orange.png");
	UINT settingsBgWidth = settingsBg.GetWidth();
	UINT settingsBgHeight = settingsBg.GetHeight();
	Rect settingsBgRect(0, 0, settingsBgWidth, settingsBgHeight);
	graphics.DrawImage(&settingsBg, settingsBgRect, 0, 0, settingsBgWidth, settingsBgHeight, UnitPixel);
}

/*---------------------------------------------------------------------------------------------*/
// Main function of trainer

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
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

	WNDCLASSEX wcexSettings;
	wcexSettings.cbSize = sizeof(WNDCLASSEX);
	wcexSettings.style = CS_HREDRAW | CS_VREDRAW;
	wcexSettings.lpfnWndProc = WndProcSettings;
	wcexSettings.cbClsExtra = 0;
	wcexSettings.cbWndExtra = 0;
	wcexSettings.hInstance = hInst;
	wcexSettings.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(Icon));
	wcexSettings.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcexSettings.hbrBackground = CreateSolidBrush(RGB(5, 0, 0));
	wcexSettings.lpszMenuName = NULL;
	wcexSettings.lpszClassName = szWindowClassSettings;
	wcexSettings.hIconSm = NULL;

	if (!RegisterClassEx(&wcexIntro) || !RegisterClassEx(&wcexMain) || !RegisterClassEx(&wcexSettings))
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

	HWND hWndSettings = CreateWindow(
		szWindowClassSettings,
		szTitle,
		WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT,
		SETTINGS_WIDTH, SETTINGS_HEIGHT,
		NULL,
		NULL,
		hInst,
		NULL
		);

	SetWindowLong(hWndSettings, GWL_STYLE, WS_VISIBLE);
	SetWindowLong(hWndSettings, GWL_EXSTYLE, GetWindowLong(hWndSettings, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(hWndSettings, RGB(5, 0, 0), 0, LWA_COLORKEY);

	// If windows faile to be created
	if (!hWndIntro && !hWndMain && !hWndSettings)
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
	ShowWindow(hWndMain, SW_HIDE);
	UpdateWindow(hWndMain);
	ShowWindow(hWndSettings, nCmdShow);
	ShowWindow(hWndSettings, SW_HIDE);
	UpdateWindow(hWndSettings);

	// Main message loop
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	GdiplusShutdown(gdiplusToken);
	return (int)msg.wParam;
}

/*---------------------------------------------------------------------------------------------*/
// Callback function to process input on intro window

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

		// Following line used to check mouse cords:
		/*wchar_t d[20];
		wsprintf(d, _T("(%i, %i"), iPosX, iPosY);
		MessageBox(hWnd, d, _T("click"), MB_OK);*/

		break;
	}

	// WM_LBUTTONUP
	case WM_LBUTTONUP:
	{
		int iPosX = GET_X_LPARAM(lParam);
		int iPosY = GET_Y_LPARAM(lParam);

		ReleaseCapture();

		// Display correct cursor for minimize and close button
		if (iPosX > 693 && iPosX < 720 && iPosY > -1 && iPosY < 26) {
			SetCursor(wcex.hCursor);
		}
		if (iPosX > 721 && iPosX < 747 && iPosY > -1 && iPosY < 26) {
			SetCursor(wcex.hCursor);
		}

		break;
	}

	// WM_MOUSEMOVE
	case WM_MOUSEMOVE:
	{
		int iPosX = GET_X_LPARAM(lParam);
		int iPosY = GET_Y_LPARAM(lParam);

		RECT rectMn{ 693, 0, 721, 27 };
		RECT rectCl{ 721, 0, 749, 27 };
		RECT rectEn{ 340, 432, 506, 412 };

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
			}
		}
		else {
			if (initializedMn) {
				initializedMn = false;
				InvalidateRect(hWnd, &rectMn, FALSE);
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
			}
		}
		else {
			if (initializedCl) {
				initializedCl = false;
				InvalidateRect(hWnd, &rectCl, FALSE);
			}
		}

		// Enter trainer: hover
		if (status() == true) {
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
					InvalidateRect(hWnd, &rectEn, FALSE);
				}
			}
			else {
				if (initialized) {
					initialized = false;
					InvalidateRect(hWnd, &rectEn, FALSE);
				}
			}
		}
		break;
	}

	// WM_PAINT
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);

		onPaintIntro(hdc);
		
		statusIntro1(hdc);
		/*thread status1(statusIntro2, hdc);
		status1.detach();
		thread status2(statusIntro3, hdc);
		status2.detach();*/

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
// Callback function to process input on main window

LRESULT CALLBACK WndProcMain(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	WNDCLASSEX wcex;
	wcex.hCursor = LoadCursor(NULL, IDC_HAND);

	HWND hWndSettings = FindWindow(szWindowClassSettings, NULL);

	switch (message)
	{
	// WM_LBUTTONDOWN
	case WM_LBUTTONDOWN:
	{
		int iPosX = GET_X_LPARAM(lParam);
		int iPosY = GET_Y_LPARAM(lParam);

		RECT rectUH{ 130, 200, 375, 340 };
		RECT rectUS{ 475, 200, 720, 340 };
		RECT rectUA{ 130, 360, 375, 480 };
		RECT rectNC{ 475, 360, 720, 480 };
		RECT rectRF{ 130, 520, 375, 640 };
		RECT rectDA{ 475, 520, 720, 640 };

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

		// Show settings window on click
		if (iPosX > 124 && iPosX < 151 && iPosY > 27 && iPosY < 61) {
			hoverSe = false;
			//ShowWindow(hWndSettings, SW_SHOW);
			SetCursor(wcex.hCursor);
			MessageBox(NULL,
				_T("Coming Soon!"),
				_T("Call of Duty Black Ops II Trainer"),
				MB_OK + MB_ICONINFORMATION);
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
				disableUH = true;
				clickUH = false;
				downUH = true;
				SetCursor(wcex.hCursor);
			}
		}
		else if (iPosX > 127 && iPosX < 378 && iPosY > 197 && iPosY < 312) {
			clickUH = true;
			downUH = true;
			SetCursor(wcex.hCursor);
		}
		static bool initializedUH;
		if (clickUH == true) {
			InvalidateRect(hWnd, &rectUH, FALSE);
		}
		else {
			if (!initializedUH) {
				initializedUH = true;
				InvalidateRect(hWnd, &rectUH, FALSE);

			}
		}

		// UNLIMITED POINTS
		if (clickUS == true) {
			if (iPosX > 472 && iPosX < 723 && iPosY > 197 && iPosY < 312) {
				disableUS = true;
				clickUS = false;
				downUS = true;
				SetCursor(wcex.hCursor);
				InvalidateRect(hWnd, &rectUS, FALSE);
			}
		}
		else if (iPosX > 472 && iPosX < 723 && iPosY > 197 && iPosY < 312) {
			clickUS = true;
			downUS = true;
			SetCursor(wcex.hCursor);
			InvalidateRect(hWnd, &rectUS, FALSE);
		}
		static bool initializedUS;
		if (clickUS == true) {
			InvalidateRect(hWnd, &rectUS, FALSE);
		}
		else {
			if (!initializedUS) {
				initializedUS = true;
				InvalidateRect(hWnd, &rectUS, FALSE);

			}
		}

		// UNLIMITED AMMO
		if (clickUA == true) {
			if (iPosX > 127 && iPosX < 378 && iPosY > 357 && iPosY < 472) {
				disableUA = true;
				clickUA = false;
				downUA = true;
				SetCursor(wcex.hCursor);
			}
		}
		else if (iPosX > 127 && iPosX < 378 && iPosY > 357 && iPosY < 472) {
			clickUA = true;
			downUA = true;
			SetCursor(wcex.hCursor);
		}
		static bool initializedUA;
		if (clickUA == true) {
			InvalidateRect(hWnd, &rectUA, FALSE);
		}
		else {
			if (!initializedUA) {
				initializedUA = true;
				InvalidateRect(hWnd, &rectUA, FALSE);

			}
		}

		// NO CLIP
		if (clickNC == true) {
			if (iPosX > 472 && iPosX < 723 && iPosY > 357 && iPosY < 472) {
				disableNC = true;
				clickNC = false;
				downNC = true;
				SetCursor(wcex.hCursor);
			}
		}
		else if (iPosX > 472 && iPosX < 723 && iPosY > 357 && iPosY < 472) {
			clickNC = true;
			downNC = true;
			SetCursor(wcex.hCursor);
		}
		static bool initializedNC;
		if (clickNC == true) {
			InvalidateRect(hWnd, &rectNC, FALSE);
		}
		else {
			if (!initializedNC) {
				initializedNC = true;
				InvalidateRect(hWnd, &rectNC, FALSE);

			}
		}

		// RAPID FIRE
		if (clickRF == true) {
			if (iPosX > 127 && iPosX < 378 && iPosY > 517 && iPosY < 632) {
				disableRF = true;
				clickRF = false;
				downRF = true;
				SetCursor(wcex.hCursor);
			}
		}
		else if (iPosX > 127 && iPosX < 378 && iPosY > 517 && iPosY < 632) {
			clickRF = true;
			downRF = true;
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

		// DISABLE ALL
		if (iPosX > 472 && iPosX < 723 && iPosY > 517 && iPosY < 632) {
			disableUH = true;
			disableUS = true;
			disableUA = true;
			disableNC = true;
			disableRF = true;

			clickDA = true;
			downDA = true;
			SetCursor(wcex.hCursor);
			InvalidateRect(hWnd, &rectDA, FALSE);
		}

		break;
	}

	// WM_LBUTTONUP
	case WM_LBUTTONUP:
	{
		int iPosX = GET_X_LPARAM(lParam);
		int iPosY = GET_Y_LPARAM(lParam);

		RECT rectUH{ 130, 200, 375, 340 };
		RECT rectUS{ 475, 200, 720, 340 };
		RECT rectUA{ 130, 360, 375, 480 };
		RECT rectNC{ 475, 360, 720, 480 };
		RECT rectRF{ 130, 520, 375, 640 };
		RECT rectDA{ 475, 520, 720, 640 };

		ReleaseCapture();

		// Display correct cursor for settings, minimize and close button
		if (iPosX > 124 && iPosX < 151 && iPosY > 27 && iPosY < 61) {
			SetCursor(wcex.hCursor);
		}
		if (iPosX > 669 && iPosX < 696 && iPosY > 27 && iPosY < 61) {
			SetCursor(wcex.hCursor);
		}
		if (iPosX > 697 && iPosX < 724 && iPosY > 27 && iPosY < 61) {
			SetCursor(wcex.hCursor);
		}

		// UNLIMITED HEALTH
		if (downUH == true) {
			downUH = false;
			InvalidateRect(hWnd, &rectUH, FALSE);

			if (clickUH == true) {
				ReadProcessMemory(hProcHck, (LPCVOID)(pointerUH), &oldValueUH, 4, NULL);
				WCHAR msg[50];
				swprintf_s(msg, L"%d was successfully changed to %d", oldValueUH, valueHealth);

				thread tUnlimitedHealth(UnlimitedHealth);
				tUnlimitedHealth.detach();

				int success = WriteProcessMemory(hProcHck, (LPVOID)pointerUH, &valueHealth, (DWORD)sizeof(valueHealth), NULL);
				// Following line used for error testing:
				// int success = WriteProcessMemory(hWndBO2, (LPVOID)pointerUH, &valueHealth, (DWORD)sizeof(valueHealth), NULL);
				if (success > 0) {
					clickUH = true;
					InvalidateRect(hWnd, &rectUH, FALSE);
					if (advanced == true) {
						MessageBox(NULL,
							msg,
							_T("Call of Duty Black Ops II Trainer"),
							MB_OK + MB_ICONINFORMATION);
					}
				}
				else {
					clickUH = false;
					InvalidateRect(hWnd, &rectUH, FALSE);
					if (advanced == true) {
						MessageBox(NULL,
							_T("Unlimited Health hack failed!"),
							_T("Call of Duty Black Ops II Trainer"),
							MB_OK + MB_ICONERROR);
					}
				}

				CloseHandle(hProcHck);
			}
			else {
				WCHAR msg[50];
				swprintf_s(msg, L"%d was successfully changed back to %d", valuePoints, oldValueUH);

				int success = WriteProcessMemory(hProcHck, (LPVOID)pointerUH, &oldValueUH, (DWORD)sizeof(oldValueUH), NULL);
				// Following line used for error testing:
				// int success = WriteProcessMemory(hWndBO2, (LPVOID)pointerUH, &oldValueUH, (DWORD)sizeof(oldValueUH), NULL);
				if (success > 0) {
					clickUH = false;
					if (advanced == true) {
						MessageBox(NULL,
							msg,
							_T("Call of Duty Black Ops II Trainer"),
							MB_OK + MB_ICONINFORMATION);
					}
				}
				else {
					clickUH = true;
					if (advanced == true) {
						MessageBox(NULL,
							_T("Unlimited Health hack failed!"),
							_T("Call of Duty Black Ops II Trainer"),
							MB_OK + MB_ICONERROR);
					}
				}

				CloseHandle(hProcHck);
			}
		}

		// UNLIMITED POINTS
		if (downUS == true) {
			downUS = false;
			InvalidateRect(hWnd, &rectUS, FALSE);

			if (clickUS == true) {
				ReadProcessMemory(hProcHck, (LPCVOID)(pointerUS), &oldValueUS, 4, NULL);
				WCHAR msg[50];
				swprintf_s(msg, L"%d was successfully changed to %d", oldValueUS, valuePoints);

				thread tUnlimitedPoints(UnlimitedPoints);
				tUnlimitedPoints.detach();

				int success = WriteProcessMemory(hProcHck, (LPVOID)pointerUS, &valuePoints, (DWORD)sizeof(valuePoints), NULL);

				// Following line used for error testing:
				// int success = WriteProcessMemory(hWndBO2, (LPVOID)pointerUS, &valuePoints, (DWORD)sizeof(valuePoints), NULL);
				if (success > 0) {
					clickUS = true;
					InvalidateRect(hWnd, &rectUS, FALSE);
					if (advanced == true) {
						MessageBox(NULL,
							msg,
							_T("Call of Duty Black Ops II Trainer"),
							MB_OK + MB_ICONINFORMATION);
					}
				}
				else {
					clickUS = false;
					InvalidateRect(hWnd, &rectUS, FALSE);
					if (advanced == true) {
						MessageBox(NULL,
							_T("Unlimited Points hack failed!"),
							_T("Call of Duty Black Ops II Trainer"),
							MB_OK + MB_ICONERROR);
					}
				}

				CloseHandle(hProcHck);
			}
			else {
				WCHAR msg[50];
				swprintf_s(msg, L"%d was successfully changed back to %d", valuePoints, oldValueUS);

				int success = WriteProcessMemory(hProcHck, (LPVOID)pointerUS, &oldValueUS, (DWORD)sizeof(oldValueUS), NULL);
				// Following line used for error testing:
				// int success = WriteProcessMemory(hWndBO2, (LPVOID)pointerUS, &oldValueUS, (DWORD)sizeof(oldValueUS), NULL);
				if (success > 0) {
					clickUS = false;
					if (advanced == true) {
						MessageBox(NULL,
							msg,
							_T("Call of Duty Black Ops II Trainer"),
							MB_OK + MB_ICONINFORMATION);
					}
				}
				else {
					clickUS = true;
					if (advanced == true) {
						MessageBox(NULL,
							_T("Unlimited Points hack failed!"),
							_T("Call of Duty Black Ops II Trainer"),
							MB_OK + MB_ICONERROR);
					}
				}

				CloseHandle(hProcHck);
			}
		}

		// UNLIMITED AMMO
		if (downUA == true) {
			downUA = false;
			InvalidateRect(hWnd, &rectUA, FALSE);

			if (clickUA == true) {
				ReadProcessMemory(hProcHck, (LPCVOID)(pointerUA1), &oldValueUA1, 4, NULL);
				ReadProcessMemory(hProcHck, (LPCVOID)(pointerUA2), &oldValueUA2, 4, NULL);
				ReadProcessMemory(hProcHck, (LPCVOID)(pointerUA3), &oldValueUA3, 4, NULL);
				WCHAR msg[175];
				swprintf_s(msg, L"%d, %d and %d were successfully changed to %d, %d and %d", oldValueUA1, oldValueUA2, oldValueUA3, valueAmmo1, valueAmmo1, valueAmmo2);

				thread tUnlimitedAmmo(UnlimitedAmmo);
				tUnlimitedAmmo.detach();

				int success1 = WriteProcessMemory(hProcHck, (LPVOID)pointerUA1, &valueAmmo1, (DWORD)sizeof(valueAmmo1), NULL);
				int success2 = WriteProcessMemory(hProcHck, (LPVOID)pointerUA2, &valueAmmo1, (DWORD)sizeof(valueAmmo1), NULL);
				int success3 = WriteProcessMemory(hProcHck, (LPVOID)pointerUA3, &valueAmmo2, (DWORD)sizeof(valueAmmo2), NULL);
				// Following lines used for error testing:
				// int success1 = WriteProcessMemory(hWndBO2, (LPVOID)pointerUA1, &valueAmmo1, (DWORD)sizeof(valueAmmo1), NULL);
				// int success2 = WriteProcessMemory(hWndBO2, (LPVOID)pointerUA2, &valueAmmo1, (DWORD)sizeof(valueAmmo1), NULL);
				// int success3 = WriteProcessMemory(hWndBO2, (LPVOID)pointerUA3, &valueAmmo2, (DWORD)sizeof(valueAmmo2), NULL);
				if (success1 > 0 && success2 > 0 && success3 > 0) {
					clickUA = true;
					InvalidateRect(hWnd, &rectUA, FALSE);
					if (advanced == true) {
						MessageBox(NULL,
							msg,
							_T("Call of Duty Black Ops II Trainer"),
							MB_OK + MB_ICONINFORMATION);
					}
				}
				else {
					clickUA = false;
					InvalidateRect(hWnd, &rectUA, FALSE);
					if (advanced == true) {
						MessageBox(NULL,
							_T("Unlimited Ammo hack failed!"),
							_T("Call of Duty Black Ops II Trainer"),
							MB_OK + MB_ICONERROR);
					}
				}

				CloseHandle(hProcHck);
			}
			else {
				WCHAR msg[175];
				swprintf_s(msg, L"%d, %d and %d were successfully changed back to %d, %d and %d", valueAmmo1, valueAmmo1, valueAmmo2, oldValueUA1, oldValueUA2, oldValueUA3);

				int success1 = WriteProcessMemory(hProcHck, (LPVOID)pointerUA1, &oldValueUA1, (DWORD)sizeof(oldValueUA1), NULL);
				int success2 = WriteProcessMemory(hProcHck, (LPVOID)pointerUA2, &oldValueUA2, (DWORD)sizeof(oldValueUA2), NULL);
				int success3 = WriteProcessMemory(hProcHck, (LPVOID)pointerUA3, &oldValueUA3, (DWORD)sizeof(oldValueUA3), NULL);
				// Following lines used for error testing:
				// int success1 = WriteProcessMemory(hWndBO2, (LPVOID)pointerUA1, &oldValueUA1, (DWORD)sizeof(oldValueUA1), NULL);
				// int success2 = WriteProcessMemory(hWndBO2, (LPVOID)pointerUA2, &oldValueUA2, (DWORD)sizeof(oldValueUA2), NULL);
				// int success3 = WriteProcessMemory(hWndBO2, (LPVOID)pointerUA3, &oldValueUA3, (DWORD)sizeof(oldValueUA3), NULL);
				if (success1 > 0 && success2 > 0 && success3 > 0) {
					clickUA = false;
					if (advanced == true) {
						MessageBox(NULL,
							msg,
							_T("Call of Duty Black Ops II Trainer"),
							MB_OK + MB_ICONINFORMATION);
					}
				}
				else {
					clickUA = true;
					if (advanced == true) {
						MessageBox(NULL,
							_T("Unlimited Ammo hack failed!"),
							_T("Call of Duty Black Ops II Trainer"),
							MB_OK + MB_ICONERROR);
					}
				}

				CloseHandle(hProcHck);
			}
		}

		// NO CLIP
		if (downNC == true) {
			downNC = false;
			InvalidateRect(hWnd, &rectNC, FALSE);

			if (clickNC == true) {
				ReadProcessMemory(hProcHck, (LPCVOID)(pointerNC1), &oldValueNC1, 4, NULL);
				ReadProcessMemory(hProcHck, (LPCVOID)(pointerNC2), &oldValueNC2, 4, NULL);
				WCHAR msg[125];
				swprintf_s(msg, L"%d and %d were successfully changed to %d and %d", oldValueNC1, oldValueNC2, valueAmmo1, valueAmmo1);

				thread tNoClip(NoClip);
				tNoClip.detach();

				int success1 = WriteProcessMemory(hProcHck, (LPVOID)pointerNC1, &valueAmmo1, (DWORD)sizeof(valueAmmo1), NULL);
				int success2 = WriteProcessMemory(hProcHck, (LPVOID)pointerNC2, &valueAmmo1, (DWORD)sizeof(valueAmmo1), NULL);
				// Following lines used for error testing:
				// int success1 = WriteProcessMemory(hWndBO2, (LPVOID)pointerNC1, &valueAmmo1, (DWORD)sizeof(valueAmmo1), NULL);
				// int success2 = WriteProcessMemory(hWndBO2, (LPVOID)pointerNC2, &valueAmmo1, (DWORD)sizeof(valueAmmo1), NULL);
				if (success1 > 0 && success2 > 0) {
					clickNC = true;
					InvalidateRect(hWnd, &rectNC, FALSE);
					if (advanced == true) {
						MessageBox(NULL,
							msg,
							_T("Call of Duty Black Ops II Trainer"),
							MB_OK + MB_ICONINFORMATION);
					}
				}
				else {
					clickNC = false;
					InvalidateRect(hWnd, &rectNC, FALSE);
					if (advanced == true) {
						MessageBox(NULL,
							_T("No Clip hack failed!"),
							_T("Call of Duty Black Ops II Trainer"),
							MB_OK + MB_ICONERROR);
					}
				}

				CloseHandle(hProcHck);
			}
			else {
				WCHAR msg[125];
				swprintf_s(msg, L"%d and %d were successfully changed back to %d and %d", valueAmmo1, valueAmmo1, oldValueNC1, oldValueNC2);

				int success1 = WriteProcessMemory(hProcHck, (LPVOID)pointerNC1, &oldValueNC1, (DWORD)sizeof(oldValueNC1), NULL);
				int success2 = WriteProcessMemory(hProcHck, (LPVOID)pointerNC2, &oldValueNC2, (DWORD)sizeof(oldValueNC2), NULL);
				// Following lines used for error testing:
				// int success1 = WriteProcessMemory(hWndBO2, (LPVOID)pointerNC1, &oldValueNC1, (DWORD)sizeof(oldValueNC1), NULL);
				// int success2 = WriteProcessMemory(hWndBO2, (LPVOID)pointerNC2, &oldValueNC2, (DWORD)sizeof(oldValueNC2), NULL);
				if (success1 > 0 && success2 > 0) {
					clickNC = false;
					if (advanced == true) {
						MessageBox(NULL,
							msg,
							_T("Call of Duty Black Ops II Trainer"),
							MB_OK + MB_ICONINFORMATION);
					}
				}
				else {
					clickNC = true;
					if (advanced == true) {
						MessageBox(NULL,
							_T("No Clip hack failed!"),
							_T("Call of Duty Black Ops II Trainer"),
							MB_OK + MB_ICONERROR);
					}
				}

				CloseHandle(hProcHck);
			}
		}

		// RAPID FIRE
		if (downRF == true) {
			downRF = false;
			InvalidateRect(hWnd, &rectRF, FALSE);

			if (clickRF == true) {
				ReadProcessMemory(hProcHck, (LPCVOID)(pointerRF), &oldValueRF, 4, NULL);
				WCHAR msg[50];
				swprintf_s(msg, L"%d was successfully changed to %d", oldValueRF, valueRapidFire);

				thread tRapidFire(RapidFire);
				tRapidFire.detach();

				int success = WriteProcessMemory(hProcHck, (LPVOID)pointerRF, &valueRapidFire, (DWORD)sizeof(valueRapidFire), NULL);
				// Following line used for error testing:
				// int success = WriteProcessMemory(hWndBO2, (LPVOID)pointerRF, &valueRapidFire, (DWORD)sizeof(valueRapidFire), NULL);
				if (success > 0) {
					clickRF = true;
					InvalidateRect(hWnd, &rectRF, FALSE);
					if (advanced == true) {
						MessageBox(NULL,
							msg,
							_T("Call of Duty Black Ops II Trainer"),
							MB_OK + MB_ICONINFORMATION);
					}
				}
				else {
					clickRF = false;
					InvalidateRect(hWnd, &rectRF, FALSE);
					if (advanced == true) {
						MessageBox(NULL,
							_T("Rapid Fire hack failed!"),
							_T("Call of Duty Black Ops II Trainer"),
							MB_OK + MB_ICONERROR);
					}
				}

				CloseHandle(hProcHck);
			}
			else {
				WCHAR msg[50];
				swprintf_s(msg, L"%d was successfully changed back to %d", valuePoints, oldValueRF);

				int success = WriteProcessMemory(hProcHck, (LPVOID)pointerRF, &oldValueRF, (DWORD)sizeof(oldValueRF), NULL);
				// Following line used for error testing:
				// int success = WriteProcessMemory(hWndBO2, (LPVOID)pointerRF, &oldValueRF, (DWORD)sizeof(oldValueRF), NULL);
				if (success > 0) {
					clickRF = false;
					if (advanced == true) {
						MessageBox(NULL,
							msg,
							_T("Call of Duty Black Ops II Trainer"),
							MB_OK + MB_ICONINFORMATION);
					}
				}
				else {
					clickRF = true;
					if (advanced == true) {
						MessageBox(NULL,
							_T("Rapid Fire hack failed!"),
							_T("Call of Duty Black Ops II Trainer"),
							MB_OK + MB_ICONERROR);
					}
				}

				CloseHandle(hProcHck);
			}
		}

		if (downDA == true) {
			if (clickUH == true || clickUS == true || clickUA == true || clickNC == true || clickRF == true) {
				int successUH = WriteProcessMemory(hProcHck, (LPVOID)pointerUH, &oldValueUH, (DWORD)sizeof(oldValueUH), NULL);
				int successUS = WriteProcessMemory(hProcHck, (LPVOID)pointerUS, &oldValueUS, (DWORD)sizeof(oldValueUS), NULL);
				int successUA1 = WriteProcessMemory(hProcHck, (LPVOID)pointerUA1, &oldValueUA1, (DWORD)sizeof(oldValueUA1), NULL);
				int successUA2 = WriteProcessMemory(hProcHck, (LPVOID)pointerUA2, &oldValueUA2, (DWORD)sizeof(oldValueUA2), NULL);
				int successUA3 = WriteProcessMemory(hProcHck, (LPVOID)pointerUA3, &oldValueUA3, (DWORD)sizeof(oldValueUA3), NULL);
				int successNC1 = WriteProcessMemory(hProcHck, (LPVOID)pointerNC1, &oldValueNC1, (DWORD)sizeof(oldValueNC1), NULL);
				int successNC2 = WriteProcessMemory(hProcHck, (LPVOID)pointerNC2, &oldValueNC2, (DWORD)sizeof(oldValueNC2), NULL);
				int successRF = WriteProcessMemory(hProcHck, (LPVOID)pointerRF, &oldValueRF, (DWORD)sizeof(oldValueRF), NULL);

				if (successUH > 0 && successUS > 0 && successUA1 > 0 && successUA1 > 0 && successUA3 > 0 && successNC1 > 0 && successNC2 > 0 && successRF > 0) {
					if (advanced == true) {
						MessageBox(NULL,
							_T("All values were successfully changed back to their previous values!"),
							_T("Call of Duty Black Ops II Trainer"),
							MB_OK + MB_ICONINFORMATION);
					}
				}
				else {
					if (advanced == true) {
						MessageBox(NULL,
							_T("Disable All failed!"),
							_T("Call of Duty Black Ops II Trainer"),
							MB_OK + MB_ICONERROR);
					}
				}

				CloseHandle(hProcHck);
			}
			else {
				if (advanced == true) {
					MessageBox(NULL,
						_T("No hacks are enabled!"),
						_T("Call of Duty Black Ops II Trainer"),
						MB_OK + MB_ICONERROR);
				}
			}

			downDA = false;
			InvalidateRect(hWnd, &rectDA, FALSE);
		}

		// GODMODE
		if (iPosX > 127 && iPosX < 378 && iPosY > 197 && iPosY < 312) {
			SetCursor(wcex.hCursor);
		}

		// UNLIMITED POINTS
		if (iPosX > 472 && iPosX < 723 && iPosY > 197 && iPosY < 312) {
			SetCursor(wcex.hCursor);
		}

		// UNLIMITED AMMO
		if (iPosX > 127 && iPosX < 378 && iPosY > 357 && iPosY < 472) {
			SetCursor(wcex.hCursor);
		}

		// NO CLIP
		if (iPosX > 472 && iPosX < 723 && iPosY > 357 && iPosY < 472) {
			SetCursor(wcex.hCursor);
		}

		// RAPID FIRE
		if (iPosX > 127 && iPosX < 378 && iPosY > 517 && iPosY < 632) {
			SetCursor(wcex.hCursor);
		}

		// DISABLE ALL
		if (iPosX > 472 && iPosX < 723 && iPosY > 517 && iPosY < 632) {
			SetCursor(wcex.hCursor);
		}

		if (clickDA == true) {
			if (clickUH == true) {
				clickUH = false;
				InvalidateRect(hWnd, &rectUH, FALSE);
			}
			if (clickUS == true) {
				clickUS = false;
				InvalidateRect(hWnd, &rectUS, FALSE);
			}
			if (clickUA == true) {
				clickUA = false;
				InvalidateRect(hWnd, &rectUA, FALSE);
			}
			if (clickNC == true) {
				clickNC = false;
				InvalidateRect(hWnd, &rectNC, FALSE);
			}
			if (clickRF == true) {
				clickRF = false;
				InvalidateRect(hWnd, &rectRF, FALSE);
			}
			if (clickDA == true) {
				clickDA = false;
				InvalidateRect(hWnd, &rectDA, FALSE);
			}
		}

		break;
	}

	// WM_MOUSEMOVE
	case WM_MOUSEMOVE:
	{
		int iPosX = GET_X_LPARAM(lParam);
		int iPosY = GET_Y_LPARAM(lParam);

		RECT rectSe{ 124, 27, 152, 62 };
		RECT rectMn{ 669, 27, 697, 62 };
		RECT rectCl{ 697, 27, 725, 62 };
		RECT rectUH{ 130, 200, 375, 330 };
		RECT rectUS{ 475, 200, 720, 330 };
		RECT rectUA{ 130, 360, 375, 470 };
		RECT rectNC{ 475, 360, 720, 470 };
		RECT rectRF{ 130, 520, 375, 630 };
		RECT rectDA{ 475, 520, 720, 630 };

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

		// Settings button: hover
		if (iPosX > 124 && iPosX < 151 && iPosY > 27 && iPosY < 61) {
			hoverSe = true;
			SetCursor(wcex.hCursor);
		}
		else {
			hoverSe = false;
		}
		static bool initializedSe;
		if (hoverSe == true) {
			if (!initializedSe) {
				initializedSe = true;
				InvalidateRect(hWnd, &rectSe, FALSE);
				//RedrawWindow(hWnd, &rectSe, NULL, RDW_INVALIDATE);
			}
		}
		else {
			if (initializedSe) {
				initializedSe = false;
				InvalidateRect(hWnd, &rectSe, FALSE);
				//RedrawWindow(hWnd, &rectSe, NULL, RDW_INVALIDATE);
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

		// NO CLIP
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

		// DISABLE ALL
		if (iPosX > 472 && iPosX < 723 && iPosY > 517 && iPosY < 632) {
			hoverDA = true;
			SetCursor(wcex.hCursor);
		}
		else {
			hoverDA = false;
		}
		static bool initializedDA;
		if (hoverDA == true) {
			if (!initializedDA) {
				initializedDA = true;
				InvalidateRect(hWnd, &rectDA, FALSE);
			}
		}
		else {
			if (initializedDA) {
				initializedDA = false;
				InvalidateRect(hWnd, &rectDA, FALSE);
			}
		}
		break;
	}

	// WM_PAINT
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);

		HWND hCheck = FindWindow(0, L"REDACTED: Nightly [2014-06-25] - #OFFLINE MODE#");
		if (hCheck == 0) {
			static bool initialized;
			if (!initialized) {
				initialized = true;
				ShowWindow(hWnd, SW_HIDE);
				MessageBox(NULL,
					_T("Call of Duty Black Ops II process could not be found!"),
					_T("Call of Duty Black Ops II Trainer"),
					NULL);
				DestroyWindow(hWnd);
			}
		}
		else {
			static bool initialized;
			if (!initialized) {
				initialized = true;
				GetWindowThreadProcessId(hWndBO2, &procId);
				hProcHck = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);
				if (!hProcHck) {
					ShowWindow(hWnd, SW_HIDE);
					MessageBox(NULL,
						_T("Failed to open process, run the trainer as administrator!"),
						_T("Call of Duty Black Ops II Trainer"),
						NULL);
					DestroyWindow(hWnd);
				}
			}
		}

		if (status() == false) {
			static bool initialized;
			if (!initialized) {
				initialized = true;
				ShowWindow(hWnd, SW_HIDE);
				MessageBox(NULL,
					_T("Call of Duty Black Ops II was shut down!"),
					_T("Call of Duty Black Ops II Trainer"),
					NULL);
				DestroyWindow(hWnd);
			}
		}

		onPaintMain(hdc);

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
// Callback function to process input on settings window

LRESULT CALLBACK WndProcSettings(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

		break;
	}

	// WM_LBUTTONUP
	case WM_LBUTTONUP:
	{
		int iPosX = GET_X_LPARAM(lParam);
		int iPosY = GET_Y_LPARAM(lParam);

		break;
	}

	// WM_MOUSEMOVE
	case WM_MOUSEMOVE:
	{
		int iPosX = GET_X_LPARAM(lParam);
		int iPosY = GET_Y_LPARAM(lParam);

		break;
	}

	// WM_PAINT
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);

		onPaintSettings(hdc);

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