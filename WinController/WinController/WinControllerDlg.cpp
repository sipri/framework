#include "stdafx.h"
#include "WinController.h"
#include "WinControllerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define WM_MOUSEMOVE_ON_SCREEN	(WM_USER + 100)

HWND CWinControllerDlg::m_hWnd;
HHOOK CWinControllerDlg::m_hMouseHook;

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}



BEGIN_MESSAGE_MAP(CWinControllerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_MOUSEMOVE_ON_SCREEN, &CWinControllerDlg::OnMouseMoveOnScreen)
	ON_WM_TIMER()
END_MESSAGE_MAP()


CWinControllerDlg::CWinControllerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_WINCONTROLLER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWinControllerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WCNT_MOUSE_POS, m_MousePos);
	DDX_Control(pDX, IDC_WCNT_HWND, m_Hwnd);
}


BOOL CWinControllerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_hWnd = GetSafeHwnd();
	//m_hMouseHook = SetWindowsHookEx(WH_MOUSE, MouseProc, NULL, GetCurrentThreadId());

	SetTimer(0, 100, NULL);


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CWinControllerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void CWinControllerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CWinControllerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CALLBACK CWinControllerDlg::MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
//	if (wParam == WM_MOUSEMOVE)
//	{
//		::PostMessage(m_hWnd, WM_MOUSEMOVE_ON_SCREEN, NULL, NULL);
//	}

    return CallNextHookEx(m_hMouseHook, nCode, wParam, lParam);
}

LRESULT CWinControllerDlg::OnMouseMoveOnScreen(WPARAM wParam, LPARAM lParam)
{
	POINT ptCursor;
	GetCursorPos(&ptCursor);

	CString strPos;
	strPos.Format(_T("(%d, %d)"), ptCursor.x, ptCursor.y);
	m_MousePos.SetWindowText(strPos);

	POINT pt;
	pt.x = ptCursor.x;
	pt.y = ptCursor.y;

	HWND hWnd = NULL;
	CWnd* pWnd = WindowFromPoint(pt);
	if (pWnd != NULL)
		hWnd = pWnd->GetSafeHwnd();

	CString strHwnd;
	strHwnd.Format(_T("0x%08x"), hWnd);
	m_Hwnd.SetWindowText(strHwnd);

	return 0;
}


void CWinControllerDlg::OnTimer(UINT_PTR nIDEvent)
{
	POINT ptCursor;
	GetCursorPos(&ptCursor);

	CString strPos;
	strPos.Format(_T("(%d, %d)"), ptCursor.x, ptCursor.y);
	m_MousePos.SetWindowText(strPos);

	POINT pt;
	pt.x = ptCursor.x;
	pt.y = ptCursor.y;

	HWND hWnd = NULL;
	CWnd* pWnd = WindowFromPoint(pt);
	if (pWnd != NULL)
		hWnd = pWnd->GetSafeHwnd();

	CString strHwnd;
	strHwnd.Format(_T("0x%08x"), hWnd);
	m_Hwnd.SetWindowText(strHwnd);

	CDialogEx::OnTimer(nIDEvent);
}
