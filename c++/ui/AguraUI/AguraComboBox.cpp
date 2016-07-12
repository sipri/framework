// AguraComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "AguraComboBox.h"
#include ".\AguraComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UINT NEAR WM_AGURACOMBOBOX_CHANGE = ::RegisterWindowMessage(_T("WM_AGURACOMBOBOX_CHANGE"));

/////////////////////////////////////////////////////////////////////////////
// CAguraComboBox

CAguraComboBox* CAguraComboBox::m_pAguraComboBox = NULL; 

CAguraComboBox::CAguraComboBox()
{
}

CAguraComboBox::~CAguraComboBox()
{
}


BEGIN_MESSAGE_MAP(CAguraComboBox, CComboBox)
	//{{AFX_MSG_MAP(CAguraComboBox)
	ON_WM_KILLFOCUS()
	ON_CONTROL_REFLECT(CBN_CLOSEUP, OnCbnCloseup)
//	ON_WM_CREATE()
	//}}AFX_MSG_MAP	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAguraComboBox message handlers

CAguraComboBox* CAguraComboBox::GetInstance()
{
	if(m_pAguraComboBox == NULL)
	{
		m_pAguraComboBox = new CAguraComboBox;
	}
	return m_pAguraComboBox;
}

void CAguraComboBox::DeleteInstance()
{
	if(m_pAguraComboBox != NULL)
	{
		if(m_pAguraComboBox->m_hWnd != NULL)
			m_pAguraComboBox->DestroyWindow();

		delete m_pAguraComboBox;
		m_pAguraComboBox = NULL;
	}
}

void CAguraComboBox::OnKillFocus(CWnd* pNewWnd) 
{
	CComboBox::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here
	::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
	::SendMessage(GetParent()->m_hWnd, WM_AGURACOMBOBOX_CHANGE, (WPARAM)m_nVK, (LPARAM)NULL);		
}

//-----------------------------------------------------------------------------------------------
// Remember that PreTranslateMessage is only called by a message loop from inside a MFC program.
// If the host is another program for an ActiveX, you can not use PreTranslateMessage!
//-----------------------------------------------------------------------------------------------
BOOL CAguraComboBox::PreTranslateMessage(MSG* pMsg) 
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
	
	return CComboBox::PreTranslateMessage(pMsg);
}
void CAguraComboBox::OnCbnCloseup()
{
	// TODO: Add your message handler code here
	::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
	::SendMessage(::GetParent(m_hWnd), WM_AGURACOMBOBOX_CHANGE, (WPARAM)m_nVK, (LPARAM)NULL);	
}

/*
int CAguraComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here	

	return 0;
}
*/