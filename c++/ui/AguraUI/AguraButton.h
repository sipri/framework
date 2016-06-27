
#ifndef _SKINBUTTON_H
#define _SKINBUTTON_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <GdiPlus.h>
#include <comutil.h>

#pragma comment(lib, "gdiplus")
#pragma comment(lib, "comsuppw.lib")

using namespace Gdiplus;

class CAguraButton : public CMFCButton
{
public:
	enum IMG {eNone, eLeft, eCenter};

    CAguraButton();
	~CAguraButton();

	BOOL	m_bIsFile;
	BOOL	m_bIsCaptionFile;
	BOOL	m_bHover;
	int		m_iImagePosition;
// 	POINT	m_ptBitmapOrg;
// 	POINT	m_ptIconOrg;
	POINT	m_ptPressedOffset;

	CString m_strNormalPath;
	CString m_strHoverPath;
	CString m_strClickPath;
	CString	m_strCaptionPath;

	int		m_iNoramlResource;
	int		m_iHoverResource;
	int		m_iClickResource;
	int		m_iCaptionResource;

	COLORREF m_clrBK;
	COLORREF m_clrText;
	ULONG_PTR m_gdiplusToken;

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DWORD SetDefaultColors(BOOL bRepaint = TRUE);

	void setCaptionImage(int iPosition, CString strCaption);
	void setCaptionImage(int iPosition, int iCaption);

	void setButtonImage(CString strNormal, CString strHover, CString strClick);
	void setButtonImage(int iNormal, int iHover, int iClick);

	void setButtonColor(COLORREF clrBK, COLORREF clrText);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();

protected:
//	BOOL		m_bMouseOnButton;				// Is mouse over the button?
	BOOL		m_bIsPressed;					// Is button pressed?
	BOOL		m_bIsFocused;					// Is button focused?
	BOOL		m_bIsDisabled;					// Is button disabled?
	BOOL		m_bIsDefault;
	BYTE		m_byAlign;						// Align mode
	UINT		m_nTypeStyle;					// Button style

	virtual void PreSubclassWindow();
	afx_msg void OnSysColorChange();
	afx_msg UINT OnGetDlgCode();
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);

private:
	LRESULT OnSetStyle(WPARAM wParam, LPARAM lParam);
	virtual DWORD DrawBackground(CDC* pDC, CRect* pRect);
	
	/*LDRA_INSPECTED 183 S */
	/*LDRA_INSPECTED 326 S */
	DECLARE_MESSAGE_MAP() //매크로 부분으로 예외처리 
};

#endif//_SKINBUTTON_H
