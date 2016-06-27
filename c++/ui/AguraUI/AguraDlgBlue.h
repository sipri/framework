#pragma once

// CAguraDlgBlue ��ȭ �����Դϴ�.
#include "AguraButton.h"
#include <GdiPlus.h>

#pragma comment(lib, "Gdiplus")

#define SYS_BTN_CX	18
#define SYS_BTN_CY	18

using namespace Gdiplus;

class CAguraDlgBlue : public CDialog
{
	DECLARE_DYNAMIC(CAguraDlgBlue)

public:
	CAguraDlgBlue(UINT nIDTemplate, CWnd* pParent = NULL);
	CAguraDlgBlue(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CAguraDlgBlue();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLG_AGURA_BLUE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	enum BTN {eMin, eMax, eExit};

	HICON				m_hIcon;
	ULONG_PTR			m_gdiplusToken;
	CAguraButton*		m_pBtn[3];
	CString				m_strTitle;
	BOOL				m_bIsMax;
	BOOL				m_bMove;
	CPoint				m_ptStandard;
	CRect				m_rtTitle;

	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	void gradationCenter(Graphics& g, CRect rt, Color& color1, Color& color2, BOOL bLine = FALSE, Color& color3 = Color(0, 0, 0));
	void DrawTextBackgroundLight(CDC& pDC,int x,int y,CString str);
};
