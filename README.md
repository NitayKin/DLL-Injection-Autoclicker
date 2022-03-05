# DLL-Injection-Autoclicker
injecting Autoclicking to other program

Windows:

how to compile:
I used Visual Studio 2022 Developer Command Prompt v17.0.1  , with Microsoft (R) C/C++ Optimizing Compiler Version 19.30.30705 for x86/x64

To compile the dll file: "cl /LD autoclickerdll.cpp /link user32.lib"
To compile the program: "cl XXX.cpp"

IMPORTANT NOTE: the dll and program should be compiled for 32/64-bit accordingly to the injected process.

how to run:
.\Program injected.exe path\to\dll.dll
