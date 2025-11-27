// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "NamedObject.h"
#include "StreamableRenderAsset.gen.h"

namespace Ayla
{
	class Graphics;

	ACLASS()
	class ENGINE_API StreamableRenderAsset : public NamedObject
	{
		GENERATED_BODY()

	private:
		SharedPtr<Graphics> m_Graphics;

	protected:
		StreamableRenderAsset(SharedPtr<Graphics> graphics);

	public:
		virtual ~StreamableRenderAsset() noexcept override;

	protected:
		AFUNCTION()
		SharedPtr<Graphics> GetGraphics();
	};
}