// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Rendering/InputElementDescription.h"

namespace Ayla
{
	class PLATFORM_NOVTABLE VertexFactory
	{
	public:
		virtual std::vector<InputElementDescription> GetInputElements() const = 0;
	};
}