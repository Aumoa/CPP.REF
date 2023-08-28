// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "System/String.h"
#include "Threading/Tasks/Task.h"
#include <stop_token>

class TextReader
{
protected:
	TextReader() = default;

public:
	virtual ~TextReader() noexcept = default;

	virtual String ReadLine() = 0;
	virtual Task<String> ReadLineAsync(std::stop_token InCancellationToken = {}) = 0;
	virtual Task<String> ReadToEndAsync(std::stop_token InCancellationToken = {}) = 0;
	virtual bool IsEOF() const = 0;
};