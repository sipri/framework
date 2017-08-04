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
	if (pMsg->message == WM_KEYDOWN)
	{
		if (GetKeyState(VK_CONTROL) < 0)
		{
			if (pMsg->wParam == 'A')
			{
				SetSel(0, -1);
				return TRUE;
			}
		}
	}

	return CEdit::PreTranslateMessage(pMsg);
}
