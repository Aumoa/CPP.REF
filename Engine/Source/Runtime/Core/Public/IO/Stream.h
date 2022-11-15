// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Threading/Tasks/Task.h"

class CORE_API Stream
{
public:
	Stream();
	virtual ~Stream() noexcept;

	virtual void Flush() = 0;
	virtual void Close() = 0;
	virtual void Write(std::span<const uint8> bytes) = 0;
	virtual Task<> WriteAsync(std::span<const uint8> bytes, std::stop_token cancellationToken = {}) = 0;
	virtual size_t Read(std::span<uint8> bytes) = 0;
	virtual Task<size_t> ReadAsync(std::span<uint8> bytes, std::stop_token cancellationToken = {}) = 0;
};