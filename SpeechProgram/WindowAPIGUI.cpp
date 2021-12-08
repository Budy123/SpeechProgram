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
//			//	//while (g_bStart == FALSE); // 서버 접속 성공 기다림
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
//			// 쓰기 완료를 알림
//			SetEvent(hWriteEvent);
//			// 입력된 텍스트 전체를 선택 표시
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
//	MessageBox(NULL, "서버에 접속했습니다.", "성공!",MB_ICONINFORMATION);
//
//	HANDLE hThread[2];
//	hThread[0] = CreateThread(NULL, 0, ReadThread, NULL, 0, NULL);
//	hThread[1] = CreateThread(NULL, 0, WriteThread, NULL, 0, NULL);
//	if (hThread[0] == NULL || hThread[1] == NULL) {
//		MessageBox(NULL, "스레드를 시작할 수 없습니다."
//			"\r\n프로그램을 종료합니다.",
//			"실패!", MB_ICONERROR);
//		exit(1);
//	}
//	//g_bStart = TRUE;
//	// 스레드 종료 대기
//	retval = WaitForMultipleObjects(2, hThread, FALSE, INFINITE);
//	retval -= WAIT_OBJECT_0;
//	if (retval == 0)
//		TerminateThread(hThread[1], 1);
//	else
//		TerminateThread(hThread[0], 1);
//	CloseHandle(hThread[0]);
//	CloseHandle(hThread[1]);
//	//g_bStart = FALSE;
//	MessageBox(NULL, "서버가 접속을 끊었습니다", "알림",
//		MB_ICONINFORMATION);
//	//EnableWindow(g_hButtonSendMsg, FALSE);
//	closesocket(sock);
//	return 0;
//}
//// 데이터 받기
//DWORD WINAPI ReadThread(LPVOID arg)
//{
//	int retval;
//	//COMM_MSG comm_msg;
//	//CHAT_MSG* chat_msg;
//	char msg[100];
//	char msg_head[] = "[받은 메시지] ";
//
//	while (1) {
//		retval = recvn(sock, (char*)&msg, BUFSIZE, 0);
//		if (retval == 0 || retval == SOCKET_ERROR) {
//			break;
//		}
//		//SetDlgItemText(hDlg, IDC_EDIT_CHAT, (char*)strcat(msg_head, msg));
//		DisplayText((char*)"[받은 메시지] %s\r\n", msg);
//
//	}
//	return 0;
//}
//// 데이터 보내기
//DWORD WINAPI WriteThread(LPVOID arg)
//{
//	int retval;
//	// 서버와 데이터 통신
//	while (1) {
//		// 쓰기 완료 기다리기
//		WaitForSingleObject(hWriteEvent, INFINITE);
//		// 문자열 길이가 0이면 보내지 않음
//		if (strlen(chatmsg) == 0) {
//			// '메시지 전송' 버튼 활성화
//			//EnableWindow(g_hButtonSendMsg, TRUE);
//			// 읽기 완료 알리기
//			SetEvent(hReadEvent);
//			continue;
//		}
//		// 데이터 보내기
//		retval = send(sock, (char*)&chatmsg, BUFSIZE, 0);
//		if (retval == SOCKET_ERROR) {
//			break;
//		}
//		// '메시지 전송' 버튼 활성화
//		//EnableWindow(hButtonSendMsg, TRUE);
//		// 읽기 완료 알리기
//		SetEvent(hReadEvent);
//	}
//	return 0;
//}
////	// 데이터 통신에 사용할 변수
////	char buf[BUFSIZE + 1];
////	int len;
////	// 서버와 데이터 통신
////	while (1) {
////		// 데이터 입력
////		printf("\n[보낼 데이터] ");
////		if (fgets(buf, BUFSIZE + 1, stdin) == NULL)
////			break;
////
////		// '\n' 문자 제거
////		len = strlen(buf);
////		if (buf[len - 1] == '\n')
////			buf[len - 1] = '\0';
////		if (strlen(buf) == 0)
////			break;
////
////		// 데이터 보내기
////		retval = send(sock, buf, strlen(buf), 0);
////		if (retval == SOCKET_ERROR) {
////			err_display("send()");
////			break;
////		}
////		printf("[TCP 클라이언트] %d바이트를 보냈습니다.\n", retval);
////
////		// 데이터 받기
////		retval = recvn(sock, buf, retval, 0);
////		if (retval == SOCKET_ERROR) {
////			err_display("recv()");
////			break;
////		}
////		else if (retval == 0)
////			break;
////
////		// 받은 데이터 출력
////		buf[retval] = '\0';
////		printf("[TCP 클라이언트] %d바이트를 받았습니다.\n", retval);
////		printf("[받은 데이터] %s\n", buf);
////	}
////
////	// closesocket()
////	closesocket(sock);
////	return 0;
////}
//
//// 에디트 컨트롤에 문자열 출력
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
//// 소켓 함수 오류 출력
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
//// 사용자 정의 데이터 수신 함수
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
