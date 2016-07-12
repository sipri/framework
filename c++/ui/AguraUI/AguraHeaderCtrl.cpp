// AguraHeaderCtrl.cpp
//
// Author:  Hans Dietrich
//          hdietrich2@hotmail.com
//
// This code is based on "Outlook 98-Style FlatHeader Control" 
// by Maarten Hoeben.
//
// See http://www.codeguru.com/listview/FlatHeader.shtml
//
// This software is released into the public domain.
// You are free to use it in any way you like.
//
// This software is provided "as is" with no expressed
// or implied warranty.  I accept no liability for any
// damage or loss of business that this software may cause.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "Test.h"
#include "AguraHeaderCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAguraHeaderCtrl

CAguraHeaderCtrl::CAguraHeaderCtrl()
{
	m_bDoubleBuffer = TRUE;
	m_iSpacing = 6;
	m_sizeArrow.cx = 8;
	m_sizeArrow.cy = 8;
	m_sizeImage.cx = 0;
	m_sizeImage.cy = 0;
	m_bStaticBorder = FALSE;
	m_nDontDropCursor = 0;
	m_bResizing = FALSE;
	m_nClickFlags = 0;

	m_cr3DHighLight = ::GetSysColor(COLOR_3DHIGHLIGHT);
	m_cr3DShadow    = ::GetSysColor(COLOR_3DSHADOW);
	m_cr3DFace      = ::GetSysColor(COLOR_3DFACE);
	m_crBtnText     = ::GetSysColor(COLOR_BTNTEXT);	

	// JCW_+ 2008.04.12 START
	m_bTracking		= FALSE;
	m_bLMouseDown	= FALSE;

	m_bFlat = FALSE;
	m_colorHeader = RGB(0,0,0);

	m_nHeight		= 30;
	m_pTextFont		= NULL;
	// JCW_+ 2008.04.12 END

	m_hBitampDivision	= (HBITMAP)::LoadImage(::AfxGetInstanceHandle(), (LPCTSTR)_T("image\\list_header_division.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE);
	m_hBitampShadow		= (HBITMAP)::LoadImage(::AfxGetInstanceHandle(), (LPCTSTR)_T("image\\list_header_shadow.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE);
}

///////////////////////////////////////////////////////////////////////////////
// dtor
CAguraHeaderCtrl::~CAguraHeaderCtrl()
{
	::DeleteObject(m_hBitampDivision);
	::DeleteObject(m_hBitampShadow);
}

///////////////////////////////////////////////////////////////////////////////
//
IMPLEMENT_DYNCREATE(CAguraHeaderCtrl, CHeaderCtrl)

BEGIN_MESSAGE_MAP(CAguraHeaderCtrl, CHeaderCtrl)
	//{{AFX_MSG_MAP(CAguraHeaderCtrl)
	ON_MESSAGE(HDM_INSERTITEM, OnInsertItem)
	ON_MESSAGE(HDM_DELETEITEM, OnDeleteItem)
	ON_MESSAGE(HDM_SETIMAGELIST, OnSetImageList)
	ON_MESSAGE(HDM_LAYOUT, OnLayout)
	ON_WM_PAINT()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	//}}AFX_MSG_MAP	
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// ModifyProperty
BOOL CAguraHeaderCtrl::ModifyProperty(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
		case FH_PROPERTY_SPACING:
			m_iSpacing = (int)lParam;
			break;

		case FH_PROPERTY_ARROW:
			m_sizeArrow.cx = LOWORD(lParam);
			m_sizeArrow.cy = HIWORD(lParam);
			break;

		case FH_PROPERTY_STATICBORDER:
			m_bStaticBorder = (BOOL)lParam;
			break;

		case FH_PROPERTY_DONTDROPCURSOR:
			m_nDontDropCursor = (UINT)lParam;
			break;

		default:
			return FALSE;
	}

	Invalidate();
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// DrawCtrl
void CAguraHeaderCtrl::DrawCtrl(CDC* pDC)
{
	CRect rectClip;
	if (pDC->GetClipBox(&rectClip) == ERROR)
		return;

	CRect rectClient, rectItem;
	GetClientRect(&rectClient);

	// JCW_U 2008.04.12 START
	HCURSOR hCursor = GetCursor();
	HCURSOR hCursorArrow = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	//pDC->FillSolidRect(rectClip, m_cr3DFace);
	pDC->FillSolidRect(rectClip, RGB(255,255,255));
	// JCW_U 2008.04.12 END

	int iItems = GetItemCount();
	ASSERT(iItems >= 0);

	CPen penHighLight(PS_SOLID, 1, m_cr3DHighLight);
	CPen penShadow(PS_SOLID, 1, m_cr3DShadow);
	CPen* pPen = pDC->GetCurrentPen();

	// JCW_U 2008.04.12 START
	CFont* pFont = pDC->SelectObject(m_pTextFont);
	//CFont* pFont = pDC->SelectObject(GetFont());
	// JCW_U 2008.04.12 END

	pDC->SetBkColor(m_cr3DFace);
	pDC->SetTextColor(m_crBtnText);

	int iWidth = 0;

	for (int i = 0; i < iItems; i++)
	{
		int iItem = OrderToIndex(i);

		TCHAR szText[FLATHEADER_TEXT_MAX];

		HDITEM hditem;
		hditem.mask = HDI_WIDTH|HDI_FORMAT|HDI_TEXT|HDI_IMAGE|HDI_BITMAP;
		hditem.pszText = szText;
		hditem.cchTextMax = sizeof(szText);
		VERIFY(GetItem(iItem, &hditem));
		VERIFY(GetItemRect(iItem, rectItem));

		// JCW_U 2008.04.12 START
		DWORD dwStyle = ::GetWindowLong(GetParent()->GetSafeHwnd(), GWL_STYLE);	
		POINT pointMouse;
		::GetCursorPos(&pointMouse);
		::ScreenToClient(m_hWnd, &pointMouse);			

		if( !(dwStyle&LVS_NOSORTHEADER) && 
			rectItem.PtInRect(pointMouse) && 
			hCursor == hCursorArrow )
		{
			if( !m_bLMouseDown )
				DrawMouseOver(pDC, rectItem);
			else
				DrawMouseDown(pDC, rectItem);
		}
		else
			DrawMouseOut(pDC, rectItem);
		// JCW_U 2008.04.12 END

		if (rectItem.right >= rectClip.left || rectItem.left <= rectClip.right)
		{
			if (hditem.fmt & HDF_OWNERDRAW)
			{
				DRAWITEMSTRUCT disItem;
				disItem.CtlType = ODT_BUTTON;
				disItem.CtlID = GetDlgCtrlID();
				disItem.itemID = iItem;
				disItem.itemAction = ODA_DRAWENTIRE;
				disItem.itemState = 0;
				disItem.hwndItem = m_hWnd;
				disItem.hDC = pDC->m_hDC;
				disItem.rcItem = rectItem;
				disItem.itemData = 0;

				DrawItem(&disItem);
			}
			else
			{
				rectItem.DeflateRect(m_iSpacing, 0);
				DrawItem(pDC, rectItem, &hditem);
				rectItem.InflateRect(m_iSpacing, 0);

				//if (m_nClickFlags & MK_LBUTTON && m_iHotIndex == iItem && m_hdhtiHotItem.flags & HHT_ONHEADER)
				//	pDC->InvertRect(rectItem);
			}

			if (i < iItems-1)
			{
				// JCW_U 2008.04.12 START
				BITMAP bmpInfo1, bmpInfo2;
				// 비트맵 핸들이 가진 비트맵 정보를 BITMAP 구조체에 저장한다.
				::GetObject(m_hBitampDivision, sizeof(BITMAP), &bmpInfo1);
				::GetObject(m_hBitampShadow, sizeof(BITMAP), &bmpInfo2);
				 				
 				CDC tmpDC;
 				tmpDC.CreateCompatibleDC(pDC);
 				tmpDC.SelectObject(m_hBitampDivision);
 				pDC->BitBlt(rectItem.right, rectItem.bottom - bmpInfo1.bmHeight - bmpInfo2.bmHeight, bmpInfo1.bmWidth, bmpInfo1.bmHeight, &tmpDC, 0, 0, SRCCOPY);
 				tmpDC.DeleteDC();
				// JCW_U 2008.04.12 END
			}
		}
		iWidth += hditem.cxy;
	}

	pDC->SelectObject(pFont);
	pDC->SelectObject(pPen);

	penHighLight.DeleteObject();
	penShadow.DeleteObject();
}

///////////////////////////////////////////////////////////////////////////////
// DrawItem
void CAguraHeaderCtrl::DrawItem(LPDRAWITEMSTRUCT)
{
	ASSERT(FALSE);  // must override for self draw header controls
}

///////////////////////////////////////////////////////////////////////////////
// DrawItem
void CAguraHeaderCtrl::DrawItem(CDC* pDC, CRect rect, LPHDITEM lphdi)
{
	ASSERT(lphdi->mask & HDI_FORMAT);

	int iWidth = 0;

	CBitmap* pBitmap = NULL;
	BITMAP BitmapInfo;

	if (lphdi->fmt & HDF_BITMAP)
	{
		ASSERT(lphdi->mask & HDI_BITMAP);
		ASSERT(lphdi->hbm);

		pBitmap = CBitmap::FromHandle(lphdi->hbm);
		if (pBitmap)
			VERIFY(pBitmap->GetObject(sizeof(BITMAP), &BitmapInfo));
	}

	rect.left += ((iWidth = DrawImage(pDC, rect, lphdi, FALSE)) != 0) ? iWidth + m_iSpacing : 0;
	rect.right -= ((iWidth = DrawBitmap(pDC, rect, lphdi, pBitmap, &BitmapInfo, TRUE)) != 0) ? 
		iWidth + m_iSpacing : 0;

	DrawText(pDC, rect, lphdi);
}

///////////////////////////////////////////////////////////////////////////////
// DrawImage
int CAguraHeaderCtrl::DrawImage(CDC* pDC, CRect rect, LPHDITEM lphdi, BOOL bRight)
{
	CImageList* pImageList = GetImageList();
	int iWidth = 0;

	if (lphdi->iImage != AGURAHEADERCTRL_NO_IMAGE)
	{
		if (pImageList)
		{
			if (rect.Width() > 0)
			{
				POINT point;

				point.y = rect.CenterPoint().y - (m_sizeImage.cy >> 1);

				if (bRight)
					point.x = rect.right - m_sizeImage.cx;
				else
					point.x = rect.left;

				SIZE size;
				size.cx = rect.Width()<m_sizeImage.cx ? rect.Width():m_sizeImage.cx;
				size.cy = m_sizeImage.cy;

				// save image list background color
				COLORREF rgb = pImageList->GetBkColor();

				// set image list background color to same as header control
				pImageList->SetBkColor(pDC->GetBkColor());
				pImageList->DrawIndirect(pDC, lphdi->iImage, point, size, CPoint(0, 0));
				pImageList->SetBkColor(rgb);

				iWidth = m_sizeImage.cx;
			}
		}
	}

	return iWidth;
}

///////////////////////////////////////////////////////////////////////////////
// DrawBitmap
int CAguraHeaderCtrl::DrawBitmap(CDC* pDC, 
								CRect rect, 
								LPHDITEM lphdi, 
								CBitmap* pBitmap, 
								BITMAP* pBitmapInfo, 
								BOOL bRight)
{
	UNUSED_ALWAYS(lphdi);

	int iWidth = 0;

	if (pBitmap)
	{
		iWidth = pBitmapInfo->bmWidth;
		if (iWidth <= rect.Width() && rect.Width() > 0)
		{
			POINT point;

			// JCW_U 2008.04.12 START
			//point.y = rect.CenterPoint().y - (pBitmapInfo->bmHeight >> 1);
			point.y = rect.bottom - pBitmapInfo->bmHeight - 10;
			// JCW_U 2008.04.12 END

			if (bRight)
				point.x = rect.right - iWidth;
			else
				point.x = rect.left;

			CDC dc;
			if (dc.CreateCompatibleDC(pDC) == TRUE) 
			{
				VERIFY(dc.SelectObject(pBitmap));
				iWidth = pDC->BitBlt(
					point.x, point.y, 
					pBitmapInfo->bmWidth, pBitmapInfo->bmHeight, 
					&dc, 
					0, 0, 
					SRCCOPY
				) ? iWidth:0;
			}
			else 
				iWidth = 0;
		}
		else
			iWidth = 0;
	}

	return iWidth;
}

///////////////////////////////////////////////////////////////////////////////
// DrawText
int CAguraHeaderCtrl::DrawText(CDC* pDC, CRect rect, LPHDITEM lphdi)
{
	// JCW_+ 2008.04.12 START
	pDC->SetBkMode(TRANSPARENT);
	// JCW_+ 2008.04.12 END

	CSize size;

	//pDC->SetTextColor(RGB(0,0,255));
	pDC->SetTextColor(m_colorHeader);

	if (rect.Width() > 0 && lphdi->mask & HDI_TEXT && lphdi->fmt & HDF_STRING)
	{
		size = pDC->GetTextExtent(lphdi->pszText);

		// JCW_U 2008.04.12 START
		rect.top = 5;
		// always center column headers
		pDC->DrawText(lphdi->pszText, -1, rect, 
			DT_CENTER|DT_END_ELLIPSIS|DT_SINGLELINE/*|DT_VCENTER*/);
		// JCW_U 2008.04.12 END
	}

	size.cx = rect.Width()>size.cx ? size.cx:rect.Width();
	return size.cx>0 ? size.cx:0;
}

///////////////////////////////////////////////////////////////////////////////
// OnInsertItem
LRESULT CAguraHeaderCtrl::OnInsertItem(WPARAM, LPARAM)
{
	return Default();
}

///////////////////////////////////////////////////////////////////////////////
// OnDeleteItem
LRESULT CAguraHeaderCtrl::OnDeleteItem(WPARAM, LPARAM)
{
	return Default();
}

///////////////////////////////////////////////////////////////////////////////
// OnSetImageList
LRESULT CAguraHeaderCtrl::OnSetImageList(WPARAM, LPARAM lParam)
{
	CImageList* pImageList;
	pImageList = CImageList::FromHandle((HIMAGELIST)lParam);

	IMAGEINFO info;
	if (pImageList->GetImageInfo(0, &info))
	{
		m_sizeImage.cx = info.rcImage.right - info.rcImage.left;
		m_sizeImage.cy = info.rcImage.bottom - info.rcImage.top;
	}

	return Default();
}

///////////////////////////////////////////////////////////////////////////////
// OnLayout
LRESULT CAguraHeaderCtrl::OnLayout(WPARAM, LPARAM lParam)
{
// JCW_U 2008.04.12 START
#if 1
	LRESULT lResult = CHeaderCtrl::DefWindowProc(HDM_LAYOUT, 0, lParam);

	if( m_nHeight > 0 )
	{
		LPHDLAYOUT lphdlayout = (LPHDLAYOUT)lParam;
		RECT *prc = lphdlayout->prc;
		WINDOWPOS *pwpos = lphdlayout->pwpos;

		pwpos->cy = m_nHeight;
		prc->top = m_nHeight;
	}
	return lResult;	
#else
	LPHDLAYOUT lphdlayout = (LPHDLAYOUT)lParam;

	if (m_bStaticBorder)
		lphdlayout->prc->right += GetSystemMetrics(SM_CXBORDER)*2;

	return CHeaderCtrl::DefWindowProc(HDM_LAYOUT, 0, lParam);
#endif
// JCW_U 2008.04.12 END
}

///////////////////////////////////////////////////////////////////////////////
// OnSysColorChange
void CAguraHeaderCtrl::OnSysColorChange() 
{
	TRACE(_T("in CAguraHeaderCtrl::OnSysColorChange\n"));

	CHeaderCtrl::OnSysColorChange();
	
	m_cr3DHighLight = ::GetSysColor(COLOR_3DHIGHLIGHT);
	m_cr3DShadow    = ::GetSysColor(COLOR_3DSHADOW);
	m_cr3DFace      = ::GetSysColor(COLOR_3DFACE);
	m_crBtnText     = ::GetSysColor(COLOR_BTNTEXT);
}

///////////////////////////////////////////////////////////////////////////////
// OnEraseBkgnd
BOOL CAguraHeaderCtrl::OnEraseBkgnd(CDC* pDC) 
{
	UNUSED_ALWAYS(pDC);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// OnPaint
void CAguraHeaderCtrl::OnPaint() 
{
    CPaintDC dc(this);

	CDC memDC;
	CBitmap memBmp;
	CBitmap *pOldBmp;

	CRect rt;
	GetClientRect(&rt);

	memDC.CreateCompatibleDC(&dc);
	memBmp.CreateCompatibleBitmap(&dc, rt.Width(), rt.Height());

	pOldBmp = memDC.SelectObject(&memBmp);

    DrawCtrl(&memDC);

	dc.BitBlt(0, 0, rt.Width(), rt.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBmp);

	memBmp.DeleteObject();
	memDC.DeleteDC();
}

// JCW_+ 2008.04.12 START
void CAguraHeaderCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	//
    if(!m_bTracking) 
	{
        TRACKMOUSEEVENT t = { sizeof(TRACKMOUSEEVENT), TME_LEAVE, m_hWnd, 0 };
        if (::_TrackMouseEvent(&t)) 
		{
            m_bTracking = TRUE;
        }
    }
	else
	{
		Invalidate();
	}

	CHeaderCtrl::OnMouseMove(nFlags, point);
}

LRESULT CAguraHeaderCtrl::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_bTracking = FALSE;
    Invalidate();
	return 0L;
}

void CAguraHeaderCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bLMouseDown = TRUE;

	CHeaderCtrl::OnLButtonDown(nFlags, point);
}

void CAguraHeaderCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bLMouseDown = FALSE;

	CHeaderCtrl::OnLButtonDown(nFlags, point);
}

///////////////////////////////////////////////////////////////////////////////
// DrawMouseOver
void CAguraHeaderCtrl::DrawMouseOver(CDC* pDC, CRect rect)
{
	BITMAP bmpInfo;
	::GetObject(m_hBitampShadow, sizeof(BITMAP), &bmpInfo);

	CDC tmpDC;
	tmpDC.CreateCompatibleDC(pDC);
	tmpDC.SelectObject(m_hBitampShadow);
	pDC->StretchBlt(rect.left, rect.bottom-bmpInfo.bmHeight, rect.Width(), bmpInfo.bmHeight, &tmpDC, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, SRCCOPY);
	tmpDC.DeleteDC();
}

///////////////////////////////////////////////////////////////////////////////
// DrawMouseOut
void CAguraHeaderCtrl::DrawMouseOut(CDC* pDC, CRect rect)
{
	BITMAP bmpInfo;
	::GetObject(m_hBitampShadow, sizeof(BITMAP), &bmpInfo);

	CDC tmpDC;
	tmpDC.CreateCompatibleDC(pDC);
	tmpDC.SelectObject(m_hBitampShadow);
	pDC->StretchBlt(rect.left, rect.bottom-bmpInfo.bmHeight, rect.Width(), bmpInfo.bmHeight, &tmpDC, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, SRCCOPY);
	tmpDC.DeleteDC();
}

///////////////////////////////////////////////////////////////////////////////
// DrawMouseDown
void CAguraHeaderCtrl::DrawMouseDown(CDC* pDC, CRect rect)
{
	BITMAP bmpInfo;
	::GetObject(m_hBitampShadow, sizeof(BITMAP), &bmpInfo);

	CDC tmpDC;
	tmpDC.CreateCompatibleDC(pDC);
	tmpDC.SelectObject(m_hBitampShadow);
	pDC->StretchBlt(rect.left, rect.bottom-bmpInfo.bmHeight, rect.Width(), bmpInfo.bmHeight, &tmpDC, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, SRCCOPY);
	tmpDC.DeleteDC();
}
// JCW_+ 2008.04.12 END
