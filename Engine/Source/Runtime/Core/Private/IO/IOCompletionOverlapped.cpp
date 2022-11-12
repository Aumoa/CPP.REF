// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "IO/IOCompletionOverlapped.h"
#include "Misc/PlatformMisc.h"

IOCompletionOverlapped::IOCompletionOverlapped()
	: _pad{}
	, _ptr(this)
{
}

IOCompletionOverlapped::~IOCompletionOverlapped() noexcept
{
}

void* IOCompletionOverlapped::ToOverlapped() const noexcept
{
	return const_cast<uint8*>(_pad + 0);
}

IOCompletionOverlapped* IOCompletionOverlapped::FromOverlapped(void* overlapped) noexcept
{
	uint8* pad = reinterpret_cast<uint8*>(overlapped);
	auto* block = reinterpret_cast<IOCompletionOverlapped**>(pad + PlatformMisc::IOOverlappedDefaultSize());
	return *block;
}