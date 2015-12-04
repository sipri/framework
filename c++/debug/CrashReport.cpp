#include "StdAfx.h"
#include "CrashReport.h"
#include <Windows.h>
#include <DbgHelp.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define CRASH_REPORT_DLL	_T("CrashRpt1402.dll")


/************************************************************
 * Crash Report Flags for crInstall function.
 ************************************************************/
#define CR_INST_STRUCTURED_EXCEPTION_HANDLER      0x1 //!< Install SEH handler (deprecated name, use \ref CR_INST_SEH_EXCEPTION_HANDLER instead).
#define CR_INST_SEH_EXCEPTION_HANDLER             0x1 //!< Install SEH handler.
#define CR_INST_TERMINATE_HANDLER                 0x2 //!< Install terminate handler.
#define CR_INST_UNEXPECTED_HANDLER                0x4 //!< Install unexpected handler.
#define CR_INST_PURE_CALL_HANDLER                 0x8 //!< Install pure call handler (VS .NET and later).
#define CR_INST_NEW_OPERATOR_ERROR_HANDLER       0x10 //!< Install new operator error handler (VS .NET and later).
#define CR_INST_SECURITY_ERROR_HANDLER           0x20 //!< Install security error handler (VS .NET and later).
#define CR_INST_INVALID_PARAMETER_HANDLER        0x40 //!< Install invalid parameter handler (VS 2005 and later).
#define CR_INST_SIGABRT_HANDLER                  0x80 //!< Install SIGABRT signal handler.
#define CR_INST_SIGFPE_HANDLER                  0x100 //!< Install SIGFPE signal handler.   
#define CR_INST_SIGILL_HANDLER                  0x200 //!< Install SIGILL signal handler.  
#define CR_INST_SIGINT_HANDLER                  0x400 //!< Install SIGINT signal handler.  
#define CR_INST_SIGSEGV_HANDLER                 0x800 //!< Install SIGSEGV signal handler.
#define CR_INST_SIGTERM_HANDLER                0x1000 //!< Install SIGTERM signal handler.  
#define CR_INST_ALL_POSSIBLE_HANDLERS          0x1FFF //!< Install all possible exception handlers.
#define CR_INST_CRT_EXCEPTION_HANDLERS         0x1FFE //!< Install exception handlers for the linked CRT module.
#define CR_INST_NO_GUI                         0x2000 //!< Do not show GUI, send report silently (use for non-GUI apps only).
#define CR_INST_HTTP_BINARY_ENCODING           0x4000 //!< Deprecated, do not use.
#define CR_INST_DONT_SEND_REPORT               0x8000 //!< Don't send error report immediately, just save it locally.
#define CR_INST_APP_RESTART                   0x10000 //!< Restart the application on crash.
#define CR_INST_NO_MINIDUMP                   0x20000 //!< Do not include minidump file to crash report.
#define CR_INST_SEND_QUEUED_REPORTS           0x40000 //!< CrashRpt should send error reports that are waiting to be delivered.
#define CR_INST_STORE_ZIP_ARCHIVES            0x80000 //!< CrashRpt should store both uncompressed error report files and ZIP archives.
#define CR_INST_SEND_MANDATORY				 0x100000 //!< This flag removes the "Close" and "Other actions" buttons from Error Report dialog, thus making the sending procedure mandatory for user.
#define CR_INST_SHOW_ADDITIONAL_INFO_FIELDS	 0x200000 //!< Makes "Your E-mail" and "Describe what you were doing when the problem occurred" fields of Error Report dialog always visible.
#define CR_INST_ALLOW_ATTACH_MORE_FILES		 0x400000 //!< Adds an ability for user to attach more files to crash report by clicking "Attach More File(s)" item from context menu of Error Report Details dialog.
#define CR_INST_AUTO_THREAD_HANDLERS         0x800000 //!< If this flag is set, installs exception handlers for newly created threads automatically.


/************************************************************
 * Crash Report Flags for crAddScreenshot function.
 ************************************************************/
#define CR_AS_VIRTUAL_SCREEN  0  //!< Take a screenshot of the virtual screen.
#define CR_AS_MAIN_WINDOW     1  //!< Take a screenshot of application's main window.
#define CR_AS_PROCESS_WINDOWS 2  //!< Take a screenshot of all visible process windows.
#define CR_AS_GRAYSCALE_IMAGE 4  //!< Make a grayscale image instead of a full-color one.
#define CR_AS_USE_JPEG_FORMAT 8  //!< Store screenshots as JPG files.
#define CR_AS_ALLOW_DELETE   16  //!< If this flag is specified, the file will be deletable from context menu of Error Report Details dialog.


/************************************************************
 * Crash Report Exception Information
 ************************************************************/
typedef struct tagCR_EXCEPTION_INFO
{
	WORD cb;                   //!< Size of this structure in bytes; should be initialized before using.
	PEXCEPTION_POINTERS pexcptrs; //!< Exception pointers.
	int exctype;               //!< Exception type.
	DWORD code;                //!< Code of SEH exception.
	unsigned int fpe_subcode;  //!< Floating point exception subcode.
	const wchar_t* expression; //!< Assertion expression.
	const wchar_t* function;   //!< Function in which assertion happened.
	const wchar_t* file;       //!< File in which assertion happened.
	unsigned int line;         //!< Line number.
	BOOL bManual;              //!< Flag telling if the error report is generated manually or not.
	HANDLE hSenderProcess;     //!< Handle to the CrashSender.exe process.
}
CR_EXCEPTION_INFO;

typedef CR_EXCEPTION_INFO* PCR_EXCEPTION_INFO;


/************************************************************
 * Crash Report Callback Information (UNICODE)
 ************************************************************/
typedef struct tagCR_CRASH_CALLBACK_INFOW
{
	WORD cb;                            //!< Size of this structure in bytes.
	int nStage;                         //!< Stage.
	LPCWSTR pszErrorReportFolder;       //!< Directory where crash report files are located.
	CR_EXCEPTION_INFO* pExceptionInfo;  //!< Pointer to information about the crash.
	LPVOID pUserParam;                  //!< Pointer to user-defined data.
	BOOL bContinueExecution;            //!< Whether to terminate the process (the default) or to continue program execution.	
}
CR_CRASH_CALLBACK_INFOW;


/************************************************************
 * Crash Report Callback Information (MBCD)
 ************************************************************/
typedef struct tagCR_CRASH_CALLBACK_INFOA
{
	WORD cb;                            //!< Size of this structure in bytes.
	int nStage;                         //!< Stage.
	LPCSTR pszErrorReportFolder;        //!< Directory where crash report files are located.
	CR_EXCEPTION_INFO* pExceptionInfo;  //!< Pointer to information about the crash.
	LPVOID pUserParam;                  //!< Pointer to user-defined data.
	BOOL bContinueExecution;            //!< Whether to terminate the process (the default) or to continue program execution.	
}
CR_CRASH_CALLBACK_INFOA;


/************************************************************
 * Crash Report Callback Information
 ************************************************************/
#ifdef UNICODE
	typedef CR_CRASH_CALLBACK_INFOW CR_CRASH_CALLBACK_INFO;
#else
	typedef CR_CRASH_CALLBACK_INFOA CR_CRASH_CALLBACK_INFO;
#endif // UNICODE


typedef BOOL (CALLBACK *LPGETLOGFILE) (__reserved LPVOID lpvState);

typedef int (CALLBACK *PFNCRASHCALLBACKW) (CR_CRASH_CALLBACK_INFOW* pInfo);

typedef int (CALLBACK *PFNCRASHCALLBACKA) (CR_CRASH_CALLBACK_INFOA* pInfo);


/************************************************************
 * Crash Report Install Information (UNICODE)
 ************************************************************/
typedef struct tagCR_INSTALL_INFOW
{
	WORD cb;                        //!< Size of this structure in bytes; must be initialized before using!
	LPCWSTR pszAppName;             //!< Name of application.
	LPCWSTR pszAppVersion;          //!< Application version.
	LPCWSTR pszEmailTo;             //!< E-mail address of crash reports recipient.
	LPCWSTR pszEmailSubject;        //!< Subject of crash report e-mail. 
	LPCWSTR pszUrl;                 //!< URL of server-side script (used in HTTP connection).
	LPCWSTR pszCrashSenderPath;     //!< Directory name where CrashSender.exe is located.
	LPGETLOGFILE pfnCrashCallback;  //!< Deprecated, do not use.
	UINT uPriorities[5];            //!< Array of error sending transport priorities.
	DWORD dwFlags;                  //!< Flags.
	LPCWSTR pszPrivacyPolicyURL;    //!< URL of privacy policy agreement.
	LPCWSTR pszDebugHelpDLL;        //!< File name or folder of Debug help DLL.
	MINIDUMP_TYPE uMiniDumpType;    //!< Minidump type.
	LPCWSTR pszErrorReportSaveDir;  //!< Directory where to save error reports.
	LPCWSTR pszRestartCmdLine;      //!< Command line for application restart (without executable name).
	LPCWSTR pszLangFilePath;        //!< Path to the language file (including file name).
	LPCWSTR pszEmailText;           //!< Custom E-mail text (used when deliverying report as E-mail).
	LPCWSTR pszSmtpProxy;           //!< Network address and port to be used as SMTP proxy.
	LPCWSTR pszCustomSenderIcon;    //!< Custom icon used for Error Report dialog.
	LPCWSTR pszSmtpLogin;           //!< Login name used for SMTP authentication when sending error report as E-mail.
	LPCWSTR pszSmtpPassword;        //!< Password used for SMTP authentication when sending error report as E-mail.	
}
CR_INSTALL_INFOW;

typedef CR_INSTALL_INFOW* PCR_INSTALL_INFOW;


/************************************************************
 * Crash Report Install Information (MBCS)
 ************************************************************/
typedef struct tagCR_INSTALL_INFOA
{
	WORD cb;                       //!< Size of this structure in bytes; must be initialized before using!
	LPCSTR pszAppName;             //!< Name of application.
	LPCSTR pszAppVersion;          //!< Application version.
	LPCSTR pszEmailTo;             //!< E-mail address of crash reports recipient.
	LPCSTR pszEmailSubject;        //!< Subject of crash report e-mail. 
	LPCSTR pszUrl;                 //!< URL of server-side script (used in HTTP connection).
	LPCSTR pszCrashSenderPath;     //!< Directory name where CrashSender.exe is located.
	LPGETLOGFILE pfnCrashCallback; //!< Deprecated, do not use.
	UINT uPriorities[5];           //!< Array of error sending transport priorities.
	DWORD dwFlags;                 //!< Flags.
	LPCSTR pszPrivacyPolicyURL;    //!< URL of privacy policy agreement.
	LPCSTR pszDebugHelpDLL;        //!< File name or folder of Debug help DLL.
	MINIDUMP_TYPE uMiniDumpType;   //!< Mini dump type.
	LPCSTR pszErrorReportSaveDir;  //!< Directory where to save error reports.
	LPCSTR pszRestartCmdLine;      //!< Command line for application restart (without executable name).
	LPCSTR pszLangFilePath;        //!< Path to the language file (including file name).
	LPCSTR pszEmailText;           //!< Custom E-mail text (used when deliverying report as E-mail).
	LPCSTR pszSmtpProxy;           //!< Network address and port to be used as SMTP proxy.
	LPCSTR pszCustomSenderIcon;    //!< Custom icon used for Error Report dialog.
	LPCSTR pszSmtpLogin;           //!< Login name used for SMTP authentication when sending error report as E-mail.
	LPCSTR pszSmtpPassword;        //!< Password used for SMTP authentication when sending error report as E-mail.
	PFNCRASHCALLBACKA pfnCrashCallback2; //!< Crash callback function.
}
CR_INSTALL_INFOA;

typedef CR_INSTALL_INFOA* PCR_INSTALL_INFOA;


/************************************************************
 * Crash Report Install Information
 ************************************************************/
#ifdef UNICODE
	typedef CR_INSTALL_INFOW CR_INSTALL_INFO;
	typedef PCR_INSTALL_INFOW PCR_INSTALL_INFO;
#else
	typedef CR_INSTALL_INFOA CR_INSTALL_INFO;
	typedef PCR_INSTALL_INFOA PCR_INSTALL_INFO; 
#endif // UNICODE


/************************************************************
 * Crash Report Function
 ************************************************************/
typedef int (WINAPI * CRINSTALLW)(__in PCR_INSTALL_INFOW pInfo);

typedef int (WINAPI * CRINSTALLA)(__in PCR_INSTALL_INFOA pInfo);

typedef int (WINAPI * CRADDSCREENSHOT2)(DWORD dwFlags, int nJpegQuality);

typedef int (WINAPI * CRUNINSTALL)();

#ifdef UNICODE
	#define CRINSTALL CRINSTALLW
#else
	#define CRINSTALL CRINSTALLA
#endif //UNICODE


static CRINSTALL g_pfnCrInstall = NULL;

static CRADDSCREENSHOT2 g_pfnCrAddScreenshot2 = NULL;

static CRUNINSTALL g_pfnCrUnInstall = NULL;


/************************************************************
 *	@brief		Constructor
 *	@retval		None
 ************************************************************/
CCrashReport::CCrashReport(void)
{
	m_hCrashReport = LoadLibrary(CRASH_REPORT_DLL);

	if (m_hCrashReport)
	{
#ifdef UNICODE
		g_pfnCrInstall = (CRINSTALL)GetProcAddress(m_hCrashReport, "crInstallW");
#else
		g_pfnCrInstall = (CRINSTALL)GetProcAddress(m_hCrashReport, "crInstallA");
#endif // UNICODE

		g_pfnCrAddScreenshot2 = (CRADDSCREENSHOT2)GetProcAddress(m_hCrashReport, "crAddScreenshot2");

		g_pfnCrUnInstall = (CRUNINSTALL)GetProcAddress(m_hCrashReport, "crUninstall");
	}
}


/************************************************************
 *	@brief		Destructor
 *	@retval		None
 ************************************************************/
CCrashReport::~CCrashReport(void)
{
	if (m_hCrashReport)
	{
		FreeLibrary(m_hCrashReport);
	}
}


/************************************************************
 *	@brief		Install crash report
 *	@retval		None
 ************************************************************/
bool CCrashReport::Install(TCHAR* pszAppName, TCHAR* pszAppVersion, TCHAR* pszEmailSubject, TCHAR* pszEmailTo)
{
	// Check null point
	if (NULL == m_hCrashReport)
	{
		return false;
	}


	// Set parameter
	CR_INSTALL_INFO info;

	memset(&info, 0, sizeof(CR_INSTALL_INFO));

	info.cb = sizeof(CR_INSTALL_INFO);

	info.pszAppName = pszAppName;

	info.pszAppVersion = pszAppVersion;

	info.pszEmailSubject = pszEmailSubject;

	info.pszEmailTo = pszEmailTo;

	info.dwFlags |= CR_INST_ALL_POSSIBLE_HANDLERS;

	info.dwFlags |= CR_INST_ALLOW_ATTACH_MORE_FILES;


	// Take screenshot of the app window at the moment of crash
	g_pfnCrAddScreenshot2(CR_AS_PROCESS_WINDOWS|CR_AS_USE_JPEG_FORMAT, 95);


	// Install crash report
	return (!g_pfnCrInstall(&info));
}


/************************************************************
 *	@brief		Uninstall crash report
 *	@retval		None
 ************************************************************/
bool CCrashReport::Uninstall(void)
{
	// Check null point
	if (NULL == m_hCrashReport)
	{
		return false;
	}


	// Uninstall crash report
	return (!g_pfnCrUnInstall());
}
