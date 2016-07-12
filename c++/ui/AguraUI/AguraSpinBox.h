#if !defined(AFX_AguraSpinBox_H__94AF797E_D6F6_442A_BE80_E964CEB27EFC__INCLUDED_)
#define AFX_AguraSpinBox_H__94AF797E_D6F6_442A_BE80_E964CEB27EFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AguraSpinBox.h : header file
//

extern UINT NEAR WM_AGURASPINBOX_CHANGING;
extern UINT NEAR WM_AGURASPINBOX_DELTA;

/////////////////////////////////////////////////////////////////////////////
// CAguraSpinBox window

class CAguraSpinBox : public CSpinButtonCtrl
{
// Construction
public:
	CAguraSpinBox();

// Attributes
	// Singleton instance
	static CAguraSpinBox* m_pAguraSpinBox;

// Operations
public:
	// Returns the instance of the class
	static CAguraSpinBox* GetInstance();

	// Deletes the instance of the class
	static void DeleteInstance();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAguraSpinBox)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAguraSpinBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAguraSpinBox)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDeltapos(NMHDR *pNMHDR, LRESULT *pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AguraSpinBox_H__94AF797E_D6F6_442A_BE80_E964CEB27EFC__INCLUDED_)
