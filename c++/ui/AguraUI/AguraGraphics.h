#pragma once

#include <GdiPlus.h>

class CAguraGraphics : public Gdiplus::Graphics
{
public:
	// Constructor
	CAguraGraphics(IN HDC hdc);
	// Constructor
	CAguraGraphics(IN HDC hdc, IN HANDLE hdevice);
	// Constructor
	CAguraGraphics(IN HWND hwnd, IN BOOL icm = FALSE);
	// Constructor
	CAguraGraphics(IN Gdiplus::Image* image);
	// Destructor
	~CAguraGraphics();
	// Draw Round Rectangle
	Gdiplus::Status DrawRoundRectangle(IN const Gdiplus::Pen* pen, IN const Gdiplus::Rect& rect, int diameter);
	// Fill Round Rectangle
	Gdiplus::Status FillRoundRectangle(IN const Gdiplus::Brush* brush, IN const Gdiplus::Rect& rect, int diameter);
};
