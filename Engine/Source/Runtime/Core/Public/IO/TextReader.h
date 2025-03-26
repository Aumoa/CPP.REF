// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "String_.h"
#include "Threading/Tasks/Task.h"
#include <stop_token>

namespace Ayla
{
	class TextReader
	{
	protected:
		TextReader() = default;

	public:
		virtual ~TextReader() noexcept = default;

		virtual String ReadLine() = 0;
		virtual Task<String> ReadLineAsync(CancellationToken InCancellationToken = {}) = 0;
		virtual Task<String> ReadToEndAsync(CancellationToken InCancellationToken = {}) = 0;
		virtual bool IsEOF() const = 0;
	};
}