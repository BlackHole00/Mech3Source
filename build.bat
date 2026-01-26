@echo off

cl /std:c11 /Zi /Fe:build\loader.exe /Fo:build\loader.obj src\loader\__build.c
cl /std:c++14 /Zi /Fe:build\inject.dll /Fo:build\inject.obj /I src\inject /experimental:deterministic "/pathmap:%CD%\src\inject\=" /LD src\inject\__build.cpp Kernel32.lib User32.lib Advapi32.lib

