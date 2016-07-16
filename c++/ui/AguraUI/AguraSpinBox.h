#pragma once

extern UINT NEAR WM_AGURASPINBOX_CHANGING;
extern UINT NEAR WM_AGURASPINBOX_DELTA;

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
