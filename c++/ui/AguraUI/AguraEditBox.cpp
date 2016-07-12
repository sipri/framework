// AguraEditBox.cpp : implementation file
//

#include "stdafx.h"
#include "AguraEditBox.h"

#include "AguraSpinBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UINT NEAR WM_AGURAEDITBOX_CHANGE = ::RegisterWindowMessage(_T("WM_AGURAEDITBOX_CHANGE"));

/////////////////////////////////////////////////////////////////////////////
// CAguraEditBox

CAguraEditBox* CAguraEditBox::m_pAguraEditBox = NULL; 

CAguraEditBox::CAguraEditBox()
{
	m_nMinNum = -1;
	m_nMaxNum = -1;

	m_nVK = 0;
}

CAguraEditBox::~CAguraEditBox()
{
	CAguraSpinBox::DeleteInstance();
}


BEGIN_MESSAGE_MAP(CAguraEditBox, CEdit)
	//{{AFX_MSG_MAP(CAguraEditBox)
	ON_WM_KILLFOCUS()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(WM_AGURASPINBOX_DELTA, OnSpinDelta)
	ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAguraEditBox message handlers

CAguraEditBox* CAguraEditBox::GetInstance()
{
	if(m_pAguraEditBox == NULL)
	{
		m_pAguraEditBox = new CAguraEditBox;
	}
	return m_pAguraEditBox;
}

void CAguraEditBox::DeleteInstance()
{
	if(m_pAguraEditBox != NULL)
	{
		if(m_pAguraEditBox->m_hWnd != NULL)
			m_pAguraEditBox->DestroyWindow();

		delete m_pAguraEditBox;
		m_pAguraEditBox = NULL;
	}
}

void CAguraEditBox::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here
	::PostMessage(m_hWnd, WM_CLOSE, 0, 0);	
	::SendMessage(GetParent()->m_hWnd, WM_AGURAEDITBOX_CHANGE, (WPARAM)m_nVK, (LPARAM)NULL);	
}

//-----------------------------------------------------------------------------------------------
// Remember that PreTranslateMessage is only called by a message loop from inside a MFC program.
// If the host is another program for an ActiveX, you can not use PreTranslateMessage!
//-----------------------------------------------------------------------------------------------
BOOL CAguraEditBox::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message==WM_KEYDOWN ) 
	{
		// Enter Key
		if( pMsg->wParam == VK_RETURN ||
			pMsg->wParam == VK_ESCAPE ||
			pMsg->wParam == VK_TAB ) 
		{
			m_nVK = pMsg->wParam;
			::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
		}
	}
	
	return CEdit::PreTranslateMessage(pMsg);
}

LRESULT CAguraEditBox::OnSpinDelta(WPARAM wParam, LPARAM lParam)
{
	BOOL bUp = (BOOL)wParam;

	CString sText(_T(""));
	GetWindowText(sText);
	int nValue = _tstoi(sText);

	if( bUp )
	{
		nValue++;
		if( nValue <= m_nMaxNum )
		{
			sText.Format(_T("%d"), nValue);
			SetWindowText(sText);
		}
	}
	else
	{
		nValue--;
		if( nValue >= m_nMinNum )
		{
			sText.Format(_T("%d"), nValue);
			SetWindowText(sText);
		}
	}

	return 0L;
}

int CAguraEditBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here

	// Create Spin Box
	if( m_nMinNum >=0 && m_nMaxNum >=0 )
	{
		ModifyStyle(0, WS_CLIPCHILDREN);	
		CAguraSpinBox::DeleteInstance();
		CAguraSpinBox *pSpinBox = CAguraSpinBox::GetInstance();
		if( pSpinBox )
		{
			CRect rc;
			GetClientRect(&rc);
			rc.left = rc.right - 16;
			pSpinBox->Create(UDS_WRAP | UDS_SETBUDDYINT | UDS_AUTOBUDDY | UDS_NOTHOUSANDS | 
				WS_CHILD | WS_VISIBLE, rc, this, NULL);
			pSpinBox->SetRange(0, 1);
		}
	}

	return 0;
}

void CAguraEditBox::OnPaint()
{
	Default();

	if( m_nMinNum >=0 && m_nMaxNum >=0 &&
		CAguraSpinBox::m_pAguraSpinBox )
	{
		CAguraSpinBox::m_pAguraSpinBox->RedrawWindow();
	}
}
