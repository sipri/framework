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
	// ������
	CSerializer(void);

	// ������
	CSerializer(void* buff, int size);

	// �Ҹ���
	virtual ~CSerializer(void);


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
	// ����
	void* m_buff;

	// ���ۻ�����
	int m_size;


	// Serialize �� ����Ʈ��
	int m_serialized_bytes;

	// Serialize �� ��Ʈ��
	int m_serialized_bits;
};

