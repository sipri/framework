#pragma once

#include "ISerializer.h"

/************************************************************
 *
 * Serializer
 *
 ************************************************************/
class CSerializer : public ISerializer
{
public:
	// 생산자
	CSerializer(void);

	// 생산자
	CSerializer(void* buff, int size);

	// 소멸자
	virtual ~CSerializer(void);


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
	// 버퍼
	void* m_buff;

	// 버퍼사이즈
	int m_size;


	// Serialize 된 바이트수
	int m_serialized_bytes;

	// Serialize 된 비트수
	int m_serialized_bits;
};

