#include <stdio.h>
#include <stdarg.h>
#include <tchar.h>
#include "simple_log.h"


/************************************************************
 *	@brief		LogA
 *	@param[in]	format			Format
 *	@param[in]	...				Variable-argument
 *	@retval		None
 ************************************************************/
void Log(LPCTSTR format, ...)
{
   va_list args;

   va_start(args, format);

#pragma warning(disable:4996)
   int size = _sntprintf(NULL, 0, format, args);
#pragma warning(default:4996)

   TCHAR* buff = new TCHAR[size+1];

   _vsntprintf_s(buff, size+1, _TRUNCATE, format, args);

   OutputDebugString(buff);

   delete [] buff;
}
