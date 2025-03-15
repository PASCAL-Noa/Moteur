#include "pch.h"
#include "Window.h"

#include "DirectX12Renderer.h"

Window::Window(HINSTANCE hInstance) : mhAppInst(hInstance), mhMainWnd(nullptr) {}

Window::~Window() = default;


bool Window::InitMainWindow()
{
    WNDCLASS wc = {};
    wc.lpfnWndProc = Window::WndProc;
    wc.hInstance = mhAppInst;
    wc.lpszClassName = L"MainWnd";

    if (!RegisterClass(&wc))
    {
        MessageBox(nullptr, L"Failed to register window class", nullptr, 0);
        return false;
    }

    mhMainWnd = CreateWindow(L"MainWnd", mMainWndCaption.c_str(),
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, mWidth, mHeight, nullptr, nullptr, mhAppInst, this);

    if (!mhMainWnd)
    {
        MessageBox(nullptr, L"Failed to create window", nullptr, 0);
        return false;
    }

    ShowWindow(mhMainWnd, SW_SHOW);
    UpdateWindow(mhMainWnd);
    return true;
}

void Window::RunMessageLoop()
{
    MSG msg = {};
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}

void Window::OnResize(UINT width, UINT height)
{
    if (width == 0 || height == 0)
        return;

    mWidth = width;
    mHeight = height;

    if (mRenderer)
    {
        mRenderer->ResizeBuffers(width, height);
    }
}

LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    Window* pThis = nullptr;

    if (msg == WM_CREATE)
    {
        CREATESTRUCT* pCreate;
        pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        pThis = static_cast<Window*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
    }
    else
    {
        pThis = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (pThis)
    {
        switch (msg)
        {
        case WM_SIZE:
            if (wParam == SIZE_MINIMIZED)
                pThis->mMinimized = true;
            else
            {
                pThis->mMinimized = false;
                pThis->OnResize(LOWORD(lParam), HIWORD(lParam));
            }
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default: ;
        }
    }
    
    return DefWindowProc(hwnd, msg, wParam, lParam);
}
