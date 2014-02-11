#include "stdafx.h"

#include "CountingSerializer.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/************************************************************
 *	@brief		생성자
 *	@retval		없음
 ************************************************************/
CCountingSerializer::CCountingSerializer(void)
{
	m_serialized_bytes	= 0;

	m_serialized_bits	= 0;
}


/************************************************************
 *	@brief		소멸자
 *	@retval		없음
 ************************************************************/
CCountingSerializer::~CCountingSerializer(void)
{
}


/************************************************************
 *	@brief		데이타설정(bool)
 *	@param[in]	pVal			설정데이타
 *	@retval		TRUE			성공
 *	@retval		FALSE			실패
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
 *	@brief		데이타설정(BYTE)
 *	@param[in]	pVal			설정데이타
 *	@retval		TRUE			성공
 *	@retval		FALSE			실패
 ************************************************************/
BOOL CCountingSerializer::SetData(BYTE* pVal)
{
	m_serialized_bytes += sizeof(BYTE);

	return TRUE;
}


/************************************************************
 *	@brief		데이타설정(WORD)
 *	@param[in]	pVal			설정데이타
 *	@retval		TRUE			성공
 *	@retval		FALSE			실패
 ************************************************************/
BOOL CCountingSerializer::SetData(WORD* pVal)
{
	m_serialized_bytes += sizeof(WORD);

	return TRUE;
}


/************************************************************
 *	@brief		데이타설정(DWORD)
 *	@param[in]	pVal			설정데이타
 *	@retval		TRUE			성공
 *	@retval		FALSE			실패
 ************************************************************/
BOOL CCountingSerializer::SetData(DWORD* pVal)
{
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
BOOL CCountingSerializer::SetData(void* pVal, int size)
{
	m_serialized_bytes += size;

	return TRUE;
}


/************************************************************
 *	@brief		Serialize 된 바이트수 설정
 *	@retval		int				Serialize 된 바이트수
 ************************************************************/
void CCountingSerializer::SetSerializedBytes(int size)
{
	m_serialized_bytes = size;
}


/************************************************************
 *	@brief		Serialize 된 비트수 설정
 *	@retval		int				Serialize 된 비트수
 ************************************************************/
void CCountingSerializer::SetSerializedBits(int size)
{
	m_serialized_bits = size;
}


/************************************************************
 *	@brief		Serialize 된 바이트수 취득
 *	@retval		int				Serialize 된 바이트수
 ************************************************************/
int CCountingSerializer::GetSerializedBytes(void)
{
	return m_serialized_bytes;
}


/************************************************************
 *	@brief		Serialize 된 비트수 취득
 *	@retval		int				Serialize 된 비트수
 ************************************************************/
int CCountingSerializer::GetSerializedBits(void)
{
	return m_serialized_bits;
}

