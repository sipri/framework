#pragma once

/************************************************************
 *
 * Serializer Interface
 *
 ************************************************************/
class ISerializer
{
public:
	// 데이터 설정(bool)
	virtual BOOL SetData(bool* pVal) = 0;

	// 데이터 설정(BYTE)
	virtual BOOL SetData(BYTE* pVal) = 0;

	// 데이터 설정(WORD)
	virtual BOOL SetData(WORD* pVal) = 0;

	// 데이터 설정(DWORD)
	virtual BOOL SetData(DWORD* pVal) = 0;

	// 데이터 설정(void*)
	virtual BOOL SetData(void* pVal, int size) = 0;


	// Serialize 된 바이트수 설정
	virtual void SetSerializedBytes(int size) = 0;

	// Serialize 된 비트수 설정
	virtual void SetSerializedBits(int size) = 0;


	// Serialize 된 바이트수 취득
	virtual int GetSerializedBytes(void) = 0;

	// Serialize 된 비트수 취득
	virtual int GetSerializedBits(void) = 0;
};

