#include "stdafx.h"
#include "AguraApplication.h"

#include <GdiPlus.h>
#include <comutil.h>

#pragma comment(lib, "gdiplus")
#pragma comment(lib, "comsuppw.lib")

ULONG_PTR gdiplusToken;

/************************************************************
 *	@brief		Constructor
 *	@retval		None
 ************************************************************/
CAguraApplication::CAguraApplication()
{
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}

/************************************************************
 *	@brief		Destructor
 *	@retval		None
 ************************************************************/
CAguraApplication::~CAguraApplication()
{
	Gdiplus::GdiplusShutdown(gdiplusToken);
}
