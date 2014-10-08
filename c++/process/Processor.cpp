#include "stdafx.h"

#include "Processor.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/************************************************************
 *	@brief		������
 *	@retval		����
 ************************************************************/
CProcessor::CProcessor (void)
{
	m_strCmd			= _T("");

	m_strParam			= _T("");

	m_pNotify			= NULL;

	m_nMsgID			= 0;

	m_bIsRunning		= FALSE;


	// Thread Handle
	m_hThread			= NULL;
}


/************************************************************
 *	@brief		������
 *	@param[in]	strCmd		Command
 *	@param[in]	pNotify			���� ������
 *	@param[in]	nMsgID			���� �޼���
 *	@retval		����
 ************************************************************/
CProcessor::CProcessor (CString strCmd, CString strParam /*= _T("")*/, CWnd* pNotify /*= NULL*/, UINT nMsgID /*= 0*/)
{
	m_strCmd			= strCmd;

	m_strParam			= strParam;

	m_pNotify			= pNotify;

	m_nMsgID			= nMsgID;

	m_bIsRunning		= FALSE;
}


/************************************************************
 *	@brief		�Ҹ���
 *	@retval		����
 ************************************************************/
CProcessor::~CProcessor (void)
{
}


/************************************************************
 *	@brief		��� ����
 *	@retval		����
 ************************************************************/
void CProcessor::SetCmd (CString strCmd)
{
	m_strCmd		= strCmd;
}


/************************************************************
 *	@brief		�Ķ���� ����
 *	@retval		����
 ************************************************************/
void CProcessor::SetParam (CString strParam)
{
	m_strParam		= strParam;
}


/************************************************************
 *	@brief		����޼��� ������ ������ ����
 *	@retval		����
 ************************************************************/
void CProcessor::SetNotifyWnd (CWnd* pNotify)
{
	m_pNotify		= pNotify;
}


/************************************************************
 *	@brief		����޼��� ID ����
 *	@retval		����
 ************************************************************/
void CProcessor::SetNotifyMsg (UINT nMsgID)
{
	m_nMsgID		= nMsgID;
}


/************************************************************
 *	@brief		Process ���� ����
 *	@retval		TRUE			����
 *	@retval		FALSE			����
 ************************************************************/
BOOL CProcessor::Run (void)
{
	// Processor Information ����
	PROCESSOR_INFO			procInfo;
	
	procInfo.m_strCmd		= m_strCmd;

	procInfo.m_strParam		= m_strParam;

	procInfo.m_pNotify		= m_pNotify;

	procInfo.m_nMsgID		= m_nMsgID;


	// Processor ����
	return RunProcess (&procInfo);
}


/************************************************************
 *	@brief		Process �񵿱� ����
 *	@retval		TRUE			����
 *	@retval		FALSE			����
 ************************************************************/
BOOL CProcessor::RunAsync (void)
{
	// Processor Information ����
	PROCESSOR_INFO			procInfo;
	
	procInfo.m_strCmd		= m_strCmd;

	procInfo.m_strParam		= m_strParam;

	procInfo.m_pNotify		= m_pNotify;

	procInfo.m_nMsgID		= m_nMsgID;


	// Processor Information �߰�
	m_Mutex.Lock();

	m_ProcessorInfos.push_back (procInfo);

	m_Mutex.Unlock();


	// Thread ����
	m_Mutex.Lock();

	if (NULL == m_hThread)
	{
		m_hThread = ::CreateThread(NULL, 0,(LPTHREAD_START_ROUTINE)RunProcess, (LPVOID)this, CREATE_SUSPENDED, NULL);

		if (NULL == m_hThread)
		{
			// ���μ��� ���� �޼��� �߻�
			if (NULL != m_pNotify)
			{
				m_pNotify->PostMessage(m_nMsgID);
			}

			m_Mutex.Unlock();

			return FALSE;
		}

		ResumeThread (m_hThread);
	}

	m_Mutex.Unlock();


	// Process ����
	m_RunEvent.SetEvent ();


	return TRUE;
}


/************************************************************
 *	@brief		Process ���� �� üũ
 *	@retval		TRUE			������
 *	@retval		FALSE			�������
 ************************************************************/
BOOL CProcessor::IsRunning (void)
{
	return m_bIsRunning;
}


/************************************************************
 *	@brief		Stop Process Thread
 *	@retval		TRUE			����
 *	@retval		FALSE			����
 ************************************************************/
BOOL CProcessor::StopThread (void)
{
	m_StopEvent.SetEvent ();

	return TRUE;
}


/************************************************************
 *	@brief		Process ����
 *	@param[in]	pProcess		CProcessorŬ����
 *	@retval		0				����
 ************************************************************/
DWORD WINAPI CProcessor::RunProcess (LPVOID pProcess)
{
	((CProcessor*)pProcess)->RunProcess();

	return 0;
}


/************************************************************
 *	@brief		Process ����
 *	@retval		TRUE			����
 *	@retval		FALSE			����
 ************************************************************/
BOOL CProcessor::RunProcess (void)
{
	BOOL bResult	= TRUE;

	// �̺�Ʈ ����Ʈ
	CSyncObject* EventList[2];

	EventList[0]	= &m_RunEvent;

	EventList[1]	= &m_StopEvent;

	// �̺�Ʈ ��� ����Ʈ �ۼ�
	CMultiLock multiLock (&EventList[0], _countof (EventList));

	for ( ; ; )
	{
		// �̺�Ʈ ���
		DWORD dwResult = multiLock.Lock(INFINITE, FALSE);

		// Run Event
		if (WAIT_OBJECT_0 == dwResult)
		{
			for ( ; ; )
			{
				// Processor Information ���
				PROCESSOR_INFO*		pProcInfo		= NULL;

				m_Mutex.Lock();

				if (m_ProcessorInfos.size())
				{
					pProcInfo	= &m_ProcessorInfos.front();
				}

				m_Mutex.Unlock();


				// Processor Information ����
				if (NULL == pProcInfo)
				{
					break;
				}


				// Process ����
				if (FALSE == RunProcess (pProcInfo))
				{
					bResult		= FALSE;
				}


				// Processor Information ����
				m_Mutex.Lock();

				m_ProcessorInfos.pop_front();

				m_Mutex.Unlock();
			}
		}

		// Stop Event
		else
		{
			break;
		}
	}


	return bResult;
}


/************************************************************
 *	@brief		Process ����
 *	@retval		TRUE			����
 *	@retval		FALSE			����
 ************************************************************/
BOOL CProcessor::RunProcess (PROCESSOR_INFO* pProcInfo)
{
	CString		strCmd		= pProcInfo->m_strCmd;

	CString		strParam	= pProcInfo->m_strParam;

	CWnd*		pNotify		= pProcInfo->m_pNotify;

	UINT		nMsgID		= pProcInfo->m_nMsgID;


	// FILE üũ
	CFileFind Find;

	if (FALSE == Find.FindFile (strCmd))
	{
		// ���μ��� ���� �޼��� �߻�
		if (NULL != pNotify)
		{
			pNotify->PostMessage (nMsgID);
		}

		return FALSE;
	}

	// FILE üũ ����
	Find.Close();


	// ���丮�� ���ϸ��� ����
	int idx = strCmd.ReverseFind(TEXT('\\'));

	CString strDir		= strCmd.Mid (0, idx);

	CString strFile		= strCmd.Mid (idx + 1);


	// ���μ��� ���� ���� ����
	SHELLEXECUTEINFO* pExecInfo = new SHELLEXECUTEINFO;

	ZeroMemory(pExecInfo, sizeof(SHELLEXECUTEINFO));

	pExecInfo->cbSize			= sizeof(SHELLEXECUTEINFO);

	pExecInfo->fMask			= SEE_MASK_NOCLOSEPROCESS;

	pExecInfo->hwnd				= NULL;

	pExecInfo->lpVerb			= NULL;

	pExecInfo->lpFile			= strFile;

	pExecInfo->lpParameters		= strParam;

	pExecInfo->lpDirectory		= strDir;

	pExecInfo->nShow			= SW_SHOW;

	pExecInfo->hInstApp			= NULL;


	// ���μ��� ���� �� ����
	m_bIsRunning = TRUE;


	// ���μ��� ����
	if (FALSE == ShellExecuteEx(pExecInfo))
	{
		// ���μ��� �ڵ� �ݱ�
		CloseHandle(pExecInfo->hProcess);

		// ���μ��� ���� ���� ����
		delete pExecInfo;


		// ���μ��� ���� �޼��� �߻�
		if (NULL != pNotify)
		{
			pNotify->PostMessage (nMsgID);
		}


		return FALSE;
	}


	// ���μ��� ���� ���
	WaitForSingleObject(pExecInfo->hProcess, INFINITE);

	// ���μ��� �ڵ� �ݱ�
	CloseHandle(pExecInfo->hProcess);

	// ���μ��� ���� ���� ����
	delete pExecInfo;


	// ���μ��� ���� �޼��� �߻�
	if (NULL != pNotify)
	{
		pNotify->PostMessage (nMsgID);
	}


	// ���μ��� ���� �� ����
	m_bIsRunning = FALSE;


	return TRUE;
}

