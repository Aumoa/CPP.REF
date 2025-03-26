// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Threading/Tasks/Task.h"
#include "IO/SeekOrigin.h"

namespace Ayla
{
	class CORE_API Stream
	{
	protected:
		Stream() = default;

	public:
		virtual ~Stream() noexcept = default;

		virtual void Flush() = 0;
		virtual void Close() = 0;

		virtual void Write(std::span<const uint8> InBytes) = 0;
		virtual Task<> WriteAsync(std::span<const uint8> InBytes, CancellationToken InCancellationToken = {}) = 0;
		virtual void Seek(int64 InSeekpos, SeekOrigin InOrigin = SeekOrigin::Begin) = 0;

		virtual size_t Read(std::span<uint8> OutBytes) = 0;
		virtual Task<size_t> ReadAsync(std::span<uint8> OutBytes, CancellationToken InCancellationToken = {}) = 0;
		virtual size_t GetLength() const = 0;
	};
}