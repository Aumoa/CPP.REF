// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Rendering/StreamableRenderAsset.h"
#include "Mesh.gen.h"

namespace Ayla
{
	ACLASS()
	class ENGINE_API Mesh : public StreamableRenderAsset
	{
		GENERATED_BODY()

	protected:
		Mesh(SharedPtr<Graphics> graphics);

	public:
		virtual ~Mesh() noexcept override;
	};
}