// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Engine.h"
#include "InitializationContext.h"

namespace Ayla
{
	Engine::Engine()
	{
	}

	RPtr<InitializationContext> Engine::PreInitialize()
	{
		return New<InitializationContext>();
	}

	void Engine::Initialize(RPtr<InitializationContext> context)
	{
		context->Progress(TEXT("Initializing engine..."));
		std::this_thread::sleep_for(3s);
	}
}