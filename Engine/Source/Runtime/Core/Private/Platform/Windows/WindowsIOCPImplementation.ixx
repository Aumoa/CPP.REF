// Copyright 2020-2023 Aumoa.lib. All right reserved.

module;

#include "Platform/Windows/WindowsCore.h"

export module Core.Platform.Windows:WindowsIOCPImplementation;

export import Core.System;
export import Core.IO;

#if PLATFORM_WINDOWS

export class WindowsIOCPImplementation
{
private:
	HANDLE hIOCP = NULL;

public:
	WindowsIOCPImplementation(size_t ThreadsHint = -1);
	~WindowsIOCPImplementation() noexcept;

	bool poll_one();
	void bind_handle(void* Handle);
	void post(std::function<void(IOCompletionOverlapped*, size_t, int32)> InWork);
};

#endif