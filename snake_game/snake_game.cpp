// snake_game.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "snake_game.h"

#include <time.h>
#include <stdlib.h> 

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// --- 스네이크 게임 변수 ---
#define TILE_SIZE 20    // 뱀의 각 세그먼트(타일) 크기 (픽셀)
#define GAME_SPEED 100  // 게임 속도 (밀리초 단위, 100ms마다 업데이트)
#define MAX_SNAKE_LENGTH 100 // 최대 뱀 길이

// 2차원 좌표를 나타내는 구조체
struct Point {
    int x;
    int y;
};

// 뱀의 몸통 위치를 저장하는 배열
Point snake[MAX_SNAKE_LENGTH];
int snake_length = 1;   // 현재 뱀의 길이
Point food;             // 먹이의 위치
enum Direction { UP, DOWN, LEFT, RIGHT };
Direction current_direction = RIGHT; // 현재 뱀이 움직이는 방향
BOOL is_game_over = FALSE;  // 게임 오버 상태

int score = 0; // 점수 변수

// 게임 보드의 크기(타일 단위) - WM_CREATE에서 실제 윈도우 크기에 맞게 계산할 예정
int board_width_tiles = 0;
int board_height_tiles = 0;
// --- 스네이크 게임 변수 끝
// 
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
void InitializeGame(HWND hWnd);

void InitializeGame(HWND hWnd)
{
    // 윈도우 클라이언트 영역의 크기를 가져와 타일 크기로 나눕니다.
    RECT clientRect;
    GetClientRect(hWnd, &clientRect);
    board_width_tiles = clientRect.right / TILE_SIZE;
    board_height_tiles = clientRect.bottom / TILE_SIZE;

    snake_length = 1;
    snake[0].x = board_width_tiles / 2;
    snake[0].y = board_height_tiles / 2;

    current_direction = RIGHT;
    is_game_over = FALSE;
    score = 0;

    // 먹이 생성 (랜덤 위치에 놓는 함수는 나중에 추가)
    // 지금은 임시 위치 지정
    food.x = board_width_tiles / 2 + 5;
    food.y = board_height_tiles / 2;

    // 게임 타이머 설정 (GAME_SPEED마다 WM_TIMER 메시지 발생)
    SetTimer(hWnd, 1, GAME_SPEED, NULL);
}

// 용도 : 뱀이 벽이나 자기 몸에 부딪혔는지 확인합니다.
BOOL CheckCollision(Point head);

BOOL CheckCollision(Point head)
{
    // 1. 벽 충돌 검사
    if (head.x < 0 || head.x >= board_width_tiles ||
        head.y < 0 || head.y >= board_height_tiles)
    {
        return TRUE;
    }
    // 2. 자기 몸 충돌 검사 (머리(0)를 제외한 나머지 몸통과 비교)
    for (int i = 1; i < snake_length; ++i)
    {
        if (head.x == snake[i].x && head.y == snake[i].y)
        {
            return TRUE;
        }
    }
    return FALSE;
}

// 용도 : 뱀이 없는 램덤 위치에 먹이를 생성합니다.
void GenerateFood();

void GenerateFood()
{
    srand((unsigned)time(NULL)); // 랜덤 시드 초기화 (time.h 필요 - 나중에 추가)
    BOOL placed = FALSE;
    while (!placed)
    {
        food.x = rand() % board_width_tiles;
        food.y = rand() % board_height_tiles;

        // 뱀의 몸통 위치와 겹치는지 확인
        BOOL collision = FALSE;
        for (int i = 0; i < snake_length; ++i)
        {
            if (food.x == snake[i].x && food.y == snake[i].y)
            {
                collision = TRUE;
                break;
            }
        }

        if (!collision)
        {
            placed = TRUE;
        }
    }
}

// 용도 : 뱀을 움직이고 게임 상태를 업데이트합니다.
void MoveSnake(HWND hWnd);

void MoveSnake(HWND hWnd)
{
    // 1. 뱀의 꼬리부터 머리 바로 뒤까지 한 칸씩 앞으로 이동
    for (int i = snake_length - 1; i > 0; --i)
    {
        snake[i] = snake[i - 1];
    }
    // 2. 새 머리 위치 계산
    Point newHead = snake[0];
    switch (current_direction)
    {
    case UP:
        newHead.y--;
        break;
    case DOWN:
        newHead.y++;
        break;
    case LEFT:
        newHead.x--;
        break;
    case RIGHT:
        newHead.x++;
        break;
    }

    // 3. 충돌 검사
    if (CheckCollision(newHead))
    {
        is_game_over = TRUE;
        KillTimer(hWnd, 1); // 게임 오버 시 타이머 해제
        InvalidateRect(hWnd, NULL, TRUE); // 화면 전체 갱신 (게임 오버 화면 표시를 위해)
        return;
    }

    // 4. 머리 위치 업데이트
    snake[0] = newHead;

    // 5. 먹이 섭취 검사
    if (snake[0].x == food.x && snake[0].y == food.y)
    {
        // 뱀 길이 늘리기 (새로운 꼬리 위치는 현재 꼬리 위치)
        if (snake_length < MAX_SNAKE_LENGTH)
        {
            // 이전에 꼬리였던 snake[snake_length-1] 위치에 새 꼬리가 생기도록
            // 이 위치는 MoveSnake 시작 시 이미 snake[snake_length]로 옮겨져야 하지만
            // MoveSnake() 시작 시 꼬리 움직임은 무시하고, 여기서 길이를 늘리면 됨.

            // 꼬리가 움직이기 전 위치를 새 몸통으로 사용
            snake[snake_length] = snake[snake_length - 1];
            snake_length++;
        }
        score += 1; // < -- - 점수 증가(예시: 1점)

        GenerateFood(); // 새 먹이 생성
    }

    // 6. 화면 갱신 요청
    InvalidateRect(hWnd, NULL, TRUE);
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        InitializeGame(hWnd);
    }
    break;
    case WM_KEYDOWN:
        if (is_game_over)
        {
            // 게임 오버 상태에서 스페이스바를 누르면 재시작 합니다
            if (wParam == VK_SPACE)
            {
                InitializeGame(hWnd);
                InvalidateRect(hWnd, NULL, TRUE);   // 화면 전체 갱신
            }
        }
        else
        {
            // 방향키 입력 처리 (반대 방향으로는 즉시 회전 불가)
            switch (wParam)
            {
            case VK_UP:
                if (current_direction != DOWN) current_direction = UP;
                break;
            case VK_DOWN:
                if (current_direction != UP) current_direction = DOWN;
                break;
            case VK_LEFT:
                if (current_direction != RIGHT) current_direction = LEFT;
                break;
            case VK_RIGHT:
                if (current_direction != LEFT) current_direction = RIGHT;
                break;
            default:
                break;
            }
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

            // --- 0. 점수 텍스트 표시 ---
            WCHAR szScore[50];
            // 점수를 문자열로 변환합니다.
            wsprintfW(szScore, L"점수: %d", score);

            SetTextColor(hdc, RGB(0, 0, 0)); // 텍스트 색상 검은색
            SetBkMode(hdc, TRANSPARENT);     // 배경 투명

            // 윈도우 클라이언트 영역의 크기를 가져와서 점수를 왼쪽 상단에 표시
            RECT scoreRect = { 10, 10, 300, 30 }; // 대략적인 위치 및 크기

            DrawTextW(hdc, szScore, -1, &scoreRect, DT_LEFT | DT_TOP);

            // 1. 먹이 그리기 (빨간색)
            HBRUSH hBrushFood = CreateSolidBrush(RGB(255, 0, 0)); // 빨간색 브러시
            HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrushFood);

            RECT foodRect = { food.x * TILE_SIZE, food.y * TILE_SIZE,
                              (food.x + 1) * TILE_SIZE, (food.y + 1) * TILE_SIZE };
            FillRect(hdc, &foodRect, hBrushFood);

            SelectObject(hdc, hOldBrush);   // 이전 브러시 복원
            DeleteObject(hBrushFood);   // 브러시 삭제

            // 2. 뱀 그리기 (초록색)
            HBRUSH hBrushSnake = CreateSolidBrush(RGB(0, 255, 0)); // 초록색 브러시
            hOldBrush = (HBRUSH)SelectObject(hdc, hBrushSnake);

            for (int i = 0; i < snake_length; ++i)
            {
                RECT snakeRect = { snake[i].x * TILE_SIZE, snake[i].y * TILE_SIZE,
                                   (snake[i].x + 1) * TILE_SIZE, (snake[i].y + 1) * TILE_SIZE };
                FillRect(hdc, &snakeRect, hBrushSnake);
            }

            SelectObject(hdc, hOldBrush);   // 이전 브러시 복원
            DeleteObject(hBrushSnake);    // 브러시 삭제

            // 3. 게임 오버 텍스트 표시
            if (is_game_over)
            {
                SetTextColor(hdc, RGB(255, 0, 0));  // 텍스트 색상 빨간색
                SetBkMode(hdc, TRANSPARENT);    // 배경 투명

                RECT clientRect;
                GetClientRect(hWnd, &clientRect);
                DrawTextW(hdc, L"GAME OVER\nPress SPACE to Restart", -1, &clientRect,
                    DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            }

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_TIMER:
        if (!is_game_over)
        {
            // 뱀 움직이는 함수 (다음 단계에서 구현)
            MoveSnake(hWnd);
        }
        break;
    case WM_DESTROY:
        KillTimer(hWnd, 1);
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
