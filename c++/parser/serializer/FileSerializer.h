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
	// ������
	CFileSerializer(CString strFileName);

	// �Ҹ���
	virtual ~CFileSerializer(void);


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
	// ���� ������
	CFile* m_pFile;

	// �ӽ� ����
	BYTE m_byTemp;


	// Serialize �� ����Ʈ��
	int m_serialized_bytes;

	// Serialize �� ��Ʈ��
	int m_serialized_bits;
};

