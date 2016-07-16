#include "stdafx.h"
#include "AguraApplication.h"

ULONG_PTR gdiplusToken;

CAguraApplication::CAguraApplication()
{
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}

CAguraApplication::~CAguraApplication()
{
	GdiplusShutdown(gdiplusToken);
}
