#pragma once


#include "ISerializer.h"


/************************************************************
 *
 * Counting Serializer
 *
 ************************************************************/
class CCountingSerializer : public ISerializer
{
public:
	// 생산자
	CCountingSerializer(void);

	// 소멸자
	virtual ~CCountingSerializer(void);


	// 데이터 설정(bool)
	BOOL SetData(bool* pVal);

	// 데이터 설정(BYTE)
	BOOL SetData(BYTE* pVal);

	// 데이터 설정(WORD)
	BOOL SetData(WORD* pVal);

	// 데이터 설정(DWORD)
	BOOL SetData(DWORD* pVal);

	// 데이터 설정(void*)
	BOOL SetData(void* pVal, int size);


	// Serialize 된 바이트수 설정
	void SetSerializedBytes(int size);

	// Serialize 된 비트수 설정
	void SetSerializedBits(int size);


	// Serialize 된 바이트수 취득
	int GetSerializedBytes(void);

	// Serialize 된 비트수 취득
	int GetSerializedBits(void);


protected:
	// Serialize 된 바이트수
	int m_serialized_bytes;

	// Serialize 된 비트수
	int m_serialized_bits;
};

