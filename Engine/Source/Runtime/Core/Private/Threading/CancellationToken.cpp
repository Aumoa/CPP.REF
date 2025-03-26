// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Threading/CancellationToken.h"
#include "OperationCanceledException.h"

namespace Ayla
{
	[[noreturn]]
	void CancellationToken::ThrowOperationCanceledException()
	{
		throw OperationCanceledException();
	}
}