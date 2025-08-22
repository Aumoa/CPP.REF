// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Resources.gen.h"

namespace Ayla
{
	class StreamableAsset;

	ACLASS()
	class ENGINE_API Resources : public Object
	{
		GENERATED_BODY()

	public:
		AFUNCTION()
		static RPtr<StreamableAsset> Load(String filePath);
	};
}