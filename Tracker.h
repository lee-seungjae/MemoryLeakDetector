#pragma once

struct Tracker
{
	static void Allocated(void* ptr, size_t size);
	static void Deallocated(void* ptr);
};