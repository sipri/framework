#include "stdafx.h"
#include "Serializer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/************************************************************
 *	@brief		������
 *	@retval		����
 ************************************************************/
CSerializer::CSerializer(void)
{
	m_buff = NULL;
	m_size = 0;
	m_serialized_bytes = 0;
	m_serialized_bits = 0;
}

/************************************************************
 *	@brief		������
 *	@param[in]	buff			����
 *	@param[in]	size			���ۻ�����
 *	@retval		����
 ************************************************************/
CSerializer::CSerializer(void* buff, int size)
{
	m_buff = buff;
	m_size = size;
	m_serialized_bytes = 0;
	m_serialized_bits = 0;
}

/************************************************************
 *	@brief		�Ҹ���
 *	@retval		����
 ************************************************************/
CSerializer::~CSerializer(void)
{
}

/************************************************************
 *	@brief		������ ����(bool)
 *	@param[in]	pVal			���� ������
 *	@retval		TRUE			����
 *	@retval		FALSE			����
 ************************************************************/
BOOL CSerializer::SetData(bool* pVal)
{
	// NULL üũ
	if ((NULL == m_buff) || (NULL == pVal)) {
		return FALSE;
	}

	// ������ üũ
	if (m_size < (m_serialized_bytes + (int)sizeof(BYTE))) {
		return FALSE;
	}

	// ������ ����
	*((BYTE*)m_buff + m_serialized_bytes) |= (*(BYTE*)pVal << m_serialized_bits);

	// Serialized Bits ����
	m_serialized_bits++;

	// Serialized Bits �� Byte ������ �Ǿ��� ���
	if (!(m_serialized_bits % 8)) {

		// Serialized Bits �ʱ�ȭ
		m_serialized_bits = 0;

		// Serialized Bytes ����
		m_serialized_bytes += sizeof(BYTE);
	}

	return TRUE;
}

/************************************************************
 *	@brief		������ ����(BYTE)
 *	@param[in]	pVal			���� ������
 *	@retval		TRUE			����
 *	@retval		FALSE			����
 ************************************************************/
BOOL CSerializer::SetData(BYTE* pVal)
{
	// NULL üũ
	if ((NULL == m_buff) || (NULL == pVal)) {
		return FALSE;
	}

	// ������ üũ
	if (m_size < (m_serialized_bytes + (int)sizeof(BYTE))) {
		return FALSE;
	}

	// ������ ����
	*((BYTE*)m_buff + m_serialized_bytes) = *pVal;

	// Serialized Bytes ����
	m_serialized_bytes += sizeof(BYTE);

	return TRUE;
}

/************************************************************
 *	@brief		������ ����(WORD)
 *	@param[in]	pVal			���� ������
 *	@retval		TRUE			����
 *	@retval		FALSE			����
 ************************************************************/
BOOL CSerializer::SetData(WORD* pVal)
{
	// NULL üũ
	if ((NULL == m_buff) || (NULL == pVal)) {
		return FALSE;
	}

	// ������ üũ
	if (m_size < (m_serialized_bytes + (int)sizeof(WORD))) {
		return FALSE;
	}

	// ������ ����
	*((WORD*)((BYTE*)m_buff + m_serialized_bytes)) = *pVal;

	// Serialized Bytes ����
	m_serialized_bytes += sizeof(WORD);

	return TRUE;
}

/************************************************************
 *	@brief		������ ����(DWORD)
 *	@param[in]	pVal			���� ������
 *	@retval		TRUE			����
 *	@retval		FALSE			����
 ************************************************************/
BOOL CSerializer::SetData(DWORD* pVal)
{
	// NULL üũ
	if ((NULL == m_buff) || (NULL == pVal)) {
		return FALSE;
	}

	// ������ üũ
	if (m_size < (m_serialized_bytes + (int)sizeof(DWORD))) {
		return FALSE;
	}

	// ������ ����
	*((DWORD*)((BYTE*)m_buff + m_serialized_bytes)) = *pVal;

	// Serialized Bytes ����
	m_serialized_bytes += sizeof(DWORD);

	return TRUE;
}

/************************************************************
 *	@brief		������ ����(void*)
 *	@param[in]	pVal			���� ������
 *	@param[in]	size			���� ������ ������
 *	@retval		TRUE			����
 *	@retval		FALSE			����
 ************************************************************/
BOOL CSerializer::SetData(void* pVal, int size)
{
	// NULL üũ
	if ((NULL == m_buff) || (NULL == pVal)) {
		return FALSE;
	}

	// ������ üũ
	if (m_size < (m_serialized_bytes + size)) {
		return FALSE;
	}

	// ������ ����
	memcpy_s((BYTE*)m_buff + m_serialized_bytes, m_size - m_serialized_bytes, pVal, size);

	// Serialized Bytes ����
	m_serialized_bytes += size;

	return TRUE;
}

/************************************************************
 *	@brief		Serialize �� ����Ʈ�� ����
 *	@retval		int				Serialize �� ����Ʈ��
 ************************************************************/
void CSerializer::SetSerializedBytes(int size)
{
	m_serialized_bytes = size;
}

/************************************************************
 *	@brief		Serialize �� ��Ʈ�� ����
 *	@retval		int				Serialize �� ��Ʈ��
 ************************************************************/
void CSerializer::SetSerializedBits(int size)
{
	m_serialized_bits = size;
}

/************************************************************
 *	@brief		Serialize �� ����Ʈ�� ���
 *	@retval		int				Serialize �� ����Ʈ��
 ************************************************************/
int CSerializer::GetSerializedBytes(void)
{
	return m_serialized_bytes;
}

/************************************************************
 *	@brief		Serialize �� ��Ʈ�� ���
 *	@retval		int				Serialize �� ��Ʈ��
 ************************************************************/
int CSerializer::GetSerializedBits(void)
{
	return m_serialized_bits;
}
