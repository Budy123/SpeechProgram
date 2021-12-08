#include <winsock2.h>//windows.h보다 위에 있어야한다...고..

#include <windows.h>
#include <process.h>
#include <stdio.h>
#include"resource.h"




LRESULT CALLBACK ChathWndEnterkeyProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam); // 채팅창에서 enter키 처리
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); //처음 시작화면 처리
LRESULT CALLBACK WndServIntroProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam); //서버 인트로 창 처리
LRESULT CALLBACK WndChatProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);  // 채팅창 처리
LRESULT CALLBACK WndClientIntroProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam); // 클라이언트 인트로창 처리

void returnEmotion(char* msg);//채팅 메시지 중 감정표현에 대한 단어를 알아냄
void openServSock(HWND hWnd); // 서버 소켓 열기
void openClientSock(HWND hWnd); // 클라이언트 소켓 열기
void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit); // 비트맵 그리기 함수
UINT WINAPI ThreadReceiv(LPVOID lpParam);  // 메시지 받기 쓰래드
BOOL LoadFile(HWND hEdit, LPSTR pszFileName); // txt파일을 읽어서 에디트 박스에 뿌려줌
BOOL delfile(LPSTR pszFileName); // 파일 삭제 함수
// LRESULT : Long -> int형 자료형
// CALLBACK : 운영체제에서 호출하는 함수

HINSTANCE g_hInst;
// HINSTANCE : 자료형
// g_ : global 이라는 의미를 가지게 하는 접두어, 즉 전역변수라는 의미



HANDLE hRecvThread; // 쓰래드 핸들
DWORD dwThreadReceivID; //쓰래드 아이디

LPCTSTR lpszClass = TEXT("First");
LPCTSTR ServerintroClass = TEXT("ServIntro");
LPCTSTR ChatClass = TEXT("Chat");
LPCTSTR ClientIntroClass = TEXT("ClientIntro");
//PCTSTR introlpszClass=TEXT("intro");
// LPCTSTR : Long Pointer ConsT STRing
// lpszClass : long pointer string zero(null로 끝나는 문자열)
// TEXT : 유니코드로 인식 아니면 아스키 코드로 인식
// 2003이나 2005에선 "test" 를 적으면 에러를 띄울수 있다.
// 그러면 L"test"를 적으면 정상적으로 인식하게 된다.


HWND servBtn, clientBtn, nameEdit, ipEdit, portEdit, servExcBtn, InputMsgEdit, MsgSendBtn, ChatViewEdit, clientExcBtn;
HWND nameStatic, ipStatic, portStatic;
HWND ServIntrohWnd, ChathWnd, ClientIntroWnd;
WNDPROC OldEditProc;
int isServer = -1; //서버인지 클라이언트인지 판별.. 1일경우 서버 0일경우 클라이언트.
SOCKET hServSock, hClntSock;        //소켓

char ip[20], port[10], name[100];
FILE* f;
HANDLE hFile;
HBITMAP BIT1, BIT2;
DWORD emotion = 0;



int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
    , LPSTR lpszCmdParam, int nCmdShow)
    /*
        APIENTRY : api의 시작지점,
                다른 의미는 함수호출 기법중의 하나인데 스택 푸시방법을 사용한다.
                A(1,2,3) 이 있으면  bottom/1-2-3/up(c 표준), bottom/3-2-1/up
                _stdcall로 정의 되어 있다. 이 매크로들은 호환성과 이식성을 위해 존재한다.
        HINSTANCE hInstance : 프로그램의 인스턴스 핸들, 즉 현재 실행중인 부분이라는 의미이다.
        HINSTANCE hPrevInstance : 이 프로그램이 실행되기 전에 실행되었던
                                  인스턴스 핸들이다. win32에서는 항상 NULL이다.
        LPSTR lpszCmdParam : 프로그램의 인자. 도스의 argv에 해당된다.
                             보통 실행 직후에 열 파일의 경로가 전달된다.
        int nCmdShow :
    */
{
    HWND hWnd;            // 윈도우 핸들러


    MSG Message;                    // 메시지
    WNDCLASS WndClass;        // 구조체
    g_hInst = hInstance;
    //////////////////
    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hbrBackground = (HBRUSH)COLOR_WINDOW;        // 화면을 흰색으로 칠함
    WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);                    // 마우스 모양을 표시함
    WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);          // 왼쪽 위 상단에 나타나는 아이콘
    WndClass.hInstance = hInstance;                           // 현재 윈도우 인스턴스
    WndClass.lpfnWndProc = WndProc;                           // 콜백함수 등록
    WndClass.lpszClassName = lpszClass;                        // 창의 이름(lpszClass는 현재 First로 입력되어 있음)
    WndClass.lpszMenuName = NULL;          // 메뉴 이름
    WndClass.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&WndClass);                              // 윈도우 클래스를 등록
    ///////////////////

    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hbrBackground = (HBRUSH)COLOR_WINDOW;        // 화면을 흰색으로 칠함
    WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);                    // 마우스 모양을 표시함
    WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);          // 왼쪽 위 상단에 나타나는 아이콘
    WndClass.hInstance = hInstance;                           // 현재 윈도우 인스턴스
    WndClass.lpfnWndProc = WndServIntroProc;                  // 콜백함수 등록
    WndClass.lpszClassName = ServerintroClass;                                // 창의 이름
    WndClass.lpszMenuName = NULL;          // 메뉴 이름
    WndClass.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&WndClass);                              // 윈도우 클래스를 등록

    ///////////////////////////////
    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hbrBackground = (HBRUSH)COLOR_WINDOW;        // 화면을 흰색으로 칠함
    WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);                    // 마우스 모양을 표시함
    WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);          // 왼쪽 위 상단에 나타나는 아이콘
    WndClass.hInstance = hInstance;                           // 현재 윈도우 인스턴스
    WndClass.lpfnWndProc = WndChatProc;                           // 콜백함수 등록
    WndClass.lpszClassName = ChatClass;                        // 창의 이름
    WndClass.lpszMenuName = NULL;          // 메뉴 이름
    WndClass.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&WndClass);

    ////////////////////////////////////////

    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hbrBackground = (HBRUSH)COLOR_WINDOW;        // 화면을 흰색으로 칠함
    WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);                    // 마우스 모양을 표시함
    WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);          // 왼쪽 위 상단에 나타나는 아이콘
    WndClass.hInstance = hInstance;                           // 현재 윈도우 인스턴스
    WndClass.lpfnWndProc = WndClientIntroProc;                                    // 콜백함수 등록
    WndClass.lpszClassName = ClientIntroClass;                                // 창의 이름
    WndClass.lpszMenuName = NULL;          // 메뉴 이름
    WndClass.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&WndClass);





    hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,    // 창을 생성
        GetSystemMetrics(SM_CXSCREEN) / 2 - 500, GetSystemMetrics(SM_CYSCREEN) / 2 - 150, 500, 482,
        NULL, (HMENU)NULL, hInstance, NULL);

    ServIntrohWnd = CreateWindow(ServerintroClass, ServerintroClass, WS_OVERLAPPEDWINDOW,      // 창을 생성
        GetSystemMetrics(SM_CXSCREEN) / 2 - 500, GetSystemMetrics(SM_CYSCREEN) / 2 - 150, 380, 150,
        NULL, (HMENU)NULL, hInstance, NULL);

    ChathWnd = CreateWindow(ChatClass, ChatClass, WS_OVERLAPPEDWINDOW,    // 창을 생성
        GetSystemMetrics(SM_CXSCREEN) / 2 - 500, GetSystemMetrics(SM_CYSCREEN) / 2 - 150, 714, 447,
        NULL, (HMENU)NULL, hInstance, NULL);

    ClientIntroWnd = CreateWindow(ClientIntroClass, ClientIntroClass, WS_OVERLAPPEDWINDOW,    // 창을 생성
        GetSystemMetrics(SM_CXSCREEN) / 2 - 500, GetSystemMetrics(SM_CYSCREEN) / 2 - 150, 380, 194,
        NULL, (HMENU)NULL, hInstance, NULL);
    /*
        1번째 인자 (lpszClassName): 생성하고자 하는 윈도우 클래스를 지정하는 문자열
        2번째 인자 (lpszWindowName) : 윈도우 타이틀 바에 나타날 문자열
        3번째 인자 (dwStyle): 만들고자 하는 윈도우의 형태를 지정하는 인수
        4번째 인자 ~ 7번째 인자(X,Y,nWidth,nHeight) : X, Y 좌표와 창의 크기 설정
        8번째 인자 (hWndParent) : 부모 윈도우가 있을 경우 부모 윈도우의 핸들을 지정
        9번째 인자 (hmenu) : 윈도우에서 사용할 메뉴의 핸들을 지정
        10번째 인자 (hinst) : 윈도우를 만드는 주체, 즉 프로그램의 핸들을 지정
        11번째 인자 (lpvParam) : CREATSTRUCT라는 구조체의 번지이며
    */

    //  introhwnd=CreateWindow(

    ShowWindow(hWnd, nCmdShow);      // 창을 보여주게 해주는 함수
                                        // 띄울 윈도우의 핸들 값과 윈도우를 화면에 출력하는 방법을 지정


    while (GetMessage(&Message, NULL, 0, 0)) // 메시지 큐에서 메시지를 읽어들인다.
    {
        TranslateMessage(&Message);  // 키보드 입력 메시지를 가공하여 프로그램에서 쉽게 쓸수 있도록 한다.
        DispatchMessage(&Message);    // 메시지 큐에서 꺼낸 메시지를 윈도우의 메시지 처리 함수(WndProc)로 전달한다.
    }
    return (int)Message.wParam;   // WM_QIUT 메시지로부터 전달된 탈출 코드(exit code)이며
                                        // 이 프로그램을 실행시킨 운영체제로 리턴된다.
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    /*
    프로그램 처음 실행시 나오는 창..

    서버와 클라이언트 모드를 선택하는 창이다.


    */

    HDC hdc;
    RECT rc;
    PAINTSTRUCT ps;
    HBITMAP Background;

    switch (iMessage) {

    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        Background = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP2));
        DrawBitmap(hdc, 0, 0, Background);
        EndPaint(hWnd, &ps);
        return 0;


    case WM_CREATE:
        servBtn = CreateWindow(
            "Button",
            "서버",
            WS_CHILD | WS_VISIBLE | WS_BORDER | WS_EX_STATICEDGE,
            105, 393, 100, 30,
            hWnd,
            (HMENU)1,
            ((LPCREATESTRUCT)lParam)->hInstance,
            NULL
        );
        clientBtn = CreateWindow(
            "Button",
            "클라이언트",
            WS_CHILD | WS_VISIBLE | WS_BORDER | WS_EX_STATICEDGE,
            260, 393, 100, 30,
            hWnd,
            (HMENU)2,
            ((LPCREATESTRUCT)lParam)->hInstance,
            NULL
        );
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == 1 && HIWORD(wParam) == BN_CLICKED && (HWND)lParam == servBtn)
        {//서버 버튼
            isServer = 1;
            ShowWindow(ServIntrohWnd, SW_SHOW); //서버인트로창을 보여줌

            DestroyWindow(hWnd); // 현재창 닫기
        }
        if (LOWORD(wParam) == 2 && HIWORD(wParam) == BN_CLICKED && (HWND)lParam == clientBtn)
        {//클라이언트 버튼
            isServer = 0;
            ShowWindow(ClientIntroWnd, SW_SHOW); // 클라이언트 인트로창을 보여줌

            DestroyWindow(hWnd); //현재창 닫기
        }
        break;

    case WM_DESTROY:
        break;
        //  PostQuitMessage(0);
            //return 0;
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;

    }
    return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

LRESULT CALLBACK WndServIntroProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    /*

        서버 선택시 서버 인트로창
        포트설정과 닉네임을 받는다.
        그리고 메시지 받는 쓰래드를 시작한다.

    */


    int len;
    switch (iMessage) {
    case WM_CREATE:
        nameStatic = CreateWindow("Static",
            TEXT("대화명 입력 :"),
            WS_CHILD | WS_VISIBLE,
            14, 14, 100, 25,
            hWnd,
            (HMENU)7,
            ((LPCREATESTRUCT)lParam)->hInstance,
            NULL
        );


        nameEdit = CreateWindow(
            "Edit",
            TEXT(""),
            WS_CHILD | WS_VISIBLE | WS_BORDER | WS_EX_STATICEDGE,
            110, 13, 100, 25,
            hWnd,
            (HMENU)3,
            ((LPCREATESTRUCT)lParam)->hInstance,
            NULL
        );
        portStatic = CreateWindow("Static",
            TEXT("PORT :"),
            WS_CHILD | WS_VISIBLE,
            14, 54, 100, 25,
            hWnd,
            (HMENU)NULL,
            ((LPCREATESTRUCT)lParam)->hInstance,
            NULL
        );

        portEdit = CreateWindow(
            "Edit",
            TEXT(""),
            WS_CHILD | WS_VISIBLE | WS_BORDER | WS_EX_STATICEDGE,
            110, 54, 100, 25,
            hWnd,
            (HMENU)5,
            ((LPCREATESTRUCT)lParam)->hInstance,
            NULL
        );
        servExcBtn = CreateWindow(
            "Button",
            TEXT("서버 실행"),
            WS_CHILD | WS_VISIBLE | WS_BORDER | WS_EX_STATICEDGE,
            220, 30, 100, 25,
            hWnd,
            (HMENU)6,
            ((LPCREATESTRUCT)lParam)->hInstance,
            NULL
        );


        break;
    case WM_COMMAND:
        if (LOWORD(wParam) == 6 && HIWORD(wParam) == BN_CLICKED && (HWND)lParam == servExcBtn)
        {//서버 실행 버튼

            GetDlgItemText(hWnd, 3, name, sizeof(name)); //닉네임 받아오기
            len = strlen(name);
            name[len] = 0;

            GetDlgItemText(hWnd, 5, port, sizeof(port)); //포트번호 받아오기


            openServSock(hWnd); //서버소켓 열기

            hRecvThread = (HANDLE)_beginthreadex(NULL, 0, ThreadReceiv, (LPVOID)hClntSock, 0, (unsigned*)&dwThreadReceivID);//메시지 받는 쓰래드 시작.






            send(hClntSock, "서버에 접속 완료", 20, 0);


            ShowWindow(ChathWnd, SW_SHOW);
            DestroyWindow(hWnd);
        }
        break;

    case WM_DESTROY:
        //PostQuitMessage(0);
        //return 0;
        break;
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
    }
    return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

LRESULT CALLBACK WndChatProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{

    /*
        채팅창
        서버모드와 클라이언트모드 둘다 같은 창을 이용한다.
        좌측에 이미지는 채팅메시지에 따라 변화하게 된다.
        채팅창의 내용은 data.dat파일로 저장하면서 채팅창 에디트 박스에 뿌려준다.

        프로그램 종료시 그 파일도 같이 소멸된다.


    */

    FILE* f; //채팅내용저장 파일 변수
    int num; //채팅창 라인수 저장 변수
    char result[1200]; // 최종 메시지
    char msg[1000]; //입력메시지.
    int len;

    char imserver[100];

    HDC hdc;
    RECT rc;
    PAINTSTRUCT ps;
    HBITMAP Background;

    switch (iMessage) {

    case WM_PAINT:

        hdc = BeginPaint(hWnd, &ps);
        /*채팅창의 내용에따라 다른 이미지를 보여줌 */

        if (emotion == 1)//ㅠㅠ
            Background = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP3));
        else if (emotion == 2)//ㅋㅋ
            Background = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP4));
        else if (emotion == 3)//하지마
            Background = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP6));
        else
            Background = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP5));
        DrawBitmap(hdc, 0, 0, Background);
        EndPaint(hWnd, &ps);



        return 0;




    case WM_CREATE:



        ChatViewEdit = CreateWindow(
            "Edit",
            TEXT(""),
            WS_VSCROLL | ES_AUTOVSCROLL | ES_READONLY | ES_WANTRETURN | WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE,
            147, 37, 550, 319,
            hWnd,
            (HMENU)NULL,
            ((LPCREATESTRUCT)lParam)->hInstance,
            NULL
        );

        InputMsgEdit = CreateWindow(
            "Edit",
            TEXT(""),
            WS_CHILD | WS_VISIBLE | WS_BORDER | WS_EX_STATICEDGE,
            147, 366, 460, 25,
            hWnd,
            (HMENU)444,
            ((LPCREATESTRUCT)lParam)->hInstance,
            NULL
        );
        MsgSendBtn = CreateWindow(
            "Button",
            TEXT("Send"),
            WS_CHILD | WS_VISIBLE | WS_BORDER | WS_EX_STATICEDGE,
            607, 366, 90, 25,
            hWnd,
            (HMENU)99,
            ((LPCREATESTRUCT)lParam)->hInstance,
            NULL
        );
        OldEditProc = (WNDPROC)SetWindowLong(InputMsgEdit, GWL_WNDPROC, (LONG)ChathWndEnterkeyProc);




        break;
    case WM_COMMAND:
        if (LOWORD(wParam) == 99 && HIWORD(wParam) == BN_CLICKED && (HWND)lParam == MsgSendBtn)
        {//메시지 send 버튼을 눌렀을경우.
            GetDlgItemText(hWnd, 444, msg, sizeof(msg)); //메시지 가저오기
            returnEmotion(msg); // 메시지를 returnEmotion함수로 전달해 감정표현 확인
            len = strlen(msg); //메시지 길이
            msg[len] = 0; // 메시지 끝.
            sprintf(result, "%s : %s", name, msg);// 닉네임과 메시지 합치기

            len = strlen(result);

            result[len] = 0;
            send(hClntSock, result, len, 0); // 메시지 보내기

            f = fopen("data.dat", "a"); // 파일열기

            fprintf(f, "%s ", result); // 내용 파일에 쓰기
            fclose(f); // 파일닫기

            LoadFile(ChatViewEdit, "data.dat"); // 파일 불러서 채팅창에 뿌리기
            num = SendMessage(ChatViewEdit, EM_GETLINECOUNT, 0, 0);//라인수 가저오기
            SendMessage(ChatViewEdit, EM_LINESCROLL, 0, num); // 스크롤 내리기
            SetWindowText(InputMsgEdit, "");// 채팅창 비우기
            InvalidateRect(hWnd, NULL, TRUE); // 이미지 다시 그리기
        }
        break;


    case WM_DESTROY: //창종료시
        delfile("data.dat"); //채팅내용 파일 삭제
        send(hClntSock, "--------- 접속종료 -----------", 30, 0); // 접속종료라는 메시지를 상대방에게 보냄.

        closesocket(hClntSock); //소켓닫기
        if (isServer == 1)
            closesocket(hServSock); //서버라면 서버소켓도 닫기
        TerminateThread(ThreadReceiv, 0); //쓰래드 종료

        PostQuitMessage(0);//끝!.
        return 0;
    }
    return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

LRESULT CALLBACK WndClientIntroProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{

    /*

        클라이언트 인트로 창

        접속할 ip주소와 포트번호, 닉네임을 입력받는다.

    */

    int len;
    switch (iMessage) {
    case WM_CREATE:
        nameStatic = CreateWindow("Static",
            TEXT("대화명 입력 :"),
            WS_CHILD | WS_VISIBLE,
            14, 14, 100, 25,
            hWnd,
            (HMENU)NULL,
            ((LPCREATESTRUCT)lParam)->hInstance,
            NULL
        );

        nameEdit = CreateWindow(
            "Edit",
            TEXT(""),
            WS_CHILD | WS_VISIBLE | WS_BORDER | WS_EX_STATICEDGE,
            110, 13, 100, 25,
            hWnd,
            (HMENU)3,
            ((LPCREATESTRUCT)lParam)->hInstance,
            NULL
        );
        ipStatic = CreateWindow("Static",
            TEXT("IP 주소 :"),
            WS_CHILD | WS_VISIBLE,
            14, 58, 100, 25,
            hWnd,
            (HMENU)NULL,
            ((LPCREATESTRUCT)lParam)->hInstance,
            NULL
        );
        ipEdit = CreateWindow(
            "Edit",
            TEXT(""),
            WS_CHILD | WS_VISIBLE | WS_BORDER | WS_EX_STATICEDGE,
            110, 58, 180, 25,
            hWnd,
            (HMENU)4,
            ((LPCREATESTRUCT)lParam)->hInstance,
            NULL
        )
            ;

        portStatic = CreateWindow("Static",
            TEXT("PORT :"),
            WS_CHILD | WS_VISIBLE,
            14, 109, 100, 25,
            hWnd,
            (HMENU)NULL,
            ((LPCREATESTRUCT)lParam)->hInstance,
            NULL
        );
        portEdit = CreateWindow(
            "Edit",
            TEXT(""),
            WS_CHILD | WS_VISIBLE | WS_BORDER | WS_EX_STATICEDGE,
            110, 109, 100, 25,
            hWnd,
            (HMENU)5,
            ((LPCREATESTRUCT)lParam)->hInstance,
            NULL);

        clientExcBtn = CreateWindow(
            "Button",
            TEXT("클라이언트 실행"),
            WS_CHILD | WS_VISIBLE | WS_BORDER | WS_EX_STATICEDGE,
            235, 109, 120, 25,
            hWnd,
            (HMENU)9,
            ((LPCREATESTRUCT)lParam)->hInstance,
            NULL
        );
        break;
    case WM_COMMAND:
        if (LOWORD(wParam) == 9 && HIWORD(wParam) == BN_CLICKED && (HWND)lParam == clientExcBtn)
        {//서버 실행 버튼
            GetDlgItemText(hWnd, 3, name, sizeof(name));

            len = strlen(name);
            name[len] = 0;
            GetDlgItemText(hWnd, 5, port, sizeof(port));
            GetDlgItemText(hWnd, 4, ip, sizeof(ip));
            openClientSock(hWnd);
            hRecvThread = (HANDLE)_beginthreadex(NULL, 0, ThreadReceiv, (LPVOID)hClntSock, 0, (unsigned*)&dwThreadReceivID);
            send(hClntSock, "클라이언트접속!!", 20, 0);
            ShowWindow(ChathWnd, SW_SHOW);
            DestroyWindow(hWnd);
        }



        break;

    case WM_DESTROY:
        //PostQuitMessage(0);
        //return 0;
        break;
    case WM_CLOSE:

        PostQuitMessage(0);
        return 0;
    }
    return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}


void openServSock(HWND hWnd)
{
    /*

        서버 소켓 열기함수

    */


    WSADATA wsaData;

    SOCKADDR_IN servAddr, clntAddr;
    int szClntAddr;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        MessageBox(hWnd, TEXT("WSAStartup() error!"), TEXT("WSAStartup() error!"), 0);
    }

    hServSock = socket(PF_INET, SOCK_STREAM, 0);
    if (hServSock == INVALID_SOCKET)
        MessageBox(hWnd, TEXT("socket() error"), TEXT("socket() error"), 0);
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(atoi(port));

    if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
        MessageBox(hWnd, TEXT("bind() error"), TEXT("bind() error"), 0);
    if (listen(hServSock, 5) == SOCKET_ERROR)
        MessageBox(hWnd, TEXT("listen() error"), TEXT("listen() error"), 0);

    szClntAddr = sizeof(clntAddr);
    hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr);
    if (hClntSock == INVALID_SOCKET)
        MessageBox(hWnd, TEXT("accetp() error"), TEXT("accetp() error"), 0);









}

void openClientSock(HWND hWnd)
{
    //클라이언트 소켓 열기 함수

    WSADATA wsaData;
    SOCKADDR_IN servAddr;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        MessageBox(hWnd, TEXT("WSAStartup() error"), TEXT("WSAStartup() error"), 0);
    hClntSock = socket(PF_INET, SOCK_STREAM, 0);
    if (hClntSock == INVALID_SOCKET)
        MessageBox(hWnd, TEXT("hSocketet() error"), TEXT("hSocketet() error"), 0);


    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(ip);
    servAddr.sin_port = htons(atoi(port));

    if (connect(hClntSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
        MessageBox(hWnd, TEXT("connect() error!"), TEXT("connect() error!"), 0);





}
BOOL LoadFile(HWND hEdit, LPSTR pszFileName)
{
    // 파일 불러와 에딧트박스에 뿌리기
     //dev c++ API 메모장 예제 참고

    BOOL bSuccess = FALSE;

    hFile = CreateFile(pszFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, 0);
    if (hFile != INVALID_HANDLE_VALUE)
    {
        DWORD dwFileSize;
        dwFileSize = GetFileSize(hFile, NULL);
        if (dwFileSize != 0xFFFFFFFF)
        {
            LPSTR pszFileText;
            pszFileText = (LPSTR)GlobalAlloc(GPTR, dwFileSize + 1);
            if (pszFileText != NULL)
            {
                DWORD dwRead;
                if (ReadFile(hFile, pszFileText, dwFileSize, &dwRead, NULL))
                {
                    pszFileText[dwFileSize] = 0; // Null terminator
                    if (SetWindowText(hEdit, pszFileText))
                        bSuccess = TRUE; // It worked!
                }
                GlobalFree(pszFileText);
            }
        }
        CloseHandle(hFile);
    }
    return bSuccess;
}
BOOL delfile(LPSTR pszFileName)
{
    //파일 삭제 함수 
    fclose(f);
    CloseHandle(hFile);

    DeleteFile(pszFileName);

    return 1;



}
UINT WINAPI ThreadReceiv(LPVOID lpParam)
{
    //메시지 받는 쓰래드 함수
    SOCKET sock;
    int num;

    char message[1000];
    int strLen;

    sock = (SOCKET)lpParam;// 인자로 받은 소켓을 저장
    while (1)
    {
        strLen = recv(sock, message, sizeof(message) - 1, 0);
        if (strLen == -1)
            return 0;
        message[strLen] = 0;

        //아래 과정은 메시지 보내는 send버튼시행시 수행하는 과정과 비슷한데
        //이는 내가 보낸 메시지도 채팅창에 떠야하고 , 내가 받는 메시지도 떠야하기때문..

        f = fopen("data.dat", "a");

        fprintf(f, "%s ", message);
        returnEmotion(message);
        fclose(f);

        LoadFile(ChatViewEdit, "data.dat");


        num = SendMessage(ChatViewEdit, EM_GETLINECOUNT, 0, 0);//라인수 가저오기
        SendMessage(ChatViewEdit, EM_LINESCROLL, 0, num); // 스크롤 내리기
        InvalidateRect(ChathWnd, NULL, TRUE);

        //SetWindowText(ChatViewEdit,message);

        message[0] = 0;
        //MessageBox (ChathWnd, message, TEXT("accetp() error"), 0);
    }
    return 0;
}

LRESULT CALLBACK ChathWndEnterkeyProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{// 채팅입력창에서 엔터키 처리
    int num;
    char result[1200];
    int len;
    char msg[1000];
    switch (iMessage) {
    case WM_KEYDOWN:
        if (wParam == VK_RETURN) {




            GetDlgItemText(ChathWnd, 444, msg, sizeof(msg));
            returnEmotion(msg);
            len = strlen(msg);
            msg[len] = 0;
            sprintf(result, "%s : %s", name, msg);
            len = strlen(result);

            result[len] = 0;
            send(hClntSock, result, len, 0);

            f = fopen("data.dat", "a");

            fprintf(f, "%s ", result);
            fclose(f);

            LoadFile(ChatViewEdit, "data.dat");

            num = SendMessage(ChatViewEdit, EM_GETLINECOUNT, 0, 0);//라인수 가저오기
            SendMessage(ChatViewEdit, EM_LINESCROLL, 0, num); // 스크롤 내리기


            InvalidateRect(hWnd, NULL, TRUE);

            SetWindowText(hWnd, "");


        }
        break;
    }

    return CallWindowProc(OldEditProc, hWnd, iMessage, wParam, lParam);




}

void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit)
{  //비트맵 그리기 함수
    HDC MemDC;
    HBITMAP OldBitmap;
    int bx, by;
    BITMAP bit;

    MemDC = CreateCompatibleDC(hdc);

    OldBitmap = (HBITMAP)SelectObject(MemDC, hBit);

    GetObject(hBit, sizeof(BITMAP), &bit);
    bx = bit.bmWidth;
    by = bit.bmHeight;

    BitBlt(hdc, x, y, bx, by, MemDC, 0, 0, SRCCOPY);
    SelectObject(MemDC, OldBitmap);
    DeleteDC(MemDC);


}

void returnEmotion(char* msg)
{

    if (strstr(msg, "ㅠ") || strstr(msg, "ㅜ"))
        emotion = 1;
    else if (strstr(msg, "ㅋ") || strstr(msg, "ㅎ"))
        emotion = 2;

    else if (strstr(msg, "ㄴㄴ")) //하지마
        emotion = 3;
    else
        emotion = 0;


}