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

#include "Intro.h"

using namespace Gdiplus;
using namespace std;
#pragma comment (lib,"Gdiplus.lib")

/*---------------------------------------------------------------------------------------------*/

// Forward declarations
static TCHAR szWindowClassIntro[] = _T("codbo_trainer1");
static TCHAR szWindowClassMain[] = _T("codbo_trainer2");
static TCHAR szTitle[] = _T("Call of Duty Black Ops II Trainer");

HINSTANCE hInst = NULL;

LRESULT CALLBACK WndProcIntro(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProcMain(HWND, UINT, WPARAM, LPARAM);

const int INTRO_WIDTH = 850;
const int INTRO_HEIGHT = 534;

const int MAIN_WIDTH = 850;
const int MAIN_HEIGHT = 700;

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
		RECT rect{ 340, 432, 506, 412 };
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
		RECT rect{ 340, 432, 506, 412 };
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

void onPaintMain(HDC hdc) {

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
	Pen orangePen(Color(255, 237, 111, 0), 3);
	Rect line(125, 24, 600, 1);
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

	Pen blackrPen(Color(0, 68, 68, 68), 3);
	Rect rectr(125, 0, 600, 100);
	graphics.DrawRectangle(&blackrPen, rectr);
	SolidBrush solidrBrush(Color(255, 68, 68, 68));
	graphics.FillRectangle(&solidrBrush, 130, 200, 590, 400);

	SolidBrush solidrrBrush(Color(255, 255, 68, 68));
	graphics.FillRectangle(&solidrrBrush, 130, 200, 225, 110);
	graphics.FillRectangle(&solidrrBrush, 495, 200, 225, 110);
	graphics.FillRectangle(&solidrrBrush, 130, 360, 225, 110);
	graphics.FillRectangle(&solidrrBrush, 495, 360, 225, 110);
	graphics.FillRectangle(&solidrrBrush, 305, 525, 225, 110);
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
	wcexIntro.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
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
	wcexMain.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
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

	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
		int iPosX = GET_X_LPARAM(lParam);
		int iPosY = GET_Y_LPARAM(lParam);

		// Pass cords when clicked on top bar
		if (iPosX > 99 && iPosX < 693 && iPosY > -1 && iPosY < 26) {
			SetCapture(hWnd);
			GetWindowRect(hWnd, &rcWindow);
			GetCursorPos(&pos);
			ScreenToClient(hWnd, &pos);
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
				//ShowWindow(hWnd, SW_HIDE);
			}
		}

		/*wchar_t d[20];
		wsprintf(d, _T("(%i, %i"), iPosX, iPosY);
		MessageBox(hWnd, d, _T("click"), MB_OK);*/

		break;
	}

	case WM_LBUTTONUP:
	{
		ReleaseCapture();
		break;
	}

	case WM_MOUSEMOVE:
	{
		int iPosX = GET_X_LPARAM(lParam);
		int iPosY = GET_Y_LPARAM(lParam);

		RECT rectMn{ 693, 0, 721, 27 };
		RECT rectCl{ 721, 0, 749, 27 };

		// Move window
		if (iPosX > 100 && iPosX < 693 && iPosY > -1 && iPosY < 26) {
			GetCursorPos(&curPos);
			if (wParam == MK_LBUTTON)
			{
				int x = curPos.x - pos.x;
				int y = curPos.y - pos.y;
				int width = rcWindow.right - rcWindow.left;
				int height = rcWindow.bottom - rcWindow.top;
				MoveWindow(hWnd, x, y, width, height, TRUE);
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

	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);

		onPaintIntro(hdc);
		statusIntro(hdc);

		/*hWnd = GetFocus();
		RECT rect{ 340, 432, 506, 352 };
		if (hWnd = GetFocus()) {
		//InvalidateRect(hWnd, &rect, FALSE);
		RedrawWindow(hWnd, &rect, NULL, RDW_INVALIDATE);
		}
		else {
		RedrawWindow(hWnd, &rect, NULL, RDW_INVALIDATE);
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

/*---------------------------------------------------------------------------------------------*/

LRESULT CALLBACK WndProcMain(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	WNDCLASSEX wcex;
	wcex.hCursor = LoadCursor(NULL, IDC_HAND);

	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
		int iPosX = GET_X_LPARAM(lParam);
		int iPosY = GET_Y_LPARAM(lParam);

		// Pass cords when clicked on top bar
		if (iPosX > 99 && iPosX < 748 && iPosY > -1 && iPosY < 26) {
			SetCapture(hWnd);
			GetWindowRect(hWnd, &rcWindow);
			GetCursorPos(&pos);
			ScreenToClient(hWnd, &pos);
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
		break;
	}

	case WM_LBUTTONUP:
	{
		ReleaseCapture();
		break;
	}

	case WM_MOUSEMOVE:
	{
		int iPosX = GET_X_LPARAM(lParam);
		int iPosY = GET_Y_LPARAM(lParam);

		RECT rectMn{ 669, 27, 697, 62 };
		RECT rectCl{ 697, 27, 725, 62 };

		// Move window
		if (iPosX > 100 && iPosX < 748 && iPosY > -1 && iPosY < 26) {
			GetCursorPos(&curPos);
			if (wParam == MK_LBUTTON)
			{
				int x = curPos.x - pos.x;
				int y = curPos.y - pos.y;
				int width = rcWindow.right - rcWindow.left;
				int height = rcWindow.bottom - rcWindow.top;
				MoveWindow(hWnd, x, y, width, height, TRUE);
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
		break;
	}

	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);

		onPaintMain(hdc);

		/*hWndIntro = GetFocus();
		RECT rect{ 340, 432, 506, 352 };
		if (hWndIntro = GetFocus()) {
		//InvalidateRect(hWndIntro, &rect, FALSE);
		RedrawWindow(hWndIntro, &rect, NULL, RDW_INVALIDATE);
		}
		else {
		RedrawWindow(hWndIntro, &rect, NULL, RDW_INVALIDATE);
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