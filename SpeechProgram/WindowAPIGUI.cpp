#pragma warning(disable:4996)
#include <WinSock2.h>
#include <windows.h>
#include <stdio.h>
#include "resource.h"
#include "TCPClient.cpp"

using namespace std;

BOOL CALLBACK Dlg_Proc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

static HINSTANCE hInst;
static HANDLE hReadEvent, hWriteEvent;

//LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow) {
	hReadEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	if (!hReadEvent) return 1;
	hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (!hWriteEvent) return 1;
	CreateThread(NULL, 0, ClientMain, NULL, 0, NULL);

	hInst = hInstance;
	DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, Dlg_Proc);

	CloseHandle(hReadEvent);
	CloseHandle(hWriteEvent);

	WSACleanup();
	return 0;
}

BOOL CALLBACK Dlg_Proc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) {
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

//LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
//
//	switch (iMsg) {
//	case WM_CREATE:
//		break;
//	case WM_DESTROY:
//		PostQuitMessage(0);
//		break;
//	}
//	return DefWindowProc(hwnd, iMsg, wParam, lParam);
//}
