// Copyright 2020-2022 Aumoa.lib. All right reserved.

import Core;

IOCompletionOverlapped::IOCompletionOverlapped(std::function<void(size_t, int32)> InWork)
	: Padding{}
	, Pointer(this)
	, Work(std::move(InWork))
{
}

IOCompletionOverlapped* IOCompletionOverlapped::FromOverlapped(void* Overlapped) noexcept
{
	uint8* PadPtr = reinterpret_cast<uint8*>(Overlapped);
	auto* Block = reinterpret_cast<IOCompletionOverlapped**>(PadPtr + OverlapDefaultPaddingSize);
	return *Block;
}