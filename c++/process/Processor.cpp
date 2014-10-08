#include "stdafx.h"

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


	// Thread Handle
	m_hThread			= NULL;
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
	// Processor Information 설정
	PROCESSOR_INFO			procInfo;
	
	procInfo.m_strCmd		= m_strCmd;

	procInfo.m_strParam		= m_strParam;

	procInfo.m_pNotify		= m_pNotify;

	procInfo.m_nMsgID		= m_nMsgID;


	// Processor Information 추가
	m_Mutex.Lock();

	m_ProcessorInfos.push_back (procInfo);

	m_Mutex.Unlock();


	// Thread 생성
	m_Mutex.Lock();

	if (NULL == m_hThread)
	{
		m_hThread = ::CreateThread(NULL, 0,(LPTHREAD_START_ROUTINE)RunProcess, (LPVOID)this, CREATE_SUSPENDED, NULL);

		if (NULL == m_hThread)
		{
			// 프로세스 종료 메세지 발생
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


	// Process 실행
	m_RunEvent.SetEvent ();


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
	m_StopEvent.SetEvent ();

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

	// 이벤트 리스트
	CSyncObject* EventList[2];

	EventList[0]	= &m_RunEvent;

	EventList[1]	= &m_StopEvent;

	// 이벤트 대기 리스트 작성
	CMultiLock multiLock (&EventList[0], _countof (EventList));

	for ( ; ; )
	{
		// 이벤트 대기
		DWORD dwResult = multiLock.Lock(INFINITE, FALSE);

		// Run Event
		if (WAIT_OBJECT_0 == dwResult)
		{
			for ( ; ; )
			{
				// Processor Information 취득
				PROCESSOR_INFO*		pProcInfo		= NULL;

				m_Mutex.Lock();

				if (m_ProcessorInfos.size())
				{
					pProcInfo	= &m_ProcessorInfos.front();
				}

				m_Mutex.Unlock();


				// Processor Information 없음
				if (NULL == pProcInfo)
				{
					break;
				}


				// Process 실행
				if (FALSE == RunProcess (pProcInfo))
				{
					bResult		= FALSE;
				}


				// Processor Information 삭제
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

