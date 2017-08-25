#include "stdafx.h"
#include "AguraGraphics.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/************************************************************
 *	@brief		Constructor
 *	@param[in]	hdc				DC Handle
 *	@retval		None
 ************************************************************/
CAguraGraphics::CAguraGraphics(IN HDC hdc)
	: Gdiplus::Graphics(hdc)
{
	SetTextRenderingHint(Gdiplus::TextRenderingHint::TextRenderingHintAntiAlias);
	SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
}

/************************************************************
 *	@brief		Constructor
 *	@param[in]	hdc				DC Handle
 *	@param[in]	hdevice			
 *	@retval		None
 ************************************************************/
CAguraGraphics::CAguraGraphics(IN HDC hdc, IN HANDLE hdevice)
	: Gdiplus::Graphics(hdc, hdevice)
{
	SetTextRenderingHint(Gdiplus::TextRenderingHint::TextRenderingHintAntiAlias);
	SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
}

/************************************************************
 *	@brief		Constructor
 *	@param[in]	hwnd			Window Handle
 *	@param[in]	icm				
 *	@retval		None
 ************************************************************/
CAguraGraphics::CAguraGraphics(IN HWND hwnd, IN BOOL icm)
	: Gdiplus::Graphics(hwnd, icm)
{
	SetTextRenderingHint(Gdiplus::TextRenderingHint::TextRenderingHintAntiAlias);
	SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
}

/************************************************************
 *	@brief		Constructor
 *	@param[in]	image			Image
 *	@retval		None
 ************************************************************/
CAguraGraphics::CAguraGraphics(IN Gdiplus::Image* image)
	: Gdiplus::Graphics(image)
{
	SetTextRenderingHint(Gdiplus::TextRenderingHint::TextRenderingHintAntiAlias);
	SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
}

/************************************************************
 *	@brief		Destructor
 *	@retval		None
 ************************************************************/
CAguraGraphics::~CAguraGraphics()
{
}

/************************************************************
 *	@brief		Fill Round Rectangle
 *	@param[in]	brush			Brush
 *	@param[in]	rect			Rect
 *	@param[in]	diameter		Diameter
 *	@retval		None
 ************************************************************/
Gdiplus::Status CAguraGraphics::FillRoundRectangle(IN const Gdiplus::Brush* brush, IN const Gdiplus::Rect& rect, int diameter)
{
	Gdiplus::GraphicsPath path;
	Gdiplus::Rect fillRect = rect;

    // diameter can't exceed width or height
    if (diameter > fillRect.Width)
		diameter = fillRect.Width;
    if (diameter > fillRect.Height)
		diameter = fillRect.Height;

    // define a corner 
    Gdiplus::Rect Corner(rect.X, fillRect.Y, diameter, diameter);

    // begin path
    path.Reset();

    // top left
    path.AddArc(Corner, 180, 90);    

    // tweak needed for fillRect.dius of 10 (dia of 20)
    if (diameter == 20)
    {
        Corner.Width += 1; 
        Corner.Height += 1; 
        fillRect.Width -=1;
		fillRect.Height -= 1;
    }

    // top fillRect.ght
    Corner.X += (rect.Width - diameter - 1);
    path.AddArc(Corner, 270, 90);    
    
    // bottom fillRect.ght
    Corner.Y += (rect.Height - diameter - 1);
    path.AddArc(Corner, 0, 90);    
    
    // bottom left
    Corner.X -= (rect.Width - diameter - 1);
    path.AddArc(Corner, 90, 90);

    // end path
    path.CloseFigure();

	return FillPath(brush, &path);
}
