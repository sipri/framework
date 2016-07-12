// AguraSpinBox.cpp : implementation file
//

#include "stdafx.h"
#include "AguraSpinBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SPIN_MODE_UP		1
#define SPIN_MODE_DOWN		0
#define SPIN_MODE_NOTUSE	-1

UINT NEAR WM_AGURASPINBOX_CHANGING = ::RegisterWindowMessage(_T("WM_AGURASPINBOX_CHANGING"));
UINT NEAR WM_AGURASPINBOX_DELTA = ::RegisterWindowMessage(_T("WM_AGURASPINBOX_DELTA"));

/////////////////////////////////////////////////////////////////////////////
// CAguraSpinBox

CAguraSpinBox* CAguraSpinBox::m_pAguraSpinBox = NULL; 

CAguraSpinBox::CAguraSpinBox()
{
}

CAguraSpinBox::~CAguraSpinBox()
{
}

BEGIN_MESSAGE_MAP(CAguraSpinBox, CSpinButtonCtrl)
	//{{AFX_MSG_MAP(CAguraSpinBox)
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(UDN_DELTAPOS, OnDeltapos)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAguraSpinBox message handlers

CAguraSpinBox* CAguraSpinBox::GetInstance()
{
	if(m_pAguraSpinBox == NULL)
	{
		m_pAguraSpinBox = new CAguraSpinBox;
	}
	return m_pAguraSpinBox;
}

void CAguraSpinBox::DeleteInstance()
{
	if(m_pAguraSpinBox != NULL)
	{
		if(m_pAguraSpinBox->m_hWnd != NULL)
			m_pAguraSpinBox->DestroyWindow();

		delete m_pAguraSpinBox;
		m_pAguraSpinBox = NULL;
	}
}

void CAguraSpinBox::OnDeltapos(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	*pResult = 1;

	int nLower=0, nUpper=0;
	GetRange(nLower, nUpper);

	int nPos = pNMUpDown->iPos;

	// Down
	if( pNMUpDown->iDelta < 0 )
	{
		//TRACE2("Down : iDelta(%d), iPos(%d)\n", pNMUpDown->iDelta, pNMUpDown->iPos);
		nPos -= 1;
		if( nPos < nLower )
			nPos = nLower;

		::PostMessage(GetParent()->m_hWnd, WM_AGURASPINBOX_DELTA, (WPARAM)FALSE, (LPARAM)NULL);
	}
	// Up
	else if( pNMUpDown->iDelta > 0 )
	{
		//TRACE2("Up : iDelta(%d), iPos(%d)\n", pNMUpDown->iDelta, pNMUpDown->iPos);
		nPos += 1;
		if( nPos > nUpper )
			nPos = nUpper;

		::PostMessage(GetParent()->m_hWnd, WM_AGURASPINBOX_DELTA, (WPARAM)TRUE, (LPARAM)NULL);
	}
	else /*if( pNMUpDown->iDelta == 0 ) */
	{
		return;
	}

	int nResult = 0;
	::SendMessage(GetParent()->m_hWnd, WM_AGURASPINBOX_CHANGING, (WPARAM)nPos, (LPARAM)&nResult);

	if( nResult == 0 )
		SetPos(nPos);
}