#include "Hooks.h"

#include <cstdio>
#include <Windows.h>

static const int overwriteSize = 6;

// http://www.codeproject.com/Articles/30140/API-Hooking-with-MS-Detours
struct Hook
{
	BYTE original[overwriteSize];
	BYTE replacement[overwriteSize];
	void* origFunc;
	DWORD oldProtect;

	void Install(void* origFunc, void* newFunc)
	{
		this->origFunc = origFunc;
		DWORD distance = ((DWORD)newFunc - (DWORD)origFunc - 5);

		replacement[0] = 0xE9;	// JMP
		memcpy(&replacement[1], &distance, 4);
		replacement[5] = 0xC3;	// RET

		VirtualProtect(origFunc, overwriteSize, PAGE_EXECUTE_READWRITE, &oldProtect);
		memcpy(original, origFunc, overwriteSize);
		
		Patch();
	}

	__inline void Patch()
	{
		memcpy(origFunc, replacement, overwriteSize);
		VirtualProtect(origFunc, overwriteSize, oldProtect, NULL);
	}

	__inline void Unpatch()
	{
		VirtualProtect(origFunc, overwriteSize, PAGE_EXECUTE_READWRITE, NULL);
		memcpy(origFunc, original, overwriteSize);
	}
};

Hook mallocHook;
static void* CustomMalloc(size_t size)
{
	mallocHook.Unpatch();
	printf("malloc:%d\n", size);
	void* rv = malloc(size);
	printf("malloc returning:%x\n", rv);
	mallocHook.Patch();
	return rv;
}

Hook freeHook;
static void CustomFree(void* ptr)
{
	freeHook.Unpatch();
	printf("free:%x\n", ptr);
	free(ptr);
	freeHook.Patch();
}

extern "C" _CRTIMP void __cdecl _free_dbg(void* pUserData, int nBlockUse);
Hook free_dbgHook;
static void CustomFree_dbg(void* ptr, int b)
{
	free_dbgHook.Unpatch();
	printf("free_dbg:%x\n", ptr);
	_free_dbg(ptr, b);
	free_dbgHook.Patch();
}

Hook reallocHook;
static void* CustomRealloc(void* ptr, size_t newSize)
{
	reallocHook.Unpatch();
	printf("realloc:%x -> %d\n", ptr, newSize);
	void* rv = realloc(ptr, newSize);
	reallocHook.Patch();
	return rv;
}

void Hooks::Install()
{
	mallocHook.Install(&malloc, &CustomMalloc);
	freeHook.Install(&free, &CustomFree);
	free_dbgHook.Install(&_free_dbg, &CustomFree_dbg);
	reallocHook.Install(&realloc, &CustomRealloc);
}

void Hooks::Uninstall()
{
	mallocHook.Unpatch();
	freeHook.Unpatch();
	free_dbgHook.Unpatch();
	reallocHook.Unpatch();
}

