#include "stdafx.h"
#include "AguraButton.h"
#include "AguraConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Gdiplus;

BEGIN_MESSAGE_MAP(CAguraButton, CButton)
	ON_MESSAGE(BM_SETSTYLE, OnSetStyle)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_ERASEBKGND()
	ON_WM_GETDLGCODE()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_SYSCOLORCHANGE()
END_MESSAGE_MAP()

CAguraButton::CAguraButton()
{
	m_bIsFile			= TRUE;
	m_bIsCaptionFile	= TRUE;
	m_bHover			= FALSE;
	m_bIsPressed		= FALSE;
	m_bIsFocused		= FALSE;
	m_bIsDisabled		= FALSE;
	m_iImagePosition	= eNone;
	
	// By default icon is aligned horizontally
	m_byAlign = 0;

	// By default the button is not the default button
	m_bIsDefault = FALSE;

	// Set default colors
	SetDefaultColors(FALSE);

	// Invalid value, since type still unknown
	m_nTypeStyle = SS_TYPEMASK;
	m_strNormalPath.Empty();
	m_strClickPath.Empty();
	m_strHoverPath.Empty();
}

CAguraButton::~CAguraButton()
{
}

void CAguraButton::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (m_bHover == FALSE)
	{
 		TRACKMOUSEEVENT t;
 		t.cbSize = sizeof(t);
 		t.dwFlags = TME_LEAVE;
 		t.hwndTrack = GetSafeHwnd();
 		t.dwHoverTime = 0x00000001;
		_TrackMouseEvent(&t);
		m_bHover = TRUE;
		Invalidate(FALSE);
	}

	CButton::OnMouseMove(nFlags, point);
}

void CAguraButton::OnMouseLeave()
{
	m_bHover = FALSE;
	Invalidate(FALSE);
	CButton::OnMouseLeave();
}

void CAguraButton::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	CDC *pDC(CDC::FromHandle(lpDIS->hDC));
	
	m_bIsPressed = (lpDIS->itemState & ODS_SELECTED);
	m_bIsFocused  = (lpDIS->itemState & ODS_FOCUS);
	m_bIsDisabled = (lpDIS->itemState & ODS_DISABLED);

	CRect captionRect = lpDIS->rcItem;
	CRect rtOrigin = captionRect;

	CDC memDC;
	CBitmap memBmp;
	CBitmap* pOldBmp;
	CFont *pOldFont;

	memDC.CreateCompatibleDC(pDC);
	memBmp.CreateCompatibleBitmap(pDC, captionRect.Width(), captionRect.Height());

	pOldBmp = memDC.SelectObject(&memBmp);
	pOldFont = memDC.SelectObject(GetFont());

	memDC.FillSolidRect(captionRect, m_clrBK);

	Graphics graphics(memDC);
	_bstr_t str;
	int iResource;

	if (m_bIsPressed == TRUE)
	{
		str = m_strClickPath;
		iResource = m_iClickResource;
	}
	else if (m_bHover == TRUE)
	{
		str = m_strHoverPath;
		iResource = m_iHoverResource;
	}
	else
	{
		str = m_strNormalPath;
		iResource = m_iNoramlResource;
	}

	REAL rTransparency = 0.5f;    // 0.1f ~ 1.0f

	ColorMatrix colorMatrix = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, rTransparency, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f};

	ImageAttributes imageAttr;
	Bitmap* pBitmap;

	if (m_bIsFile == TRUE)
		pBitmap = Bitmap::FromFile(str);
	else
		pBitmap = Bitmap::FromResource(AfxGetInstanceHandle(), (WCHAR*)MAKEINTRESOURCE(iResource));

	imageAttr.SetColorMatrix(&colorMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);

	if (pBitmap->GetLastStatus() == Gdiplus::Ok)
	{
		if (m_bIsPressed)
		{
			graphics.DrawImage(pBitmap, 0, 0, captionRect.Width(), captionRect.Height());
		}
		else
		{
			if(m_bIsDisabled)
			{
				graphics.DrawImage(pBitmap, Rect(0, 0, captionRect.Width(), captionRect.Height()), 0, 0, pBitmap->GetWidth(), pBitmap->GetHeight(), UnitPixel, &imageAttr);	
			}
			else
			{
				graphics.DrawImage(pBitmap, 0, 0, captionRect.Width(), captionRect.Height());
			}
		}
	}
	else
	{
		Pen pen(Color(40, 70, 84), 1);
		graphics.DrawRectangle(&pen, captionRect.left, captionRect.top, captionRect.Width() - 1, captionRect.Height() - 1);
	}

	CString sTitle = _T(" ");
	GetWindowText(sTitle);

	if( m_bIsPressed)	captionRect.OffsetRect(0, 1);

	// Draw Image
	if (m_iImagePosition == eLeft)
	{
		CRect rtTemp = captionRect;
		captionRect.OffsetRect(20, 0);

		Bitmap* pBitmapCaption;

		if (m_bIsCaptionFile == TRUE)
			pBitmapCaption = Bitmap::FromFile(m_strCaptionPath);
		else
			pBitmapCaption = Bitmap::FromResource(AfxGetInstanceHandle(), (WCHAR*)MAKEINTRESOURCE(m_iCaptionResource));

		if (pBitmapCaption->GetLastStatus() == Gdiplus::Ok)
		{
			rtTemp.InflateRect(-10, 0, -10, 0);

			int iHeight = 17;
			int iWidth = iHeight * pBitmapCaption->GetWidth() / pBitmapCaption->GetHeight();

			captionRect.OffsetRect(iWidth, 0);
			
			graphics.DrawImage(pBitmapCaption, rtTemp.left, rtTemp.top + (rtTemp.Height() - iHeight) / 2, iWidth, iHeight);
		}

		delete pBitmapCaption;
	}
	else if (m_iImagePosition == eCenter)
	{
		Bitmap* pBitmapCaption;

		if (m_bIsCaptionFile == TRUE)
			pBitmapCaption = Bitmap::FromFile(m_strCaptionPath);
		else
			pBitmapCaption = Bitmap::FromResource(AfxGetInstanceHandle(), (WCHAR*)MAKEINTRESOURCE(m_iCaptionResource));

		if (pBitmapCaption->GetLastStatus() == Gdiplus::Ok)
		{
			CRect rtTemp = captionRect;
			captionRect.top = rtTemp.top + captionRect.Height() / 3 * 2;
			rtTemp.bottom = captionRect.top;

			rtTemp.InflateRect(-10, -10, -10, 0);
			int iWidth = rtTemp.Width();
			int iHeight = rtTemp.Height();

			if ((iHeight / pBitmapCaption->GetHeight()) > (iWidth / pBitmapCaption->GetWidth()))
			{
				iHeight = iWidth * pBitmapCaption->GetHeight() / pBitmapCaption->GetWidth();
			}
			else
			{
				iWidth = iHeight * pBitmapCaption->GetWidth() / pBitmapCaption->GetHeight();
			}

			graphics.DrawImage(pBitmapCaption, rtTemp.left + (rtTemp.Width() - iWidth) / 2, rtTemp.top, iWidth, iHeight);
		}

		delete pBitmapCaption;
	}

	// Write the button title
	if( sTitle.IsEmpty() == FALSE )
	{		
		DWORD dwFormat = m_iImagePosition != eLeft ? DT_CENTER : DT_LEFT;
		dwFormat |= DT_WORDBREAK | DT_VCENTER | DT_SINGLELINE;

		// Center text
		CRect centerRect = captionRect;
		memDC.SetBkMode(TRANSPARENT);
 
		if(m_bIsDisabled)
		{
			memDC.SetTextColor(::GetSysColor(COLOR_3DSHADOW));
			memDC.DrawText(sTitle, -1, captionRect, dwFormat);
		}
 		else
 		{
 			memDC.SetTextColor(m_clrText);
 			memDC.DrawText(sTitle, -1, captionRect, dwFormat);
 		}
	}


	pDC->BitBlt(0, 0, rtOrigin.Width(), rtOrigin.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldFont);
	memDC.SelectObject(pOldBmp);

	memBmp.DeleteObject();
	memDC.DeleteDC();

	delete pBitmap;	
}

DWORD CAguraButton::SetDefaultColors(BOOL bRepaint)
{
	m_clrBK = AGURA_COLOR_BACK;
	m_clrText = AGURA_COLOR_TEXT;

	if( bRepaint )	Invalidate();

	return 0;
}

void CAguraButton::setCaptionImage(int iPosition, CString strCaption)
{
	m_iImagePosition = iPosition;
	m_strCaptionPath = strCaption;

	m_bIsCaptionFile = TRUE;
}

void CAguraButton::setCaptionImage(int iPosition, int iCaption)
{
	m_iImagePosition = iPosition;
	m_iCaptionResource = iCaption;

	m_bIsCaptionFile = FALSE;
}

void CAguraButton::setButtonImage(CString strNormal, CString strHover, CString strClick)
{
	m_strNormalPath	= strNormal;
	m_strHoverPath	= strHover;
	m_strClickPath	= strClick;

	m_bIsFile = TRUE;
}

void CAguraButton::setButtonImage(int iNormal, int iHover, int iClick)
{
	m_iNoramlResource	= iNormal;
	m_iHoverResource	= iHover;
	m_iClickResource	= iClick;

	m_bIsFile = FALSE;
}

void CAguraButton::setButtonColor(COLORREF clrBK, COLORREF clrText)
{
	m_clrBK = clrBK;
	m_clrText = clrText;	
}

void CAguraButton::PreSubclassWindow() 
{
	UINT nBS = GetButtonStyle();

	// Set initial control type
	m_nTypeStyle = nBS & SS_TYPEMASK;

	// Set initial default state flag
	if( m_nTypeStyle == BS_DEFPUSHBUTTON )
	{
		// Set default state for a default button
		m_bIsDefault = TRUE;

		// Adjust style for default button
		m_nTypeStyle = BS_PUSHBUTTON;
	}

	// You should not set the Owner Draw before this call
	// (don't use the resource editor "Owner Draw" or
	// ModifyStyle(0, BS_OWNERDRAW) before calling PreSubclassWindow() )
	ASSERT(m_nTypeStyle != BS_OWNERDRAW);

	// Switch to owner-draw
	ModifyStyle(SS_TYPEMASK, BS_OWNERDRAW, SWP_FRAMECHANGED);

	CButton::PreSubclassWindow();
}

void CAguraButton::OnSysColorChange() 
{
	CButton::OnSysColorChange();
	SetDefaultColors();
}

UINT CAguraButton::OnGetDlgCode() 
{
	UINT nCode = CButton::OnGetDlgCode();
	
	nCode |= (m_bIsDefault ? DLGC_DEFPUSHBUTTON : DLGC_UNDEFPUSHBUTTON);

	return nCode;
}

HBRUSH CAguraButton::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	return (HBRUSH)::GetStockObject(NULL_BRUSH);
}

LRESULT CAguraButton::OnSetStyle(WPARAM wParam, LPARAM lParam)
{	
	return DefWindowProc(BM_SETSTYLE, (wParam & ~SS_TYPEMASK) | BS_OWNERDRAW, lParam);
}

DWORD CAguraButton::DrawBackground(CDC* pDC, CRect* pRect)
{	
	pDC->FillSolidRect(pRect, m_clrBK);
	
	return 0;
}
