// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#if PLATFORM_WINDOWS

#include "CoreMinimal.h"
#include "WindowsMinimal.h"
#include "IOCPAsyncOp.h"
#include "Threading/Tasks/TaskCompletionSource.h"

struct WinOverlapped : public OVERLAPPED
{
#if DO_CHECK
	std::atomic<bool> bCheckDup = false;
#endif

	struct WinOverlappedContainer* ContainerRef;
	EIOCPAsyncOp Op;
	TaskCompletionSource<size_t> Signal;

	void AddRef();
	void Release();
};

#endif