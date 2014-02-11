#pragma once

/************************************************************
 *
 * DeSerializer Interface
 *
 ************************************************************/
class IDeSerializer
{
public:
	// ������ ���(bool)
	virtual BOOL GetData(bool* pVal) = 0;

	// ������ ���(BYTE)
	virtual BOOL GetData(BYTE* pVal) = 0;

	// ������ ���(WORD)
	virtual BOOL GetData(WORD* pVal) = 0;

	// ������ ���(DWORD)
	virtual BOOL GetData(DWORD* pVal) = 0;

	// ������ ���(void*)
	virtual BOOL GetData(void* pVal, int size) = 0;


	// De-Serialize �� ����Ʈ�� ����
	virtual void SetDeSerializedBytes(int size) = 0;

	// De-Serialize �� ��Ʈ�� ����
	virtual void SetDeSerializedBits(int size) = 0;


	// De-Serialize �� ����Ʈ�� ���
	virtual int GetDeSerializedBytes(void) = 0;

	// De-Serialize �� ��Ʈ�� ���
	virtual int GetDeSerializedBits(void) = 0;
};

