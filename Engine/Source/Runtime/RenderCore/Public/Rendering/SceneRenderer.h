// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SceneRenderer.gen.h"

namespace Ayla
{
	class SceneView;

	ACLASS()
	class RENDERCORE_API SceneRenderer : public Object
	{
		GENERATED_BODY()

	protected:
		ACONSTRUCTOR()
		SceneRenderer();

	public:
		AFUNCTION()
		virtual void Render(SharedPtr<SceneView> view) APURE;
	};
}