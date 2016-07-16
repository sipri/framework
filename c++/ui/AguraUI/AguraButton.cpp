#include "stdafx.h"
#include "AguraButton.h"
#include "AguraConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Gdiplus;

/////////////////////////////////////////////////////////////////////////////
// CAguraButton

// Mask for control's type
#define SKINBTN_TYPEMASK1 SS_TYPEMASK
// ========================================
//	1. 함 수 명 : CAguraButton()
//	2. 기    능 : 클래스 생성자
//	3. 전달인자 : 없음
//	4. 반 환 값 : 없음
//	5. 작 성 일 : 2013/06/05
//	6. 작 성 자 : 송산호
//	7. 수정기록 
//  8. 참    고
//				
// ----------------------------------------------------------------------------
CAguraButton::CAguraButton()
{
	m_bIsFile			= TRUE;
	m_bIsCaptionFile	= TRUE;
	m_bHover			= FALSE;
	m_bIsPressed		= FALSE;
	m_bIsFocused		= FALSE;
	m_bIsDisabled		= FALSE;
//	m_bMouseOnButton	= FALSE;

	m_iImagePosition	= eNone;
	
	// By default icon is aligned horizontally
	m_byAlign = 0;

	// By default the button is not the default button
	m_bIsDefault = FALSE;

	// Set default colors
	SetDefaultColors(FALSE);

	// Invalid value, since type still unknown
	m_nTypeStyle = SKINBTN_TYPEMASK1;

// 	m_ptBitmapOrg.x = 0;
// 	m_ptBitmapOrg.y = 0;
// 	m_ptIconOrg.x = 0;
// 	m_ptIconOrg.y = 0;

	m_strNormalPath.Empty();
	m_strClickPath.Empty();
	m_strHoverPath.Empty();
}
// ========================================
//	1. 함 수 명 : ~CAguraButton()
//	2. 기    능 : 클래스 소멸자
//	3. 전달인자 : 없음
//	4. 반 환 값 : 없음
//	5. 작 성 일 : 2013/06/05
//	6. 작 성 자 : 송산호
//	7. 수정기록 
//  8. 참    고
//				
// ----------------------------------------------------------------------------
CAguraButton::~CAguraButton()
{
}

BEGIN_MESSAGE_MAP(CAguraButton, CButton)
	ON_WM_SYSCOLORCHANGE()
	ON_WM_GETDLGCODE()
	ON_WM_CTLCOLOR_REFLECT()
	ON_MESSAGE(BM_SETSTYLE, OnSetStyle)
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()


// ========================================
//	1. 함 수 명 : DrawItem(LPDRAWITEMSTRUCT lpDIS)
//	2. 기    능 : owner-drawn 버튼 이 변경될 경우에 불린다. 
//	3. 전달인자 : LPDRAWITEMSTRUCT 
//	4. 반 환 값 : 없음
//	5. 작 성 일 : 2013/06/05
//	6. 작 성 자 : 송산호
//	7. 수정기록 
//  8. 참    고
//				
// ----------------------------------------------------------------------------
void CAguraButton::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
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
// ========================================
//	1. 함 수 명 : DrawItem(LPDRAWITEMSTRUCT lpDIS)
//	2. 기    능 : owner-drawn 버튼 이 변경될 경우에 불린다. 
//	3. 전달인자 : LPDRAWITEMSTRUCT 
//	4. 반 환 값 : 없음
//	5. 작 성 일 : 2013/06/05
//	6. 작 성 자 : 송산호
//	7. 수정기록 
//  8. 참    고
//				
// ----------------------------------------------------------------------------
void CAguraButton::OnMouseLeave()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_bHover = FALSE;
	Invalidate(FALSE);
	CButton::OnMouseLeave();
}
// ========================================
//	1. 함 수 명 : DrawItem(LPDRAWITEMSTRUCT lpDIS)
//	2. 기    능 : owner-drawn 버튼 이 변경될 경우에 불린다. 
//	3. 전달인자 : LPDRAWITEMSTRUCT 
//	4. 반 환 값 : 없음
//	5. 작 성 일 : 2013/06/05
//	6. 작 성 자 : 송산호
//	7. 수정기록 
//  8. 참    고
//				
// ----------------------------------------------------------------------------
void CAguraButton::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	CDC *pDC(CDC::FromHandle(lpDIS->hDC));
//	CFont *poldFont = pDC->SelectObject(GetFont());
	
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
// ========================================
//	1. 함 수 명 : PreTranslateMessage(MSG* pMsg)
//	2. 기    능 : ESC, ENTER 비활성화 
//	3. 전달인자 : 키 버튼 메시지 
//	4. 반 환 값 : BOOL
//	5. 작 성 일 : 2013/06/05
//	6. 작 성 자 : 송산호
//	7. 수정기록 
//  8. 참    고
//				
// ----------------------------------------------------------------------------
BOOL CAguraButton::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_LBUTTONDBLCLK )
		pMsg->message = WM_LBUTTONDOWN;
	/*LDRA_INSPECTED 101 S*/
	return CButton::PreTranslateMessage(pMsg);
}
// ========================================
//	1. 함 수 명 : SetDefaultColors(BOOL bRepaint)
//	2. 기    능 : 모든 색상을 기본 값으로 셋팅한다.
//	3. 전달인자 : BOOL bRepaint 새로 그려질지에 대한 TRUE/FALSE
//	4. 반 환 값 : DWORD
//	5. 작 성 일 : 2013/06/05
//	6. 작 성 자 : 송산호
//	7. 수정기록 
//  8. 참    고
//				
// ----------------------------------------------------------------------------
DWORD CAguraButton::SetDefaultColors(BOOL bRepaint)
{
	m_clrBK = AGURA_COLOR_BACK;
	m_clrText = AGURA_COLOR_TEXT;

	if( bRepaint )	Invalidate();

	return 0;
}

// ========================================
//	1. 함 수 명 : setCaptionImage(CString strCaption)
//	2. 기    능 : 버튼 이미지를 추가한다. 
//	3. 전달인자 : BOOL bRepaint 새로 그려질지에 대한 TRUE/FALSE
//	4. 반 환 값 : DWORD
//	5. 작 성 일 : 2013/06/05
//	6. 작 성 자 : 송산호
//	7. 수정기록 
//  8. 참    고
//				
// ----------------------------------------------------------------------------
void CAguraButton::setCaptionImage(int iPosition, CString strCaption)
{
	m_iImagePosition = iPosition;
	m_strCaptionPath = strCaption;

	m_bIsCaptionFile = TRUE;
}

// ========================================
//	1. 함 수 명 : setCaptionImage(CString strCaption)
//	2. 기    능 : 버튼 이미지를 추가한다. 
//	3. 전달인자 : BOOL bRepaint 새로 그려질지에 대한 TRUE/FALSE
//	4. 반 환 값 : DWORD
//	5. 작 성 일 : 2013/06/05
//	6. 작 성 자 : 송산호
//	7. 수정기록 
//  8. 참    고
//				
// ----------------------------------------------------------------------------
void CAguraButton::setCaptionImage(int iPosition, int iCaption)
{
	m_iImagePosition = iPosition;
	m_iCaptionResource = iCaption;

	m_bIsCaptionFile = FALSE;
}
// ========================================
//	1. 함 수 명 : setButtonImage(CString strNormal, CString strHover, CString strClick)
//	2. 기    능 : 버튼 이미지를 추가한다. 
//	3. 전달인자 : BOOL bRepaint 새로 그려질지에 대한 TRUE/FALSE
//	4. 반 환 값 : DWORD
//	5. 작 성 일 : 2013/06/05
//	6. 작 성 자 : 송산호
//	7. 수정기록 
//  8. 참    고
//				
// ----------------------------------------------------------------------------
/*LDRA_INSPECTED 65 D*/
void CAguraButton::setButtonImage(CString strNormal, CString strHover, CString strClick)
{
	m_strNormalPath	= strNormal;
	m_strHoverPath	= strHover;
	m_strClickPath	= strClick;

	m_bIsFile = TRUE;
}

// ========================================
//	1. 함 수 명 : setButtonImage(int iNormal, int iHover, int iClick)
//	2. 기    능 : 버튼 이미지를 추가한다. 
//	3. 전달인자 : BOOL bRepaint 새로 그려질지에 대한 TRUE/FALSE
//	4. 반 환 값 : DWORD
//	5. 작 성 일 : 2013/06/05
//	6. 작 성 자 : 송산호
//	7. 수정기록 
//  8. 참    고
//				
// ----------------------------------------------------------------------------
void CAguraButton::setButtonImage(int iNormal, int iHover, int iClick)
{
	m_iNoramlResource	= iNormal;
	m_iHoverResource	= iHover;
	m_iClickResource	= iClick;

	m_bIsFile = FALSE;
}
// ========================================
//	1. 함 수 명 : setButtonColor(COLORREF clrBK, COLORREF clrText)
//	2. 기    능 : 버튼의 색상을 인자값의 RGB로 변경한다. 
//	3. 전달인자 : BOOL bRepaint 새로 그려질지에 대한 TRUE/FALSE
//	4. 반 환 값 : DWORD
//	5. 작 성 일 : 2013/06/05
//	6. 작 성 자 : 송산호
//	7. 수정기록 
//  8. 참    고
//				
// ----------------------------------------------------------------------------
/*LDRA_INSPECTED 65 D*/
void CAguraButton::setButtonColor(COLORREF clrBK, COLORREF clrText)
{
	m_clrBK = clrBK;
	m_clrText = clrText;	
}

// ========================================
//	1. 함 수 명 : PreSubclassWindow()
//	2. 기    능 : 서브클래싱에 앞서서 호출되는 함수
//	3. 전달인자 : 없음
//	4. 반 환 값 : 없음
//	5. 작 성 일 : 2013/06/05
//	6. 작 성 자 : 송산호
//	7. 수정기록 
//  8. 참    고
//				
// ----------------------------------------------------------------------------
void CAguraButton::PreSubclassWindow() 
{
	UINT nBS = GetButtonStyle();

	// Set initial control type
	m_nTypeStyle = nBS & SKINBTN_TYPEMASK1;

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
	ModifyStyle(SKINBTN_TYPEMASK1, BS_OWNERDRAW, SWP_FRAMECHANGED);

	CButton::PreSubclassWindow();
}
// ========================================
//	1. 함 수 명 : OnSysColorChange()
//	2. 기    능 : 색상을 변경하려 할 때 불려와져서 기본 색상으로 변경한다. 
//	3. 전달인자 : 없음
//	4. 반 환 값 : 없음
//	5. 작 성 일 : 2013/06/05
//	6. 작 성 자 : 송산호
//	7. 수정기록 
//  8. 참    고
//				
// ----------------------------------------------------------------------------
void CAguraButton::OnSysColorChange() 
{
	CButton::OnSysColorChange();
	SetDefaultColors();
}

// ========================================
//	1. 함 수 명 : OnGetDlgCode()
//	2. 기    능 : CWnd 제어 자체를 처리하기 위한 타입을 선택가능하게 한다.
//	3. 전달인자 : 없음
//	4. 반 환 값 : UINT nCode
//	5. 작 성 일 : 2013/06/05
//	6. 작 성 자 : 송산호
//	7. 수정기록 
//  8. 참    고
//				
// ----------------------------------------------------------------------------
UINT CAguraButton::OnGetDlgCode() 
{
	UINT nCode = CButton::OnGetDlgCode();
	
	nCode |= (m_bIsDefault ? DLGC_DEFPUSHBUTTON : DLGC_UNDEFPUSHBUTTON);

	return nCode;
}

// ========================================
//	1. 함 수 명 : CtlColor(CDC* pDC, UINT nCtlColor) 
//	2. 기    능 : 컨트롤 색상 변경
//	3. 전달인자 : CDC* pDC, UINT nCtlColor
//	4. 반 환 값 : HBRUSH
//	5. 작 성 일 : 2013/06/05
//	6. 작 성 자 : 송산호
//	7. 수정기록 
//  8. 참    고
//				
// ----------------------------------------------------------------------------
HBRUSH CAguraButton::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	return (HBRUSH)::GetStockObject(NULL_BRUSH);
}

// ========================================
//	1. 함 수 명 : OnSetStyle(WPARAM wParam, LPARAM lParam)
//	2. 기    능 : 윈도우 스타일을 수정할 때 호출된다.
//	3. 전달인자 : WPARAM wParam, LPARAM lParam
//	4. 반 환 값 : LRESULT
//	5. 작 성 일 : 2013/06/05
//	6. 작 성 자 : 송산호
//	7. 수정기록 
//  8. 참    고
//				
// ----------------------------------------------------------------------------
LRESULT CAguraButton::OnSetStyle(WPARAM wParam, LPARAM lParam)
{	
	return DefWindowProc(BM_SETSTYLE, (wParam & ~SKINBTN_TYPEMASK1) | BS_OWNERDRAW, lParam);
}
// ========================================
//	1. 함 수 명 : DrawBackground(CDC* pDC, CRect* pRect)
//	2. 기    능 : 표준 배경을 그린다. 
//	3. 전달인자 : WPARAM wParam, LPARAM lParam
//	4. 반 환 값 : DWORD
//	5. 작 성 일 : 2013/06/05
//	6. 작 성 자 : 송산호
//	7. 수정기록 
//  8. 참    고
//				
// ----------------------------------------------------------------------------
DWORD CAguraButton::DrawBackground(CDC* pDC, CRect* pRect)
{	
	pDC->FillSolidRect(pRect, m_clrBK);
	
	return 0;
}
