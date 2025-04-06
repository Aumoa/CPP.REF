// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "EditorEngine.h"
#include "EditorInitializationContext.h"
#include "GenericPlatform/GenericSplash.h"

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

	void EditorEngine::Initialize(RPtr<InitializationContext> context)
	{
		Super::Initialize(context);
		GenericSplash::Hide();
	}
}