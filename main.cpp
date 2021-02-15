#include <cassert>
#include <cstdio>
#include <Windows.h>


int main() {
    setbuf(stdout, nullptr);
    HWND target = FindWindow(nullptr, "Inactive");
    assert(target);
    SendMessage(target, WM_ACTIVATE, 1, (LPARAM)target);
    return 0;
}