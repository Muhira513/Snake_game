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
LPCTSTR lpszClass = TEXT("SNAKE GAME");

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
    int minX = 7 + BS / 2;
    int maxX = 580 - BS / 2;
    int minY = 7 + BS / 2;
    int maxY = 560 - BS / 2; 

    x[0] = minX + rand() % (maxX - minX + 1);
    y[0] = minY + rand() % (maxY - minY + 1);
}

//뱀의 몸 출력
void Snake(HDC hdc, int length, int* x, int* y) {
    for (int i = 0; i < length; i++) {
        HBRUSH osBrush, newBrush;

        if (i == 0) {
            newBrush = CreateSolidBrush(RGB(0, 200, 0));
            osBrush = (HBRUSH)SelectObject(hdc, newBrush);

            Rectangle(hdc, x[i] - BS / 2, y[i] - BS / 2, x[i] + BS / 2, y[i] + BS / 2);

            SelectObject(hdc, osBrush);
            DeleteObject(newBrush);
        }

        else {
            newBrush = CreateSolidBrush(RGB(0, 0, 0));
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
        fColor = RGB(0, 0, 0);
        // 타이머 생성 및 시작
        SetTimer(hWnd, 1, 80, NULL);
        //SetTimer(hWnd, 2, 500, NULL);
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

                food_x[0] = 7 + BS / 2 + rand() % (580 - 7 - BS); // 먹이 위치 랜덤 재 생성
                food_y[0] = 7 + BS / 2 + rand() % (560 - 7 - BS); // 먹이 위치 랜덤 재 생성

                eat++;
                length++;
             }
        }

       /* if (eat == 10)
            start = 0;*/
        /*if (length == 50)
            start = 2;*/

        InvalidateRect(hWnd, NULL, FALSE);
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
          
        }
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

        // 더블 버퍼링 시작(깜빡임 방지)
        HDC memDC = CreateCompatibleDC(hdc);
        HBITMAP memBitmap = CreateCompatibleBitmap(hdc, 600, 600);
        HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);

        // 배경 지우기 (흰색)
        FillRect(memDC, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        // 더블 버퍼링 끝

        HPEN osPen, newPen;
        HBRUSH osBrush, newBrush;

        // 테두리 벽 그리기
        newPen = CreatePen(PS_SOLID, 4, RGB(0, 0, 0));
        osPen = (HPEN)SelectObject(memDC, newPen);

        // 맵 벽
        Rectangle(memDC, 7, 7, 580, 560);

        SelectObject(memDC, osPen);
        DeleteObject(newPen);

        TCHAR tmp[20];
        SetTextColor(memDC, fColor);
        if (start == -1)
            TextOut(memDC, 200, 250, L"----- Press the 's' KEY -----", 28);

        else if (start == 1) {  //s누르고 시작할 때 먹이 랜덤 출력
            newPen = (HPEN)CreatePen(PS_SOLID, 5, RGB(255, 102, 102));
            osPen = (HPEN)SelectObject(memDC, newPen);

            newBrush = (HBRUSH)CreateSolidBrush(RGB(255, 102, 102));
            osBrush = (HBRUSH)SelectObject(memDC, newBrush);

            Ellipse(memDC,
                food_x[0] - BS / 2, food_y[0] - BS / 2,
                food_x[0] + BS / 2, food_y[0] + BS / 2);

            SelectObject(memDC, osPen);
            SelectObject(memDC, osBrush);

            DeleteObject(newPen);
            DeleteObject(newBrush);

            Snake(memDC, length, snake_x, snake_y);
            wsprintf(tmp, L"SCORE: %d", eat);
            TextOut(memDC, 490, 10, tmp, lstrlen(tmp));
        }
        else if (start == 0) {
            TextOut(memDC, 250, 250, L"GAME OVER", 9);
        }
        else if (start == 2) {
            TextOut(memDC, 250, 250, L"TIME  OVER", 9);
        }

        // 메모리 DC를 실제 화면에 출력
        BitBlt(hdc, 0, 0, 600, 600, memDC, 0, 0, SRCCOPY);

        // 정리
        SelectObject(memDC, oldBitmap);
        DeleteObject(memBitmap);
        DeleteDC(memDC);

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
