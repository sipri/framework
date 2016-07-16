#pragma once

extern UINT NEAR WM_AGURAEDITBOX_CHANGE;

class CAguraEditBox : public CEdit
{
// Construction
public:
	CAguraEditBox();

// Attributes
private:
	// Singleton instance
	static CAguraEditBox* m_pAguraEditBox;
	int m_nMinNum;
	int m_nMaxNum;

	int m_nVK;

// Operations
public:
	// Returns the instance of the class
	static CAguraEditBox* GetInstance();

	// Deletes the instance of the class
	static void DeleteInstance();
	
	void SetMinNum(int nMinNum){ m_nMinNum = nMinNum; }
	void SetMaxNum(int nMaxNum){ m_nMaxNum = nMaxNum; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAguraEditBox)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAguraEditBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAguraEditBox)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	//}}AFX_MSG
	afx_msg LRESULT OnSpinDelta( WPARAM wParam, LPARAM lParam );

	DECLARE_MESSAGE_MAP()	
};
