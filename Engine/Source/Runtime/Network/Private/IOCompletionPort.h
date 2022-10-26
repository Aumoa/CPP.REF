// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IOCompletionPort.gen.h"

class ImplSocket;

SCLASS()
class IOCompletionPort : implements Object
{
	GENERATED_BODY()

public:
	IOCompletionPort();
	virtual ~IOCompletionPort() noexcept;

	void ChainSocket(ImplSocket* socket);

	static IOCompletionPort* Get();

protected:
	virtual void HandleChainSocket(ImplSocket* impl) = 0;
};