/*! 
Semaphore for the EpLibrary

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
#include "Semaphore.h"


Semaphore::Semaphore(unsigned int count,const TCHAR *semName, LPSECURITY_ATTRIBUTES lpsaAttributes) :BaseLock()
{
	m_lpsaAttributes=lpsaAttributes;
	m_count=count;
	m_sem=CreateSemaphore(lpsaAttributes,count,count,semName);
}
Semaphore::Semaphore(const Semaphore& b) :BaseLock()
{
	m_lpsaAttributes=b.m_lpsaAttributes;
	m_count=b.m_count;
	m_sem=CreateSemaphore(m_lpsaAttributes,m_count,m_count,NULL);
}
Semaphore::~Semaphore()
{
	CloseHandle(m_sem);
}

bool Semaphore::Lock()
{
	WaitForSingleObject(m_sem,INFINITE);
	return true;
}

long Semaphore::TryLock()
{
	long ret=0;
	if(WaitForSingleObject(m_sem,0) == WAIT_OBJECT_0 )
		ret=1;
	return ret;
}
long Semaphore::TryLockFor(const unsigned int dwMilliSecond)
{
	long ret=0;
	if( WaitForSingleObject(m_sem,dwMilliSecond) == WAIT_OBJECT_0)
		ret=1;
	return ret;
}
void Semaphore::Unlock()
{
	ReleaseSemaphore(m_sem,1,NULL);
}

long Semaphore::Release(long count, long * retPreviousCount)
{
	return ReleaseSemaphore(m_sem,count,retPreviousCount);
}


