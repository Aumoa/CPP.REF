// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "EditorEngine.h"
#include "EditorInitializationContext.h"
#include "GenericPlatform/GenericSplash.h"
#include "HAL/PlatformRenderFeature.h"

namespace Ayla
{
	EditorEngine::EditorEngine()
	{
	}

	RPtr<InitializationContext> EditorEngine::PreInitialize()
	{
		GenericSplash::Show();
		return New<EditorInitializationContext>();
	}

	void EditorEngine::Initialize(RPtr<PlatformRenderFeature> prf, RPtr<InitializationContext> context)
	{
		Super::Initialize(prf, context);
		GenericSplash::Hide();
	}
}