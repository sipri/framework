#include "stdafx.h"
#include "AguraEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/************************************************************
 *	@brief		Constructor
 *	@retval		Nothing
 ************************************************************/
CAguraEdit::CAguraEdit()
{
}

/************************************************************
 *	@brief		Destructor
 *	@retval		Nothing
 ************************************************************/
CAguraEdit::~CAguraEdit()
{
}

/************************************************************
 *	@brief		Pre-Translate Message
 *	@param[in]	pMsg			Message
 *	@retval		TRUE			Success
 *	@retval		FALSE			Failure
 ************************************************************/
BOOL CAguraEdit::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYUP )
	{
		if ((LOWORD(pMsg->wParam) & VK_CONTROL) == VK_CONTROL)
		{
			SetSel(0, -1);
		}
	}

	return CEdit::PreTranslateMessage(pMsg);
}
