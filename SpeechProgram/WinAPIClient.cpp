#include <winsock2.h>//windows.h���� ���� �־���Ѵ�...��..

#include <windows.h>
#include <process.h>
#include <stdio.h>
#include"resource.h"




LRESULT CALLBACK ChathWndEnterkeyProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam); // ä��â���� enterŰ ó��
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); //ó�� ����ȭ�� ó��
LRESULT CALLBACK WndServIntroProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam); //���� ��Ʈ�� â ó��
LRESULT CALLBACK WndChatProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);  // ä��â ó��
LRESULT CALLBACK WndClientIntroProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam); // Ŭ���̾�Ʈ ��Ʈ��â ó��

void returnEmotion(char* msg);//ä�� �޽��� �� ����ǥ���� ���� �ܾ �˾Ƴ�
void openServSock(HWND hWnd); // ���� ���� ����
void openClientSock(HWND hWnd); // Ŭ���̾�Ʈ ���� ����
void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit); // ��Ʈ�� �׸��� �Լ�
UINT WINAPI ThreadReceiv(LPVOID lpParam);  // �޽��� �ޱ� ������
BOOL LoadFile(HWND hEdit, LPSTR pszFileName); // txt������ �о ����Ʈ �ڽ��� �ѷ���
BOOL delfile(LPSTR pszFileName); // ���� ���� �Լ�
// LRESULT : Long -> int�� �ڷ���
// CALLBACK : �ü������ ȣ���ϴ� �Լ�

HINSTANCE g_hInst;
// HINSTANCE : �ڷ���
// g_ : global �̶�� �ǹ̸� ������ �ϴ� ���ξ�, �� ����������� �ǹ�



HANDLE hRecvThread; // ������ �ڵ�
DWORD dwThreadReceivID; //������ ���̵�

LPCTSTR lpszClass = TEXT("First");
LPCTSTR ServerintroClass = TEXT("ServIntro");
LPCTSTR ChatClass = TEXT("Chat");
LPCTSTR ClientIntroClass = TEXT("ClientIntro");
//PCTSTR introlpszClass=TEXT("intro");
// LPCTSTR : Long Pointer ConsT STRing
// lpszClass : long pointer string zero(null�� ������ ���ڿ�)
// TEXT : �����ڵ�� �ν� �ƴϸ� �ƽ�Ű �ڵ�� �ν�
// 2003�̳� 2005���� "test" �� ������ ������ ���� �ִ�.
// �׷��� L"test"�� ������ ���������� �ν��ϰ� �ȴ�.


HWND servBtn, clientBtn, nameEdit, ipEdit, portEdit, servExcBtn, InputMsgEdit, MsgSendBtn, ChatViewEdit, clientExcBtn;
HWND nameStatic, ipStatic, portStatic;
HWND ServIntrohWnd, ChathWnd, ClientIntroWnd;
WNDPROC OldEditProc;
int isServer = -1; //�������� Ŭ���̾�Ʈ���� �Ǻ�.. 1�ϰ�� ���� 0�ϰ�� Ŭ���̾�Ʈ.
SOCKET hServSock, hClntSock;        //����

char ip[20], port[10], name[100];
FILE* f;
HANDLE hFile;
HBITMAP BIT1, BIT2;
DWORD emotion = 0;



int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
    , LPSTR lpszCmdParam, int nCmdShow)
    /*
        APIENTRY : api�� ��������,
                �ٸ� �ǹ̴� �Լ�ȣ�� ������� �ϳ��ε� ���� Ǫ�ù���� ����Ѵ�.
                A(1,2,3) �� ������  bottom/1-2-3/up(c ǥ��), bottom/3-2-1/up
                _stdcall�� ���� �Ǿ� �ִ�. �� ��ũ�ε��� ȣȯ���� �̽ļ��� ���� �����Ѵ�.
        HINSTANCE hInstance : ���α׷��� �ν��Ͻ� �ڵ�, �� ���� �������� �κ��̶�� �ǹ��̴�.
        HINSTANCE hPrevInstance : �� ���α׷��� ����Ǳ� ���� ����Ǿ���
                                  �ν��Ͻ� �ڵ��̴�. win32������ �׻� NULL�̴�.
        LPSTR lpszCmdParam : ���α׷��� ����. ������ argv�� �ش�ȴ�.
                             ���� ���� ���Ŀ� �� ������ ��ΰ� ���޵ȴ�.
        int nCmdShow :
    */
{
    HWND hWnd;            // ������ �ڵ鷯


    MSG Message;                    // �޽���
    WNDCLASS WndClass;        // ����ü
    g_hInst = hInstance;
    //////////////////
    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hbrBackground = (HBRUSH)COLOR_WINDOW;        // ȭ���� ������� ĥ��
    WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);                    // ���콺 ����� ǥ����
    WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);          // ���� �� ��ܿ� ��Ÿ���� ������
    WndClass.hInstance = hInstance;                           // ���� ������ �ν��Ͻ�
    WndClass.lpfnWndProc = WndProc;                           // �ݹ��Լ� ���
    WndClass.lpszClassName = lpszClass;                        // â�� �̸�(lpszClass�� ���� First�� �ԷµǾ� ����)
    WndClass.lpszMenuName = NULL;          // �޴� �̸�
    WndClass.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&WndClass);                              // ������ Ŭ������ ���
    ///////////////////

    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hbrBackground = (HBRUSH)COLOR_WINDOW;        // ȭ���� ������� ĥ��
    WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);                    // ���콺 ����� ǥ����
    WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);          // ���� �� ��ܿ� ��Ÿ���� ������
    WndClass.hInstance = hInstance;                           // ���� ������ �ν��Ͻ�
    WndClass.lpfnWndProc = WndServIntroProc;                  // �ݹ��Լ� ���
    WndClass.lpszClassName = ServerintroClass;                                // â�� �̸�
    WndClass.lpszMenuName = NULL;          // �޴� �̸�
    WndClass.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&WndClass);                              // ������ Ŭ������ ���

    ///////////////////////////////
    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hbrBackground = (HBRUSH)COLOR_WINDOW;        // ȭ���� ������� ĥ��
    WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);                    // ���콺 ����� ǥ����
    WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);          // ���� �� ��ܿ� ��Ÿ���� ������
    WndClass.hInstance = hInstance;                           // ���� ������ �ν��Ͻ�
    WndClass.lpfnWndProc = WndChatProc;                           // �ݹ��Լ� ���
    WndClass.lpszClassName = ChatClass;                        // â�� �̸�
    WndClass.lpszMenuName = NULL;          // �޴� �̸�
    WndClass.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&WndClass);

    ////////////////////////////////////////

    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hbrBackground = (HBRUSH)COLOR_WINDOW;        // ȭ���� ������� ĥ��
    WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);                    // ���콺 ����� ǥ����
    WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);          // ���� �� ��ܿ� ��Ÿ���� ������
    WndClass.hInstance = hInstance;                           // ���� ������ �ν��Ͻ�
    WndClass.lpfnWndProc = WndClientIntroProc;                                    // �ݹ��Լ� ���
    WndClass.lpszClassName = ClientIntroClass;                                // â�� �̸�
    WndClass.lpszMenuName = NULL;          // �޴� �̸�
    WndClass.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&WndClass);





    hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,    // â�� ����
        GetSystemMetrics(SM_CXSCREEN) / 2 - 500, GetSystemMetrics(SM_CYSCREEN) / 2 - 150, 500, 482,
        NULL, (HMENU)NULL, hInstance, NULL);

    ServIntrohWnd = CreateWindow(ServerintroClass, ServerintroClass, WS_OVERLAPPEDWINDOW,      // â�� ����
        GetSystemMetrics(SM_CXSCREEN) / 2 - 500, GetSystemMetrics(SM_CYSCREEN) / 2 - 150, 380, 150,
        NULL, (HMENU)NULL, hInstance, NULL);

    ChathWnd = CreateWindow(ChatClass, ChatClass, WS_OVERLAPPEDWINDOW,    // â�� ����
        GetSystemMetrics(SM_CXSCREEN) / 2 - 500, GetSystemMetrics(SM_CYSCREEN) / 2 - 150, 714, 447,
        NULL, (HMENU)NULL, hInstance, NULL);

    ClientIntroWnd = CreateWindow(ClientIntroClass, ClientIntroClass, WS_OVERLAPPEDWINDOW,    // â�� ����
        GetSystemMetrics(SM_CXSCREEN) / 2 - 500, GetSystemMetrics(SM_CYSCREEN) / 2 - 150, 380, 194,
        NULL, (HMENU)NULL, hInstance, NULL);
    /*
        1��° ���� (lpszClassName): �����ϰ��� �ϴ� ������ Ŭ������ �����ϴ� ���ڿ�
        2��° ���� (lpszWindowName) : ������ Ÿ��Ʋ �ٿ� ��Ÿ�� ���ڿ�
        3��° ���� (dwStyle): ������� �ϴ� �������� ���¸� �����ϴ� �μ�
        4��° ���� ~ 7��° ����(X,Y,nWidth,nHeight) : X, Y ��ǥ�� â�� ũ�� ����
        8��° ���� (hWndParent) : �θ� �����찡 ���� ��� �θ� �������� �ڵ��� ����
        9��° ���� (hmenu) : �����쿡�� ����� �޴��� �ڵ��� ����
        10��° ���� (hinst) : �����츦 ����� ��ü, �� ���α׷��� �ڵ��� ����
        11��° ���� (lpvParam) : CREATSTRUCT��� ����ü�� �����̸�
    */

    //  introhwnd=CreateWindow(

    ShowWindow(hWnd, nCmdShow);      // â�� �����ְ� ���ִ� �Լ�
                                        // ��� �������� �ڵ� ���� �����츦 ȭ�鿡 ����ϴ� ����� ����


    while (GetMessage(&Message, NULL, 0, 0)) // �޽��� ť���� �޽����� �о���δ�.
    {
        TranslateMessage(&Message);  // Ű���� �Է� �޽����� �����Ͽ� ���α׷����� ���� ���� �ֵ��� �Ѵ�.
        DispatchMessage(&Message);    // �޽��� ť���� ���� �޽����� �������� �޽��� ó�� �Լ�(WndProc)�� �����Ѵ�.
    }
    return (int)Message.wParam;   // WM_QIUT �޽����κ��� ���޵� Ż�� �ڵ�(exit code)�̸�
                                        // �� ���α׷��� �����Ų �ü���� ���ϵȴ�.
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    /*
    ���α׷� ó�� ����� ������ â..

    ������ Ŭ���̾�Ʈ ��带 �����ϴ� â�̴�.


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
            "����",
            WS_CHILD | WS_VISIBLE | WS_BORDER | WS_EX_STATICEDGE,
            105, 393, 100, 30,
            hWnd,
            (HMENU)1,
            ((LPCREATESTRUCT)lParam)->hInstance,
            NULL
        );
        clientBtn = CreateWindow(
            "Button",
            "Ŭ���̾�Ʈ",
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
        {//���� ��ư
            isServer = 1;
            ShowWindow(ServIntrohWnd, SW_SHOW); //������Ʈ��â�� ������

            DestroyWindow(hWnd); // ����â �ݱ�
        }
        if (LOWORD(wParam) == 2 && HIWORD(wParam) == BN_CLICKED && (HWND)lParam == clientBtn)
        {//Ŭ���̾�Ʈ ��ư
            isServer = 0;
            ShowWindow(ClientIntroWnd, SW_SHOW); // Ŭ���̾�Ʈ ��Ʈ��â�� ������

            DestroyWindow(hWnd); //����â �ݱ�
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

        ���� ���ý� ���� ��Ʈ��â
        ��Ʈ������ �г����� �޴´�.
        �׸��� �޽��� �޴� �����带 �����Ѵ�.

    */


    int len;
    switch (iMessage) {
    case WM_CREATE:
        nameStatic = CreateWindow("Static",
            TEXT("��ȭ�� �Է� :"),
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
            TEXT("���� ����"),
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
        {//���� ���� ��ư

            GetDlgItemText(hWnd, 3, name, sizeof(name)); //�г��� �޾ƿ���
            len = strlen(name);
            name[len] = 0;

            GetDlgItemText(hWnd, 5, port, sizeof(port)); //��Ʈ��ȣ �޾ƿ���


            openServSock(hWnd); //�������� ����

            hRecvThread = (HANDLE)_beginthreadex(NULL, 0, ThreadReceiv, (LPVOID)hClntSock, 0, (unsigned*)&dwThreadReceivID);//�޽��� �޴� ������ ����.






            send(hClntSock, "������ ���� �Ϸ�", 20, 0);


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
        ä��â
        �������� Ŭ���̾�Ʈ��� �Ѵ� ���� â�� �̿��Ѵ�.
        ������ �̹����� ä�ø޽����� ���� ��ȭ�ϰ� �ȴ�.
        ä��â�� ������ data.dat���Ϸ� �����ϸ鼭 ä��â ����Ʈ �ڽ��� �ѷ��ش�.

        ���α׷� ����� �� ���ϵ� ���� �Ҹ�ȴ�.


    */

    FILE* f; //ä�ó������� ���� ����
    int num; //ä��â ���μ� ���� ����
    char result[1200]; // ���� �޽���
    char msg[1000]; //�Է¸޽���.
    int len;

    char imserver[100];

    HDC hdc;
    RECT rc;
    PAINTSTRUCT ps;
    HBITMAP Background;

    switch (iMessage) {

    case WM_PAINT:

        hdc = BeginPaint(hWnd, &ps);
        /*ä��â�� ���뿡���� �ٸ� �̹����� ������ */

        if (emotion == 1)//�Ф�
            Background = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP3));
        else if (emotion == 2)//����
            Background = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP4));
        else if (emotion == 3)//������
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
        {//�޽��� send ��ư�� ���������.
            GetDlgItemText(hWnd, 444, msg, sizeof(msg)); //�޽��� ��������
            returnEmotion(msg); // �޽����� returnEmotion�Լ��� ������ ����ǥ�� Ȯ��
            len = strlen(msg); //�޽��� ����
            msg[len] = 0; // �޽��� ��.
            sprintf(result, "%s : %s", name, msg);// �г��Ӱ� �޽��� ��ġ��

            len = strlen(result);

            result[len] = 0;
            send(hClntSock, result, len, 0); // �޽��� ������

            f = fopen("data.dat", "a"); // ���Ͽ���

            fprintf(f, "%s ", result); // ���� ���Ͽ� ����
            fclose(f); // ���ϴݱ�

            LoadFile(ChatViewEdit, "data.dat"); // ���� �ҷ��� ä��â�� �Ѹ���
            num = SendMessage(ChatViewEdit, EM_GETLINECOUNT, 0, 0);//���μ� ��������
            SendMessage(ChatViewEdit, EM_LINESCROLL, 0, num); // ��ũ�� ������
            SetWindowText(InputMsgEdit, "");// ä��â ����
            InvalidateRect(hWnd, NULL, TRUE); // �̹��� �ٽ� �׸���
        }
        break;


    case WM_DESTROY: //â�����
        delfile("data.dat"); //ä�ó��� ���� ����
        send(hClntSock, "--------- �������� -----------", 30, 0); // ���������� �޽����� ���濡�� ����.

        closesocket(hClntSock); //���ϴݱ�
        if (isServer == 1)
            closesocket(hServSock); //������� �������ϵ� �ݱ�
        TerminateThread(ThreadReceiv, 0); //������ ����

        PostQuitMessage(0);//��!.
        return 0;
    }
    return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

LRESULT CALLBACK WndClientIntroProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{

    /*

        Ŭ���̾�Ʈ ��Ʈ�� â

        ������ ip�ּҿ� ��Ʈ��ȣ, �г����� �Է¹޴´�.

    */

    int len;
    switch (iMessage) {
    case WM_CREATE:
        nameStatic = CreateWindow("Static",
            TEXT("��ȭ�� �Է� :"),
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
            TEXT("IP �ּ� :"),
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
            TEXT("Ŭ���̾�Ʈ ����"),
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
        {//���� ���� ��ư
            GetDlgItemText(hWnd, 3, name, sizeof(name));

            len = strlen(name);
            name[len] = 0;
            GetDlgItemText(hWnd, 5, port, sizeof(port));
            GetDlgItemText(hWnd, 4, ip, sizeof(ip));
            openClientSock(hWnd);
            hRecvThread = (HANDLE)_beginthreadex(NULL, 0, ThreadReceiv, (LPVOID)hClntSock, 0, (unsigned*)&dwThreadReceivID);
            send(hClntSock, "Ŭ���̾�Ʈ����!!", 20, 0);
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

        ���� ���� �����Լ�

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
    //Ŭ���̾�Ʈ ���� ���� �Լ�

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
    // ���� �ҷ��� ����Ʈ�ڽ��� �Ѹ���
     //dev c++ API �޸��� ���� ����

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
    //���� ���� �Լ� 
    fclose(f);
    CloseHandle(hFile);

    DeleteFile(pszFileName);

    return 1;



}
UINT WINAPI ThreadReceiv(LPVOID lpParam)
{
    //�޽��� �޴� ������ �Լ�
    SOCKET sock;
    int num;

    char message[1000];
    int strLen;

    sock = (SOCKET)lpParam;// ���ڷ� ���� ������ ����
    while (1)
    {
        strLen = recv(sock, message, sizeof(message) - 1, 0);
        if (strLen == -1)
            return 0;
        message[strLen] = 0;

        //�Ʒ� ������ �޽��� ������ send��ư����� �����ϴ� ������ ����ѵ�
        //�̴� ���� ���� �޽����� ä��â�� �����ϰ� , ���� �޴� �޽����� �����ϱ⶧��..

        f = fopen("data.dat", "a");

        fprintf(f, "%s ", message);
        returnEmotion(message);
        fclose(f);

        LoadFile(ChatViewEdit, "data.dat");


        num = SendMessage(ChatViewEdit, EM_GETLINECOUNT, 0, 0);//���μ� ��������
        SendMessage(ChatViewEdit, EM_LINESCROLL, 0, num); // ��ũ�� ������
        InvalidateRect(ChathWnd, NULL, TRUE);

        //SetWindowText(ChatViewEdit,message);

        message[0] = 0;
        //MessageBox (ChathWnd, message, TEXT("accetp() error"), 0);
    }
    return 0;
}

LRESULT CALLBACK ChathWndEnterkeyProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{// ä���Է�â���� ����Ű ó��
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

            num = SendMessage(ChatViewEdit, EM_GETLINECOUNT, 0, 0);//���μ� ��������
            SendMessage(ChatViewEdit, EM_LINESCROLL, 0, num); // ��ũ�� ������


            InvalidateRect(hWnd, NULL, TRUE);

            SetWindowText(hWnd, "");


        }
        break;
    }

    return CallWindowProc(OldEditProc, hWnd, iMessage, wParam, lParam);




}

void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit)
{  //��Ʈ�� �׸��� �Լ�
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

    if (strstr(msg, "��") || strstr(msg, "��"))
        emotion = 1;
    else if (strstr(msg, "��") || strstr(msg, "��"))
        emotion = 2;

    else if (strstr(msg, "����")) //������
        emotion = 3;
    else
        emotion = 0;


}