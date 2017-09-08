
// WinControllerDlg.h : header file
//

#pragma once


// CWinControllerDlg dialog
class CWinControllerDlg : public CDialogEx
{
// Construction
public:
	CWinControllerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WINCONTROLLER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	static LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseMoveOnScreen(WPARAM wParam, LPARAM lParam);

private:
	static HWND m_hWnd;
	static HHOOK m_hMouseHook;
	CStatic m_MousePos;
	CStatic m_Hwnd;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
