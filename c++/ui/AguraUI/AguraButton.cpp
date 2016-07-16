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
//	1. �� �� �� : CAguraButton()
//	2. ��    �� : Ŭ���� ������
//	3. �������� : ����
//	4. �� ȯ �� : ����
//	5. �� �� �� : 2013/06/05
//	6. �� �� �� : �ۻ�ȣ
//	7. ������� 
//  8. ��    ��
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
//	1. �� �� �� : ~CAguraButton()
//	2. ��    �� : Ŭ���� �Ҹ���
//	3. �������� : ����
//	4. �� ȯ �� : ����
//	5. �� �� �� : 2013/06/05
//	6. �� �� �� : �ۻ�ȣ
//	7. ������� 
//  8. ��    ��
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
//	1. �� �� �� : DrawItem(LPDRAWITEMSTRUCT lpDIS)
//	2. ��    �� : owner-drawn ��ư �� ����� ��쿡 �Ҹ���. 
//	3. �������� : LPDRAWITEMSTRUCT 
//	4. �� ȯ �� : ����
//	5. �� �� �� : 2013/06/05
//	6. �� �� �� : �ۻ�ȣ
//	7. ������� 
//  8. ��    ��
//				
// ----------------------------------------------------------------------------
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
// ========================================
//	1. �� �� �� : DrawItem(LPDRAWITEMSTRUCT lpDIS)
//	2. ��    �� : owner-drawn ��ư �� ����� ��쿡 �Ҹ���. 
//	3. �������� : LPDRAWITEMSTRUCT 
//	4. �� ȯ �� : ����
//	5. �� �� �� : 2013/06/05
//	6. �� �� �� : �ۻ�ȣ
//	7. ������� 
//  8. ��    ��
//				
// ----------------------------------------------------------------------------
void CAguraButton::OnMouseLeave()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	m_bHover = FALSE;
	Invalidate(FALSE);
	CButton::OnMouseLeave();
}
// ========================================
//	1. �� �� �� : DrawItem(LPDRAWITEMSTRUCT lpDIS)
//	2. ��    �� : owner-drawn ��ư �� ����� ��쿡 �Ҹ���. 
//	3. �������� : LPDRAWITEMSTRUCT 
//	4. �� ȯ �� : ����
//	5. �� �� �� : 2013/06/05
//	6. �� �� �� : �ۻ�ȣ
//	7. ������� 
//  8. ��    ��
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
//	1. �� �� �� : PreTranslateMessage(MSG* pMsg)
//	2. ��    �� : ESC, ENTER ��Ȱ��ȭ 
//	3. �������� : Ű ��ư �޽��� 
//	4. �� ȯ �� : BOOL
//	5. �� �� �� : 2013/06/05
//	6. �� �� �� : �ۻ�ȣ
//	7. ������� 
//  8. ��    ��
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
//	1. �� �� �� : SetDefaultColors(BOOL bRepaint)
//	2. ��    �� : ��� ������ �⺻ ������ �����Ѵ�.
//	3. �������� : BOOL bRepaint ���� �׷������� ���� TRUE/FALSE
//	4. �� ȯ �� : DWORD
//	5. �� �� �� : 2013/06/05
//	6. �� �� �� : �ۻ�ȣ
//	7. ������� 
//  8. ��    ��
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
//	1. �� �� �� : setCaptionImage(CString strCaption)
//	2. ��    �� : ��ư �̹����� �߰��Ѵ�. 
//	3. �������� : BOOL bRepaint ���� �׷������� ���� TRUE/FALSE
//	4. �� ȯ �� : DWORD
//	5. �� �� �� : 2013/06/05
//	6. �� �� �� : �ۻ�ȣ
//	7. ������� 
//  8. ��    ��
//				
// ----------------------------------------------------------------------------
void CAguraButton::setCaptionImage(int iPosition, CString strCaption)
{
	m_iImagePosition = iPosition;
	m_strCaptionPath = strCaption;

	m_bIsCaptionFile = TRUE;
}

// ========================================
//	1. �� �� �� : setCaptionImage(CString strCaption)
//	2. ��    �� : ��ư �̹����� �߰��Ѵ�. 
//	3. �������� : BOOL bRepaint ���� �׷������� ���� TRUE/FALSE
//	4. �� ȯ �� : DWORD
//	5. �� �� �� : 2013/06/05
//	6. �� �� �� : �ۻ�ȣ
//	7. ������� 
//  8. ��    ��
//				
// ----------------------------------------------------------------------------
void CAguraButton::setCaptionImage(int iPosition, int iCaption)
{
	m_iImagePosition = iPosition;
	m_iCaptionResource = iCaption;

	m_bIsCaptionFile = FALSE;
}
// ========================================
//	1. �� �� �� : setButtonImage(CString strNormal, CString strHover, CString strClick)
//	2. ��    �� : ��ư �̹����� �߰��Ѵ�. 
//	3. �������� : BOOL bRepaint ���� �׷������� ���� TRUE/FALSE
//	4. �� ȯ �� : DWORD
//	5. �� �� �� : 2013/06/05
//	6. �� �� �� : �ۻ�ȣ
//	7. ������� 
//  8. ��    ��
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
//	1. �� �� �� : setButtonImage(int iNormal, int iHover, int iClick)
//	2. ��    �� : ��ư �̹����� �߰��Ѵ�. 
//	3. �������� : BOOL bRepaint ���� �׷������� ���� TRUE/FALSE
//	4. �� ȯ �� : DWORD
//	5. �� �� �� : 2013/06/05
//	6. �� �� �� : �ۻ�ȣ
//	7. ������� 
//  8. ��    ��
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
//	1. �� �� �� : setButtonColor(COLORREF clrBK, COLORREF clrText)
//	2. ��    �� : ��ư�� ������ ���ڰ��� RGB�� �����Ѵ�. 
//	3. �������� : BOOL bRepaint ���� �׷������� ���� TRUE/FALSE
//	4. �� ȯ �� : DWORD
//	5. �� �� �� : 2013/06/05
//	6. �� �� �� : �ۻ�ȣ
//	7. ������� 
//  8. ��    ��
//				
// ----------------------------------------------------------------------------
/*LDRA_INSPECTED 65 D*/
void CAguraButton::setButtonColor(COLORREF clrBK, COLORREF clrText)
{
	m_clrBK = clrBK;
	m_clrText = clrText;	
}

// ========================================
//	1. �� �� �� : PreSubclassWindow()
//	2. ��    �� : ����Ŭ���̿� �ռ��� ȣ��Ǵ� �Լ�
//	3. �������� : ����
//	4. �� ȯ �� : ����
//	5. �� �� �� : 2013/06/05
//	6. �� �� �� : �ۻ�ȣ
//	7. ������� 
//  8. ��    ��
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
//	1. �� �� �� : OnSysColorChange()
//	2. ��    �� : ������ �����Ϸ� �� �� �ҷ������� �⺻ �������� �����Ѵ�. 
//	3. �������� : ����
//	4. �� ȯ �� : ����
//	5. �� �� �� : 2013/06/05
//	6. �� �� �� : �ۻ�ȣ
//	7. ������� 
//  8. ��    ��
//				
// ----------------------------------------------------------------------------
void CAguraButton::OnSysColorChange() 
{
	CButton::OnSysColorChange();
	SetDefaultColors();
}

// ========================================
//	1. �� �� �� : OnGetDlgCode()
//	2. ��    �� : CWnd ���� ��ü�� ó���ϱ� ���� Ÿ���� ���ð����ϰ� �Ѵ�.
//	3. �������� : ����
//	4. �� ȯ �� : UINT nCode
//	5. �� �� �� : 2013/06/05
//	6. �� �� �� : �ۻ�ȣ
//	7. ������� 
//  8. ��    ��
//				
// ----------------------------------------------------------------------------
UINT CAguraButton::OnGetDlgCode() 
{
	UINT nCode = CButton::OnGetDlgCode();
	
	nCode |= (m_bIsDefault ? DLGC_DEFPUSHBUTTON : DLGC_UNDEFPUSHBUTTON);

	return nCode;
}

// ========================================
//	1. �� �� �� : CtlColor(CDC* pDC, UINT nCtlColor) 
//	2. ��    �� : ��Ʈ�� ���� ����
//	3. �������� : CDC* pDC, UINT nCtlColor
//	4. �� ȯ �� : HBRUSH
//	5. �� �� �� : 2013/06/05
//	6. �� �� �� : �ۻ�ȣ
//	7. ������� 
//  8. ��    ��
//				
// ----------------------------------------------------------------------------
HBRUSH CAguraButton::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	return (HBRUSH)::GetStockObject(NULL_BRUSH);
}

// ========================================
//	1. �� �� �� : OnSetStyle(WPARAM wParam, LPARAM lParam)
//	2. ��    �� : ������ ��Ÿ���� ������ �� ȣ��ȴ�.
//	3. �������� : WPARAM wParam, LPARAM lParam
//	4. �� ȯ �� : LRESULT
//	5. �� �� �� : 2013/06/05
//	6. �� �� �� : �ۻ�ȣ
//	7. ������� 
//  8. ��    ��
//				
// ----------------------------------------------------------------------------
LRESULT CAguraButton::OnSetStyle(WPARAM wParam, LPARAM lParam)
{	
	return DefWindowProc(BM_SETSTYLE, (wParam & ~SKINBTN_TYPEMASK1) | BS_OWNERDRAW, lParam);
}
// ========================================
//	1. �� �� �� : DrawBackground(CDC* pDC, CRect* pRect)
//	2. ��    �� : ǥ�� ����� �׸���. 
//	3. �������� : WPARAM wParam, LPARAM lParam
//	4. �� ȯ �� : DWORD
//	5. �� �� �� : 2013/06/05
//	6. �� �� �� : �ۻ�ȣ
//	7. ������� 
//  8. ��    ��
//				
// ----------------------------------------------------------------------------
DWORD CAguraButton::DrawBackground(CDC* pDC, CRect* pRect)
{	
	pDC->FillSolidRect(pRect, m_clrBK);
	
	return 0;
}
