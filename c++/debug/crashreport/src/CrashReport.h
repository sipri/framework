#pragma once

/************************************************************
 *
 * Crash Report
 *
 ************************************************************/
class CCrashReport
{
public:
	// Constructor
	CCrashReport(void);

	// Destructor
	~CCrashReport(void);


	// Install crash report
	bool Install(TCHAR* pszAppName, TCHAR* pszAppVersion, TCHAR* pszEmailSubject, TCHAR* pszEmailTo);

	// Uninstall crash report
	bool Uninstall(void);

protected:
	// Crash Report Handle
	HMODULE m_hCrashReport;
};
