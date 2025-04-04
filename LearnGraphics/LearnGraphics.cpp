﻿// LearnGraphics.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include <iostream>
#include <memory>
#include <Windows.h>

#include "framework.h"
#include "LearnGraphics.h"
#include "Example.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance, WNDCLASSEXW& wcex);
HWND                InitInstance(HINSTANCE, int, int, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    const int width = 800, height = 480;

    WNDCLASSEXW wcex;

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LEARNGRAPHICS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance, wcex);

    // 애플리케이션 초기화를 수행합니다:
    HWND hwnd = InitInstance(hInstance, nCmdShow, width, height);
    if (!hwnd)
    {
        return FALSE;
    }

    // 예제 클래스의 인스턴스 생성
    auto example = std::make_unique<hlab::Example>(hwnd, width, height);

    // imgui 초기화
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.DisplaySize = ImVec2(width, height);
    ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplDX11_Init(example->device, example->deviceContext);
    ImGui_ImplWin32_Init(hwnd);


    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LEARNGRAPHICS));
    MSG msg = {};

    // 기본 메시지 루프입니다:
    /*while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }*/

    while (WM_QUIT != msg.message) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            // Start the Dear ImGui frame
            // ImGui_ImplDX11_NewFrame();
            // ImGui_ImplWin32_NewFrame();
            // ImGui::NewFrame();
            // ImGui::Begin("Scene Control");
            // ImGui::End();
            // ImGui::Render();

            example->Update();
            example->Render();

            // ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

            // switch the back buffer and the front buffer
            example->swapChain->Present(1, 0);
        }
    }

    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    example->Clean();
    DestroyWindow(hwnd);
    UnregisterClassW(wcex.lpszClassName, wcex.hInstance);

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance, WNDCLASSEXW& wcex)
{
    

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LEARNGRAPHICS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LEARNGRAPHICS);
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
HWND InitInstance(HINSTANCE hInstance, int nCmdShow, int width, int height)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.
   

   RECT wr = { 0, 0, width, height };

   // 툴바 크기를 포함한 넓이로 RECT 의 크기를 재계산 해줌.
   AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, wr.right - wr.left, wr.bottom - wr.top, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
        DWORD error = GetLastError();
        WCHAR errorMessage[256];
        FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            errorMessage, sizeof(errorMessage) / sizeof(WCHAR), NULL);

        OutputDebugStringW(L"CreateWindowW failed: ");
        OutputDebugStringW(errorMessage);
        // 오류 처리 코드 (예: 메시지 박스 출력, 로그 기록 등)
        return NULL;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return hWnd;
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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
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

    case WM_SIZE:
        return 0;

    /*
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // ALT 키로 메뉴 비활성화
            return 0;
        break;
    */

    case WM_MOUSEMOVE:
        std::cout << "Mouse " << LOWORD(lParam) << ", " << HIWORD(lParam) << std::endl;
        break;

    case WM_LBUTTONUP:
        break;

    case WM_RBUTTONUP:
        break;

    case WM_KEYDOWN:
        break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            EndPaint(hWnd, &ps);
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
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
