/*! 
Mutex for the EpLibrary

The MIT License (MIT)

Copyright (c) 2008-2013 Woong Gyu La <juhgiyo@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#include "Mutex.h"



Mutex::Mutex(const TCHAR *mutexName, LPSECURITY_ATTRIBUTES lpsaAttributes) :BaseLock()
{
	m_isMutexAbandoned=false;
	m_lpsaAttributes=lpsaAttributes;
	m_mutex=CreateMutex(lpsaAttributes,FALSE,mutexName);
}

Mutex::Mutex(const Mutex& b)
{
	m_isMutexAbandoned=false;
	m_lpsaAttributes=b.m_lpsaAttributes;
	m_mutex=CreateMutex(m_lpsaAttributes,FALSE,NULL);
}

Mutex::~Mutex()
{

	CloseHandle(m_mutex);
}

bool Mutex::Lock()
{

	bool returnVal=true;
	unsigned long res=WaitForSingleObject(m_mutex,INFINITE);
	if(res==WAIT_ABANDONED)
	{
		m_isMutexAbandoned=true;
		returnVal=false;
	}
	return returnVal;
}

long Mutex::TryLock()
{
	long ret=0;
	unsigned long mutexStatus=WaitForSingleObject(m_mutex,0);
	if(mutexStatus== WAIT_OBJECT_0)
		ret=1;
	else if(mutexStatus==WAIT_ABANDONED)
		m_isMutexAbandoned=true;
	return ret;

}

long Mutex::TryLockFor(const unsigned int dwMilliSecond)
{
	long ret=0;
	unsigned long mutexStatus=WaitForSingleObject(m_mutex,dwMilliSecond);
	if(mutexStatus==WAIT_OBJECT_0)
		ret=1;
	else if(mutexStatus==WAIT_ABANDONED)
		m_isMutexAbandoned=true;
	return ret;

}
void Mutex::Unlock()
{
	ReleaseMutex(m_mutex);
}

bool Mutex::IsMutexAbandoned()
{
	return m_isMutexAbandoned;
}
