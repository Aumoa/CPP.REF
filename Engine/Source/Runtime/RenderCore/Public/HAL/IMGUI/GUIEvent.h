// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

namespace Ayla
{
	struct GUIEvent
	{
		enum class Types
		{
			Repaint
		};

		const Types Type;

	public:
		static constexpr GUIEvent Repaint()
		{
			return GUIEvent{ .Type = Types::Repaint };
		}
	};
}