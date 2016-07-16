#pragma once

extern UINT NEAR WM_AGURACOMBOBOX_CHANGE;

class CAguraComboBox : public CComboBox
{
// Construction
public:
	CAguraComboBox();

// Attributes
private:
	int m_nVK;

	// Singleton instance
	static CAguraComboBox* m_pAguraComboBox;

// Operations
public:
	// Returns the instance of the class
	static CAguraComboBox* GetInstance();

	// Deletes the instance of the class
	static void DeleteInstance();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAguraComboBox)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAguraComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAguraComboBox)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnCbnCloseup();
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
