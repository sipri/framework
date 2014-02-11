#pragma once

#include "IDeSerializer.h"

/************************************************************
 *
 * File DeSerializer
 *
 ************************************************************/
class CFileDeSerializer : public IDeSerializer
{
public:
	// 생산자
	CFileDeSerializer(CString strFileName);

	// 소멸자
	virtual ~CFileDeSerializer(void);


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
	// 파일 포인터
	CFile* m_pFile;

	// 임시 변수
	BYTE m_byTemp;


	// De-Serialize 된 바이트수
	int m_deserialized_bytes;

	// De-Serialize 된 비트수
	int m_deserialized_bits;
};

