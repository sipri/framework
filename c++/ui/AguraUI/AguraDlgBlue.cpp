#include "stdafx.h"
#include "AguraDlgBlue.h"

using namespace Gdiplus;

IMPLEMENT_DYNAMIC(CAguraDlgBlue, CDialog)

BEGIN_MESSAGE_MAP(CAguraDlgBlue, CDialog)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

CAguraDlgBlue::CAguraDlgBlue(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
: CDialog(nIDTemplate, pParent)
{
	m_strTitle = _T("Dialog");
	m_bIsMax		= FALSE;
	m_bMove			= FALSE;
	m_pFont			= NULL;

	for (int i = eMin; i <= eExit; ++i)
	{
		m_pBtn[i] = NULL;
	}

	HMODULE hUser32 = GetModuleHandle(_T("USER32.DLL"));
	SetLayeredWindowAttributes = (lpfn)GetProcAddress(hUser32, "SetLayeredWindowAttributes");
}

CAguraDlgBlue::~CAguraDlgBlue()
{
	for (int i = eMin; i <= eExit; ++i)
	{
		delete m_pBtn[i];
	}
}

BOOL CAguraDlgBlue::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	ModifyStyle(WS_THICKFRAME| WS_CAPTION |  DS_MODALFRAME, 0, SWP_FRAMECHANGED);
	ModifyStyleEx(WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE, 0, SWP_FRAMECHANGED);

	CString strN, strH, strC;
	CStringArray sa;
	sa.Add(_T("Min"));
	sa.Add(_T("Max"));
	sa.Add(_T("Exit"));

	for (int i = eMin; i <= eExit; ++i)
	{
		m_pBtn[i] = new CAguraButton;
		m_pBtn[i]->Create(NULL, BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD, CRect(0, 0, SYS_BTN_CX, SYS_BTN_CY), this, 2000 + i);

		strN.Format(_T("Image\\%sN.png"), sa.GetAt(i));
		strH.Format(_T("Image\\%sC.png"), sa.GetAt(i));
		strC.Format(_T("Image\\%sC.png"), sa.GetAt(i));

		m_pBtn[i]->setButtonImage(strN, strH, strC);
		m_pBtn[i]->setButtonColor(RGB(13, 27, 34), RGB(255, 255, 255));
	}


	CRect rtDlg;

	GetClientRect(&rtDlg);
	int iLeft = rtDlg.right - (SYS_BTN_CX * 3) - 10 - 15;

	for (int i = eMin; i <= eExit; ++i)
	{
		m_pBtn[i]->MoveWindow(iLeft + (SYS_BTN_CX * i) + (5 * i), rtDlg.top + 7, SYS_BTN_CX, SYS_BTN_CY);
	}

	m_fontStatic.CreatePointFont(100, _T("����"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

BOOL CAguraDlgBlue::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
		return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

// ========================================
//	1. �� �� �� : OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//	2. ��    �� : ��Ʈ�� ���� ����
//	3. �������� : CDC* pDC, CWnd* pWnd, UINT nCtlColor
//	4. �� ȯ �� : HBRUSH
//	5. �� �� �� : 2013/05/30
//	6. �� �� �� : �ۻ�ȣ
//	7. ������� 
//  8. ��    ��
//				
// ----------------------------------------------------------------------------
HBRUSH CAguraDlgBlue::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	DeleteObject(m_hbr);

	switch(nCtlColor)
	{
	case CTLCOLOR_STATIC:
		if (m_pFont != NULL)	pDC->SelectObject(m_pFont);
		m_pFont = pDC->SelectObject(&m_fontStatic);
		pDC->SetTextColor(DLG_TEXT_COLOR);
		pDC->SetBkMode(TRANSPARENT);
//		pDC->SetBkColor(DLG_FRAME_COLOR);				// ����� ��������
		m_hbr = (HBRUSH)::GetStockObject(NULL_BRUSH);
		break;
	case CTLCOLOR_EDIT:
		pDC->SetTextColor(DLG_TEXT_COLOR);
		pDC->SetBkColor(DLG_CTRL_COLOR);
		m_hbr = (HBRUSH)::CreateSolidBrush(DLG_CTRL_COLOR);
		break;
	case CTLCOLOR_LISTBOX:
		pDC->SetTextColor(DLG_TEXT_COLOR);
		pDC->SetBkColor(DLG_CTRL_COLOR);
		m_hbr = (HBRUSH)::CreateSolidBrush(DLG_CTRL_COLOR);
		break;
	default:
		m_hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	}

	// TODO:  �⺻���� �������� ������ �ٸ� �귯�ø� ��ȯ�մϴ�.
	return m_hbr;
}

void CAguraDlgBlue::OnPaint()
{
	CPaintDC dc(this);

	CDC memDC;
	CBitmap memBitmap, *pOldBitmap;
	CFont font, *pOldFont;
	CRect rt;
	CRect rtTemp;

	GetClientRect(&rt);

	memDC.CreateCompatibleDC(&dc);
	memBitmap.CreateCompatibleBitmap(&dc, rt.Width(), rt.Height() + 5);

	font.CreatePointFont(100, _T("���� ���"));

	pOldBitmap = memDC.SelectObject(&memBitmap);
	pOldFont = memDC.SelectObject(&font);

	Image imageTitle(_T("Image\\TitleIcon.png"));
	Graphics g(memDC);
	g.SetSmoothingMode(SmoothingModeHighQuality);

	// Title Bar
	rtTemp.SetRect(rt.left, rt.top, rt.right, 30);
	m_rtTitle = rtTemp;
	//		gradationRect(g, rtTemp, Color(12, 37, 48), Color(12, 37, 48), Color(12, 37, 48));

	if (imageTitle.GetLastStatus() == Gdiplus::Ok)
	{
		RectF rtIcon((REAL)rt.left + 12, (REAL)rt.top + 6, (REAL)21, (REAL)21);
		g.DrawImage(&imageTitle, rtIcon);
	}

	memDC.SetBkMode(TRANSPARENT);
	memDC.SetTextColor(RGB(167, 169, 171));

	rtTemp.SetRect(rt.left + 38, rt.top + 10, rt.right, rt.top + 6 + 21);
	DrawTextBackgroundLight(memDC, rt.left + 39, rt.top + 11, m_strTitle);
	memDC.DrawText(m_strTitle, rtTemp, DT_LEFT | DT_BOTTOM);

	// Work Area
	rtTemp.SetRect(rt.left, rt.top, rt.right, m_rtTitle.bottom + (m_rtTitle.Height() * 3));
	gradationCenter(g, rtTemp, Color(12, 37, 48), Color(13, 27, 34));

	// Work Area
	rtTemp.SetRect(rt.left, rt.top + (m_rtTitle.Height() * 3), rt.right, rt.bottom);
	gradationCenter(g, rtTemp, Color(17, 51, 67), Color(18, 40, 50));

	// Border Line
	Pen pen(Color(40, 70, 84), 1);
	g.DrawRectangle(&pen, rt.left, rt.top, rt.Width() - 1, rt.Height() - 1);

	dc.BitBlt(0, 0, rt.Width(), rt.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBitmap);

	memBitmap.DeleteObject();
	memDC.SelectObject(pOldFont);

	font.DeleteObject();

	ReleaseDC(&memDC);
	DeleteDC(memDC);
}

void CAguraDlgBlue::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (m_pBtn[eMin] != NULL)
	{
		CRect rtDlg;

		GetClientRect(&rtDlg);
		int iLeft = rtDlg.right - (SYS_BTN_CX * 3) - 10 - 15;

		for (int i = eMin; i <= eExit; ++i)
		{
			m_pBtn[i]->MoveWindow(iLeft + (SYS_BTN_CX * i) + (5 * i), rtDlg.top + 7, SYS_BTN_CX, SYS_BTN_CY);
		}

		Invalidate(FALSE);
	}
}

void CAguraDlgBlue::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_rtTitle.PtInRect(point) == TRUE && m_bIsMax == FALSE)
	{
		m_bMove = TRUE;
		m_ptStandard = point;
		SetCapture();

		::SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) ^ WS_EX_LAYERED);
		SetLayeredWindowAttributes(m_hWnd, 0, 176, LWA_ALPHA);
		::RedrawWindow(m_hWnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CAguraDlgBlue::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bMove == TRUE)
	{
		CRect rt;

		GetWindowRect(&rt);

		MoveWindow(rt.left + point.x - m_ptStandard.x , rt.top + point.y - m_ptStandard.y , rt.Width(), rt.Height());
	}

	CDialog::OnMouseMove(nFlags, point);
}

void CAguraDlgBlue::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bMove == TRUE)
	{
		ReleaseCapture();
		m_bMove = FALSE;

		::SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) & ~WS_EX_LAYERED);
		::RedrawWindow(m_hWnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
	}

	CDialog::OnLButtonUp(nFlags, point);
}

BOOL CAguraDlgBlue::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if(2000 + eMin <= wParam && wParam <= 2000 + eExit)
	{
		switch (wParam)
		{
		case 2000 + eMin:
			ShowWindow(SW_MINIMIZE);
			break;
		case 2000 + eMax:
			if (m_bIsMax == FALSE)
			{
				m_pBtn[eMax]->setButtonImage(_T("Image\\RestoreN.png"), _T("Image\\RestoreC.png"), _T("Image\\RestoreC.png"));
				ShowWindow(SW_MAXIMIZE);
				m_bIsMax = TRUE;
			}
			else
			{
				m_pBtn[eMax]->setButtonImage(_T("Image\\MaxN.png"), _T("Image\\MaxC.png"), _T("Image\\MaxC.png"));
				ShowWindow(SW_RESTORE);
				m_bIsMax = FALSE;
			}
			break;
		case 2000 + eExit:
			OnOK();
			break;
		}
	}

	return CDialog::OnCommand(wParam, lParam);
}

void CAguraDlgBlue::gradationCenter(Graphics& g, CRect rt, Color& color1, Color& color2, BOOL bLine, Color& color3)
{
	GraphicsPath gPath;

	int iHeight = rt.Width() / 4;
	int iWidth = rt.Width() / 4;

	RectF rtF((REAL)rt.left - iWidth, (REAL)rt.top - iHeight, (REAL)rt.Width() + iWidth * 2, (REAL)rt.Height() + iHeight * 2);
	gPath.AddEllipse(rtF);
	gPath.CloseFigure();

	PathGradientBrush PGB(&gPath);
	PGB.SetCenterColor(color1); //would be some shade of blue, following your example
	int colCount = 1;
	PGB.SetSurroundColors(&color2, &colCount); //same as your center color, but with the alpha channel set to 0

	//play with these numbers to get the glow effect you want
	REAL blendFactors[] = {0.0f, 0.1f, 0.3f, 1.0f};
	REAL blendPos[] = {0.0f, 0.4f, 0.6f, 1.0f};
	//sets how transition toward the center is shaped
	PGB.SetBlend(blendFactors, blendPos, 4);
	//sets the scaling on the center. you may want to have it elongated in the x-direction
	PGB.SetFocusScales(0.2f, 0.2f);

	RectF rtDraw((REAL)rt.left, (REAL)rt.top, (REAL)rt.Width(), (REAL)rt.Height());
	g.FillRectangle(&PGB, rtDraw);

	if (bLine == TRUE)
	{
		Pen pen(color3, 1);
		g.DrawRectangle(&pen, rtDraw);
	}
}

void CAguraDlgBlue::DrawTextBackgroundLight(CDC& pDC,int x,int y,CString str)
{
	COLORREF iTextColor = pDC.SetTextColor(RGB(10, 10, 10));
	int iBkMode = pDC.SetBkMode(TRANSPARENT);

	pDC.TextOut(x+1,y+1,str);

	pDC.SetTextColor(iTextColor);
	pDC.SetBkMode(iBkMode);
}

BOOL CAguraDlgBlue::OnEraseBkgnd(CDC* pDC)
{
	return 0;
}
