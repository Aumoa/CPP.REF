// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreTypes/String.h"
#include "Threading/Tasks/Task.h"

class CORE_API TextReader
{
public:
	virtual ~TextReader() noexcept;

	virtual String ReadLine() = 0;
	virtual Task<String> ReadLineAsync(std::stop_token cancellationToken) = 0;
	virtual bool IsEOF() = 0;
};