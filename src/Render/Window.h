#pragma once

class DirectX12Renderer;
class Window
{
public:
    Window(HINSTANCE hInstance);
    ~Window();

    bool InitMainWindow();
    static void RunMessageLoop();
    void OnResize(UINT width, UINT height);
    HWND GetHandle() const { return mhMainWnd; }

private:
    
   
    
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
   
    HINSTANCE mhAppInst;
    HWND mhMainWnd;
    std::wstring mMainWndCaption = L"DirectX12 Window";

    bool mMinimized = false;
    UINT mWidth = 800;
    UINT mHeight = 600;

    DirectX12Renderer* mRenderer;
};
