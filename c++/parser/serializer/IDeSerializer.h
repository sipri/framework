#pragma once

/************************************************************
 *
 * DeSerializer Interface
 *
 ************************************************************/
class IDeSerializer
{
public:
	// 데이터 취득(bool)
	virtual BOOL GetData(bool* pVal) = 0;

	// 데이터 취득(BYTE)
	virtual BOOL GetData(BYTE* pVal) = 0;

	// 데이터 취득(WORD)
	virtual BOOL GetData(WORD* pVal) = 0;

	// 데이터 취득(DWORD)
	virtual BOOL GetData(DWORD* pVal) = 0;

	// 데이터 취득(void*)
	virtual BOOL GetData(void* pVal, int size) = 0;


	// De-Serialize 된 바이트수 설정
	virtual void SetDeSerializedBytes(int size) = 0;

	// De-Serialize 된 비트수 설정
	virtual void SetDeSerializedBits(int size) = 0;


	// De-Serialize 된 바이트수 취득
	virtual int GetDeSerializedBytes(void) = 0;

	// De-Serialize 된 비트수 취득
	virtual int GetDeSerializedBits(void) = 0;
};

