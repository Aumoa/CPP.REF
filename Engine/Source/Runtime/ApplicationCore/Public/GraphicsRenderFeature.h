// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GraphicsRenderFeature.gen.h"

namespace Ayla
{
	ACLASS()
	class APPLICATIONCORE_API GraphicsRenderFeature : public Object
	{
		GENERATED_BODY()

	protected:
		GraphicsRenderFeature();

	public:
		virtual ~GraphicsRenderFeature() noexcept override;
	};
}