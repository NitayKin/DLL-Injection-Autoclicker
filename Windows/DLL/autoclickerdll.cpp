//#include "stdafx.h"
#include <stdio.h>
#include <windows.h>

INT APIENTRY DllMain(HMODULE hDLL, DWORD Reason, LPVOID Reserved)
{
    //FILE *file;
    //fopen_s(&file, "C:/Users/nitay/Desktop/DLL-Injection-Autoclicker/checking.txt", "a+");
    INPUT ip;
    UINT uSent;
    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = 0;
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;

    switch (Reason)
    {
    case DLL_PROCESS_ATTACH:
        while (true)
        {
            Sleep(5000);
            // Press the key
            ip.ki.wVk = 0x5B;
            ip.ki.dwFlags = 0; // 0 for key press
            uSent = SendInput(1, &ip, sizeof(INPUT));
            // Press the key
            ip.ki.wVk = 0x44;
            ip.ki.dwFlags = 0; // 0 for key press
            uSent = SendInput(1, &ip, sizeof(INPUT));
            // Release the key
            ip.ki.wVk = 0x5B;
            ip.ki.dwFlags = KEYEVENTF_KEYUP;
            SendInput(1, &ip, sizeof(INPUT));
            // Release the key
            ip.ki.wVk = 0x44;
            ip.ki.dwFlags = KEYEVENTF_KEYUP;
            SendInput(1, &ip, sizeof(INPUT));
            /*
            if (uSent != 1)
                fprintf(file, "SendInput failed: 0x%x\n", HRESULT_FROM_WIN32(GetLastError()));
            else
                fprintf(file, "good");

            fclose(file);
            */
        }

        break;
    case DLL_PROCESS_DETACH:
        //fprintf(file, "DLL detach function called.");
        //fclose(file);
        break;
    case DLL_THREAD_ATTACH:
        //fprintf(file, "DLL thread attach function called.");
        //fclose(file);
        break;
    case DLL_THREAD_DETACH:
        //fprintf(file, "DLL thread detach function called.");
        //fclose(file);
        break;
    }
}