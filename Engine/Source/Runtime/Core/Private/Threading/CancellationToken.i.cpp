// Copyright 2020-2024 Aumoa.lib. All right reserved.

import Core;

void CancellationToken::ThrowOperationCanceledException()
{
	throw OperationCanceledException();
}