#ifndef __CINIFILE_H__
#define __CINIFILE_H__


extern CString GetFirstParam(CString& strName, TCHAR tDelimiter = _T(','));
extern CString GetSpecificParam(CString& strName, BOOL iOption);


class CIniFile
{
	CString m_Name;
public:
	CIniFile(CString m_FName);

	CString ReadString(LPCTSTR m_Sec, LPCTSTR m_Ident, LPCTSTR m_Def);
	BOOL WriteString(LPCTSTR m_Sec, LPCTSTR m_Ident, LPCTSTR m_Val);
	BOOL ReadSections(CStringArray& m_Secs);
	BOOL ReadSection(LPCTSTR m_Sec, CStringArray& m_Secs);
	
};

#endif