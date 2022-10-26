// Copyright 2020-2022 Aumoa.lib. All right reserved.

#if PLATFORM_WINDOWS

#include "WinOverlappedContainer.h"

WinOverlappedContainer::WinOverlappedContainer() noexcept
{
	size_t i = 0;
	for (auto& overlap : Overlaps)
	{
		overlap.ContainerRef = this;
		overlap.Op = (EIOCPAsyncOp)(i++);
	}
}

WinOverlappedContainer::~WinOverlappedContainer() noexcept
{
	check(Ref == 0);
}

void WinOverlappedContainer::AddRef() noexcept
{
	++Ref;
}

void WinOverlappedContainer::Release() noexcept
{
	if (--Ref == 0)
	{
		delete this;
	}
}

WinOverlapped* WinOverlappedContainer::GetRef(EIOCPAsyncOp op) noexcept
{
	WinOverlapped* ref = Overlaps + (size_t)op;
	static_cast<OVERLAPPED&>(*ref) = {};
	ref->Signal = TaskCompletionSource<>::Create<size_t>();
	ref->AddRef();
	return ref;
}

#endif