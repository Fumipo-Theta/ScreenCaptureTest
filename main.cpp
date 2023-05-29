#include "pch.h"
#include "ScreenCaptureTest.h"

#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <iostream>

#include <format>



HWND g_hwnd = NULL;

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    DWORD lpdwProcessId;
    GetWindowThreadProcessId(hwnd, &lpdwProcessId);
    if (lpdwProcessId == lParam)
    {
        std::cout << "Window handle: " << hwnd << std::endl;
        g_hwnd = hwnd;
        return FALSE;
    }
    return TRUE;
}

void GetWindowHandleByPID(DWORD processId) {
    EnumWindows(EnumWindowsProc, processId);
}

DWORD GetPIDByExeName(const std::wstring& exeName) {
    DWORD pid = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32 pe32;
        pe32.dwSize = sizeof(PROCESSENTRY32);
        if (Process32First(hSnap, &pe32))
        {
            do
            {
                if (exeName == pe32.szExeFile)
                {
                    pid = pe32.th32ProcessID;
                    break;
                }
            } while (Process32Next(hSnap, &pe32));
        }
        CloseHandle(hSnap);
    }
    return pid;
}

int main(int argc, char *argv)
{
    //TestGDI();
    //TestDesktopDuplication();
    std::wstring executable = L"PCDataCom-Win64-Shipping.exe";
    GetWindowHandleByPID(GetPIDByExeName(executable));
    HWND target = g_hwnd;

    TestGraphicsCapture(target);
}
