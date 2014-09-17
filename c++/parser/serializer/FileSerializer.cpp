#include "stdafx.h"

#include "FileSerializer.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/************************************************************
 *	@brief		������
 *	@param[in]	strFileName		���� �̸�
 *	@retval		����
 ************************************************************/
CFileSerializer::CFileSerializer(CString strFileName)
{
	// ���� ����
	m_pFile = new CFile();

	m_pFile->Open(strFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);


	m_byTemp = 0;

	m_serialized_bytes = 0;

	m_serialized_bits = 0;
}


/************************************************************
 *	@brief		�Ҹ���
 *	@retval		����
 ************************************************************/
CFileSerializer::~CFileSerializer(void)
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
 *	@brief		����Ÿ����(bool)
 *	@param[in]	pVal			��������Ÿ
 *	@retval		TRUE			����
 *	@retval		FALSE			����
 ************************************************************/
BOOL CFileSerializer::SetData(bool* pVal)
{
	// ���� �ڵ� ����
	if (CFile::hFileNull == m_pFile->m_hFile)
	{
		return FALSE;
	}


	// ������ ����
	m_byTemp |= *(BYTE*)pVal << m_serialized_bits;

	// Serialized Bits ����
	m_serialized_bits++;


	// Serialized Bits �� Byte ������ �Ǿ��� ���
	if (!(m_serialized_bits % 8))
	{
		// ���� ����
		m_pFile->Write(&m_byTemp, sizeof(BYTE));

		m_byTemp = 0;

		// Serialized Bits �ʱ�ȭ
		m_serialized_bits = 0;

		// Serialized Bytes ����
		m_serialized_bytes += sizeof(BYTE);
	}


	return TRUE;
}


/************************************************************
 *	@brief		����Ÿ����(BYTE)
 *	@param[in]	pVal			��������Ÿ
 *	@retval		TRUE			����
 *	@retval		FALSE			����
 ************************************************************/
BOOL CFileSerializer::SetData(BYTE* pVal)
{
	// ���� �ڵ� ����
	if (CFile::hFileNull == m_pFile->m_hFile)
	{
		return FALSE;
	}


	m_pFile->Write(pVal, sizeof(BYTE));

	m_serialized_bytes += sizeof(BYTE);


	return TRUE;
}


/************************************************************
 *	@brief		����Ÿ����(WORD)
 *	@param[in]	pVal			��������Ÿ
 *	@retval		TRUE			����
 *	@retval		FALSE			����
 ************************************************************/
BOOL CFileSerializer::SetData(WORD* pVal)
{
	// ���� �ڵ� ����
	if (CFile::hFileNull == m_pFile->m_hFile)
	{
		return FALSE;
	}


	m_pFile->Write(pVal, sizeof(WORD));

	m_serialized_bytes += sizeof(WORD);


	return TRUE;
}


/************************************************************
 *	@brief		����Ÿ����(DWORD)
 *	@param[in]	pVal			��������Ÿ
 *	@retval		TRUE			����
 *	@retval		FALSE			����
 ************************************************************/
BOOL CFileSerializer::SetData(DWORD* pVal)
{
	// ���� �ڵ� ����
	if (CFile::hFileNull == m_pFile->m_hFile)
	{
		return FALSE;
	}


	m_pFile->Write(pVal, sizeof(DWORD));

	m_serialized_bytes += sizeof(DWORD);


	return TRUE;
}


/************************************************************
 *	@brief		����Ÿ����(void*)
 *	@param[in]	pVal			��������Ÿ
 *	@param[in]	size			��������Ÿ������
 *	@retval		TRUE			����
 *	@retval		FALSE			����
 ************************************************************/
BOOL CFileSerializer::SetData(void* pVal, int size)
{
	// ���� �ڵ� ����
	if (CFile::hFileNull == m_pFile->m_hFile)
	{
		return FALSE;
	}


	m_pFile->Write(pVal, size);

	m_serialized_bytes += size;


	return TRUE;
}


/************************************************************
 *	@brief		Serialize �� ����Ʈ�� ����
 *	@retval		int				Serialize �� ����Ʈ��
 ************************************************************/
void CFileSerializer::SetSerializedBytes(int size)
{
	m_serialized_bytes = size;
}


/************************************************************
 *	@brief		Serialize �� ��Ʈ�� ����
 *	@retval		int				Serialize �� ��Ʈ��
 ************************************************************/
void CFileSerializer::SetSerializedBits(int size)
{
	m_serialized_bits = size;
}


/************************************************************
 *	@brief		Serialize �� ����Ʈ�� ���
 *	@retval		int				Serialize �� ����Ʈ��
 ************************************************************/
int CFileSerializer::GetSerializedBytes(void)
{
	return m_serialized_bytes;
}


/************************************************************
 *	@brief		Serialize �� ��Ʈ�� ���
 *	@retval		int				Serialize �� ��Ʈ��
 ************************************************************/
int CFileSerializer::GetSerializedBits(void)
{
	return m_serialized_bits;
}

