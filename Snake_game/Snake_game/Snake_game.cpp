// Snake_game.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Snake_game.h"
#include <iostream>
using namespace std;

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SNAKEGAME, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SNAKEGAME));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SNAKEGAME));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SNAKEGAME);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
#define BSIZE 15 //
#define BS 14 // 몸 사이즈

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
LPCSTR lpszClass = TEXT(L"SNAKE GAME");

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
    HWND hwnd;
    MSG msg;
    WNDCLASS WndClass;
    WndClass.style = CS_HREDRAW | CS_VREDRAW;
    WndClass.lpfnWndProc = WndProc;
    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hInstance = hInstance;
    WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    WndClass.lpszMenuName = NULL;
    WndClass.lpszClassName = lpszClass;
    RegisterClass(&WndClass);

    hwnd = CreateWindow(lpszClass,
        lpszClass,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        600,
        600,
        NULL,
        NULL,
        hInstance,
        NULL
    );
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

int Length(int food, int snake) {
    if (food >= snake)
        return (food - snake);
    else
        return (food - snake) * (-1);
}

bool correct(int food_x, int food_y, int snake_x, int snake_y) {

    if (Length(food_x, snake_x) < BSIZE && Length(food_y, snake_y) < BSIZE) {

        return TRUE;
    }
    else {
        return FALSE;
    }
}

// 음식 랜덤 생성
void Food_Random(int* x, int* y) {
    for (int i = 0; i < 10; i++) {
        x[i] = (rand() % 500);
        y[i] = (rand() % 500);
    }
}

//뱀의 몸 출력
void Snake(HDC hdc, int length, int* x, int* y) {
    for (int i = 0; i < length; i++)
        Rectangle(hdc, x[i] - BS / 2, y[i] - BS / 2, x[i] + BS / 2, y[i] + BS / 2);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    static COLORREF fColor;
    static int food_x[10];
    static int food_y[10];
    static int snake_x[50];
    static int snake_y[50];
    static int MOVE;
    static int start, length, eat;
    static char ch;

    switch (message)
    {
    case WM_CREATE:
    {
        fColor = RGB(0, 0, 0);
        // 타이머 생성 및 시작
        SetTimer(hWnd, 1, 300, NULL);
        SetTimer(hWnd, 2, 500, NULL);
        snake_x[0] = snake_y[0] = 100;

        eat = 0;
        start = -1;
        length = 1;
        Food_Random(food_x, food_y);
        MOVE = 2;
    }
    break;
    case WM_KEYDOWN:
    {
        if (wParam == VK_LEFT)
            MOVE = 1;
        else if (wParam == VK_RIGHT)
            MOVE = 2;
        else if (wParam == VK_UP)
            MOVE = 3;
        else if (wParam == VK_DOWN)
            MOVE = 4;
    }
    break;
    case WM_TIMER:
    {
        int i;
        // 게임이 시작했을 때 타이머 1번(뱀 움직임용 타이머)
        if (wParam == 1 && start == 1) {
            if (MOVE == 1) {
                for (i = 0; i < length; i++) {
                    snake_x[length - i] = snake_x[length - i - 1];
                    snake_y[length - i] = snake_y[length - i - 1];
                }
                snake_x[0] -= BS;   //뱀의 머리 x좌표를 BS만큼 -
            }
            if (MOVE == 2) {
                for (i = 0; i < length; i++) {
                    snake_x[length - i] = snake_x[length - i - 1];
                    snake_y[length - i] = snake_y[length - i - 1];
                }
                snake_x[0] += BS;    //뱀의 머리 x좌표를 BS만큼 +
            }
            if (MOVE == 3) {
                for (i = 0; i < length; i++) {
                    snake_x[length - i] = snake_x[length - i - 1];
                    snake_y[length - i] = snake_y[length - i - 1];
                }
                snake_y[0] -= BS;    //뱀의 머리 y좌표를 BS만큼 -
            }
            if (MOVE == 4) {
                for (i = 0; i < length; i++) {
                    snake_x[length - i] = snake_x[length - i - 1];
                    snake_y[length - i] = snake_y[length - i - 1];
                }
                snake_y[0] += BS;    //뱀의 머리 y좌표를 BS만큼 +
            }

            if (snake_x[0] < 7) // 벽에 부딫혔을 때 게임 종료
                start = 0;
            else if (snake_x[0] > 593) // 벽에 부딫혔을 때 게임 종료
                start = 0;
            else if (snake_y[0] < 7) // 벽에 부딫혔을 때 게임 종료
                start = 0;
            else if (snake_y[0] > 555) // 벽에 부딫혔을 때 게임 종료
                start = 0;
        }
        if (wParam == 2 && start == 1) {
            length++;
        }

        for (int i = 0; i < 10; i++) {
            if (correct(food_x[i], food_y[i], snake_x[0], snake_y[0])) {
                food_x[i] = food_y[i] = 1000;
                eat++;
            }
        }

        if (eat == 9)
            start = 0;
        if (length == 50)
            start = 2;

        InvalidateRect(hWnd, NULL, TRUE);
        break;
    }
    break;

    case WM_CHAR:   //s누르면 시작 e누르면 종료
        if (wParam == 115) {
            start = 1;
        }
        else if (wParam == 101) {
            start = 0;
        }
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

            TCHAR tmp[20];
            SetTextColor(hdc, fColor);
            if (start == -1)
                TextOut(hdc, 200, 250, L"----- Press the 's' KEY -----", 28);

            else if (start == 1) {  //s누르고 시작할 때 먹이 랜덤 출력
                for (int i = 0; i < 9; i++) {
                    TextOut(hdc, food_x[i], food_y[i], L"T", 1);
                }
                Snake(hdc, length, snake_x, snake_y);
                wsprintf(tmp, L"SCORE: %d", eat);
                TextOut(hdc, 490, 10, tmp, lstrlen(tmp));
            }
            else if (start == 0) {
                TextOut(hdc, 250, 250, L"GAME OVER", 9);
            }
            else if (start == 2) {
                TextOut(hdc, 250, 250, L"TIME  OVER", 9);
            }
            EndPaint(hWnd, &ps);
            ReleaseDC(hWnd, hdc);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
