// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Graphics.gen.h"

namespace Ayla
{
	class Window;
	class GenericWindow;

	ACLASS()
	class RENDERCORE_API Graphics : public Object
	{
		GENERATED_BODY()

	protected:
		Graphics();

	public:
		virtual ~Graphics() noexcept override;

		virtual void Initialize() = 0;

		virtual RPtr<Window> ConfigureWindow(RPtr<GenericWindow> platformWindow) = 0;
	};
}