#pragma once

#include <Windows.h>

#define Test(fmt, ...)   Log(fmt, __VA_ARGS__)

void Log(LPCTSTR format, ...);
