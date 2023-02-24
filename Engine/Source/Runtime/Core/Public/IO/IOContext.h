// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include <functional>
#include <atomic>
#include <mutex>
#include <memory>

class CORE_API IOContext
{
	IOContext(const IOContext&) = delete;

private:
	class _iocp_impl;

	std::unique_ptr<_iocp_impl> _impl;
	std::atomic<size_t> _workers = 0;

public:
	IOContext();
	~IOContext() noexcept;

	size_t Poll();
	size_t PollOne();

	size_t Run();
	void Stop();

	void BindHandle(void* nativeHandle);
	bool Post(std::function<void(size_t, int32)> work);
};