// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "HttpClient.gen.h"

SCLASS()
class NETWORK_API HttpClient : implements Object
{
	GENERATED_BODY()

public:
	HttpClient();
	virtual ~HttpClient() noexcept override;
};