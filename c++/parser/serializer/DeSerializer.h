#pragma once

#include "IDeSerializer.h"

/************************************************************
 *
 * DeSerializer
 *
 ************************************************************/
class CDeSerializer : public IDeSerializer
{
public:
	// 생산자
	CDeSerializer(void);

	// 생산자
	CDeSerializer(void* buff, int size);

	// 소멸자
	virtual ~CDeSerializer(void);


	// 데이터 취득(bool)
	BOOL GetData(bool* pVal);

	// 데이터 취득(BYTE)
	BOOL GetData(BYTE* pVal);

	// 데이터 취득(WORD)
	BOOL GetData(WORD* pVal);

	// 데이터 취득(DWORD)
	BOOL GetData(DWORD* pVal);

	// 데이터 취득(void*)
	BOOL GetData(void* pVal, int size);


	// De-Serialize 된 바이트수 설정
	void SetDeSerializedBytes(int size);

	// De-Serialize 된 비트수 설정
	void SetDeSerializedBits(int size);


	// De-Serialize 된 바이트수 취득
	int GetDeSerializedBytes(void);

	// De-Serialize 된 비트수 취득
	int GetDeSerializedBits(void);


protected:
	// 버퍼
	void* m_buff;

	// 버퍼사이즈
	int m_size;


	// De-Serialize 된 바이트수
	int m_deserialized_bytes;

	// De-Serialize 된 비트수
	int m_deserialized_bits;
};

