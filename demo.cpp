#include <cassert>
#define OEMRESOURCE  // For OCR_NORMAL
#include <Windows.h>

LRESULT CALLBACK winProc(HWND hwnd, UINT type, WPARAM wparam, LPARAM lparam)
{
    switch(type)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return CallWindowProc(DefWindowProc, hwnd, type, wparam, lparam);
    }
    return 0;
}

void registerClass(const char *className, HICON ico=nullptr)
{
    WNDCLASSEX cls;
    LPTSTR cursorImageName = MAKEINTRESOURCE(OCR_NORMAL);
    HANDLE cursorImage = LoadImage(0, cursorImageName, IMAGE_CURSOR, 0, 0, LR_SHARED|LR_DEFAULTSIZE);
    memset(&cls, 0, sizeof(cls));
    cls.cbSize = sizeof(cls);
    cls.lpfnWndProc = winProc;
    cls.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    cls.lpszClassName = className;
    cls.hIcon = ico;
    cls.hCursor = (HCURSOR)cursorImage;
    RegisterClassEx(&cls);
}

int WindowMessageLoop(HWND hWnd)
{
    BOOL bRet;
    MSG msg;
    while( (bRet = GetMessage(&msg, hWnd, 0, 0)) )
    {
        if(bRet==-1)
        {
            MessageBox(hWnd, "Error Retrieving window messages!", "Error", MB_ICONERROR);
            return -1;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

int main(int argc, char *argv[]) {
    HICON ico = ExtractIcon(0, "Shell32.dll", 2);
    registerClass("exampleWindowCls", ico);

    HWND win = CreateWindow(
        "exampleWindowCls",
        "Example Window",
        WS_SYSMENU|WS_VISIBLE,
        100,
        100,
        500,
        500,
        NULL,
        NULL,
        NULL,
        NULL
    );
    assert(win);

    HWND txt = CreateWindow(
        "STATIC",
        "Example Window",
        WS_CHILD|WS_VISIBLE,
        5,
        5,
        483,
        460,
        win,
        NULL,
        NULL,
        NULL
    );
    assert(txt);

    int exitCode = WindowMessageLoop(win);
    return exitCode;
}