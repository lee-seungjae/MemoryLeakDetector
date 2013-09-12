#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <cstdlib>
#include <vector>

#include "LeakDetector.h"

int _tmain(int argc, _TCHAR* argv[])
{
	LeakDetector::InstallHooks();

	printf("=== malloc and free ===\n");
	{
		void* p1 = std::malloc(573);
		void* p2 = ::malloc(69);
		std::free(p1);
		::free(p2);
	}

	printf("=== new and delete ===\n");
	{
		int* a = new int();
		*a = 5;
		delete a;

		int* b = new int();
		*b = 6;
		delete b;
	}

	printf("=== new[] and delete[] ===\n");
	{
		int* a = new int[10];
		delete[] a;
	}

	printf("=== std::vector ===\n");
	{
		std::vector<int> p3;
		p3.resize(25);
	}

	LeakDetector::UninstallHooks();

	return 0;
}

