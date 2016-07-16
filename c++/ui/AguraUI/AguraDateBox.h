#pragma once

extern UINT NEAR WM_AGURADATEBOX_CHANGE;

class CAguraDateBox : public CDateTimeCtrl
{
// Construction
public:
	CAguraDateBox();

// Attributes
public:
	

private:
	int m_nVK;

	// Singleton instance
	static CAguraDateBox* m_pAguraDateBox;

// Operations
public:
	// Returns the instance of the class
	static CAguraDateBox* GetInstance();

	// Deletes the instance of the class
	static void DeleteInstance();

	static CString GetDate(CTime &time, CString &sDateFormat);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAguraDateBox)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAguraDateBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAguraDateBox)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnCloseup(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
