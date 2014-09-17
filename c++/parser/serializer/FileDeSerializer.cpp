#include "stdafx.h"

#include "FileDeSerializer.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/************************************************************
 *	@brief		������
 *	@param[in]	strFileName		���� �̸�
 *	@retval		����
 ************************************************************/
CFileDeSerializer::CFileDeSerializer(CString strFileName)
{
	// ���� ����
	m_pFile = new CFile();

	m_pFile->Open(strFileName, CFile::modeRead);


	m_byTemp = 0;

	m_deserialized_bytes = 0;

	m_deserialized_bits = 0;
}


/************************************************************
 *	@brief		�Ҹ���
 *	@retval		����
 ************************************************************/
CFileDeSerializer::~CFileDeSerializer(void)
{
	if (m_pFile)
	{
		// ���� �ڵ� ����
		if (CFile::hFileNull != m_pFile->m_hFile)
		{
			// ���� �ݱ�
			m_pFile->Close();
		}

		delete m_pFile;

		m_pFile = NULL;
	}
}


/************************************************************
 *	@brief		����Ÿ���(bool)
 *	@param[out]	pVal			��浥��Ÿ
 *	@retval		TRUE			����
 *	@retval		FALSE			����
 ************************************************************/
BOOL CFileDeSerializer::GetData(bool* pVal)
{
	// ���� �ڵ� ����
	if (CFile::hFileNull == m_pFile->m_hFile)
	{
		return FALSE;
	}


	// De-Serialized Bits �� 0�� ���
	if (m_deserialized_bits)
	{
		// ������ ���
		UINT nBytesRead = m_pFile->Read(&m_byTemp, sizeof(BYTE));

		if (sizeof(BYTE) == nBytesRead)
		{
			return FALSE;
		}
	}


	// ������ ����
	*pVal = (0x01 & (m_byTemp >> m_deserialized_bits)) ? 1 : 0;

	// De-Serialized Bits ����
	m_deserialized_bits++;


	// De-Serialized Bits �� Byte ������ �Ǿ��� ���
	if (!(m_deserialized_bits % 8))
	{
		// De-Serialized Bits �ʱ�ȭ
		m_deserialized_bits = 0;

		// De-Serialized Bytes ����
		m_deserialized_bytes += sizeof(BYTE);
	}


	return TRUE;
}


/************************************************************
 *	@brief		����Ÿ���(BYTE)
 *	@param[out]	pVal			��浥��Ÿ
 *	@retval		TRUE			����
 *	@retval		FALSE			����
 ************************************************************/
BOOL CFileDeSerializer::GetData(BYTE* pVal)
{
	// ���� �ڵ� ����
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
 *	@brief		����Ÿ���(WORD)
 *	@param[out]	pVal			��浥��Ÿ
 *	@retval		TRUE			����
 *	@retval		FALSE			����
 ************************************************************/
BOOL CFileDeSerializer::GetData(WORD* pVal)
{
	// ���� �ڵ� ����
	if (CFile::hFileNull == m_pFile->m_hFile)
	{
		return FALSE;
	}


	UINT nBytesRead = m_pFile->Read(pVal, sizeof(WORD));

	m_deserialized_bytes += sizeof(WORD);


	return (sizeof(WORD) == nBytesRead);
}


/************************************************************
 *	@brief		����Ÿ���(DWORD)
 *	@param[out]	pVal			��浥��Ÿ
 *	@retval		TRUE			����
 *	@retval		FALSE			����
 ************************************************************/
BOOL CFileDeSerializer::GetData(DWORD* pVal)
{
	// ���� �ڵ� ����
	if (CFile::hFileNull == m_pFile->m_hFile)
	{
		return FALSE;
	}


	UINT nBytesRead = m_pFile->Read(pVal, sizeof(DWORD));

	m_deserialized_bytes += sizeof(DWORD);


	return (sizeof(DWORD) == nBytesRead);
}


/************************************************************
 *	@brief		����Ÿ���(void*)
 *	@param[out]	pVal			��浥��Ÿ
 *	@param[in]	size			��浥��Ÿ������
 *	@retval		TRUE			����
 *	@retval		FALSE			����
 ************************************************************/
BOOL CFileDeSerializer::GetData(void* pVal, int size)
{
	// ���� �ڵ� ����
	if (CFile::hFileNull == m_pFile->m_hFile)
	{
		return FALSE;
	}


	UINT nBytesRead = m_pFile->Read(pVal, size);

	m_deserialized_bytes += size;


	return (size == nBytesRead);
}


/************************************************************
 *	@brief		De-Serialize �� ����Ʈ�� ����
 *	@retval		int				De-Serialize �� ����Ʈ��
 ************************************************************/
void CFileDeSerializer::SetDeSerializedBytes(int size)
{
	m_deserialized_bytes = size;
}


/************************************************************
 *	@brief		De-Serialize �� ��Ʈ�� ����
 *	@retval		int				De-Serialize �� ��Ʈ��
 ************************************************************/
void CFileDeSerializer::SetDeSerializedBits(int size)
{
	m_deserialized_bits = size;
}


/************************************************************
 *	@brief		De-Serialize �� ����Ʈ�� ���
 *	@retval		int				De-Serialize �� ����Ʈ��
 ************************************************************/
int CFileDeSerializer::GetDeSerializedBytes(void)
{
	return m_deserialized_bytes;
}


/************************************************************
 *	@brief		De-Serialize �� ��Ʈ�� ���
 *	@retval		int				De-Serialize �� ��Ʈ��
 ************************************************************/
int CFileDeSerializer::GetDeSerializedBits(void)
{
	return m_deserialized_bits;
}

