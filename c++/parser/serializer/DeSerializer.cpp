#include "stdafx.h"

#include "DeSerializer.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/************************************************************
 *	@brief		생성자
 *	@retval		없음
 ************************************************************/
CDeSerializer::CDeSerializer(void)
{
	m_buff					= NULL;

	m_size					= 0;


	m_deserialized_bytes	= 0;

	m_deserialized_bits		= 0;
}


/************************************************************
 *	@brief		생성자
 *	@param[in]	buff			버퍼
 *	@param[in]	size			버퍼사이즈
 *	@retval		없음
 ************************************************************/
CDeSerializer::CDeSerializer(void* buff, int size)
{
	m_buff					= buff;

	m_size					= size;


	m_deserialized_bytes	= 0;

	m_deserialized_bits		= 0;
}


/************************************************************
 *	@brief		소멸자
 *	@retval		없음
 ************************************************************/
CDeSerializer::~CDeSerializer(void)
{
}


/************************************************************
 *	@brief		데이타취득(bool)
 *	@param[out]	pVal			취득데이타
 *	@retval		TRUE			성공
 *	@retval		FALSE			실패
 ************************************************************/
BOOL CDeSerializer::GetData(bool* pVal)
{
	// NULL체크
	if ((NULL == m_buff) || (NULL == pVal))
	{
		return FALSE;
	}


	// 사이즈체크
	if (m_size < (m_deserialized_bytes + (int)sizeof(BYTE)))
	{
		return FALSE;
	}


	// 데이타취득
	*pVal = (0x01 & (*((BYTE*)m_buff + m_deserialized_bytes) >> m_deserialized_bits)) ? 1 : 0;

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
BOOL CDeSerializer::GetData(BYTE* pVal)
{
	// NULL체크
	if ((NULL == m_buff) || (NULL == pVal))
	{
		return FALSE;
	}


	// 사이즈체크
	if (m_size < (m_deserialized_bytes + (int)sizeof(BYTE)))
	{
		return FALSE;
	}


	// 데이타취득
	*pVal = *((BYTE*)m_buff + m_deserialized_bytes);

	// De-Serialized Bytes 설정
	m_deserialized_bytes += sizeof(BYTE);


	return TRUE;
}


/************************************************************
 *	@brief		데이타취득(WORD)
 *	@param[out]	pVal			취득데이타
 *	@retval		TRUE			성공
 *	@retval		FALSE			실패
 ************************************************************/
BOOL CDeSerializer::GetData(WORD* pVal)
{
	// NULL체크
	if ((NULL == m_buff) || (NULL == pVal))
	{
		return FALSE;
	}


	// 사이즈체크
	if (m_size < (m_deserialized_bytes + (int)sizeof(WORD)))
	{
		return FALSE;
	}


	// 데이타취득
	*pVal = *((WORD*)((BYTE*)m_buff + m_deserialized_bytes));

	// De-Serialized Bytes 설정
	m_deserialized_bytes += sizeof(WORD);


	return TRUE;
}


/************************************************************
 *	@brief		데이타취득(DWORD)
 *	@param[out]	pVal			취득데이타
 *	@retval		TRUE			성공
 *	@retval		FALSE			실패
 ************************************************************/
BOOL CDeSerializer::GetData(DWORD* pVal)
{
	// NULL체크
	if ((NULL == m_buff) || (NULL == pVal))
	{
		return FALSE;
	}


	// 사이즈체크
	if (m_size < (m_deserialized_bytes + (int)sizeof(DWORD)))
	{
		return FALSE;
	}


	// 데이타취득
	*pVal = *((DWORD*)((BYTE*)m_buff + m_deserialized_bytes));

	// De-Serialized Bytes 설정
	m_deserialized_bytes += sizeof(DWORD);


	return TRUE;
}


/************************************************************
 *	@brief		데이타취득(void*)
 *	@param[out]	pVal			취득데이타
 *	@param[in]	size			취득데이타사이즈
 *	@retval		TRUE			성공
 *	@retval		FALSE			실패
 ************************************************************/
BOOL CDeSerializer::GetData(void* pVal, int size)
{
	// NULL체크
	if ((NULL == m_buff) || (NULL == pVal))
	{
		return FALSE;
	}


	// 사이즈체크
	if (m_size < (m_deserialized_bytes + size))
	{
		return FALSE;
	}


	// 데이타취득
	memcpy_s(pVal, size, (BYTE*)m_buff + m_deserialized_bytes, size);

	// De-Serialized Bytes 설정
	m_deserialized_bytes += size;


	return TRUE;
}


/************************************************************
 *	@brief		De-Serialize 된 바이트수 설정
 *	@retval		int				De-Serialize 된 바이트수
 ************************************************************/
void CDeSerializer::SetDeSerializedBytes(int size)
{
	m_deserialized_bytes = size;
}


/************************************************************
 *	@brief		De-Serialize 된 비트수 설정
 *	@retval		int				De-Serialize 된 비트수
 ************************************************************/
void CDeSerializer::SetDeSerializedBits(int size)
{
	m_deserialized_bits = size;
}


/************************************************************
 *	@brief		De-Serialize 된 바이트수 취득
 *	@retval		int				De-Serialize 된 바이트수
 ************************************************************/
int CDeSerializer::GetDeSerializedBytes(void)
{
	return m_deserialized_bytes;
}


/************************************************************
 *	@brief		De-Serialize 된 비트수 취득
 *	@retval		int				De-Serialize 된 비트수
 ************************************************************/
int CDeSerializer::GetDeSerializedBits(void)
{
	return m_deserialized_bits;
}

