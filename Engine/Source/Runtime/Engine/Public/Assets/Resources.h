// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class StreamableAsset;

class ENGINE_API Resources : public StaticClass
{
public:
	static Task<std::shared_ptr<StreamableAsset>> LoadAsync(String InResourcePath, std::stop_token InCancellationToken = {});
};