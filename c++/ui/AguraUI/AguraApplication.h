#pragma once

#include <GdiPlus.h>
#include <comutil.h>

#pragma comment(lib, "gdiplus")
#pragma comment(lib, "comsuppw.lib")

using namespace Gdiplus;

class CAguraApplication : public CWinApp
{
public:
	CAguraApplication();
	~CAguraApplication();
};
