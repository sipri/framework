
#pragma once

class CWinControllerDlg : public CDialogEx
{
public:
	// Construction
	CWinControllerDlg(CWnd* pParent = NULL);

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WINCONTROLLER_DIALOG };
#endif

	afx_msg void OnTimer(UINT_PTR nIDEvent);

protected:
	// DDX/DDV support
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	HICON m_hIcon;
	CRect m_WindowRect;
	CStatic m_MousePos;
	CStatic m_WindowHwnd;
	CStatic m_WindowPos;
	CStatic m_WindowSize;
};
