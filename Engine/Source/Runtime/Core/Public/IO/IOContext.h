// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include <functional>
#include <atomic>

class CORE_API IOContext
{
	IOContext(const IOContext&) = delete;

private:
	size_t _iocp = 0;
	std::atomic<bool> _running = true;
	std::atomic<size_t> _workers = 0;

public:
	IOContext();
	~IOContext() noexcept;

	size_t Poll();
	size_t PollOne();

	size_t Run();
	void Stop();

	void BindHandle(void* nativeHandle);
	bool Post(std::function<void()> work);
};