////////////////////////////////////////////////////////////////
// PixieLib(TM) Copyright 1997-2005 Paul DiLascia
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
// Compiles with Visual Studio.NET 7.1 or greater. Set tabsize=3.
//

//////////////////
// Helper class to build a dialog template in memory. Only supports what's
// needed for CStringDialog.
//
class CDlgTemplateBuilder {
protected:
	WORD* m_pBuffer;							 // internal buffer holds dialog template
	WORD* m_pNext;								 // next WORD to copy stuff
	WORD* m_pEndBuf;							 // end of buffer

	// align ptr to nearest DWORD
	WORD* AlignDWORD(WORD* ptr) {
		ptr++;									 // round up to nearest DWORD
		LPARAM lp = (LPARAM)ptr;			 // convert to long
		lp &= 0xFFFFFFFC;						 // make sure on DWORD boundary
		return (WORD*)lp;
	}

	void AddItemTemplate(WORD wType, DWORD dwStyle, const CRect& rc,
		WORD nID, DWORD dwStyleEx);

public:
	// Windows predefined atom names
	enum { BUTTON=0x0080, EDIT, STATIC, LISTBOX, SCROLLBAR, COMBOBOX };

	CDlgTemplateBuilder(UINT nBufLen=1024);
	~CDlgTemplateBuilder();

	DLGTEMPLATE* GetTemplate() { return (DLGTEMPLATE*)m_pBuffer; }

	// functions to build the template
	DLGTEMPLATE* Begin(DWORD dwStyle, const CRect& rc, LPCTSTR caption, DWORD dwStyleEx=0);
	WORD* AddText(WORD* buf, LPCTSTR text);
	void AddItem(WORD wType, DWORD dwStyle, const CRect& rc,
		LPCTSTR text, WORD nID=-1, DWORD dwStyleEx=0);
	void AddItem(WORD wType, DWORD dwStyle, const CRect& rc,
		WORD nResID, WORD nID=-1, DWORD dwStyleEx=0);
};
