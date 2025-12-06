// Snake_game.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Snake_game.h"
#include <iostream>
#include <ctime>  // time()
using namespace std;

//#define MAX_LOADSTRING 100

#define BSIZE 20 // 음식 판정
#define BS 25 // 몸 사이즈

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
LPCTSTR lpszClass = TEXT("SNAKE GAME"); // 제목 표지

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
    HWND hwnd;
    MSG msg;
    WNDCLASS WndClass;
    WndClass.style = CS_HREDRAW | CS_VREDRAW;   // 창 스타일
    WndClass.lpfnWndProc = WndProc;
    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hInstance = hInstance; // 윈도우즈 운영체제에서 실행되는 프로그램들을 구별하기 위한 ID값
    WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);   // 저장된 아이콘을 메모리로 불러오는 함수
    WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);     // 마우스 커서, 사용자 정의 커서 등 소스를 메모리로 불러오기 / 해당 커서 핸들을 반환하여 화면에 표시하도록 준비
    WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);   // 윈도우(배경) 하얀색 배경 불러오기
    WndClass.lpszMenuName = NULL;   // 메뉴바 추가 안함 
    WndClass.lpszClassName = lpszClass; 
    RegisterClass(&WndClass);  // 윈도우 클래스(설계도) 시스템 등록

    hwnd = CreateWindow(lpszClass,
        lpszClass,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        600,    // 600 x 600
        600,    // 600 x 600
        NULL,
        NULL,
        hInstance,
        NULL
    );  //CreateWindow 자식창 생성 : 겹쳐진 창, 팝업 창 등(창 클래스, 창 제목, 창 스타일)
    ShowWindow(hwnd, nCmdShow); // 윈도우 창 표시(결정) / CreateWindow : 윈도우 창 생성
    UpdateWindow(hwnd); // 윈도우 창 즉시 화면 그리기

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

int Length(int food, int snake) {   // 음식과 뱀 사이 거리 판정
    if (food >= snake)
        return (food - snake);  // 120 - 100 = 20
    else
        return (food - snake) * (-1);   // 80 - 100 = -20 , -20 * -1 = 20   // 서로 간에 거리가 20 정도 차이
}

bool correct(int food_x, int food_y, int snake_x, int snake_y) {    

    if (Length(food_x, snake_x) < BSIZE && Length(food_y, snake_y) < BSIZE) {   // BSIZE 충돌 범위로 사용, 

        return TRUE;    // 뱀 판정 범위와 음식 판정범위가 서로 겹치면 먹는걸로 판정
    }
    else {
        return FALSE;   // 반대로 아닐시
    }
}

// 음식 랜덤 생성
void Food_Random(int* x, int* y) {
    int minX = 7 + BS / 2;  // 원의 반지름/2
    int maxX = 580 - BS / 2;    // 오른쪽 580 벽 근처에 끼지 않도록 
    int minY = 7 + BS / 2;  
    int maxY = 560 - BS / 2; 

    x[0] = minX + rand() % (maxX - minX + 1);   // 음식의 중심 X좌표에서 벽 범위 안에서 랜덤으로 음식을 생성 
    y[0] = minY + rand() % (maxY - minY + 1);   // 음식의 중심 y좌표에서 벽 범위 안에서 랜덤으로 음식을 생성 
}

//뱀의 몸 출력
void Snake(HDC hdc, int length, int* x, int* y) {
    for (int i = 0; i < length; i++) {  // 음식을 먹을때 마다 뱀의 몸길이 1씩 증가
        HBRUSH osBrush, newBrush;

        if (i == 0) {
            newBrush = CreateSolidBrush(RGB(0, 200, 0));    // 뱀 앞에 머리 부분을 초록색으로 표현
            osBrush = (HBRUSH)SelectObject(hdc, newBrush);

            Rectangle(hdc, x[i] - BS / 2, y[i] - BS / 2, x[i] + BS / 2, y[i] + BS / 2);

            SelectObject(hdc, osBrush);
            DeleteObject(newBrush);
        }

        else {
            newBrush = CreateSolidBrush(RGB(0, 0, 0));  // 뱀 몸통, 꼬리 부분을 검은색 사각형
            osBrush = (HBRUSH)SelectObject(hdc, newBrush);

            Rectangle(hdc, x[i] - BS / 2, y[i] - BS / 2, x[i] + BS / 2, y[i] + BS / 2);

            SelectObject(hdc, osBrush);
            DeleteObject(newBrush);
        }
    }
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
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
        srand((unsigned)time(NULL));
        fColor = RGB(0, 0, 0);  // 뱀의 테두리 색, 선 색, 글자 색 등 검은색으로 표시
        // 타이머 생성 및 시작
        SetTimer(hWnd, 1, 80, NULL);    // 뱀의 속도 80ms으로 (100은 너무 느리고 70은 조금 빠른 느낌이 듬)
        //SetTimer(hWnd, 2, 500, NULL);
        snake_x[0] = snake_y[0] = 100;  // 뱀의 머리 좌표 100 100

        eat = 0;    // 음식 (시작 뱀이 음식을 안 먹은 상태로 시작하기 위해서)
        start = -1; // (게임 준비 상태)
        length = 1; // 시작시 머리 1칸
        Food_Random(food_x, food_y);    // 음식 랜덤 위치
        MOVE = 2;   // 시작 위치 왼쪽 위치에서 오른쪽으로 가는걸로 시작 (0 : 위, 1 : 아래, 2 : 왼쪽, 3 : 오른쪽) 
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
        int j;
        // 게임이 시작했을 때 타이머 1번(뱀 움직임용 타이머)
        if (wParam == 1 && start == 1) {
            if (MOVE == 1) {
                for (i = length - 1; i > 0; i--) {
                    snake_x[i] = snake_x[i - 1];
                    snake_y[i] = snake_y[i - 1];
                }
                snake_x[0] -= BS;   //뱀의 머리 x좌표를 BS만큼 -
            }
            if (MOVE == 2) {
                for (i = length - 1; i > 0; i--) {
                    snake_x[i] = snake_x[i - 1];
                    snake_y[i] = snake_y[i - 1];
                }
                snake_x[0] += BS;    //뱀의 머리 x좌표를 BS만큼 +
            }
            if (MOVE == 3) {
                for (i = length - 1; i > 0; i--) {
                    snake_x[i] = snake_x[i - 1];
                    snake_y[i] = snake_y[i - 1];
                }
                snake_y[0] -= BS;    //뱀의 머리 y좌표를 BS만큼 -
            }
            if (MOVE == 4) {
                for (i = length - 1; i > 0; i--) {
                    snake_x[i] = snake_x[i - 1];
                    snake_y[i] = snake_y[i - 1];
                }
                snake_y[0] += BS;    //뱀의 머리 y좌표를 BS만큼 +
            }
            //  몸통 충돌 체크 추가 
            for (int j = 1; j < length; j++) {
                if (snake_x[0] == snake_x[j] && snake_y[0] == snake_y[j]) {
                    start = 0;   // 게임 오버
                }
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
       /* if (wParam == 2 && start == 1) {
            length++;
        }*/

        if(start == 1) {
            if (correct(food_x[0], food_y[0], snake_x[0], snake_y[0])) {

                food_x[0] = 7 + BS / 2 + rand() % (580 - 7 - BS); // 먹이 위치 랜덤 재 생성  // 먹이가 벽에 끼는걸 방지 하기 위해서 600 x 600 사이즈 보단 살짝 작은 범위에 생성
                food_y[0] = 7 + BS / 2 + rand() % (560 - 7 - BS); // 먹이 위치 랜덤 재 생성  // 먹이가 벽에 끼는걸 방지 하기 위해서 600 x 600 사이즈 보단 살짝 작은 범위에 생성

                eat++;
                length++;
             }
        }

        if (eat == 130)
            start = 0;    // 먹이 개수 제한을 안해서 주석 처리 
        /*if (length == 130)  // 130점 달성하면 게임 클리어 // 원래는 점수 무한대로 할려고 했는데 40점 부터 메모리 누수 발생해 500점으로 되는 버그 발생
            start = 2;*/

        InvalidateRect(hWnd, NULL, FALSE);  // 깜빡임 현상 때문에 TRUE -> FALSE 다시 그리는걸 금지 시키고 대신에 메모리 DC 가상화면을 추가함 코드 줄: 252 ~ 254
        break;
    }
    break;

    case WM_CHAR:   //s누르면 시작 e누르면 종료
        if (wParam == 115) {    // 게임시작 ASCII : 115 = s
            start = 1;
        }
        else if (wParam == 101) {   // 게임 종료 ASCII : 101 = e
            PostMessage(hWnd, WM_DESTROY, 0, 0);
        }
        break;
    /*case WM_COMMAND:
        {
          
        }
        break;*/
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

        // 더블 버퍼링 시작(깜빡임 방지)
        HDC memDC = CreateCompatibleDC(hdc);    // 메모리 두수를 방지 하기 위해 메모리 DC를 생성한다
        HBITMAP memBitmap = CreateCompatibleBitmap(hdc, 600, 600);  // 비트맵 생성
        HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);    // 비트맵을 memDC에 선택

        // 배경 지우기 (흰색)
        FillRect(memDC, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));   // (메모리DC) 흰색 배경을 지우고 다시 그리는 과정
        // 더블 버퍼링 끝

        HPEN osPen, newPen; // 펜 함수
        HBRUSH osBrush, newBrush;   // 브러쉬 함수

        // 테두리 벽 그리기
        newPen = CreatePen(PS_SOLID, 4, RGB(0, 0, 0));  // 테두리 벽 검은색 두깨 4
        osPen = (HPEN)SelectObject(memDC, newPen); // 테두리 벽 적용

        // 맵 벽
        Rectangle(memDC, 7, 7, 580, 560); // 600 x 600 맵 사이즈랑 근접하게 거대한 사각형을 생성

        SelectObject(memDC, osPen); // 사용한 맵 벽 펜 회수
        DeleteObject(newPen);   // 사용한 맵 벽 펜 삭제

        TCHAR tmp[20];
        SetTextColor(memDC, fColor);
        if (start == -1)
            TextOut(memDC, 220, 250, L"----- start : s, exit : e -----", 28); // s키를 눌러서 게임을 시작하라는 문구다

        else if (start == 1) {  //s누르고 시작할 때 먹이 랜덤 출력
            newPen = (HPEN)CreatePen(PS_SOLID, 5, RGB(255, 102, 102));  // 브러쉬 생성
            osPen = (HPEN)SelectObject(memDC, newPen);  // 브러쉬 적용

            newBrush = (HBRUSH)CreateSolidBrush(RGB(255, 102, 102));    // 브러쉬 생성
            osBrush = (HBRUSH)SelectObject(memDC, newBrush); // 브러쉬 적용

            Ellipse(memDC,
                food_x[0] - BS / 2, food_y[0] - BS / 2,
                food_x[0] + BS / 2, food_y[0] + BS / 2);

            SelectObject(memDC, osPen); // memDc, osPen 회수
            SelectObject(memDC, osBrush); // memDc, osBrush 회수

            DeleteObject(newPen);   // 사용한 newPen 삭제
            DeleteObject(newBrush); // 사용한 new Brush 삭제

            Snake(memDC, length, snake_x, snake_y); 
            wsprintf(tmp, L"SCORE: %d", eat);   // 점수 판
            TextOut(memDC, 490, 10, tmp, lstrlen(tmp)); // 점수판 위치 490 길이 높이 10
        }
        else if (start == 0) {
            TextOut(memDC, 250, 250, L"GAME OVER", 9);  // 글씨 크기 9
        }
        else if (start == 2) {
            TextOut(memDC, 250, 250, L"GAME CLEAR", 9); // 130 점 얻을시 게임 클리어 표시
        }

        // 메모리 DC를 실제 화면에 출력
        BitBlt(hdc, 0, 0, 600, 600, memDC, 0, 0, SRCCOPY);  // 더블 버퍼링, 가상화면을 덮어쓰여 깜빡 거림을 방지

        // 정리
        SelectObject(memDC, oldBitmap); // 새로 생성한 비트맵 회수
        DeleteObject(memBitmap);    // 사용한 비트맵 삭제
        DeleteDC(memDC);    // 가상화면 삭제

        EndPaint(hWnd, &ps);
        //ReleaseDC(hWnd, hdc);
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
