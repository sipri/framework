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
	// ������
	CCountingSerializer(void);

	// �Ҹ���
	virtual ~CCountingSerializer(void);


	// ������ ����(bool)
	BOOL SetData(bool* pVal);

	// ������ ����(BYTE)
	BOOL SetData(BYTE* pVal);

	// ������ ����(WORD)
	BOOL SetData(WORD* pVal);

	// ������ ����(DWORD)
	BOOL SetData(DWORD* pVal);

	// ������ ����(void*)
	BOOL SetData(void* pVal, int size);


	// Serialize �� ����Ʈ�� ����
	void SetSerializedBytes(int size);

	// Serialize �� ��Ʈ�� ����
	void SetSerializedBits(int size);


	// Serialize �� ����Ʈ�� ���
	int GetSerializedBytes(void);

	// Serialize �� ��Ʈ�� ���
	int GetSerializedBits(void);


protected:
	// Serialize �� ����Ʈ��
	int m_serialized_bytes;

	// Serialize �� ��Ʈ��
	int m_serialized_bits;
};

