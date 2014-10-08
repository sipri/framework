#include "stdafx.h"
#include <Afx.h>
#include <AfxColl.h>

#include "IniFile.H"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CIniFile::CIniFile(CString m_FName)
{
	m_Name = m_FName;
}

//==============================================================================
// 
// Function Name : ReadString(CString m_Sec, CString m_Ident, CString m_Def)
// Description   : 섹션 내의 KEY값 읽기 
// Parameters    : 
//					CString m_Sec	: [in] 섹션의 이름 => [MAIN_BACK_IMAGE_SECTION]
//					CString m_Ident : [in] 섹션의 Key이름
//					CString m_Def	: [in] 기본 값

// Return        : CString
//						섹션내의 key에 할당된 값(문자열)
// Initial Date  : 2011.07.22
// Notes         : ini파일 안에서 해당 섹션의 key에 할당된 값(문자열)을 읽어온다.
// 
//=============================================================================
CString CIniFile::ReadString(LPCTSTR m_Sec, LPCTSTR m_Ident, LPCTSTR m_Def)
{
	TCHAR Buffer[2048];

	GetPrivateProfileString(m_Sec, m_Ident,m_Def, Buffer, _countof(Buffer), m_Name);
	return Buffer;
}

BOOL CIniFile::WriteString(LPCTSTR  m_Sec, LPCTSTR  m_Ident, LPCTSTR  m_Val)
{
	return WritePrivateProfileString(m_Sec, m_Ident, m_Val, m_Name);
}

BOOL CIniFile::ReadSections(CStringArray& m_Secs)
{
	LPVOID pvData = NULL;
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, 16385);
	_ASSERTE(NULL != hGlobal);

	pvData = GlobalLock(hGlobal);
	_ASSERTE(NULL != pvData);

	m_Secs.RemoveAll();
	
	if (GetPrivateProfileString(NULL, NULL, NULL, (TCHAR*) pvData, 16384, m_Name))
	{
        TCHAR *P = (TCHAR*) pvData;
        while (1)
		{
			// NULL 체크
			if (!P) {
				break;
			}

			if (*P == 0) {
				break;
			}

			m_Secs.Add(P);
			P += _tcsclen(P) + 1;
		}
	}
	GlobalUnlock(hGlobal);
	GlobalFree(hGlobal);
	return m_Secs.GetSize() > 0;
}

//==============================================================================
// 
// Function Name : ReadSection(LPCTSTR m_Sec, CStringArray& m_Secs)
// Description   : 섹션 내의 모든 Key 읽기 
// Parameters    : 
//					LPCTSTR m_Sec			: [in] 섹션의 이름 => [MAIN_BACK_IMAGE_SECTION]
//					CStringArray& m_Secs	: [out] 섹션이 가지고 있는 모든 Key이름
// Return        : BOOL
//						0: 실패
// Initial Date  : 2011.07.22
// Notes         : ini파일 안에서 해당 섹션의 모든key들의 이름을 읽어온다.
// 
//=============================================================================
BOOL CIniFile::ReadSection(LPCTSTR m_Sec, CStringArray& m_Secs)
{
	LPVOID pvData = NULL;
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, 16385);
	_ASSERTE(NULL != hGlobal);

	pvData = GlobalLock(hGlobal);
	_ASSERTE(NULL != pvData);

	m_Secs.RemoveAll();
	
	if (GetPrivateProfileString(m_Sec, NULL, NULL, (TCHAR*) pvData, 16384, m_Name))
	{
        TCHAR *P = (TCHAR*) pvData;
        while (1)
		{
			// NULL 체크
			if (!P) {
				break;
			}

			if (*P == 0) {
				break;
			}

			m_Secs.Add(P);
			P += _tcsclen(P) + 1;
		}
	}
	GlobalUnlock(hGlobal);
	GlobalFree(hGlobal);
	return m_Secs.GetSize() > 0;
}

CString GetFirstParam(CString& strName, TCHAR tDelimiter /*= _T(',')*/)
{
	int P = strName.Find(tDelimiter);
	TCHAR m_Res[1024] = {0,};

	_stprintf_s(m_Res,_countof(m_Res), _T("%s"), strName);
	if(P < 0)
	{
		return m_Res;
	}
	m_Res[P] = _T('\0');

	strName.Delete(0, P + 1);
	strName.TrimLeft();
	strName.TrimRight();

	return m_Res;
}

// ','로 구별된 Key값에서 필요한 위치의 값을 읽어오는 함수
// iOption에 위치를 넣어준다(0이 첫번지)
CString GetSpecificParam(CString& strSource, BOOL iOption)
{	
	CString			rtnString = _T(""), szTemp = _T("");

#if 0
	BOOL			iStart = 0;
	BOOL			iEnd	= -1;
	iEnd = strName.Find(_T(','), iStart);

	//쉽표가 없이 값이 1개 일때
	if( iEnd < 0 )
		return strName;

	if(iOption == 0)
	{
		rtnString = strName.Mid(iStart, iEnd - iStart);
		return rtnString;
	}
	else
	{
		for(BOOL index = 1; index <= iOption; index++)
		{
			iStart = iEnd + 1;
			iEnd = strName.Find(_T(','), iStart);

			if(iEnd < 0)
				iEnd = strName.GetLength() ;
		}
	}

	rtnString = strName.Mid(iStart, iEnd - iStart);
#endif

	szTemp = strSource;
	for (int i=0; i<iOption; i++ )
	{
		rtnString = GetFirstParam(szTemp);
	}

	return rtnString;
}

