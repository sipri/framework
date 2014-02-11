#pragma once

/************************************************************
 *
 * Serializer Interface
 *
 ************************************************************/
class ISerializer
{
public:
	// ������ ����(bool)
	virtual BOOL SetData(bool* pVal) = 0;

	// ������ ����(BYTE)
	virtual BOOL SetData(BYTE* pVal) = 0;

	// ������ ����(WORD)
	virtual BOOL SetData(WORD* pVal) = 0;

	// ������ ����(DWORD)
	virtual BOOL SetData(DWORD* pVal) = 0;

	// ������ ����(void*)
	virtual BOOL SetData(void* pVal, int size) = 0;


	// Serialize �� ����Ʈ�� ����
	virtual void SetSerializedBytes(int size) = 0;

	// Serialize �� ��Ʈ�� ����
	virtual void SetSerializedBits(int size) = 0;


	// Serialize �� ����Ʈ�� ���
	virtual int GetSerializedBytes(void) = 0;

	// Serialize �� ��Ʈ�� ���
	virtual int GetSerializedBits(void) = 0;
};

