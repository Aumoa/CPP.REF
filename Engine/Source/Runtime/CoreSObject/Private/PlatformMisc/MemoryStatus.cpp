// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "PlatformMisc/MemoryStatus.h"
#include "CoreAssert.h"

MemoryStatus::MemoryStatus(
	size_t TotalPhysicalMemory,
	size_t UsedPhysicalMemory,
	size_t ProcessUsedMemory
)
	: TotalPhysicalMemory(TotalPhysicalMemory)
	, UsedPhysicalMemory(UsedPhysicalMemory)
	, ProcessUsedMemory(ProcessUsedMemory)
{
}

#if PLATFORM_WINDOWS

#include <Windows.h>
#include <Psapi.h>
#undef GetObject

MemoryStatus MemoryStatus::Capture()
{
	MEMORYSTATUSEX StatusEx;
	StatusEx.dwLength = sizeof(MEMORYSTATUSEX);
	BOOL bStatus = GlobalMemoryStatusEx(&StatusEx);
	checkf(bStatus, L"Internal windows error. ErrorCode: {}", GetLastError());

	PROCESS_MEMORY_COUNTERS ProcessMem;
	bStatus = GetProcessMemoryInfo(GetCurrentProcess(), &ProcessMem, sizeof(ProcessMem));
	checkf(bStatus, L"Internal windows error. ErrorCode: {}", GetLastError());

	return MemoryStatus(
		(size_t)StatusEx.ullTotalPhys,
		(size_t)(StatusEx.ullTotalPhys - StatusEx.ullAvailPhys),
		(size_t)ProcessMem.WorkingSetSize
	);
}

#endif