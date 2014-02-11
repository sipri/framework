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
	// ������
	CFileDeSerializer(CString strFileName);

	// �Ҹ���
	virtual ~CFileDeSerializer(void);


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
	// ���� ������
	CFile* m_pFile;

	// �ӽ� ����
	BYTE m_byTemp;


	// De-Serialize �� ����Ʈ��
	int m_deserialized_bytes;

	// De-Serialize �� ��Ʈ��
	int m_deserialized_bits;
};

