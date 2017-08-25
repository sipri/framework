#include "stdafx.h"
#include "AguraButton.h"
#include "AguraGraphics.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/************************************************************
 *	@brief		Constructor
 *	@retval		None
 ************************************************************/
CAguraButton::CAguraButton()
{
}

/************************************************************
 *	@brief		Destructor
 *	@retval		None
 ************************************************************/
CAguraButton::~CAguraButton()
{
}

/************************************************************
 *	@brief		Pre-subclass Window
 *	@retval		None
 ************************************************************/
void CAguraButton::PreSubclassWindow() 
{
	// Switch to owner-draw
	ModifyStyle(SS_TYPEMASK, BS_OWNERDRAW, SWP_FRAMECHANGED);

	CButton::PreSubclassWindow();
}

/************************************************************
 *	@brief		Draw Item
 *	@param[in]	lpDrawItemStruct	Draw Item
 *	@retval		None
 ************************************************************/
void CAguraButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CAguraGraphics g(lpDrawItemStruct->hDC);

	Gdiplus::SolidBrush fillBrush(Gdiplus::Color(51, 122, 183));
	Gdiplus::SolidBrush textBrush(Gdiplus::Color(255, 255, 255));
	Gdiplus::Font textFont(lpDrawItemStruct->hDC);

	CString strText;
	GetWindowText(strText);

	Gdiplus::Rect fillRect(lpDrawItemStruct->rcItem.left,
		lpDrawItemStruct->rcItem.top,
		lpDrawItemStruct->rcItem.right - lpDrawItemStruct->rcItem.left,
		lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top);

	g.FillRoundRectangle(&fillBrush, fillRect, 5);

	Gdiplus::RectF textRect((Gdiplus::REAL)lpDrawItemStruct->rcItem.left,
		(Gdiplus::REAL)lpDrawItemStruct->rcItem.top,
		(Gdiplus::REAL)lpDrawItemStruct->rcItem.right - lpDrawItemStruct->rcItem.left,
		(Gdiplus::REAL)lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top);

	Gdiplus::StringFormat textFormat;
	textFormat.SetAlignment(Gdiplus::StringAlignmentCenter);
	textFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter);
	g.DrawString(strText, strText.GetLength(), &textFont, textRect, &textFormat, &textBrush);
}
