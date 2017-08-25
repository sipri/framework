#pragma once

/************************************************************
 * Agura Button
 ************************************************************/
class CAguraButton : public CButton
{
public:
	// Constructor
    CAguraButton();
	// Destructor
	~CAguraButton();

	// Pre-subclass Window
	virtual void PreSubclassWindow();
	// Draw Item
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
};
