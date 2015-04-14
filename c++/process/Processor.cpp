#include "stdafx.h"

#include <TlHelp32.h>

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
	StopThread();
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
	m_Mutex.Lock();


	// Processor Information �߰�
	PROCESSOR_INFO			procInfo;
	
	procInfo.m_strCmd		= m_strCmd;

	procInfo.m_strParam		= m_strParam;

	procInfo.m_pNotify		= m_pNotify;

	procInfo.m_nMsgID		= m_nMsgID;

	m_ProcessorInfos.push_back (procInfo);


	// Thread ����
	HANDLE hThread = ::CreateThread(NULL, 0,(LPTHREAD_START_ROUTINE)RunProcess, (LPVOID)this, CREATE_SUSPENDED, NULL);

	if (NULL == hThread)
	{
		// ���μ��� ���� �޼��� �߻�
		if (NULL != m_pNotify)
		{
			m_pNotify->PostMessage(m_nMsgID);
		}

		m_Mutex.Unlock();

		return FALSE;
	}

	ResumeThread (hThread);

	m_ThreadHandles.push_back(hThread);


	m_Mutex.Unlock();


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
	// Kill Process
	for (auto it = m_ProcessorIDs.begin(); it != m_ProcessorIDs.end(); it++)
	{
		KillProcess(*it);
	}

	m_ProcessorIDs.clear();


	// Close Thread Handle
	for (auto it = m_ThreadHandles.begin(); it != m_ThreadHandles.end(); it++)
	{
		WaitForSingleObject(*it, INFINITE);

		CloseHandle(*it);
	}

	m_ThreadHandles.clear();


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


	// Processor Information ���
	PROCESSOR_INFO	procInfo;

	BOOL			bProcInfo	= FALSE;


	m_Mutex.Lock();

	if (m_ProcessorInfos.size())
	{
		procInfo	= m_ProcessorInfos.front();

		m_ProcessorInfos.pop_front();

		bProcInfo	= TRUE;
	}

	m_Mutex.Unlock();


	// Process ����
	if (bProcInfo)
	{
		if (FALSE == RunProcess (&procInfo))
		{
			bResult		= FALSE;
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


	// ���μ��� ID �߰�
	m_ProcessorIDs.push_back(GetProcessId(pExecInfo->hProcess));


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


/************************************************************
 *	@brief		Is Process Run
 *	@retval		����
 ************************************************************/
BOOL CProcessor::IsProcessRun(LPCTSTR pszProcName)
{
	BOOL	bResult			= FALSE;

	HANDLE	hProcessSnap	= INVALID_HANDLE_VALUE;


	PROCESSENTRY32 pe32;

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    pe32.dwSize = sizeof(PROCESSENTRY32);

    Process32First(hProcessSnap, &pe32);


	while(Process32Next(hProcessSnap, &pe32))
    {
		if(!_tcsicmp(pszProcName, pe32.szExeFile))
		{
			bResult = TRUE;

			break;
		}
	}


	CloseHandle(hProcessSnap);


	return bResult;
}


/************************************************************
 *	@brief		Is Process Run
 *	@retval		����
 ************************************************************/
BOOL CProcessor::IsProcessRun(int nProcID)
{
	BOOL	bResult			= FALSE;

	HANDLE	hProcessSnap	= INVALID_HANDLE_VALUE;;


	PROCESSENTRY32 pe32;

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    pe32.dwSize = sizeof(PROCESSENTRY32);

    Process32First(hProcessSnap, &pe32);


	while(Process32Next(hProcessSnap, &pe32))
    {
		if(nProcID == pe32.th32ProcessID)
		{
			bResult = TRUE;

			break;
		}
	}


	CloseHandle(hProcessSnap);


	return bResult;
}


/************************************************************
 *	@brief		Kill Process
 *	@retval		����
 ************************************************************/
BOOL CProcessor::KillProcess(LPCTSTR pszProcName)
{
	HANDLE	hProcessSnap	= INVALID_HANDLE_VALUE;

	HANDLE	hProcess		= INVALID_HANDLE_VALUE;


	PROCESSENTRY32 pe32;

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    pe32.dwSize = sizeof(PROCESSENTRY32);

    Process32First(hProcessSnap, &pe32);


	while(Process32Next(hProcessSnap, &pe32))
    {
		if(!_tcsicmp(pszProcName, pe32.szExeFile))
		{
			hProcess = OpenProcess(PROCESS_TERMINATE, 0, pe32.th32ProcessID);

			if (NULL == hProcess)
			{
				return FALSE;
			}

			if (FALSE == TerminateProcess(hProcess, 0))
			{
				return FALSE;
			}
		}
	}


	CloseHandle(hProcessSnap);

	CloseHandle(hProcess);


	return TRUE;
}


/************************************************************
 *	@brief		Kill Process
 *	@retval		����
 ************************************************************/
BOOL CProcessor::KillProcess(int nProcID)
{
	HANDLE	hProcessSnap	= INVALID_HANDLE_VALUE;;

	HANDLE	hProcess		= INVALID_HANDLE_VALUE;;


	PROCESSENTRY32 pe32;

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	pe32.dwSize = sizeof(PROCESSENTRY32);

	Process32First(hProcessSnap, &pe32);


	while(Process32Next(hProcessSnap, &pe32))
	{
		if(nProcID == pe32.th32ProcessID)
		{       
			hProcess = OpenProcess(PROCESS_TERMINATE, 0, pe32.th32ProcessID);
           
			if (NULL == hProcess)
			{
				return FALSE;
			}

			if (FALSE == TerminateProcess(hProcess, 0))
			{
				return FALSE;
			}

			break;
		}
	}


	CloseHandle(hProcessSnap);

	CloseHandle(hProcess);


	return TRUE;
}

