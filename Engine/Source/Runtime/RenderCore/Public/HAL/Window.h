// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Window.gen.h"

namespace Ayla
{
	ACLASS()
	class RENDERCORE_API Window : public Object
	{
		GENERATED_BODY()

	public:
		virtual void OnPreRender() = 0;
		virtual void OnGUI() = 0;
		virtual void Present() = 0;
	};
}