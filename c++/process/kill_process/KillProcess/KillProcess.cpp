#include "stdafx.h"
#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <io.h>
#include <fcntl.h>

#ifdef UNICODE
#define GetLastErrorMessage GetLastErrorMessageW
#else
#define GetLastErrorMessage GetLastErrorMessageA
#endif

/************************************************************
*	@brief		Get last error message
*	@param[in]	dwLastError		Last error
*	@retval		string			Last error message
************************************************************/
std::string GetLastErrorMessageA(DWORD dwLastError)
{
	LPSTR pszLastErrorMsg = NULL;
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, dwLastError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&pszLastErrorMsg, 0, NULL);

	std::string strLastErrorMsg(pszLastErrorMsg, size);

	LocalFree(pszLastErrorMsg);

	return strLastErrorMsg;
}

/************************************************************
*	@brief		Get last error message
*	@param[in]	dwLastError		Last error
*	@retval		wstring			Last error message
************************************************************/
std::wstring GetLastErrorMessageW(DWORD dwLastError)
{
	LPWSTR pszLastErrorMsg = NULL;
	size_t size = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, dwLastError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&pszLastErrorMsg, 0, NULL);

	std::wstring strLastErrorMsg(pszLastErrorMsg, size);

	LocalFree(pszLastErrorMsg);

	return strLastErrorMsg;
}

/************************************************************
 *	@brief		Kill Process
 *	@param[in]	pszProcName		Name of process
 *	@retval		TRUE			Success
 *	@retval		FALSE			Fail
 ************************************************************/
BOOL KillProcess(LPCTSTR pszProcName)
{
	HANDLE	hProcessSnap = INVALID_HANDLE_VALUE;
	HANDLE	hProcess = INVALID_HANDLE_VALUE;

	PROCESSENTRY32 pe32;
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	pe32.dwSize = sizeof(PROCESSENTRY32);
	Process32First(hProcessSnap, &pe32);

	while (Process32Next(hProcessSnap, &pe32))
	{
		if (!_tcsicmp(pszProcName, pe32.szExeFile))
		{
			hProcess = OpenProcess(PROCESS_TERMINATE, 0, pe32.th32ProcessID);
			if (NULL == hProcess)
			{
				_tprintf(_T("Failed to OpenProcess: %s"), GetLastErrorMessage(GetLastError()).c_str());
				return FALSE;
			}

			if (FALSE == TerminateProcess(hProcess, 0))
			{
				_tprintf(_T("Failed to TerminateProcess, %s"), GetLastErrorMessage(GetLastError()).c_str());
				return FALSE;
			}

			DWORD dwResult = WaitForSingleObject(hProcess, INFINITE);
			switch (dwResult)
			{
			case WAIT_ABANDONED:
				_tprintf(_T("Failed to WaitForSingleObject: WAIT_ABANDONED"));
				return FALSE;
			case WAIT_TIMEOUT:
				_tprintf(_T("Failed to WaitForSingleObject: WAIT_TIMEOUT"));
				return FALSE;
			case WAIT_FAILED:
				_tprintf(_T("Failed to WaitForSingleObject: WAIT_FAILED"));
				return FALSE;
			}
		}
	}

	CloseHandle(hProcessSnap);
	CloseHandle(hProcess);

	return TRUE;
}

/************************************************************
 *	@brief		Main
 *	@param[in]	argc			Count of argument
 *	@param[in]	argv			Arguments
 *	@retval		Nothing
 ************************************************************/
int _tmain(int argc, TCHAR* argv[])
{
	_setmode(_fileno(stdout), _O_WTEXT);

	if (argc > 1)
	{
		for (int nCnt = 1; nCnt < argc; nCnt++)
			KillProcess(argv[nCnt]);
	}

    return 0;
}
