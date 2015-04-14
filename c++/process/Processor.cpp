#include "stdafx.h"

#include <TlHelp32.h>

#include "Processor.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/************************************************************
 *	@brief		생성자
 *	@retval		없음
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
 *	@brief		생성자
 *	@param[in]	strCmd		Command
 *	@param[in]	pNotify			통지 윈도우
 *	@param[in]	nMsgID			통지 메세지
 *	@retval		없음
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
 *	@brief		소멸자
 *	@retval		없음
 ************************************************************/
CProcessor::~CProcessor (void)
{
	StopThread();
}


/************************************************************
 *	@brief		명령 설정
 *	@retval		없음
 ************************************************************/
void CProcessor::SetCmd (CString strCmd)
{
	m_strCmd		= strCmd;
}


/************************************************************
 *	@brief		파라메터 설정
 *	@retval		없음
 ************************************************************/
void CProcessor::SetParam (CString strParam)
{
	m_strParam		= strParam;
}


/************************************************************
 *	@brief		종료메세지 통지할 윈도우 설정
 *	@retval		없음
 ************************************************************/
void CProcessor::SetNotifyWnd (CWnd* pNotify)
{
	m_pNotify		= pNotify;
}


/************************************************************
 *	@brief		종료메세지 ID 설정
 *	@retval		없음
 ************************************************************/
void CProcessor::SetNotifyMsg (UINT nMsgID)
{
	m_nMsgID		= nMsgID;
}


/************************************************************
 *	@brief		Process 동기 실행
 *	@retval		TRUE			성공
 *	@retval		FALSE			실패
 ************************************************************/
BOOL CProcessor::Run (void)
{
	// Processor Information 설정
	PROCESSOR_INFO			procInfo;
	
	procInfo.m_strCmd		= m_strCmd;

	procInfo.m_strParam		= m_strParam;

	procInfo.m_pNotify		= m_pNotify;

	procInfo.m_nMsgID		= m_nMsgID;


	// Processor 실행
	return RunProcess (&procInfo);
}


/************************************************************
 *	@brief		Process 비동기 실행
 *	@retval		TRUE			성공
 *	@retval		FALSE			실패
 ************************************************************/
BOOL CProcessor::RunAsync (void)
{
	m_Mutex.Lock();


	// Processor Information 추가
	PROCESSOR_INFO			procInfo;
	
	procInfo.m_strCmd		= m_strCmd;

	procInfo.m_strParam		= m_strParam;

	procInfo.m_pNotify		= m_pNotify;

	procInfo.m_nMsgID		= m_nMsgID;

	m_ProcessorInfos.push_back (procInfo);


	// Thread 생성
	HANDLE hThread = ::CreateThread(NULL, 0,(LPTHREAD_START_ROUTINE)RunProcess, (LPVOID)this, CREATE_SUSPENDED, NULL);

	if (NULL == hThread)
	{
		// 프로세스 종료 메세지 발생
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
 *	@brief		Process 실행 중 체크
 *	@retval		TRUE			실행중
 *	@retval		FALSE			비실행중
 ************************************************************/
BOOL CProcessor::IsRunning (void)
{
	return m_bIsRunning;
}


/************************************************************
 *	@brief		Stop Process Thread
 *	@retval		TRUE			성공
 *	@retval		FALSE			실패
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
 *	@brief		Process 실행
 *	@param[in]	pProcess		CProcessor클래스
 *	@retval		0				성공
 ************************************************************/
DWORD WINAPI CProcessor::RunProcess (LPVOID pProcess)
{
	((CProcessor*)pProcess)->RunProcess();

	return 0;
}


/************************************************************
 *	@brief		Process 실행
 *	@retval		TRUE			성공
 *	@retval		FALSE			실패
 ************************************************************/
BOOL CProcessor::RunProcess (void)
{
	BOOL bResult	= TRUE;


	// Processor Information 취득
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


	// Process 실행
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
 *	@brief		Process 실행
 *	@retval		TRUE			성공
 *	@retval		FALSE			실패
 ************************************************************/
BOOL CProcessor::RunProcess (PROCESSOR_INFO* pProcInfo)
{
	CString		strCmd		= pProcInfo->m_strCmd;

	CString		strParam	= pProcInfo->m_strParam;

	CWnd*		pNotify		= pProcInfo->m_pNotify;

	UINT		nMsgID		= pProcInfo->m_nMsgID;


	// FILE 체크
	CFileFind Find;

	if (FALSE == Find.FindFile (strCmd))
	{
		// 프로세스 종료 메세지 발생
		if (NULL != pNotify)
		{
			pNotify->PostMessage (nMsgID);
		}

		return FALSE;
	}

	// FILE 체크 해제
	Find.Close();


	// 디렉토리와 파일명을 구분
	int idx = strCmd.ReverseFind(TEXT('\\'));

	CString strDir		= strCmd.Mid (0, idx);

	CString strFile		= strCmd.Mid (idx + 1);


	// 프로세스 실행 정보 설정
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


	// 프로세스 실행 중 설정
	m_bIsRunning = TRUE;


	// 프로세스 실행
	if (FALSE == ShellExecuteEx(pExecInfo))
	{
		// 프로세스 핸들 닫기
		CloseHandle(pExecInfo->hProcess);

		// 프로세스 실행 정보 해제
		delete pExecInfo;


		// 프로세스 종료 메세지 발생
		if (NULL != pNotify)
		{
			pNotify->PostMessage (nMsgID);
		}


		return FALSE;
	}


	// 프로세스 ID 추가
	m_ProcessorIDs.push_back(GetProcessId(pExecInfo->hProcess));


	// 프로세스 종료 대기
	WaitForSingleObject(pExecInfo->hProcess, INFINITE);

	// 프로세스 핸들 닫기
	CloseHandle(pExecInfo->hProcess);

	// 프로세스 실행 정보 해제
	delete pExecInfo;


	// 프로세스 종료 메세지 발생
	if (NULL != pNotify)
	{
		pNotify->PostMessage (nMsgID);
	}


	// 프로세스 실행 중 해제
	m_bIsRunning = FALSE;


	return TRUE;
}


/************************************************************
 *	@brief		Is Process Run
 *	@retval		없음
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
 *	@retval		없음
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
 *	@retval		없음
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
 *	@retval		없음
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

