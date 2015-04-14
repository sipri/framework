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
	// 생성자
	CProcessor (void);

	// 생성자
	CProcessor (CString strCmd, CString strParam = _T(""), CWnd* pNotify = NULL, UINT nMsgID = 0);

	// 소멸자
	virtual ~CProcessor (void);


	// 명령 설정
	void SetCmd (CString strCmd);

	// 파라메터 설정
	void SetParam (CString strParam);

	// 종료메세지 통지할 윈도우 설정
	void SetNotifyWnd (CWnd* pNotify);

	// 종료메세지 ID 설정
	void SetNotifyMsg (UINT nMsgID);


	// Process 동기 실행
	BOOL Run (void);

	// Process 비동기 실행
	BOOL RunAsync (void);

	// Process 실행 중 체크
	BOOL IsRunning (void);

	// Stop Process Thread
	BOOL StopThread (void);


	// Is Process Run
	static BOOL IsProcessRun(LPCTSTR pszProcName);

	// Is Process Run
	static BOOL IsProcessRun(int nProcID);


	// Kill Process
	static BOOL KillProcess(LPCTSTR pszProcName);

	// Kill Process
	static BOOL KillProcess(int nProcID);


protected:
	// Command
	CString		m_strCmd;

	// Parameter
	CString		m_strParam;

	// 종료 메세지 통지 윈도우
	CWnd*		m_pNotify;

	// 종료 메세지 ID
	UINT		m_nMsgID;

	// Processor 실행 중
	BOOL		m_bIsRunning;

	// Processor Infos
	std::list<PROCESSOR_INFO>	m_ProcessorInfos;

	// Thread Handle
	std::list<HANDLE>			m_ThreadHandles;

	// Processor IDs
	std::list<int>				m_ProcessorIDs;

	// Mutex
	CMutex		m_Mutex;


	// Process 실행
	static DWORD WINAPI RunProcess (LPVOID pProcess);

	// Process 실행
	BOOL RunProcess (void);

	// Process 실행
	BOOL RunProcess (PROCESSOR_INFO* pProcInfo);
};

