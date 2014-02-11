#pragma once

#include "ISerializer.h"

/************************************************************
 *
 * File Serializer
 *
 ************************************************************/
class CFileSerializer : public ISerializer
{
public:
	// 생산자
	CFileSerializer(CString strFileName);

	// 소멸자
	virtual ~CFileSerializer(void);


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
	// 파일 포인터
	CFile* m_pFile;

	// 임시 변수
	BYTE m_byTemp;


	// Serialize 된 바이트수
	int m_serialized_bytes;

	// Serialize 된 비트수
	int m_serialized_bits;
};

