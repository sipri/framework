#include "stdafx.h"
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <Windows.h>

void hello(void)
{
	__asm {
		mov edi, edi
	}
	puts("hello");
}

void goodbye(void)
{
	static int x;
	printf("goodbye %d\n", x++);
}

void hotpatch(void *target, void *replacement)
{
//	assert(((uintptr_t)target & 0x07) == 0);
	void *page = (void *)((uintptr_t)target & ~0xfff);
	DWORD dwNewProtect = PAGE_EXECUTE_READWRITE;
	DWORD dwOldProtect;
	VirtualProtect(page, 4096, dwNewProtect, &dwOldProtect);
	uint32_t rel = (char *)replacement - (char *)target - 5;
	union {
		uint8_t bytes[8];
		uint64_t value;
	} instruction = { { 0xe9, rel >> 0, rel >> 8, rel >> 16, rel >> 24 } };
	*(uint64_t *)target = instruction.value;
	VirtualProtect(page, 4096, dwOldProtect, &dwNewProtect);
}

DWORD WINAPI worker(LPVOID lpThreadParameter)
{
	for (;;) {
		hello();
		Sleep(1000);
	}
	return NULL;
}

int main(void)
{
	HANDLE hThread = CreateThread(NULL, 0, worker, NULL, 0, NULL);
	getchar();
	hotpatch(hello, goodbye);
	WaitForSingleObject(hThread, INFINITE);
	return 0;
}
