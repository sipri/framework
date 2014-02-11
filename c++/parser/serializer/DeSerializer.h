#pragma once

#include "IDeSerializer.h"

/************************************************************
 *
 * DeSerializer
 *
 ************************************************************/
class CDeSerializer : public IDeSerializer
{
public:
	// ������
	CDeSerializer(void);

	// ������
	CDeSerializer(void* buff, int size);

	// �Ҹ���
	virtual ~CDeSerializer(void);


	// ������ ���(bool)
	BOOL GetData(bool* pVal);

	// ������ ���(BYTE)
	BOOL GetData(BYTE* pVal);

	// ������ ���(WORD)
	BOOL GetData(WORD* pVal);

	// ������ ���(DWORD)
	BOOL GetData(DWORD* pVal);

	// ������ ���(void*)
	BOOL GetData(void* pVal, int size);


	// De-Serialize �� ����Ʈ�� ����
	void SetDeSerializedBytes(int size);

	// De-Serialize �� ��Ʈ�� ����
	void SetDeSerializedBits(int size);


	// De-Serialize �� ����Ʈ�� ���
	int GetDeSerializedBytes(void);

	// De-Serialize �� ��Ʈ�� ���
	int GetDeSerializedBits(void);


protected:
	// ����
	void* m_buff;

	// ���ۻ�����
	int m_size;


	// De-Serialize �� ����Ʈ��
	int m_deserialized_bytes;

	// De-Serialize �� ��Ʈ��
	int m_deserialized_bits;
};

