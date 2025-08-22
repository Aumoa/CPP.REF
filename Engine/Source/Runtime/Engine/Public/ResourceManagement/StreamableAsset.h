// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "SerializableObject.h"
#include "StreamableAsset.gen.h"

namespace Ayla
{
	ACLASS()
	class ENGINE_API StreamableAsset : public SerializableObject
	{
		GENERATED_BODY()

	protected:
		StreamableAsset();

	public:
		virtual ~StreamableAsset() noexcept override;
	};
}