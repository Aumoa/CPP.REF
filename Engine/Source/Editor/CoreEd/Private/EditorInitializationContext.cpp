// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "EditorInitializationContext.h"
#include "GenericPlatform/GenericSplash.h"

namespace Ayla
{
	void EditorInitializationContext::Progress(String text)
	{
		GenericSplash::SetSplashText(text);
	}
}