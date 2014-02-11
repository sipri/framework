#include "stdafx.h"

#include "DeSerializer.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/************************************************************
 *	@brief		������
 *	@retval		����
 ************************************************************/
CDeSerializer::CDeSerializer(void)
{
	m_buff					= NULL;

	m_size					= 0;


	m_deserialized_bytes	= 0;

	m_deserialized_bits		= 0;
}


/************************************************************
 *	@brief		������
 *	@param[in]	buff			����
 *	@param[in]	size			���ۻ�����
 *	@retval		����
 ************************************************************/
CDeSerializer::CDeSerializer(void* buff, int size)
{
	m_buff					= buff;

	m_size					= size;


	m_deserialized_bytes	= 0;

	m_deserialized_bits		= 0;
}


/************************************************************
 *	@brief		�Ҹ���
 *	@retval		����
 ************************************************************/
CDeSerializer::~CDeSerializer(void)
{
}


/************************************************************
 *	@brief		����Ÿ���(bool)
 *	@param[out]	pVal			��浥��Ÿ
 *	@retval		TRUE			����
 *	@retval		FALSE			����
 ************************************************************/
BOOL CDeSerializer::GetData(bool* pVal)
{
	// NULLüũ
	if ((NULL == m_buff) || (NULL == pVal))
	{
		return FALSE;
	}


	// ������üũ
	if (m_size < (m_deserialized_bytes + (int)sizeof(BYTE)))
	{
		return FALSE;
	}


	// ����Ÿ���
	*pVal = (0x01 & (*((BYTE*)m_buff + m_deserialized_bytes) >> m_deserialized_bits)) ? 1 : 0;

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
BOOL CDeSerializer::GetData(BYTE* pVal)
{
	// NULLüũ
	if ((NULL == m_buff) || (NULL == pVal))
	{
		return FALSE;
	}


	// ������üũ
	if (m_size < (m_deserialized_bytes + (int)sizeof(BYTE)))
	{
		return FALSE;
	}


	// ����Ÿ���
	*pVal = *((BYTE*)m_buff + m_deserialized_bytes);

	// De-Serialized Bytes ����
	m_deserialized_bytes += sizeof(BYTE);


	return TRUE;
}


/************************************************************
 *	@brief		����Ÿ���(WORD)
 *	@param[out]	pVal			��浥��Ÿ
 *	@retval		TRUE			����
 *	@retval		FALSE			����
 ************************************************************/
BOOL CDeSerializer::GetData(WORD* pVal)
{
	// NULLüũ
	if ((NULL == m_buff) || (NULL == pVal))
	{
		return FALSE;
	}


	// ������üũ
	if (m_size < (m_deserialized_bytes + (int)sizeof(WORD)))
	{
		return FALSE;
	}


	// ����Ÿ���
	*pVal = *((WORD*)((BYTE*)m_buff + m_deserialized_bytes));

	// De-Serialized Bytes ����
	m_deserialized_bytes += sizeof(WORD);


	return TRUE;
}


/************************************************************
 *	@brief		����Ÿ���(DWORD)
 *	@param[out]	pVal			��浥��Ÿ
 *	@retval		TRUE			����
 *	@retval		FALSE			����
 ************************************************************/
BOOL CDeSerializer::GetData(DWORD* pVal)
{
	// NULLüũ
	if ((NULL == m_buff) || (NULL == pVal))
	{
		return FALSE;
	}


	// ������üũ
	if (m_size < (m_deserialized_bytes + (int)sizeof(DWORD)))
	{
		return FALSE;
	}


	// ����Ÿ���
	*pVal = *((DWORD*)((BYTE*)m_buff + m_deserialized_bytes));

	// De-Serialized Bytes ����
	m_deserialized_bytes += sizeof(DWORD);


	return TRUE;
}


/************************************************************
 *	@brief		����Ÿ���(void*)
 *	@param[out]	pVal			��浥��Ÿ
 *	@param[in]	size			��浥��Ÿ������
 *	@retval		TRUE			����
 *	@retval		FALSE			����
 ************************************************************/
BOOL CDeSerializer::GetData(void* pVal, int size)
{
	// NULLüũ
	if ((NULL == m_buff) || (NULL == pVal))
	{
		return FALSE;
	}


	// ������üũ
	if (m_size < (m_deserialized_bytes + size))
	{
		return FALSE;
	}


	// ����Ÿ���
	memcpy_s(pVal, size, (BYTE*)m_buff + m_deserialized_bytes, size);

	// De-Serialized Bytes ����
	m_deserialized_bytes += size;


	return TRUE;
}


/************************************************************
 *	@brief		De-Serialize �� ����Ʈ�� ����
 *	@retval		int				De-Serialize �� ����Ʈ��
 ************************************************************/
void CDeSerializer::SetDeSerializedBytes(int size)
{
	m_deserialized_bytes = size;
}


/************************************************************
 *	@brief		De-Serialize �� ��Ʈ�� ����
 *	@retval		int				De-Serialize �� ��Ʈ��
 ************************************************************/
void CDeSerializer::SetDeSerializedBits(int size)
{
	m_deserialized_bits = size;
}


/************************************************************
 *	@brief		De-Serialize �� ����Ʈ�� ���
 *	@retval		int				De-Serialize �� ����Ʈ��
 ************************************************************/
int CDeSerializer::GetDeSerializedBytes(void)
{
	return m_deserialized_bytes;
}


/************************************************************
 *	@brief		De-Serialize �� ��Ʈ�� ���
 *	@retval		int				De-Serialize �� ��Ʈ��
 ************************************************************/
int CDeSerializer::GetDeSerializedBits(void)
{
	return m_deserialized_bits;
}

