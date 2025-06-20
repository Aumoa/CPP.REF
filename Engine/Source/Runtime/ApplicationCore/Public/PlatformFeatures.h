// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "PlatformFeatures.gen.h"

namespace Ayla
{
	class GraphicsRenderFeature;

	ACLASS()
	class APPLICATIONCORE_API PlatformFeatures : public Object
	{
		GENERATED_BODY()

	protected:
		PlatformFeatures();

	public:
		virtual ~PlatformFeatures() noexcept override;

	public:
		virtual size_t NumSupportGraphicsRenderFeature() = 0;
		virtual RPtr<GraphicsRenderFeature> CreateGraphicsRenderFeature(size_t index) = 0;
	};
}