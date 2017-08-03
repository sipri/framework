#pragma once

/************************************************************
 *	@brief		Agura Edit
 ************************************************************/
class CAguraEdit : public CEdit
{
public:
	// Constructor
	CAguraEdit();
	// Destructor
	virtual ~CAguraEditBox();

	// Pre-Translate Message
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
