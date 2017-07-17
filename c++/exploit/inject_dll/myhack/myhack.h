// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MYHACK_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MYHACK_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef MYHACK_EXPORTS
#define MYHACK_API __declspec(dllexport)
#else
#define MYHACK_API __declspec(dllimport)
#endif

// This class is exported from the myhack.dll
class MYHACK_API Cmyhack {
public:
	Cmyhack(void);
	// TODO: add your methods here.
};

extern MYHACK_API int nmyhack;

MYHACK_API int fnmyhack(void);
