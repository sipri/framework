#pragma once

// CAguraDlgBlue 대화 상자입니다.
#include "AguraButton.h"
#include <GdiPlus.h>

#pragma comment(lib, "Gdiplus")

#define SYS_BTN_CX	18
#define SYS_BTN_CY	18

#define DLG_TEXT_COLOR RGB(255, 255, 255)
#define DLG_CTRL_COLOR RGB(48, 48, 48)
#define DLG_FRAME_COLOR RGB(38, 38, 38)

using namespace Gdiplus;

typedef BOOL (WINAPI *lpfn) (HWND hWnd, COLORREF cr, BYTE bAlpha, DWORD dwFlags);

class CAguraDlgBlue : public CDialog
{
	DECLARE_DYNAMIC(CAguraDlgBlue)

public:
	CAguraDlgBlue(UINT nIDTemplate, CWnd* pParent = NULL);
	CAguraDlgBlue(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAguraDlgBlue();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_AGURA_BLUE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	enum BTN {eMin, eMax, eExit};

	CFont				m_fontStatic;
	CFont*				m_pFont;
	HBRUSH				m_hbr;
	HICON				m_hIcon;
	ULONG_PTR			m_gdiplusToken;
	CAguraButton*		m_pBtn[3];
	CString				m_strTitle;
	BOOL				m_bIsMax;
	BOOL				m_bMove;
	CPoint				m_ptStandard;
	CRect				m_rtTitle;
	lpfn				SetLayeredWindowAttributes;

	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	void gradationCenter(Graphics& g, CRect rt, Color& color1, Color& color2, BOOL bLine = FALSE, Color& color3 = Color(0, 0, 0));
	void DrawTextBackgroundLight(CDC& pDC,int x,int y,CString str);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
