// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GUIContent.gen.h"

namespace Ayla
{
	ACLASS()
	class RENDERCORE_API GUIContent : public Object
	{
		GENERATED_BODY()

	public:
		virtual void SetText(String text) = 0;
		virtual String GetText() = 0;
	};
}