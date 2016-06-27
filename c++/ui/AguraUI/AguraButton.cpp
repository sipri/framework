#include "stdafx.h"
#include "AguraButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
	m_bHover			= false;
	m_bIsPressed		= FALSE;
	m_bIsFocused		= FALSE;
	m_bIsDisabled		= FALSE;
	m_bMouseOnButton	= FALSE;
	
	// By default icon is aligned horizontally
	m_byAlign = 0;

	// By default the button is not the default button
	m_bIsDefault = FALSE;

	// Set default colors
	SetDefaultColors(FALSE);

	// Invalid value, since type still unknown
	m_nTypeStyle = SKINBTN_TYPEMASK1;

	m_ptBitmapOrg.x = 0;
	m_ptBitmapOrg.y = 0;
	m_ptIconOrg.x = 0;
	m_ptIconOrg.y = 0;

	m_strNormalPath.Empty();
	m_strClickPath.Empty();
	m_strHoverPath.Empty();

	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
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
	GdiplusShutdown(m_gdiplusToken);	
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
	if (m_bHover == false)
	{
 		TRACKMOUSEEVENT t;
 		t.cbSize = sizeof(t);
 		t.dwFlags = TME_LEAVE;
 		t.hwndTrack = GetSafeHwnd();
 		t.dwHoverTime = 0x00000001;
		_TrackMouseEvent(&t);
		m_bHover = true;
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
	m_bHover = false;
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
	CFont *poldFont = pDC->SelectObject(GetFont());

	m_bIsPressed = (lpDIS->itemState & ODS_SELECTED);
	m_bIsFocused  = (lpDIS->itemState & ODS_FOCUS);
	m_bIsDisabled = (lpDIS->itemState & ODS_DISABLED);

	CRect captionRect = lpDIS->rcItem;

	pDC->FillSolidRect(captionRect, m_clrBK);

	Graphics graphics(*pDC);
	_bstr_t str;

	if (m_bIsPressed == TRUE)
	{
		str = m_strClickPath;
	}
	else if (m_bHover == true)
	{
		str = m_strHoverPath;
	}
	else
	{
		str = m_strNormalPath;
	}

	REAL rTransparency = 0.5f;    // 0.1f ~ 1.0f

	ColorMatrix colorMatrix = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, rTransparency, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f};

	ImageAttributes imageAttr;
	Image image(str);
	imageAttr.SetColorMatrix(&colorMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);

	if (m_bIsPressed)
	{
		graphics.DrawImage(&image, 0, 0, captionRect.Width(), captionRect.Height());
	}
	else
	{
		if(m_bIsDisabled)
		{
			graphics.DrawImage(&image, Rect(0, 0, captionRect.Width(), captionRect.Height()), 0, 0, image.GetWidth(), image.GetHeight(), UnitPixel, &imageAttr);	
		}
		else
		{
			graphics.DrawImage(&image, 0, 0, captionRect.Width(), captionRect.Height());
		}
	}

	CString sTitle = _T(" ");
	GetWindowText(sTitle);	

	// Write the button title
	if( sTitle.IsEmpty() == FALSE )
	{		
		if( m_bIsPressed)
			captionRect.OffsetRect(1, 1);

		// Center text
		CRect centerRect = captionRect;
		pDC->DrawText(sTitle, -1, captionRect, DT_WORDBREAK | DT_CENTER | DT_CALCRECT);
		captionRect.OffsetRect((centerRect.Width() - captionRect.Width())/2, (centerRect.Height() - captionRect.Height())/2);
		pDC->SetBkMode(TRANSPARENT);

		if(m_bIsDisabled)
		{
			captionRect.OffsetRect(0, 0);
			pDC->SetTextColor(::GetSysColor(COLOR_3DSHADOW));
			pDC->DrawText(sTitle, -1, captionRect, DT_WORDBREAK | DT_CENTER);
		}
		else
		{
			pDC->SetTextColor(m_clrText);
			pDC->DrawText(sTitle, -1, captionRect, DT_WORDBREAK | DT_CENTER);
		}
	}

	pDC->SelectObject(poldFont);
	
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
	m_clrBK = RGB(12, 37, 48);
	m_clrText = RGB(255, 255, 255);

	if( bRepaint )	Invalidate();

	return 0;
} 
// ========================================
//	1. �� �� �� : vSetButtonImage(CString strNormal, CString strHover, CString strClick)
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
void CAguraButton::vSetButtonImage(CString strNormal, CString strHover, CString strClick)
{
	m_strNormalPath	= strNormal;
	m_strHoverPath	= strHover;
	m_strClickPath	= strClick;
}
// ========================================
//	1. �� �� �� : vSetButtonColor(COLORREF clrBK, COLORREF clrText)
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
void CAguraButton::vSetButtonColor(COLORREF clrBK, COLORREF clrText)
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
