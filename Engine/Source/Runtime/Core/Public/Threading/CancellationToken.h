// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include <stop_token>
#include "System/StaticClass.h"
#include "System/OperationCanceledException.h"

class CORE_API CancellationToken : StaticClass
{
public:
	static void ThrowIfCancellationRequested(std::stop_token& Token)
	{
		if (Token.stop_requested())
		{
		}
	}

private:
	[[noreturn]] static void ThrowOperationCanceledException()
	{
		throw OperationCanceledException();
	}
};