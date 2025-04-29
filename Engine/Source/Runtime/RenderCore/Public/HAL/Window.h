// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Numerics/VectorInterface/Vector.h"
#include "Window.gen.h"

namespace Ayla
{
	class GUI;

	ACLASS()
	class RENDERCORE_API Window : public Object
	{
		GENERATED_BODY()

	public:
		virtual Vector2F GetSize() = 0;

		virtual RPtr<GUI> BeginGUI() = 0;
		virtual void EndGUI() = 0;
	};
}