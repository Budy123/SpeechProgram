//#pragma warning(disable:4996)
//#define _WINSOCK_DEPRECATED_NO_WARNINGS
//#pragma comment(lib, "ws2_32")
//#include <winsock2.h>
//#include <stdlib.h>
//#include <stdio.h>
//#include <windows.h>
//#include <winbase.h>
//#include "resource.h"
//
//using namespace std;
//
//#define SERVERIP   "127.0.0.1"
//#define SERVERPORT 9000
//#define BUFSIZE    512
//
//BOOL CALLBACK Dlg_Proc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
//DWORD WINAPI ClientMain(LPVOID arg);
//void err_quit(const char* msg);
//void err_display(const char* msg);
//int recvn(SOCKET s, char* buf, int len, int flags);
//DWORD WINAPI ReadThread(LPVOID arg);
//DWORD WINAPI WriteThread(LPVOID arg);
//void DisplayText(char* fmt, ...);
//
//static HWND hEditStatus;
//static HINSTANCE hInst;
//static HANDLE hReadEvent, hWriteEvent;
//static HANDLE hClientThread;
//static SOCKET sock;
//static char chatmsg[256];
//
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow) {
//	WSADATA wsa;
//	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;
//
//	hReadEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
//	if (!hReadEvent) return 1;
//	hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
//	if (!hWriteEvent) return 1;
//
//	hInst = hInstance;
//	DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, Dlg_Proc);
//
//
//	CloseHandle(hReadEvent);
//	CloseHandle(hWriteEvent);
//
//	WSACleanup();
//	return 0;
//}
//
//BOOL CALLBACK Dlg_Proc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) {
//	static HWND hEditMsg;
//
//	SendMessage(hEditMsg, EM_SETLIMITTEXT, BUFSIZE/2, 0);
//	switch (iMsg)
//	{
//	case WM_INITDIALOG:
//		return 1;
//	case WM_COMMAND:
//		switch (LOWORD(wParam)) {
//		case ID_CONNECT:
//			hClientThread = CreateThread(NULL, 0, ClientMain, NULL, 0, NULL);
//			if (hClientThread == NULL) {
//				return 1;
//			}
//			//else {
//			//	//EnableWindow(hButtonConnect, FALSE);
//			//	//while (g_bStart == FALSE); // ���� ���� ���� ��ٸ�
//			//	//EnableWindow(hButtonIsIPv6, FALSE);
//			//	//EnableWindow(hEditIPaddr, FALSE);
//			//	//EnableWindow(hEditPort, FALSE);
//			//	//EnableWindow(g_hButtonSendMsg, TRUE);
//			//	//SetFocus(hEditMsg);
//			//}
//			break;
//		case ID_SEND:
//			WaitForSingleObject(hReadEvent, INFINITE);
//			GetDlgItemText(hDlg, IDC_EDIT_Input, chatmsg, BUFSIZE/2);
//			// ���� �ϷḦ �˸�
//			SetEvent(hWriteEvent);
//			// �Էµ� �ؽ�Ʈ ��ü�� ���� ǥ��
//			SendMessage(GetDlgItem(hDlg, IDC_EDIT_Input), EM_SETSEL, 0, -1);
//			return TRUE;
//		case IDCANCEL:
//			EndDialog(hDlg, 0);
//			break;
//		}
//		break;
//	}
//	return 0;
//}
//
//DWORD WINAPI ClientMain(LPVOID arg)
//{
//	int retval;
//
//	// socket()
//	sock = socket(AF_INET, SOCK_STREAM, 0);
//	if (sock == INVALID_SOCKET) err_quit("socket()");
//
//	// connect()
//	SOCKADDR_IN serveraddr;
//	ZeroMemory(&serveraddr, sizeof(serveraddr));
//	serveraddr.sin_family = AF_INET;
//	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
//	serveraddr.sin_port = htons(SERVERPORT);
//	retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
//	if (retval == SOCKET_ERROR) err_quit("connect()");
//	MessageBox(NULL, "������ �����߽��ϴ�.", "����!",MB_ICONINFORMATION);
//
//	HANDLE hThread[2];
//	hThread[0] = CreateThread(NULL, 0, ReadThread, NULL, 0, NULL);
//	hThread[1] = CreateThread(NULL, 0, WriteThread, NULL, 0, NULL);
//	if (hThread[0] == NULL || hThread[1] == NULL) {
//		MessageBox(NULL, "�����带 ������ �� �����ϴ�."
//			"\r\n���α׷��� �����մϴ�.",
//			"����!", MB_ICONERROR);
//		exit(1);
//	}
//	//g_bStart = TRUE;
//	// ������ ���� ���
//	retval = WaitForMultipleObjects(2, hThread, FALSE, INFINITE);
//	retval -= WAIT_OBJECT_0;
//	if (retval == 0)
//		TerminateThread(hThread[1], 1);
//	else
//		TerminateThread(hThread[0], 1);
//	CloseHandle(hThread[0]);
//	CloseHandle(hThread[1]);
//	//g_bStart = FALSE;
//	MessageBox(NULL, "������ ������ �������ϴ�", "�˸�",
//		MB_ICONINFORMATION);
//	//EnableWindow(g_hButtonSendMsg, FALSE);
//	closesocket(sock);
//	return 0;
//}
//// ������ �ޱ�
//DWORD WINAPI ReadThread(LPVOID arg)
//{
//	int retval;
//	//COMM_MSG comm_msg;
//	//CHAT_MSG* chat_msg;
//	char msg[100];
//	char msg_head[] = "[���� �޽���] ";
//
//	while (1) {
//		retval = recvn(sock, (char*)&msg, BUFSIZE, 0);
//		if (retval == 0 || retval == SOCKET_ERROR) {
//			break;
//		}
//		//SetDlgItemText(hDlg, IDC_EDIT_CHAT, (char*)strcat(msg_head, msg));
//		DisplayText((char*)"[���� �޽���] %s\r\n", msg);
//
//	}
//	return 0;
//}
//// ������ ������
//DWORD WINAPI WriteThread(LPVOID arg)
//{
//	int retval;
//	// ������ ������ ���
//	while (1) {
//		// ���� �Ϸ� ��ٸ���
//		WaitForSingleObject(hWriteEvent, INFINITE);
//		// ���ڿ� ���̰� 0�̸� ������ ����
//		if (strlen(chatmsg) == 0) {
//			// '�޽��� ����' ��ư Ȱ��ȭ
//			//EnableWindow(g_hButtonSendMsg, TRUE);
//			// �б� �Ϸ� �˸���
//			SetEvent(hReadEvent);
//			continue;
//		}
//		// ������ ������
//		retval = send(sock, (char*)&chatmsg, BUFSIZE, 0);
//		if (retval == SOCKET_ERROR) {
//			break;
//		}
//		// '�޽��� ����' ��ư Ȱ��ȭ
//		//EnableWindow(hButtonSendMsg, TRUE);
//		// �б� �Ϸ� �˸���
//		SetEvent(hReadEvent);
//	}
//	return 0;
//}
////	// ������ ��ſ� ����� ����
////	char buf[BUFSIZE + 1];
////	int len;
////	// ������ ������ ���
////	while (1) {
////		// ������ �Է�
////		printf("\n[���� ������] ");
////		if (fgets(buf, BUFSIZE + 1, stdin) == NULL)
////			break;
////
////		// '\n' ���� ����
////		len = strlen(buf);
////		if (buf[len - 1] == '\n')
////			buf[len - 1] = '\0';
////		if (strlen(buf) == 0)
////			break;
////
////		// ������ ������
////		retval = send(sock, buf, strlen(buf), 0);
////		if (retval == SOCKET_ERROR) {
////			err_display("send()");
////			break;
////		}
////		printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� ���½��ϴ�.\n", retval);
////
////		// ������ �ޱ�
////		retval = recvn(sock, buf, retval, 0);
////		if (retval == SOCKET_ERROR) {
////			err_display("recv()");
////			break;
////		}
////		else if (retval == 0)
////			break;
////
////		// ���� ������ ���
////		buf[retval] = '\0';
////		printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� �޾ҽ��ϴ�.\n", retval);
////		printf("[���� ������] %s\n", buf);
////	}
////
////	// closesocket()
////	closesocket(sock);
////	return 0;
////}
//
//// ����Ʈ ��Ʈ�ѿ� ���ڿ� ���
//void DisplayText(char* fmt, ...)
//{
//	va_list arg;
//	va_start(arg, fmt);
//	char cbuf[1024];
//	vsprintf(cbuf, fmt, arg);
//	int nLength = GetWindowTextLength(hEditStatus);
//	SendMessage(hEditStatus, EM_SETSEL, nLength, nLength);
//	SendMessage(hEditStatus, EM_REPLACESEL, FALSE, (LPARAM)cbuf);
//	va_end(arg);
//}
//
//void err_quit(const char* msg)
//{
//	LPVOID lpMsgBuf;
//	FormatMessage(
//		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
//		NULL, WSAGetLastError(),
//		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
//		(LPTSTR)&lpMsgBuf, 0, NULL);
//	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
//	LocalFree(lpMsgBuf);
//	exit(1);
//}
//
//// ���� �Լ� ���� ���
//void err_display(const char* msg)
//{
//	LPVOID lpMsgBuf;
//	FormatMessage(
//		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
//		NULL, WSAGetLastError(),
//		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
//		(LPTSTR)&lpMsgBuf, 0, NULL);
//	printf("[%s] %s", msg, (char*)lpMsgBuf);
//	LocalFree(lpMsgBuf);
//}
//
//// ����� ���� ������ ���� �Լ�
//int recvn(SOCKET s, char* buf, int len, int flags)
//{
//	int received;
//	char* ptr = buf;
//	int left = len;
//
//	while (left > 0) {
//		received = recv(s, ptr, left, flags);
//		if (received == SOCKET_ERROR)
//			return SOCKET_ERROR;
//		else if (received == 0)
//			break;
//		left -= received;
//		ptr += received;
//	}
//	return (len - left);
//}
