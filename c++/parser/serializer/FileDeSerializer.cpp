#include "stdafx.h"

#include "FileDeSerializer.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/************************************************************
 *	@brief		생성자
 *	@param[in]	strFileName		파일 이름
 *	@retval		없음
 ************************************************************/
CFileDeSerializer::CFileDeSerializer(CString strFileName)
{
	// 파일 열기
	m_pFile = new CFile();

	m_pFile->Open(strFileName, CFile::modeRead);


	m_byTemp = 0;

	m_deserialized_bytes = 0;

	m_deserialized_bits = 0;
}


/************************************************************
 *	@brief		소멸자
 *	@retval		없음
 ************************************************************/
CFileDeSerializer::~CFileDeSerializer(void)
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
 *	@brief		데이타취득(bool)
 *	@param[out]	pVal			취득데이타
 *	@retval		TRUE			성공
 *	@retval		FALSE			실패
 ************************************************************/
BOOL CFileDeSerializer::GetData(bool* pVal)
{
	// 파일 핸들 없음
	if (CFile::hFileNull == m_pFile->m_hFile)
	{
		return FALSE;
	}


	// De-Serialized Bits 가 0일 경우
	if (m_deserialized_bits)
	{
		// 데이터 취득
		UINT nBytesRead = m_pFile->Read(&m_byTemp, sizeof(BYTE));

		if (sizeof(BYTE) == nBytesRead)
		{
			return FALSE;
		}
	}


	// 데이터 설정
	*pVal = (0x01 & (m_byTemp >> m_deserialized_bits)) ? 1 : 0;

	// De-Serialized Bits 설정
	m_deserialized_bits++;


	// De-Serialized Bits 가 Byte 단위로 되었을 경우
	if (!(m_deserialized_bits % 8))
	{
		// De-Serialized Bits 초기화
		m_deserialized_bits = 0;

		// De-Serialized Bytes 설정
		m_deserialized_bytes += sizeof(BYTE);
	}


	return TRUE;
}


/************************************************************
 *	@brief		데이타취득(BYTE)
 *	@param[out]	pVal			취득데이타
 *	@retval		TRUE			성공
 *	@retval		FALSE			실패
 ************************************************************/
BOOL CFileDeSerializer::GetData(BYTE* pVal)
{
	// 파일 핸들 없음
	if (CFile::hFileNull == m_pFile->m_hFile)
	{
		return FALSE;
	}


	UINT nBytesRead = m_pFile->Read(pVal, sizeof(BYTE));

	m_deserialized_bits++;

	if (!(m_deserialized_bits % 8))
	{
		m_deserialized_bytes += sizeof(BYTE);
	}


	return (sizeof(BYTE) == nBytesRead);
}


/************************************************************
 *	@brief		데이타취득(WORD)
 *	@param[out]	pVal			취득데이타
 *	@retval		TRUE			성공
 *	@retval		FALSE			실패
 ************************************************************/
BOOL CFileDeSerializer::GetData(WORD* pVal)
{
	// 파일 핸들 없음
	if (CFile::hFileNull == m_pFile->m_hFile)
	{
		return FALSE;
	}


	UINT nBytesRead = m_pFile->Read(pVal, sizeof(WORD));

	m_deserialized_bytes += sizeof(WORD);


	return (sizeof(WORD) == nBytesRead);
}


/************************************************************
 *	@brief		데이타취득(DWORD)
 *	@param[out]	pVal			취득데이타
 *	@retval		TRUE			성공
 *	@retval		FALSE			실패
 ************************************************************/
BOOL CFileDeSerializer::GetData(DWORD* pVal)
{
	// 파일 핸들 없음
	if (CFile::hFileNull == m_pFile->m_hFile)
	{
		return FALSE;
	}


	UINT nBytesRead = m_pFile->Read(pVal, sizeof(DWORD));

	m_deserialized_bytes += sizeof(DWORD);


	return (sizeof(DWORD) == nBytesRead);
}


/************************************************************
 *	@brief		데이타취득(void*)
 *	@param[out]	pVal			취득데이타
 *	@param[in]	size			취득데이타사이즈
 *	@retval		TRUE			성공
 *	@retval		FALSE			실패
 ************************************************************/
BOOL CFileDeSerializer::GetData(void* pVal, int size)
{
	// 파일 핸들 없음
	if (CFile::hFileNull == m_pFile->m_hFile)
	{
		return FALSE;
	}


	UINT nBytesRead = m_pFile->Read(pVal, size);

	m_deserialized_bytes += size;


	return (size == nBytesRead);
}


/************************************************************
 *	@brief		De-Serialize 된 바이트수 설정
 *	@retval		int				De-Serialize 된 바이트수
 ************************************************************/
void CFileDeSerializer::SetDeSerializedBytes(int size)
{
	m_deserialized_bytes = size;
}


/************************************************************
 *	@brief		De-Serialize 된 비트수 설정
 *	@retval		int				De-Serialize 된 비트수
 ************************************************************/
void CFileDeSerializer::SetDeSerializedBits(int size)
{
	m_deserialized_bits = size;
}


/************************************************************
 *	@brief		De-Serialize 된 바이트수 취득
 *	@retval		int				De-Serialize 된 바이트수
 ************************************************************/
int CFileDeSerializer::GetDeSerializedBytes(void)
{
	return m_deserialized_bytes;
}


/************************************************************
 *	@brief		De-Serialize 된 비트수 취득
 *	@retval		int				De-Serialize 된 비트수
 ************************************************************/
int CFileDeSerializer::GetDeSerializedBits(void)
{
	return m_deserialized_bits;
}

