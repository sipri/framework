#include "stdafx.h"
#include "Serializer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/************************************************************
 *	@brief		생성자
 *	@retval		없음
 ************************************************************/
CSerializer::CSerializer(void)
{
	m_buff = NULL;
	m_size = 0;
	m_serialized_bytes = 0;
	m_serialized_bits = 0;
}

/************************************************************
 *	@brief		생성자
 *	@param[in]	buff			버퍼
 *	@param[in]	size			버퍼사이즈
 *	@retval		없음
 ************************************************************/
CSerializer::CSerializer(void* buff, int size)
{
	m_buff = buff;
	m_size = size;
	m_serialized_bytes = 0;
	m_serialized_bits = 0;
}

/************************************************************
 *	@brief		소멸자
 *	@retval		없음
 ************************************************************/
CSerializer::~CSerializer(void)
{
}

/************************************************************
 *	@brief		데이터 설정(bool)
 *	@param[in]	pVal			설정 데이터
 *	@retval		TRUE			성공
 *	@retval		FALSE			실패
 ************************************************************/
BOOL CSerializer::SetData(bool* pVal)
{
	// NULL 체크
	if ((NULL == m_buff) || (NULL == pVal)) {
		return FALSE;
	}

	// 사이즈 체크
	if (m_size < (m_serialized_bytes + (int)sizeof(BYTE))) {
		return FALSE;
	}

	// 데이터 설정
	*((BYTE*)m_buff + m_serialized_bytes) |= (*(BYTE*)pVal << m_serialized_bits);

	// Serialized Bits 설정
	m_serialized_bits++;

	// Serialized Bits 가 Byte 단위로 되었을 경우
	if (!(m_serialized_bits % 8)) {

		// Serialized Bits 초기화
		m_serialized_bits = 0;

		// Serialized Bytes 설정
		m_serialized_bytes += sizeof(BYTE);
	}

	return TRUE;
}

/************************************************************
 *	@brief		데이터 설정(BYTE)
 *	@param[in]	pVal			설정 데이터
 *	@retval		TRUE			성공
 *	@retval		FALSE			실패
 ************************************************************/
BOOL CSerializer::SetData(BYTE* pVal)
{
	// NULL 체크
	if ((NULL == m_buff) || (NULL == pVal)) {
		return FALSE;
	}

	// 사이즈 체크
	if (m_size < (m_serialized_bytes + (int)sizeof(BYTE))) {
		return FALSE;
	}

	// 데이터 설정
	*((BYTE*)m_buff + m_serialized_bytes) = *pVal;

	// Serialized Bytes 설정
	m_serialized_bytes += sizeof(BYTE);

	return TRUE;
}

/************************************************************
 *	@brief		데이터 설정(WORD)
 *	@param[in]	pVal			설정 데이터
 *	@retval		TRUE			성공
 *	@retval		FALSE			실패
 ************************************************************/
BOOL CSerializer::SetData(WORD* pVal)
{
	// NULL 체크
	if ((NULL == m_buff) || (NULL == pVal)) {
		return FALSE;
	}

	// 사이즈 체크
	if (m_size < (m_serialized_bytes + (int)sizeof(WORD))) {
		return FALSE;
	}

	// 데이터 설정
	*((WORD*)((BYTE*)m_buff + m_serialized_bytes)) = *pVal;

	// Serialized Bytes 설정
	m_serialized_bytes += sizeof(WORD);

	return TRUE;
}

/************************************************************
 *	@brief		데이터 설정(DWORD)
 *	@param[in]	pVal			설정 데이터
 *	@retval		TRUE			성공
 *	@retval		FALSE			실패
 ************************************************************/
BOOL CSerializer::SetData(DWORD* pVal)
{
	// NULL 체크
	if ((NULL == m_buff) || (NULL == pVal)) {
		return FALSE;
	}

	// 사이즈 체크
	if (m_size < (m_serialized_bytes + (int)sizeof(DWORD))) {
		return FALSE;
	}

	// 데이터 설정
	*((DWORD*)((BYTE*)m_buff + m_serialized_bytes)) = *pVal;

	// Serialized Bytes 설정
	m_serialized_bytes += sizeof(DWORD);

	return TRUE;
}

/************************************************************
 *	@brief		데이터 설정(void*)
 *	@param[in]	pVal			설정 데이터
 *	@param[in]	size			설정 데이터 사이즈
 *	@retval		TRUE			성공
 *	@retval		FALSE			실패
 ************************************************************/
BOOL CSerializer::SetData(void* pVal, int size)
{
	// NULL 체크
	if ((NULL == m_buff) || (NULL == pVal)) {
		return FALSE;
	}

	// 사이즈 체크
	if (m_size < (m_serialized_bytes + size)) {
		return FALSE;
	}

	// 데이터 설정
	memcpy_s((BYTE*)m_buff + m_serialized_bytes, m_size - m_serialized_bytes, pVal, size);

	// Serialized Bytes 설정
	m_serialized_bytes += size;

	return TRUE;
}

/************************************************************
 *	@brief		Serialize 된 바이트수 설정
 *	@retval		int				Serialize 된 바이트수
 ************************************************************/
void CSerializer::SetSerializedBytes(int size)
{
	m_serialized_bytes = size;
}

/************************************************************
 *	@brief		Serialize 된 비트수 설정
 *	@retval		int				Serialize 된 비트수
 ************************************************************/
void CSerializer::SetSerializedBits(int size)
{
	m_serialized_bits = size;
}

/************************************************************
 *	@brief		Serialize 된 바이트수 취득
 *	@retval		int				Serialize 된 바이트수
 ************************************************************/
int CSerializer::GetSerializedBytes(void)
{
	return m_serialized_bytes;
}

/************************************************************
 *	@brief		Serialize 된 비트수 취득
 *	@retval		int				Serialize 된 비트수
 ************************************************************/
int CSerializer::GetSerializedBits(void)
{
	return m_serialized_bits;
}
