// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "IO/IOCompletionOverlapped.h"
#include "Misc/PlatformMisc.h"

IOCompletionOverlapped::IOCompletionOverlapped(std::function<void(size_t, int32)> work)
	: _pad{}
	, _ptr(this)
	, _work(std::move(work))
{
}

IOCompletionOverlapped* IOCompletionOverlapped::FromOverlapped(void* overlapped) noexcept
{
	uint8* pad = reinterpret_cast<uint8*>(overlapped);
	auto* block = reinterpret_cast<IOCompletionOverlapped**>(pad + OverlapDefaultPaddingSize);
	return *block;
}