#include <Windows.h>
#include <stdio.h>
#include "tchar.h"
#include <cstdio>
#include <tlhelp32.h>

int _tmain(int argc, _TCHAR *argv[])
{
    if (argc!=3)
    {
        printf("usage : .\\Program injected.exe path\\to\\dll.dll \n");
        getchar();
        return 1;
    }
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);
    bool process_found_flag = false;

    /* get snapshot of all processes*/
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    /*loop inside the processes snapshot, and check for the exe name*/
    if (Process32First(snapshot, &entry) == TRUE)
    {
        while (Process32Next(snapshot, &entry) == TRUE)
        {
            if (stricmp(entry.szExeFile, argv[1]) == 0)
            {
                process_found_flag = true;

                /* the dll full name*/
                char *buffer = argv[2];
                /* get victim process handle*/
                HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);
                if (process == NULL)
                {
                    printf("Error: %s process couldnt be found.", argv[1]);
                    CloseHandle(snapshot);
                    return 1;
                }

                /* Get address of the LoadLibrary function.
                because Kernel32.dll is almost always loaded on the same memory location for every process, we can assume that it is in the same memory location for the injected process too.
                NOTE! we need to check wether the injected program is X32 or X64, and compile accordingly. */
                LPVOID addr = (LPVOID)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");
                if (addr == NULL)
                {
                    printf("Error: the LoadLibraryA function was not found inside kernel32.dll library.");
                    CloseHandle(process);
                    CloseHandle(snapshot);
                    return 1;
                }

                /*Allocate new memory region inside the process’s address space for the dll name.*/
                LPVOID arg = (LPVOID)VirtualAllocEx(process, NULL, strlen(buffer), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
                if (arg == NULL)
                {
                    printf("Error: the memory could not be allocated inside %s.", argv[1]);
                    CloseHandle(process);
                    CloseHandle(snapshot);
                    return 1;
                }

                /* Write the argument(the dll name) to LoadLibraryA to the process’s newly allocated memory region.*/
                int n = WriteProcessMemory(process, arg, buffer, strlen(buffer), NULL);
                if (n == 0)
                {
                    printf("Error: there was no bytes written to %s address space.", argv[1]);
                    CloseHandle(process);
                    CloseHandle(snapshot);
                    return 1;
                }

                /*Inject our DLL into the process’s address space.*/
                HANDLE threadID = CreateRemoteThread(process, NULL, 0, (LPTHREAD_START_ROUTINE)addr, arg, NULL, NULL);
                if (threadID == NULL)
                {
                    printf("Error: the remote thread could not be created.");
                }
                else
                {
                    printf("Success: the remote thread was successfully created.");
                }

                /*
                 * Close the handle to the process, becuase we’ve already injected the DLL.
                 */
                CloseHandle(process);
            }
        }
    }
    if (!process_found_flag)
        printf("couldnt find proper process");
    CloseHandle(snapshot);
    getchar();
    return 0;
}
