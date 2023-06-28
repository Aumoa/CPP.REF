// Copyright 2020-2022 Aumoa.lib. All right reserved.

#if PLATFORM_WINDOWS

import Core;
import Core.Platform.Windows;

using Implementation = WindowsIOCPImplementation;

#endif

IOContext::IOContext()
	: Impl(new Implementation())
{
}

IOContext::~IOContext() noexcept
{
	if (Impl)
	{
		delete (Implementation*)Impl;
		Impl = nullptr;
	}
}

size_t IOContext::Poll()
{
	size_t Count = 0;
	while (PollOne())
	{
		++Count;
	}
	return Count;
}

size_t IOContext::PollOne()
{
	if (Impl)
	{
		return ((Implementation*)Impl)->poll_one() ? 1 : 0;
	}
	else
	{
		return 0;
	}
}

size_t IOContext::Run()
{
	if (Impl == nullptr)
	{
		return 0;
	}

	size_t Count = 0;
	++Workers;

	while (Impl)
	{
		Count += ((Implementation*)Impl)->poll_one();
	}

	--Workers;
	return Count;
}

void IOContext::Stop()
{
	size_t LoadWorkers = Workers;
	for (size_t i = 0; i < LoadWorkers; ++i)
	{
		((Implementation*)Impl)->post([this](size_t, int32) { --Workers; });
	}

	while (Workers)
	{
		for (size_t i = 0; i < 1024; ++i)
		{
			PlatformMisc::YieldProcessor();
		}
	}

	Poll();

	delete (Implementation*)Impl;
	Impl = nullptr;
}

void IOContext::BindHandle(void* NativeHandle)
{
	((Implementation*)Impl)->bind_handle(NativeHandle);
}

bool IOContext::Post(std::function<void(size_t, int32)> InWork)
{
	if (Impl)
	{
		((Implementation*)Impl)->post(std::move(InWork));
		return true;
	}
	return false;
}