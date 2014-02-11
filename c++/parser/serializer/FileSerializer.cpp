#include "stdafx.h"

#include "FileSerializer.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/************************************************************
 *	@brief		생성자
 *	@param[in]	strFileName		파일 이름
 *	@retval		없음
 ************************************************************/
CFileSerializer::CFileSerializer(CString strFileName)
{
	// 파일 열기
	m_pFile = new CFile();

	m_pFile->Open(strFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);


	m_byTemp = 0;

	m_serialized_bytes = 0;

	m_serialized_bits = 0;
}


/************************************************************
 *	@brief		소멸자
 *	@retval		없음
 ************************************************************/
CFileSerializer::~CFileSerializer(void)
{
	if (m_pFile)
	{
		// 파일 핸들 있음
		if (CFile::hFileNull != m_pFile->m_hFile)
		{
			// 파일 닫기
			m_pFile->Close();
		}

		delete m_pFile;

		m_pFile = NULL;
	}
}


/************************************************************
 *	@brief		데이타설정(bool)
 *	@param[in]	pVal			설정데이타
 *	@retval		TRUE			성공
 *	@retval		FALSE			실패
 ************************************************************/
BOOL CFileSerializer::SetData(bool* pVal)
{
	// 파일 핸들 없음
	if (CFile::hFileNull == m_pFile->m_hFile)
	{
		return FALSE;
	}


	// 데이터 설정
	m_byTemp |= *(BYTE*)pVal << m_serialized_bits;

	// Serialized Bits 설정
	m_serialized_bits++;


	// Serialized Bits 가 Byte 단위로 되었을 경우
	if (!(m_serialized_bits % 8))
	{
		// 파일 쓰기
		m_pFile->Write(&m_byTemp, sizeof(BYTE));

		m_byTemp = 0;

		// Serialized Bits 초기화
		m_serialized_bits = 0;

		// Serialized Bytes 설정
		m_serialized_bytes += sizeof(BYTE);
	}


	return TRUE;
}


/************************************************************
 *	@brief		데이타설정(BYTE)
 *	@param[in]	pVal			설정데이타
 *	@retval		TRUE			성공
 *	@retval		FALSE			실패
 ************************************************************/
BOOL CFileSerializer::SetData(BYTE* pVal)
{
	// 파일 핸들 없음
	if (CFile::hFileNull == m_pFile->m_hFile)
	{
		return FALSE;
	}


	m_pFile->Write(pVal, sizeof(BYTE));

	m_serialized_bytes += sizeof(BYTE);


	return TRUE;
}


/************************************************************
 *	@brief		데이타설정(WORD)
 *	@param[in]	pVal			설정데이타
 *	@retval		TRUE			성공
 *	@retval		FALSE			실패
 ************************************************************/
BOOL CFileSerializer::SetData(WORD* pVal)
{
	// 파일 핸들 없음
	if (CFile::hFileNull == m_pFile->m_hFile)
	{
		return FALSE;
	}


	m_pFile->Write(pVal, sizeof(WORD));

	m_serialized_bytes += sizeof(WORD);


	return TRUE;
}


/************************************************************
 *	@brief		데이타설정(DWORD)
 *	@param[in]	pVal			설정데이타
 *	@retval		TRUE			성공
 *	@retval		FALSE			실패
 ************************************************************/
BOOL CFileSerializer::SetData(DWORD* pVal)
{
	// 파일 핸들 없음
	if (CFile::hFileNull == m_pFile->m_hFile)
	{
		return FALSE;
	}


	m_pFile->Write(pVal, sizeof(DWORD));

	m_serialized_bytes += sizeof(DWORD);


	return TRUE;
}


/************************************************************
 *	@brief		데이타설정(void*)
 *	@param[in]	pVal			설정데이타
 *	@param[in]	size			설정데이타사이즈
 *	@retval		TRUE			성공
 *	@retval		FALSE			실패
 ************************************************************/
BOOL CFileSerializer::SetData(void* pVal, int size)
{
	// 파일 핸들 없음
	if (CFile::hFileNull == m_pFile->m_hFile)
	{
		return FALSE;
	}


	m_pFile->Write(pVal, size);

	m_serialized_bytes += size;


	return TRUE;
}


/************************************************************
 *	@brief		Serialize 된 바이트수 설정
 *	@retval		int				Serialize 된 바이트수
 ************************************************************/
void CFileSerializer::SetSerializedBytes(int size)
{
	m_serialized_bytes = size;
}


/************************************************************
 *	@brief		Serialize 된 비트수 설정
 *	@retval		int				Serialize 된 비트수
 ************************************************************/
void CFileSerializer::SetSerializedBits(int size)
{
	m_serialized_bits = size;
}


/************************************************************
 *	@brief		Serialize 된 바이트수 취득
 *	@retval		int				Serialize 된 바이트수
 ************************************************************/
int CFileSerializer::GetSerializedBytes(void)
{
	return m_serialized_bytes;
}


/************************************************************
 *	@brief		Serialize 된 비트수 취득
 *	@retval		int				Serialize 된 비트수
 ************************************************************/
int CFileSerializer::GetSerializedBits(void)
{
	return m_serialized_bits;
}

