#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include "resource.h"

using namespace std;

BOOL CALLBACK Dlg_Proc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

HINSTANCE hInst;

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow) {
	HWND hwnd;
	MSG msg;
	WNDCLASS WndClass;
	hInst = hInstance;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_QUESTION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = _T("Window Class Name");
	RegisterClass(&WndClass);
	hwnd = CreateWindow(_T("Window Class Name"), _T("202113018±èÇö¿í"),
		WS_OVERLAPPEDWINDOW, 200,
		300, 819,
		614, NULL,
		NULL, hInstance, NULL);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

BOOL CALLBACK Dlg_Proc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	static TCHAR num[20];
	static double number1 = -12345, number2 = -12345;
	double result;
	static int count = 0;
	static enum State {
		NULLSTATE,
		PLUS,
		MINUS,
		MULTI,
		DIVIDE
	};
	static State curState = NULLSTATE;

	switch (iMsg)
	{
	case WM_INITDIALOG:
		return 1;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDCANCEL:
			EndDialog(hDlg, 0);
			break;
		}
		break;
	}
	return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {

	switch (iMsg) {
	case WM_CREATE:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, Dlg_Proc);
		PostQuitMessage(0);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
