// codbo2_trainer: Main.cpp
// Gijs de Jong

// Includes
#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <objidl.h>
#include <gdiplus.h>
#include "FindProcess.h"

using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

// Forward declarations
static TCHAR szWindowClass[] = _T("codbo_trainer");
static TCHAR szTitle[] = _T("Call of Duty Black Ops II Trainer");

HINSTANCE hInst;

const int INTRO_WIDTH = 850;
const int INTRO_HEIGHT = 534;

const int MAIN_WIDTH = 645;
const int MAIN_HEIGHT = 534;

bool hoverEnter = false;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void OnPaint(HDC hdc)
{
	Graphics graphics(hdc);
	graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);

	// Draw background image for intro
	Image introBg(L"Intro_border.png");
	UINT introBgWidth = introBg.GetWidth();
	UINT introBgHeight = introBg.GetHeight();
	Rect introBgRect(0, 0, introBgWidth, introBgHeight);
	graphics.DrawImage(&introBg, introBgRect, 0, 0, introBgWidth, introBgHeight, UnitPixel);

	// Draw close button for intro
	Image introCl(L"Close.png");
	UINT introClWidth = introCl.GetWidth();
	UINT introClHeight = introCl.GetHeight();
	Rect introClRect(0, 0, introClWidth, introClHeight);
	graphics.DrawImage(&introCl, introClRect, 0, 0, introClWidth, introClHeight, UnitPixel);

	// Draw close and minimaze bar for intro
	Pen blackPen(Color(255, 0, 0, 0), 3);
	Rect rect(205 / 2, 0, 645, 35);
	graphics.DrawRectangle(&blackPen, rect);
	SolidBrush solidBrush(Color(255, 255, 0, 0));
	graphics.FillRectangle(&solidBrush, 205 / 2, 0, 645, 35);
}

void status(HDC hdc)
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
	else if(status() == false) {
		WCHAR status[] = L"Running";
		Font fontStatus(&fontFamily, 20, FontStyleBoldItalic, UnitPoint);
		PointF pointFStatus(370.0f, 355.0f);
		// green
		SolidBrush solidBrushStatus(Color(255, 0, 255, 0));

		graphics.DrawString(status, -1, &fontStatus, pointFStatus, NULL, &solidBrushStatus);
	}

	if (hoverEnter == true) {
		WCHAR enter[] = L"Enter trainer";
		Font fontEnter(&fontFamily, 20, FontStyleBoldItalic, UnitPoint);
		PointF pointFEnter(348.0f, 405.0f);
		// #999
		SolidBrush solidBrushEnter(Color(255, 153, 153, 153));

		graphics.DrawString(enter, -1, &fontEnter, pointFEnter, NULL, &solidBrushEnter);
	}
	else {
		WCHAR enter[] = L"Enter trainer";
		Font fontEnter(&fontFamily, 20, FontStyleBold, UnitPoint);
		PointF pointFEnter(348.0f, 405.0f);
		// #999
		SolidBrush solidBrushEnter(Color(255, 153, 153, 153));

		graphics.DrawString(enter, -1, &fontEnter, pointFEnter, NULL, &solidBrushEnter);
	}
}

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{

	// Initialize GDI+.
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = CreateSolidBrush(RGB(5, 0, 0));
	//wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Call of Duty Black Ops II Trainer"),
			NULL);

		return 1;
	}

	hInst = hInstance;

	HWND hWnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_VISIBLE,
		//WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		INTRO_WIDTH, INTRO_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL
		);

	SetWindowLong(hWnd, GWL_STYLE, WS_VISIBLE);
	SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(hWnd, RGB(5, 0, 0), 0, LWA_COLORKEY);

	HWND hTrainer = CreateWindow(
		szWindowClass,
		szTitle,
		WS_VISIBLE | WS_CHILD,
		CW_USEDEFAULT, CW_USEDEFAULT,
		MAIN_WIDTH, MAIN_HEIGHT,
		hWnd,
		NULL,
		hInstance,
		NULL
		);

	SetWindowLong(hTrainer, GWL_STYLE, WS_VISIBLE);
	SetWindowLong(hTrainer, GWL_EXSTYLE, GetWindowLong(hTrainer, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(hTrainer, RGB(0, 0, 0), 0, LWA_COLORKEY);

	// If one of windows failes to be created
	if (!hWnd || !hTrainer)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Call of Duty Black Ops II Trainer"),
			NULL);

		return 1;
	}

	// Show intro windows
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	ShowWindow(hTrainer, nCmdShow);
	UpdateWindow(hTrainer);

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR test[] = _T("testtesttest");
	HWND hTrainer;

	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
		int iPosX = GET_X_LPARAM(lParam);
		int iPosY = GET_Y_LPARAM(lParam);
		if (iPosX > 348 && iPosX < 502 && iPosY > 412 && iPosY < 432) {
			//ShowWindow(hWnd, SW_HIDE);
			//ShowWindow(hWnd, SW_MINIMIZE);
			//ShowWindow(hTrainer, SW_SHOW);
			hoverEnter = true;
		}
		/*wchar_t d[20];
		wsprintf(d, _T("(%i, %i"), iPosX, iPosY);
		MessageBox(hWnd, d, _T("click"), MB_OK);*/
		break;
	}

	case WM_MOUSEMOVE:
	{
		int iPosX = GET_X_LPARAM(lParam);
		int iPosY = GET_Y_LPARAM(lParam);
		if (iPosX > 340 && iPosX < 510 && iPosY > 350 && iPosY < 390) {
			//graphics.status = 
		}
		/*wchar_t d[20];
		wsprintf(d, _T("(%i, %i"), iPosX, iPosY);
		MessageBox(hWnd, d, _T("click"), MB_OK);*/
		break;
	}

	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);

		OnPaint(hdc);
		status(hdc);

		/*TextOut(hdc,
		5, 5,
		test, _tcslen(test));*/

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