#include <string>
#include <vector>
#include <cassert>
#include <cstdio>
#include <Windows.h>
#include <psapi.h>

const std::string getWindowModuleName(HWND hwnd);
BOOL windowSearchEnumWindows(HWND hwnd, LPARAM windowSearchPtr);

class WindowSearch {
public:
    typedef std::vector<HWND> WindowVector;

    WindowSearch(const std::string &moduleName) : moduleName(moduleName) {
        BOOL bRet = EnumWindows(windowSearchEnumWindows, (LPARAM)this);
        assert(bRet);
    }

    const WindowVector &results() {
        return enumResults;
    }

protected:
    const std::string moduleName;
    WindowVector enumResults;

private:
    friend windowSearchEnumWindows(HWND hwnd, LPARAM windowSearchPtr);

    BOOL enumWindowsCallBack(HWND hwnd) {
        auto windowModule = getWindowModuleName(hwnd);
        // printf("(%p)%s != %s\n", hwnd, windowModule.c_str(), moduleName.c_str());
        if(windowModule == moduleName) {
            enumResults.push_back(hwnd);
        }
        return TRUE;
    }
};

const std::string getWindowModuleName(HWND hwnd) {
    constexpr DWORD desiredAccess = PROCESS_QUERY_LIMITED_INFORMATION;
    constexpr size_t bufferSize = 1024;
    char nameBuffer[bufferSize];

    DWORD pid = GetWindowThreadProcessId(hwnd, nullptr);
    HANDLE hProcess = OpenProcess(desiredAccess, FALSE, pid);
    // printf("%p, %x, %p, %i\n", (void*)hwnd, (int)pid, (void*)hProcess, (int)GetLastError());
    
    DWORD dRet = 0;
    if(hProcess) {
        dRet = GetProcessImageFileNameA(hProcess, nameBuffer, bufferSize - 1);
        CloseHandle(hProcess);
    }
    nameBuffer[dRet] = 0;
    return std::string(nameBuffer);
}

BOOL windowSearchEnumWindows(HWND hwnd, LPARAM windowSearchPtr) {
    WindowSearch *windowSearch = (WindowSearch*)windowSearchPtr;
    return windowSearch->enumWindowsCallBack(hwnd);
}

int main() {
    setbuf(stdout, nullptr);
    WindowSearch search("explorer.exe");
    auto results = search.results();
    for(auto result : results) {
        printf("%p\n", result);
    }
    return 0;
}