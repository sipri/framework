#pragma once


#include <list>

#include <afxmt.h>


struct PROCESSOR_INFO
{
	CString		m_strCmd;

	CString		m_strParam;

	CWnd*		m_pNotify;

	UINT		m_nMsgID;
};


/************************************************************
 *
 * CProcessor
 *
 ************************************************************/
class AFX_EXT_CLASS CProcessor
{
public:
	// ������
	CProcessor (void);

	// ������
	CProcessor (CString strCmd, CString strParam = _T(""), CWnd* pNotify = NULL, UINT nMsgID = 0);

	// �Ҹ���
	virtual ~CProcessor (void);


	// ��� ����
	void SetCmd (CString strCmd);

	// �Ķ���� ����
	void SetParam (CString strParam);

	// ����޼��� ������ ������ ����
	void SetNotifyWnd (CWnd* pNotify);

	// ����޼��� ID ����
	void SetNotifyMsg (UINT nMsgID);


	// Process ���� ����
	BOOL Run (void);

	// Process �񵿱� ����
	BOOL RunAsync (void);

	// Process ���� �� üũ
	BOOL IsRunning (void);

	// Stop Process Thread
	BOOL StopThread (void);


protected:
	// Command
	CString		m_strCmd;

	// Parameter
	CString		m_strParam;

	// ���� �޼��� ���� ������
	CWnd*		m_pNotify;

	// ���� �޼��� ID
	UINT		m_nMsgID;

	// Processor ���� ��
	BOOL		m_bIsRunning;

	// Processor Infos
	std::list<PROCESSOR_INFO>	m_ProcessorInfos;

	// Thread Handle
	HANDLE		m_hThread;

	// Run Event
	CEvent		m_RunEvent;

	// Stop Event
	CEvent		m_StopEvent;

	// Mutex
	CMutex		m_Mutex;


	// Process ����
	static DWORD WINAPI RunProcess (LPVOID pProcess);

	// Process ����
	BOOL RunProcess (void);

	// Process ����
	BOOL RunProcess (PROCESSOR_INFO* pProcInfo);
};

