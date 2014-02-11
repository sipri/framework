#include "stdafx.h"

#include "CountingSerializer.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/************************************************************
 *	@brief		������
 *	@retval		����
 ************************************************************/
CCountingSerializer::CCountingSerializer(void)
{
	m_serialized_bytes	= 0;

	m_serialized_bits	= 0;
}


/************************************************************
 *	@brief		�Ҹ���
 *	@retval		����
 ************************************************************/
CCountingSerializer::~CCountingSerializer(void)
{
}


/************************************************************
 *	@brief		����Ÿ����(bool)
 *	@param[in]	pVal			��������Ÿ
 *	@retval		TRUE			����
 *	@retval		FALSE			����
 ************************************************************/
BOOL CCountingSerializer::SetData(bool* pVal)
{
	m_serialized_bits++;

	if (!(m_serialized_bits % 8))
	{
		m_serialized_bits = 0;

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
BOOL CCountingSerializer::SetData(BYTE* pVal)
{
	m_serialized_bytes += sizeof(BYTE);

	return TRUE;
}


/************************************************************
 *	@brief		����Ÿ����(WORD)
 *	@param[in]	pVal			��������Ÿ
 *	@retval		TRUE			����
 *	@retval		FALSE			����
 ************************************************************/
BOOL CCountingSerializer::SetData(WORD* pVal)
{
	m_serialized_bytes += sizeof(WORD);

	return TRUE;
}


/************************************************************
 *	@brief		����Ÿ����(DWORD)
 *	@param[in]	pVal			��������Ÿ
 *	@retval		TRUE			����
 *	@retval		FALSE			����
 ************************************************************/
BOOL CCountingSerializer::SetData(DWORD* pVal)
{
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
BOOL CCountingSerializer::SetData(void* pVal, int size)
{
	m_serialized_bytes += size;

	return TRUE;
}


/************************************************************
 *	@brief		Serialize �� ����Ʈ�� ����
 *	@retval		int				Serialize �� ����Ʈ��
 ************************************************************/
void CCountingSerializer::SetSerializedBytes(int size)
{
	m_serialized_bytes = size;
}


/************************************************************
 *	@brief		Serialize �� ��Ʈ�� ����
 *	@retval		int				Serialize �� ��Ʈ��
 ************************************************************/
void CCountingSerializer::SetSerializedBits(int size)
{
	m_serialized_bits = size;
}


/************************************************************
 *	@brief		Serialize �� ����Ʈ�� ���
 *	@retval		int				Serialize �� ����Ʈ��
 ************************************************************/
int CCountingSerializer::GetSerializedBytes(void)
{
	return m_serialized_bytes;
}


/************************************************************
 *	@brief		Serialize �� ��Ʈ�� ���
 *	@retval		int				Serialize �� ��Ʈ��
 ************************************************************/
int CCountingSerializer::GetSerializedBits(void)
{
	return m_serialized_bits;
}

